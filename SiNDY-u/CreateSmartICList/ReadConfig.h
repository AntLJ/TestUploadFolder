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

typedef std::set<long> lIDSET;

struct Parameters
{
	CString		m_strNwDB;
	lIDSET		m_lMeshSet;
	CString		m_strOut;
};

class CReadConfig
{
public:
	CReadConfig(void);
	~CReadConfig(void);

	/*
	 *	@brief	設定ファイルから文字列の取得
	 *	@param	 strConfigFile	[in]	設定ファイルのパス
	 *	@retval	true	成功
	 *	@retval	false	失敗
	 */
	Parameters getenv(const char* strConfigFile);
};
