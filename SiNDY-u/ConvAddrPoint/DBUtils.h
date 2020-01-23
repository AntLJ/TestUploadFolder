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
 * @file	DBUtils.h
 */

// �t�B�[���hIndex
typedef std::map<CString, long> FIELD_INDEX;

/**
 * @class	CDBUtils
 * @brief	DB���[�e�B���e�B�N���X
 */
class CDBUtils
{
public:
	CDBUtils(void);
	~CDBUtils(void);

	/**
	* @brief	DB������
	* @param	dbConnection	[in]	DB�ڑ����
	* @param	ipWorkspace		[out]	���[�N�X�y�[�X
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool initDB(const CString& dbConnection, IWorkspacePtr& ipWorkspace);

	/**
	* @brief	�t�B�[�`���N���X�I�[�v��
	* @param	ipWorkspace			[in]	���[�N�X�y�[�X
	* @param	featureClassName	[in]	�t�B�[�`���N���X��
	* @param	ipFeatureClass		[out]	�t�B�[�`���N���X
	* @param	fieldIndex			[out]	�t�B�[���hINDEX
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool openFeatureClass(const IWorkspacePtr& ipWorkspace, const CString& featureClassName, IFeatureClassPtr& ipFeatureClass, FIELD_INDEX& fieldIndex);

	/**
	* @brief	�e�[�u���I�[�v��
	* @param	ipWorkspace		[in]	���[�N�X�y�[�X
	* @param	tabelName		[in]	�e�[�u����
	* @param	ipTable			[out]	�t�B�[�`���N���X/�e�[�u��
	* @param	fieldIndex		[out]	�t�B�[���hINDEX
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool openTable(const IWorkspacePtr& ipWorkspace, const CString& tabelName, ITablePtr& ipTable, FIELD_INDEX& fieldIndex);

	/**
	* @brief	�e�[�u�������擾
	* @param	ipTable			[in]	�t�B�[�`���N���X/�e�[�u��
	* @param	strWhere		[in]	����SQL
	* @param	lCount			[out]	�e�[�u������
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool getRowCount(const ITablePtr& ipTable, const CString& strWhere, long& lCount);

	/**
	* @brief	DB�J�[�\���擾
	* @param	ipTable			[in]	�t�B�[�`���N���X/�e�[�u��
	* @param	strWhere		[in]	����SQL
	* @param	strSort			[in]	�\�[�g�t�B�[���h
	* @param	ipCursor		[out]	�J�[�\��
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool getCursor(const ITablePtr& ipTable, const CString& strWhere, const CString& strSort, _ICursorPtr& ipCursor);

	/**
	* @brief	�t�B�[���hINDEX�擾
	* @param	fieldIndex		[in]	�t�B�[���hINDEX
	* @param	strFieldName	[in]	�t�B�[���h��
	* @param	index			[out]	INDEX
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index);

	/**
	* @brief	�l�擾
	* @param	ipRow			[in]	���R�[�h/�t�B�[�`��
	* @param	strFieldName	[in]	�t�B�[���h��
	* @param	index			[in]	INDEX
	* @param	strVal			[out]	�l
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal);

private:
	/**
	* @brief	�t�B�[���hINDEX�쐬
	* @param	ipTable			[in]	�t�B�[�`���N���X/�e�[�u��
	* @param	fieldIndex		[out]	�t�B�[���hINDEX
	* @retval	true	����
	* @retval	false	���s
	*/
	static bool createFieldIndex(const ITablePtr& ipTable, FIELD_INDEX& fieldIndex);
};


