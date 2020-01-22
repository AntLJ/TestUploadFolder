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
 * @brief  ログベースクラス
 */
class FileController
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	FileController(void):m_pFile(nullptr),m_bUTF8(false){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~FileController(void){ finalize(); }

	/**
	 * @brief  ファイルオ作成、ヘッダ書き込み
	 */
	bool initialize( const std::wstring& strDirPath, const std::wstring& mode );

	/**
	 * @brief  ファイルクローズ
	 */
	void finalize() 
	{ 
		if(!m_pFile)
			return;
		fclose(m_pFile);
		m_pFile = nullptr;
	}
	
	/**
	 * @brief  メッセージ出力
	 */
	void log( const std::wstring& strMessage );
	
	/**
	 * @brief  メッセージのコードの変換
	 */
	std::string wstringToString( const std::wstring& wstr );
	
	FILE*      m_pFile; //!< ファイルストリーム
	bool       m_bUTF8; //!< ファイルはUTF-8で開きますか
};
