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

#include "global.h"

class CShpMaker
{
public:
	CShpMaker(void);
	~CShpMaker(void);

	/**
	 * @brief �V�F�[�v�쐬
	 * @param bIsNew [in] �V�K�쐬���ǂ���
	 * @param listField [in] �t�B�[���h���
	 * @param listShpInfo [in] �f�[�^���X�g
	 * @param lpszOutputFile [out] �V�F�[�v�t�@�C���̃p�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool MakeShape(bool bIsNew, const std::list<PAIR_FIELD>& listField, const std::list<SHAPE_INFO>& listShpInfo, LPCTSTR lpszOutputFile);

	/**
	 * @brief �V�F�[�v�f�[�^�ǉ�
	 * @param listShpInfo [in] �f�[�^���X�g
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool AddData(const std::list<SHAPE_INFO>& listShpInfo);

	void Clear()
	{
		m_ipTable = NULL;
		m_mapFieldIndex.clear();
	};

private:
	/**
	 * @brief �t�B�[���h�쐬
	 * @param listField [in] �t�B�[���h���
	 * @param ipGeom [in] �`��
	 * @return �t�B�[���h
	 */
	IFieldsPtr CreateFields(const std::list<PAIR_FIELD>& listField, const IGeometryPtr& ipGeom);

	/**
	 * @brief �V�F�[�v�폜
	 * @param strFile [in] �폜�Ώۃt�@�C��
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool DeleteShape(const CString& strFile);

	/**
	 * @brief �V�F�[�v�쐬
	 * @param bIsNew [in] �V�K�쐬���ǂ���
	 * @param ipWorkspace [in] �V�F�[�v�̃��[�N�X�y�[�X
	 * @param strShpDir [in] �V�F�[�v�t�@�C���o�͐�
	 * @param strFeatureName [in] �e�[�u�����i�V�F�[�v�t�@�C�����j
	 * @param listField [in] �t�B�[���h���
	 * @param ipGeom [in] �`��t�B�[���h�ݒ�p�̌`��f�[�^
	 * @param ipOutTable [out] �V�F�[�v�t�@�C���̃e�[�u��
	 * @param mapOutFieldIndex [out] �V�F�[�v�t�@�C���̃t�B�[���h�C���f�b�N�X
	 * @retval true  ��������
	 * @retval false �������s
	 */
	bool CreateNewShape(bool bIsNew, IWorkspacePtr ipWorkspace, const CString& strShpDir, const CString& strFeatureName, const std::list<PAIR_FIELD>& listField, const IGeometryPtr& ipGeom, ITablePtr& ipOutTable, std::map<CString, long>& mapOutFieldIndex);

private:
	ITablePtr m_ipTable;
	std::map<CString, long> m_mapFieldIndex;
};

