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

typedef uh::tstring ClippingDefinition;

/// クリッピングする単位
enum ClipType : int
{
	clipTypeMesh,  //!< メッシュ境界でクリッピング
	clipTypeLot, //!< ロット境界でクリッピング
};

//クリッピング実施定義管理クラス
class ClippingDefinitionManager
{
public:
	ClippingDefinitionManager() : m_setHistory(false), m_emClipType(clipTypeMesh) {}
	~ClippingDefinitionManager(){}
	ClippingDefinitionManager(const ClippingDefinitionManager &o);

	/// クリッピング実施定義がされているかどうか
	operator bool() const
	{
		return ! m_ClippingDefinitions.empty();
	}

	/**
	 * @brief  クリッピング実施定義クラスの数を返す
	 * @retval クリッピング実施定義クラスの数
	 */
	const unsigned size() const{ return m_ClippingDefinitions.size(); }

	/**
	 * @brief  インデックスに対応するクリッピング実施定義クラスを返す(const版)
	 * @param  i  [in]  インデックス
	 * @retval クリッピング実施定義クラス
	 */
	const ClippingDefinition& get (unsigned i) const
	{
		assert(i < m_ClippingDefinitions.size());
		assert(i >= 0);
		return m_ClippingDefinitions[i];
	}

	/**
	 * @brief  インデックスに対応するクリッピング実施定義クラスを返す
	 * @param  i  [in]  インデックス
	 * @retval クリッピング実施定義クラス
	 */
	const ClippingDefinition& get(unsigned i)
	{
		assert(i < m_ClippingDefinitions.size());
		assert(i >= 0);
		return m_ClippingDefinitions[i];
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

	/// クリップ処理のタイプを取得する
	ClipType getClipType() const { return m_emClipType; }
private:
	std::vector<ClippingDefinition> m_ClippingDefinitions;	//!< クリッピング実施定義クラスのリスト
	bool m_setHistory; //!< 履歴を付与するかどうか
	ClipType m_emClipType; //!< クリッピングする単位
};
