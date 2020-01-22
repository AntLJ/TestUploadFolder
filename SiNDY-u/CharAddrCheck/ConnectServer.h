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

#include <oracl.h>

/**
 * @brief	DB�ڑ��N���X
 * @class	CConnectServer
 */
class CConnectServer
{
public:
	CConnectServer();
	~CConnectServer();

	/**
	 * @brief �Ώۓ��HDB�ɐڑ�����B
	 * @param	lpcszConf[in]				�ݒ�t�@�C��
	 * @param	lpcszGouPoint[in]			���|�C���g�̃`�F�b�N���X�g
	 * @param	lpcszCityAdmin[in]			�s�s�n�}�s���E�̃`�F�b�N���X�g
	 * @param	lpcszCommandLine[in]		�R�}���h���C������	
	 * @retval	true	����I��
	 * @rettval	false	�ُ�I��
	 */
	//bool run( LPCTSTR lpcszConf, LPCTSTR lpcszGouPoint, LPCTSTR lpcszCityAdmin );
	bool run( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpszCommandLine[] );

private:
	/**
	 * @brief	�ݒ�t�@�C���̓ǂݍ���
	 * @note	�w��ݒ�t�@�C����ǂݍ���
	 * @param	lpcszConf[in]				�ݒ�t�@�C���p�X
	 * @param	lpcszCommandLine[in]		�R�}���h���C������	
	 */
	void ReadConf( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpszCommandLine[] );

	bool OraDBConnect( LPCTSTR lpcszConnect );

private:
	CString m_strSDEServer;					//!< SDE�T�[�o
	CString m_strConvertTable;				//!< �ϊ��e�[�u��
	CString m_strAddrTable;					//!< �Z���e�[�u��
	CString m_strOutput;					//!< �o�͐惍�O�p�X
	bool m_bInfoAddr;						//!< �R�}���h���C���ł̃I�v�V�����i�P�ꃊ�X�g�o�́j
	CString m_strPurposServer;				//!< ��ƖړI�T�[�o

	ODatabase m_eOraDB;						//!< �ڑ�DB

	CString m_strGouPointText;				//!< ���|�C���g�̓ǂݕϊ���̏o�̓��X�g
	CString m_strCityAdminText;				//!< �s�s�n�}�s����̓ǂݕϊ���̏o�̓��X�g
};
