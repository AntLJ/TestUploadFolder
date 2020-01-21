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
 * @file modifier.h
 * @brief 属性値不一致時属性更新クラス定義ヘッダ。
 */
#ifndef ARCTL_FIELD_VALUE_MODIFIER_H_
#define ARCTL_FIELD_VALUE_MODIFIER_H_

#include <arctl/field/value.h>

namespace arctl {

namespace field {

/**
 * @brief 属性値不一致時属性更新クラス。
 *
 * <b>IRowBuffer</b>継承オブジェクトを引数とする単項演算クラス。<br>
 * 値が異なる場合のみ属性値を更新し、 @b true を返す。<br>
 * 属性値の同一性比較には@ref arctl::field::value_t::equal_value() "equal_value()"を、
 * 値の更新には@ref arctl::field::value_t::write() "write()"を使用する。
 */
template <typename Value>
struct modifier : public std::unary_function<IRowBuffer*, bool>
{
public:
// 定義
	typedef Value field_value_type;
	typedef typename value_traits<field_value_type>::index_type index_type;

//@{ @name コンストラクタとデストラクタ
	modifier(const field_value_type& rValue, const index_type& rIndex) :
	m_rValue(rValue),
	m_rIndex(rIndex)
	{
	}
//@}

//@{ @name 演算
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		func<IRowBuffer> aFunc(ipRowBuffer, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}

	bool operator()(IFeatureBuffer* ipFeatureBuffer) const
	{
		func<IFeatureBuffer> aFunc(ipFeatureBuffer, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}

	bool operator()(IFeature* ipFeature) const
	{
		func<IFeature> aFunc(ipFeature, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}
//@}

private:
// 定義
	template <typename Interface>
	class func
	{
	public:
	//@{ @name コンストラクタとデストラクタ
		func(Interface* pInterface, const index_type& rIndex) :
		m_pInterface(pInterface),
		m_rIndex(rIndex),
		m_bResult(false)
		{
		}
	//@}

	//@{ @name 取得
		bool result() const { return m_bResult; }
	//@}

	//@{ @name 演算
		template <typename ValueHolder>
		void operator()(const ValueHolder& rValueHolder)
		{
			if(! rValueHolder.equal_value(m_pInterface, m_rIndex)) {
				m_bResult |= rValueHolder.write(m_pInterface, m_rIndex);
			}
		}
	//@}

	private:
	// 変数
		Interface* m_pInterface;
		const index_type& m_rIndex;
		bool m_bResult;
	};

// 変数
	const field_value_type& m_rValue;
	const index_type& m_rIndex;
};

/**
 * @brief modifier生成支援関数。
 *
 * @param rValue [in] @ref field_value_groupか@ref ARCTL_FIELD_VALUE_CLASS。
 * @param rIndex [in] @p rValueのインデックス。
 */
template <typename TValue>
inline modifier<TValue> make_modifier(const TValue& rValue, const typename TValue::index_type& rIndex)
{
	return modifier<TValue>(rValue, rIndex);
}

} // namespace field

} // namespace arctl

#endif // ARCTL_FIELD_VALUE_MODIFIER_H_
