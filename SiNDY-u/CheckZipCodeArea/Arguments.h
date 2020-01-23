#pragma once

namespace exec_opt
{
	// チェック実行モードオプション引数
	extern const uh::tstring CHECK_TABLE;
	extern const uh::tstring CHECK_TABLEMASTER;
	extern const uh::tstring CHECK_TABLEADDR;
	extern const uh::tstring CHECK_POLYGON;
	extern const uh::tstring CHECK_POLYGONTABLE;
	extern const uh::tstring CHECK_POLYGONADDR;
	extern const uh::tstring CHECK_ALL;
}

/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments()
	{
	}

	/**
	 * @brief コマンドライン引数の解析
	 * @param	argc [in]	コマンドライン引数の数
	 * @param	argv [in]	コマンドライン引数の内容
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool parse(int argc, _TCHAR * argv[]);

	/**
	 * @brief  コマンドライン引数の確認
	 * @param  optName  [in]  オプション名
	 * @param  optValue [in]  オプション値
	 * @param  bFile    [in]  ファイルであるか（デフォルトfalse）
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParam(const char* const optName, const uh::tstring& optValue, bool bFile = false);

	/**
	 * @brief 全指定オプションを文字列として取得する
	 * @retval 指定オプションの文字列
	 */
	CString GetAllOptionInfo();

public:
	uh::tstring m_checkMode; //!< チェックモード
	uh::tstring m_zipDb; //!< 郵便番号ポリゴンDB接続先
	uh::tstring m_addrDb; //!< 住所データDB接続先
	uh::tstring m_charConv; //!< 文字付き住所コード変換テーブル
	uh::tstring m_charConvTmp; //!< 文字付き住所コード変換仮テーブル
	uh::tstring m_jusyoMaster; //!< 住所マスタ接続文字列
	uh::tstring m_kajyoMaster; //!< 加除マスタ接続文字列
	uh::tstring m_dupliRate; //!< ポリゴン重複率の閾値
	uh::tstring m_mesh; //!< メッシュリスト
	uh::tstring m_runLog; //!< 実行ログパス
	uh::tstring m_errLog; //!< エラーログパス
};
