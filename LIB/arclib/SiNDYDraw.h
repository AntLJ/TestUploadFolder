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

#ifndef __SINDY_DRAW_H__
#define __SINDY_DRAW_H__

//#include <atlwin.h>
//#include <atlmisc.h>
#include "../../WinLib/ATLControls.h"
#include "SiNDYDrawI.h"

//////////////////////////////////////////////////////////////////////
// 定義
//////////////////////////////////////////////////////////////////////

/**
 * @brief Windows標準のGUIでArcの形状を描画するためのクラス
 */
class SiNDYDraw : public CWindowImpl<SiNDYDraw, ATLControls::CStatic, CControlWinTraits>, public SiNDYDrawI
{
public:
	SiNDYDraw() {}
	virtual ~SiNDYDraw() {}

BEGIN_MSG_MAP(CHwyPointDraw)
	MESSAGE_HANDLER(WM_PAINT, OnPaint)
	MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown);
	MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp);
	MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown);
	MESSAGE_HANDLER(WM_RBUTTONUP, OnRButtonUp);
	MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove);
	MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel);
END_MSG_MAP()

// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
// 処理
	virtual void setScale(double dScaleForGeoToViewport);

	void setCenter(IPoint* ipPoint);
	virtual void setCenter(double dX, double dY);

	void zoom(double dZoom);
	void zoomToCursorPos(double dZoom);

	void overviewEnvelope(IEnvelope* ipEnv);
	void overviewRect(RECT& rect, bool bZoomIn);

	void scroll(SIZE szScroll);

// 描画
	void drawPolyline(HDC hDc, IPointCollection* ipPointCol);

// 座標変換
	BOOL GeoToViewport(IFeature* ipFeature, LPPOINT lpPoint) const;
	BOOL GeoToViewport(IPoint* ipGeoPoint, LPPOINT lpViewPoint) const;
	BOOL GeoToViewport(double dX, double dY, LPPOINT lpViewPoint) const;
	BOOL GeoToViewport(IEnvelope* ipGeoEnvelope, LPRECT lpViewRect) const;
	BOOL ViewportToGeo(LPPOINT lpPoint, double& dX, double& dY) const;
	BOOL ViewportToGeo(LPRECT lpRect, double& dXMin, double& dYMin, double& dXMax, double& dYMax) const;
	BOOL ViewportClient(LPPOINT lpPos) const;
	BOOL ViewportClient(LPRECT lpRect) const;
	BOOL ClientToViewport(LPPOINT lpPos) const;
	BOOL ClientToViewport(LPRECT lpRect) const;

protected:
	void prepareHDC(HDC hDc);

// （純粋）仮想関数
	virtual void OnDraw(HDC hDc) = 0;

	virtual bool beginLDrag(short x, short y);
	virtual bool beginRDrag(short x, short y);
	virtual bool beginDrag(short x, short y);
	virtual void endDrag();

	virtual void onLDrag(POINT& posCurrent, POINT& posPrev, POINT& posDragStart);
	virtual void onRDrag(POINT& posCurrent, POINT& posPrev, POINT& posDragStart);
	virtual void onDrag(EMouseMode eMouseMode, POINT& posCurrent, POINT& posPrev, POINT& posDragStart);
};


//////////////////////////////////////////////////////////////////////
// 実装
//////////////////////////////////////////////////////////////////////

// イベント処理
//////////////////////////////////////////////////////////////////////

inline LRESULT SiNDYDraw::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 描画開始宣言
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(&ps);

	// 描画領域
	RECT rectClip;
	GetClientRect(&rectClip);

	// クリップ領域限定
	::IntersectClipRect(hDc, rectClip.left, rectClip.top, rectClip.right, rectClip.bottom);

	// 領域塗りつぶし
	HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);

	::GetClipRgn(hDc, hRgn);
	::FillRgn(hDc, hRgn, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

	::DeleteObject(hRgn);

	prepareHDC(hDc);

	OnDraw(hDc);

	// 描画終了宣言
	EndPaint(&ps);

	return 0;
}

inline LRESULT SiNDYDraw::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(! beginLDrag(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))))
		endDrag();

	return 0;
}

inline LRESULT SiNDYDraw::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	endDrag();
	return 0;
}

inline LRESULT SiNDYDraw::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(! beginRDrag(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam))))
		endDrag();

	return 0;
}

inline LRESULT SiNDYDraw::OnRButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	endDrag();
	return 0;
}

inline LRESULT SiNDYDraw::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(! m_bDragging)
		return 0;

	// 現カーソル位置取得
	CPoint pos(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)));

	if(m_bRButton) {
		onRDrag(pos, m_posPrev, m_posStart);
	}
	else {
		onLDrag(pos, m_posPrev, m_posStart);
	}

	// 前カーソル位置更新
	m_posPrev = pos;

	return 0;
}

inline LRESULT SiNDYDraw::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	short nDeltaZ = HIWORD(wParam);
	if(nDeltaZ > 0) {
		zoomToCursorPos(1.0 / 1.2);
	}
	else if(nDeltaZ < 0) {
		zoomToCursorPos(1.2);
	}

	return 0;
}

// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief 表示倍率を直接指定する
 *
 * @param dScaleForGeoToViewport [in] 倍率（縮尺ではない）
 */
inline void SiNDYDraw::setScale(double dScaleForGeoToViewport)
{
	if(! dScaleForGeoToViewport)
		return;

	if(dScaleForGeoToViewport != m_dScale) {
		m_dScale = dScaleForGeoToViewport;
		Invalidate();
	}
}

/**
 * @brief 視点を変更する
 *
 * @param ipPoint [in] 視点座標
 */
inline void SiNDYDraw::setCenter(IPoint* ipPoint)
{
	if(! ipPoint)
		return;

	double dX, dY;
	ipPoint->get_X(&dX);
	ipPoint->get_Y(&dY);

	setCenter(dX, dY);
}

/**
 * @brief 視点を変更する
 *
 * @param dX [in] X座標
 * @param dY [in] Y座標
 */
inline void SiNDYDraw::setCenter(double dX, double dY)
{
	m_dCenterX = dX;
	m_dCenterY = dY;
	Invalidate();
}

/**
 * @brief 表示倍率を拡大/縮小する
 *
 * @param dZoom [in] 現在の表示倍率に掛け合わせる値
 */
inline void SiNDYDraw::zoom(double dZoom)
{
	if(! dZoom)
		return;

	setScale(m_dScale * dZoom);
}

/**
 * @brief 現在カーソルが存在している座標を中心として、表示倍率を拡大/縮小する
 *
 * @param dZoom [in] 現在の表示倍率に掛け合わせる値
 */
inline void SiNDYDraw::zoomToCursorPos(double dZoom)
{
	if(! m_dScale)
		return;

	CPoint pos;
	if(! ::GetCursorPos(&pos))
		return;

	if(! ScreenToClient(&pos))
		return;

	double dScale = m_dScale;

	CRect rectClient;
	GetClientRect(&rectClient);

	POINT posClientCenter = rectClient.CenterPoint();
	pos -= posClientCenter;

	zoom(dZoom);
	setCenter(m_dCenterX + (pos.x - pos.x / dZoom) / dScale, m_dCenterY - (pos.y - pos.y / dZoom) / dScale);
	Invalidate();
}

/**
 * @brief 指定した矩形を内包するのに最適な表示倍率を設定する
 *
 * @param ipEnv [in] Geo矩形
 */
inline void SiNDYDraw::overviewEnvelope(IEnvelope* ipEnv)
{
	if(ipEnv == 0)
		return;

	double dXMin, dYMin, dXMax, dYMax;
	ipEnv->QueryCoords(&dXMin, &dYMin, &dXMax, &dYMax);
	double dXCenter = (dXMax + dXMin) / 2.0;
	double dYCenter = (dYMax + dYMin) / 2.0;
	double dWidth = dXMax - dXMin;
	double dHeight = dYMax - dYMin;

	CRect rect;
	GetClientRect(&rect);

	double dXScale = rect.Width() / (dWidth ? dWidth : 1.0);
	double dYScale = rect.Height() / (dHeight ? dHeight : 1.0);
	m_dScale = (dXScale < dYScale ? dXScale : dYScale) * 0.9;
	m_dCenterX = dXCenter;
	m_dCenterY = dYCenter;

	Invalidate();
}

/**
 * @brief 指定した矩形を内包するのに最適な表示倍率を設定する
 *
 * @param rect [in] スクリーン矩形
 * @param bZoomIn [in] falseの場合、倍率を反転させる（2倍→1/2倍）
 */
inline void SiNDYDraw::overviewRect(RECT& rect, bool bZoomIn)
{
	CRect rectOverview(rect);
	if(rectOverview.IsRectEmpty())
		return;

	CRect rectClient;
	GetClientRect(&rectClient);
	if(rectClient.IsRectEmpty())
		return;

	double dX, dY;
	POINT pos = rectOverview.CenterPoint();
	if(! (ClientToViewport(&pos) && ViewportToGeo(&pos, dX, dY)))
		return;

	double dXScale =
		bZoomIn ?
		static_cast<double>(rectClient.Width()) / rectOverview.Width() :
		static_cast<double>(rectOverview.Width()) / rectClient.Width();

	double dYScale =
		bZoomIn ?
		static_cast<double>(rectClient.Height()) / rectOverview.Height() :
		static_cast<double>(rectOverview.Height()) / rectClient.Height();

	setCenter(dX, dY);
	zoom((dXScale < dYScale ? dXScale : dYScale) * 0.9);

	Invalidate();
}

/**
 * @brief 指定方向へスクロールする
 *
 * @param szScroll [in] スクロール方向
 */
inline void SiNDYDraw::scroll(SIZE szScroll)
{
	if(szScroll.cx == 0 && szScroll.cy == 0)
		return;

	if(! m_dScale)
		return;

	// 中心緯度経度スクロール
	m_dCenterX -= szScroll.cx / m_dScale;
	m_dCenterY += szScroll.cy / m_dScale;

	// 画面スクロール
	RECT rect;
	GetClientRect(&rect);
	::ScrollWindowEx(m_hWnd, szScroll.cx, szScroll.cy, NULL, &rect, NULL, NULL, SW_INVALIDATE);
	UpdateWindow();
}

// 描画
//////////////////////////////////////////////////////////////////////

/**
 * @brief デバイスコンテキストの初期準備を行う
 *
 * @param hDc [in] デバイスコンテキストハンドル
 */
inline void SiNDYDraw::prepareHDC(HDC hDc)
{
	// 描画領域
	RECT rectClip;
	GetClientRect(&rectClip);

	int nClipCenterX = (rectClip.left + rectClip.right) / 2;
	int nClipCenterY = (rectClip.top + rectClip.bottom) / 2;

	// 描画領域の中心に原点を移動
	::SetViewportOrgEx(hDc, nClipCenterX, nClipCenterY, NULL);

	// 上下反転
	::SetMapMode(hDc, MM_ANISOTROPIC);
	::SetViewportExtEx(hDc, 1, -1, NULL);

	// 描画領域の中心に視点を持ってくる
	::OffsetViewportOrgEx(hDc, static_cast<int>(-(m_dCenterX * m_dScale)), static_cast<int>((m_dCenterY * m_dScale)), NULL);
}

/**
 * @brief 点列を描画する
 *
 * @param hDc [in] デバイスコンテキスタ
 * @param ipPointCol [in] 点列
 */
inline void SiNDYDraw::drawPolyline(HDC hDc, IPointCollection* ipPointCol)
{
	if(! (hDc && ipPointCol))
		return;

	ViewportPolyline aViewportPolyline;
	aViewportPolyline.setPolyline(ipPointCol, m_dScale);
	::Polyline(hDc, aViewportPolyline.polyline(), aViewportPolyline.count());
}

// 
//////////////////////////////////////////////////////////////////////

inline bool SiNDYDraw::beginLDrag(short x, short y)
{
	// 既にドラッグ中の場合は無視
	if(m_bDragging)
		return false;

	m_bRButton = false;

	return beginDrag(x, y);
}

inline bool SiNDYDraw::beginRDrag(short x, short y)
{
	// 既にドラッグ中の場合は無視
	if(m_bDragging)
		return false;

	m_bRButton = true;

	return beginDrag(x, y);
}

inline bool SiNDYDraw::beginDrag(short x, short y)
{
	// 既にドラッグ中の場合は無視
	if(m_bDragging)
		return false;

	// キャプチャーモードへ移行
	SetCapture();

	// 各種設定
	m_bDragging = true;
	m_posStart.x = x;
	m_posStart.y = y;
	m_posPrev = m_posStart;

	return true;
}

inline void SiNDYDraw::endDrag()
{
	// ドラッグ中でなければ無視
	if(! m_bDragging)
		return;

	EMouseMode eMouseMode = m_bRButton ? m_eRMouseMode : m_eLMouseMode;
	if(eMouseMode == kZoomInMode || eMouseMode == kZoomOutMode) {
		HDC hDc = GetDC();
		::SelectObject(hDc, GetStockObject(WHITE_PEN));
		::SelectObject(hDc, GetStockObject(NULL_BRUSH));
		::SetROP2(hDc, R2_XORPEN);
		::Rectangle(hDc, m_posStart.x, m_posStart.y, m_posPrev.x, m_posPrev.y);
		::SetROP2(hDc, R2_COPYPEN);
		CRect rect(m_posStart.x, m_posStart.y, m_posPrev.x, m_posPrev.y);
		rect.NormalizeRect();
		overviewRect(rect, eMouseMode == kZoomInMode);
	}

	// 各種設定解除
	m_bDragging = false;
	m_bRButton = false;

	// キャプチャーモード解除
	ReleaseCapture();
}

inline void SiNDYDraw::onLDrag(POINT& posCurrent, POINT& posPrev, POINT& posDragStart)
{
	onDrag(m_eLMouseMode, posCurrent, posPrev, posDragStart);
}

inline void SiNDYDraw::onRDrag(POINT& posCurrent, POINT& posPrev, POINT& posDragStart)
{
	onDrag(m_eRMouseMode, posCurrent, posPrev, posDragStart);
}

inline void SiNDYDraw::onDrag(EMouseMode eMouseMode, POINT& posCurrent, POINT& posPrev, POINT& posDragStart)
{
	switch(eMouseMode) {
	case kPanMode:
		scroll(CSize(posCurrent.x - posPrev.x, posCurrent.y - posPrev.y));
		break;
	case kZoomInMode:
	case kZoomOutMode:
		{
			HDC hDc = GetDC();
			::SelectObject(hDc, GetStockObject(WHITE_PEN));
			::SelectObject(hDc, GetStockObject(NULL_BRUSH));
			::SetROP2(hDc, R2_XORPEN);
			::Rectangle(hDc, posDragStart.x, posDragStart.y, posPrev.x, posPrev.y);
			::Rectangle(hDc, posDragStart.x, posDragStart.y, posCurrent.x, posCurrent.y);
			::SetROP2(hDc, R2_COPYPEN);
		}
		break;
	case kPanZoomMode:
		{
			double dYMove = (posCurrent.y - posPrev.y) * 0.02;
			if(dYMove) {
				if(dYMove > 0) {
					zoom(dYMove + 1.0);					
				}
				else if(dYMove < 0) {
					zoom(1.0 / (1.0 - dYMove));
				}
			}
		}
		break;
	}
}

// 変換
//////////////////////////////////////////////////////////////////////

/**
 * @brief ArcMap点フィーチャの座標を視点座標に変換
 *
 * @note 経緯度それぞれにm_dScaleを乗算。
 *
 * @param ipFeature [in] 点フィーチャ
 * @param lpPoint [in] 視点座標
 * @retval TRUE 成功
 * @retval FALSE 引数がNULL。ipFeatureが点フィーチャではない。
 */
inline BOOL SiNDYDraw::GeoToViewport(IFeature* ipFeature, LPPOINT lpPoint) const
{
	if(! (ipFeature && lpPoint))
		return FALSE;

	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);

	return GeoToViewport(IPointPtr(ipGeom), lpPoint);
}

/**
 * @brief 経緯度を視点座標に変換
 *
 * @note 経緯度それぞれにm_dScaleを乗算。
 *
 * @param ipGeoPoint [in] 経緯度
 * @param lpViewPoint [in] 視点座標
 * @retval TRUE 成功
 * @retval FALSE 引数がNULL
 */
inline BOOL SiNDYDraw::GeoToViewport(IPoint* ipGeoPoint, LPPOINT lpViewPoint) const
{
	return SiNDYDrawI::GeoToViewport(ipGeoPoint, m_dScale, lpViewPoint);
}

inline BOOL SiNDYDraw::GeoToViewport(double dX, double dY, LPPOINT lpViewPoint) const
{
	return SiNDYDrawI::GeoToViewport(dX, dY, m_dScale, lpViewPoint);
}

inline BOOL SiNDYDraw::GeoToViewport(IEnvelope* ipEnvelope, LPRECT lpViewRect) const
{
	if(! (ipEnvelope && lpViewRect))
		return FALSE;

	POINT posTopLeft;
	IPointPtr ipLowerLeftPoint;
	ipEnvelope->get_LowerLeft(&ipLowerLeftPoint);
	GeoToViewport(ipLowerLeftPoint, &posTopLeft);

	POINT posBottomRight;
	IPointPtr ipUpperRightPoint;
	ipEnvelope->get_UpperRight(&ipUpperRightPoint);
	GeoToViewport(ipUpperRightPoint, &posBottomRight);

	*lpViewRect = CRect(posTopLeft, posBottomRight);

	return TRUE;
}

/**
 * @brief 視点座標を経緯度に変換
 *
 * @note 視点座標からm_dScaleを除算。
 *
 * @param lpPoint [in] 視点座標
 * @param dX [in] 経度
 * @param dY [in] 緯度
 * @retval TRUE 成功
 * @retval lpPointがNULL。m_dScaleが0。
 */
inline BOOL SiNDYDraw::ViewportToGeo(LPPOINT lpPoint, double& dX, double& dY) const
{
	if(! (lpPoint && m_dScale))
		return FALSE;

	dX = lpPoint->x / m_dScale;
	dY = lpPoint->y / m_dScale;

	return TRUE;
}

/**
 * @brief 視点座標矩形を経緯度に変換
 *
 * @note 視点座標からm_dScaleを除算。
 *
 * @param lpRect [in] 視点座標矩形
 * @param dXMin [in] 最小経度
 * @param dYMin [in] 最小緯度
 * @param dXMax [in] 最大経度
 * @param dYMax [in] 最大緯度
 * @retval TRUE 成功
 * @retval lpPointがNULL。m_dScaleが0。
 */
inline BOOL SiNDYDraw::ViewportToGeo(LPRECT lpRect, double& dXMin, double& dYMin, double& dXMax, double& dYMax) const
{
	if(! lpRect)
		return FALSE;

	POINT posBottomLeft = { lpRect->left, lpRect->bottom };
	if(! ViewportToGeo(&posBottomLeft, dXMin, dYMin))
		return FALSE;

	POINT posTopRight = { lpRect->right, lpRect->top };
	if(! ViewportToGeo(&posTopRight, dXMax, dYMax))
		return FALSE;

	return TRUE;
}

inline BOOL SiNDYDraw::ViewportClient(LPPOINT lpPos) const
{
	if(! lpPos)
		return FALSE;

	if(! m_dScale)
		return FALSE;

	// 描画領域
	CRect rectClient;
	GetClientRect(&rectClient);

	POINT posClientCenter = rectClient.CenterPoint();

	CSize sz = CPoint(*lpPos) - posClientCenter;
	lpPos->x = static_cast<LONG>(m_dCenterX / m_dScale - sz.cx);
	lpPos->y = static_cast<LONG>(m_dCenterY / m_dScale + sz.cy);

	return TRUE;
}

inline BOOL SiNDYDraw::ViewportClient(LPRECT lpRect) const
{
	if(! lpRect)
		return FALSE;

	if(! m_dScale)
		return FALSE;

	POINT posTopLeft = { lpRect->left, lpRect->top };
	if(! ViewportClient(&posTopLeft))
		return FALSE;

	POINT posBottomRight = { lpRect->right, lpRect->bottom };
	if(! ViewportClient(&posBottomRight))
		return FALSE;

	lpRect->left = posTopLeft.x;
	lpRect->top = posTopLeft.y;
	lpRect->right = posBottomRight.x;
	lpRect->bottom = posBottomRight.y;

	return TRUE;
}

/**
 * @brief クライアント座標を視点座標に変換
 *
 * @param lpPos [in] クライアント座標
 * @retval TRUE 成功
 * @retval lpPosがNULL
 */
inline BOOL SiNDYDraw::ClientToViewport(LPPOINT lpPos) const
{
	if(! lpPos)
		return FALSE;

	// 描画領域
	CRect rectClient;
	GetClientRect(&rectClient);

	POINT posClientCenter = rectClient.CenterPoint();

	CSize sz = CPoint(*lpPos) - posClientCenter;
	lpPos->x = static_cast<LONG>(m_dCenterX * m_dScale + sz.cx);
	lpPos->y = static_cast<LONG>(m_dCenterY * m_dScale - sz.cy);

	return TRUE;
}

/**
 * @brief クライアント矩形の座標を視点座標に変換
 *
 * @param lpRect [in] クライアント矩形
 * @retval TRUE 成功
 * @retval lpRectがNULL
 */
inline BOOL SiNDYDraw::ClientToViewport(LPRECT lpRect) const
{
	if(! lpRect)
		return FALSE;

	POINT posTopLeft = { lpRect->left, lpRect->top };
	if(! ClientToViewport(&posTopLeft))
		return FALSE;

	POINT posBottomRight = { lpRect->right, lpRect->bottom };
	if(! ClientToViewport(&posBottomRight))
		return FALSE;

	lpRect->left = posTopLeft.x;
	lpRect->top = posTopLeft.y;
	lpRect->right = posBottomRight.x;
	lpRect->bottom = posBottomRight.y;

	return TRUE;
}

#endif // __SINDY_DRAW_H__
