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

#include "StationChecker.h"

// STL
#include <memory>

// sindylib_core,sindylib_base
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableFinder.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>


/**
 * @brief   チェック管理クラスのインターフェース
 * @note    どのチェックを行うかを管理する
 * @note    実際のチェックロジックは StationChecker クラス に委譲
 */
class CheckHandler
{
public:

	/**
	 * @brief   初期化
	 * @param   workspace [in] チェック対象のワークスペース
	 * @param   checker   [in] チェックロジックを保持するオブジェクト
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	virtual bool init( const sindy::CWorkspace& workspace, SPStationChecker checker ) = 0;

	/**
	 * @brief   チェック実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	virtual bool check() = 0;

	/**
	 * @brief   チェック対象テーブル名を返す
	 * @return  テーブル名
	 */
	virtual uh::tstring getTarget() const = 0;

};

typedef std::shared_ptr<CheckHandler> SPCheckHandler;
