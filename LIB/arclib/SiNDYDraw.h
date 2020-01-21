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
// ��`
//////////////////////////////////////////////////////////////////////

/**
 * @brief Windows�W����GUI��Arc�̌`���`�悷�邽�߂̃N���X
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

// �n���h���̃v���g�^�C�v:
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
// ����
	virtual void setScale(double dScaleForGeoToViewport);

	void setCenter(IPoint* ipPoint);
	virtual void setCenter(double dX, double dY);

	void zoom(double dZoom);
	void zoomToCursorPos(double dZoom);

	void overviewEnvelope(IEnvelope* ipEnv);
	void overviewRect(RECT& rect, bool bZoomIn);

	void scroll(SIZE szScroll);

// �`��
	void drawPolyline(HDC hDc, IPointCollection* ipPointCol);

// ���W�ϊ�
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

// �i�����j���z�֐�
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
// ����
//////////////////////////////////////////////////////////////////////

// �C�x���g����
//////////////////////////////////////////////////////////////////////

inline LRESULT SiNDYDraw::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// �`��J�n�錾
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(&ps);

	// �`��̈�
	RECT rectClip;
	GetClientRect(&rectClip);

	// �N���b�v�̈����
	::IntersectClipRect(hDc, rectClip.left, rectClip.top, rectClip.right, rectClip.bottom);

	// �̈�h��Ԃ�
	HRGN hRgn = ::CreateRectRgn(0, 0, 0, 0);

	::GetClipRgn(hDc, hRgn);
	::FillRgn(hDc, hRgn, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

	::DeleteObject(hRgn);

	prepareHDC(hDc);

	OnDraw(hDc);

	// �`��I���錾
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

	// ���J�[�\���ʒu�擾
	CPoint pos(static_cast<short>(LOWORD(lParam)), static_cast<short>(HIWORD(lParam)));

	if(m_bRButton) {
		onRDrag(pos, m_posPrev, m_posStart);
	}
	else {
		onLDrag(pos, m_posPrev, m_posStart);
	}

	// �O�J�[�\���ʒu�X�V
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

// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �\���{���𒼐ڎw�肷��
 *
 * @param dScaleForGeoToViewport [in] �{���i�k�ڂł͂Ȃ��j
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
 * @brief ���_��ύX����
 *
 * @param ipPoint [in] ���_���W
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
 * @brief ���_��ύX����
 *
 * @param dX [in] X���W
 * @param dY [in] Y���W
 */
inline void SiNDYDraw::setCenter(double dX, double dY)
{
	m_dCenterX = dX;
	m_dCenterY = dY;
	Invalidate();
}

/**
 * @brief �\���{�����g��/�k������
 *
 * @param dZoom [in] ���݂̕\���{���Ɋ|�����킹��l
 */
inline void SiNDYDraw::zoom(double dZoom)
{
	if(! dZoom)
		return;

	setScale(m_dScale * dZoom);
}

/**
 * @brief ���݃J�[�\�������݂��Ă�����W�𒆐S�Ƃ��āA�\���{�����g��/�k������
 *
 * @param dZoom [in] ���݂̕\���{���Ɋ|�����킹��l
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
 * @brief �w�肵����`������̂ɍœK�ȕ\���{����ݒ肷��
 *
 * @param ipEnv [in] Geo��`
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
 * @brief �w�肵����`������̂ɍœK�ȕ\���{����ݒ肷��
 *
 * @param rect [in] �X�N���[����`
 * @param bZoomIn [in] false�̏ꍇ�A�{���𔽓]������i2�{��1/2�{�j
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
 * @brief �w������փX�N���[������
 *
 * @param szScroll [in] �X�N���[������
 */
inline void SiNDYDraw::scroll(SIZE szScroll)
{
	if(szScroll.cx == 0 && szScroll.cy == 0)
		return;

	if(! m_dScale)
		return;

	// ���S�ܓx�o�x�X�N���[��
	m_dCenterX -= szScroll.cx / m_dScale;
	m_dCenterY += szScroll.cy / m_dScale;

	// ��ʃX�N���[��
	RECT rect;
	GetClientRect(&rect);
	::ScrollWindowEx(m_hWnd, szScroll.cx, szScroll.cy, NULL, &rect, NULL, NULL, SW_INVALIDATE);
	UpdateWindow();
}

// �`��
//////////////////////////////////////////////////////////////////////

/**
 * @brief �f�o�C�X�R���e�L�X�g�̏����������s��
 *
 * @param hDc [in] �f�o�C�X�R���e�L�X�g�n���h��
 */
inline void SiNDYDraw::prepareHDC(HDC hDc)
{
	// �`��̈�
	RECT rectClip;
	GetClientRect(&rectClip);

	int nClipCenterX = (rectClip.left + rectClip.right) / 2;
	int nClipCenterY = (rectClip.top + rectClip.bottom) / 2;

	// �`��̈�̒��S�Ɍ��_���ړ�
	::SetViewportOrgEx(hDc, nClipCenterX, nClipCenterY, NULL);

	// �㉺���]
	::SetMapMode(hDc, MM_ANISOTROPIC);
	::SetViewportExtEx(hDc, 1, -1, NULL);

	// �`��̈�̒��S�Ɏ��_�������Ă���
	::OffsetViewportOrgEx(hDc, static_cast<int>(-(m_dCenterX * m_dScale)), static_cast<int>((m_dCenterY * m_dScale)), NULL);
}

/**
 * @brief �_���`�悷��
 *
 * @param hDc [in] �f�o�C�X�R���e�L�X�^
 * @param ipPointCol [in] �_��
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
	// ���Ƀh���b�O���̏ꍇ�͖���
	if(m_bDragging)
		return false;

	m_bRButton = false;

	return beginDrag(x, y);
}

inline bool SiNDYDraw::beginRDrag(short x, short y)
{
	// ���Ƀh���b�O���̏ꍇ�͖���
	if(m_bDragging)
		return false;

	m_bRButton = true;

	return beginDrag(x, y);
}

inline bool SiNDYDraw::beginDrag(short x, short y)
{
	// ���Ƀh���b�O���̏ꍇ�͖���
	if(m_bDragging)
		return false;

	// �L���v�`���[���[�h�ֈڍs
	SetCapture();

	// �e��ݒ�
	m_bDragging = true;
	m_posStart.x = x;
	m_posStart.y = y;
	m_posPrev = m_posStart;

	return true;
}

inline void SiNDYDraw::endDrag()
{
	// �h���b�O���łȂ���Ζ���
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

	// �e��ݒ����
	m_bDragging = false;
	m_bRButton = false;

	// �L���v�`���[���[�h����
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

// �ϊ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief ArcMap�_�t�B�[�`���̍��W�����_���W�ɕϊ�
 *
 * @note �o�ܓx���ꂼ���m_dScale����Z�B
 *
 * @param ipFeature [in] �_�t�B�[�`��
 * @param lpPoint [in] ���_���W
 * @retval TRUE ����
 * @retval FALSE ������NULL�BipFeature���_�t�B�[�`���ł͂Ȃ��B
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
 * @brief �o�ܓx�����_���W�ɕϊ�
 *
 * @note �o�ܓx���ꂼ���m_dScale����Z�B
 *
 * @param ipGeoPoint [in] �o�ܓx
 * @param lpViewPoint [in] ���_���W
 * @retval TRUE ����
 * @retval FALSE ������NULL
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
 * @brief ���_���W���o�ܓx�ɕϊ�
 *
 * @note ���_���W����m_dScale�����Z�B
 *
 * @param lpPoint [in] ���_���W
 * @param dX [in] �o�x
 * @param dY [in] �ܓx
 * @retval TRUE ����
 * @retval lpPoint��NULL�Bm_dScale��0�B
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
 * @brief ���_���W��`���o�ܓx�ɕϊ�
 *
 * @note ���_���W����m_dScale�����Z�B
 *
 * @param lpRect [in] ���_���W��`
 * @param dXMin [in] �ŏ��o�x
 * @param dYMin [in] �ŏ��ܓx
 * @param dXMax [in] �ő�o�x
 * @param dYMax [in] �ő�ܓx
 * @retval TRUE ����
 * @retval lpPoint��NULL�Bm_dScale��0�B
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

	// �`��̈�
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
 * @brief �N���C�A���g���W�����_���W�ɕϊ�
 *
 * @param lpPos [in] �N���C�A���g���W
 * @retval TRUE ����
 * @retval lpPos��NULL
 */
inline BOOL SiNDYDraw::ClientToViewport(LPPOINT lpPos) const
{
	if(! lpPos)
		return FALSE;

	// �`��̈�
	CRect rectClient;
	GetClientRect(&rectClient);

	POINT posClientCenter = rectClient.CenterPoint();

	CSize sz = CPoint(*lpPos) - posClientCenter;
	lpPos->x = static_cast<LONG>(m_dCenterX * m_dScale + sz.cx);
	lpPos->y = static_cast<LONG>(m_dCenterY * m_dScale - sz.cy);

	return TRUE;
}

/**
 * @brief �N���C�A���g��`�̍��W�����_���W�ɕϊ�
 *
 * @param lpRect [in] �N���C�A���g��`
 * @retval TRUE ����
 * @retval lpRect��NULL
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
