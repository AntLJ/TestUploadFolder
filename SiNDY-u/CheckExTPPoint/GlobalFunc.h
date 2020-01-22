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

namespace gf
{

	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @param	lpcszDBProp	[in]	�ڑ��v���p�e�B
	 * @return	���[�N�X�y�[�X
	 */
	inline IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
	{
		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( NULL == ipName )
			return NULL;
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		return ipUnknown;
	}

	/**
	 * @brief	��Ԍ��� (+ ��������)
	 * @param	ipGeo			[in]	�Ώۂ̃W�I���g��
	 * @param	ipFeatureClass	[in]	�������̃t�B�[�`���N���X
	 * @param	ppFeatureCursor	[out]	�������ʂ̃J�[�\��
	 * @param	spatialRel		[in]	��������
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @return	�����Ɉ�v��������
	 */
	LONG SelectByShapeWithCount( IGeometryPtr ipGeo,
								 IFeatureClassPtr ipFeatureClass,
								 IFeatureCursor** ppFeatureCursor,
				 				 esriSpatialRelEnum spatialRel,
								 LPCTSTR lpszWhereClause = NULL,
								 LPCTSTR lpszSubFields = NULL );

}
