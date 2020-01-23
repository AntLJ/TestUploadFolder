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
 * @file	FileEUC.h
 */

/**
 * @class	CFileEUC
 * @brief	EUCファイルクラス
 */
class CFileEUC
{
public:
	CFileEUC(void);
	~CFileEUC(void);

	/**
	* @brief	ファイルオープン
	* @param	strDir	[in]	ディレクトリ名
	* @param	strFile	[in]	ファイル名
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool open(const CString& strDir, const CString& strFile);

	/**
	* @brief	ファイルクローズ
	*/
	void close();

	/**
	* @brief	ファイル書き込み
	* @param	strMsg	[in]	出力メッセージ
	*/
	void write(const std::string& strMsg);

private:
	/**
	* @brief	SJIS→EUC変換
	* @param	str	[in]	文字列
	* @return	変換後文字列
	*/
	std::string ConvSJIS2EUC(const std::string& str) const;

private:
	std::ofstream	m_ofsFile;
};
