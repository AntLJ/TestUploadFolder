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
#include <sindy/libschema.h>

/**
 * @brief ���O�Ǘ��N���X
 */
class CLogger
{
public:
	explicit CLogger(const uh::tstring& path) : m_logPath( path ){};

	/**
	 * @brief ���O�t�@�C�����J��
	 * @retval	true	����
	 * @retval	false	���s
	 */
	bool open()
	{
		m_ofs.open(m_logPath);
		if( m_ofs )
			return true;

		std::wcerr << m_logPath << L"���J���܂���" << std::endl;
		return false;
	}

	/**
	 * @brief ���O�t�@�C���ɏ�������
	 * @param msg[in] �������ޕ�����
	 */
	void write( const CString& msg )
	{
		m_ofs << msg.GetString() << std::endl;
	}

protected:
	const uh::tstring& m_logPath; //!< �t�@�C���p�X

	std::wofstream m_ofs; //!< �o�͐�X�g���[��
};
