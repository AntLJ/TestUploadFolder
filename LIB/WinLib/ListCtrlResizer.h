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
// 定義
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief レポート式リストコントロールの各列を、その列に存在する文字列を表示するのにちょうど良い幅にする関数クラス
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
// ListCtrlResizer 実装

inline ListCtrlResizer::ListCtrlResizer(HWND hWnd) :
m_lc(hWnd),
m_hc(m_lc.GetHeader())
{
}

/**
 * @brief 全ての列の幅調整を行う
 */
inline void ListCtrlResizer::operator()()
{
	int nColCount = m_hc.GetItemCount();
	for(int i = 0; i < nColCount; i++) {
		resizeColumn(i);
	}
}

/**
 * @brief 列の幅調整を行う
 *
 * @param nCol [in] 列番号。
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
 * @brief 列の最長文字列幅を取得する
 *
 * @note チェックボックスが存在する場合はその幅も考慮。
 * @note ヘッダの文字列は非考慮。
 *
 * @param rListCtrl [in] リストコントロール。
 * @param nCol [in] 列番号。
 * @return 最長文字列幅。
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
