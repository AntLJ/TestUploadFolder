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
#include "RuleModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy::schema;

const CRuleModelNetwork        g_cRuleModelNetwork;        //!< ネットワークモデル定義クラス
const CRuleRelationshipContain g_cRuleRelationshipContain; //!< 内包関連定義クラス
const CRuleRelationTable       g_cRuleRelationTable;       //!< 外部テーブル参照定義クラス
const CRuleLinkQueue           g_cRuleLinkQueue;           //!< リンクキュー定義クラス

// CRuleModelNetwork
CRuleModelNetwork::CRuleModelNetwork()
{
	InitRule();
}
CRuleModelNetwork::~CRuleModelNetwork()
{
	ClearRule();
}
bool CRuleModelNetwork::IsNetworkLink( LPCTSTR lpcszName ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( 0 == it->first.Compare( lpcszName ) )
			return true;
	}
	return false;
}
bool CRuleModelNetwork::IsNetworkNode( LPCTSTR lpcszName ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		if( 0 == it->second.Compare( lpcszName ) )
			return true;
	}
	return false;
}
CString CRuleModelNetwork::GetPairName( LPCTSTR lpcszName ) const
{
	for( const_iterator it = m_cRule.begin(); it != m_cRule.end(); ++it )
	{
		if( 0 == it->first.Compare( lpcszName ) )
			return it->second;
		else if( 0 == it->second.Compare( lpcszName ) )
			return it->first;
	}
	return NULL;
}
void CRuleModelNetwork::InitRule()
{
	m_cRule.push_back( std::make_pair( road_link::kTableName, road_node::kTableName ) );
	m_cRule.push_back( std::make_pair( walk_link::kTableName, walk_node::kTableName ) );
	m_cRule.push_back( std::make_pair( sj::subway_link::kTableName, sj::subway_node::kTableName ) );
	m_cRule.push_back( std::make_pair( sj::railway_link::kTableName, sj::railway_node::kTableName ) );
	// [ADAM] 上下線モデル、車線モデル、区画線モデル、道路境界線モデルのNWを定義
	m_cRule.push_back( std::make_pair( adam_v2::lane_link::kTableName, adam_v2::lane_node::kTableName ) );
	m_cRule.push_back( std::make_pair( adam_v2::updown_link::kTableName, adam_v2::updown_node::kTableName ) );
	m_cRule.push_back( std::make_pair( adam_v2::compart_link::kTableName, adam_v2::compart_node::kTableName ) );
	m_cRule.push_back( std::make_pair( adam_v2::border_link::kTableName, adam_v2::border_node::kTableName ) );
}
void CRuleModelNetwork::ClearRule()
{
	m_cRule.clear();
}

// CRuleRelationshipContain
CRuleRelationshipContain::CRuleRelationshipContain()
{
	InitRule();
}
CRuleRelationshipContain::~CRuleRelationshipContain()
{
	ClearRule();
}
CRuleRelationshipContain::iterator CRuleRelationshipContain::find( LPCTSTR lpcszTableName )
{
	iterator it = m_cRule.find(lpcszTableName);
	if( it != m_cRule.end() )
	{
		// キーだけできる事がある（value_iteratorを返す場合）
		// があるので、本当にルールが存在するかどうか確認
		// する必要がある
		if( it->second.empty() )
			return end();
	}
	return it;
}
CRuleRelationshipContain::iterator CRuleRelationshipContain::find_value( LPCTSTR lpcszTableName )
{
	for( iterator it = begin(); it != end(); ++it )
	{
		for( value_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == it2->first.CompareNoCase( lpcszTableName ) )
				return it;
		}
	}
	return end();
}
// 「含む」側テーブル名をキーとして検索
CRuleRelationshipContain::const_iterator CRuleRelationshipContain::find( LPCTSTR lpcszTableName ) const
{
	const_iterator it = m_cRule.find(lpcszTableName);
	if( it != m_cRule.end() )
	{
		// キーだけできる事がある（value_iteratorを返す場合）
		// があるので、本当にルールが存在するかどうか確認
		// する必要がある
		if( it->second.empty() )
			return end();
	}
	return it;
}
// 「含まれる」側テーブル名をキーとして検索
CRuleRelationshipContain::const_iterator CRuleRelationshipContain::find_value( LPCTSTR lpcszTableName ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		for( const_value_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
		{
			if( 0 == it2->first.CompareNoCase( lpcszTableName ) )
				return it;
		}
	}
	return end();
}
void CRuleRelationshipContain::InitRule()
{
	// 道路ネットワーク
	m_cRule[_T("ROAD_LINK")].push_back( std::pair<CString,CString>(_T("SEGMENT_ATTR"), _T("LINK_ID")) );
	m_cRule[_T("ROAD_LINK")].push_back( std::pair<CString,CString>(_T("GRAD"),         _T("LINK_ID")) );
//		// 歩行者ネットワーク
//		m_cRule[_T("WALKING_LINK")].push_back(_T("WALKING_ATTR"));
	// 家形と段差
	m_cRule[_T("BUILDING")].push_back( std::pair<CString,CString>(_T("BUILDING_STEP"), _T("")) );
}
void CRuleRelationshipContain::ClearRule()
{
	m_cRule.clear();
}

// CRuleRelationTable
CRuleRelationTable::CRuleRelationTable()
{
	InitRule();
}
CRuleRelationTable::~CRuleRelationTable()
{
	ClearRule();
}
void CRuleRelationTable::InitRule()
{
	using namespace sindy::schema;
	// 道路リンク
	m_cRule[ road_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(road_link::kOnewayRF, time_oneway::kTableName, time_oneway::kLinkID ));
	m_cRule[ road_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(road_link::kNoPassageRF,time_nopassage::kTableName, time_nopassage::kLinkID));
	m_cRule[ road_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(road_link::kVehicleRegRF,vehicle_reg::kTableName, vehicle_reg::kLinkID));
	m_cRule[ vehicle_reg::kTableName ].push_back(boost::tuple<CString,CString,CString>(_T(""), vehicle_type::kTableName, vehicle_type::kRegID));
	// 歩道リンク
	m_cRule[ walk_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(walk_link::kOnewayRF, walk_oneway::kTableName, walk_oneway::kLinkID));
	m_cRule[ walk_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(walk_link::kNoPassageRF, walk_nopassage::kTableName, walk_nopassage::kLinkID));
	m_cRule[ walk_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(walk_link::kLinkRelationRF, link_relation::kTableName, link_relation::kBaseLinkID));
	// 注記
	m_cRule[_T("CITY_ANNOTATION")  ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("CITY_DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("D1_ANNOTATION")    ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("C_SC1DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("BASE_ANNOTATION")  ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("B_SC4DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("BASE_ANNOTATION")  ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("B_SC3DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("BASE_ANNOTATION")  ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("B_SC2DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("BASE_ANNOTATION")  ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("B_SC1DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("MIDDLE_ANNOTATION")].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("M_SC4DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("MIDDLE_ANNOTATION")].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("M_SC3DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("MIDDLE_ANNOTATION")].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("M_SC2DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("MIDDLE_ANNOTATION")].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("M_SC1DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("TOP_ANNOTATION")   ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("T_SC4DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("TOP_ANNOTATION")   ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("T_SC3DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("TOP_ANNOTATION")   ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("T_SC2DISP_LINE"), _T("ANNO_ID")));
	m_cRule[_T("TOP_ANNOTATION")   ].push_back(boost::tuple<CString,CString,CString>(_T(""),_T("T_SC1DISP_LINE"), _T("ANNO_ID")));
	// 鉄道リンク
	m_cRule[ sj::railway_link::kTableName ].push_back(boost::tuple<CString,CString,CString>(_T(""), sj::rel_railway_link::kTableName, sj::rel_railway_link::kRwLinkID));
	// 鉄道ノード
	m_cRule[ sj::railway_node::kTableName ].push_back(boost::tuple<CString,CString,CString>(_T(""), sj::rel_railway_node::kTableName, sj::rel_railway_node::kRwNodeID));
}
void CRuleRelationTable::ClearRule()
{
	m_cRule.clear();
}

// CRuleLinkQueue
CRuleLinkQueue::CRuleLinkQueue()
{
	InitRule();
}
CRuleLinkQueue::~CRuleLinkQueue()
{
	ClearRule();
}
void CRuleLinkQueue::InitRule()
{
	// 道路ネットワーク用
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_BYWAY")),     CString(_T("LQ_BYWAY")),     CString(_T("BYWAY_LQRF")),     CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_DIRGUIDE")),  CString(_T("LQ_DIRGUIDE")),  CString(_T("DIRGUIDE_LQRF")),  CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_GUIDE")),     CString(_T("LQ_GUIDE")),     CString(_T("GUIDE_LQRF")),     CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_LANE")),      CString(_T("LQ_LANE")),      CString(_T("LANE_LQRF")),      CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_VICS")),      CString(_T("LQ_VICS")),      CString(_T("VICS_LQRF")),      CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_TURNREG")),   CString(_T("LQ_TURNREG")),   CString(_T("TURNREG_LQRF")),   CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_ROUTE")),     CString(_T("LQ_ROUTE")),     CString(_T("ROUTE_LQRF")),     CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_UTURN")),     CString(_T("LQ_UTURN")),     CString(_T("UTURN_LQRF")),     CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_INTERNAVI")), CString(_T("LQ_INTERNAVI")), CString(_T("INTERNAVI_LQRF")), CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_IPCVICS")),   CString(_T("LQ_IPCVICS")),   CString(_T("IPCVICS_LQRF")),   CString(_T("LINK_ID")) ) );
	m_cRule[_T("ROAD_LINK")].push_back( boost::tuples::make_tuple( CString(_T("INF_INTERSECTION")),   CString(_T("LQ_INTERSECTION")),   CString(_T("INTERSECTION_LQRF")),   CString(_T("LINK_ID")) ) );
}
void CRuleLinkQueue::ClearRule()
{
	m_cRule.clear();
}
