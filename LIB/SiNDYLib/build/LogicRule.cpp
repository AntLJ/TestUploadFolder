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

/**
 * @file LogicRule.h
 * @brief 形状ルール定義ファイル
 * @author 地図ＤＢ制作部システム開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "GeometryRule.h"
#include "TableTypeUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {

CLogicRule::CLogicRule(){ init(); }
CLogicRule::~CLogicRule(){}

void CLogicRule::init()
{
	m_dDefaultMinLen = 0.005; // 5mm
	m_dDefaultMinDeg = 20.0;
	m_dDefaultMaxDeg = 340.0;
	m_pairDefaultRelThresholdPx.first = 0.0;
	m_pairDefaultRelThresholdPx.second = 0.0;
	m_bUseEditableRule = true;
	m_bDisableCheckToNoSubstance = false;
	m_bDisableCheckNoChange = true;
	m_bDoCheckModify = true;

	// ここは本当はルールから読み込むべき
	m_mapMinLen[schema::road_link::kTableName] = 0.21;	// 道路リンクは20cm未満はNG
	m_mapMinDeg[schema::city_site::kTableName] = 3;
//	m_mapMinDeg[schema::admin_poly::kTableName] = 10;
	m_mapMinDeg[schema::city_station::kTableName] = 20;
	m_mapMinDeg[schema::building::kTableName] = 5;
	m_mapMinDeg[schema::base_site::kTableName] = 10;
	m_mapMinDeg[schema::base_station::kTableName] = 20;
	m_mapMinDeg[schema::base_contour::kTableName] = 20;
	m_mapMaxDeg[schema::city_site::kTableName] = 357;
//	m_mapMaxDeg[schema::admin_poly::kTableName] = 350;
	m_mapMaxDeg[schema::city_station::kTableName] = 340;
	m_mapMaxDeg[schema::building::kTableName] = 355;
	m_mapMaxDeg[schema::base_site::kTableName] = 350;
	m_mapMaxDeg[schema::base_station::kTableName] = 340;
	m_mapMaxDeg[schema::base_contour::kTableName] = 340;
}

double CLogicRule::GetMinLen( LPCTSTR lpcszTableName ) const
{
	std::map<CString,double>::const_iterator it = m_mapMinLen.find( lpcszTableName );
	return ( it != m_mapMinLen.end() ) ? it->second : m_dDefaultMinLen;
}

double CLogicRule::GetMinDeg( LPCTSTR lpcszTableName ) const
{
	std::map<CString,double>::const_iterator it = m_mapMinDeg.find( lpcszTableName );
	return ( it != m_mapMinDeg.end() ) ? it->second : m_dDefaultMinDeg;
}

double CLogicRule::GetMaxDeg( LPCTSTR lpcszTableName ) const
{
	std::map<CString,double>::const_iterator it = m_mapMaxDeg.find( lpcszTableName );
	return ( it != m_mapMaxDeg.end() ) ? it->second : m_dDefaultMaxDeg;
}

const std::pair<double,double>& CLogicRule::GetRelThresholdPx( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2 ) const
{
	std::map<CString, std::map<CString, std::pair<double,double> > >::const_iterator it1 = m_mapRelThresholdPx.find( lpcszTableName1 );
	if( it1 != m_mapRelThresholdPx.end() )
	{
		std::map<CString, std::pair<double,double> >::const_iterator it2 = it1->second.find( lpcszTableName2 );
		if( it2 != it1->second.end() )
			return it2->second;
	}
	return m_pairDefaultRelThresholdPx;
}
void CLogicRule::SetRelThresholdPx( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2, const std::pair<double,double>& pairThreshold )
{
	m_mapRelThresholdPx[lpcszTableName1][lpcszTableName2] = pairThreshold;
}

const CBasicLogicRule& CLogicRule::GetBasicRule( LPCTSTR lpcszTableName, LPCTSTR lpcszConnectUserName, CBasicLogicRule& cRule ) const
{
	cRule.m_dMinLen = GetMinLen( lpcszTableName );
	cRule.m_dMinDeg = GetMinDeg( lpcszTableName );
	cRule.m_dMaxDeg = GetMaxDeg( lpcszTableName );
	cRule.m_strTableName = lpcszTableName;
	cRule.m_strConnUserName = lpcszConnectUserName;

	switch( AheTableName2TableType( lpcszTableName ) )
	{
		case sindyTableType::road_link:
			cRule.m_bDoCheckRoop = true;
			cRule.m_bDoCheckOneselfTouches = true;
			break;
		default:
			cRule.m_bDoCheckRoop = false;
			cRule.m_bDoCheckOneselfTouches = false;
			break;
	}
	return cRule;
}

} // sindy
