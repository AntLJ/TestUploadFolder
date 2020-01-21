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

#ifndef __SINDY_ALGORITHM_FIND_H__
#define __SINDY_ALGORITHM_FIND_H__

namespace sindy {

template <typename TPredicate, typename TContainer>
element_holder_of<container_traits<TContainer> >::type find_if(TContainer cContainer, TPredicate pred)
{
	typedef container_traits<TContainer> container;

	for(container::iterator first = container::make_first(cContainer); first != last_iterator(); ++first) {
		if(pred(*first))
			return *first;
	}

	return container::ElementHolder();
}

//

template <typename TElement, typename TPredicate, typename TContainer>
TElement find_if_element(TContainer* pContainer, TPredicate pred)
{
	typedef traits<TContainer>::accessor<TElement> accessor;

	for(accessor::iterator first = accessor::make_first(pContainer); first != last_iterator(); ++first) {
		if(pred(*first))
			return *first;
	}

	return 0;
}

template <typename TElement, typename TPredicate, typename TIIID>
TElement find_if_element(const _com_ptr_t<TIIID>& rInterface, TPredicate pred)
{
	typedef TIIID::Interface TContainer;
	typedef traits<TContainer>::accessor<TElement> accessor;
	TContainer* pContainer = rInterface;

	for(accessor::iterator first = accessor::make_first(pContainer); first != last_iterator(); ++first) {
		if(pred(*first))
			return *first;
	}

	return 0;
}

//

template <typename TElement, typename TPredicate, typename TContainer>
TElement find_if_sole_element(TContainer* pContainer, TPredicate pred)
{
	typedef traits<TContainer>::accessor<TElement> accessor;

	accessor::sole_iterator first = accessor::make_sole_first(pContainer);
	for(; first != last_iterator(); ++first) {
		if(pred(*first))
			return *first;
	}

	return 0;
}

template <typename TElement, typename TPredicate, typename TIIID>
TElement find_if_sole_element(const _com_ptr_t<TIIID>& rInterface, TPredicate pred, TElement cNotFound = 0)
{
	typedef TIIID::Interface TContainer;
	typedef traits<TContainer>::accessor<TElement> accessor;
	TContainer* pContainer = rInterface;

	accessor::sole_iterator first = accessor::make_sole_first(pContainer);
	for(; first != last_iterator(); ++first) {
		if(pred(*first))
			return *first;
	}

	return cNotFound;
}

} // namespace sindy

#endif // __SINDY_ALGORITHM_FIND_H__
