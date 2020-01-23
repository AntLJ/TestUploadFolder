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
#include <fstream>

enum ErrorLevel
{
	Info,
	Warning,
	Error,
	Fatal,
};

class LogManager
{
public:
	LogManager(void);
	~LogManager(void);

	/**
	 * @brief 現在時刻の取得
	 * @retval 現在のシステム時刻
	 */
	CString GetCurrentTimeString()
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString currentTime;
		currentTime.AppendFormat(
			_T("%04d/%02d/%02d %02d:%02d:%02d"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		return currentTime;
	}

	/**
	 * @brief 実行ログを開く
	 * @param path [in] 実行ログのパス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool OpenRunLog(const CString& path);

	/**
	 * @brief エラーログを開く
	 * @param path [in] エラーログのパス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool OpenErrLog(const CString& path);

	/**
	 * @brief 実行ログの書き込み
	 * @param path [in] 書き込む文字列
	 */
	void WriteRunLog(const CString& message);

	/**
	 * @brief エラーログにコメントを出力
	 * @note 与えた文字列の先頭に#を付けて出力する。
	 */
	void WriteErrComment(const CString& comment);

	/**
	 * @brief エラーログ書き込み
	 * @param layer [in] レイヤ名
	 * @param objectId [in] オブジェクトID
	 * @param level [in] エラーレベル
	 * @param message [in] エラーメッセージ
	 * @param info1 [in] 情報1
	 * @param info2 [in] 情報2
	 */
	void WriteErrLog(
		const CString& layer, long objectId,
		ErrorLevel level, const CString& message,
		const CString& info1 = _T(""), const CString& info2 = _T(""));

private:
	/**
	 * @brief ログを開く
	 * @param path [in] エラーログのパス
	 * @param ofs [out] ログファイルストリーム
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool OpenLog(const CString& path, std::ofstream& ofs);

	/**
	 * @brief ログ書き込み
	 * @param message [in] 書き込む文字列
	 * @param ofs [in] ログファイルストリーム
	 */
	void WriteLog(const CString& message, std::ofstream& ofs);

private:
	std::ofstream m_ofsRunLog; //!< 実行ログファイルストリーム
	std::ofstream m_ofsErrLog; //!< エラーログファイルストリーム
};

