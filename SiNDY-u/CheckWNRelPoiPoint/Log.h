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
#include "Param.h"

#define RUN_LOG(msg) CLog::WriteRunLog(msg)
#define ERR_LOG(layer, nid, pid, mesh, msg) CLog::WriteErrLog(layer, nid, pid, mesh, LOG_ERROR, msg)

#define NULL_CHAR	""
#define DSP_OK		"OK"
#define DSP_NG		"NG"
#define DSP_HYPHEN	"-"

enum LOG_TYPE
{
	ETYPE_RUN,
	ETYPE_ERR
};

/**
 * @brief ログ出力用クラス
 */
/**
 * @class CLog
 * @brief ログ出力用クラス\n
 * @note  実行ログ・エラーログを出力する
 */
class CLog
{
public:
	CLog(void);
	~CLog(void);

	/**
	 * @brief	ログ初期化
	 * @param	cParam	[in]	入力パラメータ
	 * @return					処理成否
	 */
	static bool initLog(CParam& cParam);

	/**
	 * @brief	実行ログ書込み
	 * @param	strMsg	[in]	出力メッセージ
	 */
	static void WriteRunLog(const std::string& strMsg);

	/**
	 * @brief	エラーログ書込み
	 * @param	sLayer		[in]	レイヤ
	 * @param	sNodeId		[in]	NODE ID
	 * @param	sPoiId		[in]	POI ID
	 * @param	sMeshCode	[in]	メッシュコード
	 * @param	sErrType	[in]	エラータイプ
	 * @param	strMsg		[in]	出力メッセージ
	 */
	static void WriteErrLog(const std::string& sLayer,
							const std::string& sNodeId,
							const std::string& sPoiId,
							const std::string& sMeshCode,
							const std::string& sErrType,
							const std::string& strMsg);

	/**
	 * @brief	実行ログ書込み(処理開始)
	 * @param	sysTime	[in]	開始時間
	 */
	static void WriteRunLogSTART(const SYSTEMTIME& sysTime);

	/**
	 * @brief	実行ログ書込み(処理終了)
	 * @param	sysTime	[in]	終了時間
	 * @param	csTotal	[in]	処理時間
	 */
	static void WriteRunLogEND(const SYSTEMTIME& sysTime, const CString& csTotal);

	/**
	 * @brief	実行ログ書込み(改行無し)
	 * @param	strMsg	[in]	出力メッセージ
	 */
	static void WriteRunLogNotNewLine(const std::string& strMsg);

private:
	/**
	 * @brief	実行ログ書込み(時間)
	 * @param	sysTime		[in]	時間
	 * @param	sMessage	[in]	処理内容
	 * @param	sTotal		[in]	処理時間
	 */
	static void WriteRunLogTIME(const SYSTEMTIME& sysTime,
								const CString& csMessage,
								const CString& csTotal);

	/**
	 * @brief	ログファイルオープン
	 * @param	eType	[in]	ログタイプ
	 * @param	strPath	[in]	ログ出力ディレクトリ
	 * @return					処理成否
	 */
	static bool OpenLog(const LOG_TYPE eType, const std::string& strPath);

public:
	static std::ofstream	m_ofsRunLog;
	static std::ofstream	m_ofsErrLog;	
	static bool				m_bRunLog;		// 実行ログ存在チェック用
	static bool				m_bErrLog;		// エラーログ存在チェック用
};
