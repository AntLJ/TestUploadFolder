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

// MeshDraw.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "MeshDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMeshDraw

COLORREF CMeshDraw::m_sForeColor = RGB(191,191,191);
COLORREF CMeshDraw::m_sBkColor = RGB(31,31,31);
COLORREF CMeshDraw::m_sMeshBrushColor = RGB(63,63,63);
COLORREF CMeshDraw::m_sSelectColor = RGB(255,255,0);

CPen CMeshDraw::m_s2ndMeshPen(PS_SOLID, 1, m_sForeColor);
CPen CMeshDraw::m_sSelectedMeshPen(PS_SOLID, 1, m_sSelectColor);

CBrush CMeshDraw::m_sMeshBrush(m_sMeshBrushColor);
CBrush CMeshDraw::m_sSelectedMeshBrush(m_sSelectColor);

CMeshDraw::CMeshDraw() :
m_dMeshWidth(2.0) ,
m_dMeshHeight(2.0) ,
m_pntOffset(600, 840),
m_bDragging(FALSE)
{
}

CMeshDraw::~CMeshDraw()
{
}


BEGIN_MESSAGE_MAP(CMeshDraw, CStatic)
	//{{AFX_MSG_MAP(CMeshDraw)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMeshDraw ���b�Z�[�W �n���h��

void CMeshDraw::OnPaint() 
{
	CPaintDC dc(this); // �`��p�̃f�o�C�X �R���e�L�X�g
	
	// �`��̈�
    CRect rect;
    GetClientRect(&rect);

	// �N���b�v�̈����
	dc.IntersectClipRect(&rect);

	// �̈�h��Ԃ�
	RECT rectClip;
	dc.GetClipBox(&rectClip);
	dc.FillSolidRect(&rectClip, m_sBkColor);

	// �r���[�|�[�g�ړ�
	CPoint	org = rect.CenterPoint();
	dc.SetViewportOrg(org);
	dc.OffsetViewportOrg(- m_pntOffset.x, m_pntOffset.y);

	// �㉺���]
	dc.SetMapMode(MM_ANISOTROPIC);
	dc.SetViewportExt(1, -1);

	// ���b�V���R�[�h�ݒ肪�Ȃ�������p�X
	if(! m_cMeshCodeSet.size())
		return;

	// �T���v���Ƃ��ă��b�V���`��擾
	CSize sizeMeshSample((int)m_dMeshWidth, (int)m_dMeshHeight);
	BOOL bRectIsSmall = sizeMeshSample.cx < 2 || sizeMeshSample.cy < 2;

	// �t�H���g�p��
	CFont* pFontCurrent = dc.GetCurrentFont();

	CFont font;
	font.CreatePointFont(80, "�l�r �S�V�b�N");
	dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);

	// �����T�C�Y��r
	CSize sizeFont = dc.GetTextExtent(_T("999999"), 6);
	BOOL bWriteMeshCode = (sizeFont.cx+2) < sizeMeshSample.cx && (sizeFont.cy+2) < sizeMeshSample.cy;
	sizeFont.cy = -sizeFont.cy;

	dc.SelectObject(&m_s2ndMeshPen);
	for(MeshCodeSet::const_iterator itr = m_cMeshCodeSet.begin(); itr != m_cMeshCodeSet.end(); ++itr) {
		const MeshCode& rMeshCode = *itr;
		CRect rectMesh(GetMeshRect(rMeshCode));
		BOOL bSelectedMesh = m_cSelectedMeshCodeSet.find(rMeshCode) != m_cSelectedMeshCodeSet.end();

		dc.SelectObject(bSelectedMesh ? &m_sSelectedMeshBrush : &m_sMeshBrush);

		// ���b�V�����N���b�v�͈͓��ɑ��݂��Ă�����`��
		if(dc.RectVisible(&rectMesh)) {
			if(bRectIsSmall) {
				POINT points[2] = { {rectMesh.left, rectMesh.top}, {rectMesh.right, rectMesh.bottom} };
				dc.Polyline(points, 2);
			}
			else {
				dc.SelectObject(m_s2ndMeshPen);
				dc.Rectangle(&rectMesh);
				if(bWriteMeshCode) {
					CString strMeshCode;
					strMeshCode.Format(_T("%6d"), rMeshCode);

					dc.SetTextColor(bSelectedMesh ? m_sBkColor : m_sForeColor);
					dc.DrawText(strMeshCode, &rectMesh, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
			}
		}
	}

	if(bRectIsSmall) {
		dc.SelectObject(&m_sSelectedMeshPen);
		for(MeshCodeSet::const_iterator itr = m_cSelectedMeshCodeSet.begin(); itr != m_cSelectedMeshCodeSet.end(); ++itr) {
			const MeshCode& rMeshCode = *itr;
			RECT rectMesh(GetMeshRect(rMeshCode));
			if(dc.RectVisible(&rectMesh)) {
				dc.MoveTo(rectMesh.left, rectMesh.top);
				dc.LineTo(rectMesh.right-1, rectMesh.bottom-1);
			}
		}
	}

	// �t�H���g��߂�
	dc.SelectObject(pFontCurrent);
}

void CMeshDraw::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_bDragging = TRUE;
	m_pntPrevious = point;
}

void CMeshDraw::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_bDragging = FALSE;
}

void CMeshDraw::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(! m_bDragging)
		return;

	RECT rect;
	GetClientRect(&rect);

	m_pntOffset.x += m_pntPrevious.x - point.x;
	m_pntOffset.y -= m_pntPrevious.y - point.y;
	ScrollWindow(- (m_pntPrevious.x - point.x), - (m_pntPrevious.y - point.y), NULL, &rect);

	m_pntPrevious = point;
}

/////////////////////////////////////////////////////////////////////////////
// �ݒ�
/////////////////////////////////////////////////////////////////////////////

void CMeshDraw::appendSelectBy(std::istream& rStream)
{
	CWaitCursor wait;

	int nMeshCode;
	while(rStream >> nMeshCode) {
		appendSelect(nMeshCode);
	}
}

/////////////////////////////////////////////////////////////////////////////
// �擾
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �w�肵�����b�V���̋�`���擾����
 *
 * @param nMeshCode [in] �Ώۃ��b�V���R�[�h
 * @return ��`
 */
RECT CMeshDraw::GetMeshRect(int nMeshCode)
{
	RNSMesh cMeshCode(nMeshCode);
	int nNorth = cMeshCode.northSouthDec();
	int nEast = cMeshCode.eastWestDec();

	return CRect(
		(int)(nEast * m_dMeshWidth), (int)((nNorth) * m_dMeshHeight), 
		(int)((nEast+1) * m_dMeshWidth + 1), (int)((nNorth+1) * m_dMeshHeight + 1)
	);
}

/////////////////////////////////////////////////////////////////////////////
// �ړ�
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief �Œ�{���Ŋg�傷��
 */
void CMeshDraw::zoomIn()
{
	zoom(1.2);
}

/**
 * @brief �Œ�{���ŏk������
 */
void CMeshDraw::zoomOut()
{
	zoom(0.8);
}

/**
 * @brief �w�肵���{���Ŋg��E�k������
 *
 * @param dScale [in] �{��
 */
void CMeshDraw::zoom(double dScale)
{
	m_dMeshWidth *= dScale;
	m_dMeshHeight *= dScale;
	m_pntOffset.x = (int)(m_pntOffset.x * dScale);
	m_pntOffset.y = (int)(m_pntOffset.y * dScale);
	Invalidate();
}
