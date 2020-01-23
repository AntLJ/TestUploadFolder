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
* @file	FileSJIS.h
*/

/**
* @class	CFileSJIS
* @brief	SJISファイルクラス
*/
class CFileSJIS
{
public:
	CFileSJIS();
	~CFileSJIS();

	/**
	* @brief	ファイルオープン
	* @param	strDir	[in]	ディレクトリ名
	* @param	strFile	[in]	ファイル名
	* @param	ofs		[out]	出力ファイルストリーム
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool open(const CString& strDir, const CString& strFile, std::ofstream&	ofs);

	/**
	* @brief	ファイルクローズ
	*/
	void close();

private:
	std::ofstream*	m_ofsFile;
};

