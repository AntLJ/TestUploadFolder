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

#include <iostream>
#include <string>
#include <vector>
#include "Conditions.h"

/// マージ時の属性コピー方法定義
enum AttributeSelector : int
{
	selectorAny, //!< どちらでもよい
	selectorNotNull, //!< NULLじゃないほう
	selectorBigger, //!< 大きい方
	selectorSmaller, //!< 小さい方
};

/**
 * @brief マージ実施定義クラス
 */
class MergingDefinition
{
public:
	MergingDefinition();
	MergingDefinition(const MergingDefinition &o);
	~MergingDefinition();

	uh::tstring m_strTableName;					//!< マージするテーブル名称
	Condition m_cCondition;
	IndexList m_idxList; //!< カラム対応表
};

//マージ実施定義管理クラス
class MergingDefinitionManager
{
public:
	MergingDefinitionManager() : m_setHistory(false) {}
	~MergingDefinitionManager(){}
	MergingDefinitionManager(const MergingDefinitionManager &o);

	/// マージ実施定義がされているかどうか
	operator bool() const
	{
		return ! m_mergingDefinitions.empty();
	}

	/**
	 * @brief  マージ実施定義クラスの数を返す
	 * @retval マージ実施定義クラスの数
	 */
	const unsigned size() const{ return m_mergingDefinitions.size(); }

	/**
	 * @brief  インデックスに対応するマージ実施定義クラスを返す(const版)
	 * @param  i  [in]  インデックス
	 * @retval マージ実施定義クラス
	 */
	const MergingDefinition& get (unsigned i) const
	{
		assert(i < m_mergingDefinitions.size());
		assert(i >= 0);
		return m_mergingDefinitions[i];
	}

	/**
	 * @brief  インデックスに対応するマージ実施定義クラスを返す
	 * @param  i  [in]  インデックス
	 * @retval マージ実施定義クラス
	 */
	const MergingDefinition& get(unsigned i)
	{
		assert(i < m_mergingDefinitions.size());
		assert(i >= 0);
		return m_mergingDefinitions[i];
	}

	/**
	 * @brief  XMLをを解析する
	 * @param  path  [in]  XMLファイルパス
	 * @retval
	 */
	bool  parse(const uh::tstring& path);

	/**
	* @brief 保存時に履歴を付与するかどうか
	* @retval false 履歴を付与しない
	* @retval true 履歴を付与する
	*/
	bool isSetHistory() const { return m_setHistory; }
private:
	std::vector<MergingDefinition> m_mergingDefinitions;	//!< マージ実施定義クラスのリスト
	bool m_setHistory; //!< 履歴を付与するかどうか
};
