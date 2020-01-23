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
#include "ZipCodeDataContainer.h"

class ShapeFileManager
{
public:
	ShapeFileManager(void){}
	~ShapeFileManager(void){}

	/**
	 * @brief ���[�N�X�y�[�X�̃I�[�v��
	 * @param workspaceName [in] ���[�N�X�y�[�X��(�f�B���N�g���p�X)
	 * @retval true ����
	 * @retval false ���s
	 */
	bool OpenWorkspace(const CString& workspaceName);

	/**
	 * @brief �t�B�[�`���N���X�쐬
	 * @param fileName [in] �V�F�[�v�t�@�C����(=�t�B�[�`���N���X��)
	 * @param fields [in] �t�B�[�`���N���X�̃t�B�[���h���
	 * @retval �쐬�����t�B�[�`���N���X(���s������nullptr)
	 */
	IFeatureClassPtr CreateFeatureClass(
		const CString& fileName, IFieldsEditPtr& fields);

	/**
	 * @brief �t�B�[�`���N���X�쐬
	 * @param fileName [in] �V�F�[�v�t�@�C����(=�e�[�u����)
	 * @param fields [in] �e�[�u���̃t�B�[���h���
	 * @retval �쐬�����t�B�[�`���N���X(���s������nullptr)
	 */
	ITablePtr CreateTable(const CString& fileName, IFieldsEditPtr& fields);

	/**
	 * @brief �V�F�[�v�t�B�[���h�̍쐬
	 * @param fields [in,out] �t�B�[���h���
	 * @param spatialRef [in] ��ԎQ��
	 * @retval true ����
	 * @retval false ���s
	 */
	bool CreateShapeField(
		IFieldsEditPtr& fields, const ISpatialReferencePtr& spatialRef);

private:
	IWorkspacePtr m_outWorkspace; //!< ���[�N�X�y�[�X
};

