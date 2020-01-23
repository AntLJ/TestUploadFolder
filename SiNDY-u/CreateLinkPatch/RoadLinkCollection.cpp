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
 *	@file RoadLinkCollection.cpp
 *	@brief 道路リンク集合クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/21		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "RoadLinkCollection.h"
#include "GlobalFunction.h"
#include <map>
#include <sstream>
#include "crd_cnv.h"


RoadLink::RoadLink(void) : 
	m_RoadCode(0),
	m_RoadClass(0),
	m_NaviClass(0),
	m_UpDownClass(0),
	m_MainLinkClass(0),
	m_FromNodeID(0),
	m_ToNodeID(0),
	m_pShape(NULL)
{
}

RoadLink::~RoadLink()
{
}



const std::string RoadLinkCollection::m_FeatureClassName = "Road_Link";

RoadLinkCollection::RoadLinkCollection(void)
{
}
RoadLinkCollection::~RoadLinkCollection()
{
}


/**
 *	@brief	読み込み
 *	@note	指定されたWhereClauseとSubFieldに従ってRoad_Linkデータを読み込む
 *	@param	cpWorkspace	対象ワークスペースへのポインタ
 *	@param	cWhereClauseStr	WHERE句文字列
 *	@param	cSubFieldStr	SubField指定文字列
 **/
void RoadLinkCollection::read( IWorkspace* cpWorkspace, std::string cWhereClauseStr, std::string cSubFieldStr )
{
	std::string aFunc = "RoadLinkCollection::read()";

	if (cpWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

	// 対象フィーチャクラスを開く
	IFeatureClassPtr	ipFeatureClass;
	IFeatureWorkspacePtr	ipFeatureWorkspace( cpWorkspace );
	if (ipFeatureWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + "ワークスペース取得失敗" ); }
	if (ipFeatureWorkspace->OpenFeatureClass( CComBSTR(_T( m_FeatureClassName.c_str() )), &ipFeatureClass ) != S_OK ) {
		throw std::runtime_error( "R1F:" + aFunc + m_FeatureClassName + "フィーチャクラスのオープンに失敗" );
	}

	// SQLフィルタ設定
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_WhereClause( CComBSTR(cWhereClauseStr.c_str()) );
	ipQueryFilter->put_SubFields( CComBSTR(cSubFieldStr.c_str()) );

	// フィルタリング
	IFeatureCursorPtr ipFeatureCursor;
	if (ipFeatureClass->Search( ipQueryFilter, FALSE, &ipFeatureCursor ) != S_OK) {
		throw std::runtime_error( "R1F:" + aFunc + " : Search()失敗" );
	}

	IFeaturePtr	ipFeature;
	int aObjectCount = 0;
	while ((ipFeatureCursor->NextFeature( &ipFeature )==S_OK) && (ipFeature != NULL)) {
		++aObjectCount;
#ifdef	_DEBUG
		std::cout << m_FeatureClassName.c_str() << "オブジェクトを取得中 : " << aObjectCount << "\r";
#endif
		// 格納
		assign( ipFeature );
	}
#ifdef	_DEBUG
	std::cout << std::endl;
#endif
}



/**
 *	@brief	格納
 *	@param	cpFeature	[in]	対象フィーチャへのポインタ
 **/
void RoadLinkCollection::assign( IFeature* cpFeature )
{
	std::string aFunc = "RoadLinkCollection::assign()";

	RoadLink aRoadLink;
	long aObjectID = GlobalFunction::getValue( cpFeature, _T("OBJECTID") ).lVal;
	aRoadLink.m_RoadCode = GlobalFunction::getValue( cpFeature, _T("ROAD_CODE") ).lVal;
	aRoadLink.m_RoadClass = GlobalFunction::getValue( cpFeature, _T("ROADCLASS_C") ).lVal;
	aRoadLink.m_NaviClass = GlobalFunction::getValue( cpFeature, _T("NAVICLASS_C") ).lVal;
	aRoadLink.m_UpDownClass = GlobalFunction::getValue( cpFeature, _T("UPDOWNCLASS_C") ).lVal;
	aRoadLink.m_MainLinkClass = GlobalFunction::getValue( cpFeature, _T("MAIN_LINKCLASS_C") ).lVal;
	aRoadLink.m_FromNodeID = GlobalFunction::getValue( cpFeature, _T("FROM_NODE_ID") ).lVal;
	aRoadLink.m_ToNodeID = GlobalFunction::getValue( cpFeature, _T("TO_NODE_ID") ).lVal;

	IGeometryPtr pGeometry;
	if (cpFeature->get_Shape( &pGeometry ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : get_Shape()失敗" ); }
	aRoadLink.m_pShape = pGeometry;

	m_Data.insert( std::pair<long, RoadLink>(aObjectID, aRoadLink) );
}


/**
 *	@brief	全リンクの情報を表示
 *	@param	crStream	[out]	出力ストリーム
 **/
void RoadLinkCollection::disp( std::ostream& crStream ) const
{
//	for (std::set<RoadLink>::const_iterator aIter = m_Data.begin(); aIter != m_Data.end(); ++aIter ) {
	for (RoadLinkIter aIter = m_Data.begin(); aIter != m_Data.end(); ++aIter ) {
		crStream << "0" << "\t";
		crStream << m_FeatureClassName << "\t";
		crStream << aIter->first << "\t";
		crStream << aIter->second.m_RoadCode << "\t";
		crStream << aIter->second.m_RoadClass << "\t";
		crStream << aIter->second.m_NaviClass << "\t";
		crStream << aIter->second.m_UpDownClass << "\t";
		crStream << aIter->second.m_MainLinkClass << "\t";
		crStream << aIter->second.m_FromNodeID << "\t";
		crStream << aIter->second.m_ToNodeID << "\t";
		crStream << "";
		crStream << std::endl;
	}
}


/**
 *	@brief	指定されたオブジェクトIDを持つRoadLinkオブジェクトを返す
 *	@param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 **/
RoadLink RoadLinkCollection::getRoadLink( long cObjectID ) const
{
	RoadLinkIter aIter = m_Data.find( cObjectID );
	_ASSERT( aIter != m_Data.end() );
	return aIter->second;
}


/**
 *	@brief	指定されたオブジェクトIDを持つRoadLinkオブジェクトが存在する２次メッシュコードを返す
 *	@param	cObjectID	[in]	対象RoadLinkオブジェクトのオブジェクトID
 *	@return	対象Road_Linkオブジェクトが存在する２次メッシュコード
 **/
int RoadLinkCollection::getMeshCode( long cObjectID ) const
{
	bool aResult = false;
	RoadLink aRoadLink = getRoadLink( cObjectID );

	// ２点を結ぶリンク上の中心点のメッシュコードを調べる
	IPolylinePtr pPolyline( aRoadLink.m_pShape );
	IPointPtr ipPoint(CLSID_Point);
	pPolyline->QueryPoint( esriNoExtension, 0.5, VARIANT_TRUE, ipPoint );
	
	double aMidPointLon, aMidPointLat;
	ipPoint->get_X( &aMidPointLon );
	ipPoint->get_Y( &aMidPointLat );

	crd_cnv aCrd_cnv;
	int aMidX, aMidY;	// 正規化座標
	int aMeshCode;
	aCrd_cnv.LLtoMesh( aMidPointLon, aMidPointLat, 2, &aMeshCode, &aMidX, &aMidY, 0);

	return aMeshCode;
}

