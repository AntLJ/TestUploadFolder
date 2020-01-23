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

#include "StdAfx.h"
#include "SDEManager.h"
#include <sindy/workspace.h>

CSDEManager::CSDEManager(void)
{
}

CSDEManager::~CSDEManager(void)
{
}

// �w��o�[�W�������擾����
bool CSDEManager::ConnectSDE(const LPCTSTR lpszUserName,
							 const LPCTSTR lpszPassword,
							 const LPCTSTR lpszInstance,
							 const LPCTSTR lpszServer,
							 const LPCTSTR lpszVersion,
							 IWorkspacePtr& ppWorkspace)
{
	// OLE2T ���̈ꕔ�̃}�N�����g�p
	USES_CONVERSION;

	CString connectionInfo;
	connectionInfo.Format(_T("%s/%s/%s/%s"), lpszUserName, lpszPassword, lpszVersion, lpszServer );
	// SDE�ڑ�
	ppWorkspace = sindy::create_workspace(connectionInfo);
	if( !ppWorkspace ) {
		return false;
	}
	return true;
}

//�^�[�Q�b�g���C���̃I�[�v��
bool CSDEManager::OpenTargetLayer(const LPCTSTR lpcszLayerName, IWorkspacePtr ipWorkspace, IFeatureClass **ppFeatureClass)
{
	// �t�B�[�`�����[�N�X�y�[�X�̍쐬
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	// �e�[�u���̃I�[�v���Ɏ��s������false��Ԃ�
	if( FAILED( ipFeatureWorkspace->OpenFeatureClass(CComBSTR(lpcszLayerName), ppFeatureClass))){
		cerr << "�Ώۃ��C��" << lpcszLayerName << "�̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	return true;
}

//�^�[�Q�b�g�e�[�u���̃I�[�v��
bool CSDEManager::OpenTargetTable(const LPCTSTR lpcszTableName, IWorkspacePtr ipWorkspace, ITable **ppTable)
{
	// �t�B�[�`�����[�N�X�y�[�X�̍쐬
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	if( FAILED( ipFeatureWorkspace->OpenTable(CComBSTR(lpcszTableName), ppTable))){
		cerr << "�Ώۃe�[�u��" << lpcszTableName << "�̃I�[�v���Ɏ��s" << endl;
		return false;
	}
	return true;
}	

//�J�[�\���̎擾
bool CSDEManager::GetCursor(ITablePtr ipTable, const LPCTSTR lpszWhereClause, _ICursor **ppCursor)
{
	IQueryFilterPtr		ipFilter(CLSID_QueryFilter);
						ipFilter->put_WhereClause(CComBSTR(lpszWhereClause));

	// �e�[�u�����������ăJ�[�\���Ɋi�[
	if( FAILED(ipTable->Search( ipFilter, VARIANT_FALSE, ppCursor ))){
		cerr << "�s�̌����Ɏ��s" << endl;
		return false;
	}	
	return true;
}

//�t�B�[�`���J�[�\���̎擾
bool CSDEManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, 
							   const LPCTSTR lpszWhereClause,
							   IFeatureCursor **ppFeatureCursor)
{
	IQueryFilterPtr		ipFilter(CLSID_QueryFilter);
						ipFilter->put_WhereClause(CComBSTR(lpszWhereClause));
	// �e�[�u�����������ăJ�[�\���Ɋi�[
	if( FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor ))){
		cerr << "�s�̌����Ɏ��s" << endl;
		return false;
	}
	return true;
}

// �^����ꂽRow�̎w�肳�ꂽ�t�B�[���h�̒l���擾
CComVariant CSDEManager::GetValue( _IRowPtr ipRow, LPCTSTR lpcszFieldName )
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;

	long lFieldIndex;
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}
	return vaValue;
}
