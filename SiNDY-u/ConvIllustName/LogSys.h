#pragma once
#include "ctime.h"

/**
* @brief ログ管理クラス（シングルトン）
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
	 * @param bCerr [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void WriteLog(std::ofstream *ofsLog, bool bCout, bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		std::string curr = GetTimeText();
		if (bCout) {
			if (bTime)
				std::cout << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cout << CT2CA(lpszMsg);
		}
		if (bCerr) {
			if (bTime)
				std::cerr << "[" << curr.c_str() << "]\t" << CT2CA(lpszMsg);
			else
				std::cerr << CT2CA(lpszMsg);
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
	static LogSys& GetInstance(void){
		static LogSys kInstance;
		return kInstance;
	}

	/**
	 * @brief ログファイルを作成
	 * @param cFilePath [in] ログファイルのパス
	 * @param ofsLogFile [in] ログ出力ファイルストリーム
	 * @return 成功したかどうか
	 */
	bool SetLogFile(const _TCHAR *cFilePath, std::ofstream *ofsLogFile);

	/**
	 * @brief 実行ログファイルを設定
	 * @param cFilePath [in] 実行ログファイルのパス
	 * @return 成功したかどうか
	 */
	bool SetProcLog(const _TCHAR *cFilePath);

	/**
	 * @brief エラーログファイルを設定
	 * @param cFilePath [in] エラーログファイルのパス
	 * @return 成功したかどうか
	 */
	bool SetErrLog(const _TCHAR *cFilePath);

	/**
	 * @brief 実行ログの出力設定
	 * @param bCout [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void WriteProcLog(bool bCout, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsProcLog, bCout, false, bTime, lpszMsg);
	}

	/**
	 * @brief エラーログの出力設定
	 * @param bCerr [in] 標準出力の有無
	 * @param bTime [in] 時間出力の有無
	 * @param lpszMsg [in] 出力する文字列
	 */
	inline void WriteErrLog(bool bCerr, bool bTime, LPCTSTR lpszMsg)
	{
		WriteLog(&m_ofsErrLog, false, bCerr, bTime, lpszMsg);
	}


private:
	std::ofstream m_ofsProcLog;	// 実行ログ出力ファイルストリーム
	std::ofstream m_ofsErrLog;	// エラーログ出力ファイルストリーム
};

