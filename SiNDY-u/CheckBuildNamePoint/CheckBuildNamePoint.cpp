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
#include "CheckBuildNamePoint.h"

#include <sindy/workspace.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include <TDC/useful_headers/str_util.h>
#include <TDC/common_check/NameYomiChk.h>
#include "lonlat.h"
#include "NameChk.h"

using namespace std;
using namespace uh;
using namespace sindy;
using namespace sindy::schema::sj::buildingname_point;

namespace
{
	crd_cnv g_crd;
	using namespace sindy::schema::sj;

	// ここで定義したフィールドしかチェックできない（SUB FIELDで絞られる）ので注意

	const vector<CString> blNameFields = boost::assign::list_of
		( buildingname_point::kObjectID )
		( buildingname_point::kShape )
		( buildingname_point::kBuildingOID )
		( buildingname_point::kName )
		( buildingname_point::kPriority )
		( buildingname_point::kMatchPer )
		( buildingname_point::kYomiSeparated )
		( buildingname_point::kNameSeparated )
		( buildingname_point::kBldgClass1 )
		( buildingname_point::kBldgClass2 )
		( buildingname_point::kBldgClass3 )
		( buildingname_point::kFloors )
		( buildingname_point::kFixClass );

	using namespace sindy::schema;
	const vector<CString> buildingFields = boost::assign::list_of
		(building::kObjectID)
		(ipc_feature::kShape)
		(building::kBldClass);

	const vector<CString> cAnnoFields = boost::assign::list_of
		(city_annotation::kObjectID)
		(ipc_feature::kShape)
		(city_annotation::kNameString1);


	const CString nStr = _T("");

	const int nameMaxLength = 80;
}

// 初期化
bool CCheckBuildNamePoint::init()
{
	// 各種レイヤオープン
	if( ! OpenLayers( m_strBlNameDB, m_strBuildingDB, m_strAnnoDB, m_strMeshDB ) )
		return false;

	// メッシュリストオープン
	if( ! util::loadLongSet( m_strMeshList, m_meshList ) ){
		m_log.OutputStdErr( err_def::OpenMeshFail, m_strMeshList );
		return false;
	}

	// NG建物種別リストオープン
	if( ! util::loadLongSet( m_strNgBuild, m_ngBuildClass ) ){
		m_log.OutputStdErr( err_def::OpenNgBlCFail, m_strNgBuild );
		return false;
	}

	// NG文字列リストオープン
	if( ! util::loadStringList( m_strErrWord, m_errWordVec ) ){
		m_log.OutputStdErr( err_def::OpenNgWordFail, m_strErrWord );
		return false;
	}
	if( ! util::loadStringList( m_strWarnWord, m_warnWordVec ) ){
		m_log.OutputStdErr( err_def::OpenNgWordFail, m_strWarnWord );
		return false;
	}

	// 文字列比較用置換リストオープン
	if( ! util::loadPairVec( m_strReplaceWord, m_replaceWordVec ) ){
		m_log.OutputStdErr( err_def::OpenReplaceFail, m_strReplaceWord );
		return false;
	}

	// 注記比較用結合文字列リストオープン
	if( ! util::loadVecMap( m_strJoinAnno, m_joinAnnoMap ) ){
		m_log.OutputStdErr( err_def::OpenJoinAnnoFail, m_strJoinAnno );
		return false;
	}

	// 建物種別別OKワードリストオープン
	if( ! util::loadStrKeyMap( m_strBldcOKWord, m_bldcOKWordMap ) ){
		m_log.OutputStdErr( err_def::OpenBldcOKWordFail, m_strBldcOKWord );
		return false;
	}

	// 建物種別別NGワードリストオープン
	if( ! util::loadVecMap( m_strBldcNGWord, m_bldcNGWordMap ) ){
		m_log.OutputStdErr( err_def::OpenBldcNGWordFail, m_strBldcNGWord );
		return false;
	}
	// 分割数チェック時除外記号リストオープン
	if (!m_strExclude_mark.IsEmpty() && !util::loadStringSet(m_strExclude_mark, m_ExcludeMarkVec)) {
		m_log.OutputStdErr(err_def::OpenExcludMarkFail, m_strExclude_mark);
		return false;
	}

	// ログファイルオープン
	if( ! m_log.Open( m_strOutput ) ){
		m_log.OutputStdErr( err_def::OpenLogFail, m_strOutput );
		return false;
	}
	return true;
}

// 与えたメッシュ隣接9メッシュリストを作成する
void CCheckBuildNamePoint::Get9Mesh( long mesh, std::set<long>& meshlist )
{
	meshlist.insert( mesh );                              // 中心
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  1 ) ); // 左上
	meshlist.insert( g_crd.GetMeshCode( mesh,  0,  1 ) ); // 上
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  1 ) ); // 右上
	meshlist.insert( g_crd.GetMeshCode( mesh, -1,  0 ) ); // 左
	meshlist.insert( g_crd.GetMeshCode( mesh,  1,  0 ) ); // 右
	meshlist.insert( g_crd.GetMeshCode( mesh, -1, -1 ) ); // 左下
	meshlist.insert( g_crd.GetMeshCode( mesh,  0, -1 ) ); // 下
	meshlist.insert( g_crd.GetMeshCode( mesh,  1, -1 ) ); // 右下
};

// メッシュリストから、結合したメッシュポリゴンを取得
bool CCheckBuildNamePoint::GetMeshPolygons( long mesh, const set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo )
{
	ISpatialReferencePtr ipSpRef;
	CString strWhere;
	strWhere.Format( _T("%s in ( "), basemesh::kMeshCode );
	int i = 0;
	for( auto& mesh : meshlist ){
		if( i != 0 )
			strWhere.AppendFormat( _T(", ") );
		strWhere.AppendFormat( _T("%ld"), mesh );
		++i;
	}
	strWhere.AppendFormat( _T(" )") );

	IFeatureCursorPtr ipMeshCursor = m_ipCityMeshFC->_Search( AheInitQueryFilter( NULL, NULL, strWhere ), VARIANT_FALSE );
	if( !ipMeshCursor )
		return false;

	i = 0;
	IFeaturePtr ipMeshFeature;
	IGeometryCollectionPtr ipGeoCol( CLSID_GeometryBag );
	while( S_OK == ipMeshCursor->NextFeature( &ipMeshFeature ) && ipMeshFeature ){

		IGeometryPtr ipTempMeshGeo = ipMeshFeature->GetShapeCopy();
		if( !ipTempMeshGeo )
			return false;

		// 空間参照も取得しておく
		if( i == 0 ){
			ipTempMeshGeo->get_SpatialReference( &ipSpRef );
			((IGeometryPtr)ipGeoCol)->putref_SpatialReference( ipSpRef );
		}

		// 自身のメッシュを取得しておく
		if( mesh == (ipMeshFeature->GetValue( m_cityMeshFM.GetFieldIndex( basemesh::kMeshCode ) )).lVal ){
			ipMeshGeo = ipTempMeshGeo;

			// メッシュの中心座標から投影情報を取得する
			WKSPoint meshCenter = {};
			g_crd.MeshtoLL( mesh, 500000, 500000, &(meshCenter.X), &(meshCenter.Y));
			int sysNum = g_crd.Sys_Number(meshCenter.X, meshCenter.Y);

			// 投影座標系の情報を設定する
			// sysNumは1～19の19座標系の番号を意味する。
			// 投影情報の番号はesriSRProjCS_TokyoJapan1～19まであるので、
			// 第1系の値にsysNum-1を足せば投影情報のコードが取得できる
			// 19座標系のコードが連続している前提に基づくが、この値はEPSGコード。
			// これはESRIが定義した値ではないし、そうそう変わるものでもないので問題ないだろう(割り当てたのはESRIだけど…)
			ISpatialReferenceFactory2Ptr ipSpRefFct(CLSID_SpatialReferenceEnvironment);
			ipSpRefFct->CreateProjectedCoordinateSystem(esriSRProjCS_TokyoJapan1 + sysNum -1, &m_ipPrjCoordinateSystem);
		}

		// 後で一括でConstructUnionするために、ジオメトリを格納する
		ipGeoCol->AddGeometry( ipTempMeshGeo );

		++i;
	}

	if( ! ipMeshGeo )
		return false;

	// 詰めたものをConstructUnion
	IGeometryPtr ipTempUnionMeshGeo( CLSID_Polygon );
	IEnumGeometryPtr ipEnumGeo( ipGeoCol );
	if( FAILED(((ITopologicalOperatorPtr)ipTempUnionMeshGeo)->ConstructUnion( ipEnumGeo ) ))
		return false;

	ipTempUnionMeshGeo->putref_SpatialReference( ipSpRef );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->put_IsKnownSimple( VARIANT_FALSE );
	((ITopologicalOperator2Ptr)ipTempUnionMeshGeo)->Simplify();

	ipUnionMeshGeo = ipTempUnionMeshGeo;

	return true;
}

// 与えられたメッシュ形状内のオブジェクトを取得
bool CCheckBuildNamePoint::GetObjInMesh( const IGeometryPtr& ipMeshGeo, const vector<CString>& fields, const IFeatureClassPtr& ipFeatureClass, const CFieldMap& fieldMap, map<long, GeoClass>& geoMap )
{
	// サブフィールド指定
	CString subField = str_util::join( fields, _T(",") );

	// ジオメトリを保持するため、VARIANT_FALSE
	IFeatureCursorPtr ipCursor = ipFeatureClass->_Search( AheInitQueryFilter( AheInitSpatialFilter( NULL, ipMeshGeo ), subField, NULL ), VARIANT_FALSE );
	if( !ipCursor )
		return false;

	IFeaturePtr ipFeature;

	// 取得したオブジェクトのループ
	while( S_OK == ipCursor->NextFeature( &ipFeature ) && ipFeature ){

		// ObjectIDと形状取得
		GeoClass obj;

		long oID     = ipFeature->GetOID();
		if( ! GetFieldValues( ipFeature, fields, fieldMap, obj ) )
			return false;

		// ObjectIDをキーに、mapに格納
		geoMap[oID]  = obj;
	}

	return true;
}

// フィールド名分の値を取得し、オブジェクトコンテナに詰める
bool CCheckBuildNamePoint::GetFieldValues( const IFeaturePtr& ipFeature, const vector<CString>& fields, const CFieldMap& fieldMap, GeoClass& obj )
{
	for( const auto& field : fields ){
		if( 0 == field.CompareNoCase( sindy::schema::ipc_feature::kShape ) )
			obj.SetGeometry( ipFeature->GetShape() );
		else
			obj.SetFieldValue( field, str_util::ToString( ipFeature->GetValue( fieldMap.GetFieldIndex( field ) )));
	}
	return true;
}

// R-Treeを生成する
void CCheckBuildNamePoint::createRTree( const map<long, GeoClass>& geoMap, BGRTree& rTree )
{
	for(const auto& geo : geoMap){
		long  oID = geo.first;
		BGBox box = geo.second.GetBox();
		rTree.insert( make_pair( box, static_cast<unsigned int>(oID) ) );
	}
}

// R-Treeから、与えた箱の範囲に引っかかるID群を取得
set<long> CCheckBuildNamePoint::getIntersectsIDs( const BGBox& box, const BGRTree& rTree )
{
	set<long> retIDs;
	vector<pair<BGBox, unsigned int>> results;
	bgi::query( rTree, bgi::intersects( box ), std::back_inserter(results) );

	for( const auto& result : results )
		retIDs.insert( static_cast<long>( result.second ) );

	return retIDs;
}

// ログヘッダ出力
void CCheckBuildNamePoint::OutputLogHeader()
{
	m_log.OutputHeader();
	m_log.OutputLog( err_def::StrBlNameDB,    m_strBlNameDB );
	m_log.OutputLog( err_def::StrBuildingDB,  m_strBuildingDB );
	m_log.OutputLog( err_def::StrAnnoDB,      m_strAnnoDB );
	m_log.OutputLog( err_def::StrMeshDB,      m_strMeshDB );
	m_log.OutputLog( err_def::StrLogFile,     m_strOutput );
	m_log.OutputLog( err_def::StrMeshList,    m_strMeshList );
	m_log.OutputLog( err_def::StrNgBuild,     m_strNgBuild );
	m_log.OutputLog( err_def::StrReplace,     m_strReplaceWord );
	m_log.OutputLog( err_def::StrErrWord,     m_strErrWord );
	m_log.OutputLog( err_def::StrWarnWord,    m_strWarnWord );
	m_log.OutputLog( err_def::StrBldcOKWord,  m_strBldcOKWord );
	m_log.OutputLog( err_def::StrBldcNGWord,  m_strBldcNGWord );
	m_log.OutputLog( err_def::StrChkFixclass, m_chkFixclass ? _T("true") : _T("false") );
	m_log.OutputLog( err_def::StrJoinAnno,    m_strJoinAnno );
	m_log.OutputLog(err_def::StrExcludeMark, m_strExclude_mark.IsEmpty()? _T("指定なし"): m_strExclude_mark );
	m_log.OutputLog( err_def::StrBufferRange, str_util::ToString( m_bufferRange ) );
};

// 実行
bool CCheckBuildNamePoint::execute()
{
	// 初期化
	if( !init() )
		return false;

	// ヘッダ出力
	OutputLogHeader();
	m_log.OutputLog( err_def::StrStartTime, util::GetCurTime() );

	// メッシュごとの処理
	for( auto& mesh : m_meshList ){

		CString strMesh;
		strMesh.Format( _T("%ld"), mesh );

		m_log.OutputStdErr( strMesh );
		m_log.OutputLog( strMesh );

		// 隣接9メッシュコード取得
		set<long> rin9Mesh;
		Get9Mesh( mesh, rin9Mesh );

		// 9メッシュ分のポリゴン取得
		IGeometryPtr ipMeshGeo, ip9MeshGeo;
		if( ! GetMeshPolygons( mesh, rin9Mesh, ipMeshGeo, ip9MeshGeo ) ){
			m_log.OutputStdErr( err_def::GetMeshFail, strMesh );
			m_log.OutputLog( err_def::GetMeshFail, strMesh );
			continue;
		}

		// 該当範囲内の、ビル名称、建物、注記を全て覚える
		map<long, GeoClass> blNameMap, buildingMap, cAnnoMap;
		if( ! GetObjInMesh( ip9MeshGeo, blNameFields,   m_ipBlNameFC,   m_blNameFM,   blNameMap ) ||
			! GetObjInMesh( ip9MeshGeo, buildingFields, m_ipBuildingFC, m_buildingFM, buildingMap ) ||
			! GetObjInMesh( ip9MeshGeo, cAnnoFields,    m_ipCityAnnoFC, m_cityAnnoFM, cAnnoMap ) ){

			m_log.OutputStdErr( err_def::GetObjFail, strMesh );
			m_log.OutputLog( err_def::GetObjFail, strMesh );
			continue;
		}

		// RTree作成
		BGRTree blNameRTree, buildingRTree, cAnnoRTree;
		createRTree( blNameMap,   blNameRTree );
		createRTree( buildingMap, buildingRTree );
		createRTree( cAnnoMap,    cAnnoRTree );

		// チェック
		CheckMain( ipMeshGeo, blNameMap, buildingMap, cAnnoMap, blNameRTree, buildingRTree, cAnnoRTree );
	}

	m_log.OutputLog( err_def::StrEndTime, util::GetCurTime() );

	return true;
}

// チェックのメイン関数
void CCheckBuildNamePoint::CheckMain( const IGeometryPtr& ipMeshGeo, const map<long, GeoClass>& blNameMap, const map<long, GeoClass>& buildingMap, const map<long, GeoClass>& cAnnoMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree, const BGRTree& cAnnoRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// ビル名称ポイントごとのチェック
	for( const auto& blName : blNameMap ){

		long  blNameOid = blName.first;
		auto& blNameObj = blName.second;

		// 対象メッシュと関係ないものはスキップ
		if( blNameObj.IsDisjoint( ipMeshGeo ) )
			continue;

		// ビルディングID
		long  buildID = _ttol( blNameObj.GetFieldValue( kBuildingOID ) );

		// 家形IDと建物の整合が合うか確認
		bool bBuildOK = CheckBuildIdPos( blNameOid, blNameObj, buildID, buildingMap, buildingRTree );

		// 名称関連のチェック
		CheckBlNameStr( blNameOid, blNameObj );

		// 付加価値情報チェック
		CheckBlNameAddInfo( blNameOid, blNameObj );

		// 以下は所属家形が正しいことを前提に進める
		if( ! bBuildOK )
			continue;

		// 所属建物取得（一致していることは上で確認済み）
		auto  buildItr = buildingMap.find( buildID );
		if( buildItr == buildingMap.end() )
			continue; // ないはず
		auto& buildObj = buildItr->second;

		// NG属性じゃないかチェック
		CheckBuildClass( blNameOid, blNameObj, buildID, buildObj );

		// 同一建物に、複数ポイントが発生していないかチェック
		CheckDupBlName( blNameOid, blNameObj, buildObj, blNameMap, blNameRTree );

		// ビル名称から、バッファを広げたジオメトリ
		IGeometryPtr ipBufferGeom = getBufferGeometry( blNameObj.GetGeometry() );

		// バッファを広げて、近隣のビル名と一致がないかチェック
		CheckBufferRangeBlName( blNameOid, blNameObj, ipBufferGeom, blNameMap, buildingMap, blNameRTree, buildingRTree );

		// 注記との整合チェック
		CheckBufferRangeAnno( blNameOid, buildID, blNameObj, buildObj, ipBufferGeom, cAnnoMap, buildingMap, cAnnoRTree, buildingRTree );
	
	}
}

// ビル名称ポイントの建物IDと位置の整合チェック
bool CCheckBuildNamePoint::CheckBuildIdPos( long blNameOid, const GeoClass& blNameObj, long buildID, const map<long, GeoClass>& buildingMap, const BGRTree& buildingRTree )
{
	// RTreeで関係建物ID取得
	bool bFind = false, bOK = false, bHole = false;
	long findID = -1;
	auto& findBuildIDs = getIntersectsIDs( blNameObj.GetBox(0.1), buildingRTree );
	for( const auto& findBuildID : findBuildIDs ){
		auto itr = buildingMap.find( findBuildID );
		if( itr == buildingMap.end() )
			continue;
		auto& findBuildObj = itr->second;

		// 点とポリゴンのため、Disjointで良い（線上もOK）
		// 穴あきを埋めた形状で先に見る
		if( blNameObj.IsDisjoint( getExterior( findBuildObj.GetGeometry() ) ) )
			continue;

		// 元の状態で確認し、穴あきに落ちてるかどうか
		if( blNameObj.IsDisjoint( findBuildObj.GetGeometry() ) )
			bHole = true;

		bFind = true;
		findID = findBuildID;
		if( buildID != findBuildID )
			continue;

		// ここまでくればOK（ただし、後で穴あきかどうかを見る）
		bOK = true;
		break;
	}

	// エラー出力
	if( ! bFind ){ // 家形外に存在する( ! bFind )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eDisjointBuild );
		return false;
	}

	if( ! bOK ){
		if( bHole ) // 家形IDが一致しない家形の中抜き部分に存在する( bFind && ! bOK && bHole )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eHoleUnMatchID, str_util::ToString( findID ), nStr );
		else // 家形IDが一致しない( bFind && ! bOK && ! bHole )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eUnMatchBuildID, str_util::ToString( findID ), nStr );
			
		return false;
	}

	if( bHole ){ // 家形IDが一致する家形の中抜き部分に存在する( bFind && bOK && bHole )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::wHoleMatchID, str_util::ToString( findID ), nStr );
		return false;
	}

	// OK( bFind && bOK && ! bHole )
	return true;
}

// ビル名称ポイントの名称チェック
void CCheckBuildNamePoint::CheckBlNameStr( long blNameOid, const GeoClass& blNameObj )
{
	using namespace sindy::schema::sj::buildingname_point;
	auto& nameStr = blNameObj.GetFieldValue( kName );

	std::string str = nameStr;
	err_def::ECode errDef = err_def::Ok;

	if( m_cNameChk.IsNullString( str ) ){
		// 名称が空エラー
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNoName );
		return;
	}

	// 文字列内に仕様範囲（第一・第二漢字、かなカナ、全英数字、ギリシャ文字、記号）外の文字が無いか確認
	if( ! m_cNameChk.IsOkString( str ) || ! m_cNameChk.IsEvenSize( str ) ){
		// 仕様範囲外エラー
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eOutRangeWord );
		return; 
	}

	// 文字列長チェック(bug12343で追加)
	if (nameStr.GetLength() > nameMaxLength) {
		m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eLengthOver);
	}
	// ｜が存在したらエラー
	if( m_cNameChk.IsFindPipe( str ) )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::ePipeExist );

	// 全角スペースの確認
	if( ! m_cNameChk.IsOkSpace( str ) )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgSpace );

	// ハイフン関連が正常に格納されているか
	errDef = m_cNameChk.IsOkHiphen( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// 小文字関連が正常に格納されているか
	errDef = m_cNameChk.IsOkLowStr( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// カッコの整合確認
	errDef = m_cNameChk.IsOkKakko( str );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );

	// NGワードの確認
	for( const auto& errStr : m_errWordVec ){
		if( string::npos != m_cNameChk.string_find( str, errStr ) )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgWord, errStr.c_str() );
	}
	for( const auto& warnStr : m_warnWordVec ){
		if( string::npos != m_cNameChk.string_find( str, warnStr ) )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::wNgWord, warnStr.c_str() );
	}

	// 種別確定コード取得
	long fixclass = _ttol( blNameObj.GetFieldValue( kFixClass ) );

	// "chk_fixclass"オプションがtrueの場合は
	// 種別確定コードが「未確認（0）」でないオブジェクトに対しては建物種別コード別OK/NGワードチェックを行わない
	if( m_chkFixclass && fixclass != 0 )
		return;

	// 建物種別コード別OK/NGワードチェック
	CString errStr;
	errDef = CheckBlNameByBldcWord( blNameObj, str, errStr );
	if( errDef != err_def::Ok )
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, errStr );
}

// 建物種別コード別OK/NGワードによる名称チェック
err_def::ECode CCheckBuildNamePoint::CheckBlNameByBldcWord( const GeoClass& blNameObj, const std::string& blName, CString& judgeStr )
{
	using namespace sindy::schema::sj::buildingname_point;

	// 建物種別コード1~3取得
	set<long> bldClassSet;
	CString bldClass1 = blNameObj.GetFieldValue( kBldgClass1 );
	CString bldClass2 = blNameObj.GetFieldValue( kBldgClass2 );
	CString bldClass3 = blNameObj.GetFieldValue( kBldgClass3 );

	if( !bldClass1.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass1 ) );
	if( !bldClass2.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass2 ) );
	if( !bldClass3.IsEmpty() )
		bldClassSet.insert( _ttol( bldClass3 ) );

	// 判定に用いられた文字列格納用
	CString okWord, ngWord;

	// 建物種別コード別NGワードを持っているかチェック
	bool hasNGWord = HasBldcNGWord( blName, bldClassSet, ngWord );

	// 建物種別コード別OKワードのチェック
	err_def::ECode errDef = err_def::Ok;
	errDef = CheckBldNameByBldcOKWord( blName, bldClassSet, okWord );

	// OKワードチェック結果がOKで、建物種別コード別NGワードを持っている
	if( ( errDef == err_def::oOKWordByBldcOKWord || errDef == err_def::oOkWordByMultiBldcOKWord ) && hasNGWord ){
		judgeStr = ngWord;
		return err_def::wConflictBldcWordChk;
	}

	// 建物種別コード別NGワードを持っている
	if( hasNGWord ){
		judgeStr = ngWord;
		return err_def::eNGNameByBldcNGWord;
	}

	judgeStr = okWord;
	return errDef;
}

// 建物種別コード別NGワードを名称に持つかどうか
bool CCheckBuildNamePoint::HasBldcNGWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr )
{
	for( const auto& bldClass : bldClassSet )
	{
		if( !bldClass )
			continue;

		// 建物種別コード別NGワードリストに定義がある建物種別かどうか
		auto& itr = m_bldcNGWordMap.find( bldClass );
		if( itr == m_bldcNGWordMap.end() )
			continue;

		// 建物種別コード別NGワードを持っているか
		for( const auto& ngWord : itr->second )
		{
			if( string::npos != m_cNameChk.string_find( blName, ngWord.GetString() ) ){
				judgeStr = ngWord.GetString();
				return true;
			}
		}
	}
	return false;
}

// 建物種別コード別OKワードによるチェック
err_def::ECode CCheckBuildNamePoint::CheckBldNameByBldcOKWord( const std::string& blName, const std::set<long>& bldClassSet, CString& judgeStr )
{
	// エラー種別格納用
	err_def::ECode errDef = err_def::Ok;

	// 建物種別コード別OKワードリストに定義があるOKワードのループ
	for( const auto& okWord : m_bldcOKWordMap )
	{
		// 名称にOKワードを含むか
		if( string::npos == m_cNameChk.string_find( blName, okWord.first.GetString() ) )
			continue;

		switch( errDef ){
		// 既にOKワードリストの定義に合致している場合は、
		// 名称にOKワードリストの定義に反する文字列も含むかどうかチェックする
		case err_def::oOKWordByBldcOKWord:
		case err_def::oOkWordByMultiBldcOKWord:
			if( err_def::wNGNameByBldcOKWord == CheckOKBldc( bldClassSet, okWord.second ) ){
				judgeStr = okWord.first.GetString();
				return err_def::wConflictOKWordChk;
			}
			break;
		// 既に名称がOKワードリストの定義に反している場合は、
		// 名称にOKワードリストの定義に合致する文字列も含むかどうかチェックする
		case err_def::wNGNameByBldcOKWord:
			errDef = CheckOKBldc( bldClassSet, okWord.second );
			if( errDef == err_def::oOKWordByBldcOKWord || errDef == err_def::oOkWordByMultiBldcOKWord ){
				return err_def::wConflictOKWordChk;
			}
			break;
		case err_def::Ok:
		default:
			judgeStr = okWord.first.GetString();
			errDef = CheckOKBldc( bldClassSet, okWord.second );
			break;
		}

	}
	return errDef;
}

// 建物種別コードチェック
err_def::ECode CCheckBuildNamePoint::CheckOKBldc( const std::set<long>& chkBldcSet, const std::vector<long>& okBldcVec )
{
	// chkBldcSetに、okBldcVecに含まれる建物種別コードがあるかどうか
	for( const auto& bldClass : chkBldcSet )
	{
		if( !bldClass )
			continue;

		if( okBldcVec.end() != find( okBldcVec.begin(), okBldcVec.end(), bldClass ) ){
			// 複数種別で定義されているOKワードかどうか
			return okBldcVec.size() > 1 ? err_def::oOkWordByMultiBldcOKWord : err_def::oOKWordByBldcOKWord;
		}
	}
	return err_def::wNGNameByBldcOKWord;
}

namespace check_addinfo_func{
	// ｜（パイプ）に関するチェック
	err_def::ECode checkPipe( const GeoClass& blNameObj, const CString& field )
	{
		// 名称とヨミでエラーを分けるラムダ式
		auto getErr = [&field]{
			if( field.CompareNoCase( kNameSeparated ) == 0 )
				return err_def::eNamePipeInvalid;
			
			if( field.CompareNoCase( kYomiSeparated ) == 0 )
				return err_def::eYomiPipeInvalid;

			assert( false );
			return err_def::Ok;
		};

		// UNICODEでないと先頭、末尾の取得がうまくいかない
		const CStringW checkStr = blNameObj.GetFieldValue( field );

		// 先頭または末尾
		if( ( checkStr.Left(1).Compare( L"｜" ) == 0 ) ||
			( checkStr.Right(1).Compare( L"｜" ) == 0 ) )
			return getErr();

		// 連続
		if( -1 < checkStr.Find( L"｜｜" ) )
			return getErr();

		return err_def::Ok;

	}

	err_def::ECode checkMustInfo(const GeoClass& blNameObj)
	{
		// 付加情報必須項目
		static const std::vector<CString> mustFields = boost::assign::list_of
			(kNameSeparated)
			(kYomiSeparated)
			(kBldgClass1);

		// 値が入っていないものをカウント
		long nulls = std::count_if(mustFields.begin(), mustFields.end(),
			[&blNameObj](const CString& field) {
				return blNameObj.GetFieldValue(field).IsEmpty();
		});

		// 全部入っていればOK
		if( nulls == 0 )
			return err_def::Ok;

		// 全部入っていない場合は、他に余計なものがなければOK
		if (nulls == mustFields.size() )
		{
			static const std::vector<CString> notMust = boost::assign::list_of
				(kBldgClass2)
				(kBldgClass3)
				(kFloors);

			for (const auto& fld : notMust)
			{
				if (!blNameObj.GetFieldValue(fld).IsEmpty())
					return err_def::eInsufficientInfo;
			}

			// ここはOK
			return err_def::Ok;
		}

		return err_def::eRequiredInfoInvalid;
	}

	err_def::ECode checkBldClass(const GeoClass& blNameObj)
	{
		const auto bldClass1 = blNameObj.GetFieldValue(kBldgClass1);
		// 種別１が空だったら２，３も空なのは保証される（必須項目チェック）
		if (bldClass1.IsEmpty())
			return err_def::Ok;


		const auto bldClass2 = blNameObj.GetFieldValue(kBldgClass2);
		const auto bldClass3 = blNameObj.GetFieldValue(kBldgClass3);

		// 種別2中抜け
		if (!bldClass1.IsEmpty() &&
			bldClass2.IsEmpty() &&
			!bldClass3.IsEmpty())
			return err_def::eNoBldClass2;

		// 同一種別
		if (bldClass1.Compare(bldClass2) == 0 ||
			bldClass1.Compare(bldClass3) == 0 )
			return err_def::eSameBldClass;

		// 2と3は空でない場合のみ比較
		if( !bldClass2.IsEmpty() && 
			(bldClass2.Compare(bldClass3) == 0) )
			return err_def::eSameBldClass;

		return err_def::Ok;
	}
} // check_addinfo_func

// ビル名称ポイント付加価値情報関連チェック
void CCheckBuildNamePoint::CheckBlNameAddInfo( long blNameOid, const GeoClass& blNameObj )
{
	using namespace check_addinfo_func;

	// パイプチェック
	static const std::vector<CString> fields = boost::assign::list_of
		(kNameSeparated)
		(kYomiSeparated);

	for( const auto& field : fields )
	{
		auto errDef = checkPipe( blNameObj, field );
		if( errDef != err_def::Ok )
			m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef );
	}

	// 必須項目チェック
	auto errDef = checkMustInfo(blNameObj);
	if (errDef != err_def::Ok)
		m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef);

	// 種別チェック
	errDef = checkBldClass(blNameObj);
	if (errDef != err_def::Ok)
		m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef);

	// 名称と名称（分割）の整合チェック
	auto nameSeparated = blNameObj.GetFieldValue(kNameSeparated);
	vector<CString> nameStrings;
	// そもそも分割が入っていないものは対象外
	if (!nameSeparated.IsEmpty())
	{
		auto nameForLog = nameSeparated;	// 改行コードが入っていると、ログが乱れるため
		nameForLog.Replace(_T("\r\n"), _T("<改行>"));
		// Compare()で比較（と等価）
		if (blNameObj.GetFieldValue(kName) !=
			uh::str_util::replace(nameSeparated, _T("｜")))
		{
			m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eDifferentName, nameForLog);
		}
		else {
			// 分割されたそれぞれの句に対して、文字列の妥当性をチェック（bug12336で追加）
			nameStrings = ExcludeListedMark(uh::str_util::split(nameSeparated, _T("｜"), true));
			for (const auto& str : nameStrings) {
				// ハイフン関連が正常に格納されているか
				errDef = m_cNameChk.IsOkHiphen( str.GetString() );
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, nameForLog);

				// 小文字関連が正常に格納されているか（bug12336で追加）
				errDef = m_cNameChk.IsOkLowStr( str.GetString() );
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, nameForLog);
			}
		}
	}

	// ヨミ（分割）のチェック
	auto yomiSeparated = blNameObj.GetFieldValue(kYomiSeparated);
	if ( !yomiSeparated.IsEmpty() ) {
		auto yomiForLog = yomiSeparated;	// 改行コードが入っていると、ログが乱れるため
		yomiForLog.Replace(_T("\r\n"), _T("<改行>"));

		// カナチェック(「｜」を除いたものでチェック)
		if (!CheckYomiCharacter(uh::str_util::replace(yomiSeparated, _T("｜")).GetString()))
		{
			m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eYomiKanaInvalid, yomiForLog);
		}
		else {
			// 名称（分割）とヨミ（分割）の分割数チェック（bug12337で追加）
			// 名称（分割）は、除外記号リストを考慮しての比較
			auto yomiStrings = uh::str_util::split(yomiSeparated, _T("｜"), true);
			const auto nameSize = nameStrings.size();
			if ( nameSize > 0 && yomiStrings.size() != nameSize)
			{
				m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eDifferentCount, yomiForLog);
			}
			// 分割されたそれぞれの句に対して、文字列の妥当性をチェック（bug12336で追加）
			for (const auto& str : yomiStrings) {
				// ハイフン関連が正常に格納されているか
				errDef = m_cNameChk.IsOkHiphen(str.GetString());
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, yomiForLog);

				// 小文字関連が正常に格納されているか（bug12336で追加）
				errDef = m_cNameChk.IsOkLowStr(str.GetString());
				if (errDef != err_def::Ok)
					m_log.OutputLog(kTableName, blNameOid, blNameObj, errDef, yomiForLog);
			}
		}

	}

	//階数チェック「NULL」はOK「0」が残っていたらNG（bug12344で追加）
	auto kaisu = blNameObj.GetFieldValue(kFloors);
	if (!kaisu.IsEmpty() && _ttol(kaisu) == 0)
	{
		m_log.OutputLog(kTableName, blNameOid, blNameObj, err_def::eKaisuZero);
	}

}

// ビル名称ポイントの所属家形の属性チェック
void CCheckBuildNamePoint::CheckBuildClass( long blNameOid, const GeoClass& blNameObj, long buildID, const GeoClass& buildObj )
{
	using namespace sindy::schema;
	long buildClass = _ttol( buildObj.GetFieldValue( building::kBldClass ) );
	auto ngItr = m_ngBuildClass.find( buildClass );
	if( ngItr == m_ngBuildClass.end() ) // 除外属性のみ格納されているため、見つからなければOK
		return;

	// NG属性エラー
	m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eNgBlClass, str_util::ToString( buildID ), str_util::ToString( buildClass ) );
}

// 同一建物内のビル名称ポイント個数チェック
void CCheckBuildNamePoint::CheckDupBlName( long blNameOid, const GeoClass& blNameObj, const GeoClass& buildObj, const std::map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree )
{
	// RTreeで関係ビル名称ID取得
	auto& findBlNameIDs = getIntersectsIDs( buildObj.GetBox(), blNameRTree );
	for( const auto& findBlNameID : findBlNameIDs ){

		// 自身はスキップ
		if( findBlNameID == blNameOid )
			continue;

		auto itr = blNameMap.find( findBlNameID );
		if( itr == blNameMap.end() )
			continue;
		auto& findBlNameObj = itr->second;

		// 点とポリゴンのため、Disjointで良い（線上もOK）
		// 中抜きを埋めた形状で検索（bug 12342）
		if (findBlNameObj.IsDisjoint(getExterior(buildObj.GetGeometry())))
			continue;

		// 同一家形複数ビル名称エラー
		auto& findBlName = findBlNameObj.GetFieldValue( kName );
		cLonLat findLonLat( findBlNameObj.GetGeometry() );
		m_log.OutputLog( kTableName, blNameOid, blNameObj, err_def::eDupPointInOne, findBlName, str_util::ToString( findBlNameID ), nStr, findLonLat );
	}
}

// バッファ範囲のビル名称ポイント名称チェック
void CCheckBuildNamePoint::CheckBufferRangeBlName( long blNameOid, const GeoClass& blNameObj, const IGeometryPtr& ipBufferGeom, const map<long, GeoClass>& blNameMap, const map<long, GeoClass>& buildingMap, const BGRTree& blNameRTree, const BGRTree& buildingRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// 探索元のビル名称
	auto& blName = blNameObj.GetFieldValue( kName );
	// バッファにかかる建物のループ
	GeoClass bufferObj;
	bufferObj.SetGeometry( ipBufferGeom );
	auto& findBuildingIDs = getIntersectsIDs( bufferObj.GetBox(), buildingRTree );
	for( const auto& findBuildingID : findBuildingIDs ){

		// 実際に接触しているか確認
		auto buildItr = buildingMap.find( findBuildingID );
		if( buildItr == buildingMap.end() )
			continue;
		auto& findBuildObj = buildItr->second;

		if( findBuildObj.IsDisjoint( ipBufferGeom ) )
			continue;

		// 付近の家形に所属するビル名称との名称一致チェック
		CheckBufferRangeName( blNameOid, blName, blNameObj, findBuildObj, blNameMap, blNameRTree );
	}
}

// 名称比較チェック
void CCheckBuildNamePoint::CheckBufferRangeName( long blNameOid, const CString& blName, const GeoClass& blNameObj, const GeoClass& buildObj, const map<long, GeoClass>& blNameMap, const BGRTree& blNameRTree )
{
	using namespace sindy::schema::sj::buildingname_point;

	// バッファにかかる建物に所属するビル名称ポイントのループ
	auto& findBlNameIDs = getIntersectsIDs( buildObj.GetBox(), blNameRTree );
	for( const auto& findBlNameID : findBlNameIDs ){

		// 自身はスキップ
		if( findBlNameID == blNameOid )
			continue;

		// 実際に接触しているか確認
		auto blItr = blNameMap.find( findBlNameID );
		if( blItr == blNameMap.end() )
			continue;
		auto& findBlNameObj = blItr->second;

		if( buildObj.IsDisjoint( findBlNameObj.GetGeometry() ) )
			continue;

		// 名称が一致、もしくはどちらか包含の場合エラー
		bool bSame = false;
		auto& findBlName = findBlNameObj.GetFieldValue( kName );
		if( ! IsLikeName( blName, findBlName, bSame ) )
			continue;

		// 一致エラー
		cLonLat findLonLat( findBlNameObj.GetGeometry() );
		err_def::ECode errDef = bSame ? err_def::wSameNameInBuf : err_def::wLikeNameInBuf;
		m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findBlName, str_util::ToString( findBlNameID ), str_util::ToString( m_bufferRange ), findLonLat );
	}
}

// 建物ビル名と注記名称が一致するか（どちらか包含を含む）
bool CCheckBuildNamePoint::CompAnnoStr( const CString& blName, const GeoClass& annoObj, bool& bSame )
{
	// 名称が一致、もしくはどちらか包含の場合はOK
	auto& findAnnoName = annoObj.GetFieldValue( city_annotation::kNameString1 );
	if( IsLikeName( blName, findAnnoName, bSame) )
		return true; // 一致

	// 注記種別によって文字列をつなげて確認する
	long findAnnoClass = _ttol( annoObj.GetFieldValue( city_annotation::kAnnoCode ) );
	auto itr = m_joinAnnoMap.find( findAnnoClass );
	if( itr == m_joinAnnoMap.end() )
		return false; // 不一致
	for( const auto& joinAnnoStr : itr->second ){
		CString joinAnnoName1 = findAnnoName + joinAnnoStr;
		CString joinAnnoName2 = joinAnnoStr + findAnnoName;
		if( IsLikeName( blName, joinAnnoName1, bSame) || IsLikeName( blName, joinAnnoName2, bSame) )
			return true; // 一致
	}
	return false;
}

// 名称比較チェック
void CCheckBuildNamePoint::CheckBufferRangeAnno( long blNameOid, long buildID, const GeoClass& blNameObj, const GeoClass& buildObj, const IGeometryPtr& ipBufferGeom, const map<long, GeoClass>& cAnnoMap, const map<long, GeoClass>& buildingMap, const BGRTree& cAnnoRTree, const BGRTree& buildingRTree )
{
	// 自身の家形内に一致名称があれば、付近は見る必要が無い
	// 自身の家形内に注記が無い、もしくは一致名称が無い場合、付近を見て、エラー出しわけ
	using namespace sindy::schema;
	using namespace sj;

	// 探索元のビル名称
	auto& blName = blNameObj.GetFieldValue( buildingname_point::kName );

	// 自身の建物内に注記が存在するか確認
	long sameBuildAnnoCnt = 0; // 自身の建物内に存在する注記数
	auto& findSameBuildAnnoIDs = getIntersectsIDs( buildObj.GetBox(), cAnnoRTree );
	for( const auto& findSameBuildAnnoID : findSameBuildAnnoIDs ){
		
		// 実際に接触しているか確認
		auto annoItr = cAnnoMap.find( findSameBuildAnnoID );
		if( annoItr == cAnnoMap.end() )
			continue;
		auto& findAnnoObj = annoItr->second;

		if( buildObj.IsDisjoint( findAnnoObj.GetGeometry() ) )
			continue;

		++sameBuildAnnoCnt;
		bool bSame = false;
		if( CompAnnoStr( blName, findAnnoObj, bSame ) )
			return; // 自身の家形内で一致したらOK
	}

	// バッファにかかる建物のループ
	IGeometryPtr ipUnionGeom, ipTempGeom = buildObj.GetGeometry(); // 後で建物内外判定用
	ISpatialReferencePtr ipSpRef = buildObj.GetGeometry()->GetSpatialReference();
	bool bBuildUnionFail = false;
	bool bOtherSameName  = false;
	GeoClass bufferObj;
	bufferObj.SetGeometry( ipBufferGeom );
	auto& findBuildingIDs = getIntersectsIDs( bufferObj.GetBox(), buildingRTree );
	for( const auto& findBuildingID : findBuildingIDs ){

		// 自身の所属建物は上で確認しているため、スキップ
		if( findBuildingID == buildID )
			continue;

		// 形状取得
		auto buildItr = buildingMap.find( findBuildingID );
		if( buildItr == buildingMap.end() )
			continue;
		auto& findBuildObj = buildItr->second;

		// 後で家形外の注記判別用にUnion
		if( FAILED( ((ITopologicalOperatorPtr)ipTempGeom)->Union( findBuildObj.GetGeometry(), &ipUnionGeom )) )
			bBuildUnionFail = true;
		ipUnionGeom->PutRefSpatialReference( ipSpRef );
		ipTempGeom = ipUnionGeom;
		
		// 実際に接触しているか確認
		if( findBuildObj.IsDisjoint( ipBufferGeom ) )
			continue;

		// 該当建物内に存在する注記のループ
		auto& findOtherBuildAnnoIDs = getIntersectsIDs( findBuildObj.GetBox(), cAnnoRTree );
		for( const auto& findOtherBuildAnnoID : findOtherBuildAnnoIDs ){

			// 実際に接触しているか確認
			auto annoItr = cAnnoMap.find( findOtherBuildAnnoID );
			if( annoItr == cAnnoMap.end() )
				continue;
			auto& findAnnoObj = annoItr->second;

			if( findBuildObj.IsDisjoint( findAnnoObj.GetGeometry() ) )
				continue;

			// 名称が一致、もしくはどちらか包含の場合はエラー
			bool bSame = false;
			if( CompAnnoStr( blName, findAnnoObj, bSame ) ){
				
				// エラー出力
				err_def::ECode errDef = GetExistAnnoErrDef( sameBuildAnnoCnt, bSame );
				auto& findAnnoName = findAnnoObj.GetFieldValue( city_annotation::kNameString1 );
				cLonLat findLonLat( findAnnoObj.GetGeometry() );
				m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findAnnoName, str_util::ToString( findOtherBuildAnnoID ), str_util::ToString( m_bufferRange ), findLonLat );
				bOtherSameName = true;
			}
		}
	}

	// バッファにかかる注記のループ
	auto& findBufferAnnoIDs = getIntersectsIDs( bufferObj.GetBox(), cAnnoRTree );
	for( const auto& findBufferAnnoID : findBufferAnnoIDs ){
		
		auto itr = cAnnoMap.find( findBufferAnnoID );
		if( itr == cAnnoMap.end() )
			continue;
		auto& findBufferAnnoObj = itr->second;

		// 建物に接触しているものは確認済みのためスキップ
		if( ! bBuildUnionFail ){
			if( ! findBufferAnnoObj.IsDisjoint( ipUnionGeom ) )
				continue;
		}
		else{ // 最低限自分の家形だけ外していれば、処理上問題なし(余計に同一エラーが出る)
			if( ! findBufferAnnoObj.IsDisjoint( buildObj.GetGeometry() ) )
				continue;
		}

		// 名称が一致、もしくはどちらか包含の場合はエラー
		bool bSame = false;
		if( CompAnnoStr( blName, findBufferAnnoObj, bSame ) ){

			// エラー出力
			err_def::ECode errDef = GetExistAnnoErrDef( sameBuildAnnoCnt, bSame );
			auto& findAnnoName = findBufferAnnoObj.GetFieldValue( city_annotation::kNameString1 );
			cLonLat findLonLat( findBufferAnnoObj.GetGeometry() );
			m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, findAnnoName, str_util::ToString( findBufferAnnoID ), str_util::ToString( m_bufferRange ), findLonLat );
			bOtherSameName = true;
		}
	}	

	// 周辺に存在する場合はここで終了
	if( bOtherSameName )
		return;

	// 周辺に存在しなかった場合のエラー出力
	err_def::ECode errDef = GetNoAnnoErrDef( sameBuildAnnoCnt );
	if( errDef == err_def::Ok )
		return;
	
	// エラー出力
	cLonLat dummyLonLat;
	m_log.OutputLog( kTableName, blNameOid, blNameObj, errDef, nStr, nStr, str_util::ToString( m_bufferRange ), dummyLonLat );
}

// 指定範囲分のバッファを取得する
IGeometryPtr CCheckBuildNamePoint::getBufferGeometry(const IGeometryPtr& ipGeometry)
{
	// 元の空間参照
	ISpatialReferencePtr ipSpRef = ipGeometry->GetSpatialReference();

	IGeometryPtr ipBuffer;

	//ジオメトリの情報を変更するので、クローンを作成する
	IClonePtr ipClone;
	IClonePtr(ipGeometry)->Clone(&ipClone);

	IGeometryPtr ipNewGeometry = ipClone;

	//測地系の変更(メッシュセンターで取得している19座標系)	
	ipNewGeometry->Project(m_ipPrjCoordinateSystem);

	ITopologicalOperatorPtr ipTopo( ipNewGeometry );

	//単位はメートル(m)
	ipTopo->Buffer( m_bufferRange, &ipBuffer );

	// 測地系を戻す
	ipBuffer->Project( ipSpRef );

	return ipBuffer;
}

// 中抜きポリゴンを埋める(Exteriorのみ取得)
IGeometryPtr CCheckBuildNamePoint::getExterior( const IGeometryPtr& ipGeom )
{
	// ジオメトリカウント取得
	IGeometryPtr ipRetGeom = ipGeom;
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long geoCnt = ipGeoCol->GetGeometryCount();
	if( geoCnt < 2 )
		return ipRetGeom;

	// 二つ以上の場合は、Exteriorのみで再構築する
	ISpatialReferencePtr ipSpRef = ipGeom->GetSpatialReference();
	IGeometryPtr ipExteriorGeo = ipGeoCol->GetGeometry(0);
	IPointCollectionPtr ipPointCol = ipExteriorGeo;
	long exteriorPointCnt = ipPointCol->GetPointCount();
	IPointCollectionPtr ipPolygon( CLSID_Polygon );
	((IGeometryPtr)ipPolygon)->PutRefSpatialReference( ipSpRef );
	for( int i = 0 ; i < exteriorPointCnt ; ++i )
		ipPolygon->AddPoint( ipPointCol->GetPoint( i ) );
	
	ipRetGeom = (IGeometryPtr)ipPolygon;

	return ipRetGeom;
};

// 二つの文字列が一致、もしくはどちらか包含か？
bool CCheckBuildNamePoint::IsLikeName( const CString& cStr1, const CString& cStr2, bool& bSame )
{
	bSame = false;
	if( cStr1.IsEmpty() || cStr2.IsEmpty() )
		return false;
	string str1 = ReplaceStr(cStr1), str2 = ReplaceStr(cStr2);
	if( str1.empty() || str2.empty() )
		return false;

	if( 0 == str1.compare( str2 ) ){
		bSame = true;
		return true; // 一致
	}

	if( string::npos != m_cNameChk.string_find( str1, str2 ) )
		return true; // str2はstr1に包含される

	if( string::npos != m_cNameChk.string_find( str2, str1 ) )
		return true; // str1はstr2に包含される

	return false;
}

string CCheckBuildNamePoint::ReplaceStr( const CString& cStr )
{
	string str = cStr;
	for( const auto& replaceWord : m_replaceWordVec ){
		long size = str.size();
		for( int i = 0 ; i < size ; ){
			string::size_type pos = m_cNameChk.string_find( str, replaceWord.first, i );
			if( pos == string::npos )
				break;

			if( 0 == replaceWord.second.compare( _T("（削除）") ) )
				str = str.substr( 0, pos ) + str.substr( pos + replaceWord.first.length() );
			else
				str = str.substr( 0, pos ) + replaceWord.second + str.substr( pos + replaceWord.first.length() );
			i = pos;
		}
	}
	return str;
}

// 除外リスト上の記号を除いた状態の分割文字列を求める
std::vector<CString> CCheckBuildNamePoint::ExcludeListedMark( const std::vector<CString>& cStr)
{
	std::vector<CString> resultStrings;
	for (const auto& str : cStr) {
		if (m_ExcludeMarkVec.find( str.GetString() ) == m_ExcludeMarkVec.end() ){
			resultStrings.push_back( str );
		}
	}
	return resultStrings;
}
