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
 * @brief INデータパス
 */
struct INPath
{
	CString eMntDir;		///< .mntデータ格納ディレクトリパス
	CString ePolDir;		///< pol_code.txt格納ディレクトリパス
	CString eLatestDir;		///< latestの.mntデータ格納ディレクトリパス
};

/**
 * @brief データチェック実行クラス
 */
class CRoadApplication
{
public:
	CRoadApplication(void);
	~CRoadApplication(void);

	/**
	 * @brief INデータをチェック
	 * @note  指定INデータをチェックする。
	 *
	 * @param	rConfig			[in]	Configファイルへのパス
	 * @param	lpcszCustomer	[in]	仕向け先
	 * @param	bLatest			[in]	latestの指定があるか（B版以降のチェックかどうか）
	 *
	 * @return	0:正常終了 1:異常終了 2:異常終了(例外発生) 3:チェックにエラーあり
	 */
	long run( CString& rConfig, LPCTSTR lpcszCustomer, bool bLatest );

private:

	/**
	 * @brief Configファイルの設定を読み込む
	 * @note  指定したConfigファイルの設定を読み込む。
	 *
	 * @param	rConfig		[in]	Configファイルへのパス
	 *
	 * @return	なし
	 */
	void GetConf( CString& rConfig );

	/**
	 * @brief	仕向け先指定
	 * @param	lpcszCustomer		[in]	仕向け先
	 * @return	Customer
	 */
	Customer CRoadApplication::GetCustomer( LPCTSTR lpcszCustomer );

	/**
	 * @brief .mntファイルが存在するメッシュリストを作成する。
	 * @note  指定したmnt格納Dir以下に存在する.mntファイルのメッシュリストを作成する。
	 * @param	lpcszMntDir	[in]	検索ディレクトリ
	 *
	 * @return	mntファイルがあるメッシュのリスト
	 */
	std::set<int> CreateList( LPCTSTR lpcszMntDir );

	/**
	 * @brief .mntファイルが存在するかを検索する。
	 * @note  指定階層以下に.mntファイルが存在するかを検索する。
	 *
	 * @param	hFind		[in]	指定ファイルハンドル
	 * @param	fd			[in]	検索用ファイルシステム
	 * @param	lpPath		[in]	指定階層絶対パス
	 * @param	setMesh		[in]	mntファイルがあるメッシュのリスト
	 *
	 * @return	なし
	 */
	void FindFile(HANDLE hFind, WIN32_FIND_DATA &fd, LPCTSTR lpPath, std::set<int>& setMesh );

	INPath		m_INPathData;			///< 指定INデータ格納ディレクトリ
	set<int>	m_setDataMeshlist;		///< 2次メッシュリスト
	set<int>	m_setLatestMeshlist;	///< latest2次メッシュリスト
};
