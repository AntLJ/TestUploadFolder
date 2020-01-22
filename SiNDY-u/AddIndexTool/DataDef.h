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

namespace index_def
{

/**
 * @brief インデックスの操作タイプ
 */
enum TYPE
{
	NONE,//!< 定義外
	ADD, //!< インデックス付与
	DEL, //!< インデックス削除
};

/**
 * @brief インデックス操作のために必要な情報の定義
 */
class IndexInfo
{
public:
	/// コンストラクタ
	IndexInfo():
		m_type(ADD)
		, m_order_asc(true)
		, m_unique(true)
	{}

	uh::tstring m_name;	//!< インデックスを生成するフィールドの名称
	TYPE m_type;        //!< インデックスの操作タイプ
	bool m_order_asc;   //!< インデックスの並び順(true:昇順, false:降順)
	bool m_unique;      //!< インデックスがユニークか否か
};


/**
 * @brief インデックス付与のために必要な情報を設定ファイルの定義順で格納
 * <レイヤ名, インデックス情報>
 */
class INDEXINFOMAP
{
protected:
	/// <レイヤ名, インデックス情報>
	typedef std::pair<uh::tstring,std::vector<IndexInfo>> VALUETYPE;

	std::vector<VALUETYPE> m_valueMap; // 定義を保持
public:
	/**
	 * @brief 指定のレイヤ名のインデックス情報を取得
	 * @detail 指定のレイヤ名の情報が存在しない場合、作成する
	 * @param name [in] レイヤ名
	 * @return レイヤ名に対応するインデックス情報
	 */
	std::vector<IndexInfo>& getIndexInfo(const uh::tstring& name);

	/**
	 * @brief 指定のレイヤ名の情報が存在するか判定
	 * @param name [in] レイヤ名
	 * @retval true  [in] 存在する
	 * @retval false [in] 存在しない
	 */
	bool isExist(const uh::tstring& name) const;

	/**
	 * @brief 先頭のイテレータを返す
	 * @return 先頭のイテレータ
	 */
	std::vector<VALUETYPE>::iterator begin()
	{
		return m_valueMap.begin();
	}
	/**
	 * @brief 末尾のイテレータを返す
	 * @return 末尾のイテレータ
	 */
	std::vector<VALUETYPE>::iterator end()
	{
		return m_valueMap.end();
	}

	/**
	 * @brief 先頭のイテレータを返す
	 * @return 先頭のイテレータ
	 */
	std::vector<VALUETYPE>::const_iterator begin() const
	{
		return m_valueMap.cbegin();
	}
	/**
	 * @brief 末尾のイテレータを返す
	 * @return 末尾のイテレータ
	 */
	std::vector<VALUETYPE>::const_iterator end() const
	{
		return m_valueMap.cend();
	}
};

} // index_def
