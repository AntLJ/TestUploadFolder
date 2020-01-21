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

// SiNDYTableName.cpp: SiNDYTableName �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <sindy/schema/road.h>
#include "SiNDYTableName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYTableName::SiNDYTableName() :
m_bIsInfTableName(false),
m_bIsLqTableName(false),
m_bIsRoadLinkTableName(false)
{
}

SiNDYTableName::SiNDYTableName(_IRow* ipRow)
{
	setTableName(ipRow);
}

SiNDYTableName::SiNDYTableName(ITable* ipTable)
{
	setTableName(IDatasetPtr(ipTable));
}

SiNDYTableName::SiNDYTableName(IFeatureClass* ipFeatureClass)
{
	setTableName(IDatasetPtr(ipFeatureClass));
}

SiNDYTableName::SiNDYTableName(IDataset* ipDataset)
{
	setTableName(ipDataset);
}

SiNDYTableName::SiNDYTableName(IDatasetName* ipDatasetName)
{
	setTableName(ipDatasetName);
}

SiNDYTableName::SiNDYTableName(const CString& rName)
{
	setTableName(rName);
}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

bool SiNDYTableName::setTableName(_IRow* ipRow)
{
	if(! ipRow)
		return false;

	ITablePtr ipTable;
	ipRow->get_Table(&ipTable);
	return setTableName(IDatasetPtr(ipTable));
}

bool SiNDYTableName::setTableName(IDataset* ipDataset)
{
	if(! ipDataset)
		return false;

	CComBSTR bstrName;
	ipDataset->get_Name(&bstrName);
	return setTableName(CString(bstrName));
}

bool SiNDYTableName::setTableName(IDatasetName* ipDatasetName)
{
	if(! ipDatasetName)
		return false;

	CComBSTR bstrName;
	ipDatasetName->get_Name(&bstrName);
	return setTableName(CString(bstrName));
}

bool SiNDYTableName::setTableName(const CString& rName)
{
	if(rName.IsEmpty())
		return false;

	m_strFullName = rName;
	m_strFullName.MakeUpper();

	int nFind = m_strFullName.Find(_T('.'));
	if(nFind >= 0) {
		m_strOwnerName = m_strFullName.Left(nFind);
		m_strTableName = m_strFullName.Mid(nFind + 1);
	}
	else {
		m_strTableName = m_strFullName;
	}

	m_bIsInfTableName = ::_tcsnicmp(_T("INF_"), m_strTableName, 4) == 0;
	m_bIsLqTableName = ::_tcsnicmp(_T("LQ_"), m_strTableName, 3) == 0;
	m_bIsRoadLinkTableName = m_strTableName.CompareNoCase(schema::road_link::kTableName) == 0;

	return true;
}

/**
 * @brief �^����ꂽ�e�[�u�����ɁA�I�[�i����⊮���ĕԂ�
 *
 * @note �^����ꂽ�e�[�u�����ɁA���ɃI�[�i�������݂��Ă����ꍇ�͎���������Ԃ��B
 *
 * @param lpszTableName [in] �e�[�u����
 * @return �⊮���ꂽ�e�[�u����
 */
CString SiNDYTableName::complementOwnerName(LPCTSTR lpszTableName) const
{
	if(lpszTableName == 0)
		return _T("");

	LPCTSTR lpszFind = ::_tcschr(lpszTableName, _T('.'));
	if(lpszFind)
		return lpszTableName;

	if(m_strOwnerName.IsEmpty())
		return lpszTableName;

	CString strResult;
	strResult.Format(_T("%s.%s"), m_strOwnerName, lpszTableName);

	return strResult;
}

/**
 * @brief Inf�e�[�u�������擾����
 *
 * @note Inf�e�[�u�������ALq�e�[�u�������i�[����Ă���ꍇ�̂ݗL���B
 *
 * @return Inf�e�[�u����
 */ 
CString SiNDYTableName::getInfFullNameString() const
{
	if(isInfTableName())
		return m_strFullName;

	if(! isLqTableName())
		return _T("");

	CString strTableName;
	strTableName.Format(_T("%s%s"), _T("INF_"), static_cast<LPCTSTR>(tableNameString()) + 3);
	return complementOwnerName(strTableName);
}

/**
 * @brief Lq�e�[�u�������擾����
 *
 * @note Inf�e�[�u�������ALq�e�[�u�������i�[����Ă���ꍇ�̂ݗL���B
 *
 * @return Lq�e�[�u����
 */ 
CString SiNDYTableName::getLqFullNameString() const
{
	if(isLqTableName())
		return m_strFullName;

	if(! isInfTableName())
		return _T("");

	CString strTableName;
	strTableName.Format(_T("%s%s"), _T("LQ_"), static_cast<LPCTSTR>(tableNameString()) + 4);
	return complementOwnerName(strTableName);
}

/**
 * @brief ���H�����N��LqRf�����t�B�[���h�����擾����
 *
 * @note Inf�e�[�u�������ALq�e�[�u�������i�[����Ă���ꍇ�̂ݗL���B
 *
 * @return LqRf�����t�B�[���h��
 */ 
CString SiNDYTableName::getLqRfFieldNameString() const
{
	CString strFieldName;
	if(isLqTableName()) {
		strFieldName.Format(_T("%s_LqRf"), static_cast<LPCTSTR>(tableNameString()) + 3);
	}
	else if(isInfTableName()) {
		strFieldName.Format(_T("%s_LqRf"), static_cast<LPCTSTR>(tableNameString()) + 4);
	}

	return strFieldName;
}

/**
 * @brief �����e�[�u�����w���Ă���ƍl���ėǂ���
 *
 * @param rTableName [in] ��r�Ώۃe�[�u��
 * @retval true ������
 * @retval false �قȂ�
 */
bool SiNDYTableName::isSameTable(const SiNDYTableName& rTableName) const
{
	if(m_strTableName != rTableName.m_strTableName)
		return false;

	return m_strOwnerName.IsEmpty() || rTableName.m_strOwnerName.IsEmpty() || m_strOwnerName.CompareNoCase(rTableName.m_strOwnerName) == 0;
}
