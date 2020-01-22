#pragma once

// プログラム引数
static const char* g_strFileListPath = "filelist";
static const char* g_strFileType = "filetype";
static const char* g_strCharCodeListPath = "charcodelist";
static const char* g_strExitMode = "exitmode";
static const char* g_strRunLogFilePath = "runlog";
static const char* g_strErrLogFilePath = "errlog";

static const CString g_strSHAPE = "shape";
static const CString g_strIMMEDIATE = "immediate";
static const CString g_strALL = "all";

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments(){};

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief 標準出力、実行ログに出力するオプションの一覧を出力する
	 * @return 入力されたオプションの一覧の文字列
	 */
	CString Arguments::GetOption();

	/**
	 * @brief オプションで入力された値が問題ないかチェックする
	 * @retval true  オプションの値に問題なし
	 * @retval false オプションの値に問題あり
	 */
	bool Arguments::CheckOptionParameter();

private:
	/**
	 * @brief 標準出力、実行ログに出力するオプションとその値の情報を整形する
	 * @param strParam [in] オプションのパラメータ名
	 * @param strValue [in] オプションに対して入力された値
	 * @return オプションとその値を整形した文字列
	 */
	CString GetOptionLine(const CString& strParam, const CString& strValue);

public:
	uh::tstring	strFileListPath;		//!< 入力ファイルリストのパス
	uh::tstring strFileType;			//!< チェック対象のファイルタイプ
	uh::tstring strCharCodeListPath;	//!< 許容文字コード範囲指定ファイルのパス
	uh::tstring strExitMode;			//!< 不正文字コード検出時の終了モード
	uh::tstring strRunLogFilePath;		//!< 実行ログのパス
	uh::tstring strErrLogFilePath;		//!< エラーログのパス
};
