#pragma once

#include "ctime.h"

namespace err_level {
	enum ELevel {
		warning,
		err,
		fatal,
	};
}

/**
* @brief ログ管理クラス
*/
class LogSys
{
private:
	/**
	 * @brief コンストラクタ
	 */
	LogSys(void);

	/**
	 * @brief デストラクタ
	 */
	~LogSys(void);

	/**
	 * @brief ログの出力設定
	 * @param ofsLog [in] ログファイル
	 * @param bCout [in] 標準出力の有無
	 * @param bCerr [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void writeLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = getTimeText();
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cout << CT2CA(lpszMsg);
			std::cout << std::flush;
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cerr << CT2CA(lpszMsg);
			std::cerr << std::flush;
		}
		if (ofsLog->is_open()){
			if (bTime)
				*ofsLog << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				*ofsLog << CT2CA(lpszMsg);
		}
	}

public:

	/**
	 * @brief インスタンスの取得
	 * @return インスタンス
	 */
	static LogSys& getInstance(void){
		static LogSys kInstance;
		return kInstance;
	}

	/**
	 * @brief ログファイルを作成
	 * @param cFilePath [in] ログファイルのパス
	 * @param ofsLogFile [in] ログ出力ファイルストリーム
	 * @retval	true 成功
	 * @retval	false 失敗
	 */
	bool setLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);

	/**
	 * @brief 実行ログファイルを設定
	 * @param cFilePath [in] 実行ログファイルのパス
	 * @retval	true 成功
	 * @retval	false 失敗
	 */
	bool setProcLog(const _TCHAR *cFilePath);

	/**
	 * @brief エラーログファイルを設定
	 * @param cFilePath [in] エラーログファイルのパス
	 * @retval	true 成功
	 * @retval	false 失敗
	 */
	bool setErrLog(const _TCHAR *cFilePath);


	/**
	 * @brief 実行ログの出力設定
	 * @param bCout [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void writeProcLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		writeLog(&m_ofsProcLog, bCout, false, bTime, lpszMsg);
	}

	/**
	 * @brief エラーログの出力設定
	 * @param bCerr [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void writeErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		writeLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}

	/**
	 * @brief エラーレベルの設定
	 * @param bCerr [in] 標準出力の有無
	 * @param eErrLevel [in] エラーレベル
	 * @param lpszErrMsg [in] 出力する文字列
	 * @param lpszLayer [in] 出力するレイヤ名
	 * @param lObjectId [in] 出力するオブジェクトID
	 */
	inline void writeFreeStyleLog(bool bCerr, err_level::ELevel eErrLevel, LPCTSTR lpszErrMsg, LPCTSTR lpszLayer = _T("-"), long lObjectId = -1)
	{
		CString strErrLevel;
		switch(eErrLevel){
		case err_level::warning:
			strErrLevel = _T("WARNING");
			break;
		case err_level::err:
			strErrLevel = _T("ERR");
			break;
		case err_level::fatal:
			strErrLevel = _T("FATAL");
			break;
		default:
			strErrLevel = _T("unknown");
		}
		CString strMsg;
		strMsg.Format(_T("0\t%s\t%ld\t%s\t%s\n"), lpszLayer, lObjectId, strErrLevel, lpszErrMsg);

		writeErrLog(bCerr, false, strMsg);
	}

private:
	std::ofstream m_ofsProcLog;	// 実行ログ出力ファイルストリーム
	std::ofstream m_ofsErrLog;	// エラーログ出力ファイルストリーム

};

