/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

#include "stdafx.h"


#include "Defs.h"
#include "NodelistCreator.h"
#include "MeshcodeUtility.h"

// useful_headers
#include <TDC/useful_headers/str_util.h>
#include <TDC/useful_headers/directory_util.h>

// ArcHelperEx,WinLib
#include <AheLayerFunctions.h>
#include <GlobalFunctions.h>

// boost
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assign.hpp>
#include "boost/range/algorithm/find_if.hpp"
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/operator.hpp>

using namespace sindy;
using namespace schema;
using namespace uh;


static crd_cnv gCrdCnv;

// 初期化
bool CNodelistCreator::init( const CString& strDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg )
{
	return init( strDBConnect, strDBConnect, strOutputPath, strLatLon, strErrMsg );
}


// 初期化(道路ノードの接続先DBをイラストリンクとは別に指定)
bool CNodelistCreator::init( const CString& strDBConnect, const CString& strRoadDBConnect, const CString& strOutputPath, const CString& strLatLon, CString& strErrMsg )
{
	// テーブルファインダ設定
	//   ILLUST_LINK
	CWorkspace ipWorkspace;
	if( ! connectSeveralTimes( strDBConnect, 3, ipWorkspace )) {
		strErrMsg = str_util::format( _T("接続先DBが存在しないか、開けません : %s"), strDBConnect );
		return false;
	}

	ITablePtr ipIllustTable = ipWorkspace.OpenSameOwnersTable( illust_link::kTableName );
	if( ! ipIllustTable ) {
		strErrMsg = str_util::format( _T("フィーチャークラスが存在しないため処理を中断します: %s"), illust_link::kTableName );
		return false;
	}
	m_ipTableFinder.InitCollection( ipIllustTable , CModel() );


	// テーブルファインダ設定
	//   ROAD_NODE
	if( ! connectSeveralTimes( strRoadDBConnect, 3, ipWorkspace ) )
	{
		strErrMsg = str_util::format( _T("接続先DBが存在しないか、開けません : %s"), strRoadDBConnect );
		return false;
	}

	ITablePtr ipRoadNodeTable = ipWorkspace.OpenSameOwnersTable( road_node::kTableName );
	if( ! ipRoadNodeTable )
	{
		strErrMsg = str_util::format( _T("フィーチャークラスが存在しないため処理を中断します: %s"), road_node::kTableName );
		return false;
	}
	m_ipTableFinder.InitCollection( ipRoadNodeTable , CModel() );


	// ノードリスト出力先フォルダパス
	m_strOutputPath = strOutputPath;

	// 緯度経度出力設定
	m_LatLon = strLatLon;

	return true;
}


// ノードリスト作成
bool CNodelistCreator::create( const CIllustOutputInfo::OutputInfo info, const std::vector<CString>& rule, CString& strErrMsg )
{
	// ILLUST_LINK へ接続
	const auto& table = CAST_ILLUSTLINKTABLE( m_ipTableFinder.FindTable( illust_link::kTableName )); 
	if( ! table )
		return false;

	// イラスト種別と仕向け先で検索
	CString strWhereClause = str_util::format( _T("%s=%d AND %s=%d"), 
												illust_link::kIllustClass,  info.mIllustClass,
												illust_link::kCustomerCode, info.mCustomerClass 
												);
	table->_Select( AheInitQueryFilter( nullptr, nullptr, strWhereClause ), true );
	table->CreateCache();

	// 出力ファイルストリーム生成
	std::ofstream ofs = getOutputFileStream( info.mOutputFileName.GetString() );
	if( ofs.fail() ){
		strErrMsg += _T("出力ファイル作成に失敗しました");
		return false;
	}


	// ノードリスト出力
	std::map<long, CSPIllustLinkRow> records; // [bug11680]出力レコードをOID順にソート

	// [bug11679]トンネルサフィックス付与フラグを引数に追加
	CIllustNameCreator cCreator( info.mPrefix.GetString(), info.mSuffix.GetString(), rule, info.mTunnelSuffix );
	for( const auto& row : *table )
	{
		const auto& illust_row = CAST_ILLUSTLINKROW( row );

		// ノードリスト出力除外対象ならば出力しない
		if( CIllustOutputInfo::IsException( illust_row, info ))
			continue;

		records[ illust_row->GetOID() ] = illust_row;
	}

	for( const auto& rec : records )
	{
		// 1レコード出力分のコンテナ
		std::vector<CString> OutRec;

		// 設定ファイルの指定フィールドの値を出力
		if( ! getFieldValue( cCreator, rec.second, info.mOutputField, OutRec ) )
		{
			strErrMsg += uh::str_util::format( _T("設定ファイルに存在しないフィールド名が指定されています: 種別: %d, 仕向け先: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}
		
		// 緯度経度出力
		if( ! getLatLon( rec.second, OutRec ) ) {
			strErrMsg += uh::str_util::format( _T("緯度経度の出力に失敗しました: 種別: %d, 仕向け先: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}

		// リンク列を出力
		if( ! getLQ( rec.second, info.mOutputStartNode, info.mOutputEndNode, OutRec ) )
		{
			strErrMsg += uh::str_util::format( _T("リンク列の出力に失敗しました: 種別: %d, 仕向け先: %d ( Line %d )")
									, info.mIllustClass, info.mCustomerClass, info.mLineNum );
			ofs.close();
			return false;
		}

		// 1レコード出力
		ofs << CT2A( uh::str_util::join( OutRec, _T("\t") )) << std::endl;
	}

	table->DeleteCache();
	table->clear();

	ofs.close();
	return true;
}


// 指定回数だけDBへの接続をトライする
bool CNodelistCreator::connectSeveralTimes( const CString& strDBConnect, int nTryCount, sindy::CWorkspace& cWorkspace )
{
	for( int i = 0; i < nTryCount; ++i )
	{
		if(  cWorkspace.Connect( strDBConnect.GetString() ) )
			return true;
		// 3秒後リトライ
		Sleep(3000);
	}
	return false;
}


// 設定ファイル記載の指定フィールドの値を取得
bool CNodelistCreator::getFieldValue( sindy::CIllustNameCreator cNameCreator, sindy::CSPIllustLinkRow spRow , const std::vector<CString>& OutputFields, std::vector<CString>& OutString )
{
	for( const auto& field : OutputFields )
	{
		// イラスト名取得
		if( field == setting::ILLUSTNAME )
		{
			CString strIllustName = cNameCreator.GetIllustName( spRow );
			if( strIllustName.IsEmpty() )
				return false;

			OutString.push_back( strIllustName ); 
			continue;
		}

		// 存在しないフィールド名が指定されていたらエラー
		if( spRow->FindField( field ) == -1 )
			return false;

		// フィールド値を取得
		OutString.push_back( convFieldValue( spRow, field ));
	}

	return true;
}


// フィールドの値を変換して返す
CString CNodelistCreator::convFieldValue(sindy::CSPIllustLinkRow spRow, const CString& strFieldName)
{
	using namespace illust_link;
	using namespace illust_class;
	using namespace branch_dir_code;
	using namespace uh;

	long lIllustClass  = spRow->GetValue( kIllustClass ).lVal;			// イラスト種別
	long lStraightCode = spRow->GetValue( kStraightCode ).lVal;			// 直進方向コード

	if(  strFieldName.Compare( kBranchDirectionCode ) == 0 )
	{
		long lCode = spRow->GetValue( strFieldName ).lVal;

		switch ( lIllustClass )
		{
		case kGrade:
			if( lStraightCode == straight_code::kStraight )
			{
				if( lCode == kLeft ) return _T("SL");
				if( lCode == kRight) return _T("SR");

			}
			else
			{
				if( lCode == kLeft ) return _T("L");
				if( lCode == kRight) return _T("R");
			}
			break;

		default:
			break;
		}
	}

	return str_util::ToString( spRow->GetValue( strFieldName ));
}


// 設定ファイル記載の指定フィールドの値をファイルに出力する
bool CNodelistCreator::getLatLon(sindy::CSPIllustLinkRow spRow, std::vector<CString>& OutString)
{
	if( m_LatLon != latlon::DEG && m_LatLon != latlon::DMS )
		return true;

	// ILLUST_LINK へ接続
	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName ); 
	if( ! table )
		return false;	

	// ノード2のID取得
	long lNodeId = spRow->GetValue( illust_link::kNode2 ).lVal;

	// ノードの座標取得
	WKSPoint latlon = getLatLonByNodeId( lNodeId );
	if( latlon.X == -1 && latlon.Y != -1 )
		return false;
	
	// 座標値を出力形式に変換
	CString strX, strY;
	if( m_LatLon == latlon::DMS) 
	{
		// 度分秒に変換
		const dms sX = *gCrdCnv.DECtoDMS( latlon.X );
		const dms sY = *gCrdCnv.DECtoDMS( latlon.Y );

		strX = uh::str_util::format( _T("%d.%d.%2.2f"), sX.deg, sX.min, sX.sec );
		strY = uh::str_util::format( _T("%d.%d.%2.2f"), sY.deg, sY.min, sY.sec );
	}
	else if( m_LatLon == latlon::DEG )
	{
		// 桁数固定
		strX = uh::str_util::format( _T("%3.7f"), latlon.X );
		strY = uh::str_util::format( _T("%3.7f"), latlon.Y );
	}
	
	// 座標値を取得
	OutString.push_back( strY );
	OutString.push_back( strX );

	return true;
}


// リンク列を出力する
bool CNodelistCreator::getLQ(sindy::CSPIllustLinkRow spRow, int nStartNode, int nEndNode, std::vector<CString>& OutString)
{
	// 出力ノードの末尾を取得
	int nEndNodeNum =  ( nEndNode == 0 ? getLastEnableNodeField( spRow ) : nEndNode );

	// 各ノードのノードIDとメッシュコードの対応を取得
	std::vector<NodeInfo> NodeInfos;
	for( int i = nStartNode; i <= nEndNodeNum; ++i )
	{
		NodeInfo info;
		// ノードID
		long id = spRow->GetValue( illust_link::kNode[i] ).lVal;
		if( id == 0 )
			return false;
		info.id = id;
		// メッシュコード
		std::vector<int> meshcodes = getMeshcodes( id );
		if( meshcodes.empty() )
			return false;
		for( const auto mesh : meshcodes )
			info.addMesh( mesh );

		NodeInfos.push_back(info);
	}

	// 先頭ノードの1つ前のノードが属するメッシュコードを取得
	// (2次メッシュ上のノードの出力順考慮に使用)
	int nPreMesh = getPreRoadNodeMeshcode( NodeInfos, NodeInfos.begin() );
	if( nPreMesh == -1 )
		return false;


	// ノードリストに出力するメッシュ&ノードIDを順に格納
	for( auto iter = NodeInfos.begin(); iter != NodeInfos.end(); ++iter )
	{
		// 2次メッシュ上ではない場合
		if( ! iter->isOn2ndMesh() )
		{
			OutString.push_back( str_util::ToString( *iter->meshes.begin() ));
			OutString.push_back( str_util::ToString( iter->id ));
			nPreMesh = *iter->meshes.begin();
			continue;
		}

		// 2次メッシュ上の場合
		using boost::phoenix::arg_names::arg1;
		int nCurrentMesh = *std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != nPreMesh );

		//  2次メッシュ上の場合、跨いだ両側のメッシュ2つ分のノード情報を出力(出力順は考慮する)
		//  ただし、先頭/末尾ノードの場合は1つだけ出力
		if( iter != NodeInfos.begin() ){
			OutString.push_back( str_util::ToString( nPreMesh ));
			OutString.push_back( str_util::ToString( iter->id ));
		}
		if( iter+1 != NodeInfos.end() ){
			OutString.push_back( str_util::ToString( nCurrentMesh ));
			OutString.push_back( str_util::ToString( iter->id ));
		}

		nPreMesh = nCurrentMesh;
	}

	return true;
}


// メッシュコード群を取得する
std::vector<int> CNodelistCreator::getMeshcodes( long lNodeId )
{
	WKSPoint lonlat = getLatLonByNodeId( lNodeId );
	if( lonlat.X == -1 || lonlat.Y == -1)
		return std::vector<int>(); 

	return CMeshcodeUtility::GetBelong2ndMeshcode( lonlat );
}


// ノードIDから実数緯度経度を取得する
WKSPoint CNodelistCreator::getLatLonByNodeId(long lNodeId)
{
	WKSPoint err_point = { -1, -1 };

	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName );
	if( ! table )
		return err_point;

	// ノード取得
	table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
	const auto& rows = table->finds_by_key( road_node::kObjectID, lNodeId );
	if( rows.empty() )
		return err_point;

	const auto& row = *CAST_ROWC( **rows.begin() );
	IPointPtr ipPoint = row.CRowBase::GetShapeCopy();
	if( ! ipPoint )
		return err_point;

	// 座標取得
	double dX = 0.0, dY = 0.0;
	ipPoint->QueryCoords( &dX, &dY );

	WKSPoint lonlat = { dX, dY };
	return lonlat;
}


// ILLUST_LINKのリンク列(ノードID群)の1つ前のノードが属するメッシュコードを取得する
int CNodelistCreator::getPreRoadNodeMeshcode( std::vector<CNodelistCreator::NodeInfo>& infos, std::vector<CNodelistCreator::NodeInfo>::iterator iter )
{
	// メッシュ境界上のノードでなければ、先頭ノードのメッシュコードをそのまま取得
	if( ! iter->isOn2ndMesh() )
		return *iter->meshes.begin();


	// 2次メッシュ上のノードならば、次のノードを見に行く
	auto next_iter = iter + 1;
	if( next_iter == infos.end() )
		return -1;

	// 次のノードが2次メッシュ上のノードでないならば、
	// 次のノードのメッシュコードではない方のメッシュコードを返す
	if( ! next_iter->isOn2ndMesh() )
	{
		using boost::phoenix::arg_names::arg1;
		auto mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != *next_iter->meshes.begin() );
		if( mesh_iter == iter->meshes.end() )
			return -1;

		return *mesh_iter;
	}

	// 同一メッシュ境界上ではない
	if( ! iter->isEqualMesh( *next_iter ))
	{
		int mesh = iter->getCommonMesh( *next_iter );

		using boost::phoenix::arg_names::arg1;
		auto mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != mesh );
		if( mesh_iter == iter->meshes.end() )
			return -1;
	}

	// 同一メッシュ境界上のノードが続く場合を考慮
	int result_mesh = getPreRoadNodeMeshcode( infos, next_iter );
	if( result_mesh == -1 )
		return - 1;

	using boost::phoenix::arg_names::arg1;
	auto result_mesh_iter = std::find_if( iter->meshes.begin(), iter->meshes.end(), arg1 != result_mesh );
	if( result_mesh_iter == iter->meshes.end() )
		return -1;

	return *result_mesh_iter;
}



// ILLUST_LINKのリンク列(ノードID群)からIDが格納されている末尾のインデックスを取得する
int CNodelistCreator::getLastEnableNodeField( sindy::CSPIllustLinkRow spRow )
{
	for( int i = 1; i <= illust_link::kMaxNodeNumber; ++i )
	{
		if( spRow->GetValue( illust_link::kNode[i] ).vt == VT_NULL )
			return i - 1;
	}

	return -1;
}


// ノードが2次メッシュ上にあるかを判定する 
bool CNodelistCreator::isOn2ndMesh(long lNodeId)
{
	const auto& table = m_ipTableFinder.FindTable( road_node::kTableName );
	if( ! table )
		return false;

	 // ノード取得
	table->SetAutoCache( sindyContainerAutoOnDemandCacheNoDetach );
	const auto& rows = table->finds_by_key( road_node::kObjectID, lNodeId );
	if( rows.empty() )
		return false;

	const auto& row = *CAST_ROWC( **rows.begin() );
	long lNodeClass = row.GetValue( road_node::kNodeClass ).lVal;

	// ノード種別で判断する
	if( lNodeClass == road_node::node_class::kEdge || lNodeClass == road_node::node_class::kPecEdge )
		return true;

	return false;
}


// ノードリスト出力ストリームを取得する
std::ofstream CNodelistCreator::getOutputFileStream(const CString& strFilePath)
{
	// 出力先ファイルパス作成
	CString strOutputFilePath = m_strOutputPath + _T("\\") + strFilePath;

	// 出力ファイルオープン( 既に存在するなら追記モード )
	std::ofstream ofs;
	if( uh::isFile( strOutputFilePath.GetString() )) {
		ofs.open( strOutputFilePath.GetString(), std::ios::out | std::ios::app );
	}else {
		ofs.open( strOutputFilePath.GetString(), std::ios::out );
	}

	return ofs;
}
