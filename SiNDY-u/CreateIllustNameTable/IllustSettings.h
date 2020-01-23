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

#include <TDC/illustlib/IllustOutputInfo.h>

/**
 * @brief 処理情報設定レコードクラス
 */
class IllustSettingsRec
{
public:
	/**
	 * @brief コンストラクタ
	 */
	IllustSettingsRec() {}

	/**
	 * @brief デストラクタ
	 */
	~IllustSettingsRec() {}

	/**
	 * @brief イラストファイル命名規則の設定
	 * @param strNamingRule [in] 命名規則文字列（カンマ区切り）
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool setNamingRule( const std::string& strNamingRule );

public:
	int						m_nIllustClass;		//!< イラスト種別コード
	int						m_nCustomer;		//!< 仕向先コード
	CString					m_strPrefix;		//!< イラストファイルのプレフィックス
	CString					m_strExtension;		//!< イラストファイルの拡張子
	std::vector<CString>	m_vecNamingRule;	//!< イラストファイルの命名規則
	CString					m_strIllustDir;		//!< イラストファイルの格納先ディレクトリ
	CIllustOutputInfo::OutputInfo	m_cOutputInfo;	//!< ノードリスト出力情報ファイルレコード（出力除外条件のために使用）
	// ↑本当は std::vector<CIllustOutputInfo::ExceptionCond> のみを利用したいが、出力除外条件に合致するかどうか判定する
	// CIllustOutputInfo::IsException() のパラメータとして CIllustOutputInfo::OutputInfo が求められるため、このようにしている
};

/**
 * @brief 処理情報設定クラス
 * @note  「イラスト種別コード」「仕向先コード」の組（pair）をキーとしたmapでレコード情報を保持する
 */
class IllustSettings : public std::map< std::pair< int, int >, IllustSettingsRec >
{
public:
	/**
	 * @brief コンストラクタ
	 */
	IllustSettings();

	/**
	 * @brief デストラクタ
	 */
	~IllustSettings();

	/**
	 * @brief 処理情報設定ファイルを読み込み
	 * @param strFileName [in] 処理情報設定ファイル名
	 * @param ipIllustLinkTable [in] イラストリンクテーブル
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool load( const CString& strFileName, const ITablePtr& ipIllustLinkTable );

private:
	/**
	 * @brief イラスト種別コード・仕向先コードがコード値ドメインの範囲に含まれるかをチェック
	 * @param ipIllustLinkTable [in] イラストリンクテーブル
	 * @retval true  コード値ドメインの範囲に含まれる
	 * @retval false コード値ドメインの範囲に含まれない
	 */
	bool checkCodesRange( const ITablePtr& ipIllustLinkTable );

	/**
	 * @brief イラストファイルの格納先ディレクトリが存在することをチェック
	 * @retval true  すべてのディレクトリが存在する
	 * @retval false 存在しないディレクトリがある
	 */
	bool checkDirExistence();

private:
	CIllustOutputInfo	m_cIllustOutputInfo;	//!< ノードリスト出力情報ファイル管理クラス（出力除外条件の処理のために使用）
};
