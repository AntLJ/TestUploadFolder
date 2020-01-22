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

#pragma once

#include <sindy/workspace.h>

typedef std::map< long, CString > DOMAINMAP;	/// �h���C���}�b�v

namespace gf
{
	/**
	 * @brief	PGDB���[�h���ۂ�
	 * @return	bool
	 */
	inline bool IsPGDBMode()
	{
		return NULL != _tgetenv(_T("PGDB_MODE"));
	}

	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @param	lpcszDBProp	[in]	�ڑ��v���p�e�B
	 * @return	IWorkspace
	 */
	inline IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( NULL == ipName )
			return NULL;
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		return ipUnknown;
	}

	/**
	 * @brief	�h���C���l�E�h���C�����}�b�v�쐬
	 * @param	ipTable			[in]	�h���C���t�B�[���h�����e�[�u��
	 * @param	lpcszFieldName	[in]	�h���C�������t�B�[���h��
	 * @param	map				[in,out]�h���C���}�b�v�i�����ꍇ�̓T�C�Y0�̃h���C���}�b�v��Ԃ��j
	 * @return	bool
	 */
	bool GetDomainMap( ITablePtr ipTable, LPCTSTR lpcszFieldName, DOMAINMAP* map );

	/**
	 * @brief	��Ԍ��� (+ ��������)
	 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
	 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
	 * @patam	ipFeatureCursor	[out]	�t�B�[�`���J�[�\��
	 * @param	pCount			[out]	�������ʃ��R�[�h��
	 * @param	spatialRel		[in]	��������
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @return	bool
	 */
	bool SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long* pCount,
								 esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief	��Ԍ��� (+ ��������)
	 * @note	�����̂�
	 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
	 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
	 * @param	spatialRel		[in]	��������
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @return	���R�[�h����(�G���[������ -1)
	 */
	long SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause = NULL );
}
