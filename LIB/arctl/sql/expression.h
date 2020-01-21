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
 * @file
 * @brief SQL評価式ライブラリ
 *
 * -# SQL評価式を parse() で解析する
 * -# parse() によって得られた解法と IRowBuffer を evaluate() に渡して評価する
 */
#ifndef ARCTL_SQL_EXPRESSION_H_
#define ARCTL_SQL_EXPRESSION_H_

#include <boost/version.hpp>
#if BOOST_VERSION <= 103400
#include <boost/spirit/core.hpp>
#else
#include <boost/spirit/include/classic_core.hpp>
#endif // BOOST_VERSION <= 103400
#include <sqltl/expression/grammar.h>
#include <sqltl/expression/com/lazy_evaluator.h>
#include <atl2/exception.h>
#include <CodeAnalysis/SourceAnnotations.h>

namespace arctl {

namespace sql {

namespace expression {

namespace detail {

typedef sqltl::expression::com_lazy_evaluator<IRowBuffer*> lazy_evaluator_base;

/**
 * @brief 評価時にフィールド名称からインデックスを割り出して属性値を求める、属性フィールド値評価クラス
 */
struct named_column_factor_evaluator : public lazy_evaluator_base::evaluator_type
{
public:
	/**
	 * @param[in] first 属性フィールド名文字列始点。
	 * @param[in] last 属性フィールド名文字列終点。
	 */
	template <typename InputIterator>
	named_column_factor_evaluator(
		InputIterator first,
		InputIterator last
	) :
	m_bstrColumn(std::basic_string<std::iterator_traits<TCHAR*>::value_type>(first, last).c_str())
	{
	}

	virtual _variant_t eval(argument_type& ipRowBuff) const
	{
		IFieldsPtr ipFields;
		atl2::valid(ipRowBuff->get_Fields(&ipFields));

		long nIndex;
		atl2::valid(ipFields->FindField(m_bstrColumn, &nIndex));
		
		_variant_t aVal;
		atl2::valid(ipRowBuff->get_Value(nIndex, &aVal));

		return aVal;
	}

private:
	const CComBSTR m_bstrColumn; ///< 属性フィールド名称。
};

/**
 * @brief 予め求めておいた属性フィールドインデックスを使って属性値を求める、属性フィールド値評価クラス
 */
struct indexed_factor_evaluator : public lazy_evaluator_base::evaluator_type
{
public:
	/**
	 * @param[in] nIndex 属性フィールドインデックス。
	 */
	indexed_factor_evaluator(
		long nIndex
	) :
	m_nIndex(nIndex)
	{
	}

	virtual _variant_t eval(argument_type& ipRowBuff) const
	{
		_variant_t aVal;
		atl2::valid(ipRowBuff->get_Value(m_nIndex, &aVal));
		return aVal;
	}

private:
	const long m_nIndex; ///< 属性フィールドインデックス。
};

} // namespace detail

/**
 * @brief IRowBuffer 属性フィールド値を遅延評価するオブジェクトを生成するクラス
 *
 * 実際に評価される瞬間になって初めて属性フィールド値を IRowBuffer から取り出す評価オブジェクトを生成する。
 */
class row_lazy_evaluator : public detail::lazy_evaluator_base
{
public:
	/**
	 * @param[in] ipFields 属性フィールド一覧。評価式を解析する段階で属性フィールドインデックスを求めておくのが無理な場合はnullにする。
	 */
	explicit row_lazy_evaluator(
		IFields* ipFields = 0
	) :
	m_ipFields(ipFields)
	{
	}

	template <typename InputIterator>
	term_type column_factor(InputIterator first, InputIterator last) const
	{
		typedef typename std::iterator_traits<InputIterator>::value_type char_type;

		if(m_ipFields != 0) {
			long nField;
			m_ipFields->FindField(CComBSTR(std::basic_string<char_type>(first, last).c_str()), &nField);
			return term_type(new detail::indexed_factor_evaluator(nField));
		}
		else
			return term_type(new detail::named_column_factor_evaluator(first, last));
	}

private:
	IFieldsPtr m_ipFields; ///< 属性フィールド一覧。
};

/**
 * @brief 解法
 */
typedef detail::lazy_evaluator_base::term_type solution;

/**
 * @brief arctl::sql::expression::parse() の戻り値
 *
 * boost::spirit::parse_info 互換変数と arctl::sql::expression::evaluate() 用変数を持つ。
 */
template <typename IteratorT>
struct parse_info
{
	/**
	 * @param rOrginal [in] SQL式の解析情報オリジナル。
	 * @param rActor [in] SQL式の解析に使用した解析時アクション。
	 */
	template <typename Evaluator>
	parse_info(
#if BOOST_VERSION <= 103400
		const boost::spirit::parse_info<IteratorT>& rOrginal,
#else
		const boost::spirit::classic::parse_info<IteratorT>& rOrginal,
#endif
		const sqltl::expression::evaluation_actor<Evaluator>& rActor
	) :
	stop(rOrginal.stop),
	hit(rOrginal.hit),
	full(rOrginal.full),
	length(rOrginal.length)
	{
		if(full) {
			solution = rActor.answer();
		}
	}
/**
 * @name boost::spirit::parse_info 互換変数
 */
//@{
	IteratorT stop;
	bool hit;
	bool full;
	std::size_t length;
//@}

/**
 * @name arctl::sql::expression::evaluate() 用変数
 */
//@{
	solution solution;
//@}
};

/**
 * @brief SQL式を解析する。
 *
 * @param[in] lpszExpression 評価式。
 * @param[in] ipFields インデックスを予め求めておきたい場合に指定する IFields 。
 * @return 解析結果。
 *
 * ipFields を指定した方が evaluate() のコストは低くて済む。
 */
template <typename CharT>
inline parse_info<CharT const*>
parse([SA_Pre(Null=SA_No), SA_Pre(NullTerminated=SA_Yes)] CharT const* lpszExpression, IFields* ipFields = 0)
{
	namespace se = sqltl::expression;

	se::evaluation_actor<row_lazy_evaluator> aActor((row_lazy_evaluator(ipFields)));
	return parse_info<CharT const*>(se::parse(lpszExpression, aActor), aActor);
}

/**
 * @brief RowBuffer オブジェクトを評価する。
 *
 * @param[in] ipRowBuff 評価対象。
 * @param[in] rSolution 評価式を parse() して編み出された解法。
 * @throw std::runtime_error 解法が編み出されていない。
 */
inline bool evaluate([SA_Pre(Null=SA_No)] IRowBuffer* ipRowBuff, const solution& rSolution)
{
	if(! rSolution)
		throw std::runtime_error("illegal parse_info.");

	return rSolution->eval(ipRowBuff);
}

} // namespace expression

} // namespace sql

} // namespace arctl

#endif // ARCTL_SQL_EXPRESSION_H_
