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
#include "define.h"
/**
 * @file	GlobalFunc.h
 * @brief	�O���[�o���֐���`
 */

namespace gf
{
	/**
	 * @brief	���[�N�X�y�[�X�擾
	 * @note	SDE PGDB FGDB ��������n�j
	 * @param	[in] lpcszDBProp		�c�a�ڑ��v���p�e�B
	 * @return	���[�N�X�y�[�X
	 */
	IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief	��������
	 * @note	�����G���[�������́A������-1��ݒ肵�܂�
	 * @param	ipTable			[in]	�������̃t�B�[�`���N���X
	 * @param	pCount			[out]	�������R�[�h�������擾���Ȃ��Ȃ�ANULL�w��)
	 * @param	lpszWhereClause	[in]	��������������Ȃ�w�肷��(�f�t�H���g��NULL)
	 * @param	lpszSubFields	[in]	�K�v�ȃt�B�[���h(�J���}��؂�)
	 * @return	�t�B�[�`���J�[�\��(�������R�[�h�� = 0�Ȃ�ANULL�Ԃ�)
	 */
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount = NULL, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );
	
	/**
	 * @brief	�}�[�W
	 * @param	[in] ipGeometryCollection �W�I���g���R���N�V����
	 * @param	[in] ipSpatialReference   ��ԎQ��
	 * @return	IGeometryPtr
	 */
	IGeometryPtr constructUnion( const IGeometryPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference );
	
	/**
	 * @brief	�|���S���̍���
	 * @param	[in] ipMeshGeometry    ���b�V���W�I���g��
	 * @param	[in] ipMergeGeometry   �}�[�W�W�I���g��
	 * @return	IGeometryPtr
	 */
	IGeometryPtr difference( const IGeometryPtr& ipMeshGeometry, const IGeometryPtr& ipMergeGeometry );

	/**
	 * @brief   �ڐG����
	 * @param   [in] ipBaseGeometry   �x�[�X�W�I���g��
	 * @param   [in] ipOtherGeometry  �x�[�X�Ƃ̐ڐG������s�������W�I���g��
	 * @retval  true(�ڐG���Ă���)�Afalse(�ڐG���Ă��Ȃ�)
	 */
	bool isTouches( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry );

	/**
	 * @brief   �܂ޔ���
	 * @param   [in] ipBaseGeometry   �x�[�X�W�I���g��
	 * @param   [in] ipOtherGeometry  �x�[�X�Ƃ̊܂ޔ�����s�������W�I���g��
	 * @retval  true(�܂ނ��Ă���)�Afalse(�܂ނ��Ă��Ȃ�)
	 */
	bool IsContains( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry );

	/**
	* @brief	�t�B�[���h�}�b�v�쐬
	* @param	[in] ipTable    �e�[�u��
	* @return  �t�B�[���h�}�b�v�l
	*/
	FIELDSMAP createFieldMap( const ITablePtr& ipTable );

	/**
	* @brief	�����l�擾
	* @param	[in] ipFeature    �t�B�`��
	* @param	[in] fieldIdx     �t�B�[���h����   
	* @return  CComVariant�l
	*/
	CComVariant getValue( const IFeaturePtr &ipFeature, long fieldIdx) ;

} // gf
