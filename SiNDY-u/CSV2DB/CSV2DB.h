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

#include "Arguments.h"
#include <TDC/sindylib_base/TableFinder.h>

namespace sindy{
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}


/**
 * @brief   メインクラス
 */
class CCSV2DB
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCSV2DB( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	~CCSV2DB(){};

	/**
	 * @brief   実行
	 * @note エラーがあった場合は例外を投げる
	 */
	void execute();

private:

	/**
	 * @brief 対象テーブルを取得する
	 * @note  失敗した場合は例外を投げる
	 * @param work [in] 対象のワークスペース。nullptrの場合は引数の接続先
	 * @return workに存在する引数で指定したテーブル
	 */
	sindy::CSPTableContainer GetDestTable( const IWorkspacePtr work = nullptr );

private:
	Arguments m_args;             //!< 引数
	sindy::CTableFinder m_finder; //!< ファインダ
};

