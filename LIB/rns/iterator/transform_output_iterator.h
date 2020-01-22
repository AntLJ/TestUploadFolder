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

#ifndef RNS_TRANSFORM_OUTPUT_ITERATOR_H_
#define RNS_TRANSFORM_OUTPUT_ITERATOR_H_

#include <iterator>

namespace rns {

/**
 * @brief 反復子への書き込みの結果を引数として渡された関数の戻り値を、元の反復子に書き込む反復子。
 *
 * @note
 * boost::transform_iterator の出力反復子版。
 */
template <typename UnaryFunction, typename OutputIterator>
class transform_output_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef transform_output_iterator self;
public:
	class output_proxy
	{
	public:
		output_proxy(const OutputIterator& it) :
		m_it(it)
		{
		}

		output_proxy(const OutputIterator& it, UnaryFunction func) :
		m_it(it),
		m_Func(func)
		{
		}

		template <typename Value>
		output_proxy& operator=(const Value& rhs)
		{
			*m_it = m_Func(rhs);
			++m_it;
			return *this;
		}

	private:
		OutputIterator m_it;
		UnaryFunction m_Func;
	};

	transform_output_iterator(const OutputIterator& it, UnaryFunction func) : m_Proxy(it, func) {}
	explicit transform_output_iterator(const OutputIterator& it) : m_Proxy(it) {}

	output_proxy& operator*() { return m_Proxy; }
	self& operator++() { return *this; }
	self& operator++(int) { return *this; }

private:
	output_proxy m_Proxy;
};

template <typename UnaryFunction, typename OutputIterator>
transform_output_iterator<UnaryFunction, OutputIterator>
make_transform_output_iterator(OutputIterator it, UnaryFunction func)
{
	return transform_output_iterator<UnaryFunction, OutputIterator>(it, func);
}

} // namespace rns

#endif // RNS_TRANSFORM_OUTPUT_ITERATOR_H_
