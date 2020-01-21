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

// EnumBSTR.h: CEnumBSTR クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENUMBSTR_H__C68BDA4D_9B92_4CE4_ACCB_21295E76B218__INCLUDED_)
#define AFX_ENUMBSTR_H__C68BDA4D_9B92_4CE4_ACCB_21295E76B218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

/**
 * @brief IEnumBSTRの中身。その一つの実装型。
 */
class _CEnumBSTR :
	public IEnumBSTR,
	public CComObjectRoot
{
public:
	_CEnumBSTR();
	~_CEnumBSTR();

// IUnknown
	BEGIN_COM_MAP(_CEnumBSTR)
		COM_INTERFACE_ENTRY(IEnumBSTR)
	END_COM_MAP()

// IEnumRow
	STDMETHOD(Next)(BSTR* pBSTR);
	STDMETHOD(Reset)();

// CEnumRow
	void push_back(LPCSTR lpszStr);
	void push_back(LPCWSTR lpszStr);
	void pop_back();

private:
	std::vector<BSTR> m_cBstrList;
	std::vector<BSTR>::size_type m_nCur;
};

typedef CComObject<_CEnumBSTR> CEnumBSTR;

#endif // !defined(AFX_ENUMBSTR_H__C68BDA4D_9B92_4CE4_ACCB_21295E76B218__INCLUDED_)
