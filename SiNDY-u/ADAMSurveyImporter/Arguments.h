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


/**
 * @brief コマンドライン引数クラス
 */
class Arguments
{
public:
	/**
	 * @brief コンストラクタ
	 */
	Arguments();

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

	/**
	 * @brief  データ作成元の確認
	 * @param  optValue [in]  companyオプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParamCompany(const uh::tstring& optValue);

	/**
	 * @brief  データ受領日の確認
	 * @param  optValue [in]  companyオプション値
	 * @retval true:    引数OK
	 * @retval false:   引数NG
	 */
	bool CheckParamDate(const uh::tstring& optValue);
public:
	//データ作成元 固定値
	static const uh::tstring PS;	//!< パスコ
	static const uh::tstring AT;	//!< アイサン
	static const uh::tstring KK;	//!< 国際航業
	static const uh::tstring IPC;	//!< iPC

	uh::tstring		survey_dir;		//!< [in]インポートファイルディレクトリパス
	uh::tstring		import_db;		//!< インポート先DB接続情報
	uh::tstring		company;		//!< データ作成元
	uh::tstring		date;			//!< データ受領日
	uh::tstring		lot;			//!< ロット名
	uh::tstring		log_dir;		//!< ログ出力先フォルダパス
	uh::tstring		crd_param;		//!< 座標変換用パラメータファイル

	bool			unuse_version;		//!< バージョン利用するか(true:しない)
	bool			skip_delete_record;	//!< 更新コード「削除」のレコードをスキップするか
};
