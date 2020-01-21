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
 * @file index.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_INDEX_H_
#define ARCTL_FIELD_INDEX_H_

#include <boost/preprocessor.hpp>
#include <atl2/type_traits.h>
#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {

namespace field {

/**
 * @defgroup field_index_group 属性フィールドインデックスクラス
 *
 * 属性フィールドインデックスの取得・保持を行うクラス群。
 * 必要な定義・関数を備えることで、 @ref ARCTL_FIELD_INDEX_CLASS_D による多重継承が可能となる。
 */
//@{
/**
 * @brief 属性フィールド名による属性フィールドインデックスクラス。
 *
 * IFields などから属性フィールドインデックスを求め、保持するクラス。
 * 
 * @par テンプレート引数:
 * - @a TFieldName : 属性名称広域変数。
 *
 * @test @ref unit_test FieldIndexTest
 */
template <LPCTSTR TFieldName>
class index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name 構築/消滅
	/// デフォルトコンストラクタで生成されたオブジェクトは、正しいインデックスが代入されるまで無効である。
	index_t() : m_nIndex(-1) {}

	/// IFields::FindField() による構築。
	index_t(IFields* ipFields) { operator=(ipFields); }

	/// IRowBuffer::get_Fields() によって得られた IFields による構築。
	index_t(IRowBuffer* ipRowBuffer) { operator=(ipRowBuffer); }

	/// IClass::get_Fields() によって得られた IFields による構築。
	index_t(IClass* ipClass) { operator=(ipClass); }
//@}

//@{ @name 演算子
	long index() const { return m_nIndex; }
//@}

//@{ @name 演算子
	/**
	 * @brief 属性フィールドインデックスを取得する。
	 *
	 * @remarks
	 * いずれextractorの存在が煩わしく思えてくる時があるかもしれない。<br>
	 * その時のために、なるべくなら index() を使うようにしてほしい。
	 */
	operator long() const { return index(); }

	/**
	 * @brief オブジェクトの無効診断を行う。
	 *
	 * オブジェクトは以下のときに無効となる:
	 * - デフォルトコンストラクタで構築された。
	 * - 該当する属性フィールドが存在しない IFields が渡された。
	 *
	 * 無効になったオブジェクトは、以下のときに再び有効となる:
	 * - 該当する属性フィールドが存在する IFields が渡された。
	 */
	bool operator!() const { return m_nIndex == -1; }

	/**
	 * @brief IFields::FindField() によるインデックス更新。
	 */
	index_t& operator=(IFields* ipFields)
	{
		atl2::valid(ipFields->FindField(CComBSTR(name()), &m_nIndex));
		return *this;
	}

	/**
	 * @brief IRowBuffer::get_Fields() によって得られた IFields によるインデックス更新。
	 */
	index_t& operator=(IRowBuffer* ipRowBuffer)
	{
		IFieldsPtr ipFields;
		atl2::valid(ipRowBuffer->get_Fields(&ipFields));
		return operator=(ipFields);
	}

	/**
	 * @brief IClass::get_Fields() によって得られた IFields によるインデックス更新。
	 */
	index_t& operator=(IClass* ipClass)
	{
		IFieldsPtr ipFields;
		atl2::valid(ipClass->get_Fields(&ipFields));
		return operator=(ipFields);
	}
//@}
	
//@{ @name 静的関数
	/**
	 * @brief 属性フィールド名を取得する。
	 */
	static LPCTSTR name() { return TFieldName; }

	/**
	 * @brief 属性フィールド名をBSTR型で取得する（oid_t、shape_t互換用）。
	 */
	static CComBSTR bname(IClass*) { return CComBSTR(name()); }

	/**
	 * @brief 属性フィールド名を出力する。
	 */
	template <typename TOutputIterator>
	static void copy_name(TOutputIterator result)
	{
		*result = name();
		++result;
	}

	/**
	 * @brief 属性フィールド名を出力する。
	 *
	 * 属性フィールド名称の取得に ITable や IFeatureClass が必要な
	 * arctl::field::oid_t や arctl::field::shape_t との互換を保つために定義された関数。<br>
	 * ユーザが明示的に呼び出す道理はない。
	 */
	template <typename TOutputIterator>
	static void copy_name(IClass*, TOutputIterator result)
	{
		copy_name(result);
	}
//@}

private:
// 変数
	long m_nIndex; ///< 属性フィールドインデックス。
};
//@}

} // namespace field

} // namespace arctl

/**
 * @brief @ref arctl::field::index_t "index_t" を多重継承し、まとめて管理するクラスを定義するマクロ。
 *
 * @param c
 * クラス名。
 * @param seq
 * 属性名称広域変数の<a href="http://boost.cppll.jp/HEAD/libs/preprocessor/doc/data/sequences.html">シーケンス</a>。
 * 各々が @ref arctl::field::index_t "index_t" のテンプレート引数となる。
 *
 * @par 用例:
 * @code
 * using namespace arctl::field;
 *
 * // 属性名称広域変数
 * const TCHAR kValue1[] = _T("Value1");
 * const TCHAR kValue2[] = _T("Value2");
 *
 * // 属性値管理クラス定義
 * ARCTL_FIELD_INDEX_CLASS(index_type, (kValue1) (kValue2));
 * @endcode
 * - <b>クラス名:</b> index_type
 * - <b>属性フィールド名一覧:</b>
 *   - "Value1"
 *   - "Value2"
 * 
 * <h2>定義される関数</h2>
 * 以下は、クラス @a c に定義されるメンバ関数の一覧である。<br>
 * 無効診断以外は全てテンプレート関数であるため、必要が無ければ実体化されない。
 *
 * @code
 * template <typename T>
 * explicit c(T const& rT);
 * @endcode
 * @b operator=() を呼び出す。
 *
 * @code
 * template <typename T>
 * c& operator=(T const& rT);
 * @endcode
 * 代入演算。<br>
 * 全ての@ref field_index_group "属性フィールドインデックスオブジェクト"に対して
 * @b operator=(rT) を行う。
 *
 * @code
 * bool operator!() const;
 * @endcode
 * オブジェトの無効診断。<br>
 * 全ての@ref field_index_group "属性フィールドインデックスオブジェクト"に対して
 * @b operator!() を行い、その戻り値の論理和を返す。
 *
 * @code
 * template <typename TOutputIterator>
 * static void copy_name(TOuptutIterator result);
 * @endcode
 * 属性フィールド名書き出しアルゴリズム。<br>
 * 全ての@ref field_index_group "属性フィールドインデックスオブジェクト"に対して
 * @ref arctl::field::index_t::copy_name(TOutputIterator) "copy_name(result)" を行う。
 *
 * @code
 * template <typename TClass, typename TOutputIterator>
 * static void copy_name(TClass const& rClass, TOutputIterator result);
 * @endcode
 * 属性フィールド名書き出しアルゴリズム。<br>
 * 全ての@ref field_index_group "属性フィールドインデックスオブジェクト"に対して
 * @ref arctl::field::index_t::copy_name(IClass*,TOutputIterator) "copy_name(rClass,result)" を行う。<br>
 *
 * @test @ref unit_test FieldIndexTest
 */
#define ARCTL_FIELD_INDEX_CLASS(c, seq) ARCTL_FIELD_INDEX_CLASS_D(c, BOOST_PP_SEQ_TRANSFORM(ARCTL_FIELD_INDEX_CLASS_EXPAND, _, seq))

/**
 * @brief @ref field_index_groupを多重継承し、まとめて管理するクラスを定義するマクロ。
 */
#define ARCTL_FIELD_INDEX_CLASS_D(c, seq)										\
	class c : BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_INDEX_CLASS_INHERIT, _, seq)	\
	{																			\
	public:																		\
		enum { is_arctl_multi_inherit = true };									\
																				\
		c() {}																	\
		template <typename T> explicit c(T const& rT) { operator=(rT); }		\
	\
		template <typename T>											\
		c& operator=(T const& r)										\
		{																\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_SUB, r, seq)	\
			return *this;												\
		}																\
	\
		bool operator!() const														\
		{																			\
			return BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_INDEX_CLASS_NOT, r, seq);	\
		}																			\
	\
		template <typename TOutputIterator>										\
		static void copy_name(TOutputIterator result)							\
		{																		\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_COPY, result, seq)	\
		}																		\
	\
		template <typename TClass, typename TOutputIterator>					\
		static void copy_name(TClass const& rClass, TOutputIterator result)		\
		{																		\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_COPY2, result, seq)	\
		}																		\
	}

#ifndef DOXYGEN

/// 属性フィールド名を属性フィールドクラスに変換するマクロ。
#define ARCTL_FIELD_INDEX_CLASS_EXPAND(d, data, elem) arctl::field::index_t<elem>

/// 継承実装マクロ。
#define ARCTL_FIELD_INDEX_CLASS_INHERIT(r, data, i, elem) BOOST_PP_COMMA_IF(i) public elem

/// 代入関数実装マクロ。
#define ARCTL_FIELD_INDEX_CLASS_SUB(r, data, elem) static_cast<elem&>(*this) = data;

/// 否定演算アルゴリズム実装マクロ。
#define ARCTL_FIELD_INDEX_CLASS_NOT(r, data, i, elem) BOOST_PP_EXPR_IF(i, ||) ! *static_cast<const elem*>(this)

/// copy_nameアルゴリズム実装マクロ。
#define ARCTL_FIELD_INDEX_CLASS_COPY(r, data, elem) elem::copy_name(data);

/// 二項copy_nameアルゴリズム実装マクロ。
#define ARCTL_FIELD_INDEX_CLASS_COPY2(r, data, elem) elem::copy_name(rClass, data);

#endif // DOXYGEN

#endif // ARCTL_FIELD_INDEX_H_
