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
#include <TDC/useful_headers/directory_util.h>


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
	 * @brief デストラクタ
	 */
	~Arguments()
	{
	}

	/**
	 * @brief コマンドライン引数の解析
	 * @param argc [in] コマンドライン引数の数
	 * @param argv [in] コマンドライン引数の内容
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool parse(int argc, TCHAR* argv[]);

private:
	/**
	 * @brief 引数チェック
	 * @retval true チェックOK
	 * @retval false チェックNG
	 */
	bool chkOptions();

	/*
	 * @brief ファイル存在チェック
	 * @param file [in] チェック対象ファイル
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool fileExistChk(const CString& file);

	/**
	 * @brief ディレクトリ存在チェック
	 * @param dir [in] チェック対象ディレクトリ
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool dirExistChk(const CString& dir);

	/**
	 * @brief ファイル格納ディレクトリ存在チェック
	 * @param filePath [in] チェック対象ファイル
	 * @retval true 存在する
	 * @retval false 存在しない
	 */
	bool fileDirExistChk(const CString& filePath);

	/**
	 * @brief ログの初期化
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool initLog();

	/**
	* @brief コマンドライン引数出力用文字列取得
	* @return コマンドライン引数文字列
	*/
	CString GetOption() const;

public:
	boost::program_options::variables_map m_vm;
	uh::tstring m_roadDB;
	uh::tstring m_roadOwner;
	uh::tstring m_nameDB;
	uh::tstring m_nameOwner;
	uh::tstring m_dicPath;
	uh::tstring m_outDir;
	uh::tstring m_runLog;
	uh::tstring m_errLog;
};

