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
#include "Logger.h"
#include <TDC/sindylib_core/Workspace.h>
namespace sindy {
	class CTableFinder;
	class CTableContainer;
	typedef boost::shared_ptr<CTableContainer> CSPTableContainer;
}

/**
 * @brief   メインクラス
 */
class CheckConflict
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CheckConflict( const Arguments& args ){
		m_args = args;
	}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CheckConflict(){}

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();
	
	/**
	 * @brief   ログファイルの初期化
	 * @retval true  成功
	 * @retval false 失敗
	 */
	bool logInit();

	/**
	 * @brief   コンフリクトチェック
	 */
	void checkConflict();

	/**
	 * @brief   設定ファイル読み込み
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool readConfig();

	/**
	 * @brief   テーブルファインダ初期化
	 * @param   workspace [in] ワークスペース
	 * @param   table [in] テーブル名
	 * @param   finder [out] テーブルファインダ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool initTableFinder(const sindy::CWorkspace& workspace, const std::string& table, sindy::CTableFinder& finder);

	/**
	 * @brief   処理対象レコード群を検索し、キャッシュする
	 * @param   roopCount [in]     ループ回数（EXECCOUNTごとにループを回す）
	 * @param   table [in, out]    テーブルコンテナ
	 * @param   listTgtOIDs [out]  処理対象OBJECTID群
	 */
	void getCheckRecords(const long roopCount, sindy::CSPTableContainer& table, std::list<long>& listTgtOIDs);

	/**
	 * @brief   2テーブルのレコード同士を比較し、値に差異があるかをチェックする
	 * @param   listTgtOIDs [in] 処理対象OBJECTID群
	 * @param   tgtT  [in] チェック対象テーブル
	 * @param   diffT [in] 差分(比較先)テーブル
	 */
	void checkConflict(const std::list<long>& listTgtOIDs, const sindy::CSPTableContainer& tgtT, const sindy::CSPTableContainer& diffT);

private:

	Arguments m_args;      //!< 引数
	sindy::CWorkspace m_tgtWork;  //!< チェック対象接続先
	sindy::CWorkspace m_diffWork; //!< 差分DB接続先
	std::list<std::string> m_tgtTables;  //!< 処理対象テーブル群
};

