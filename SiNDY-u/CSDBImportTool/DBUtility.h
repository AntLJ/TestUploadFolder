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

// DBUtility.h: CDBUtility �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_)
#define AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "schema.h"

namespace DBUtility
{

IWorkspacePtr OpenSDEWorkspace(LPCTSTR lpcszUser, LPCTSTR lpcszPassword, LPCTSTR lpcszVersion, LPCTSTR lpcszInstance, LPCTSTR lpcszServer);
IFeatureClassPtr OpenFeatureClass(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);
ITablePtr OpenTable(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);
ISpatialReferencePtr GetSpatialReference(IFeatureClassPtr ipFeatureClass);
DOUBLE CalcDistance(IPointPtr ipPoint1, IPointPtr ipPoint2);

/**
* @brief	�q�o�[�W�������쐬���A���̃��[�N�X�y�[�X��Ԃ�
*
* @note		���ɓ����̃o�[�W�������������ꍇ�͏����Ă���V���ɍ�蒼��
*
* @param	ipWorkspace			[in]	�e�o�[�W�����̃��[�N�X�y�[�X
* @param	strVersionName		[in]	���o�[�W�����̃o�[�W������
* @return	�V����������q�o�[�W�����̃��[�N�X�y�[�X
*/
IWorkspacePtr CreateEditVersion( IWorkspacePtr ipWorkspace, LPCTSTR strVersionName );

/**
* @brief	����t�B�[�`���N���X�Ɠ����t�B�[���h�����t�B�[�`���N���X�������̃f�[�^�Z�b�g���ɐV�K�쐬����
*
* @param	lpcszName			[in]	���t�B�[�`���N���X�̖��O
* @param	ipImitativeFeature	[in]	���̃t�B�[�`���N���X�̃t�B�[���h�\����^����
* @param	ipTargetWorkspace	[in]	���̃��[�N�X�y�[�X�ɍ쐬����B
                                        NULL�Ȃ�ipImitativeFeature�Ɠ������[�N�X�y�[�X�i�f�t�H���g�j
* @param	lpcszDatasetname	[in]	���̃f�[�^�Z�b�g���ɍ쐬����B�K�����݂���O��
                                        NULL�Ȃ�ipImitativeFeature �Ɠ����f�[�^�Z�b�g�i�f�t�H���g�j
* @return	������t�B�[�`���N���X
*/
IFeatureClassPtr ImitateFeatureClass( LPCTSTR lpcszName, IFeatureClass* ipImitativeFeature, IWorkspace* ipTargetWorkspace = NULL, LPCTSTR lpcszDatasetname = NULL );

// �t�B�[�`���N���X���烏�[�N�X�y�[�X���擾
IWorkspacePtr GetWorkspace( IFeatureClass* ipFeatureClass );

// �t�B�[�`���N���X���폜
void DeleteFeatureClass( IFeatureClass* ipFeatureClass );

// �f�[�^�Z�b�g���擾
IDatasetPtr GetDataset( IWorkspace* ipWorkspace, LPCTSTR lpcszDatasetName );

IFeatureDatasetPtr OpenDataset(IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);

IDatasetPtr CreateDataset( IWorkspacePtr ipWorkspace, LPCTSTR lpcszName);

// ���[�N�X�y�[�X�Đڑ�
void ReOpenWorkspace( IWorkspace* ipWorkspace );

// �t�B�[�`���N���X�쐬
IFeatureClassPtr CreateFeatureClass(IWorkspacePtr ipWorkspace, IDataset* ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType );

IFeatureClassPtr CreateFeatureClassInDataset(IDatasetPtr ipDataset, LPCTSTR lpcszLayerName, IFieldsPtr ipFields, esriFeatureType eType );


// �t�B�[���h�쐬
IFieldPtr CreateField( esriFieldType cType, CString cFieldName, LPCTSTR cAliasName, long cLength, long cPrecision, long cScale, VARIANT_BOOL cIsNullable, VARIANT_BOOL cEditable );

IFieldPtr CreateShapeField(LPCTSTR lpcszFieldName, esriGeometryType eGeometryType, bool bHasM, bool bHasZ );

IFieldsPtr CreateFieldsFromSchema( tFieldSchema tFieldinfo[], int nSize );

IFieldsPtr CreateFCFields( tFieldSchema tFieldinfo[] );


template <class T>
LONG GetFieldIndex(T ipFeature, LPCTSTR lpcszFieldName)
{
	IFieldsPtr ipFields = 0;
	ipFeature->get_Fields(&ipFields);
	LONG nIndex = 0;
	ipFields->FindField(CComBSTR(lpcszFieldName), &nIndex);
	return nIndex;
}

template <class T>
VOID PutValue(T ipFeature, LPCTSTR lpcszFieldName, VARIANT vaVal)
{
	ipFeature->put_Value(GetFieldIndex<T>(ipFeature, lpcszFieldName), vaVal);
	ipFeature->Store();
}

template <class T>
VOID GetValue(T ipFeature, LPCTSTR lpcszFieldName, VARIANT *vaVal)
{
	ipFeature->get_Value(GetFieldIndex<T>(ipFeature, lpcszFieldName), vaVal);
}

}

#endif // !defined(AFX_DBUTILITY_H__60E28852_A288_4034_96D6_7DB4606368D6__INCLUDED_)
