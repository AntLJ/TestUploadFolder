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
class GridField
{
public:
	/**
	*	@フィールド名とグリッド列の対応
	*/
	struct FieldGridCol
	{
		CString m_field_name;	//!< フィールド名
		int m_col;				//!< グリッド列
		

		FieldGridCol(const CString& field, int col)
			: m_field_name(field), m_col(col) {}

		// グリッド列順に並べる
		bool operator<(const FieldGridCol& rec) const {
			return m_col < rec.m_col;
		}
	};

public:
	typedef std::set<FieldGridCol>::const_iterator CIter;

public:
	GridField() {};
	~GridField() {};

	CIter begin() const { return m_Data.begin(); }
	CIter end() const { return m_Data.end(); }
	size_t size() const { return m_Data.size(); }

	/**
	 *	@brief フィールド名とグリッド列の対応の追加
	 *  @param[in] field_name フィールド名
	 *  @param[in] col        グリッド列番号
	 */
	void add(const CString& field_name, int col) { m_Data.emplace(field_name, col); }

	/**
	 *	@brief 指定したフィールド名に対応したグリッド列番号を返す
	 *  @param[in] field_name フィールド名
	 *  @return 列番号(見つからなかった場合は-1を返す
	 */
	long getCol(const CString& field_name) const;

	/**
	 *	@brief 指定したグリッド列番号に対応したフィールド名を返す
	 *  @param[in] col  グリッド列番号
	 *  @return  フィールド名(見つからなかった場合は空文字を返す)
	 */
	const CString& getFieldName(int col) const;

private:
	std::set<FieldGridCol> m_Data;	//!< フィールド名とグリッドの対応リスト
};

