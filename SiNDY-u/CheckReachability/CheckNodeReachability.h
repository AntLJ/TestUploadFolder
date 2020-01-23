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

#pragma once

#include <utility>
#include <boost/shared_ptr.hpp>
#include "RegionPyramidCreator.h"

typedef boost::shared_ptr<RegionPyramidCreator> RegionPyramidCreatorRef;

/**
 * @brief 指定したノードが最上位の道路リンクに／から辿り付けるかを検査するクラス
 */
class CheckNodeReachability
{
public:
	CheckNodeReachability([SA_Pre(Null=SA_No)] IFeatureClass* ipRoadNodeClass, RegionPyramidCreatorRef pRgnPyramidCreator);

	std::pair<bool, bool> check(long nNodeID);

private:
	const IFeatureClassPtr m_ipRoadNodeClass;
	const RegionPyramidCreatorRef m_pRgnPyramidCreator;
};
