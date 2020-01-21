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

#include "stdafx.h"
#include "SiNDYDrawI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// SiNDYDrawI::ViewportPolyline
//////////////////////////////////////////////////////////////////////

SiNDYDrawI::ViewportPolyline::ViewportPolyline() :
m_pPoints(0),
m_nCount(0)
{
	m_Rect.left = 0;
	m_Rect.top = 0;
	m_Rect.right = 0;
	m_Rect.bottom = 0;
}

SiNDYDrawI::ViewportPolyline::~ViewportPolyline()
{
	clear();
}

void SiNDYDrawI::ViewportPolyline::clear()
{
	delete [] m_pPoints;
	m_pPoints = 0;
	m_nCount = 0;

	m_Rect.left = 0;
	m_Rect.top = 0;
	m_Rect.right = 0;
	m_Rect.bottom = 0;
}

void SiNDYDrawI::ViewportPolyline::setPolyline(IFeature* ipFeature, double dScaleForGeoToViewport)
{
	if(! ipFeature)
		return;

	IGeometryPtr ipGeometry;
	ipFeature->get_Shape(&ipGeometry);

	setPolyline(IPointCollectionPtr(ipGeometry), dScaleForGeoToViewport);
}

void SiNDYDrawI::ViewportPolyline::setPolyline(IPointCollection* ipPointCol, double dScaleForGeoToViewport)
{
	clear();

	if(! ipPointCol)
		return;

	long nPointCount;
	ipPointCol->get_PointCount(&nPointCount);
	if(nPointCount == 0)
		return;

	m_pPoints = new POINT[nPointCount];
	m_nCount = nPointCount;

	for(long i = 0; i < nPointCount; i++) {
		IPointPtr ipPoint;
		ipPointCol->get_Point(i, &ipPoint);

		POINT* pPoint = m_pPoints + i;
		if(GeoToViewport(ipPoint, dScaleForGeoToViewport, pPoint)) {
			if(m_Rect.left == m_Rect.right && m_Rect.top == m_Rect.bottom) {
				m_Rect.left = pPoint->x;
				m_Rect.top = pPoint->y;
				m_Rect.right = pPoint->x + 1;
				m_Rect.bottom = pPoint->y + 1;
			}
			else {
				if(pPoint->x < m_Rect.left) m_Rect.left = pPoint->x;
				if(pPoint->y < m_Rect.top) m_Rect.top = pPoint->y;
				if(pPoint->x >= m_Rect.right) m_Rect.right = pPoint->x + 1;
				if(pPoint->y >= m_Rect.bottom) m_Rect.bottom = pPoint->y + 1;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// SiNDYDrawI
//////////////////////////////////////////////////////////////////////

// 構築/消滅
//////////////////////////////////////////////////////////////////////

SiNDYDrawI::SiNDYDrawI() :
m_dCenterX(140.0), //< 適当
m_dCenterY(40.0), //< 適当
m_dScale(5.0), //< 適当
m_bDragging(false),
m_eLMouseMode(kNoneMode),
m_eRMouseMode(kNoneMode)
{
}

// 静的関数
//////////////////////////////////////////////////////////////////////

/**
 * @brief 経緯度を視点座標に変換
 *
 * @note 経緯度それぞれにdScaleForGeoToViewportを乗算。
 *
 * @param ipGeoPoint [in] 経緯度
 * @param dScaleForGeoToViewport [in] 始点座標との縮尺差
 * @param lpViewPoint [in] 視点座標
 * @retval TRUE 成功
 * @retval FALSE 引数がNULL
 */
BOOL SiNDYDrawI::GeoToViewport(IPoint* ipGeoPoint, double dScaleForGeoToViewport, LPPOINT lpViewPoint)
{
	if(! (ipGeoPoint && lpViewPoint))
		return FALSE;

	double dX, dY;
	ipGeoPoint->get_X(&dX);
	ipGeoPoint->get_Y(&dY);

	return GeoToViewport(dX, dY, dScaleForGeoToViewport, lpViewPoint);
}

BOOL SiNDYDrawI::GeoToViewport(double dX, double dY, double dScaleForGeoToViewport, LPPOINT lpViewPoint)
{
	if(! lpViewPoint)
		return FALSE;

	lpViewPoint->x = static_cast<LONG>(dX * dScaleForGeoToViewport);
	lpViewPoint->y = static_cast<LONG>(dY * dScaleForGeoToViewport);

	return TRUE;
}
