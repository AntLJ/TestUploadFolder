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
#include "WinLib\VersionInfo.h"

// �t�B�[�`���N���X���ʑ����X�V�p�N���X
class CommonAttribute
{
public:
	CommonAttribute(void)
	{
		m_userName = _T("sindy");
		CVersion cVersion;
		m_toolName = cVersion.GetInternalName();

		// YYYY/MM/DD hh:mm:ss
		SYSTEMTIME stSystemTime = {};
		::GetLocalTime( &stSystemTime);
		m_modifyDate.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"),
			stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
			stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
	}
	virtual ~CommonAttribute(void);

	/*
	 * @brief �ŏI�X�V�җp�̕�����擾
	 * @retval �X�V�Җ�(sindy)
	 */
	CString GetLastUserName(){return m_userName;}

	/*
	 * @brief �ŏI�X�V�v���O�������p�̕�����擾
	 * @retval �X�V�v���O������
	 */
	CString GetToolName(){return m_toolName;}

	// �v���O�����ŏI�X�V����
	/*
	* @brief �v���O�����ŏI�X�V�����p�̕�����擾
	* @retval �X�V��(YYYY/MM/DD hh:mm:ss)
	*/
	CString GetModifyDate(){return m_modifyDate;}

private:
	CString m_userName; //!< �ŏI�X�V�Җ�
	CString m_toolName; //!< �ŏI�X�V�v���O������
	CString m_modifyDate; //!< �v���O�����ŏI�X�V����
};

