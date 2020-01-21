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

// EnumRow.cpp: _CEnumRow クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnumRow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

_CEnumRow::_CEnumRow() :
m_nCur(0)
{

}

_CEnumRow::~_CEnumRow()
{
	for(std::vector<_IRow*>::const_iterator it =  m_cRowList.begin(); it != m_cRowList.end(); ++it) {
		_IRow* ipRow = *it;
		ipRow->Release();
	}
}

/////////////////////////////////////////////////////////////////////////////
// IEnumRow

STDMETHODIMP _CEnumRow::get_Count(LONG * Count)
{
	if (Count == NULL)
		return E_POINTER;

	*Count = m_cRowList.size();

	return S_OK;
}

STDMETHODIMP _CEnumRow::Next(_IRow * * NextRow)
{
	if (NextRow == NULL)
		return E_POINTER;
	
	*NextRow = 0;

	if(m_nCur >= m_cRowList.size())
		return S_FALSE;
	
	*NextRow = m_cRowList[m_nCur];
	(*NextRow)->AddRef();
	m_nCur++;

	return S_OK;
}

STDMETHODIMP _CEnumRow::Reset()
{
	m_nCur = 0;

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CEnumRoadCodeRow

/**
 * @brief _IRowを末尾に追加する
 *
 * @param ipRow [in] 対象IRow
 */
void _CEnumRow::push_back(_IRow* ipRow)
{
	if(! ipRow)
		return;

	ipRow->AddRef();
	m_cRowList.push_back(ipRow);
}

/**
 * @brief 末尾のIRowを除外する
 */
void _CEnumRow::pop_back()
{
	if(m_cRowList.empty())
		return;

	(*m_cRowList.rbegin())->Release();
	m_cRowList.pop_back();
}
