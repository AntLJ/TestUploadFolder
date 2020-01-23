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
#include <boost/utility.hpp>
#include <CodeanAlysis/SourceAnnotations.h>
#include "RegionProperty.h"

/**
 * @brief 座標からリージョンオブジェクト識別情報を管理するクラス
 *
 * @attention
 * このクラスのインスタンスはスレッド安全である。
 */
class RegionPropertyManager : boost::noncopyable
{
	class Impl;
public:
	RegionPropertyManager(
		IFeatureClass* ipInnerClass,
		IFeatureClass* ipOuterClass,
		[SA_Pre(NullTerminated=SA_No)] LPCWSTR lpszRoadLinkWhereClause
	);
	~RegionPropertyManager();

	boost::shared_ptr<RegionProperty> getRegionProperty([SA_Pre(Null=SA_No)] IPoint* ipPoint);

private:
	boost::scoped_ptr<Impl> pimpl;
};
