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

// EnumBSTR.cpp: CEnumBSTR クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnumBSTR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

_CEnumBSTR::_CEnumBSTR() :
m_nCur(0)
{

}

_CEnumBSTR::~_CEnumBSTR()
{
	for(std::vector<BSTR>::const_iterator it =  m_cBstrList.begin(); it != m_cBstrList.end(); ++it) {
		::SysFreeString(*it);
	}
}

/////////////////////////////////////////////////////////////////////////////
// IEnumBSTR

STDMETHODIMP _CEnumBSTR::Next(BSTR* pBSTR)
{
	if (pBSTR == NULL)
		return E_POINTER;

	if(m_nCur >= m_cBstrList.size())
		return S_FALSE;
	
	*pBSTR = ::SysAllocString(m_cBstrList[m_nCur]);
	m_nCur++;

	return S_OK;
}

STDMETHODIMP _CEnumBSTR::Reset()
{
	m_nCur = 0;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// _CEnumBSTR

/**
 * @brief 文字列を末尾に追加する
 *
 * @param lpszStr [in] 対象文字列
 */
void _CEnumBSTR::push_back(LPCSTR lpszStr)
{
	USES_CONVERSION;
	push_back(A2BSTR(lpszStr));
}

/**
 * @brief 文字列を末尾に追加する
 *
 * @param lpszStr [in] 対象文字列
 */
void _CEnumBSTR::push_back(LPCWSTR lpszStr)
{
	if(lpszStr == NULL)
		return;

	m_cBstrList.push_back(::SysAllocString(lpszStr));
}

/**
 * @brief 末尾の文字列を除外する
 */
void _CEnumBSTR::pop_back()
{
	if(m_cBstrList.empty())
		return;

	::SysFreeString(*m_cBstrList.rbegin());
	m_cBstrList.pop_back();
}
