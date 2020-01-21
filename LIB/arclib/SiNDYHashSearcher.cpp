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
#pragma warning (disable : 4786)
#include <sstream>
#include "SiNDYAttrCommunity.h"
#include "SiNDYHashSearcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// 
// HashSearcher
// 
//////////////////////////////////////////////////////////////////////

/**
 * @param ipFeatureWorkspace [in] �t�B�[�`�����[�N�X�y�[�X�B
 * @param bstrTableName [in] �e�[�u�����B
 * @param ipQueryFilter [in] �N�G���t�B���^�B
 * @param nStep [in] ��x�Ɏ��o�����B
 */
HashSearcher::HashSearcher(
	IFeatureWorkspace* ipFeatureWorkspace,
	BSTR bstrTableName,
	IQueryFilter* ipQueryFilter,
	int nStep) :
m_ipQueryFilter(ipQueryFilter),
m_ObjectIdRange(ipFeatureWorkspace, bstrTableName),
m_nStep(nStep),
m_nCurrent(m_ObjectIdRange.first)
{
	// �N�G���t�B���^���p�ӂ���Ă���Ƃ��́A��������Where������o��
	if(m_ipQueryFilter == 0) {
		m_ipQueryFilter.CreateInstance(CLSID_QueryFilter);
	}
	else {
		USES_CONVERSION;
		CComBSTR bstrWhereClause;
		m_ipQueryFilter->get_WhereClause(&bstrWhereClause);
		m_strWhereClause = OLE2T(bstrWhereClause);
	}
}

/**
 * @brief ����Search()�ɗp����N�G���t�B���^���擾����
 *
 * @return ����Search()�ɗp����N�G���t�B���^�B���̂�m_ipQueryFilter�B
 */
IQueryFilter* HashSearcher::nextQueryFilter()
{
	if(m_nCurrent > m_ObjectIdRange.second)
		return 0;

#ifdef _UNICODE
	std::wostringstream aWhereClauseStream;
#else
	std::ostringstream aWhereClauseStream;
#endif // _UNICODE

	std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
	aWhereClauseStream.imbue(loc);a
	bool bHaveWhereClause = ! m_strWhereClause.empty();

	if(bHaveWhereClause) {
		aWhereClauseStream << _T('(');
	}
	aWhereClauseStream << kObjectId << _T(" BETWEEN ") << m_nCurrent << _T(" AND ") << m_nCurrent + m_nStep - 1;
	if(bHaveWhereClause) {
		aWhereClauseStream << _T(") AND (") << m_strWhereClause.c_str() << _T(')');
	}
	aWhereClauseStream << std::ends;

	m_nCurrent += m_nStep;

	CComBSTR bstrWhereClause = aWhereClauseStream.str().c_str();

	if(m_ipQueryFilter->put_WhereClause(CComBSTR(aWhereClauseStream.str().c_str())) == S_OK)
		return m_ipQueryFilter;
	else
		return 0;
}
