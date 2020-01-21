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

/**
 * @file SiNDYeSettings.h
 * @brief CSiNDYeSettingsクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _SINDYESETTINGS_H_
#define _SINDYESETTINGS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * @class CSiNDYeSettings
 * @brief <b>SiNDY-eの初期設定レジストリやsettings.ini
 * の内容を管理するクラス</b>\n
 * このクラスで扱う設定は以下のとおり：
 * @li SiNDY-eインストールフォルダ名
 * @li iniファイル名（settings.ini）
 * @li SiNDY-eバージョン名
 * @li ルール接続用サーバ名
 * @li SiNDY-e警告レベル
 * @warning このクラスはexportしていないメンバ変数を宣言していますが、privateなため外から
 * アクセスされることはありません。
 * なお、上記のような場合その旨ワーニングが出るのですが、ウザイのでワーニングを消しています。
 */
class CSiNDYeSettings
{
public:
	CSiNDYeSettings();
	virtual ~CSiNDYeSettings();

	//@{ @name 取得系関数
	/**
	 * @brief <b>SiNDY-eのインストールフォルダを取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\InstallRoot\\Path
	 * @return インストールフォルダ（最後に\が付く）
	 */
	LPCTSTR GetInstallRoot() const { return m_strInstallRoot; }

	/**
	 * @brief <b>インストールフォルダ内に格納されるiniファイル名を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\InstallRoot\\InitFileName\n
	 * 通常はsettings.iniです。
	 * @return iniファイル名
	 */
	LPCTSTR GetInitFileName() const { return m_strInitFileName; }

	/**
	 * @brief <b>インストールされているSiNDY-eのバージョンを取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version\\Version
	 * @return バージョン名
	 */
	LPCTSTR GetVersion() const { return m_strVersion; }

	/**
	 * @brief <b>警告レベルを取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\Warning\\Level
	 * @return 警告レベル
	 */
	long GetWarnLevel() const { return m_lWarnLevel; }

	/**
	 * @brief <b>ルール用のパーソナル・ジオデータベースのパスを取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\SiNDY-e\\Tools\\COMMON\\PGDBFileName
	 * @return PGDBファイルパス
	 */
	LPCTSTR GetPGDBFileName() const { return m_strPGDBFileName; }

	/**
	 * @brief <b>ルールデータベースにパーソナル・ジオデータベースを使用するかどうかのフラグを取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\UsePGDB
	 * @return PGDBを使用する場合はtrue
	 */
	bool GetUsePGDB() const { return m_bUsePGDB; }

	/**
	 * @brief <b>iniファイルにかかれているサーバ名を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：settings.ini\\workdb\\servername\n
	 * 通常このサーバ名はルール接続用のサーバ名になります。
	 * @return サーバ名
	 */
	LPCTSTR GetServerName() const { return m_strServerName; }

	/**
	 * @brief <b>iniファイルにかかれている選択閾値を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：settings.ini\\Tools\\Tolerance\n
	 * @return 選択閾値
	 */
	int GetTolerance() const;// { return m_nTolerance; }

	/**
	 * @brief <b>再描画範囲の拡張幅を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\RefreshBuffer
	 * @return 再描画範囲の拡張幅（px）
	 */
	int GetRefreshBuffer() const { return m_nRefreshBuffer; }

	/**
	 * @brief <b>トレースログファイル名を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName
	 * @return トレースログファイルパス
	 */
	LPCTSTR GetTraceLogFileName() const { return m_strTraceLogFileName; }

	/**
	 * @brief <b>エラーログファイル名を取得する</b>\n
	 * Loadで情報を読み込んだ後に実行してください。\n
	 * 取得するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileName
	 * @return エラーログファイルパス
	 */
	LPCTSTR GetErrorLogFileName() const { return m_strErrorLogFileName; }
	//@}

	//@{ @name 書き込み系関数
	/**
	 * @brief <b>ルール用のパーソナル・ジオデータベースのパスをレジストリに書き込む</b>\n
	 * 設定するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\PGDBFileName
	 * @param lpcszFileName		[in]	PGDBファイル名（フルパス）
	 * @retval sindyErr_RegOpenFailed	レジストリのオープンに失敗した
	 * @retval sindyErr_RegWriteFailed	レジストリの書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetPGDBFileName( LPCTSTR lpcszFileName );

	/**
	 * @brief <b>ルールデータベースにパーソナル・ジオデータベースを使用するかどうかのフラグをレジストリに書き込む</b>\n
	 * 設定するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\UsePGDB
	 * @param bUsePGDB			[in]	PGDBをルールに使用するかどうか
	 * @retval sindyErr_RegOpenFailed	レジストリのオープンに失敗した
	 * @retval sindyErr_RegWriteFailed	レジストリの書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetUsePGDB( bool bUsePGDB );

	/**
	 * @brief <b>iniファイルに選択閾値を設定する</b>\n
	 * SiNDY-eのインストールパスを取得する必要があるので、Loadで情報を読み込んだ後に実行してください。\n
	 * 設定するキー：settings.ini\\Tools\\Tolerance\n
	 * @retval sindyErr_RegWriteFailed	書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetTolerance( int nTolerance );

	/**
	 * @brief <b>再描画領域の拡張幅を設定する</b>\n
	 * 設定するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName\\RefreshBuffer
	 * @retval sindyErr_RegWriteFailed	書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetRefreshBuffer( int nRefreshBuf );

	/**
	 * @brief <b>トレースログファイル名を設定する</b>\n
	 * 設定するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileName
	 * @param lpcszFileName		[in]	トレースログファイル名
	 * @retval sindyErr_RegOpenFailed	レジストリのオープンに失敗した
	 * @retval sindyErr_RegWriteFailed	レジストリの書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetTraceLogFileName( LPCTSTR lpcszFileName );

	/**
	 * @brief <b>エラーログファイル名を設定する</b>\n
	 * 設定するキー：HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileName
	 * @param lpcszFileName		[in]	エラーログファイル名
	 * @retval sindyErr_RegOpenFailed	レジストリのオープンに失敗した
	 * @retval sindyErr_RegWriteFailed	レジストリの書き込みに失敗した
	 * @retval sindyErr_NoErr			書き込み成功
	 */
	bool SetErrorLogFileName( LPCTSTR lpcszFileName );
	//@]
	/**
	 * @brief レジストリやsettings.iniの内容をメンバにキャッシュする
	 * @return sindyErrCode
	 */
	bool Load();
private:
	CString	m_strInstallRoot;		//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot\\Pathの内容のキャッシュ
	CString	m_strInitFileName;		//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\InstallRoot\\InitFileNameの内容のキャッシュ
	CString	m_strVersion;			//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Version\\Versionの内容のキャッシュ
	CString	m_strServerName;		//!< settings.ini\\workdb\\servernameの内容のキャッシュ
	long	m_lWarnLevel;			//!< HKEY_LOCAL_MACHINE\\Software\\INCREMENT P CORPORATION\\SiNDY-e\\Warning\\Levelの内容のキャッシュ
	CString m_strPGDBFileName;		//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\PGDBFileNameの内容のキャッシュ
	bool	m_bUsePGDB;				//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\UsePGDBsの内容のキャッシュ
	CString m_strTraceLogFileName;	//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\TraceLogFileNameの内容のキャッシュ
	CString m_strErrorLogFileName;	//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\ErrorLogFileNameの内容のキャッシュ
	int		m_nTolerance;			//!< settings.ini\\Tools\\Toleranceの内容のキャッシュ
	int		m_nRefreshBuffer;		//!< HKEY_CURRENT_USER\\Software\\Increment P Corporation\\SiNDY-e\\Tools\\COMMON\\RefreshBufferの内容のキャッシュ
};

#endif // _SINDYESETTINGS_H_
