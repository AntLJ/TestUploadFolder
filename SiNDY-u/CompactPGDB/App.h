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

#define TOOL_VERSION	_T("Ver 0.9,0,1")

class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	使用法
	 * @retval	false
	 */
	static bool printUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool init( int argc, TCHAR* argv[] );

	/**
	 * @brief	実行
	 * @return	bool
	 */
	bool execute();

	/**
	 * @brief	コンパクト化
	 * @param	lpszFile	[in]	PGDBファイル
	 * @return	bool
	 */
	bool compact( LPCTSTR lpszFile );

private:

	/**
	 * @brief	引数チェック
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool checkArg( int argc, TCHAR* argv[] );

	/**
	 * @brief	メッシュリスト読み込み
	 * @param	lpszFile	[in]	メッシュリスト
	 * @return	bool
	 */
	bool loadMeshList( LPCTSTR lpszFile );

///////////////////////////////////
//	メンバ変数
///////////////////////////////////

	CString					m_strBaseDir;		/// ベースディレクトリ

	std::vector<CString>	m_vecFile;			/// PGDBファイルリスト

	std::vector<long>		m_vecMesh;			/// メッシュリスト

};
