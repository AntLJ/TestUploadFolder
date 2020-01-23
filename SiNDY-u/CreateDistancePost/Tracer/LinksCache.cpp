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

// LinksCache.cpp: LinksCache クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "LinksCache.h"

using namespace sindy::schema;
namespace sindy
{

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinksCache::LinksCache()
{

}

LinksCache::~LinksCache()
{

}

//////////////////////////////////////////////////////////////////////
// クリア
//////////////////////////////////////////////////////////////////////

void LinksCache::clearCache()
{
	m_ipEnvelope = nullptr;
	m_ipCachedLinkClass = nullptr;
	m_cLinkMap.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定した道路リンクフィーチャクラスをキャッシュ用に登録する
 *
 * @param ipClass [in] 道路リンクフィーチャクラス
 */
void LinksCache::setLinkClass(const IFeatureClassPtr& ipClass)
{
	if(m_ipCachedLinkClass != ipClass) {
		clearCache();
		m_ipLinkClass = ipClass;
	}
}

//////////////////////////////////////////////////////////////////////
// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定した矩形内に存在するリンク、及びリンクに関連する右左折禁止をキャッシュする
 *
 * @note 指定した矩形が、前回キャッシュした矩形に内包される場合、既存のキャッシュを利用する。
 * @note 再キャッシュを強要したい場合、clearCache()したあとでcache()すれば良い。
 *
 * @param ipEnvelope [in] キャッシュ対象矩形
 * @param lpLinkWhereClause [in] リンク選定Where句
 * @retval true キャッシュに成功した。あるいは、既にキャッシュされている。
 * @retval false キャッシュに失敗した。
 */
bool LinksCache::init_cache()
{
	m_ipCachedLinkClass = m_ipLinkClass;

	return true;
}

bool LinksCache::add_cache(long nOID, const _IRowPtr& ipRow)
{
	m_cLinkMap[nOID] = Link(ipRow);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 検索
//////////////////////////////////////////////////////////////////////

/**
 * @brief キャッシュの中から、指定した座標の近くに存在するリンクを取得する
 *
 * @param ipPoint [in] 対象座標
 * @param dTolerance [in] 閾値
 * @return リンク。対象座標から閾値内にリンクが存在しなかった場合0。
 */
const Link* LinksCache::findLink(const IPointPtr& ipPoint, double dTolerance) const
{
	if(! ipPoint)
		return nullptr;

	double dDistance = 0;
	const Link* pResult = nullptr;

	LinkMap::const_iterator itr = m_cLinkMap.begin();
	if(itr == m_cLinkMap.end())
		return pResult;
	
	IClonePtr ipClone;
	IClonePtr(ipPoint)->Clone(&ipClone);
	IPointPtr ipClonePoint(ipClone);

	ISpatialReferencePtr ipSpRef;
	itr->second.m_ipGeometry->get_SpatialReference(&ipSpRef);
	ipClonePoint->putref_SpatialReference(ipSpRef);

	IProximityOperatorPtr ipProOp(ipClonePoint);

	for(; itr != m_cLinkMap.end(); ++itr) {
		const Link& rLink = itr->second;

		double dDist;
		if(FAILED(ipProOp->ReturnDistance(rLink.m_ipGeometry, &dDist)))
			continue;

		if(dDist <= dTolerance && (! pResult || dDist < dDistance)) {
			pResult = &rLink;
			dDistance = dDist;
		}
	}

	return pResult;
}

/**
 * @brief キャッシュの中から、指定した座標の近くに存在するリンクのIDを取得する
 *
 * @param ipPoint [in] 対象座標
 * @param dTolerance [in] 閾値
 * @return リンクID。対象座標から閾値内にリンクが存在しなかった場合0。
 */
int LinksCache::findLinkId(const IPointPtr& ipPoint, double dTolerance) const
{
	const Link* pLink = findLink(ipPoint, dTolerance);

	if(! pLink)
		return 0;

	return pLink->m_nObjectId;
}

//////////////////////////////////////////////////////////////////////
// 判定
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定した形状が、キャッシュしたエリア内に収まっているか
 *
 * @param ipGeometry [in] 検証形状
 * @retval true エリア内に存在する
 * @retval false エリア内に存在しない。あるいはキャッシュエリアが存在しない。
 */
bool LinksCache::cacheAreaIsContain(const IGeometryPtr& ipGeometry)
{
	if(! ipGeometry || ! m_ipEnvelope )
		return false;

	IRelationalOperatorPtr ipRelOp(m_ipEnvelope);
	VARIANT_BOOL vbContain;
	ipRelOp->Contains(ipGeometry, &vbContain);
	return vbContain == VARIANT_TRUE;
}

}
