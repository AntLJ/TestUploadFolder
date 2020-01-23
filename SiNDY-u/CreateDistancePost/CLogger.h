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
#include "COutput.h"

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
	public COutput
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
	~CLogger()
	{
		EndLog();
	}

	/**
	 * @brief ���O�����ݒ�
	 * @param lpszLogFile [in] ���O�t�@�C����
	 * @param pair [in] �H���ԍ��A�����R�[�h�A�����W�L��
	 * @param lpcszOpParam [in] �p�����[�^
	*/
	void initLog(LPCTSTR lpszLogFile, boost::tuple<long,long,CString> pair, LPCTSTR lpcszOpParam);

	/**
	 * @brief ���O��������
	 * @param nOID [in] �I�u�W�F�N�gID
	 * @param lpcszMark [in] �����W�L��
	 * @param dbVal [in] �����W�l
	*/
	void Log(long nOID, LPCTSTR lpcszMark, double dbVal);

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
