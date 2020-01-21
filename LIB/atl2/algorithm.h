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
 * @file atl2/algorithm.h
 * @brief �A���S���Y���n�e���v���[�g���C�u�����B
 */
#ifndef ATL2_ALGORITHM_H_
#define ATL2_ALGORITHM_H_

#include <algorithm>
#include <boost/type_traits/add_reference.hpp>
#include <atl2/iterator/iterator_base.h>

namespace atl2 {

/**
 * @ingroup container_algorithm_group �R���e�i�A���S���Y��
 *
 * �n�I�_�����q�̑���ɃR���e�i���̂��̂��󂯎��A�{���̃A���S���Y���ɓn���֐��̃O���[�v�B<br>
 * �K�v�ȃA���S���Y���̒�`�iinclude�Ȃǁj�͌Ăяo�����ōs�����ƁB
 */
/*@{*/

/// std::for_each() �ւ̋��n���A���S���Y���B
template <typename TContainer, typename TUnaryOperation>
inline TUnaryOperation for_each(TContainer const& rContainer, TUnaryOperation op)
{
	return std::for_each<iterator_of<TContainer>::type, TUnaryOperation>(begin(rContainer), end(rContainer), op);
}

/// std::find_if() �ւ̋��n���A���S���Y���B
template <typename Container, typename Predicate>
inline typename iterator_of<Container>::type find_if(Container const& rContainer, Predicate pred)
{
	return std::find_if<iterator_of<Container>::type, Predicate>(begin(rContainer), end(rContainer), pred);
}

/// std::copy() �ւ̋��n���A���S���Y���B
template <typename Container, typename OutputIterator>
inline OutputIterator copy(Container const& rContainer, OutputIterator result)
{
	return std::copy<iterator_of<Container>::type, OutputIterator>(begin(rContainer), end(rContainer), result);
}

/// �W���ɂ͑��݂��Ȃ� copy_if �A���S���Y���̎����B
template <typename Container, typename OutputIterator, typename Predicate>
inline OutputIterator copy_if(Container const& rContainer, OutputIterator result, Predicate pred)
{
	for(typename iterator_of<Container>::type it(begin(rContainer)), last(end(rContainer)); it != last; ++it) {
		if(pred(*it)) {
			*result = *it;
			++result;
		}
	}
	return result;
}

/// std::remove_copy_if() �ւ̋��n���A���S���Y���B
template <typename Container, typename OutputIterator, typename Predicate>
inline OutputIterator remove_copy_if(Container const& rContainer, OutputIterator result, Predicate pred)
{
	return std::remove_copy_if<iterator_of<Container>::type, OutputIterator, Predicate>(begin(rContainer), end(rContainer), result, pred);
}

/// std::transform() �ւ̋��n���A���S���Y���B
template <typename Container, typename OutputIterator, typename UnaryOperation>
inline OutputIterator transform(Container const& rContainer, OutputIterator result, UnaryOperation op)
{
	return std::transform<iterator_of<Container>::type, OutputIterator, UnaryOperation>(begin(rContainer), end(rContainer), result, op);
}

/*@}*/

} // namespace atl2

#endif // ATL2_ALGORITHM_H_
