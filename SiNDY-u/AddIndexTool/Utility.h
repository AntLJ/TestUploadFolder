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
namespace util
{

// TODO: str_utilに実装する
/**
 * @brief 文字列の大文字小文字を無視して比較
 * @retval true  文字は一致している
 * @retval false 文字は一致していない
 */
inline bool eq( const uh::tstring& s1, const uh::tstring& s2 )
{
	return 0 == _tcsicmp( s1.c_str(), s2.c_str() );
}

/**
 * @brief 対象レイヤのインデックス群を取得
 * @param layer [in] レイヤ
 * @return 対象レイヤのインデックス群
 */
IIndexesPtr getIndexes(ITablePtr layer);

/**
 * @brief 対象レイヤから指定のフィールドを取得
 * @param fieldName [in] フィールド名
 * @param layer     [in] レイヤ
 * @return 取得したフィールド名
 */
IFieldPtr getField(const uh::tstring& fieldName, ITablePtr layer);

} // util
