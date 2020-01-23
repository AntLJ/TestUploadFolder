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

// Node.cpp: CNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoadNode.h"
#include "meshutil.h"
#include "ErrorObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace sindy {
namespace road_node {
using namespace errorcode;

// 整合性をチェックする
errorcode::sindyErrCode CRoadNode::CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const
{
	// 基本チェック
	CFeature::CheckLogic( cRule, cErrs );

	CErrorInfos cErrInfos;
	// ノードの位置の妥当性チェック（二次メッシュ区画線との整合）
	int ix = 0, iy = 0;
	CGeometry::const_point_iterator it = GetShape()->point_begin();
	LL2MESH( it.get_x(), it.get_y(), 2, ix, iy );

	// ノードが4隅にないかどうか
	if( (0 == ix || 1000000 == ix) && (0 == iy || 1000000 == iy) )
		cErrInfos.push_back( CErrorInfo( sindyErr_NodeExist4MeshEdge, sindyErrLevel_ERR ) );

	// 道路、歩行者のノード種別名（同じだけど、、）
	std::map<CString, CString> nodeClassNames = boost::assign::map_list_of
		(CString(schema::road_node::kTableName), CString(schema::road_node::kNodeClass))
		(CString(schema::walk_node::kTableName), CString(schema::walk_node::kNodeClass));

	// 各ノードの2次メッシュ上ノード種別
	std::map< CString, std::set<long> > onMeshNodeClass = boost::assign::map_list_of
		(CString(schema::road_node::kTableName), boost::assign::list_of          // 道路
			((long)schema::road_node::node_class::kEdge)
			((long)schema::road_node::node_class::kPecEdge))

		(CString(schema::walk_node::kTableName), boost::assign::list_of          // 歩行者
			((long)schema::walk_node::node_class::kOutline));

	// ノード種別
	long nodeClass = GetLongValueByFieldName(nodeClassNames[GetTableName()], -1);

	// 各種のノード種別でメッシュ上チェック（bug 12213）
	if (onMeshNodeClass[GetTableName()].count(nodeClass) > 0)
	{
		// メッシュ区画線上にあるかどうか
		if ((0 != ix && 1000000 != ix) && (0 != iy && 1000000 != iy))
			cErrInfos.push_back(CErrorInfo(sindyErr_NodeOfMeshEdgeNotExistMeshEdge, sindyErrLevel_ERR));
	}
	else
	{
		// メッシュ区画線上にないかどうか
		if ((0 == ix || 1000000 == ix) || (0 == iy || 1000000 == iy))
			cErrInfos.push_back(CErrorInfo(sindyErr_NodeOfMeshEdgeNotExistMeshEdge, sindyErrLevel_ERR));
	}

	if( sindyErr_NoErr != cErrInfos.GetDefaultErrCode() )
	{
		CErrorObject cErr(*this); // TODO: cErrs.push_back( *this, cErrorInfos ) とかできたほうが良い
		cErr.push_back( cErrInfos );
		cErrs.push_back( cErr );
	}
	return cErrInfos.GetDefaultErrCode();
}

// 正しい種別が設定されているか
errorcode::sindyErrCode CRoadNode::CheckNodeClass() const
{
	sindyErrCode emErr = sindyErr_NoErr;	// 返り値
/*	/// ノード接する道路リンクを取得
	int nCount = 0;//GetTouchedRoadLink();
	
	node_class::ECode eNode = GetNodeClass_C();

	if ( 1 == nCount && eNode != node_class::kTerminal ) {
		/// 道路が一本しかないのに端点ノードじゃない
		return errorcode::sindyErr_InvalidNodeClass;
	}

	if ( 3 >= nCount && eNode != node_class::kCross ) {
		/// 三本以上の道路が接してるのに交差点じゃない
		return errorcode::sindyErr_InvalidNodeClass;
	}

	return errorcode::sindyErr_NoErr;*/

	emErr = sindyErr_NoImplimentation;
	return emErr;
}

bool CRoadNode::IsExistAttrNode( bool bCheckAll/* = true*/ ) const
{
	// 交差点フラグが付いているかどうか
	if ( GetSignalCode() == 1 )
		 return true;

	// 交差点名称があるかどうか
	TCHAR lpcszNameKanji[2000];

	if ( GetNameKanji( 2000, lpcszNameKanji ) && lstrcmpi( _T(""), lpcszNameKanji ) != 0 )
		return true;

	// 交差点かな名称があるかどうか
	TCHAR lpcszNameKana[2000];

	if ( GetNameYomi( 2000, lpcszNameKana ) && lstrcmpi( _T(""), lpcszNameKana ) != 0 )
		return true;

	if ( bCheckAll ) {
		// 高さコードが付いているかどうか
		if ( GetHeightCode() != 0 )
			return true;

		// 高さデータが入っているかどうか
		if ( GetHeight() != 0 )
			return true;
	}

	return false;
}

bool CRoadNode::IsBorderNode() const
{
	bool bRet = false;
	long nNodeClass = GetLongValueByFieldName( schema::road_node::kNodeClass, -1 );

	if ( nNodeClass == schema::road_node::node_class::kEdge || 
		nNodeClass == schema::road_node::node_class::kPecEdge || 
		nNodeClass == schema::walk_node::node_class::kOutline )
		bRet = true;

	return bRet;
}

} // road_node

} // sindy
