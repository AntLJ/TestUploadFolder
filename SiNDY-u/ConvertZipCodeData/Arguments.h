#pragma once

#include <TDC/useful_headers/directory_util.h>
#include <TDC/useful_headers/boost_program_options.h>

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments(){}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

private:
	/**
	 * @brief コマンドライン引数の解析
	 * @param strRunLog [in] 実行ログパス
	 * @param strErrLog [in] エラーログパス
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool initLog(const CString& strRunLog, const CString& strErrLog);

	/**
	 * @brief コマンドライン引数出力用文字列取得
	 * @return コマンドライン引数文字列
	 */
	CString GetOption() const;

public:
	boost::program_options::variables_map m_vm;
	uh::tstring	m_zip_db;		//!< 郵便番号ポリゴンの接続先
	uh::tstring	m_zip_schema;	//!< 郵便番号ポリゴン接続先のスキーマ名（RONLY接続やレプリカで使用）
	uh::tstring	m_addr_db;		//!< 市区町村代表ポイント出力時の住所DB接続先
	uh::tstring	m_addr11_dir;	//!< 11桁住所データ格納先ディレクトリ
	uh::tstring	m_out_dir;		//!< データ出力先ディレクトリ
	uh::tstring	m_run_log;		//!< 実行ログ
	uh::tstring	m_err_log;		//!< エラーログ
};
