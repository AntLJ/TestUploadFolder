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

// SiNDYTablesBuffer.cpp: SiNDYTablesBuffer �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiNDYTablesBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// SiNDYTablesBuffer.cpp: SiNDYTablesBuffer �N���X�̃C���v�������e�[�V����
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

SiNDYTablesBuffer::SiNDYTablesBuffer()
{
}

SiNDYTablesBuffer::SiNDYTablesBuffer(IFeatureWorkspacePtr ipWorkspace) :
m_ipFeatureWorkspace(ipWorkspace)
{
}

SiNDYTablesBuffer::~SiNDYTablesBuffer()
{

}

//////////////////////////////////////////////////////////////////////
// �ݒ�/�擾
//////////////////////////////////////////////////////////////////////

void SiNDYTablesBuffer::setWorkspace(IFeatureWorkspacePtr ipWorkspace)
{
	m_cTableMap.clear();
	m_ipFeatureWorkspace = ipWorkspace;
}

ITablePtr SiNDYTablesBuffer::getTable(LPCTSTR lpszTableName)
{
	return getTable(CString(lpszTableName));
}

/**
 * @brief �e�[�u�����擾����
 *
 * @note �t�B�[�`���N���X�̖��̂��w�肵���ꍇ�A�t�B�[�`���N���X���擾����B
 * @note ��x�擾�����e�[�u���i�t�B�[�`���N���X�j�̓C���X�^���X�ɃL���b�V�������B
 *
 * @param strTableName [in] �e�[�u����
 * @return �w�肵���e�[�u���������e�[�u���B�擾�ł��Ȃ������ꍇNULL�B
 */
ITablePtr SiNDYTablesBuffer::getTable(CString strTableName)
{
	if(m_ipFeatureWorkspace == 0)
		return 0;

	ITablePtr ipTable;

	// �啶���Ő��񂵂Ă���
	strTableName.MakeUpper();
	TableMap::const_iterator it = m_cTableMap.find(strTableName);

	// �L���b�V��������
	if(it != m_cTableMap.end()) {
		// �L���b�V������Ă����炻���
		ipTable =it->second;
	}
	else {
		// ����Ă��Ȃ�������A�V�����J��
		IFeatureClassPtr ipFeatureClass;
		if(m_ipFeatureWorkspace->OpenFeatureClass(CComBSTR(strTableName), &ipFeatureClass) == S_OK) {
			ipTable = ipFeatureClass;
		}
		else {
			m_ipFeatureWorkspace->OpenTable(CComBSTR(strTableName), &ipTable);
		}
		m_cTableMap[strTableName] = ipTable;
	}

	return ipTable;
}

ITablePtr SiNDYTablesBuffer::getTable(const TableName& rTableName)
{
	return getTable(rTableName.fullNameString());
}
