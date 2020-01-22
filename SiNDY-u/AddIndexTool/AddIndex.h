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
#include "DataDef.h"

/**
 * @brief インデックスを付与するクラス
 */
class CAddIndex
{
public:
	/**
	 * @brief コンストラクタ
	 * @param db           [in] 対象DBの接続文字列
	 * @param indexInfoMap [in] インデックス付与に必要な情報
	 */
	CAddIndex(
		const uh::tstring& db
		, const index_def::INDEXINFOMAP& indexInfoMap):
		m_db(db)
		, m_indexInfoMap(indexInfoMap)
	{}

	/**
	 * @brief インデックス付与処理を実行
	 * @retval true  処理に成功
	 * @retval false 処理に失敗
	 */
	bool execute();

protected:
	/**
	 * @brief スキーマをロックする
	 * @param layer     [in] ロック対象レイヤ
	 * @param layerName [in] ロック対象レイヤの名前
	 * @retval true  ロックに成功
	 * @retval false ロックに失敗
	 */
	bool lockSchema(ITablePtr& layer, const uh::tstring& layerName);

	/**
	 * @brief スキーマのロックを解除する
	 * @note ロック解除に失敗した場合、エラーメッセージを吐く
	 * @param layer     [in] ロック対象レイヤ
	 * @param layerName [in] ロック対象レイヤの名前
	 */
	void releaseSchema(ITablePtr& layer, const uh::tstring& layerName);

	/**
	 * @brief 対象レイヤにインデックス付与処理を実行
	 * @param layer         [in] 対象レイヤ
	 * @param layerName     [in] 対象レイヤの名前
	 * @param indexInfoList [in] インデックス操作に関する情報
	 */
	void addIndex(ITablePtr& layer
		, const uh::tstring& layerName
		, const std::vector<index_def::IndexInfo>& indexInfoList);

	/**
	 * @brief 対象レイヤに存在するインデックスが付与されたフィールド一覧を取得
	 * @param layer [in] 対象レイヤ
	 * @return インデックス名とそのインデックスが付与されたフィールド名リストの対応表
	 */
	std::map<uh::tstring, std::vector<uh::tstring> > loadIndexes(const ITablePtr& layer);

protected:
	const uh::tstring m_db; //!< 対象DBの接続文字列
	/// インデックス付与に必要な情報群
	const index_def::INDEXINFOMAP& m_indexInfoMap;
};
