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

#ifndef RNS_SECOND_ITERATOR_H_
#define RNS_SECOND_ITERATOR_H_

#include <boost/iterator/iterator_adaptor.hpp>

namespace rns {

template<class PairIterator>
class second_iterator : 
	public boost::iterator_adaptor<
		second_iterator<PairIterator>, PairIterator, typename boost::iterator_value<PairIterator>::type::second_type
	>
{
	friend class boost::iterator_core_access;

	typedef boost::iterator_adaptor<
		second_iterator<PairIterator>, PairIterator, typename boost::iterator_value<PairIterator>::type::second_type
	> super;
	
public:
	second_iterator() :
	super()
	{
	}

	second_iterator(PairIterator it) :
	super(it)
	{
	}

	second_iterator(const second_iterator& it) :
	super(it.base_reference())
	{
	}
	
private:
	typename super::reference dereference() const
	{
		return base_reference()->second;
	}
};

template <typename PairIterator>
second_iterator<PairIterator> make_second_iterator(PairIterator it)
{
	return second_iterator<PairIterator>(it);
}

} // namespace rns

#endif // RNS_SECOND_ITERATOR_H_
