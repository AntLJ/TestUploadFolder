#pragma once

/**
 * @brief オプション名
*/
namespace arg {
	const char * const k_ipc_addr_db      = "ipc_addr_db";     // SiNDY（住所）接続先
	const char * const k_ipc_bg_db        = "ipc_bg_db";       // SiNDY（背景）接続先
	const char * const k_mid_addr_db      = "mid_addr_db";     // 住所中間ファイル
	const char * const k_mid_addr_layer   = "mid_addr_layer";  // 住所中間ファイルレイヤ名
	const char * const k_citycode_list    = "citycode_list";   // 市区町村コードリスト
	const char * const k_point_dist       = "point_dist";      // ポイント距離閾値
	const char * const k_run_log          = "run_log";         // 実行ログファイルパス
	const char * const k_err_log          = "err_log";         // エラーログフォルダパス
};

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
private:

/**
 * @brief オプション値種類
 */
enum ArgValueType {
	File,    // ファイル
	Folder,  // フォルダ
	Other    // その他
};

public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments() {}

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
	 * @param  type     [in]  オプション値の種類
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam( const char* const optName, const uh::tstring& optValue, ArgValueType type = ArgValueType::Other );


	/**
	 * @brief  オプション（point_dist）値の検査
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @retval true:    OK
	 * @retval false:   エラー
	 */
	bool CheckDistValue( const char* const optName, const uh::tstring& optValue );

public:
	uh::tstring m_ipc_addr_db;     //!< SiNDY（住所）接続先
	uh::tstring m_ipc_bg_db;       //!< SiNDY（背景）接続先
	uh::tstring m_ipc_mesh_db;     //!< SiNDY（メッシュ）接続先
	uh::tstring m_mid_addr_db;     //!< 住所中間ファイル
	uh::tstring m_mid_addr_layer;  //!< 住所中間ファイルレイヤ名
	uh::tstring m_citycode_list;   //!< 市区町村コードリスト
	uh::tstring m_point_dist;      //!< ポイント距離閾値
	uh::tstring m_run_log;         //!< 実行ログファイルパス
	uh::tstring m_err_log;         //!< エラーログフォルダパス
};
