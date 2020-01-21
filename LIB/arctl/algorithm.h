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
 * @file arctl/algorithm.h
 * @brief �A���S���Y���n�e���v���[�g���C�u�����B
 */
#ifndef ARCTL_ALGORITHM_H_
#define ARCTL_ALGORITHM_H_

#include <algorithm>
#include <boost/type_traits/add_reference.hpp>

namespace arctl {

/**
 * @defgroup container_algorithm_group �R���e�i�A���S���Y��
 *
 * �n�I�_�����q�̑���ɃR���e�i���̂��̂��󂯎��A�{���̃A���S���Y���ɓn���֐��̃O���[�v�B<br>
 * �K�v�ȃA���S���Y���̒�`�iinclude�Ȃǁj�͌Ăяo�����ōs�����ƁB
 */
/*@{*/

/// arctl::update(TInputIterator, TInputIterator, TUnaryOperation) �ւ̋��n���A���S���Y���B
template <typename TContainer, typename TUnaryOperation>
inline TUnaryOperation update(TContainer const& rContainer, TUnaryOperation op)
{
	using namespace atl2;
	return
		update<iterator_of<TContainer>::type, boost::add_reference<TUnaryOperation>::type>(
			begin(rContainer), end(rContainer), op
		);
}

/// arctl::delete_all(TInputIterator, TInputIterator) �ւ̋��n���A���S���Y���B
template <typename TContainer>
inline void delete_all(TContainer const& rContainer)
{
	using namespace atl2;
	delete_all<iterator_of<TContainer>::type>(begin(rContainer), end(rContainer));
}

/*@}*/

} // namespace arctl

#endif // ARCTL_ALGORITHM_H_
