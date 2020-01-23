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
class CorrectHighwayText
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CorrectHighwayText( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CorrectHighwayText(){}

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
	* @brief   編集開始
	* @note    編集を行うのはHIGHWAY_TEXTで件数も多くないので、StartEditOperation()も関数内で行う
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool startEdit();

	/**
	* @brief   編集終了
	* @retval  true  成功
	* @retval  false 失敗
	*/
	bool stopEdit(bool bSave);

	/**
	 * @brief  編集セッションを破棄する
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool abortEditOperation();

	/**
	 * @brief  HIGHWAY_TEXTの整合合わせ処理
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool correctHwyText();

	/**
	 * @brief   テーブルファインダ初期化
	 * @param   table  [in] テーブル名
	 * @param   finder [out] テーブルファインダ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool initTableFinder(const CString& table, sindy::CTableFinder& finder);

	/**
	 * @brief   不正なPOINT_IDの値を修正する
	 * @param   spHwyTextT [in] HIGHWAY_TEXTテーブル
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool correctIllegalPointID(sindy::CSPTableContainer& spHwyTextT);

	/**
	* @brief   不正なPOINT_IDと同じ値のPOINT_IDが存在するか
	* @note    存在する場合、correctRelPointIDFields()で元々の正常なデータも更新されてしまうのでチェックする。
	* @param   spHwyTextT [in] HIGHWAY_TEXTテーブル
	* @param   pointIDs [in] 更新前POINT_ID群
	* @retval  true  存在する
	* @retval  false 存在しない
	*/
	bool existBeforeEditPointID(sindy::CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs);

	/**
	 * @brief   不正なPOINT_IDと同じ値を持っている関連フィールド群を修正する
	 * @param   spHwyTextT [in] HIGHWAY_TEXTテーブル
	 * @param   pointIDs [in] 不正なPOINT_ID群
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool correctRelPointIDFields(sindy::CSPTableContainer& spHwyTextT, const std::vector<long>& pointIDs);

	/**
	 * @brief   編集したデータをストア
	 * @param   spHwyTextT [in] HIGHWAY_TEXTテーブル
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool store(sindy::CSPTableContainer& spHwyTextT);

private:

	Arguments m_args;            //!< 引数
	sindy::CWorkspace m_tgtWork; //!< 処理対象接続先
	std::map<long, long> m_illegalPointIDs; //!< first：修正対象POINT_ID, second：更新後POINT_ID（OBJECTID）

};

