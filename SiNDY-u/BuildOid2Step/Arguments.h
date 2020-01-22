#pragma once


/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments():m_buf_distance(0.0){}

	/**
	 * @brief デストラクタ
	 */
	virtual ~Arguments(){}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, bool bFile = false );
public:
	uh::tstring  m_db;				//!< 処理対象DB
	uh::tstring  m_mesh_db;			//!< メッシュ参照先DB
	uh::tstring  m_mesh_list;		//!< メッシュリスト
	uh::tstring  m_run_log;			//!< 実行ログ
	uh::tstring  m_err_log;			//!< エラーログ
	double  m_buf_distance;			//!< メッシュバッファ距離（メートル）
};
