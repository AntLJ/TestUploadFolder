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

#include <boost/ptr_container/ptr_vector.hpp>
#include <CodeanAlysis/SourceAnnotations.h>
#include "RegionManager.h"
#include "RegionPropertyManager.h"
#include "RegionPyramid.h"

typedef boost::shared_ptr<RegionManager> RegionManagerRef;
typedef boost::ptr_vector<RegionPropertyManager> RegionPropertyManagerPyramid;
typedef boost::shared_ptr<RegionPropertyManagerPyramid> RegionPropertyManagerPyramidRef;

/**
 * @note
 * このクラスのインスタンスはスレッド安全である。
 */
class RegionPyramidCreator
{
public:
	RegionPyramidCreator(RegionManagerRef pRegMng, RegionPropertyManagerPyramidRef pRegPropMngPyramid);

	std::auto_ptr<RegionPyramid> create([SA_Pre(Null=SA_No)] IPoint* ipPoint);

private:
	RegionManagerRef m_pRegMng;
	RegionPropertyManagerPyramidRef m_pRPMP;
};
