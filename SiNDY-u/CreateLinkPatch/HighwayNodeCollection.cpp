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
 *	@file HighwayNodeCollection.cpp
 *	@brief ハイウェイノード集合クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/21		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "HighwayNodeCollection.h"
#include "GlobalFunction.h"
#include "LogFile.h"

const std::string HighwayNodeCollection::m_FeatureClassName = "Highway_Node";


bool point_less::operator()( IPoint* lhs, IPoint* rhs ) const
{
	_ASSERT(lhs && rhs);
	int aResult;
	lhs->Compare( rhs, &aResult );
	return aResult<0;
}

HighwayNodeCollection::HighwayNodeCollection(void)
{
}

HighwayNodeCollection::~HighwayNodeCollection()
{
}

/**
 *	@brief	読み込み
 *	@note	全てのHighway_Nodeデータを読み込む
 **/
void HighwayNodeCollection::read( IWorkspace* cpWorkspace )
{
	std::string aFunc = "HighwayNodeCollection::read()";

	if (cpWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : NULL" ); }

	// 対象フィーチャクラスを開く
	IFeatureClassPtr	ipFeatureClass;
	IFeatureWorkspacePtr	ipFeatureWorkspace( cpWorkspace );
	if (ipFeatureWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : ワークスペース作成失敗" ); }
	if (ipFeatureWorkspace->OpenFeatureClass( CComBSTR(_T( m_FeatureClassName.c_str() )), &ipFeatureClass ) != S_OK ) {
		throw std::runtime_error( "R1F:" + aFunc + " : " + m_FeatureClassName + "フィーチャクラスのオープンに失敗" );
	}

	// SQLフィルタ設定
	std::string aSubFieldStr( "OBJECTID, ROAD_CODE, TOLLCLASS_C, SHAPE" );
	IQueryFilterPtr	ipQueryFilter( CLSID_QueryFilter );
	ipQueryFilter->put_SubFields( CComBSTR(aSubFieldStr.c_str()) );	

	// フィルタリング
	IFeatureCursorPtr ipFeatureCursor;
	if (ipFeatureClass->Search( ipQueryFilter, FALSE, &ipFeatureCursor ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : Search()失敗" ); }

	IFeaturePtr	ipFeature;
	int aObjectCount = 0;
	while ((ipFeatureCursor->NextFeature( &ipFeature )==S_OK) && (ipFeature != NULL)) {

		// 座標(IPoint)を求める
		IGeometryPtr ipGeometry;
		if (ipFeature->get_Shape( &ipGeometry ) != S_OK) { throw std::runtime_error( "R1F:" + aFunc + " : get_Shape()失敗" ); }
		IPointPtr ipPoint = ipGeometry;	// ダウンキャスト(?)

		// 格納
		assign( ipPoint, ipFeature );

		++aObjectCount;

#ifdef	_DEBUG
		std::cout << m_FeatureClassName.c_str() << "オブジェクトを取得中 : " << aObjectCount << "\r";
#endif
	}

#ifdef	_DEBUG
	std::cout << std::endl;
#endif
}


/**
 *	@brief	格納
 *	@param	cpPoint	[in]	座標オブジェクトへのポインタ
 *	@param	cpFeature	[in]	対象フィーチャへのポインタ
 **/
void HighwayNodeCollection::assign( IPoint* cpPoint, IFeature* cpFeature )
{
	LogFile* pLog = LogFile::Instance();

	std::string aFunc = "HighwayNodeCollection::assign()";

	HighwayNode aHighwayNode;
	aHighwayNode.m_ObjectID = GlobalFunction::getValue( cpFeature, _T("OBJECTID") ).lVal;
	aHighwayNode.m_RoadCode = GlobalFunction::getValue( cpFeature, _T("ROAD_CODE") ).lVal;
	aHighwayNode.m_TollClass = GlobalFunction::getValue( cpFeature, _T("TOLLCLASS_C") ).lVal;

	m_Data.insert( std::pair<IPointPtr, HighwayNode>(cpPoint, aHighwayNode) );
}


/**
 *	@brief	HighwayNodeの取得
 *	@note	指定された座標に存在するHighwayNodeオブジェクトへのアドレス(const)を返す
 *	@param	cpPoint	[in]	取得するHighwayNodeを持つHighwayNodeの座標
 *	@return	NULL	指定された座標にHighwayNodeが存在しない
 **/
void HighwayNodeCollection::getHighwayNode( IPoint* cpPoint, std::set<const HighwayNode*>* cpHighwayNodeArray ) const
{
	std::string aFunc = "HighwayNodeCollection::getHighwayNode()";

	if (cpPoint == NULL) { throw std::runtime_error( aFunc + " : NULL" ); }

	Data::const_iterator aIter = m_Data.find( cpPoint );
	if (aIter != m_Data.end()) {
		for ( ; aIter != m_Data.upper_bound( cpPoint ); ++aIter) {
			cpHighwayNodeArray->insert( &aIter->second );
			long aObjectID = aIter->second.m_ObjectID;
			long aRoadCode = aIter->second.m_RoadCode;
			long aTollClass = aIter->second.m_TollClass;
		}
	}
}


/**
 *	@brief	全データ表示
 *	@param	crStream	[in]	出力ストリーム
 **/
void HighwayNodeCollection::disp( std::ostream& crStream ) const
{
	crStream << "FLAG	LAYER	OBJECTID	ROAD_CODE	TOLLCLASS_C" << std::endl;

	for (Data::const_iterator aIter = m_Data.begin(); aIter != m_Data.end(); ++aIter ) {
		const HighwayNode* pHighwayNode = &aIter->second;
		crStream << 0 << "\t";
		crStream << "HIGHWAY_NODE" << "\t";
		crStream << pHighwayNode->m_ObjectID << "\t";
		crStream << pHighwayNode->m_RoadCode << "\t";
		crStream << pHighwayNode->m_TollClass << std::endl;
	}
}

