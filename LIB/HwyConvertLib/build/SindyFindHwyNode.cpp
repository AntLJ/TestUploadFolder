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

// SindyFindHwyNode.cpp: SindyFindHwyNode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyFindHwyNode.h"


SindyFindHwyNode::SindyFindHwyNode(SindyHighwayInfo *cHwyInfo) :
SindyHighwayTrace(),
mpHwyInfo(cHwyInfo),
mRoadCode(0),
mRoadSeq(0)
{
	set_direction(sindy::rnw::relation::back);
}

SindyFindHwyNode::~SindyFindHwyNode()
{

}

HRESULT SindyFindHwyNode::exit(SindyRoadInfo *cRoadInfo, IFeaturePtr ipNode, bool *cExit)
{
	HRESULT hr;
	const char *func = "SindyFindHwyNode::exit";

	*cExit = false;

	IGeometryPtr ipGeometry = ipNode->GetShapeCopy();

	// ハイウェイノードの検索
	GDBPNTRANGE aRange;
	if (FAILED(hr = mpHwyInfo->eHighwayNode.SearchByPoint((IPointPtr)ipGeometry, &aRange))) return hr;

	for (GDBPNTITER aIter = aRange.first; aIter != aRange.second; aIter++)
	{
		IFeaturePtr ipHwyNode = aIter->second;

		// 分岐ポイントでなければならない
		CComVariant vaInOut;
		if (FAILED(hr = GDBGetValue(ipHwyNode, sindy::highway_node::INOUT_C, &vaInOut))) return hr;
		vaInOut.ChangeType(VT_I4);
		if (vaInOut.lVal != sindy::highway_node::inout_exit) continue;

		// 路線コードと路線内シーケンスのチェック
		// 路線コードまたは路線内シーケンスのどちらかがセットされていたら、
		// チェックに使用するためハイウェイノードの路線コード・路線内シーケンスを取得
		long aRoadCode = 0, aRoadSeq = 0;
		if (mRoadCode != 0 || mRoadSeq != 0)
		{
			if (FAILED(hr = mpHwyInfo->GetFacilCode(ipHwyNode, &aRoadCode, &aRoadSeq))) return hr;
		}
		bool aResult = true;
		// 路線コードがセットされていれば、ハイウェイノードの路線コードはそれと一致しなければならない
		if (mRoadCode != 0)
		{
			if (aRoadCode != mRoadCode)
			{
				aResult = false;
			}
		}
		// 路線内シーケンスがセットされていれば、ハイウェイノードの路線内シーケンスはそれと一致しなければならない
		if (mRoadSeq != 0)
		{
			if (aRoadSeq != mRoadSeq)
			{
				aResult = false;
			}
		}

		// 条件を満たすハイウェイノードが見つかったら記憶
		if (aResult)
		{
			mFoundHwyNodeArray.push_back(ipHwyNode);
//			mHwyNode = ipHwyNode;
//			*cExit = true;
//			return S_OK;
		}
	}

	return S_OK;
}
