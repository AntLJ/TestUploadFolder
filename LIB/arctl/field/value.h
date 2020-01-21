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
 * @file value.h
 * @brief 静的属性フィールド値クラス定義ヘッダ。
 */
#ifndef ARCTL_FIELD_VALUE_H_
#define ARCTL_FIELD_VALUE_H_

#include <functional>
#include <boost/preprocessor.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/operators.hpp>
#include <boost/call_traits.hpp>
#include <arctl/field/index.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {

namespace field {

/**
 * @brief IRowBuffer から属性値を関数クラス
 *
 * @note
 * 関数の実装は、以下のコードとほぼ等価である。
 * @code
 * std::mem_fun(&IRowBuffer::GetValue)
 * @endcode
 */
struct get_value : public std::binary_function<IRowBuffer*, long, _variant_t>
{
	_variant_t operator()(IRowBuffer* ipRowBuffer, long nIndex) const
	{
		VARIANT va;
		::VariantInit(&va);
		atl2::valid(ipRowBuffer->get_Value(nIndex, &va));
		return _variant_t(va, false);
	}
};

/**
 * @brief インデックスが有効値である場合に限り IRowBuffer から属性値を取り出す関数クラス
 *
 * インデックスが無効値（-1）である場合、戻り値は VT_EMPTY となる。
 */
struct get_optional_value : public std::binary_function<IRowBuffer*, long, _variant_t>
{
	_variant_t operator()(IRowBuffer* ipRowBuffer, long nIndex) const
	{
		return nIndex >= 0 ? get_value()(ipRowBuffer, nIndex) : _variant_t();
	}
};

/**
 * @brief 属性値クラスの基底部。
 *
 * @par テンプレート引数:
 * - TFieldName : 属性フィールド名。
 */
template <LPCTSTR FieldName>
struct value_base_t : public arctl::detail::multi_inherit::base_t
{
// 定義
	typedef field::index_t<FieldName> index_type;

//@{ @name 静的関数
	/// 属性フィールド名を取得する。
	static LPCTSTR name() { return FieldName; }
//@}
};

template <LPCTSTR FieldName, typename ValueType>
class value_holder_t : public value_base_t<FieldName>, boost::totally_ordered<value_holder_t<FieldName, ValueType> >
{
public:
// 定義
	typedef ValueType value_type;

/// @name コンストラクタとデストラクタ
//@{
	/**
	 * @brief 属性値はデフォルトコンストラクタで初期化される。
	 *
	 * @note
	 * VC7.0以前のコンパイラでは、POD型のメンバ変数をデフォルト初期化でゼロ初期化してくれないので、少し小細工している。
	 */
	value_holder_t() :
#if _MSC_VER <= 1300
	m_Value(ValueType())
#else
	m_Value()
#endif
	{
	}

	value_holder_t(typename boost::call_traits<ValueType>::param_type cVal) :
	m_Value(cVal)
	{
	}
//@}

/// @name 取得
//@{
	/// 属性値の可変参照を返す。
	ValueType& value() { return m_Value; }

	/// 属性値の不変参照を返す。
	const ValueType& value() const { return m_Value; }
//@}

/// @name 演算子
//@{
	value_holder_t& operator=(typename boost::call_traits<ValueType>::param_type cValue)
	{
		m_Value = cValue;
		return *this;
	}

	template <typename ValueType2>
	bool operator==(const value_holder_t<FieldName, ValueType2>& rhs) const
	{
		return std::equal_to<ValueType>()(value(), rhs.value());
	}

	bool operator==(typename boost::call_traits<ValueType>::param_type rhs) const
	{
		return std::equal_to<ValueType>()(value(), rhs);
	}

	template <typename ValueType2>
	bool operator<(const value_holder_t<FieldName, ValueType2>& rhs) const
	{
		return std::less<ValueType>()(value(), rhs.value());
	}

	bool operator<(typename boost::call_traits<ValueType>::param_type rhs) const
	{
		return std::less<ValueType>()(value(), rhs);
	}
//@}

private:
	value_type m_Value; ///< 属性値。
};

/**
 * @defgroup field_value_group 属性値クラス
 *
 * 属性値の取得・保持・（可能であれば）書き出しを行う、@ref arctl::detail::multi_inherit::base_t継承クラス。<br>
 * 必要な定義・関数を備えることで、@ref ARCTL_FIELD_VALUE_CLASSによる多重継承が可能となる。
 */
//@{

/**
 * @brief value_t の @ref value_t::write() "write()" 未実装版。
 */
template <LPCTSTR FieldName, typename ValueType, typename GetFunction = get_value>
class static_value_t : public value_holder_t<FieldName, ValueType>
{
	typedef value_holder_t<FieldName, ValueType> super;
public:
//@{ @name コンストラクタとデストラクタ
	/**
	 * @brief 属性値はデフォルトコンストラクタで初期化される。
	 */
	static_value_t()
	{
	}

	/**
	 * @brief 様々な型のオブジェクトによる初期化を試みる。
	 *
	 * @p rRowは以下のいずれかであることを期待する:
	 * - 当クラスと同じ value_base_t を継承したオブジェクト。
	 * - <b>IRowBuffer*</b> へ暗黙の型変換が可能なオブジェクト。
	 *   - @ref value_base_t::index_type "index_type" への暗黙の型変換も用意されている場合、処理は最適化される。
	 *
	 * @param rRow [in] オブジェクト。
	 */
	template <typename Row>
	static_value_t(Row const& rRow) :
	super(constructor<boost::is_convertible<Row, const value_base_t<FieldName>&>::value>::construct(rRow, rRow))
	{
	}

	/**
	 * @brief <b>IRowBuffer</b>継承オブジェクトと属性フィールドインデックスから求められる属性値で初期化する。
	 *
	 * @param ipRowBuffer [in] オブジェクト。
	 * @param rIndex [in] @ref value_base_t::index_type "index_type" への暗黙の型変換が可能な属性フィールドインデックス。
	 */
	template <typename Index>
	static_value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :
	super(constructor<false>::construct(ipRowBuffer, rIndex))
	{
	}
//@}

//@{ @name 処理
	/**
	 * @brief @a m_Value と @p ipRowBuffer の属性値を比較する。
	 *
	 * 属性値の同一性確認には <b>std:equal_to<_variant_t>()</b> を用いる。
	 *
	 * @param ipRowBuffer [in] 属性値比較対象オブジェクト。
	 * @param rIndex [in] 属性フィールドインデックス。
	 * @retval true 属性値は等しい。
	 * @retval false 属性値は等しくない。
	 */
	bool equal_value(IRowBuffer* ipRowBuffer, const index_type& rIndex) const
	{
		return std::equal_to<_variant_t>()(GetFunction()(ipRowBuffer, rIndex), value());
	}

	bool equal_value(IRowBuffer* ipRowBuffer) const
	{
		return equal_value(ipRowBuffer, ipRowBuffer);
	}
//@}

//@{ @name 演算子
	/**
	 * @brief 様々な型のオブジェクトによる属性値の入れ替えを試みる。
	 *
	 * 代入演算子であることを除けば value_t(TRow const&) に等しい。
	 *
	 * @param rRow [in] オブジェクト。
	 */
	template <typename Row>
	static_value_t& operator=(Row const& rRow)
	{
		super::operator=(
			constructor<
				boost::is_convertible<Row, const value_base_t<FieldName>&>::value
			>::construct(rRow, rRow)
		);
		return *this;
	}

	/**
	 * @brief 属性値を直接入れ替える。
	 *
	 * @param cValue [in] 新しい属性値。
	 */
	static_value_t& operator=(typename boost::call_traits<value_type>::param_type cValue)
	{
		super::operator=(cValue);
		return *this;
	}
//@}

private:
// 定義
	/// 構築補助クラス。
	template <bool TCopy>
	class constructor
	{
	public:
		template <typename TRow, typename TIndex>
		static value_type construct(TRow const& rRow, TIndex const& rIndex)
		{
			typedef atl2::interface_pointer_of<TIndex>::type rhs_type;

			return
				detail_t<
					boost::is_convertible<TRow, IRowBuffer*>::value, 
					boost::is_convertible<rhs_type, index_type>::value
				>::construct(rRow, static_cast<rhs_type const&>(rIndex));
		}

	private:
		/// 値をそのまま代入。
		template <bool TIsRow, bool THaveIndex>
		struct detail_t
		{
			template <typename TValue, typename T>
			static value_type construct(TValue const& rValue, T const&) { return rValue; }
		};

		/// IRowBuffer* と index_type からの属性取り出し。
		template<>
		struct detail_t<true, true>
		{
			static value_type construct(IRowBuffer* ipRowBuffer, index_type const& rIndex)
			{
				return GetFunction()(ipRowBuffer, rIndex);
			}
		};
	};

	/// 構築補助クラスをコピーコンストラクタ用に特殊化。
	template <>
	class constructor<true>
	{
	public:
		template <typename TRow>
		static value_type construct(TRow const& rRow, TRow const&)
		{
			return get<FieldName>(rRow);
		}
	};
};

/**
 * @brief 静的な属性フィールド名によって特定される@ref field_value_group。
 *
 * ユニークな名前によって属性フィールドを特定し、値の取得・保持・書き出しを行う。<br>
 * 属性フィールド名によってクラスが実体化されるため、属性フィールド名を動的に変えることはできない。
 *
 * @par 属性値取得可能オブジェクト:
 * IRowBuffer
 * @par 属性値の取得:
 * IRowBuffer::get_Value()
 * @par 属性値の書き出し:
 * IRowBuffer::put_Value()
 *
 * @par テンプレート引数:
 * - FieldName : 属性フィールド名。
 * - ValueType : 属性値の型。以下に示す条件を満たすこと。デフォルトは @b _variant_t 。
 *   - デフォルト初期化・複製・代入可能
 *   - _variant_t から暗黙の型変換が可能
 *   - _variant_t へ暗黙の型変換が可能
 *
 * @test @ref unit_test FieldValueTest
 */
template <LPCTSTR FieldName, typename ValueType = _variant_t>
class value_t : public static_value_t<FieldName, ValueType>
{
	typedef static_value_t<FieldName, ValueType> super;
public:
///@name コンストラクタとデストラクタ
//@{
	value_t() {}

	template <typename Row>
	value_t(Row const& rRow) : super(rRow) {}

	template <typename Index>
	value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :super(ipRowBuffer, rIndex) {}
//@}

///@name 処理
//@{
	/**
	 * @brief @p ipRowBufferに属性値を書き出す。
	 *
	 * @param ipRowBuffer [in] 属性値を書き出すオブジェクト。
	 * @param rIndex [in] 属性フィールドインデックス。
	 * @retval true 属性の書き出しに成功した。
	 * @retval false 属性の書き出しに失敗した。
	 */
	bool write(IRowBuffer* ipRowBuffer, const super::index_type& rIndex) const
	{
		return atl2::valid(ipRowBuffer->put_Value(rIndex, static_cast<_variant_t const&>(value()))) == S_OK;
	}

	bool write(IRowBuffer* ipRowBuffer) const
	{
		return write(ipRowBuffer, ipRowBuffer);
	}
//@}
};

template <LPCTSTR FieldName>
class optional_value_t : public static_value_t<FieldName, _variant_t, get_optional_value>
{
	typedef static_value_t<FieldName, _variant_t, get_optional_value> super;
public:
///@name コンストラクタとデストラクタ
//@{
	optional_value_t() {}

	template <typename Row>
	optional_value_t(Row const& rRow) : super(rRow) {}

	template <typename Index>
	optional_value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :super(ipRowBuffer, rIndex) {}
//@}

///@name 処理
//@{
	/**
	 * @brief 値が空値ではなく、インデックスも正常な値だったならば、 @p ipRowBufferに属性値を書き出す。
	 *
	 * @param ipRowBuffer [in] 属性値を書き出すオブジェクト。
	 * @param rIndex [in] 属性フィールドインデックス。
	 * @retval true 属性の書き出しに成功した。
	 * @retval false 属性の書き出しは行われなかった。
	 */
	bool write(IRowBuffer* ipRowBuffer, const super::index_type& rIndex) const
	{
		if(V_VT(&value()) == VT_EMPTY || rIndex < 0)
			return false;

		return atl2::valid(ipRowBuffer->put_Value(rIndex, value())) == S_OK;
	}

	bool write(IRowBuffer* ipRowBuffer) const
	{
		return write(ipRowBuffer, ipRowBuffer);
	}
//@}
};

//@} // field_value_group

/**
 * @ingroup field_value_group
 * @defgroup field_value_get_group 属性オブジェクト／値取得関数
 *
 * @ref ARCTL_FIELD_VALUE_CLASS 定義クラスのオブジェクト、またはオブジェクトに格納されている値を取り出す関数群。
 */
//@{

#define ARCTL_GLOBAL_VALUE_METHOD_DEFINE(type) \
	template <LPCTSTR FieldName> inline const type get(const static_value_t<FieldName, type>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline type& get(static_value_t<FieldName, type>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline const type get(const static_value_t<FieldName, type, get_optional_value>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline type& get(static_value_t<FieldName, type, get_optional_value>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline const value_t<FieldName, type>& value_cast(const value_t<FieldName, type>& rVal) { return rVal; } \
	template <LPCTSTR FieldName> inline value_t<FieldName, type>& value_cast(value_t<FieldName, type>& rVal) { return rVal; }

ARCTL_GLOBAL_VALUE_METHOD_DEFINE(long)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(unsigned long)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(double)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(_variant_t)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(_bstr_t)

#undef ARCTL_GLOBAL_VALUE_METHOD_DEFINE
//@}

template <typename TValue>
struct value_traits
{
	typedef typename TValue::index_type index_type;
};

/**
 * @brief IRowBuffer派生クラス を TValue に変換する関数クラス。
 *
 * ただ変換するだけであれば @b std::identity で良い。<br>
 * あらかじめ求めておいたインデックスオブジェクトを使えることにこのクラスの意味がある。
 *
 * @sa @ref make_interpret_iterator_group
 */
template <typename TValue>
struct interpreter : public std::unary_function<IRowBuffer*, TValue>
{
public:
// 定義
	typedef TValue field_value_type; ///< 属性値型。
	typedef typename value_traits<field_value_type>::index_type index_type; ///< 属性インデックス型。

// コンストラクタとデストラクタ
	/**
	 * @brief あらかじめ求めておいたインデックスを渡す。
	 *
	 * @p pIndex がNULL可なのは、 @b boost::transform_iterator の終端反復子として使えるようにするため。
	 *
	 * @param pIndex [in,nullable] あらかじめ求めておいたインデックスオブジェクトへのポインタ。
	 */
	interpreter(const index_type* pIndex) :
	m_pIndex(pIndex)
	{
	}

///@name 演算
//@{
	field_value_type operator()(IRowBuffer* ipRowBuffer) const
	{
		return m_pIndex ? field_value_type(ipRowBuffer, *m_pIndex) : field_value_type(ipRowBuffer); 
	}

	field_value_type operator()(_IRow* ipRow) const
	{
		return m_pIndex ? field_value_type(ipRow, *m_pIndex) : field_value_type(ipRow); 
	}

	field_value_type operator()(IFeature* ipFeature) const
	{
		return m_pIndex ? field_value_type(ipFeature, *m_pIndex) : field_value_type(ipFeature); 
	}
//@}

private:
// メンバ変数
	const index_type* m_pIndex; ///< インデックスオブジェクト。
};

/**
 * @defgroup make_interpret_iterator_group interpreter 生成支援関数
 */
//@{
template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<interpreter<TValue>, TInputIterator>
	make_interpret_iterator(TInputIterator const& it, typename value_traits<TValue>::index_type const& index)
{
	return make_interpret_iterator<TValue>(it, &index);
}

template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<interpreter<TValue>, TInputIterator>
	make_interpret_iterator(TInputIterator const& it, typename value_traits<TValue>::index_type const* index = 0)
{
	return boost::transform_iterator<interpreter<TValue>, TInputIterator>(it, interpreter<TValue>(index));
}
//@}

template <typename TValue>
struct indirect_t : public std::unary_function<const TValue, typename TValue::value_type>
{
public:
	typedef TValue field_value_type;
	typename field_value_type::value_type operator()(const field_value_type& rFieldValue) const { return rFieldValue.value(); }
};

template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<indirect_t<TValue>, TInputIterator>
	make_indirect_iterator(TInputIterator const& it)
{
	return boost::transform_iterator<detail::indirect_t<TValue>, TInputIterator>(it, indirect_t<TValue>());
}

} // namespace field
} // namespace arctl

/**
 * @brief @ref field_value_groupを多重継承し、まとめて管理するクラスを定義するマクロ。
 *
 * @param c
 * クラス名。
 * @param seq
 * 継承する@ref field_value_groupの<a href="http://boost.cppll.jp/HEAD/libs/preprocessor/doc/data/sequences.html">シーケンス</a>。<br>
 * @ref field_value_groupの指定方法は以下の3種類。
 * -# 属性値クラスを直接指定する
 *   - <b>(属性値クラス)</b>
 * -# 属性名称広域変数を指定する（<b>(@link arctl::field::value_t value_t@<属性名称広域変数@>@endlink)</b>と同義）
 *   - <b>1(属性名称広域変数) ... =</b>
 * -# 属性名称広域変数と型を指定する（<b>(@link arctl::field::value_t value_t@<属性名称広域変数, 型@>@endlink)</b>と同義）
 *   - <b>2(属性名称広域変数, 型)</b>
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
 * ARCTL_FIELD_VALUE_CLASS(field_value_type, ((oid_t)) (1(kValue1)) (2(kValue2, long)));
 * @endcode
 * - <b>クラス名:</b> field_value_type
 * - <b>属性値一覧:</b>
 *   - <b>オブジェクトID</b>
 *     - <b>型:</b> long （@ref arctl::field::oid_t "oid_t" 参照）
 *   - <b>"Value1"</b>
 *     - <b>型:</b> _variant_t
 *   - <b>"Value2"</b>
 *     - <b>型:</b> long
 *
 * <h2>定義される関数</h2>
 * 以下は、クラス @a c に定義されるメンバ関数の一覧である。<br>
 * デフォルトコンストラクタ以外は全てテンプレート関数であるため、必要が無ければ実体化されない。
 *
 * @code
 * c();
 * @endcode
 * 全ての@ref field_value_group "属性値オブジェクト"のデフォルトコンストラクタを呼び出す。
 *
 * @code
 * template <typename T>
 * explicit c(T const& rT)
 * @endcode
 * 全ての@ref field_value_group "属性値オブジェクト"を
 * @a rT で構築する。
 *
 * @code
 * template <typename TIndex>
 * c(IRowBuffer* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(_IRow* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(IFeatureBuffer* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(IFeature* ipRow, TIndex const& rIndex)
 * @endcode
 * 全ての@ref field_value_group "属性値オブジェクト"を
 * @a rRow 、 @a rIndex で構築する。
 *
 * @code
 * c(...)
 * @endcode
 * 多重継承時にのみ有効となる実装。<br>
 * 引数の数は継承したクラスの数と同じ。<br>
 * 渡された引数で@ref field_value_group "属性値オブジェクト"をそれぞれ初期化する。
 *
 * @code
 * template <typename T>
 * c& operator=(T const& rT)
 * @endcode
 * 値代入演算。<br>
 * 全ての@ref field_value_group "属性値オブジェクト"に
 * @b operator=(rT) を行う。
 *
 * @code
 * template <typename T>
 * c& operator==(T const& rT)
 * @endcode
 * 等号演算。<br>
 * 全ての@ref field_value_group "属性値オブジェクト"に対して
 * <b>std::equal_to()(*this, rT)</b> を行う。<br>
 * 必要なときにのみ実装されるようテンプレート関数化しているが、@p T は @b c に暗黙型変換できなければならない。
 *
 * @code
 * template <typename T>
 * c& operator<(T const& rT)
 * @endcode
 * 小なり比較演算。<br>
 * 全ての@ref field_value_group "属性値オブジェクト"を辞書順で比較する。<br>
 * 必要なときにのみ実装されるようテンプレート関数化しているが、@p T は @b c に暗黙型変換できなければならない。
 *
 * @code
 * template <typename TRow>
 * bool write(TRow const& rRow) const
 * @endcode
 * 値の書き出し。<br>
 * 全ての@ref field_value_group "属性値オブジェクト"に
 * @ref arctl::field::value_t::write() "write(rRow, rRow)" を行う。<br>
 * 戻り値は論理和で返される。
 *
 * @code
 * template <typename TRow, typename TIndex>
 * bool write(TRow const& rRow, TIndex const& rIndex) const
 * @endcode
 * 値の書き出し。<br>
 * 全ての@ref field_value_group "属性値オブジェクト"に
 * @ref arctl::field::value_t::write() "write(rRow, rIndex)" を行う。<br>
 * 戻り値は論理和で返される。
 *
 * @code
 * template <typename Function>
 * Function for_each(Function f) const
 * @endcode
 * 全ての@ref field_value_group "属性値オブジェクト"に関数 @a f() を適用する。
 *
 * @test @ref unit_test FieldValueTest
 */
#define ARCTL_FIELD_VALUE_CLASS(c, seq)											\
	class c : BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_INHERIT, _, seq), boost::totally_ordered<c> \
	{																			\
		friend arctl::detail::multi_inherit::function_selector<true>;			\
	public:																		\
		ARCTL_FIELD_INDEX_CLASS_D(index_type, BOOST_PP_SEQ_TRANSFORM(ARCTL_FIELD_VALUE_CLASS_INDEX, _, seq));	\
		enum { is_arctl_multi_inherit = true }; \
	\
		c() {}	\
	\
		template <typename T> explicit c(T const& rT) :									\
		BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN1, rT, seq) {}	\
	\
		BOOST_PP_REPEAT_FROM_TO(0, BOOST_PP_ARRAY_SIZE(ARCTL_ROW_FAMILY), ARCTL_FIELD_VALUE_CLASS_CONSTRUCTS2, (c, seq))	\
	\
		BOOST_PP_EXPR_IF(BOOST_PP_GREATER(BOOST_PP_SEQ_SIZE(seq), 1), ARCTL_FIELD_VALUE_CLASS_CONSTRUCT(c, seq))	\
	\
		template <typename T>											\
		c& operator=(T const& rT)										\
		{																\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_SUB, rT, seq)	\
			return *this;												\
		}																\
	\
		template <typename T>																				\
		bool operator==(T const& rT) const																	\
		{																									\
			return BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_EQUAL, static_cast<const c&>(rT), seq);	\
		}																									\
	\
		template <typename T>																	\
		bool operator<(T const& rT) const														\
		{																						\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_LESS, static_cast<const c&>(rT), seq)	\
			return false;																		\
		}																						\
	\
		template <typename TRow>			\
		bool write(TRow const& rRow) const	\
		{									\
			return write(rRow, rRow);		\
		}									\
	\
		template <typename TRow, typename TIndex>							\
		bool write(TRow const& rRow, TIndex const& rIndex) const			\
		{																	\
			bool bResult = true;											\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_WRITE, _, seq);	\
			return bResult;													\
		}																	\
	\
		template <typename TFunction>										\
		TFunction for_each(TFunction f) const								\
		{																	\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_FOR_EACH, _, seq)	\
			return f;														\
		}																	\
	\
		template <typename TFunction>														\
		void distribute_value(TFunction f) const											\
		{																					\
			f(BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_DISTRIBUTE_VALUE, _, seq));	\
		}																					\
	\
	private:																\
		template <typename TFunction>										\
		void for_each_ref(TFunction& f) const								\
		{																	\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_FOR_EACH, _, seq)	\
		}																	\
	}

#ifndef DOXYGEN

#define ARCTL_ROW_FAMILY (4, (_IRow, IRowBuffer, IFeatureBuffer, IFeature))

/// 属性値クラス名展開マクロ。
#define ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_(elem) elem
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_1(name) arctl::field::value_t<name>
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_2(name, type) arctl::field::value_t<name, type>
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_O1(name) arctl::field::optional_value_t<name>

/// 属性型展開マクロ。
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_(elem) elem::value_type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_1(name) arctl::field::value_t<name>::value_type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_2(name, type) type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_O1(name, type) arctl::field::optional_value_t<name>::value_type

/// 属性フィールドインデックスクラス名展開マクロ。
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_(elem) arctl::field::value_traits< elem >::index_type
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_1(name) arctl::field::index_t<name>
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_2(name, type) arctl::field::index_t<name>
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_O1(name) arctl::field::index_t<name>

/// 継承実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_INHERIT(r, data, i, elem) BOOST_PP_COMMA_IF(i) public ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)

/// 値直接代入方コンストラクタ定義マクロ。
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT(c, seq)								\
	c(BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_VALUE_ARGS, x, seq)) :	\
	BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN, x, seq) {}

// 二項コンストラクタ実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCTS2(z, i, data)															\
	template <typename TIndex>																					\
	BOOST_PP_TUPLE_ELEM(2, 0, data)(BOOST_PP_ARRAY_ELEM(i, ARCTL_ROW_FAMILY)* ipRow, TIndex const& rIndex) :	\
	BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN2, ipRow, BOOST_PP_TUPLE_ELEM(2, 1, data)) {}

/// 値直接代入方コンストラクタ仮引数群展開マクロ。
#define ARCTL_FIELD_VALUE_CLASS_VALUE_ARGS(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) const ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND(elem)& data ## i

/// 属性フィールドインデックス定義マクロ。
#define ARCTL_FIELD_VALUE_CLASS_INDEX(d, data, elem) ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem)

#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data ## i)

/// コンストラクタチェイン実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN1(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data)

/// 二項コンストラクタチェイン実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN2(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data, rIndex)

/// 等号演算実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_EQUAL(r, data, i, elem) \
	BOOST_PP_EXPR_IF(i, &&) std::equal_to<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) >()(*this, data)

/// 小なり演算実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_LESS(r, data, elem)			\
	{															\
		std::less<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) > less_;	\
		if(less_(*this, data)) return true;						\
		else if(less_(data, *this)) return false;				\
	}

/// 代入関数実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_SUB(r, data, elem) static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)&>(*this) = data;

/// value_t::write() 連続実行関数実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_WRITE(r, data, elem) \
	bResult &= static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this)->write(rRow, ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem)(rIndex));

/// for_each実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_FOR_EACH(r, data, elem) \
	arctl::detail::multi_inherit::for_each_ref(*static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this), f);

/// 値分配関数実装マクロ。
#define ARCTL_FIELD_VALUE_CLASS_DISTRIBUTE_VALUE(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this)->value()

#endif // DOXYGEN

#endif // ARCTL_FIELD_VALUE_H_
