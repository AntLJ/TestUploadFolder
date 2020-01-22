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

#include "stdafx.h"

#include <sindy/workspace.h>
#include <TDC/sindylib_core/FieldMap.h>

/**
 * @brief   ���C�����Ǘ��N���X
 */
class LayerInfoManager
{
public:

	/**
	 * @brief   �R���X�g���N�^
	 */
	LayerInfoManager() {}

	/**
	 * @brief   �R���X�g���N�^
	 * @param   isFeatureClass [in]  �t�B�[�`�����ǂ����itrue�F�t�B�[�`���Afalse�F�e�[�u���j
	 */
	LayerInfoManager(bool isFeatureClass) : m_isFeatureClass(isFeatureClass) {}

	/**
	 * @brief   �f�X�g���N�^
	 */
	virtual ~LayerInfoManager() {}

	/**
	 * @brief   �ڑ����ݒ�
	 * @param   workspace [in]  ���[�N�X�y�[�X
	 * @param   tableName [in]  �e�[�u����
	 * @retval  true  ����
	 * @retval  false ���s
	 */
	bool setFeatureClassInfo(
			const IFeatureWorkspacePtr& workspace,
			const CString& tableName
		);

	/**
	 * @brief   �t�B�[���h�}�b�v�擾
	 * @return  �t�B�[���h�}�b�v
	 */
	sindy::CFieldMap LayerInfoManager::getFieldMap() const { return m_fieldMap; }

	/**
	 * @brief   �e�[�u�����擾
	 * @return  �e�[�u����
	 */
	CString getTableName() const { return m_tableName; }

	/**
	 * @brief   �t�B�[�`���N���X�擾
	 * @return  �t�B�[�`���N���X
	 */
	IFeatureClassPtr getFeatureClass() const { return m_featureClass; }

	/**
	 * @brief   �e�[�u���擾
	 * @return  �e�[�u��
	 */
	ITablePtr getTable() const { return ( ( m_isFeatureClass) ? (ITablePtr)m_featureClass : m_table ); }

private:

	IFeatureClassPtr m_featureClass;   //!< �t�B�[�`���N���X
	ITablePtr m_table;                 //!< �e�[�u��
	sindy::CFieldMap m_fieldMap;       //!< �t�B�[���h�}�b�v
	bool m_isFeatureClass;             //!< �t�B�[�`�����ǂ����itrue�F�t�B�[�`���Afalse�F�e�[�u���j
	CString m_tableName;               //!< �e�[�u����
};

