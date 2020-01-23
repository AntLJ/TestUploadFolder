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
 * @file	Addr2TsvBase.h
 */

// �{�N���X���g�p����ɂ͈ȉ���include���K�v
// �\�Ȍ���.h�ł�include�͔�����ׂ������A�قڑS�Ă�.cpp�ɉe�������邽�ߖ{�w�b�_��include���Ă��܂�
#include <crd_cnv/coord_converter.h>

/**
 * @class	CAddr2TsvBase
 * @brief	TSV�o�̓x�[�X�N���X
 */
class CAddr2TsvBase abstract
{
public:
	CAddr2TsvBase(void);
	~CAddr2TsvBase(void);

	/**
	* @brief	������
	* @retval	true	����
	* @retval	false	���s
	*/
	virtual bool init(void);

	/**
	* @brief	�������s
	* @param	bDispProgress	[in]	�i�����o�͗L��
	* @param	strWhere		[in]	��������
	* @retval	true	����
	* @retval	false	���s
	*/
	bool execute(const bool bDispProgress = true, const CString& strWhere = NULL);

protected:
	/**
	* @brief	���R�[�h�ϊ�
	* @note		�������z�֐��i�p����Ŏ������K�v�j
	* @param	ipRow	[in]	���R�[�h
	* @retval	true	����
	* @retval	false	���s
	*/
	virtual bool convRecord(const _IRowPtr& ipRow) = 0;

	/**
	* @brief	�t�B�[���h�C���f�b�N�X�擾
	* @param	fieldIndex		[in]	�t�B�[���hINDEX
	* @param	strFieldName	[in]	�t�B�[���h��
	* @param	index			[out]	�擾�����C���f�b�N�X
	* @retval	true	����
	* @retval	false	���s
	*/
	bool getFieldIndex(const FIELD_INDEX& fieldIndex, const CString& strFieldName, long& index) const;

	/**
	* @brief	�l�擾
	* @param	ipRow			[in]	���R�[�h
	* @param	strFieldName	[in]	�t�B�[���h��
	* @param	index			[in]	�C���f�b�N�X
	* @param	strVal			[out]	�擾�����l
	* @retval	true	����
	* @retval	false	���s
	*/
	bool getValue(const _IRowPtr& ipRow, const CString& strFieldName, const long& index, CString& strVal) const;

	/**
	* @brief	2�����b�V��XY�擾
	* @param	ipFeature	[in]	�t�B�[�`��
	* @param	mesh		[in]	2�����b�V���R�[�h
	* @param	x			[in]	X���W
	* @param	y			[out]	Y���W
	* @retval	true	����
	* @retval	false	���s
	*/
	bool getMeshXY(const IFeaturePtr& ipFeature, int& mesh, int &x, int& y) const;

	/**
	* @brief	�e��ݒ�
	*/
	void setWorkspace(const IWorkspacePtr& ipWS) { m_ipWorkspace = ipWS; };
	void setOwner(const CString& strOwner) { m_strOwner = strOwner; };
	void setTableName(const CString& strTableName) { m_strTableName = strTableName; };
	void setSortField(const CString& strSortField) { m_strSortField = strSortField; };
	void setProgressLogPrefix(const CString& strProgressLogPrefix) { m_strProgressLogPrefix = strProgressLogPrefix; };

protected:
	IWorkspacePtr		m_ipWorkspace;
	ITablePtr			m_ipTable;
	FIELD_INDEX			m_fieldIndex;

	CString				m_strOwner;
	CString				m_strTableName;
	CString				m_strSortField;
	CString				m_strProgressLogPrefix;

	static crd_cnv		m_cCrdCnv;
};
