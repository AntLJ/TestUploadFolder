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

// EnumRow.h: CEnumRow クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMROW_H__7D529547_C6BF_4FB7_AFF0_6C71EC9FBED5__INCLUDED_)
#define AFX_ENUMROW_H__7D529547_C6BF_4FB7_AFF0_6C71EC9FBED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

/**
 * @brief IEnumRowの中身。その一つの実装型。
 */
class _CEnumRow :
	public IEnumRow,
	public CComObjectRoot
{
public:
	_CEnumRow();
	~_CEnumRow();

// IUnknown
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(_CEnumRow)
		COM_INTERFACE_ENTRY(IEnumRow)
	END_COM_MAP()

// IEnumRow
	STDMETHOD(get_Count)(LONG * Count);
	STDMETHOD(Next)(_IRow * * NextRow);
	STDMETHOD(Reset)();

// CEnumRow
	void push_back(_IRow* ipRow);
	void pop_back();

private:
	std::vector<_IRow*> m_cRowList;
	std::vector<_IRow*>::size_type m_nCur;
};

typedef CComObject<_CEnumRow> CEnumRow;

#endif // !defined(AFX_ENUMROW_H__7D529547_C6BF_4FB7_AFF0_6C71EC9FBED5__INCLUDED_)
