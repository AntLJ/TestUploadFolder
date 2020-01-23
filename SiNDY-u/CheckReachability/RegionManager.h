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

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <CodeanAlysis/SourceAnnotations.h>
#include "RegionGraphData.h"
#include "Region.h"

typedef boost::shared_ptr<const SharingData> SharingDataRef;
typedef boost::shared_ptr<const Region> RegionRef;
typedef boost::shared_ptr<const RegionProperty> RegionPropertyRef;

/**
 * @brief リージョンオブジェクト識別情報からリージョンオブジェクトを作成し、保管するクラス
 *
 * @note
 * このクラスのインスタンスはスレッド安全である。
 */
class RegionManager
{
	class Impl;
public:
	RegionManager(
		[SA_Pre(Null=SA_No)] IFeatureClass* ipRoadLinkClass,
		[SA_Pre(Null=SA_No)] IFeatureClass* ipRoadNodeClass,
		SharingDataRef pSharingData
	);
	~RegionManager();

	RegionRef getRegion(RegionPropertyRef pRegProp);

	bool releaseLRURegion();

private:
	boost::scoped_ptr<Impl> pimpl;
};
