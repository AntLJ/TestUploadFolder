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
 * @brief 反復子間の距離を上限付きで求める。
 *
 * <i>std::min(std::distance(first, last), n)</i> を、反復子の種類に応じた効率的な方法で行う。
 *
 * @param first [in] 始点反復子。
 * @param last [in] 終点反復子。
 * @param n [in] 最長距離。
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
 * @brief std::equal(it1, last1, first2) が真となる it1 を、[first1, last1)の中から探す。
 *
 * 言い換えると「範囲1の終わりの方と範囲2の始まりの方で、要素の並びが一致する箇所を探す」ということ。
 *
 * @param first1 [in] 範囲1始点。
 * @param last1 [in] 範囲1終点。
 * @param first2 [in] 範囲2始点。
 * @retval !=last1 条件を満たす反復子。
 * @retval ==last1 条件を満たす反復子は存在しない。
 * @pre std::advance(first2, std::distance(first1, last1)) が可能。
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
 * @brief 値が同じ範囲を求める。
 *
 * @param first1 [in] 範囲1始点。
 * @param last1 [in] 範囲1終点。
 * @param first2 [in] 範囲2始点。
 * @param last2 [in] 範囲2終点。
 * @param pred [in] 比較関数。
 * @return 値が同じ範囲（範囲1）。
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
 * @brief 指定された長さ以上の、値が同じ範囲を求める。
 *
 * @param first1 [in] 範囲1始点。
 * @param last1 [in] 範囲1終点。
 * @param first2 [in] 範囲2始点。
 * @param last2 [in] 範囲2終点。
 * @param dist [in] 最低保証範囲長。
 * @param pred [in] 比較関数。
 * @return 値が同じ範囲（範囲1）。
 *
 * @post 戻り値.first == 戻り値.second || std::distance(戻り値) >= @p dist
 *
 * @note @p dist の型は std::iterator_traits<ForwardIterator1>::difference_type にするべきなのだが、 IntersectionRange.cpp を VC6+STLPort でコンパイルしようとしてエラーが発生したため、とりあえずこの型にする。
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
 * @brief 指定された長さ以上の、値が同じ範囲（反転可）を求める。
 *
 * @param first1 [in] 範囲1始点。
 * @param last1 [in] 範囲1終点。
 * @param first2 [in] 範囲2始点。
 * @param last2 [in] 範囲2終点。
 * @param dist [in] 最低保証範囲長。
 * @param pred [in] 比較関数。
 * @return 値が同じ範囲（範囲1）。
 *
 * @post 戻り値.first == 戻り値.second || std::distance(戻り値) >= @p dist
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

	// 順方向に一致範囲を求める。
	std::pair<ForwardIterator1, ForwardIterator1> aPair(intersection_range_d(first1, last1, first2, last2, dist, pred));

	if(aPair.first == aPair.second) {
		// 求められなかったら、逆方向に一致範囲を求める。
		using namespace boost;
		aPair = intersection_range_d(first1, last1, make_reverse_iterator(last2), make_reverse_iterator(first2), dist, pred);
	}

	return aPair;
}

/**
 * @brief 線形リストの自己組織化検索を行う。
 *
 * @param lst [in] 線形リストコンテナ。
 * @param val [in] 比較要素。
 * @retval lst.end() 比較要素と一致する要素が存在しなかった。
 * @retval lst.begin() != lst.end() であるとき、比較要素と一致する要素を指す反復子。
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
 * @brief 線形リストの自己組織化検索を行う。
 *
 * @param lst [in] 線形リストコンテナ。
 * @param pred [in] 条件関数。
 * @retval lst.end() 条件を満たす要素が存在しなかった。
 * @retval lst.begin() != lst.end() であるとき、条件を満たす要素を指す反復子。
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
