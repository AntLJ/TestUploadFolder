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

#if !defined(AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_)
#define AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MeshDraw.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMeshDraw �E�B���h�E

class CMeshDraw : public CStatic
{
// �R���X�g���N�V����
public:
	CMeshDraw();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMeshDraw)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMeshDraw();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMeshDraw)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
// ��`
	typedef int MeshCode;
	typedef std::set<MeshCode> MeshCodeSet;

// �ݒ�
	void setMeshCodeSet(const MeshCodeSet& rMeshCodeSet) { m_cMeshCodeSet = rMeshCodeSet; }

	void setSelect(const MeshCodeSet& rMeshCode) { m_cSelectedMeshCodeSet = rMeshCode; }
	void appendSelect(const MeshCode& rMeshCode) { m_cSelectedMeshCodeSet.insert(rMeshCode); }
	void appendSelectBy(std::istream& rStream);

	void clearSelect() { m_cSelectedMeshCodeSet.clear(); }

// �擾
	const MeshCodeSet& meshCodeSet() const { return m_cMeshCodeSet; }
	const MeshCodeSet& selectedMeshCodeSet() const { return m_cSelectedMeshCodeSet; }

// �ړ�
	void zoomIn();
	void zoomOut();
	void zoom(double dScale);

protected:
// �Q�����b�V����`�擾
	RECT GetMeshRect(int nMeshCode);

// �ÓI�v���p�e�B
	static COLORREF m_sForeColor; ///< �O�i�F
	static COLORREF m_sBkColor; ///< �w�i�F
	static COLORREF m_sMeshBrushColor; ///< ���b�V���w�i�F
	static COLORREF m_sSelectColor; ///< �I��F

	static CPen m_s2ndMeshPen; ///< �Q�����b�V���y��
	static CPen m_sSelectedMeshPen; ///< �I�����b�V���y��

	static CBrush m_sMeshBrush; ///< ���b�V���h��Ԃ��u���V
	static CBrush m_sSelectedMeshBrush; ///< �I�����b�V���h��Ԃ��u���V

private:
// �v���p�e�B
	MeshCodeSet m_cMeshCodeSet; ///< �S���b�V���R�[�h�Z�b�g
	MeshCodeSet m_cSelectedMeshCodeSet; ///< �I�����ꂽ���b�V���R�[�h�Z�b�g

	double m_dMeshWidth; ///< ���b�V����
	double m_dMeshHeight; ///< ���b�V������
	CPoint m_pntOffset; ///< ���S�ʒu

	BOOL m_bDragging; ///< �h���b�O���H
	CPoint m_pntPrevious; ///< �h���b�O���̒��O�̃}�E�X�ʒu
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MESHDRAW_H__8DA5B93D_75D2_485F_84CE_5FA362AD9B0D__INCLUDED_)
