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
#include <memory>
#include <fstream>

/// 処理時間保持＆計算用
struct ExecTime
{
	time_t m_startTime; //!< 開始時刻
	time_t m_endTime;   //!< 終了時刻
};

enum ErrorCode
{
	info = 0,
	warning = 1,
	error = 2,
	critical = 3,
};

class CLogFile
{
public:
	CLogFile(void){};
	CLogFile( const CString& baseDirectory );
	~CLogFile(void);

	CStringA ChangeErrorCode2ErrorLevel( ErrorCode code )
	{
		CStringA level;
		switch( code )
		{
		case info:
			level = "INFO";
			break;
		case warning:
			level = "WARNING";
			break;
		case error:
			level = "ERROR";
			break;
		case critical:
			level = "CRITICAL";
			break;
		default:
			break;
		}
		return level;
	}

	/**
	 * @brief SINDYSTDLOG形式で出力
	 * @param layer [in] レイヤ名
	 * @param objectId [in] オブジェクトID
	 * @param errorCode [in] エラーコード
	 * @param message [in] エラーメッセージ
	 * @param meshCode [in] メッシュコード
	 */
	void WriteLog( const CString& layer, long objectId, ErrorCode errorCode, const CString& message, long meshCode );

	void WriteLogCore(long meshCode, const CStringA& errLog);

	void WriteLog(
		const CString& layer, long objectId, double lon, double lat, ErrorCode errorCode,
		const CString& message, long meshCode );

	/**
	 * @brief ログファイル作成
	 * @param meshCode [in] メッシュコード
	 */
	void InitLogFile( long meshCode );

	/**
	 * @brief ログファイル閉じる
	 * @param [in] ofs ログファイル出力ストリーム
	 */
	void CloseLogFile();

private:
	CString m_logDirctory; //!< ログ保存ディレクトリ
	std::map<long,ExecTime> m_execMesh; //!< 処理対象メッシュコード
	// TODO: 各executeALL()内でログ作るようにすれば、処理対象メッシュコードを全部持たなくても良くなるので、検討してみる
};

