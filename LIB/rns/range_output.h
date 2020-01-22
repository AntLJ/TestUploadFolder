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

#ifndef RNS_RANGE_OUTPUT_H_
#define RNS_RANGE_OUTPUT_H_

#include <iostream>

namespace rns {

template <typename FowardIterator>
class range_output
{
public:
	typedef FowardIterator iterator;

	range_output(iterator first, iterator last, const char* lpszDelim = 0, const char* lpszTerm = 0) :
	m_itFirst(first),
	m_itLast(last),
	m_lpszDelim(lpszDelim),
	m_lpszTerm(lpszTerm ? lpszTerm : m_lpszDelim)
	{
	}

	void output(std::ostream& os) const
	{
		if(m_itFirst != m_itLast) {
			iterator first = m_itFirst;

			os << *first;

			while(++first != m_itLast) {
				if(m_lpszDelim) {
					os << m_lpszDelim;
				}
				os << *first;
			}

			if(m_lpszTerm) {
				os << m_lpszTerm;
			}
		}
	}

private:
	const iterator m_itFirst;
	const iterator m_itLast;
	const char* m_lpszDelim;
	const char* m_lpszTerm;
};

template <typename FowardIterator>
range_output<FowardIterator>
make_range_output(FowardIterator first, FowardIterator last, const char* lpszDelim = 0, const char* lpszTerm = 0)
{
	return range_output<FowardIterator>(first, last, lpszDelim, lpszTerm);
}

} // namespace rns

template <typename It>
inline std::ostream& operator<<(std::ostream& os, const rns::range_output<It>& rhs)
{
	rhs.output(os);
	return os;
}

#endif // RNS_RANGE_OUTPUT_H_
