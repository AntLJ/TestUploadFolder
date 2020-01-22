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

#ifndef RNS_ANY_OUTPUT_ITERATOR_H_
#define RNS_ANY_OUTPUT_ITERATOR_H_

#include <memory>
#include <boost/shared_ptr.hpp>

namespace rns {

/**
 * @brief 仮想出力反復子クラス
 *
 * 使用する反復子を実行時に決められる出力反復子。<br>
 * Boost.Function の出力反復子版。のようなもの。
 *
 * @par テンプレート引数:
 * - Value - 反復子を介して出力する要素の型。
 *
 * @note
 * 出力反復子は以下の2種類に分けられる。
 * <ul>
 * <li>参照はがしによって要素の代入ができ、インクリメント演算を行うことで次の要素を指すことができるもの
 * <li>要素を代入すると自動的に次の代入先を指し、インクリメント演算子は何の意味も持たないもの
 * </ul>
 * any_output_iterator は、<b>格納している反復子の種類に関わらず後者として動作する</b>。<br>
 * 後者であれば、仮想呼び出しが「代入演算」だけで済むからだ。（前者は「インクリメント演算」の仮想化も要求する）
 * 
 * @test AnyOutputIteratorTest
 */
template <typename Value>
class any_output_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
	/**
	 * @brief 要素の代入を受け付けるクラス
	 */
	class output_proxy
	{
	public:
		virtual ~output_proxy()
		{
		}

		output_proxy& operator=(const Value& rhs)
		{
			output(rhs);
			return *this;
		}

	protected:
		/**
		 * @brief 要素の代入を行い、反復子に次の要素を指させる。
		 *
		 * @param rhs [in] 代入する要素。
		 */
		virtual void output(const Value& rhs) = 0;
	};

	/**
	 * @brief 反復子を格納する。
	 *
	 * @test VirtualOutputIteratorTest
	 */
	template <typename Iterator>
	any_output_iterator(const Iterator& it) :
	m_pIt(new output_iterator_holder<Iterator>(it))
	{
	}

	/**
	 * @brief 要素の代入を肩代わりする output_proxy を表に出す。
	 *
	 * @test VirtualOutputIteratorTest
	 */
	output_proxy& operator*()
	{
		return *m_pIt;
	}

  	/**
	 * @brief インクリメント演算では何も行わない。
	 *
	 * @test VirtualOutputIteratorTest
	 */
	any_output_iterator& operator++()
	{
		return *this;
	}

	/**
	 * @brief 後置インクリメントでも何も行わない。
	 */
    any_output_iterator& operator++(int)
	{
		return *this;
	}

private:
	/**
	 * @brief コンストラクタで受け取った反復子を格納するクラス
	 */
	template<typename Iterator>
	class output_iterator_holder : public output_proxy
	{
	public:
		output_iterator_holder(const Iterator& it) :
		m_it(it)
		{
		}

	protected:
		virtual void output(const Value& rhs)
		{
			*m_it = rhs;
			++m_it;
		}

	private:
		Iterator m_it;
	};

	boost::shared_ptr<output_proxy> m_pIt;
};

} // namespace rns

#endif // RNS_ANY_OUTPUT_ITERATOR_H_
