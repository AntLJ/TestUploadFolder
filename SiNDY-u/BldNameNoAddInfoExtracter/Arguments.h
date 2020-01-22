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
#include <TDC/useful_headers/boost_program_options.h>

namespace option{
	extern const std::string kBldNameDb;     //!< 建物ビル名称テーブルのあるDBの接続文字
	extern const std::string kAddrDb;        //!< 住所DB（areacode_masterのあるDB）
	extern const std::string kAdminDb;       //!< CITY_ADMINのあるDBの接続文字列
	extern const std::string kPoiDb;         //!< POI_POINT_ORGのあるDBの接続文字列
	extern const std::string kBaseAdminList; //!< 行政界コード（5桁）リスト
	extern const std::string kRunLog;        //!< 実行ログファイルのパス
	extern const std::string kErrLog;        //!< エラーログファイルのパス
	extern const std::string kResult;        //!< 実行結果リストのパス
} // namespace arg

  /**
  * @brief 引数情報クラス
  * 単体オプションの情報を管理する
  */
class CArgInfo
{
private:
	CArgInfo();
public:
	/**
	* @brief コンストラクタ
	* @note 扱うオプションの登録を行う
	* @param arg_ [in] オプション名 
	* @param desc_ [in] オプションの説明
	* @param required_ [in] 必須ならtrue説明
	* @param flag_ [in] フラグオプションならtrue説明
	* @param default_ [in] デフォルト値
	*/
	CArgInfo( const std::string& arg_, const std::string& desc_, bool required_ = false, bool flag_ = false, const std::string& default_ = "" );

	std::string arg;      //!< オプション名
	std::string desc;     //!< 説明
	bool required;        //!< 必須か
	bool flag;            //!< フラグオプションか
	std::string def_val;  //!< デフォルト値
};

/**
* @brief 引数管理クラス
* オプション全体の情報を管理する
*/
class CArgInfos
{
public:
	void add(const CArgInfo& arg);
	bool parse(int argc, TCHAR* argv[]);
	CString getValue( const std::string& arg ) const;
	bool checkArg() const;
	bool hasOption( const std::string& arg ) const;

	std::vector<CArgInfo>::const_iterator begin() const { return m_argInfos.cbegin(); }
	std::vector<CArgInfo>::const_iterator end() const { return m_argInfos.cend(); }

	std::vector<CArgInfo> m_argInfos;
	boost::program_options::variables_map m_args;
};

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

	/**
	* @brief 引数で指定された値を取得する
	* @param arg [in] 引数名
	* @return 引数で指定された値
	*/
	CString getValue( const std::string& arg ) const { return m_args.getValue( arg ); }

	/**
	* @brief 引数で指定されているか
	* @note フラグ引数の確認に使用する
	* @param arg [in] 引数名
	* @return 引数で指定されていればtrue
	*/
	bool hasOption( const std::string& arg ) const { return m_args.hasOption( arg ); };

	/**
	* @brief 指定されている引数を取得する
	* @note ログや標準出力への表示用
	* @return 引数名、値
	*/
	CString description() const;

public:
	CArgInfos m_args; //!< 引数群
};
