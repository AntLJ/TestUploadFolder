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

namespace common_proc
{
	/**
	 * @brief ���[�N�X�y�[�X�̎擾
	 * @param strConnect [in] ���[�N�X�y�[�X���iDB���j
	 * @param ipWorkspace [out] ���[�N�X�y�[�X
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getWorkspace( const CString& strConnect, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief �e�[�u���̎擾
	 * @param strName [in] �e�[�u����
	 * @param ipWorkspace [in] ���[�N�X�y�[�X
	 * @param ipTable [out] �e�[�u��
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable );

	/**
	 * @brief �t�B�[���h�̎擾
	 * @param ipTable [in] �Ώۃe�[�u��
	 * @param strFieldName [in] �Ώۃt�B�[���h��
	 * @param ipField [out] �t�B�[���h
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getField( const ITablePtr& ipTable, const CString& strFieldName, IFieldPtr& ipField );

	/**
	 * @breif �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g���擾
	 * @param ipField [in] �Ώۃt�B�[���h
	 * @param setValues [out] �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getCodedValues( const IFieldPtr& ipField, std::set< long, std::less<long> >& setValues );

	/**
	 * @breif �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g���擾
	 * @param ipTable [in] �Ώۃe�[�u��
	 * @param strFieldName [in] �Ώۃt�B�[���h��
	 * @param setValues [out] �R�[�h�l�h���C���Œ�`����Ă���R�[�h�l�̃Z�b�g
	 * @retval true  ����
	 * @retval false ���s
	 */
	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::set< long, std::less<long> >& setValues );
}
