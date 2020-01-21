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
 * @file iterator_cc.h
 * @brief for iterator.h
 */

#include <boost/call_traits.hpp>

namespace arctl {

namespace detail {

#ifndef ARCTL_ITERATOR_CC_H_
#define ARCTL_ITERATOR_CC_H_

template <typename TIContainer>
class add_iterator_base : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
// 定義
	typedef TIContainer container_type;

//@{ @name 構築/消滅
	add_iterator_base(TIContainer* ipContainer) : m_ipContainer(ipContainer) {}
//@}

//@{ @name 取得
	TIContainer* container() const { return m_ipContainer; }
//@}

//@{ @name 反復子関数の部分実装
	add_iterator_base& operator++() { return *this; }
//@}

private:
// 変数
	TIContainer* m_ipContainer;
};

#endif // ARCTL_ITERATOR_CC_H_

} // namespace detail

#ifndef DOXYGEN
 
/**
 * @brief 単項出力反復子。
 */
template <typename TIContainer, typename TArg>
class ARCTL_ADDER_NAME(add_iterator1) : public detail::add_iterator_base<TIContainer>
{
public:
// 定義
	typedef ARCTL_ADDER_NAME(add_iterator1) own_type;
	typedef TArg value_type;
	typedef HRESULT (ARCTL_ADDER_CC TIContainer::*function_type)(TArg);

// 構築/消滅
	own_type(TIContainer* ipContainer, function_type fp) :
	super(ipContainer),
	m_fp(fp)
	{
	}

// 処理
	void operator=(typename boost::call_traits<value_type>::param_type cElement) { (container()->*m_fp)(cElement); }
	own_type& operator*() { return *this; }
	
private:
// 定義
	typedef detail::add_iterator_base<TIContainer> super;

// 処理
	own_type dereference() const { return *this; }

// 変数
	function_type m_fp;
};

/**
 * @brief 二項出力反復子。
 */
template <typename TIContainer, typename TArg1, typename TArg2>
class ARCTL_ADDER_NAME(add_iterator2) : public detail::add_iterator_base<TIContainer>
{
public:
// 定義
	typedef ARCTL_ADDER_NAME(add_iterator2) own_type;
	typedef std::pair<TArg1, TArg2> value_type;
	typedef HRESULT (ARCTL_ADDER_CC TIContainer::*function_type)(TArg1, TArg2);

// 構築/消滅
	own_type(TIContainer* ipContainer, function_type fp) :
	super(ipContainer),
	m_fp(fp)
	{
	}

// 処理
	void operator=(std::pair<TArg1, TArg2> const& cValue) { (container()->*m_fp)(cValue.first, cValue.second); }
	own_type& operator*() { return *this; }

private:
// 定義
	typedef detail::add_iterator_base<TIContainer> super;

// 変数
	function_type m_fp;
};


/**
 * @fn arctl::adder(TInterfacePtr const& ipInterface, HRESULT (TInterface::*fp)(TArg))
 * @brief 要素への代入にAdd*メンバ関数を呼ぶ反復子を生成する関数。
 *
 * コンセプトは std::back_inserter() 。
 * 
 * @par 用例:
 * @code
 * CComBSTR rgbs[] = { _T("Field1"), _T("Field2") };
 *
 * IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
 * std::copy(rgbs, rgbs + 2, arctl::adder(ipQueryFilter, IQueryFilter::AddField));
 * @endcode
 */
template <typename TInterfacePtr, typename TInterface, typename TArg>
ARCTL_ADDER_NAME(add_iterator1)<TInterface, TArg>
adder(TInterfacePtr const& ipInterface, HRESULT (ARCTL_ADDER_CC TInterface::*fp)(TArg))
{
	return ARCTL_ADDER_NAME(add_iterator1)<TInterface, TArg>(ipInterface, fp);
}

template <typename TInterfacePtr, typename TInterface, typename TArg1, typename TArg2>
ARCTL_ADDER_NAME(add_iterator2)<TInterface, TArg1, TArg2>
adder(TInterfacePtr const& ipInterface, HRESULT (ARCTL_ADDER_CC TInterface::*fp)(TArg1, TArg2))
{
	return ARCTL_ADDER_NAME(add_iterator2)<TInterface, TArg1, TArg2>(ipInterface, fp);
}

#else

/**
 * @defgroup adder_iterator_function 追加反復子生成関数
 */
//@{
/**
 * @brief 要素への代入にAdd*メンバ関数を呼ぶ反復子を生成する関数。
 *
 * コンセプトは std::back_inserter() 。<br>
 * raw_method版しかドキュメント化していないが、high_method版も存在する。
 * 
 * @par 用例:
 * @code
 * CComBSTR rgbs[] = { _T("Field1"), _T("Field2") };
 *
 * IQueryFilterPtr ipQueryFilter(CLSID_QueryFilter);
 * std::copy(rgbs, rgbs + 2, arctl::adder(ipQueryFilter, IQueryFilter::AddField));
 * @endcode
 *
 * @param ipInterface [in] インタフェースポインタ。
 * @param fp [in] Add*メンバ関数。
 * @return 要素への代入にAdd*メンバ関数を呼ぶ反復子。
 */
template <typename TInterfacePtr, typename TInterface, typename TArg>
add_iterator1_stdcall<TInterface, TArg>
adder(TInterfacePtr const& ipInterface, HRESULT (__stdcall TInterface::*fp)(TArg))
{
	return add_iterator1_stdcall<TInterface, TArg>(ipInterface, fp);
}
//@}

#endif // DOXYGEN

} // namespace arctl
