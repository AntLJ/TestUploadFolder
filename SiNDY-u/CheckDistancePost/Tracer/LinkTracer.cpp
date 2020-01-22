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

// LinkTracer.cpp: LinkTracer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include <WinLib/WaitCursor.h>
#include "LinkTracer.h"
#include <iostream>
#include <sindy/workspace.h>

using namespace sindy::schema;

namespace sindy
{


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

LinkTracer::LinkTracer() :
m_pDijkstra(nullptr),
m_eSection(kSelectStartLink),
m_dbBuf(8.0f)
{
	
}

LinkTracer::~LinkTracer()
{
	delete m_pDijkstra;
	m_pDijkstra = nullptr;
}

void LinkTracer::clearCache()
{
	toStartSelectLinkSection();
	m_cLinksCache.clearCache();
	m_ipCachedFeatureClass = nullptr;
	m_listRouteIDs.clear();
}

//////////////////////////////////////////////////////////////////////
// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief 経路探索エンジンを変更する
 *
 * @param pNewEngine [in] 新しい経路探索エンジン
 */
void LinkTracer::setEngine(Dijkstra* pNewEngine)
{
	delete m_pDijkstra;
	m_pDijkstra = pNewEngine;
}

/**
 * @brief 描画するルートを設定する
 *
 * @param rRoute [in] 描画するルート
 */
void LinkTracer::setRoute(const Route& rRoute)
{
	m_ipRoutePolyline = nullptr;

	if(! rRoute.isValid())
		return;

	IPointCollectionPtr ipPointCol;
	for(const Route* pRoute = &rRoute; pRoute; pRoute = pRoute->m_pPrevRoute) {
		const Link* pLink = pRoute->link();
		long nNodeId = pRoute->m_nNodeId;

#if _DEBUG
		CString strTmp;
		strTmp.Format(_T(" Route OID:%d\n"), pLink->m_nObjectId);
		OutputDebugString(strTmp);
#endif
		m_listRouteIDs.push_back( pLink->m_nObjectId );

		IClonePtr ipClone;
		IClonePtr(pLink->m_ipGeometry)->Clone(&ipClone);
		IPolylinePtr ipLinkPolyline(ipClone);

		// 最後に一括して反転するので、それぞれのリンクでは反転して入れておく
		if(pLink->m_nFromNodeId == nNodeId) {
			ipLinkPolyline->ReverseOrientation();
		}

		if(! ipPointCol ) {
			ipPointCol = ipLinkPolyline;
		}
		else {
			IPointCollectionPtr ipPointCol2 = ipLinkPolyline;
			ipPointCol2->RemovePoints(0, 1);
			ipPointCol->AddPointCollection(ipPointCol2);
		}
	}

	if(ipPointCol ) {
		m_ipRoutePolyline = ipPointCol;
		m_ipRoutePolyline->ReverseOrientation();
	}

	m_listRouteIDs.reverse();
}

//////////////////////////////////////////////////////////////////////
// 変更
//////////////////////////////////////////////////////////////////////

/**
 * @brief リンク選定Where句を変更する
 *
 * @param lpWhereClause [in] リンク選定Where句
 * @retval true 成功
 * @retval false 失敗
 */
bool LinkTracer::changeLinkWhereClause(LPCTSTR lpWhereClause)
{
	return true;
}

/**
 * @brief セクションを変更する
 *
 * @param section [in] セクション
 */
void LinkTracer::changeSection(ESection section)
{
	m_eSection = section;
}

//////////////////////////////////////////////////////////////////////
// 反応処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief ツールがアクティブになった直後の処理
 */
void LinkTracer::onActivate()
{

}

/**
 * @brief 処理を取りやめ、開始リンク選択セクションに戻る
 */
void LinkTracer::toStartSelectLinkSection()
{
	if(m_ipRoutePolyline )
		m_ipRoutePolyline = nullptr;
	m_ipBasePoint = nullptr;
	m_ipBaseGeometry = nullptr;
	if(m_pDijkstra)
		m_pDijkstra->clear();
	m_cLinkTraceSpace.clear();
	changeSection(kSelectStartLink);
}

/**
 * @brief ツールが非アクティブになる直前の処理
 */
void LinkTracer::onDeactivate()
{
	clearCache();
}

/**
 * @brief 到達可能リンク判別モード
 *
 * @param ipPoint [in] クリックされた座標
 */
void LinkTracer::onArriveMode(const IPointPtr& ipPoint)
{
	long nId = 0;
	IGeometryPtr ipGeometry;

	const Link* pLink = m_cLinksCache.findLink(ipPoint, getMapSelectUnitTolerance());
	if(pLink) {
		nId = pLink->m_nObjectId;
		ipGeometry = pLink->m_ipGeometry;
	}

	if(nId && ipGeometry)
	{
		m_ipBaseGeometry = ipGeometry;
		m_ipBasePoint = ipPoint;

		if(startTrace(nId))
		{
			m_ipRoutePolyline = nullptr;
			changeSection(kStartTrace);
		}
		else
			m_ipBaseGeometry = nullptr;
	}
}

/**
 * @brief 通常経路探索モード
 *
 * @param ipPoint [in] クリックされた座標
 */
void LinkTracer::onNormalMode(const IPointPtr& ipPoint)
{
	switch(m_eSection) {
	case kSelectStartLink:
		onArriveMode(ipPoint);
		break;
	case kSelectEndLink:
	case kStartTrace:
	case kTracing:
		// 終点リンク選択
		int nLinkId = m_cLinksCache.findLinkId(ipPoint, getMapSelectUnitTolerance());
		if(nLinkId) {
			changeSection(kStartTrace);
			m_pDijkstra->setEndLinkId(nLinkId);
			traceToEnd();
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////
/**
 * @brief 有視界内のリンクをキャッシュする
 *
 * @retval true 成功
 * @retval false 失敗
 */
bool LinkTracer::init_cache(const IFeatureClassPtr& ipClass, double dbBuf)
{
	try
	{
		ESection eSection = m_eSection;

		m_dbBuf = dbBuf;
		clearCache();
		m_ipCachedFeatureClass = ipClass;
		m_cLinksCache.setLinkClass(m_ipCachedFeatureClass);
		m_cLinksCache.init_cache();
	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

bool LinkTracer::add_cache(long nOID, const _IRowPtr& ipRow)
{
	return m_cLinksCache.add_cache(nOID, ipRow);
}

/**
 * @brief 有視界内のリンクをキャッシュする
 *
 * @retval true 成功
 * @retval false 失敗
 */
bool LinkTracer::cache()
{
	try
	{
		// 追跡空間の設定
		if(m_cLinkTraceSpace.empty())
		{
			m_cLinkTraceSpace.setLinks(m_cLinksCache);
			changeSection(m_eSection);
		}
	}
	catch(std::exception& e)
	{
		return false;
	}

	return true;
}

/**
 * @brief 経路探索の始点リンクを設定し、探索の開始を宣言する
 *
 * @param ipStartLink [in] 始点リンク
 * @retval true 経路探索の開始に成功
 * @retval false 経路探索の開始に失敗
 */
bool LinkTracer::startTrace(const IFeaturePtr& ipStartLink)
{
	ATLASSERT(ipStartLink);

	long nOid;
	ipStartLink->get_OID(&nOid);

	return startTrace(nOid);
}

/**
 * @brief 経路探索の始点リンクを設定し、探索の開始を宣言する
 *
 * @param nStartLinkId [in] 始点リンクID
 * @retval true 経路探索の開始に成功
 * @retval false 経路探索の開始に失敗
 */
bool LinkTracer::startTrace(long nStartLinkId)
{
	setEngine(new Dijkstra);
	
	if(! m_pDijkstra->setLinks(m_cLinkTraceSpace, nStartLinkId))
		return false;

	return true;
}

/**
 * @brief 経路探索を１世代進める
 *
 * @retval true 経路探索はまだ続く
 * @retval false 経路探索は終了した
 */
bool LinkTracer::trace()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	CWaitCursor wait;

	bool bResult = m_pDijkstra->trace();
	if(bResult) {
		changeSection(kTracing);
	}
	else {
		changeSection(kSelectEndLink);
	}

	return true;
}

/**
 * @brief 次の経路探索路を描画し、探索を続ける
 *
 * @retval true 経路探索はまだ続く
 * @retval false 経路探索は終了した
 */
bool LinkTracer::traceWithDraw()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	bool bResult = m_pDijkstra->trace();

	const Route* pRoute = m_pDijkstra->getRouteToEndLink();
	if(pRoute) {
		setRoute(*pRoute);
		return false;
	}

	const Route* pNextRoute = m_pDijkstra->getNextTraceRoute();
	if(pNextRoute) {
		setRoute(*pNextRoute);
	}

	return bResult;
}

/**
 * @brief 経路探索が終了するまで探索を続ける
 *
 * @note 発見された経路は描画用に登録される
 * @retval true 探索は終了し、適切な経路が発見された
 * @retval false 適切な経路を発見することが出来なかった
 */
bool LinkTracer::traceToEnd()
{
	if(! m_pDijkstra || ! (m_eSection == kStartTrace || m_eSection == kTracing))
		return false;

	CWaitCursor wait;

	changeSection(kTracing);

	while(m_pDijkstra->trace()) {
	}

	changeSection(kSelectEndLink);

	const Route* pRoute = m_pDijkstra->getRouteToEndLink();
	if(! pRoute)
		return false;

	setRoute(*pRoute);

	return true;
}

/**
 * @brief 経路を辿れるだけ辿る
 *
 * @note 辿った場合、辿れたリンクを描画する
 * @retval true 辿った
 * @retval false 辿れなかった
 */
bool LinkTracer::traceToLimit()
{
	if(! m_pDijkstra || ! (m_eSection == kSelectEndLink || m_eSection == kStartTrace))
		return false;

	CWaitCursor wait;

	changeSection(kTracing);

	m_pDijkstra->setEndLinkId(0);
	while(m_pDijkstra->trace()) {
	}

	changeSection(kSelectEndLink);

	return true;
}

//////////////////////////////////////////////////////////////////////
// 描画
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

/**
 * @brief 指定した座標にもっとも近いリンクを取得する
 *
 * @return 指定した座標にもっとも近いリンク。取得できなかった場合NULL。
 */
IFeaturePtr LinkTracer::getLink(const IPointPtr& ipPoint)
{
	ATLASSERT(ipPoint);

	// 選択フィーチャ格納コンポーネットの初期化
	ITopologicalOperatorPtr ipTopoOp(ipPoint);

	// 入力されたポイントを選択エリア半径でバッファし、選択エリアを作成
	IGeometryPtr ipRegion;
	ipTopoOp->Buffer(1, &ipRegion);

	// リンクレイヤ取得
	ILayerPtr ipLinkLayer = nullptr;
	if(! ipLinkLayer )
		return nullptr;

	// リンクレイヤから近傍リンクを取得
	IFeatureCursorPtr ipFeatureCursor = nullptr;

	if(! ipFeatureCursor )
		return nullptr;

	ISpatialReferencePtr ipSpRef;
	ipPoint->get_SpatialReference(&ipSpRef);

	IProximityOperatorPtr ipProOp(ipPoint);

	IFeaturePtr ipResultLink;
	double dResultDistance = 0;

	IFeaturePtr ipLink;
	while(SUCCEEDED(ipFeatureCursor->NextFeature(&ipLink)) && ipLink ) {
		IGeometryPtr ipGeometry;
		ipLink->get_ShapeCopy(&ipGeometry);

		double dDistance;
		ipProOp->ReturnDistance(ipGeometry, &dDistance);
		if(ipResultLink == 0 || dDistance < dResultDistance) {
			dResultDistance = dDistance;
			ipResultLink = ipLink;
		}
	}

	return ipResultLink;
}

/**
 * @brief 現在の拡大モードで選択閾値を取得する
 *
 * @return 選択閾値
 */
double LinkTracer::getMapSelectUnitTolerance()
{
	IUnitConverterPtr ipUnit( CLSID_UnitConverter );
	double dbRet = 0.0f;

	atl2::valid(ipUnit->ConvertUnits( m_dbBuf, esriMeters, esriDecimalDegrees, &dbRet ));
	return dbRet;
}

}
