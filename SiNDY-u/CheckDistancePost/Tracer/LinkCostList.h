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

// LinkCostList.h: LinkCostList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <map>

namespace sindy
{
class LinkCostList  
{
public:
// 定義
	typedef std::map<long, double> RoadClassCostMap;
	typedef std::map<long, double> NaviClassCostMap;
	typedef std::map<long, double> RoadWidthCostMap;
	typedef std::map<long, double> LinkClassCostMap;

// 構築/消滅
	LinkCostList();
	virtual ~LinkCostList();

// プロパティ
	static RoadClassCostMap m_cRoadClassCostMap;
	static NaviClassCostMap m_cNaviClassCostMap;
	static RoadWidthCostMap m_cRoadWidthCostMap;
	static LinkClassCostMap m_cLinkClassCostMap;
};

}

