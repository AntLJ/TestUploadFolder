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

#include "MakeAddrRec.h"

/**
 * @enum	EListMode
 * @brief	リストモード(どういった形で市区町村コードリストを作るか)
 */
enum EListMode
{
	kAll,		//!< 全件
	kRange,		//!< 範囲指定
	kList,		//!< リスト指定
	kSingle,	//!< 単独
};

/**
 * @class	CApp
 * @brief	アプリケーションクラス
 */
class CApp
{
public:
	CApp(void);
	~CApp(void);

	/**
	 * @brief	使用法
	 */
	static void PrintUsage();

	/**
	 * @brief	初期化
	 * @param	argc	[in]	引数の数
	 * @param	argv	[in]	引数
	 * @return	bool
	 */
	bool Init( int argc, _TCHAR* argv[] );

	/**
	 * @brief	実行
	 */
	void Run();

private:

	/**
	 * @brief	設定ファイル読み込み
	 * @param	lpcszFile	[in]	設定ファイル
	 * @return	bool
	 */
	bool LoadInitFile( LPCTSTR lpcszFile );

	/**
	 * @brief	市区町村コードリスト作成
	 * @return	bool
	 */
	bool MakeCityCodeList();

	/**
	 * @brief	データ書き出し
	 * @param	lpcszCityCode	[in]	市区町村コード
	 * @param	rPntList		[in]	PNTレコードリスト
	 * @param	rGouList		[in]	GOUレコードリスト
	 */
	bool WriteData( LPCTSTR lpcszCityCode, const PNTMAP& rPntList, const GOUMAP& rGouList );

private:

	EListMode			m_emListMode;		//!< リストモード
	CString				m_strListInfo;		//!< リストモードに応じた補足情報
	std::set<CString>	m_setCode;			//!< 市区町村コードリスト

	CParameter			m_cParam;			//!< 設定情報

	IWorkspacePtr		m_ipAddrWorkspace;	//!< 住所系ワークスペース
	IWorkspacePtr		m_ipMapWorkspace;	//!< 地図系ワークスペース

	IFeatureClassPtr	m_ipCityAdminFC;	//!< 都市地図行政界フィーチャクラス
	IFeatureClassPtr	m_ipGouPointFC;		//!< 号ポイントフィーチャクラス

	CFieldMap			m_cAdminFID;		//!< 都市地図行政界フィールドインデックスリスト
	CFieldMap			m_cGouFID;			//!< 号ポイントフィールドインデックスリスト

	CMakeAddrRec		m_cMakeAddrRec;		//!< 住所レコード生成クラス
};
