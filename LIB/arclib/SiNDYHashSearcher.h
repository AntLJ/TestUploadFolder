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

/**
 * @file SiNDYHashSearcher.h
 * @brief 分割Searchクラスの定義
 * @note このヘッダに定義されているクラスの使用にはライブラリが必要です。
 * @author 地図DB制作部道路制作グループ 菊地福太郎
 * $Id$
 */
#ifndef _SINDY_HASH_SEARCHER_H_
#define _SINDY_HASH_SEARCHER_H_

#include <string>
#include "SiNDYDatasetName.h"
#include "SiNDYObjectIdRange.h"
#include "SiNDYRowSetSearcher.h"

namespace sindy {

#pragma warning(push)
#pragma warning(disable : 4251)
/**
 * @brief オブジェクトIDが連続するオブジェクトを約10万件（デフォルト）ずつSearchするクラス
 *
 * 形状含みのオブジェクトを一度に大量にSearch()すると、サーバに大きな負荷が掛かる。
 */
class SINDYLIB_API HashSearcher
{
public:
	HashSearcher(
		IFeatureWorkspace* ipFeatureWorkspace,
		BSTR bstrTableName,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000);

// 設定/取得
	long first() const { return m_ObjectIdRange.first; }
	long last() const { return m_ObjectIdRange.second; }
	long current() const { return m_nCurrent; }

protected:
// 処理
	IQueryFilter* nextQueryFilter();

private:
// 変数
	IQueryFilterPtr m_ipQueryFilter; ///< クエリフィルタ
#ifdef _UNICODE
	std::wstring m_strWhereClause; ///< Where句（wchar_t）
#else
	std::string m_strWhereClause; ///< Where句（char）
#endif

	ObjectIdRange m_ObjectIdRange; ///< OBJECTID範囲
	int m_nStep; ///< 一度に取り出すオブジェクト数

	long m_nCurrent; ///< 次回Search()の開始オブジェクトID
};
#pragma warning(pop)

template <typename TRow>
class RowHashSearcher_ : public RowSetSearcher_<TRow>, public HashSearcher
{
public:
	RowHashSearcher_(
		TableInterface* ipTable,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000
	) :
	RowSetSearcher_<TRow>(ipTable),
	HashSearcher(FeatureWorkspacePtr(ipTable), CDatasetNameBSTR(ipTable), ipQueryFilter, nStep),
	m_vbRecycling(vbRecycling)
	{
	}

	RowHashSearcher_(
		IFeatureWorkspace* ipFeatureWorkspace,
		BSTR bstrTableName,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0,
		int nStep = 100000
	) :
	RowSetSearcher_<TRow>(getData<row_traits::TableInterfacePtr>(ipFeatureWorkspace, bstrTableName)),
	HashSearcher(ipFeatureWorkspace, bstrTableName, ipQueryFilter, nStep),
	m_vbRecycling(vbRecycling)
	{
	}

// 純粋仮想関数の解決
	virtual HRESULT __stdcall SearchNext(CursorInterface** ppCursor)
	{
		IQueryFilter* ipQueryFilter = nextQueryFilter();
		return ipQueryFilter ? table()->Search(ipQueryFilter, m_vbRecycling, ppCursor) : S_FALSE;
	}

private:
	VARIANT_BOOL m_vbRecycling;
};

typedef RowHashSearcher_<_IRow> RowHashSearcher; ///< IRow仕様の_RowHashSearcherに名前付け
typedef RowHashSearcher_<IFeature> FeatureHashSearcher; ///< IFeature仕様の_RowHashSearcherに名前付け

// traits

template <>
struct pointer_traits<RowHashSearcher*> : public _row_set_searcher_traits<RowSetSearcher> {};

template <>
struct pointer_traits<FeatureHashSearcher*> : public _row_set_searcher_traits<FeatureSetSearcher> {};

/**
 * @addtogroup for_each
 * @brief _RowHashSearcher系専用のfor_each()。
 *
 * @note TFunctionを参照化するにはboost::call_taits.hppが必要。
 */
/*@{*/

// for_each

#include "detail/def_reference_to.h"

template <typename TFunction, typename TRow>
inline TFunction for_each_row(RowHashSearcher_<TRow> cSearcher, TFunction f)
{
	for_each_element<_IRow*, _REFERENCE_TO(TFunction)>(&cSearcher, f);
	return f;
}

#include "detail/undef_reference_to.h"

/*@}*/

} // namespace sindy

#endif // _SINDY_HASH_SEARCHER_H_
