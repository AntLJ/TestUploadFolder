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
 * @brief ログ管理クラス
 */
class CLogger
{
public:
	explicit CLogger(const uh::tstring& path) : m_logPath( path ){};

	/**
	 * @brief ログファイルを開く
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool open()
	{
		m_ofs.open(m_logPath);
		if( m_ofs )
			return true;

		std::wcerr << m_logPath << L"を開けません" << std::endl;
		return false;
	}

	/**
	 * @brief ログファイルに書き込む
	 * @param msg[in] 書き込む文字列
	 */
	void write( const CString& msg )
	{
		m_ofs << msg.GetString() << std::endl;
	}

protected:
	const uh::tstring& m_logPath; //!< ファイルパス

	std::wofstream m_ofs; //!< 出力先ストリーム
};
