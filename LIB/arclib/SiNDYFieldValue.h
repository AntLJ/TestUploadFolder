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
 * @file SiNDYFieldValue.h
 * @brief 属性値名取得・保持クラス定義
 * @author 地図DB制作部道路制作グループ 菊地福太郎
 * $Id$
 */
#ifndef _SINDY_FIELDVALUE_H_
#define _SINDY_FIELDVALUE_H_

#include "SiNDYFieldIndex.h"
#include "SiNDYValueGetter.h"
#include "SiNDYAlgorithm.h"

namespace sindy {

template <LPCTSTR TFieldName>
struct FieldValueB {};

template<typename T, LPCTSTR TFieldName>
struct inheres_fieldvalue
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile FieldValueB<TFieldName>&); };

	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief 属性値取得・保持クラス
 */
template <LPCTSTR TFieldName, typename TValueType = _variant_t, bool TNullZero = false>
class FieldValue : public FieldValueB<TFieldName>
{
public:
// 定義
	typedef TValueType ValueType;

	/**
	 * @note typdefで定義すると継承時に不具合が起こるので、あえて継承を使用。
	 */
	class FieldIndex : public sindy::FieldIndex<TFieldName>
	{
	public:
		template <typename T>
		FieldIndex(T const& rArg) : sindy::FieldIndex<TFieldName>(rArg)
		{
		}
	};

// 構築
	FieldValue()
	{
	}

	/// FieldValueオブジェクトか、 _IRow* (IFeature*) とFieldIndexを取り出せるオブジェクトによる構築
	template<typename TRow>
	FieldValue(TRow const& rRow) :
	m_Value(Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow))
	{
	}

	/// FieldValueオブジェクトか、 _IRow* (IFeature*) を取り出せるオブジェクトと、FieldIndexオブジェクトによる構築
	template<typename TRow, typename TFieldIndex>
	FieldValue(TRow const& rRow, TFieldIndex const& rFieldIndex) :
	m_Value(Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow, rFieldIndex))
	{
	}

	/// 属性値の直接指定による構築
	FieldValue(const TValueType& rValue) :
	m_Value(rValue)
	{
	}

// 取得
	TValueType& value()
	{
		return m_Value;
	}

	const TValueType& getValue() const
	{
		return m_Value;
	}

// 代入/取得演算
	template<typename TRow>
	FieldValue& operator = (TRow const& rRow)
	{
		m_Value = Construct<inheres_fieldvalue<TRow, TFieldName>::value>()(rRow);
		return *this;
	}

	FieldValue& operator = (const TValueType& rValue)
	{
		m_Value = rValue;
		return *this;
	}

	/// 非可変参照形式で属性値を返す
	operator const TValueType& () const
	{
		return m_Value;
	}

// 静的変数
	static LPCTSTR fieldName()
	{
		return TFieldName;
	}

private:
	/**
	 * @brief 構築補佐クラス
	 *
	 * @par テンプレート引数:
	 * TCopy : コピーコンストラクトか。 
	 */
	template <bool TCopy>
	class Construct
	{
	public:
	// 定義
		/**
		 * @brief 属性取得補佐クラス
		 *
		 * @par テンプレート引数:
		 * T : VT_NULL を0と解釈するか。
		 */
		template <bool T>
		class GetValue
		{
		public:
			/**
			 * @brief TRowからIRowとFieldIndexを一意に取り出せた場合の処理
			 *
			 * @param ipRow [in] 属性値保持者。
			 * @param rFieldIndex [in] 属性フィールドインデックス。
			 * @return 属性値。
			 */
			TValueType operator()(_IRow* ipRow, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				return sindy::getValue(ipRow, static_cast<long>(rFieldIndex));
			}

			/**
			 * @brief TRowからIFeatureとFieldIndexを一意に取り出せた場合の処理
			 *
			 * @param ipFeature [in] 属性値保持者。
			 * @param rFieldIndex [in] 属性フィールドインデックス。
			 * @return 属性値。
			 */
			TValueType operator()(IFeature* ipFeature, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				return sindy::getValue(ipFeature, static_cast<long>(rFieldIndex));
			}
		};

		template <>
		class GetValue<true>
		{
		public:
			TValueType operator()(_IRow* ipRow, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				_variant_t va;
				sindy::getValue(ipRow, static_cast<long>(rFieldIndex), va);

				if(va.vt == VT_NULL) { va = 0L; }

				return va;
			}

			TValueType operator()(IFeature* ipFeature, const sindy::FieldIndex<TFieldName>& rFieldIndex)
			{
				_variant_t va;
				sindy::getValue(ipFeature, static_cast<long>(rFieldIndex), va);

				if(va.vt == VT_NULL) { va = 0L; }

				return va;
			}
		};

	// 処理
		template <typename TRow>
		TValueType operator()(TRow const& rRow)
		{
			return GetValue<TNullZero>()(rRow, rRow);
		}


		template <typename TRow, typename TFieldIndex>
		TValueType operator()(TRow const& rRow, TFieldIndex const& rFieldIndex)
		{
			return GetValue<TNullZero>()(rRow, rFieldIndex);
		}
	};

	template <>
	class Construct<true>
	{
	public:
	// 処理
		template <typename TRow>
		TValueType operator()(TRow const& rRow)
		{
			return get<TFieldName>(rRow);
		}

		template <typename TRow, typename TFieldIndex>
		TValueType operator()(TRow const& rRow, TFieldIndex const&)
		{
			return get<TFieldName>(rRow);
		}
	};

// 変数
	TValueType m_Value; ///< 属性値
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// fieldvalue_cast
//
//////////////////////////////////////////////////////////////////////////////////////////

/**
 * @defgroup fieldvalue_cast FieldValueキャスト関数
 *
 * FieldValue多重継承クラスから、しかるべきFieldValueを抜き出す。<BR>
 * 実質的な処理は何も定義せず、暗黙の型変換に依存する。
 */
/*@{*/

/**
 * @brief 属性フィールド名と型を指定して、該当するFieldValue型へのキャストを試みる関数。
 */
template <LPCTSTR TName, typename TType>
inline const FieldValue<TName, TType>& fieldvalue_cast(const FieldValue<TName, TType>& rFieldValues)
{
	return rFieldValues;
}

/**
 * @brief 属性フィールド名と型、NULLを0と解釈するか否かを指定して、該当するFieldValue型へのキャストを試みる関数。
 */
template <LPCTSTR TName, typename TType, bool TNullZero>
inline const FieldValue<TName, TType, TNullZero>& fieldvalue_cast(const FieldValue<TName, TType, TNullZero>& rFieldValues)
{
	return rFieldValues;
}

/*@}*/

/**
 * @defgroup fieldvalue_mutable_cast FieldValue可変キャスト関数
 *
 * @note FieldValue多重継承クラスから、しかるべきFieldValueを可変形式で抜き出す。
 */
/*@{*/

template <LPCTSTR TName, typename TType>
inline FieldValue<TName, TType>& fieldvalue_mutable_cast(FieldValue<TName, TType>& rFieldValues)
{
	return rFieldValues;
}

/*@}*/

/**
 * @defgroup fieldvalue_get VC6用の、属性取り出し関数群
 *
 * @note VC6のテンプレート解析機構ではこれが精一杯。
 * @note 属性値の型をテンプレート引数にすると、常に型指定が必要になるので却下。
 */
/*@{*/

template <LPCTSTR TFieldName> long get(const FieldValue<TFieldName, long>& rFieldValue) { return rFieldValue; }
template <LPCTSTR TFieldName> double get(const FieldValue<TFieldName, double>& rFieldValue) { return rFieldValue; }
template <LPCTSTR TFieldName> _variant_t get(const FieldValue<TFieldName, _variant_t>& rFieldValue) { return rFieldValue; }

/*@}*/

} // namespace sindy

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELDVALUE_CLASS
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "SiNDYInheritMacro.h"

/**
 * @brief FieldValue多重継承クラス設計支援マクロ
 *
 * このマクロによって生成されたクラスは以下のメンバを持つ。
 * - クラス
 *   - FieldIndex 継承したFieldValueのメンバクラスであるFieldIndexをすべて継承したクラス。
 * - メソッド
 *   - c(); デフォルトコンストラクタ。
 *   - c(TRow const& rRow); 継承した全てのFieldValueにコンストラクト。
 *   - c& operator = (TRow const& rRow); 継承した全てのFieldValueに代入演算。
 *
 * - 静的関数
 * @code
 * template <typename _Functor, typename TRow>
 * static void distribute(TRow const& rRow, _Functor* pFunctor); 
 * @endcode
 * (*pFunctor)(属性1, 属性2, ... 属性n）を適用する。
 *
 * @code
 * templat <typename TFunctor, typename TRow>
 * static void for_each(TRow const& rRow, TFunctor f);
 * @endcode
 * f(FieldValue)を繰り返し適用する。
 * 
 * @param c クラス名。
 * @param n tupleに列挙されている基本クラス数。 
 * @param tuple カンマ区切りの基本クラス一覧を括弧でくくったもの。基本クラスの表記は以下に従う。<br>
 *   <table>
 *	   <tr><td>_(〈基本クラス名〉)</td><td>継承するクラス名を直接指定。</td></tr>
 *	   <tr><td>_1(〈属性フィールド名〉)</td><td>FieldValue<〈属性フィールド名〉>を継承する。</td></tr>
 *	   <tr><td>_2(〈属性フィールド名〉,〈属性型名〉)</td><td>FieldValue<〈属性フィールド名〉,〈属性型名〉>を継承する。</td></tr>
 *   </table>
 */
#define SINDY_FIELDVALUE_CLASS(c, n, tuple) SINDY_FIELDVALUE_CLASS_I(c, n, _LIST_##n tuple)

#define SINDY_FIELDVALUE_CLASS_I(c, n, list) \
class c : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_INHERIT, _COMMA, list) \
{ \
public: \
	SINDY_FIELDINDEX_CLASS_D(FieldIndex, n, (_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FIELDINDEX, _COMMA, list))); \
	\
	c() {} \
\
	template<typename TRow> \
	c(TRow const& rRow) : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_CONSTRUCT, _COMMA, list) {} \
\
	template<typename TRow, typename TFieldIndex> \
	c(TRow const& rRow, TFieldIndex const& rFieldIndex) : _SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_CONSTRUCT2, _COMMA, list) {} \
\
	template<typename TRow> \
	c& operator = (TRow const& rRow) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_COPYOPERATOR, _SEMICOLON, list);\
		return *this; \
	} \
\
	template <typename _Functor, typename TRow> \
	static void distribute(TRow const& rRow, _Functor* pFunctor) \
	{ \
		(*pFunctor)(_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FUNCTOR, _COMMA, list)); \
	} \
\
	template <typename TFunction, typename TRow> \
	static void for_each(TRow const& rRow, TFunction f) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FOR_EACH, _SEMICOLON, list); \
	} \
\
	template <typename _Function> \
	class Distributor \
	{ \
	public: \
		Distributor(_Function cFunction) : \
		m_Function(cFunction) \
		{ \
		} \
\
		template <typename TRow> \
		void operator()(TRow const& rRow) \
		{ \
			m_Function(_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDVALUE_FUNCTOR, _COMMA, list)); \
		} \
	private: \
		_Function m_Function; \
	}; \
\
	template <typename _Function> \
	Distributor<_Function> static make_Distributor(_Function cFunction) \
	{ \
		return Distributor<_Function>(cFunction); \
	} \
};

#define _SINDY_FIELDVALUE_INHERIT(arg) _SINDY_FIELDVALUE_INHERIT_D(arg)
#define _SINDY_FIELDVALUE_INHERIT_D(arg) _SINDY_FIELDVALUE_INHERIT ## arg
#define _SINDY_FIELDVALUE_INHERIT_(arg) public arg
#define _SINDY_FIELDVALUE_INHERIT_1(name) public sindy::FieldValue<name>
#define _SINDY_FIELDVALUE_INHERIT_2(name, type) public sindy::FieldValue<name, type>

#define _SINDY_FIELDVALUE_CONSTRUCT(arg) _SINDY_FIELDVALUE_CONSTRUCT_D(arg)
#define _SINDY_FIELDVALUE_CONSTRUCT_D(arg) _SINDY_FIELDVALUE_CONSTRUCT ## arg
#define _SINDY_FIELDVALUE_CONSTRUCT_(arg) arg(rRow)
#define _SINDY_FIELDVALUE_CONSTRUCT_1(name) sindy::FieldValue<name>(rRow)
#define _SINDY_FIELDVALUE_CONSTRUCT_2(name, type) sindy::FieldValue<name, type>(rRow)

#define _SINDY_FIELDVALUE_CONSTRUCT2(arg) _SINDY_FIELDVALUE_CONSTRUCT2_D(arg)
#define _SINDY_FIELDVALUE_CONSTRUCT2_D(arg) _SINDY_FIELDVALUE_CONSTRUCT2 ## arg
#define _SINDY_FIELDVALUE_CONSTRUCT2_(arg) arg(rRow, rFieldIndex)
#define _SINDY_FIELDVALUE_CONSTRUCT2_1(name) sindy::FieldValue<name>(rRow, rFieldIndex)
#define _SINDY_FIELDVALUE_CONSTRUCT2_2(name, type) sindy::FieldValue<name, type>(rRow, rFieldIndex)

#define _SINDY_FIELDVALUE_COPYOPERATOR(arg) _SINDY_FIELDVALUE_COPYOPERATOR_D(arg)
#define _SINDY_FIELDVALUE_COPYOPERATOR_D(arg) _SINDY_FIELDVALUE_COPYOPERATOR ## arg
#define _SINDY_FIELDVALUE_COPYOPERATOR_(arg) static_cast<arg&>(*this) = rRow
#define _SINDY_FIELDVALUE_COPYOPERATOR_1(name) static_cast<sindy::FieldValue<name>&>(*this) = rRow
#define _SINDY_FIELDVALUE_COPYOPERATOR_2(name, type) static_cast<sindy::FieldValue<name, type>&>(*this) = rRow

#define _SINDY_FIELDVALUE_FIELDINDEX(arg) _SINDY_FIELDVALUE_FIELDINDEX_D(arg)
#define _SINDY_FIELDVALUE_FIELDINDEX_D(arg) _SINDY_FIELDVALUE_FIELDINDEX ## arg
#define _SINDY_FIELDVALUE_FIELDINDEX_(c) c::FieldIndex
#define _SINDY_FIELDVALUE_FIELDINDEX_1(name) sindy::FieldIndex<name>
#define _SINDY_FIELDVALUE_FIELDINDEX_2(name, type) sindy::FieldIndex<name>

#define _SINDY_FIELDVALUE_FUNCTOR(arg) _SINDY_FIELDVALUE_FUNCTOR_D(arg)
#define _SINDY_FIELDVALUE_FUNCTOR_D(arg) _SINDY_FIELDVALUE_FUNCTOR ## arg
#define _SINDY_FIELDVALUE_FUNCTOR_(arg) static_cast<const arg&>(rRow).getValue()
#define _SINDY_FIELDVALUE_FUNCTOR_1(name) sindy::fieldvalue_cast<name, _variant_t>(rRow).getValue()
#define _SINDY_FIELDVALUE_FUNCTOR_2(name, type) sindy::fieldvalue_cast<name, type>(rRow).getValue()

#define _SINDY_FIELDVALUE_FOR_EACH(arg) _SINDY_FIELDVALUE_FOR_EACH_D(arg)
#define _SINDY_FIELDVALUE_FOR_EACH_D(arg) _SINDY_FIELDVALUE_FOR_EACH ## arg
#define _SINDY_FIELDVALUE_FOR_EACH_(arg) f(static_cast<const arg&>(rRow))
#define _SINDY_FIELDVALUE_FOR_EACH_1(name) f(sindy::fieldvalue_cast<name, _variant_t>(rRow))
#define _SINDY_FIELDVALUE_FOR_EACH_2(name, type) f(sindy::fieldvalue_cast<name, type>(rRow))

#endif // _SINDY_FIELDVALUE_H_
