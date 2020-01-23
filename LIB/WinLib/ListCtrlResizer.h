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

#ifndef __LISTCTRL_RESIZER_H__
#define __LISTCTRL_RESIZER_H__

#include "ATLControls.h"

/////////////////////////////////////////////////////////////////////////////
// ��`
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief ���|�[�g�����X�g�R���g���[���̊e����A���̗�ɑ��݂��镶�����\������̂ɂ��傤�Ǘǂ����ɂ���֐��N���X
 */
class ListCtrlResizer
{
public:
	ListCtrlResizer(HWND hWnd);

	void operator()();

	static int calcColumnWidthToFit(ATLControls::CListViewCtrl& rListCtrl, int nCol);

private:
	void resizeColumn(int nCol);

	ATLControls::CListViewCtrl m_lc;
	ATLControls::CHeaderCtrl m_hc;
	HDITEM m_HdItem;
};

/////////////////////////////////////////////////////////////////////////////
// ListCtrlResizer ����

inline ListCtrlResizer::ListCtrlResizer(HWND hWnd) :
m_lc(hWnd),
m_hc(m_lc.GetHeader())
{
}

/**
 * @brief �S�Ă̗�̕��������s��
 */
inline void ListCtrlResizer::operator()()
{
	int nColCount = m_hc.GetItemCount();
	for(int i = 0; i < nColCount; i++) {
		resizeColumn(i);
	}
}

/**
 * @brief ��̕��������s��
 *
 * @param nCol [in] ��ԍ��B
 */
inline void ListCtrlResizer::resizeColumn(int nCol)
{
	TCHAR sz[MAX_PATH];
	m_HdItem.mask = HDI_TEXT;
	m_HdItem.pszText = sz;
	m_HdItem.cchTextMax = MAX_PATH;

	int nHeaderColWidth = m_hc.GetItem(nCol, &m_HdItem) ? m_lc.GetStringWidth(m_HdItem.pszText) : 0;
	int nItemColWidth = calcColumnWidthToFit(m_lc, nCol);
	int nWidth = nHeaderColWidth > nItemColWidth ? nHeaderColWidth : nItemColWidth;

	if(nWidth > 0) {
		m_lc.SetColumnWidth(nCol, nWidth + 12);
	}
}

/**
 * @brief ��̍Œ������񕝂��擾����
 *
 * @note �`�F�b�N�{�b�N�X�����݂���ꍇ�͂��̕����l���B
 * @note �w�b�_�̕�����͔�l���B
 *
 * @param rListCtrl [in] ���X�g�R���g���[���B
 * @param nCol [in] ��ԍ��B
 * @return �Œ������񕝁B
 */
inline int ListCtrlResizer::calcColumnWidthToFit(ATLControls::CListViewCtrl& rListCtrl, int nCol)
{
	TCHAR sz[MAX_PATH];
	int nMaxWidth = 0;

	for(int i = 0; i < rListCtrl.GetItemCount(); i++) {
		if(rListCtrl.GetItemText(i, nCol, sz, MAX_PATH) > 0) {
			int nWidth = rListCtrl.GetStringWidth(sz);
			if(nWidth > nMaxWidth) {
				nMaxWidth = nWidth;
			}
		}
	}

	if(nMaxWidth && nCol == 0 && rListCtrl.GetExtendedListViewStyle() & LVS_EX_CHECKBOXES) {
		nMaxWidth += 15;
	}

	return nMaxWidth;
}

#endif // __LISTCTRL_RESIZER_H__
