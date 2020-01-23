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

#include "stdafx.h"
#include "Common.h"


// プログラム引数
char* const g_strIniFile	= "ini";
char* const g_strRunLog		= "run_log";
char* const g_strErrLog		= "err_log";
#define OPT_INI_FILE	"ini,i"
#define OPT_RUN_LOG		"run_log,r"
#define OPT_ERR_LOG		"err_log,e"

#define INI_REL_TBL		"REL_TBL"
#define INI_WN_DIR		"WN_DIR"
#define INI_POI_LIST	"POI_FILE_LIST"
#define INI_MESHLIST	"MESHLIST"
#define INI_CODE_LIST	"CONTENTS_CODE_LIST"
#define INI_NODECLASS	"NODECLASS"
#define INI_DISTANCE	"DISTANCE"
#define INI_PARAM		"PARAM"

#define DISTANCE_DEF    "50.0"

const CString CHECK_INTEGER_STR	= _T("1234567890");
const CString CHECK_DOUBLE_STR	= _T("1234567890.");

namespace po = boost::program_options;

class CParam
{
public:
	CParam(void);
	~CParam(void);

	/**
	 * @brief	パラメータチェック
	 * @param	argc	[in]	入力パラメータ数
	 * @param	argv[]	[in]	入力パラメータ
	 * @return					処理成否
	 */
	bool checkParam(int argc, _TCHAR* argv[]);

	/**
	 * @brief	指定オプション表示用文字列取得
	 */
	std::string getOption();

	/**
	 * @brief	設定ファイルパラメータ取得
	 */
	std::string getIniFile() const { return uh::toStr(m_tstrIniFile); };

	/**
	 * @brief	実行ログパラメータ取得
	 */
	std::string getRunLog() const { return uh::toStr(m_tstrRunLog); };

	/**
	 * @brief	エラーログパラメータ取得
	 */
	std::string getErrLog() const { return uh::toStr(m_tstrErrLog); };

	/**
	 * @brief	関連テーブル設定値取得
	 */
	std::string getRelTbl() const { return m_strRelTbl; };

	/**
	 * @brief	道路・歩行者ノード格納Dir設定値取得
	 */
	std::string getWnDir() const { return m_strWnDir; };

	/**
	 * @brief	POIリストファイル設定値取得
	 */
	std::string getPoiList() const { return m_strPoiList; };

	/**
	 * @brief	メッシュリストファイル設定値取得
	 */
	std::string getMeshList() const { return m_strMeshList; };

	/**
	 * @brief	コンテンツコードリストファイル設定値取得
	 */
	std::string getCodeList() const { return m_strCodeList; };

	/**
	 * @brief	ノード種別設定値取得
	 */
	std::string getNodeClass() const { return m_strNodeClass; };

	/**
	 * @brief	距離閾値設定値取得
	 */
	std::string getDistance() const { return m_strDistance; };

	/**
	 * @brief	世界測地系パラメータファイル設定値取得
	 */
	std::string getParam() const { return m_strParam; };

private:
	/**
	 * @brief	設定ファイルチェック
	 * @return		処理成否
	 */
	bool checkIni();

	/**
	 * @brief	指定オプション表示用文字列取得（1Line用）
	 */
	std::string getOptionLine(const char* const cParam, std::string strValue);

	/**
	 * @brief	設定ファイル項目表示用文字力取得
	 */
	std::string getIniItem();

	/**
	 * @brief	設定ファイル項目表示用文字列取得（1Line用）
	 */
	std::string getIniItemLine(const char* const cParam, std::string strValue);

	/**
	 * @brief	エラー出力
	 */
	void outputCerrItem(std::string errKind, std::string itemName, std::string msg);

private:
	uh::tstring	m_tstrIniFile;		// 設定ファイル						（フルパス）
	uh::tstring	m_tstrRunLog;		// 実行ログファイル					（フルパス）
	uh::tstring	m_tstrErrLog;		// エラーファイル					（フルパス）

	std::string	m_strRelTbl;		// 関連テーブルファイル				（フルパス）
	std::string	m_strWnDir;			// 道路・歩行者ノード格納ディレクトリ	（フルパス）
	std::string	m_strPoiList;		// POIファイルリスト					（フルパス）
	std::string	m_strMeshList;		// メッシュリスト					（フルパス）
	std::string	m_strCodeList;		// コンテンツコードリスト				（フルパス）
	std::string	m_strNodeClass;		// 歩行者ノード種別
	std::string	m_strDistance;		// 距離チェック用閾値
	std::string	m_strParam;			// 世界測地系パラメータファイル		（フルパス）
};
