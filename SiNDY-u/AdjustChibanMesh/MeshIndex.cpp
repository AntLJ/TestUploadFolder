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

// MeshIndex.cpp: MeshIndex �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MeshIndex.h"

//#define _PASCO
//////////////////////////////////////////////////////////////////////
// �\�z/����
//////////////////////////////////////////////////////////////////////

//�@�R���X�g���N�^
MeshIndex::MeshIndex()
: m_MeshIndex(-1)
{
}

//�@�R���X�g���N�^
MeshIndex::MeshIndex(IFeatureClassPtr ipFeature)
{
	m_ipFeature = ipFeature;
}

//�f�X�g���N�^
MeshIndex::~MeshIndex()
{
}

//�@�������B�i�����Ŏw��j
bool MeshIndex::init(LPCTSTR cMeshField, const CString &cWhere)
{
	//���b�V�����X�g��������Γs�s�n�}�G���A�S������WHERE������v�̂���
	/// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString aWhere = cWhere;
	if(cWhere != _T("")){
		aWhere += _T(" AND CREATE_YEAR IS NOT NULL");
	}else{
		aWhere = _T("CREATE_YEAR IS NOT NULL");		
	}
	ipFilter->put_WhereClause(CComBSTR(aWhere));

	LONG	lFeatureCount = 0;
	/// �������ʂ� 0 �Ȃ烊�^�[��
	m_ipFeature->FeatureCount( ipFilter, &lFeatureCount );
	if( lFeatureCount < 1 ) {
		_ftprintf(stderr, _T("�Y�����b�V����������܂���B\n"));
		return false;
	}

	LONG	lFieldIndex = 0;
	CComBSTR bstrName = cMeshField;
	m_ipFeature->FindField(bstrName, &lFieldIndex);
	if(lFieldIndex < 0){
		m_MeshIndex = -1;
		_ftprintf(stderr, _T("���b�V���R�[�h�̃t�B�[���h�����m�F���ĉ������B:%s\n"), cMeshField);
		return false;
	}else{
		m_MeshIndex = lFieldIndex;
	}
#if 0
	/// ����
	m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipCursor );
#else
	ITableSortPtr ipTableSort(CLSID_TableSort); 
	ipTableSort->put_Fields(bstrName);
	ipTableSort->putref_Table(ITablePtr(m_ipFeature));
	ipTableSort->putref_QueryFilter(ipFilter);
	ipTableSort->Sort(NULL);
	/// ����
	_ICursorPtr	ipCursor;
	ipTableSort->get_Rows(&ipCursor);
	m_ipCursor = IFeatureCursorPtr(ipCursor);
	//	m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipFeatureCursor );

//	return lFeatureCount;
#endif
	return true;
}

//�@�������B�i���b�V���R�[�h�Ŏw��j
bool MeshIndex::init(LONG cMeshCode, LPCTSTR cMeshField)
{
	/// �N�G���t�B���^�쐬
	IQueryFilterPtr ipFilter( CLSID_QueryFilter );
	CString aWhere;
#ifdef _PASCO
	aWhere.Format("%s = %d", cMeshField, cMeshCode);
#else
//	aWhere.Format("%s = %d", cMeshField, cMeshCode);
	aWhere.Format(_T("%s = %d AND CREATE_YEAR IS NOT NULL"), cMeshField, cMeshCode);
#endif
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	CString aSubField;
	aSubField.Format(_T("SHAPE,%s"), cMeshField);
	ipFilter->put_SubFields(CComBSTR(aSubField));

	LONG	lFeatureCount = 0;
	/// �������ʂ� 0 �Ȃ烊�^�[��
	m_ipFeature->FeatureCount( ipFilter, &lFeatureCount );
	if( lFeatureCount < 1 ) {
		_ftprintf(stderr, _T("�s�s�n�}�G���A���ɊY�����b�V����������܂���B,%d\n"), cMeshCode);
		return false;
	}
	LONG	lFieldIndex = 0;
	CComBSTR bstrName(cMeshField);
	m_ipFeature->FindField(bstrName, &lFieldIndex);
	if(lFieldIndex < 0){
		m_MeshIndex = -1;
		_ftprintf(stderr, _T("���b�V���R�[�h�̃t�B�[���h�����m�F���ĉ������B:%s\n"), cMeshField);
		return false;
	}else{
		m_MeshIndex = lFieldIndex;
	}
	/// ����
	if(SUCCEEDED(m_ipFeature->Search( ipFilter, VARIANT_FALSE, &m_ipCursor ))){
		return true;
	}else{
		_ftprintf(stderr, _T("���b�V���|���S���̌������s�B:%d\n"), cMeshCode);
		return false;
	}
}

//�@�i���́j�Ώۃ��b�V���|���S�����擾�B
IFeaturePtr MeshIndex::nextFeature(LONG *cpMeshCode)
{
	IFeaturePtr ipFeature;
	if(m_ipCursor->NextFeature( &ipFeature ) == S_OK){
		CComVariant vaValue;
		ipFeature->get_Value( m_MeshIndex, &vaValue );
		*cpMeshCode = vaValue.intVal;
	}else{
		*cpMeshCode = 0;
		ipFeature = NULL;
	}
	return ipFeature;
}
