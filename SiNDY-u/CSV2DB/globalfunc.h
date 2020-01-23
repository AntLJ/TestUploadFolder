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

#include <boost/shared_ptr.hpp>

extern const CString NULL_VALUE;    // NULL値指定

namespace sindy{
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}


/**
* @brief テーブルをオープンする
* @note 失敗した場合は例外を投げる
* @param work [in] 対象ワークスペース
* @param table [in] 対象のテーブル名
* @return 対象のテーブル
*/
ITablePtr TryOpenTable( const IWorkspacePtr& work, const CString& table );

/**
* @brief ワークスペースに接続する
* @note 失敗した場合は例外を投げる
* @param work [in] ワークスペース接続文字列
* @return 対象のワークスペース
*/
IWorkspacePtr TryConnect( const CString& conn );

/**
* @brief 子バージョンを作成する
* @note 失敗した場合は例外を投げる
* @param table [in] 対象のテーブル
* @return 作成した子バージョン。
*         バージョン非対応だった場合は対象テーブルのバージョン
*/
IWorkspacePtr TryCreateVersion( const sindy::CTableContainer& table );
