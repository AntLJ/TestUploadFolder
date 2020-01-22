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

#ifndef RNS_ALGORITHM_H_
#define RNS_ALGORITHM_H_

#include <algorithm>
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/concept_check.hpp>

namespace rns {

#if _MSC_VER >= 1400 
namespace detail {

template <typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type
distance_n(
	InputIterator first,
	InputIterator last,
	typename std::iterator_traits<InputIterator>::difference_type n,
	std::input_iterator_tag
)
{
	typename std::iterator_traits<InputIterator>::difference_type i;
	for(i = 0; n > 0 && first != last; i++) {
		n--;
		++first;
	}
	return i;
}

template <typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type
distance_n(
	InputIterator first,
	InputIterator last,
	typename std::iterator_traits<InputIterator>::difference_type n,
	std::random_access_iterator_tag
)
{
	return std::min(std::distance(first, last), n);
}

} // namespace detail

/**
 * @brief �����q�Ԃ̋���������t���ŋ��߂�B
 *
 * <i>std::min(std::distance(first, last), n)</i> ���A�����q�̎�ނɉ����������I�ȕ��@�ōs���B
 *
 * @param first [in] �n�_�����q�B
 * @param last [in] �I�_�����q�B
 * @param n [in] �Œ������B
 * @return std::min(std::distance(first, last), n)
 * @pre n >= 0
 */
template <typename InputIterator>
typename std::iterator_traits<InputIterator>::difference_type
distance_n(InputIterator first, InputIterator last, typename std::iterator_traits<InputIterator>::difference_type n)
{
	boost::function_requires<boost::InputIteratorConcept<InputIterator> >();

	return detail::distance_n(first, last, n, std::iterator_traits<InputIterator>::iterator_category());
}
#endif // _MSC_VER >= 1400 

/**
 * @brief std::equal(it1, last1, first2) ���^�ƂȂ� it1 ���A[first1, last1)�̒�����T���B
 *
 * ����������Ɓu�͈�1�̏I���̕��Ɣ͈�2�̎n�܂�̕��ŁA�v�f�̕��т���v����ӏ���T���v�Ƃ������ƁB
 *
 * @param first1 [in] �͈�1�n�_�B
 * @param last1 [in] �͈�1�I�_�B
 * @param first2 [in] �͈�2�n�_�B
 * @retval !=last1 �����𖞂��������q�B
 * @retval ==last1 �����𖞂��������q�͑��݂��Ȃ��B
 * @pre std::advance(first2, std::distance(first1, last1)) ���\�B
 */
template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 overlap(ForwardIterator1 first1, const ForwardIterator1 last1, const ForwardIterator2 first2)
{
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator1> >();
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator2> >();

	for(; first1 != last1; ++first1) {
		if(std::equal(first1, last1, first2))
			break;
	}

	return first1;
}

/**
 * @brief �l�������͈͂����߂�B
 *
 * @param first1 [in] �͈�1�n�_�B
 * @param last1 [in] �͈�1�I�_�B
 * @param first2 [in] �͈�2�n�_�B
 * @param last2 [in] �͈�2�I�_�B
 * @param pred [in] ��r�֐��B
 * @return �l�������͈́i�͈�1�j�B
 */
template <typename ForwardIterator1, typename InputIterator2, typename Predicate>
std::pair<ForwardIterator1, ForwardIterator1>
intersection_range(
	ForwardIterator1 first1, const ForwardIterator1 last1,
	InputIterator2 first2, const InputIterator2 last2,
	Predicate pred
)
{
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator1> >();
	boost::function_requires<boost::InputIteratorConcept<InputIterator2> >();

	for(; first2 != last2; ++first2) {
		ForwardIterator1 it1(std::find_if(first1, last1, std::bind1st(pred, *first2)));
		if(it1 != last1) {
			const ForwardIterator1 beg1(it1);
			while(++it1, ++first2, (it1 != last1 && first2 != last2)) {
				if(! pred(*it1, *first2))
					break;
			}
			return std::make_pair(beg1, it1);
		}
	}

	return std::make_pair(last1, last1);
}

/**
 * @brief �w�肳�ꂽ�����ȏ�́A�l�������͈͂����߂�B
 *
 * @param first1 [in] �͈�1�n�_�B
 * @param last1 [in] �͈�1�I�_�B
 * @param first2 [in] �͈�2�n�_�B
 * @param last2 [in] �͈�2�I�_�B
 * @param dist [in] �Œ�ۏ͈ؔ͒��B
 * @param pred [in] ��r�֐��B
 * @return �l�������͈́i�͈�1�j�B
 *
 * @post �߂�l.first == �߂�l.second || std::distance(�߂�l) >= @p dist
 *
 * @note @p dist �̌^�� std::iterator_traits<ForwardIterator1>::difference_type �ɂ���ׂ��Ȃ̂����A IntersectionRange.cpp �� VC6+STLPort �ŃR���p�C�����悤�Ƃ��ăG���[�������������߁A�Ƃ肠�������̌^�ɂ���B
 */
template <typename ForwardIterator1, typename ForwardIterator2, typename Predicate>
std::pair<ForwardIterator1, ForwardIterator1>
intersection_range_d(
	ForwardIterator1 first1, const ForwardIterator1 last1,
	ForwardIterator2 first2, const ForwardIterator2 last2,
	unsigned int dist,
	Predicate pred
)
{
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator1> >();
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator2> >();

	std::pair<ForwardIterator1, ForwardIterator1> aPair;

	while(1) {
		aPair = intersection_range(first1, last1, first2, last2, pred);
		unsigned int nDist = std::distance(aPair.first, aPair.second);
		if(nDist == 0 || nDist >= dist)
			break;

		first1 = aPair.second;
	}

	return aPair;
}

/**
 * @brief �w�肳�ꂽ�����ȏ�́A�l�������͈́i���]�j�����߂�B
 *
 * @param first1 [in] �͈�1�n�_�B
 * @param last1 [in] �͈�1�I�_�B
 * @param first2 [in] �͈�2�n�_�B
 * @param last2 [in] �͈�2�I�_�B
 * @param dist [in] �Œ�ۏ͈ؔ͒��B
 * @param pred [in] ��r�֐��B
 * @return �l�������͈́i�͈�1�j�B
 *
 * @post �߂�l.first == �߂�l.second || std::distance(�߂�l) >= @p dist
 */
template <typename ForwardIterator1, typename BidirectionalIterator2, typename Predicate>
std::pair<ForwardIterator1, ForwardIterator1>
reversible_intersection_range_d(
	ForwardIterator1 first1, const ForwardIterator1 last1,
	BidirectionalIterator2 first2, const BidirectionalIterator2 last2,
	unsigned int dist,
	Predicate pred
)
{
	boost::function_requires<boost::ForwardIteratorConcept<ForwardIterator1> >();
	boost::function_requires<boost::BidirectionalIteratorConcept<BidirectionalIterator2> >();

	// �������Ɉ�v�͈͂����߂�B
	std::pair<ForwardIterator1, ForwardIterator1> aPair(intersection_range_d(first1, last1, first2, last2, dist, pred));

	if(aPair.first == aPair.second) {
		// ���߂��Ȃ�������A�t�����Ɉ�v�͈͂����߂�B
		using namespace boost;
		aPair = intersection_range_d(first1, last1, make_reverse_iterator(last2), make_reverse_iterator(first2), dist, pred);
	}

	return aPair;
}

/**
 * @brief ���`���X�g�̎��ȑg�D���������s���B
 *
 * @param lst [in] ���`���X�g�R���e�i�B
 * @param val [in] ��r�v�f�B
 * @retval lst.end() ��r�v�f�ƈ�v����v�f�����݂��Ȃ������B
 * @retval lst.begin() != lst.end() �ł���Ƃ��A��r�v�f�ƈ�v����v�f���w�������q�B
 */
template<typename List, typename Value>
typename List::iterator mru_find(List& lst, Value val)
{
	const typename List::iterator first(lst.begin()), last(lst.end());
	const typename List::iterator it(std::find(first, last, val));
	if(it != first && it != last) {
		lst.splice(lst.begin(), lst, it);
		return lst.begin();
	}
	return it;
}

/**
 * @brief ���`���X�g�̎��ȑg�D���������s���B
 *
 * @param lst [in] ���`���X�g�R���e�i�B
 * @param pred [in] �����֐��B
 * @retval lst.end() �����𖞂����v�f�����݂��Ȃ������B
 * @retval lst.begin() != lst.end() �ł���Ƃ��A�����𖞂����v�f���w�������q�B
 */
template<typename List, typename Predicate>
typename List::iterator mru_find_if(List& lst, Predicate pred)
{
	const typename List::iterator first(lst.begin()), last(lst.end());
	const typename List::iterator it(std::find_if(first, last, pred));
	if(it != first && it != last) {
		lst.splice(lst.begin(), lst, it);
		return lst.begin();
	}
	return it;
}

} // namespace rns

#endif // RNS_ALGORITHM_H_
