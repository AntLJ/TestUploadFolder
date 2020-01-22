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

#include <oracl.h>

/**
 * @brief	DB接続クラス
 * @class	CConnectServer
 */
class CConnectServer
{
public:
	CConnectServer();
	~CConnectServer();

	/**
	 * @brief 対象道路DBに接続する。
	 * @param	lpcszConf[in]				設定ファイル
	 * @param	lpcszGouPoint[in]			号ポイントのチェックリスト
	 * @param	lpcszCityAdmin[in]			都市地図行政界のチェックリスト
	 * @param	lpcszCommandLine[in]		コマンドライン引数	
	 * @retval	true	正常終了
	 * @rettval	false	異常終了
	 */
	//bool run( LPCTSTR lpcszConf, LPCTSTR lpcszGouPoint, LPCTSTR lpcszCityAdmin );
	bool run( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpszCommandLine[] );

private:
	/**
	 * @brief	設定ファイルの読み込み
	 * @note	指定設定ファイルを読み込む
	 * @param	lpcszConf[in]				設定ファイルパス
	 * @param	lpcszCommandLine[in]		コマンドライン引数	
	 */
	void ReadConf( LPCTSTR lpcszConf, int nCommandLine, LPTSTR lpszCommandLine[] );

	bool OraDBConnect( LPCTSTR lpcszConnect );

private:
	CString m_strSDEServer;					//!< SDEサーバ
	CString m_strConvertTable;				//!< 変換テーブル
	CString m_strAddrTable;					//!< 住所テーブル
	CString m_strOutput;					//!< 出力先ログパス
	bool m_bInfoAddr;						//!< コマンドラインでのオプション（単語リスト出力）
	CString m_strPurposServer;				//!< 作業目的サーバ

	ODatabase m_eOraDB;						//!< 接続DB

	CString m_strGouPointText;				//!< 号ポイントの読み変換後の出力リスト
	CString m_strCityAdminText;				//!< 都市地図行政会の読み変換後の出力リスト
};
