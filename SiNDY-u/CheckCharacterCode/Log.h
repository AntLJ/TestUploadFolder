#pragma once

/**
 * @brief ログ出力用クラス
 */
class CLog
{
public:
	/**
	 * @brief コンストラクタ
	 */
	CLog(){}

	/**
	 * @brief デストラクタ
	 */
	~CLog(){
		if(m_ofsRunLog)
			m_ofsRunLog.close();
		if(m_ofsErrLog)
			m_ofsErrLog.close();
	}

public:
	/**
	 * @brief ログ出力クラスのインスタンス取得
	 * @return インスタンス
	 */
	static CLog& GetInstance(void){
		static CLog cInstance;
		return cInstance;
	}

	/**
	 * @brief ログ出力用のファイルストリームを設定する関数
	 * @param strFilePath [in] ログファイルパス
	 * @param ofsLogFile [out] ログファイルストリーム
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool SetLogFile(const CString& strFilePath, std::ofstream& ofsLogFile);

	/**
	 * @brief 実行ログのファイルストリームを設定する関数
	 * @param strFilePath [in] 実行ログファイルパス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool SetRunLog(const CString& strFilePath);

	/**
	 * @brief エラーログのファイルストリームを設定する関数
	 * @param strFilePath [in] エラーログファイルパス
	 * @retval true  処理成功
	 * @retval false 処理失敗
	 */
	bool SetErrLog(const CString& strFilePath);

	/**
	 * @brief メッセージ出力
	 * @note  この関数でcout,cerrに出力させている
	 * @param bCout      [in] 標準出力に出力するかどうか
	 * @param bCerr      [in] 標準エラー出力に出力するかどうか
	 * @param bRun       [in] 実行ログに出力するかどうか
	 * @param bErr       [in] エラーログに出力するかどうか
	 * @param strMsg     [in] ログ出力用メッセージ
	 * @param bLineBreak [in] 改行するか
	 */
	void PrintLog(bool bCout, bool bCerr, bool bRun, bool bErr, const CString& strMsg, bool bLineBreak = true);

private:
	std::ofstream m_ofsRunLog;	//!< 実行ログ用ファイルストリーム
	std::ofstream m_ofsErrLog;	//!< エラーログ用ファイルストリーム
};
