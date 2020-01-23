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

// LinkCostList.cpp: LinkCostList クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkCostList.h"

namespace sindy
{
LinkCostList::RoadClassCostMap LinkCostList::m_cRoadClassCostMap;
LinkCostList::NaviClassCostMap LinkCostList::m_cNaviClassCostMap;
LinkCostList::RoadWidthCostMap LinkCostList::m_cRoadWidthCostMap;
LinkCostList::LinkClassCostMap LinkCostList::m_cLinkClassCostMap;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinkCostList::LinkCostList()
{

}

LinkCostList::~LinkCostList()
{

}

}
