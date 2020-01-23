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

// LinksCache.h: LinksCache クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Link.h"

namespace sindy
{
typedef std::map<long, Link> LinkMap;

/**
 * @brief リンクほかキャッシュクラス
 */
class LinksCache  
{
public:
// 構築/消滅
	LinksCache();
	virtual ~LinksCache();

	void setLinkClass(const IFeatureClassPtr& ipClass);

// 処理
	bool init_cache();
	bool add_cache(long nOID, const _IRowPtr& ipRow);
	void clearCache();

// 検索
	const Link* findLink(const IPointPtr& ipPoint, double dTolerance) const;
	int findLinkId(const IPointPtr& ipPoint, double dTolerance) const;

// 判定
	bool cacheAreaIsContain(const IGeometryPtr& ipGeometry);

// プロパティ
	LinkMap m_cLinkMap; ///< リンクキャッシュ

	IEnvelopePtr m_ipEnvelope; ///< キャッシュ矩形

private:
	IFeatureClassPtr m_ipLinkClass;

	IFeatureClassPtr m_ipCachedLinkClass; ///< キャッシュした道路リンクフィーチャクラス
};

}
