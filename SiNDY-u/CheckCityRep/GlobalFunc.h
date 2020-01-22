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

/**
 * @file  GlobalFunc.h
 * @brief �O���[�o���֐���`
 */

namespace gf
{
	/**
	 * @brief  ���[�N�X�y�[�X�擾(���̂��ڑ����s���邱�Ƃ�����̂�5�񃊃g���C)
	 * @note   SDE PGDB SHAPE ��������n�j
	 * @param  lpcszDBProp	 [in]	�c�a�ڑ��v���p�e�B
	 * @retval IWorkspacePtr ���[�N�X�y�[�X
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief  ���ϐ��擾
	 * @param  lpcszEnv [in] ���ϐ���
	 * @retval CString  �w�肵�����ϐ��̏��
	 */
	inline CString GetEnvValue( const _TCHAR* lpcszEnv )
	{
		CString strTmp;
		return strTmp.GetEnvironmentVariable(lpcszEnv)? strTmp : NULL;
	}

	/**
	 * @brief  ���ϐ�������A���[�N�X�y�[�X�ƃt�B�[�`���N���X���I�[�v��
	 * @param  lpcszEnv    [in]  ���ϐ���
	 * @param  ipWorkspace [out] ���[�N�X�y�[�X
	 * @param  ipFC        [out] �t�B�[�`���N���X
	 * @retval true  �I�[�v������
	 * @retval false ���s
	 */
	bool GetFCFromEnv( const _TCHAR* lpcszEnv, IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFC );

	/**
	 * @brief  ��Ԍ��� (+ ��������)
	 * @note   [Bug 6419]�����G���[�������́A������-1��ݒ肵�܂�
	 * @param  ipGeo			[in]	�Ώۂ̃W�I���g��
	 * @param  ipFeatureClass	[in]	�������̃t�B�[�`���N���X
	 * @param  pCount			[out]	�������R�[�h��([Bug 6175]�����擾���Ȃ��Ȃ�ANULL�w��)
	 * @param  spatialRel		[in]	��������
	 * @param  lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param  lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @retval �t�B�[�`���J�[�\��(�������R�[�h�� = 0�Ȃ�ANULL�Ԃ�)
	 */
	IFeatureCursorPtr SelectByShape( IGeometryPtr& ipGeo, IFeatureClassPtr& ipFeatureClass, long* pCount = NULL,
									 esriSpatialRelEnum spatialRel = esriSpatialRelIntersects,
									 LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief  ���ݎ������uYYYY/MM/DD hh:mm:ss�v�̌`�Ŏ擾
	 * @retval CString ����
	 */
	inline CString GetCurrentTime()
	{
		CString strTime;
		SYSTEMTIME	st;
		GetLocalTime(&st);
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"),
						st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		return strTime;
	}
}
