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

class CDBManager
{
public:
	CDBManager(void);
	~CDBManager(void);

	/**
	 * @brief �t�B�[�`���J�[�\���̎擾
	 * @param  ipFeatureClass		[in]	�t�B�[�`���N���X�|�C���^
	 * @param  bstrWhereClause		[in]	Where�啶����
	 * @retval	IFeatureCursorPtr	�t�B�[�`���J�[�\���|�C���^
	 */
	IFeatureCursorPtr GetFeatureCursor(IFeatureClassPtr ipFeatureClass, CComBSTR bstrWhereClause);

	/**
	 *	@brief	�w�肵���`���esriSpatialRelEnum�ł��������Ɉ�v����t�B�[�`���J�[�\���̎擾
	 *	@param	ipFeatureClass		[in]	�t�B�[�`���N���X
	 *	@param	ipGeom				[in]	��r���`��
	 *	@param	eStType				[in]	�`�󔻒�p�����[�^
	 *	@param	lpcszSubFields		[in]	�T�u�t�B�[���h�ݒ�p������
	 *	@retval	IFeatureCursorPtr	�t�B�[�`���J�[�\���|�C���^
	 */
	IFeatureCursorPtr GetFeatureCursor(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, esriSpatialRelEnum eStType, LPCTSTR lpcszSubFields=NULL);

	/**
	 * @brief �N�G���t�B���^�̍쐬
	 * @param bstrWhereClause	[in]		Where�啶����
	 * @retval	IQueryFilterPtr	�N�G���t�B���^�|�C���^
	 */
	IQueryFilterPtr CreateQueryFilter(CComBSTR bstrWhereClause);

	/**
	 *	@brief	OBJECTID�̎擾
	 *	@param	ipRow	[in]	�ꃌ�R�[�h
	 *	@retval	ObjectID
	 */
	long GetOID(const _IRowPtr& ipRow);

	/**
	 *	@brief	�`��(�W�I���g��)�̎擾
	 *	@param	ipFeature	[in]	�`����������ꃌ�R�[�h�̏��
	 *	@retval	�`��
	 */
	IGeometryPtr CDBManager::GetGeom(const IFeaturePtr& ipFeature);

	/**
	 *	@brief	OID�ƃt�B�[�`���N���X����`��(�W�I���g��)�̎擾
	 *	@param	lOID	[in]	�`����擾������Objectid
	 *	@param	ipFC	[in]	�`����擾�������t�B�[�`���N���X
	 *	@retval	�`��
	 */
	IGeometryPtr CDBManager::GetGeomFromOID(long lOID, IFeatureClassPtr ipFC);

	/**
	 *	@brief	�w�肵���`��Ɏw�肵�������Ńq�b�g�����`��̓���̏������Z�b�g�Ɋi�[���ĕԂ�
	 *	@param	ipFeatureClass	[in]	�t�B�[�`���N���X
	 *	@param	ipGeom			[in]	�W�I���g��
	 *	@param	lFieldIndex		[in]	�����擾�����t�B�[���h�̃C���f�b�N�X
	 *	@param	eStType			[in]	esriSpatialRelEnum
	 *	@retval	std::set<long>			[out]	�q�b�g�����`���OID���i�[����set
	 */
	std::set<CAdapt< CComVariant>> CDBManager::GetHitValSet(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom, long lFieldIndex, esriSpatialRelEnum eStType);

	/**
	 *	@brief	�t�B�[���h�C���f�b�N�X���w�肵�Ă̒l�̎擾
	 *	@param	ipRow		[in]	�擾����I�u�W�F�N�g�̍s
	 *	@param	lFieldIndex	[in]	�擾�������̃C���f�b�N�X
	 *	@return �w�肳�ꂽ�t�B�[���h��CComVariant�l
	 */
	CComVariant		GetValue(_IRowPtr ipRow, long lFieldIndex);
};
