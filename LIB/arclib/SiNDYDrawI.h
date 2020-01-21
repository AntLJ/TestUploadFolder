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

#ifndef __SINDY_DRAW_I_H__
#define __SINDY_DRAW_I_H__

class SiNDYDrawI
{
public:
// 定義
	/**
	 * @brief ビューポート点列形状クラス
	 */
	class ViewportPolyline
	{
	public:
	// 構築/消滅
		ViewportPolyline();
		~ViewportPolyline();

	// 設定/取得
		void clear();

		void setPolyline(IFeature* ipFeature, double dScaleForGeoToViewport);
		void setPolyline(IPointCollection* ipPointCol, double dScaleForGeoToViewport);

		LPPOINT polyline() const { return m_pPoints; }
		unsigned int count() const { return m_nCount; }
		const RECT& rect() const { return m_Rect; }

	private:
	// 構築/消滅
		ViewportPolyline(const ViewportPolyline&) {}

	// 変数
		LPPOINT m_pPoints;
		unsigned int m_nCount;
		RECT m_Rect;
	};

	/// マウス操作による基礎機能
	enum EMouseMode {
		kNoneMode, ///< 機能なし
		kPanMode, ///< 移動
		kZoomInMode, ///< ラバーバンドによる拡大
		kZoomOutMode, ///< ラバーバンドによる縮小
		kPanZoomMode, ///< マウスの上下動による拡縮
	};

// 構築
	SiNDYDrawI();

// 設定/取得
	void setLMouseMode(EMouseMode eMouseMode) { m_eLMouseMode = eMouseMode; }
	void setRMouseMode(EMouseMode eMouseMode) { m_eRMouseMode = eMouseMode; }

	EMouseMode getLMouseMode() const { return m_eLMouseMode; }
	EMouseMode getRMouseMode() const { return m_eRMouseMode; }

// 静的関数
	static BOOL GeoToViewport(IPoint* ipGeoPoint, double dScaleForGeoToViewport, LPPOINT lpViewPoint);
	static BOOL GeoToViewport(double dX, double dY, double dScaleForGeoToViewport, LPPOINT lpViewPoint);

protected:
// 変数
	double m_dCenterX; ///< 中心経度
	double m_dCenterY; ///< 中心緯度
	double m_dScale; ///< 倍率（Pixel/°）

	bool m_bDragging; ///< ドラッグ中
	bool m_bRButton; ///< 右ボタンによるドラッグ
	POINT m_posStart; ///< ドラッグ開始位置
	POINT m_posPrev; ///< 前カーソル位置
	EMouseMode m_eLMouseMode; ///< 標準の挙動L
	EMouseMode m_eRMouseMode; ///< 標準の挙動R
};

#endif // __SINDY_DRAW_I_H__
