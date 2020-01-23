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
 * @file	ConvAddrPoint.h
 */

// �����\�t�@�C������`
const CString C_COUNT_AUTH_ADR_FILE = _T("count_auth_adr.txt");
const CString C_COUNT_AUTH_POS_FILE = _T("count_auth_pos.txt");

/**
 * @class	CConvAddrPoint
 * @brief	�V�Z���pTSV�ϊ����C���N���X
 */
class CConvAddrPoint
{
public:
	CConvAddrPoint(CParam& cParam);
	~CConvAddrPoint(void);

	/**
	* @brief	��������
	* @retval	true	����
	* @retval	false	���s
	*/
	bool init(void);

	/**
	* @brief	�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool execute(void);

private:
	/**
	* @brief	adr_authdb�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool adr_authdb(void);

	/**
	* @brief	�Z�������\�쐬
	* @param	adrp001	[in]	ADRP001�ϊ��N���X
	* @param	adrp002	[in]	ADRP002�ϊ��N���X
	* @param	adrp003	[in]	ADRP003�ϊ��N���X
	* @param	adrp004	[in]	ADRP004�ϊ��N���X
	* @param	adrp005	[in]	ADRP005�ϊ��N���X
	* @param	adrp006	[in]	ADRP006�ϊ��N���X
	* @retval	true	����
	* @retval	false	���s
	*/
	bool make_count_auth_adr(const CAdrp001& adrp001, const CAdrp002& adrp002, const CAdrp003& adrp003, const CAdrp004& adrp004, const CAdrp005& adrp005, const CAdrp006& adrp006);

	/**
	* @brief	pos_authdb�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool pos_authdb(void);

	/**
	* @brief	�X�֔ԍ������\�쐬
	* @param	zipp001	[in]	Zipp001�ϊ��N���X
	* @retval	true	����
	* @retval	false	���s
	*/
	bool make_count_auth_pos(const CZipp001& zipp001);

	/**
	* @brief	adrmst_authdb�������s
	* @retval	true	����
	* @retval	false	���s
	*/
	bool adrmst_authdb(void);

private:
	CParam&			m_cParam;
	IWorkspacePtr	m_ipWorkspaceAddr;
	IWorkspacePtr	m_ipWorkspaceMlang;
};

