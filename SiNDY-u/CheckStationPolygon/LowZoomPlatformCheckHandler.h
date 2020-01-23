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

#include "CheckHandler.h"


/**
 * @brief   プラットフォーム全体ポリゴン(LOWZOOM_PLATFORM)のチェック管理クラス
 */
class LowZoomPlatformCheckHandler : public CheckHandler
{
public:
	LowZoomPlatformCheckHandler(void){}
	~LowZoomPlatformCheckHandler(void){}

	/**
	 * @brief   初期化
	 * @param   workspace [in] チェック対象のワークスペース
	 * @param   checker   [in] チェックロジックを保持するオブジェクト
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	virtual bool init( const sindy::CWorkspace& workspace, SPStationChecker checker ) override;

	/**
	 * @brief   チェック実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	virtual bool check() override;

	/**
	 * @brief   チェック対象テーブル名を返す
	 * @return  テーブル名
	 */
	virtual uh::tstring getTarget() const override { return sindy::schema::sj::lowzoom_platform::kTableName; }


private:

	SPStationChecker m_Checker;			//!< チェッカー

	sindy::CTableFinder m_TableFinder;	//!< テーブルファインダ

};
