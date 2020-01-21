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

#ifndef __SINDY_INCLAUSE_STREAM_H__
#define __SINDY_INCLAUSE_STREAM_H__

#include <functional>
#include <string>
#include <sstream>
#include "SiNDYTraits.h"

namespace sindy {

/**
 * @brief IN演算ストリーム
 *
 * IN演算式に要素を溜め込んでいき、一定数（デフォルト1000）に達したら演算式を適用するクラス。
 *
 * - TFunction : std::unary_function<LPCTSTR, void>
 *
 * @note 純粋仮想クラスSynchronizerへの参照を第１引数とした方がスマートかもしれない。
 * @note Synchronizer継承クラスを関数内で定義して使えるし。
 */
template <typename TFunction>
class InClauseStream
{
public:
// 構築/消滅
	/**
	 * @param cFunction [in] IN演算式を単項引数とするファンクタ。
	 * @param lpszFieldName [in] 属性フィールド名。
	 * @param nMaxCount [in] IN演算式最大要素数。
	 */
	InClauseStream(TFunction cFunction, LPCTSTR lpszFieldName, unsigned int nMaxCount = 1000) :
	m_Function(cFunction),
	m_strFieldName(lpszFieldName),
	m_nMaxCount(nMaxCount),
	m_nCount(0)
	{
	}

	InClauseStream(const InClauseStream& rStream) :
	m_Function(rStream.m_Function),
	m_strFieldName(rStream.m_strFieldName),
	m_nMaxCount(rStream.m_nMaxCount),
	m_nCount(0)
	{
	}

	~InClauseStream()
	{
		flush();
	}

// 処理
	/**
	 * @brief sync()し、IN演算式を空にする
	 *
	 * 要素数が0の場合は何もしない。
	 */
	void flush()
	{
		// 要素数が0の場合は何もしない
		if(m_nCount == 0)
			return;

		// IN演算式を閉じる
		m_WhereClauseStream << _T(')');

		// IN演算式を適用する
		sync(m_WhereClauseStream.str().c_str());

		// IN演算式を空にする
		m_WhereClauseStream.str(_T(""));
		m_nCount = 0;
	}

// 演算子のオーバーライド
	/**
	 * @brief IN演算式に要素を追加する。
	 *
	 * 要素数が最大要素数に達したらflush()。
	 *
	 * @param rT [in] 追加する要素。
	 */
	template <typename T>
	InClauseStream& operator<<(T const& rT)
	{
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		m_WhereClauseStream.imbue(loc);
		if(m_nCount) {
			m_WhereClauseStream << _T(',');
		}
		else {
			// IN演算式を新たに構築する
			m_WhereClauseStream << m_strFieldName.c_str() << _T(" IN (");
		}

		// IN演算式に要素を追加する
		m_WhereClauseStream << rT;

		// 要素数が最大要素数に達したらflush()
		if(++m_nCount == m_nMaxCount) {
			flush();
		}

		return *this;
	}

	/**
	 * @brief IN演算式に要素を追加する。
	 */
	template <typename T>
	void operator()(T const& rT)
	{
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		operator.imbue(loc);
		operator<<(rT);
	}

private:
	/**
	 * @brief IN演算式を適用する
	 *
	 * @param lpszWhereClause [in] IN演算式。
	 */
	void sync(LPCTSTR lpszWhereClause)
	{
		m_Function(lpszWhereClause);
	}

// 変数
	TFunction m_Function; ///< IN演算式適用ファンクタ
	unsigned int m_nMaxCount; ///< IN演算式最大要素数
	unsigned int m_nCount; ///< IN演算式現在要素数
#ifdef _UNICODE
	std::wstring m_strFieldName; ///< 属性フィールド名
	std::wostringstream m_WhereClauseStream; ///< IN演算式
#else
	std::string m_strFieldName; ///< 属性フィールド名
	std::ostringstream m_WhereClauseStream; ///< IN演算式
#endif // _UNICODE
};

/**
 * @brief Where句毎にサーチするクラス。
 *
 * - TFunction : TRowを引数とする単項関数オブジェクト。あるいはその参照。
 * - TRow : TFunctionの引数型（IRowかIFeature）。
 *
 * @note TFunctionは、関数オブジェクトへの参照でも、関数オブジェクト自身でも良い。
 * @note 後者の場合、先んじてboost::call_traitsをインクルードしておかないと、for_eachの度にコピーコンストラクタが働く。
 */
template <typename TFunction, typename TRow>
class SearchByWhereClause
{
public:
	/**
	 * @brief サーチするための情報を登録する。
	 *
	 * @param cFunction [in] サーチ結果の処理を行う単項関数オブジェクト。コンストラクタに渡す時点では未初期化でも構わない。
	 * @param ipTable [in,ref] サーチ対象テーブル。
	 * @param vbRecycling [in] IRowの再利用を行うか？
	 * @param ipQueryFilter [in,optional] サーチに用いるクエリフィルタ。ただし、Where句は無視される。
	 */
	SearchByWhereClause(
		TFunction cFunction,
		traits<TRow>::TableInterface* ipTable,
		VARIANT_BOOL vbRecycling,
		IQueryFilter* ipQueryFilter = 0) :
	m_Function(cFunction),
	m_ipTable(ipTable),
	m_vbRecycling(vbRecycling),
	m_ipQueryFilter(ipQueryFilter)
	{
		if(m_ipTable) {
			m_ipTable->AddRef();
		}
		if(m_ipQueryFilter == 0) {
			m_ipQueryFilter.CreateInstance(CLSID_QueryFilter);
		}
	}

	SearchByWhereClause(const SearchByWhereClause& rClause) :
	m_Function(rClause.m_Function),
	m_ipTable(rClause.m_ipTable),
	m_vbRecycling(rClause.m_vbRecycling),
	m_ipQueryFilter(rClause.m_ipQueryFilter)
	{
		if(m_ipTable) {
			m_ipTable->AddRef();
		}
	}

	~SearchByWhereClause()
	{
		if(m_ipTable) {
			m_ipTable->Release();
		}
	}

	void operator()(LPCTSTR lpszWhereClause)
	{
		operator()(CComBSTR(lpszWhereClause));
	}

	void operator()(BSTR bstrWhereClause)
	{
		// クエリフィルタに新しいWhere句を登録
		m_ipQueryFilter->put_WhereClause(bstrWhereClause);

		// サーチ
		traits<TRow>::CursorInterfacePtr ipCursor;
		m_ipTable->Search(m_ipQueryFilter, m_vbRecycling, &ipCursor);

		// 処理
		m_Function(static_cast<traits<TRow>::CursorInterface*>(ipCursor));
	}

protected:
	IQueryFilterPtr m_ipQueryFilter; ///< サーチに用いるクエリフィルタ

private:
	TFunction m_Function; ///< 単項関数オブジェクト
	traits<TRow>::TableInterfacePtr m_ipTable; ///< 対象テーブル
	VARIANT_BOOL m_vbRecycling;
};

//////////////////////////////////////////////////////////////////////
// ヘルパ関数
//////////////////////////////////////////////////////////////////////

/**
 * @brief IN演算ストリーム構築ヘルパ関数。
 *
 * @param cFunction [in] IN演算式を単項引数とするファンクタ。
 * @param lpszFieldName [in] 属性フィールド名。
 * @param nMaxCount [in] IN演算式最大要素数。
 */
template <typename TFunction>
inline InClauseStream<TFunction> make_InClauseStream(TFunction cFunction, LPCTSTR lpszFieldName, unsigned int nMaxCount = 1000)
{
	return InClauseStream<TFunction>(cFunction, lpszFieldName, nMaxCount);
}

} // namespace sindy

#endif // __SINDY_VALUESET_WHERECLAUSE_STREAM_H__
