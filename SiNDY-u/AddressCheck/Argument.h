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
namespace po = boost::program_options;

/**
* @brief	引数格納用コンテナ
*/
struct Arguments
{
	uh::tstring edtAddrDB;     //!< 住所（編集）DB
	uh::tstring meshDB;        //!< メッシュDB
	uh::tstring mapDB;         //!< 背景ポリゴンDB
	uh::tstring cityMeshList;  //!< 都市地図メッシュコードリスト
	uh::tstring baseMeshList;  //!< ベースメッシュコードリスト
	uh::tstring layer;         //!< チェック対象リスト
	uh::tstring logDir;        //!< ログフォルダ
	uh::tstring iniFile;       //!< 設定ファイル
	uh::tstring master;        //!< テキスト住所マスタ
	uh::tstring runLog;        //!< 実行ログファイル
};

/**
 * @enum	argumentType
 * @brief	引数タイプ
 */
enum argumentType
{
	kAddressDBPath     = 1,   //!< 住所（編集）DB
	kMeshDBPath        = 2,   //!< メッシュDB
	kMapDBPath         = 3,   //!< 背景ポリゴンDB
	kMasterPath        = 4,   //!< テキスト住所マスタ
	kIniFilePath       = 5,   //!< 設定ファイル
	kCityMeshListPath  = 6,   //!< 都市地図メッシュコードリスト
	kBaseMeshListPath  = 7,   //!< ベースメッシュコードリスト
	kLayer             = 8,   //!< チェック対象リスト
	kLogDir            = 9,   //!< ログフォルダ
	kRunLog            = 10,  //!< 実行ログファイル
};

/**
 * @class	Argument
 * @brief	コマンドライン引数の処理
 * @note	
 */
class Argument
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	Argument(void){}

	/**
	 * @brief  デストラクタ
	 */
	virtual ~Argument(void){}

	/**
	* @brief 必要なオプション情報があるかをチェックする
	* @param [in] vm           引数のmap
	* @param [in] option       オプション名称
	* @param [in] desctiption  オプションの記述
	* @retval true 必要なオプションあり
	* @retval false 必要なオプションなし
	*/
	bool checkParameter(const po::variables_map& vm, LPCSTR option, LPCSTR desctiption) const;

	/**
	* @brief コマンドライン引数の解析
	* @param	[in] argc 	コマンドライン引数の数
	* @param	[in] argv 	コマンドライン引数の内容
	* @retval	true	成功
	* @retval	false	失敗
	*/
	bool setParameter(int argc, TCHAR** argv);

	/**
	* @brief   引数の取得
	* @param	[in] type 	引数タイプ
	* @return	引数の内容
	*/
	const CString& getValue(argumentType type);	

private:

	CString           m_addressDBPath; //!< 住所（編集）DB
	CString           m_meshDBPath;    //!< 都市地図メッシュDBパス
	CString           m_mapDBPath;     //!< 背景ポリゴンDBパス
	CString           m_masterPath;    //!< テキスト住所ファイルパス
	CString           m_iniFilePath;   //!< 設定ファイルパス
	CString           m_cityMeshListPath;  //!< 都市地図メッシュリストパス
	CString           m_baseMeshListPath;  //!< ベースメッシュリストパス
	CString           m_layer;         //!< チェック対象レイヤ
	CString           m_logDir;        //!< ログの出力フォルダパス
	CString           m_runLog;        //!< 実行ログファイルパス
};
