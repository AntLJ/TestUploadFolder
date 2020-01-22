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

// LinkTracer.h: LinkTracer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "LinkTraceSpace.h"
#include "LinksCache.h"
#include "Dijkstra.h"

namespace sindy
{

class LinkTracer  
{
public:
	enum ESection {
		kSelectStartLink,
		kSelectEndLink,
		kStartTrace,
		kTracing,
	};

// 構築/消滅
	LinkTracer();
	virtual ~LinkTracer();

	void clearCache();

// 変更
	bool changeLinkWhereClause(LPCTSTR lpWhereClause);

// ハンドル
	void onActivate();
	void toStartSelectLinkSection();
	void onDeactivate();
	void onMouseDown(LONG Button, const IPointPtr& ipPoint);
	void onRefresh();

// 取得
	IFeaturePtr getLink(const IPointPtr& ipPoint);
	IPolylinePtr getRouteGeom() { return m_ipRoutePolyline; }
	std::list<long> getRouteIDs() { return m_listRouteIDs; };

// 処理
	bool trace();
	bool traceWithDraw();
	bool traceToEnd();
	bool traceToLimit();

// 処理
	bool init_cache(const IFeatureClassPtr& ipClass, double dbBuf);
	bool add_cache(long nOID, const _IRowPtr& ipRow);
	bool cache();

// ハンドル
	void onArriveMode(const IPointPtr& ipPoint);
	void onNormalMode(const IPointPtr& ipPoint);

private:
// 設定
	void setEngine(Dijkstra* pNewEngine);
	void setRoute(const Route& rRoute);

// 取得
	double getMapSelectUnitTolerance();

// 変更
	void changeSection(ESection section);

// 処理
	
	bool startTrace(const IFeaturePtr& ipStartLink);
	bool startTrace(long nStartLinkId);

// プロパティ
	CString m_strLinkWhereClause;
	ESection m_eSection;

	LinkTraceSpace m_cLinkTraceSpace; ///< リンク追跡空間
	LinksCache m_cLinksCache; ///< リンクキャッシュ
	IFeatureClassPtr m_ipCachedFeatureClass; ///< キャッシュしているフィーチャクラス

	Dijkstra* m_pDijkstra; ///< リンク追跡エンジン

	IGeometryPtr m_ipBaseGeometry; ///< 基準形状
	IPointPtr m_ipBasePoint; ///< 基準形状
//	IFeaturePtr m_ipStartLink; ///< 開始リンク
	IPolylinePtr m_ipRoutePolyline; ///< ルート形状
	IPolygonPtr m_ipPolygon; ///< 何か矩形

	std::list<long> m_listRouteIDs;
	double m_dbBuf;
};

}
