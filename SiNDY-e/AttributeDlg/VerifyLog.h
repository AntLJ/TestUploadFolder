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

// VerifyLog.h: CVerifyLog �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_)
#define AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "atlwfile.h"

#define	DELIMITA	_T(",")

/**
	���e�m�F�p���O�t�@�C���n���h�����O�N���X

�@�@�o�͐�F%SiNDY-e%\log
	���O�����K��:[�z�X�g��]_[���[�U��]_[�N����].log
	���O�o�̓t�H�[�}�b�g:���O�n���h���c�[���W������(�J���}��؂�e�L�X�g)
		- 1)�`�F�b�N�σt���O
		- 2)���C����
		- 3)�I�u�W�F�N�g�h�c
		- 4)2�����b�V���R�[�h
		- 5)�s�s�n�}���b�V���R�[�h
		- 6)�m�F��Ǝ��̃��[�U�� 
		- 7)�m�F��Ǝ��̂o�b��
		- 8)�m�F��Ǝ��̃{�^��������������(2003/06/26 12:00:00)
		- 9)�m�F��Ǝ���MXD�t�@�C����
		- 10)�m�F��Ǝ��̃c�[���̃o�[�W����(episode???)
		- 11)�ŏI�ύX���̃��[�U��
		- 12)�ŏI�ύX���̍�ƖړI
		- 13)�ŏI�ύX����
		- 14)�ŏI�ύX���̕ύX���e
		- 15)�v���O�����ŏI�ύX����
		- 16)�v���O�����ŏI�ύX���̃v���O������
		- 17)���̑��R�����g�s
*/
class CVerifyLog  
{
public:
	CVerifyLog();
	virtual ~CVerifyLog();

	BOOL AppendVerifyLog( IMap* ipMap, _IRowPtr ipRow );

protected:
	BOOL Initialize();

protected:
	CFile				m_VerifyLogFile;	//!< ���e�m�F�σ��O�t�@�C��
	ISiNDYRulePtr	m_pISiNDYRuleObj;	//!< SiNDY���[��
	CString				m_strInstallDir;	//!< SiNDY-e�C���X�g�[���f�B���N�g����
	CString				m_strHostName;		//!< �z�X�g��
	CString				m_strLogDirName;	//!< ���O�i�[�f�B���N�g����
	CString				m_strLogFileName;	//!< ���O�t�@�C����
	CString				m_strOperator;		//!< �I�y���[�^�[��
	CString				m_strToolVer;		//!< �c�[���o�[�W����
	SYSTEMTIME			m_SystemTime;		//!< ���e�m�F���̎���

	CString				m_strPrevLayerName;	//!< �O����e�m�F�������C��
	long				m_nPrevOID;			//!< �O����e�m�F�����I�u�W�F�N�gID
	CFieldDef			m_UpdateFiled;
	CFieldDef			m_ProgUpdateFiled;
	DOMAINMAP*			m_UpdateFiledDomain;//!< �X�V���t�B�[���h�h���C��
};

#endif // !defined(AFX_VERIFYLOG_H__5535F33B_DE87_4F11_A0FA_1EDC46CF699E__INCLUDED_)
