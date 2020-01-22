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
#include "Output.h"

namespace sindy
{

/**
 * @class CLogger
 * @brief ���O�Ǘ��N���X
 * @file  CLogger.h
 * @author �r�b�g�G�C�W�@�`
 * $Id$
*/
class CLogger :
	public Output
{
public:
	/**
	 * @brief �R���X�g���N�^
	 */
	CLogger()
	{
		m_stream = nullptr;
	}

	/**
	 * @brief �f�X�g���N�^
	 */
	virtual ~CLogger()
	{
		EndLog();
	}

	/**
	 * @brief ���O�����ݒ�
	 * @param lpszLogFile [in] ���O�t�@�C����
	 * @param nRoadCode [in] �H���ԍ�
	 * @param nDirCode [in] �����R�[�h
	 * @param nDirCode [in] �����W�L��
	 * @param lpcszOpParam [in] �p�����[�^
	 * @param bSetHeader [in] �w�b�_���Z�b�g���邩�ǂ���
	*/
	void initLog(LPCTSTR lpszLogFile, long nRoadCode, long nDirCode, LPCTSTR lpcszMark, LPCTSTR lpcszOpParam, bool bSetHeader);

	/**
	 * @brief ���O��������
	 * @param nOID [in] �G���[�I�u�W�F�N�g
	 * @param dbVal [in] �`�F�b�N��I�u�W�F�N�g
	 * @param nCode [in] 
	 * @param lpcszMsg [in] 
	*/
	void AddLog(sindy::CSPRowBase pRow, long nCode, LPCTSTR lpcszMsg);

	/**
	 * @brief ���O��������
	 * @param nOID [in] �G���[�I�u�W�F�N�g
	 * @param dbVal [in] �`�F�b�N��I�u�W�F�N�g
	 * @param nCode [in] 
	 * @param lpcszMsg [in] 
	*/
	void AddLog(long nOID, double dbVal, LPCTSTR lpcszMark, long nCode, LPCTSTR lpcszMsg);

	/**
	 * @brief ���O�I������
	*/
	void EndLog();

private:
	FILE *m_stream;                         ///< ���O�t�@�C���|�C���^
	CString m_strLogFile;                   ///< ���O�t�@�C����
};

typedef boost::shared_ptr<CLogger> CSPLogger;

}
