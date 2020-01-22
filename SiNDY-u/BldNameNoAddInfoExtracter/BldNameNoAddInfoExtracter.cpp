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
#include "BldNameNoAddInfoExtracter.h"
#include "globalfunc.h"
#include "Logger.h"
#include <ArcHelperEx/GlobalFunctions.h>
#include <ArcHelperEx/AheGlobalsMisc.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <TDC/sindylib_base/GeometryBase.h>
#include <WinLib/TimeTrace.h>

using namespace sindy;
using namespace schema;
using namespace sj;
using namespace sindyk;

CModel getBldNameModel()
{
	CModel model;
	model.Add( buildingname_point::kTableName, buildingname_point::kSrcOID,	nullptr, nullptr );
	model.Add( buildingname_src_point::kTableName, buildingname_src_point::kObjectID, buildingname_point::kTableName, buildingname_point::kSrcOID );
	return model;
}

// 初期化
void BldNameNoAddInfoExtracter::init()
{
	// 出力ファイル
	m_tsvWriter.init( m_args.getValue( option::kResult ) );

	// エラーログ
	ERRLOGGER.Initialize( m_args.getValue( option::kErrLog ).GetString() );

	// 建物ビル名称ファインダ初期化
	m_bldNameFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kBldNameDb ), buildingname_point::kTableName ), getBldNameModel() );
	
	// 行政界ファインダ初期化
	m_adminFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kAdminDb ), city_admin::kTableName ), CModel() );

	// POIファインダ初期化
	m_poiFinder.InitCollection( 
		tryOpenTable( m_args.getValue( option::kPoiDb ), sindyk::poi_point_org::kTableName ), CModel() );
	// 補完用POIの出典元に設定するため、ユーザ名を取得しておく
	m_poiUser = uh::str_util::split( m_args.getValue( option::kPoiDb ), _T("/") )[0];

	// 住所DB
	m_addrDb.init( m_args.getValue( option::kAddrDb ) );

}

namespace static_func{

	// 中縮行政界形状群取得
	std::map<CString, IGeometryPtr> getBaseAdminGeoms( CTableContainer& baseAdminT,  // BASE_ADMIN
		                                               const CString& adminList )    // 行政界コードリスト
	{
		using namespace uh::text_util;

		// key:行政界コード（5桁） value:形状
		std::map<CString, IGeometryPtr> admins;

		// リスト読み込み
		std::set<CString> strCity;
		for( const auto& city : ToTStrList( adminList.GetString() ) )
		{
			strCity.insert( city.c_str() );
		}

		// IN句の中身用に整形
		std::list<CString> cityCods;
		CTableBase::IDs2Str(std::list<CString>(strCity.begin(), strCity.end()), cityCods, _T("'") );
		// 検索
		for( const auto& city : cityCods )
		{
			auto query = AheInitQueryFilter( nullptr, nullptr, 
							_T("%s IN ( %s )"), base_admin::kCityCode, city);

			baseAdminT._Select( query, false );
		}

		std::set<CString> selectedCity;
		for( const auto& ba : baseAdminT )
		{
			const auto& row = *CAST_ROWC(ba);
			CString cityCode = row.GetValue(base_admin::kCityCode, CString());
			if (admins[cityCode]) {
				//既に存在していたら、ポリゴンをマージする（通常マルチパートになるはず）
				CGeometryBase basePolygon = admins[cityCode];
				basePolygon.Union(row.CRowBase::GetShapeCopy());
				admins[cityCode] = basePolygon.Clone();
			} else {
				admins[cityCode] = row.CRowBase::GetShapeCopy();
			}

			// チェック用
			selectedCity.insert(cityCode);
		}

		// リストで指定された行政界コードのレコードが全て存在するかチェック
		std::set<CString> diff;
		std::set_difference(
			strCity.begin(), strCity.end(),
			selectedCity.begin(), selectedCity.end(),
			std::inserter(diff, diff.end()));

		if (!diff.empty())
		{
			auto msg = uh::str_util::formatA(_T("指定された行政界のレコードが存在しない： %s"),
				uh::str_util::join(diff, _T(" ")));

			throw std::runtime_error(msg);
		}
		
		baseAdminT.clear();

		return admins;
	}

	// 11桁コード取得
	CString getCityAddrCode( const CRowContainer& row,  // 対象のビル名称
		                 CTableContainer& cityAdminT )  // CITY_ADMIN
	{
		cityAdminT._Select( (IQueryFilterPtr)AheInitSpatialFilter( nullptr, row.CRowBase::GetShapeCopy() ), true );
		if( cityAdminT.empty() )
		{
			ERRLOGGER.WriteLog( row, _T("CITY ADMIN 取得失敗。") );
			return CString();
		}

		CString cityCode = CAST_ROWC(*cityAdminT.begin())->GetValue( city_admin::kCityCode );
		CString addrCode = CAST_ROWC(*cityAdminT.begin())->GetValue( city_admin::kAddrCode );

		// begin()で1件だけ拾ってくるので毎回消さないとダメ
		cityAdminT.clear();
		return cityCode + addrCode;
	}

} // namespace static_func


// 実行
void BldNameNoAddInfoExtracter::execute()
{
	using namespace static_func;

	// 初期化
	init();
	
	// 各種テーブル
	auto& bldNameT = *m_bldNameFinder.FindTable( buildingname_point::kTableName );
	auto& cityAdminT = *m_adminFinder.FindTable( city_admin::kTableName );
	auto& srcPointT = *m_bldNameFinder.FindTable( buildingname_src_point::kTableName );
	auto& poiT = *m_poiFinder.FindTable( poi_point_org::kTableName );

	// 行政界（5桁）形状取得
	auto baseAdmins = getBaseAdminGeoms( 
		*m_adminFinder.FindTable( base_admin::kTableName), 
		m_args.getValue( option::kBaseAdminList ) );

	// 行政界単位で処理
	for( const auto& ba : baseAdmins )
	{
		if( !ba.second)
			continue;

		// 市町村単位で住所をキャッシュ
		m_addrDb.cacheAddrInCity( ba.first );

		// 行政界内を空間検索
		auto query = AheInitSpatialFilter( nullptr, ba.second );
		if( bldNameT.FindField(buildingname_point::kYomiSeparated) > -1 )
		{
			query = AheInitQueryFilter( (IQueryFilterPtr)AheGetClone(query), nullptr,
				_T("%s IS NULL"), buildingname_point::kYomiSeparated );
		}


		// 件数確認
		RUNLOGGER.Log( uh::str_util::format( _T("%s： %ld件 "), ba.first, bldNameT.RowCount( query )), true );
		boost::timer timer;
		uh::scope_guard sg( [&timer]{
			RUNLOGGER.Log( uh::str_util::format( _T("[完了] %lf[s]\n"), timer.elapsed() ), true );
		});

		// 検索実行
		std::map<CString, IQueryFilterPtr> querys = boost::assign::map_list_of
			( buildingname_point::kTableName, query );
		bldNameT.Select( querys, true );

		// BUIDNAME_POINTごとの処理
		for( const auto& bn : bldNameT )
		{
			const auto& bldNameRow = *CAST_ROWC( bn );

			// 1行分貯めこむ
			m_tsvWriter.addRecord(
				bldNameRow,
				getSrcPoint( bldNameRow, poiT ),
				m_addrDb.getAddrInfo( getCityAddrCode( bldNameRow, cityAdminT ) ),
				getPoiYomi( bldNameRow, poiT )
			);
		}

		// 結果書き込み
		m_tsvWriter.flush();

		bldNameT.clear();
		cityAdminT.clear();
		srcPointT.clear();
		poiT.clear();
	}
}


CSPRowContainer BldNameNoAddInfoExtracter::getSrcPoint( const CRowContainer& bldName, CTableContainer& poiT )
{
	CSPRowContainer srcP;
	switch (bldName.GetValue( buildingname_point::kSourceType, -1L ))
	{
	case buildingname_point::source_type::kCompAnnoPOI:
		{
			// 補完用POIは素材にいないので、ダミーを作ってでっち上げる
			srcP = bldName.FindTable( buildingname_src_point::kTableName )->CreateDummyRow();
			
			// POI_POINT_ORGのテーブルから探す
			auto it = poiT.find_by_key( kObjectID, bldName.GetValue( buildingname_point::kSrcOID, -1L ) );
			if( it == poiT.end() )
				return CSPRowContainer();

			// 出典資料IDはPOIシーケンス
			auto poi = CAST_ROWC(*it);
			long conCode = poi->GetValue( poi_point_org::kContentsCode, -1L );
			std::vector<CString> poiSeq = boost::assign::list_of
				( uh::str_util::ToString( conCode / 100000 ) )
				( uh::str_util::ToString( conCode % 100000 ) )
				( poi->GetValue( poi_point_org::kContentsSeq, _T("") ) );
			srcP->SetValue( buildingname_src_point::kSourceID, uh::str_util::join( poiSeq, _T("-") ) );

			// 出典コードは102固定
			srcP->SetValue( buildingname_src_point::kSourceType, (long)buildingname_src_point::source_type::kCompAnnoPOI );
			// 出典元は接続先のユーザ名
			srcP->SetValue( buildingname_src_point::kSourceName, m_poiUser );
		}
		break;

	default:
		// 補完用POI以外はモデルの対応付けにより紐付いているはず
		auto& srcPointT = *bldName.GetItemTable( buildingname_src_point::kTableName );
		if( srcPointT.empty() )
		{
			ERRLOGGER.WriteLog( bldName, _T("紐付く素材が見つからない。") );
			return srcP;
		}
		srcP = CAST_ROWC(*srcPointT.begin());
		break;
	}

	return srcP;
}

CString BldNameNoAddInfoExtracter::getPoiYomi( const CRowContainer& bldName, CTableContainer& poiT )
{
	// NULL表示用文字列（空文字）
	static const CString NULL_STRING = _T("");

	poiT.SetAutoCache(sindyContainerAutoOnDemandCacheDetach);

	// xxxx-x-xxx を コンテンツコードとシーケンスにするラムダ式
	auto toCodeAndSeq = [&bldName](const CString& seqStr )
	{
		auto seq = uh::str_util::split( seqStr, _T("-") );
		if( seq.size() != 3 )
		{
			ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
				_T("POIのシーケンスが不正[ %s ]"), seqStr ) );
			return std::make_pair(0L, CString() );
		}
		long conCode = _ttol(seq[0])*100000 + _ttol(seq[1]);
		return std::make_pair( conCode, seq[2] );
	};

	CSPRowContainer poi;
	switch (bldName.GetValue( buildingname_point::kSourceType, -1L ))
	{
	// 補完用POIはSrcOIDに直接POIのOIDを持っている
	case buildingname_point::source_type::kCompAnnoPOI:
		{
			long poiId = bldName.GetValue( buildingname_point::kSrcOID ).lVal;
			auto it = poiT.find_by_key( kObjectID, poiId);
			if( it != poiT.end() )
				poi = CAST_ROWC(*it);
			else
			{
				ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
					_T("元のPOI（補完用）が見つからない（OID：%ld）"), poiId ) );
			}
		}
		break;

	// 注記POIは素材PのOIDを持っていて、その素材PがPOIのシーケンスを持っている。。。
	case buildingname_point::source_type::kAnnoPOI:
		{
			// 素材OIDからコンテンツコードとシーケンスを取得する
			const auto& srcP = *getSrcPoint( bldName, poiT );
			CString strSeq = srcP.GetValue( buildingname_src_point::kSourceID, _T("") );
			auto codeAndSeq = toCodeAndSeq( strSeq );

			// コンテンツコードとコンテンツ内シーケンスをタグにして検索
			static const auto tags = std::make_pair( 
				poi_point_org::kContentsCode, poi_point_org::kContentsSeq );

			auto keys = std::make_pair( codeAndSeq.first, codeAndSeq.second );
			auto it = poiT.find_by_key( tags, keys );
			if( it != poiT.end() )
				poi = CAST_ROWC(*it);
			else
			{
				ERRLOGGER.WriteLog( bldName, uh::str_util::format( 
					_T("元のPOIが見つからない（POI：%s）"), strSeq ) );
			}
		}
		break;

	// POI以外はNULL
	default:
		return NULL_STRING;
		break;
	}

	// 見つからなかったらNULL
	if( !poi )
		return NULL_STRING;

	// 「ヨミヌル」ならNULL
	CString yomi = poi->GetValue( poi_point_org::kYomi, _T("") );
	if( yomi.Compare( _T("ヨミヌル") ) == 0 )
		return NULL_STRING;

	// ｜パイプは削除
	return uh::str_util::replace(yomi, _T("｜"));
}
