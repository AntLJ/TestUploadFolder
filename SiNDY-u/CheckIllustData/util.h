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

namespace util
{
	/**
	 * @brief   �t�@�C�����ɓ��t��ǋL����
	 * @param   time  [in]  ���ݎ���
	 * @param   orgFilePath [in] ���̃t�@�C����
	 * @return  �ϊ���̃t�@�C����
	 */
	CString AddDateToFileName(const SYSTEMTIME& time, const CString& orgFilePath);

	/**
	 * @brief   �t�B�[�`���N���X���擾
	 * @param   name [in] �t�B�[�`���N���X��
	 * @param   work [in] ���[�N�X�y�[�X
	 * @param   featureC [out] �t�B�[�`���N���X
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool OpenFeatureClass(const CString& name, IWorkspacePtr work, IFeatureClassPtr& featureC);

	/**
	 * @brief   �e�[�u�����擾
	 * @param   name [in] �e�[�u����
	 * @param   work [in] ���[�N�X�y�[�X
	 * @param   table [out] �e�[�u��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool OpenTable(const CString& name, IWorkspacePtr work, ITablePtr& table);

	extern int JAPANESE_LANGUAGE ;               ///< ���{��̌���R�[�h(�V�X�e�����ꔻ��p)

}