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

#ifndef ARCTL_SUB_FIELDS_RESET_ITERATOR_H_
#define ARCTL_SUB_FIELDS_RESET_ITERATOR_H_

#include <iterator>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/or.hpp>
#include <atl2/io.h>
#include <atl2/exception.h>

namespace arctl {

/**
 * @brief サブフィールド初期化反復子
 *
 * 渡された要素でサブフィールドを初期化する反復子。<br>
 * 最初の要素を put_SubFields し、その後の要素は AddField する。<br>
 * 反復子が、複製分含めて全て消えた時点で初期化は終了する。
 *
 * 渡せる値は以下の2種。
 * - BSTRのスマートポインタ
 *   - CComBSTR
 *   - _bstr_t
 * - 出力ストリームに出力可能な値
 *
 * 要素が1つも渡されなかった場合、 サブフィールドは変化しない。
 *
 * @note
 * BSTR型の値はLPWSTR型の値と区別が付かないので、念のため CComBSTR を通す。
 * 
 * @note
 * rns::string_concatenate_iterator を使おうと思っていたのだが、書いてみるとあまりメリットがない。
 */
template <typename InterfacePtr>
class sub_fields_reset_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef sub_fields_reset_iterator self;
public:
	class output_proxy
	{
	public:
		explicit output_proxy(const InterfacePtr& filter) :
		m_filter(filter),
		m_first(true)
		{
		}

		template <typename T>
		output_proxy& operator=(const T& rhs)
		{
			if(m_first) {
				atl2::valid(m_filter->put_SubFields(to_bstr(rhs)));
				m_first = false;
			}
			else {
				atl2::valid(m_filter->AddField(to_bstr(rhs)));
			}

			return *this;
		}

	private:
		template <typename T>
		struct is_smart_bstr : 
			public boost::mpl::or_<
				boost::is_same<T, CComBSTR>,
				boost::is_same<T, _bstr_t>
			>
		{};

		/**
		 * @brief BSTRのスマートポインタなら値をそのまま参照する。
		 *
		 * @param[in] BSTRのスマートポインタ。
		 *
		 * @note
		 * 「BSTRに型変換可能な値」という条件も考えたが、その場合LPWSTRも含まれるので断念。
		 */
		template <typename T>
		static typename boost::enable_if<is_smart_bstr<T>, BSTR>::type to_bstr(const T& rhs)
		{
			return rhs;
		}

		template <typename T>
		static typename boost::disable_if<is_smart_bstr<T>, CComBSTR>::type to_bstr(const T& rhs)
		{
			return boost::lexical_cast<CComBSTR>(rhs);
		}

		InterfacePtr m_filter;
		bool m_first;
	};

	explicit sub_fields_reset_iterator(const InterfacePtr& filter) :
	m_proxy(new output_proxy(filter))
	{
	}

/// @name 反復子的関数
//@{
	output_proxy& operator*() { return *m_proxy; }
	self& operator++() { return *this; }
	self& operator++(int) { return *this; }
//@}

private:
	boost::shared_ptr<output_proxy> m_proxy;
};

template <typename InterfacePtr>
inline sub_fields_reset_iterator<InterfacePtr> sub_fields_resetter(const InterfacePtr& ipFilter)
{
	return sub_fields_reset_iterator<InterfacePtr>(ipFilter);
}

} // namespace arctl

#endif // ARCTL_SUB_FIELDS_RESET_ITERATOR_H_
