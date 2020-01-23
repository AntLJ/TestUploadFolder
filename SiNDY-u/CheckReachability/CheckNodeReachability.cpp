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
#include <sstream>
#include <rns/algorithm.h>
#include <atl2/io.h>
#include "CheckUpperReachability.h"
#include "CheckNodeReachability.h"

/**
 * @param ipRoadNodeClass [in] 道路ノードフィーチャクラス。
 * @param pRgnPyramidCreator [in] リージョン階層作成オブジェクト。
 */
CheckNodeReachability::CheckNodeReachability(IFeatureClass* ipRoadNodeClass, RegionPyramidCreatorRef pRgnPyramidCreator) :
m_ipRoadNodeClass(ipRoadNodeClass),
m_pRgnPyramidCreator(pRgnPyramidCreator)
{
}

/**
 * @brief 指定したノードが最上位の道路リンクに／から辿り付けるかを検査する。
 *
 * @param nNodeID [in] 検査対象ノードID。
 * @return 最上位の道路リンクに／からたどり着けるか。
 */
std::pair<bool, bool> CheckNodeReachability::check(long nNodeID)
{
	// 道路ノードIDをキーとして道路ノードを検索。
	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));

	ipQueryFilter->SubFields = m_ipRoadNodeClass->ShapeFieldName;

	std::basic_ostringstream<TCHAR> oss;
	oss << m_ipRoadNodeClass->OIDFieldName << _T('=') << nNodeID;
	atl2::valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

	const IFeatureCursorPtr ipFeatureCursor(m_ipRoadNodeClass->_Search(ipQueryFilter, VARIANT_TRUE));

	// １つも見つからなかったり、２つ以上見つかったらおかしいので、とりあえず「非到達」とする。
	IFeaturePtr ipRoadNode, ipRoadNodeDummy;
	if(ipFeatureCursor->NextFeature(&ipRoadNode) != S_OK || ipFeatureCursor->NextFeature(&ipRoadNodeDummy) == S_OK)
		return std::make_pair(false, false);

	// 道路ノードが属するリージョンピラミッドを求める。
	const std::auto_ptr<RegionPyramid> pRegPyramid(m_pRgnPyramidCreator->create(IPointPtr(ipRoadNode->Shape)));
	if(! pRegPyramid.get())
		return std::make_pair(false, false);
	
	// 到達性を検査する。
	return 
		std::make_pair(
			CheckUpperReachability(pRegPyramid->begin(), pRegPyramid->end(), false).checkByNodeID(nNodeID),
			CheckUpperReachability(pRegPyramid->begin(), pRegPyramid->end(), true).checkByNodeID(nNodeID)
		);
}
