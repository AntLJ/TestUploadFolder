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
#include "BuddyRoadMgr.h"
#include <ArcHelperEx/Map.h>

using namespace sindy;
using namespace sindy::schema;

CBuddyRoad::CBuddyRoad( const IFeaturePtr& roadLink, const _IRowPtr& linkRel /*= nullptr*/ )
	: m_roadID(AheGetOID(roadLink)), 
	  m_geom(AheGetShapeCopy(roadLink)),
	  m_roadLink( roadLink ),
	  m_linkRel( linkRel ),
	  m_addF(false),
	  m_deleteF(false),
	  m_objectID(AheGetOID(linkRel)){}

CBuddyRoad::~CBuddyRoad()
{
}

CBuddyRoadMgr::CBuddyRoadMgr()
{
}


CBuddyRoadMgr::~CBuddyRoadMgr()
{
}

void CBuddyRoadMgr::Init( std::list<CFeatureDef>* featureDefs, const CArcHelper& ah  )
{ 
	m_searchedRoads.clear();
	// あり得ないはずだけど
	if( featureDefs->empty() )
		return;

	// FeatureDefsは他のレイヤも含まれている可能性がある
	auto it = boost::find_if( *featureDefs, []( CFeatureDef& def ){
		return ( _tcscmp( walk_link::kTableName, def.GetTableName() ) == 0 );
	});
	_ASSERTE( it != featureDefs->end() );

	// RINK_RELATIONは歩行者リンクと同じユーザのはず
	ITablePtr walkT = AheGetFeatureClass( it->GetFeature() );
	m_LinkRelT = AheOpenSameOwnerTable( walkT, link_relation::kTableName );
	if( !m_LinkRelT )
	{
		CString msg = uh::str_util::format( AheLoadString( IDS_NOT_FOUND_RELTBL ), AheGetFeatureClassOwnerName( walkT ) );
		MessageBox( ah.GetFocusMapHWND(), msg, AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return;
	}

	// ROAD_LINKは同じグループレイヤにいるもの
	CMap cMap( ah.GetFocusMap() );
	m_roadLinkT = AheGetFeatureClass( cMap.FindSameGroupLayer( walkT, road_link::kTableName ) );
	if( !m_roadLinkT )
	{
		// ダウンコンバート版の時には道路リンクがなくても、以下のメッセージを出さない。
		if( walk_type::type_class::kDownConvert != AheIsDownconvert( it->GetFeature() ) )
			MessageBox( ah.GetFocusMapHWND(), AheLoadString( IDS_NOT_FOUND_ROADLINK ), AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return;
	}
	// それぞれの歩行者リンクに紐付く道路リンクを確保
	for( auto& def : *featureDefs )
	{
		if( _tcscmp( walk_link::kTableName, def.GetTableName() ) != 0 )
			continue;

		long walkID = AheGetOID(def.GetFeature());

		// 既に紐付いてる道路リンクを探す
		_ICursorPtr cur;
		m_LinkRelT->Search( 
			AheInitQueryFilter( nullptr, nullptr, _T("%s=%ld"), link_relation::kBaseLinkID, walkID ),
			VARIANT_FALSE, &cur );

		// 道路リンクの情報をリストに取得
		_IRowPtr row;
		while( (S_OK == cur->NextRow( &row )) && row )
		{ 
			long oid = AheGetAttribute(row, link_relation::kObjectID).lVal;

			// 既にリストにいる場合はスルー（bug 11409）
			// しないと（ツリーで選択切り替えて）表示し直す度に増え続ける
			// 道路のIDではなく、LINK_RELATIONテーブル自体のオブジェクトIDでユニークになるよう変更（bug 12472）
			const auto it = boost::find_if( m_buddyRoads[walkID], [&oid](const CBuddyRoad& buddy ){
				return buddy.m_objectID == oid;
			});
			if( it != m_buddyRoads[walkID].end() )
				continue;

			IFeaturePtr roadLink;
			m_roadLinkT->GetFeature(AheGetAttribute(row, link_relation::kRelatedLinkID).lVal, &roadLink );
			m_buddyRoads[walkID].push_back( CBuddyRoad(roadLink, row) );
		}
	}
}

// 変更チェック
bool CBuddyRoadMgr::IsChanged( long walkID )
{
	// 1つでも変更があれば変更ありとする
	const auto& roadLinks = GetBuddyRoads(walkID);
	auto it = boost::find_if( roadLinks, []( const CBuddyRoad& road ){
		return road.IsChanged();
	});

	return ( it != roadLinks.end() );
}

// 存在チェック
bool CBuddyRoadMgr::HasBuddy( long walkID )
{
	// 削除されてるものは無視
	long count = boost::count_if( GetBuddyRoads( walkID ), []( const CBuddyRoad& road ){
		return !road.m_deleteF;
	});

	return ( count > 0 );
}


RoadList& CBuddyRoadMgr::GetBuddyRoads( long walkID )
{ 
	// 持ってなければダミーを返す
	auto it = m_buddyRoads.find( walkID );
	if( m_buddyRoads.end() == it )
	{
		static RoadList dummy;
		return dummy;
	}

	return it->second;
}

// 検索済み道路リンクからバディを追加
void CBuddyRoadMgr::AddFromSearchedRoads( long walkID )
{
	// 検索済みで削除フラグが立っているものが対象
	for( const auto& searched : m_searchedRoads )
	{
		if( !searched.m_deleteF )
			continue;

		// 既にあるバディから探す
		auto& buddys = GetBuddyRoads(walkID);

		if( !buddys.empty() )
		{
			auto it = boost::find( buddys, searched );
			if( buddys.end() != it )
			{
				// 歩行者リンクが既にバディを持っていて、自分がその中に含まれる状態
				// 最初からあったのを一旦消して、検索しなおして、また追加といった場合がありうる
				it->m_deleteF = false;
				continue;
			}
		}

		// 歩行者リンクに対するバディな道路が1つもない場合、GetBuddyRoads()は
		// ダミーを返すので、そこにpush_backしてはいけない。必ず自分のコンテナに入れること。
		m_buddyRoads[walkID].push_back(searched);

		// 追加フラグはここでのみ追加される前提
		m_buddyRoads[walkID].back().m_addF = true;
		m_buddyRoads[walkID].back().m_deleteF = false;
	}
	
	// 検索済みリンクからは削除
	boost::range::remove_erase_if( m_searchedRoads, [](const CBuddyRoad& road){
		return road.m_deleteF;
	});
}

// バディ関係にあるか
bool CBuddyRoadMgr::IsBuddy( long walkID, long roadID )
{
	const auto& roads = GetBuddyRoads( walkID );

	// 目当てのIDを探すラムダ式
	auto finder = [&roadID](const CBuddyRoad& road){
		return (road.m_roadID == roadID) && 
			   !road.m_deleteF;
	};

	return roads.end() != boost::find_if( roads, finder );
}

// 道路リンクのBuddy対象種別（bug 11283）
// TODO: 自動ひも付けでも同じことやってるので、ライブラリに分ける
bool isBuddyRoadClass( const _IRowPtr& roadRow )
{
	using namespace road_link::road_class;

	// 対象外種別（フェリー以降だが、庭園路はOK）
	switch ( AheGetAttribute( roadRow, road_link::kRoadClass ).lVal )
	{
	case kNone:                // 表示種別なし
	case kFerryNoDisp:         // フェリー航路（非表示）
	case kFerryS2:             // フェリー航路（S2破線）
	case kFerryS3:             // フェリー航路（S3破線）
	case kFerryS4:             // フェリー航路（S4破線）
	case kVirtual:             // 仮想リンク
	case kBridge:              // ブリッジリンク
	case kFacilityEntrance:    // 施設出入り口リンク
	case kParkingEntrance:     // 施設内リンク
	case kParking:             // 駐車場構内道路（PEC）
	case kMatching:            // マッチングリンク（PEC）
	case kMatchingBT:          // マッチング用リンク(BT)
		return false;
		break;

	default:
		break;
	};

	// 自動車専用
	if( 0 != AheGetAttribute( roadRow, road_link::kCarOnly ).lVal )
		return false;

	return true;
}

// 周辺検索
RoadList& CBuddyRoadMgr::ArroundSearch( IFeaturePtr walkLink, double dRange)
{
	if( !m_roadLinkT )
	{
		MessageBox( nullptr, AheLoadString( IDS_NOT_FOUND_ROADLINK ), AheLoadString( IDS_CAPTION ), MB_ICONWARNING );
		return m_searchedRoads;
	}

	// LINK_RELATIONテーブル検索
	auto getLinkRel = []( const ITablePtr& rekT, long walkID, long roadID )
	{
		static auto query = uh::str_util::format( _T("%s=%ld AND %s=%ld"),
			link_relation::kBaseLinkID, walkID,
			link_relation::kRelatedLinkID, roadID );

		_ICursorPtr cur;
		rekT->Search( AheInitQueryFilter( nullptr, nullptr, query ), VARIANT_FALSE, &cur );

		// あっても1個だけのはず
		_IRowPtr row;
		if( cur->NextRow( &row ) != S_OK )
			row = nullptr;

		return row;
	};

	m_searchedRoads.clear();

	ITopologicalOperatorPtr ipTopo( AheGetShapeCopy( walkLink ) );

	// m → 度変換
	IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
	double dConvRange = 0.0;
	ipUnitConv->ConvertUnits( dRange, esriMeters, esriDecimalDegrees, &dConvRange );

	// 形状をバッファリング
	IGeometryPtr ipGeoBuffer;
	ipTopo->Buffer( dConvRange, &ipGeoBuffer );

	// 空間検索
	IFeatureCursorPtr cur;
	m_roadLinkT->Search( AheInitSpatialFilter(nullptr, ipGeoBuffer), VARIANT_FALSE, &cur );

	long walkID = AheGetOID( walkLink );
	IFeaturePtr row;
	while( (S_OK == cur->NextFeature( &row )) && row )
	{
		// 既に紐付いているものは無視
		long roadID = AheGetOID(row);
		if( IsBuddy( walkID, roadID ) )
			continue;

		// 対象の道路種別だけ
		if( !isBuddyRoadClass( row ) )
			continue;

		m_searchedRoads.push_back( CBuddyRoad( row, getLinkRel( m_LinkRelT, walkID, roadID ) ) );
	}

	return m_searchedRoads;
}

// 論理チェック
bool CBuddyRoadMgr::CheckLogic( UINT& stringID )
{
	// TODO: チェックすることなくなった。。（bug 11283）
	//       一応、クチは残しておく。
	return true;
}

// ストア処理
bool CBuddyRoadMgr::UpdateLinkRelTable()
{
	for( auto& roads : m_buddyRoads )
	{
		for( auto& road : roads.second )
		{
			// 変更がないものは無視
			if( !road.IsChanged() )
				continue;

			// 追加（作成）
			if( road.m_addF )
			{
				_IRowPtr row = AheCreateRow( m_LinkRelT );
				if( !row )
					return false;

				// 属性付与
				if( !AheSetModifyData( row, sindyUpdateTypeCreate ) ||
					!AheSetAttribute( row, link_relation::kBaseLinkID, roads.first ) ||
					!AheSetAttribute( row, link_relation::kRelatedLinkID, road.m_roadID ) )
					return false;
				
				road.m_linkRel = row;
			}

			// 削除
			else if( road.m_deleteF )
			{
				road.m_linkRel->Delete();
			}

			// ストア
			if( FAILED(road.m_linkRel->Store() ) )
				return false;
		}
	}
	return true;
}

// 保持しているテーブル類の後始末
void CBuddyRoadMgr::ClearLinkMap()
{
	m_buddyRoads.clear();
	m_searchedRoads.clear();
}
