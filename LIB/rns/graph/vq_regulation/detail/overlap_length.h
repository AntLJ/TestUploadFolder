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

#ifndef RNS_VQ_REGULATION_GRAPH_DETAIL_OVERLAP_LENGTH_H_
#define RNS_VQ_REGULATION_GRAPH_DETAIL_OVERLAP_LENGTH_H_

#include <boost/version.hpp>
#include <boost/range/functions.hpp>
#include <rns/algorithm.h>

namespace rns {

namespace graph {

namespace vq_regulation {

namespace detail {

/**
 * @param r [in] 頂点列規制。
 * @param v [in] 現行頂点。
 * @param first [in] 先行頂点列始点。
 * @param last [in] 先行頂点列終点。
 *
 * @note
 * r は v を含まなくても良い。
 */
template <typename VQRegulation, typename VertexDescriptor, typename ForwardIterator>
#if BOOST_VERSION < 103500
typename std::iterator_traits<typename boost::range_const_reverse_iterator<VQRegulation>::type>::difference_type
#else
typename std::iterator_traits<typename boost::range_reverse_iterator<const VQRegulation>::type>::difference_type
#endif // BOOST_VERSION < 103500
overlap_length(const VQRegulation& r, VertexDescriptor v, ForwardIterator first, ForwardIterator last)
{
	// 頂点列規制の長さが0の場合、重なるわけがない。
	if(boost::empty(r))
		return 0;

	const std::iterator_traits<ForwardIterator>::difference_type n = rns::distance_n(first, last, boost::size(r) - 1) + 1;
#if BOOST_VERSION < 103500
	boost::range_const_reverse_iterator<VQRegulation>::type vi(boost::prior(boost::rend(r), n)), vi_end(boost::rend(r));
#else
	boost::range_reverse_iterator<const VQRegulation>::type vi(boost::prior(boost::rend(r), n)), vi_end(boost::rend(r));
#endif // BOOST_VERSION < 103500
	while(std::find(vi, vi_end, v) != vi_end) {
		if(std::equal(++vi, vi_end, first))
			return std::distance(vi, vi_end) + 1;
	}

	return 0;
}

} // namespace detail

/**
 * @brief 経路探索を続ける上で記憶しておかなければならない頂点数を求める。
 *
 * @param rs [in] 頂点列規制一覧。
 * @param v [in] 現行頂点。
 * @param pi [in] 先行頂点列始点。
 * @param pi_end [in] 先行頂点列終点。
 * @retval 0 指定した頂点への経路探索は頂点列規制によって封じられている。
 * @retval >0 記憶しておかなければならない頂点数。先行頂点数+1。
 *
 * @note
 * 頂点列規制一覧には、 v を含まない頂点列規制が含まれていても構わない。
 */
template <typename VQRegulations, typename VertexDescriptor, typename PredIterator>
unsigned int typical_path_length(
	const VQRegulations& rs,
	VertexDescriptor v,
	PredIterator pi,
	PredIterator pi_end
)
{
	using namespace boost;

	unsigned int longest_regulation_overlap_length = 1;

	for(VQRegulations::const_iterator ri(boost::begin(rs)), ri_end(boost::end(rs)); ri != ri_end; ++ri) {
		typedef VQRegulations::value_type VQRegulationValue;

		const VQRegulationValue& r = *ri;
		const unsigned int regulation_overlap_length =
			static_cast<unsigned int>(detail::overlap_length(r, v, pi, pi_end));

		if(regulation_overlap_length == size(r)) {
			longest_regulation_overlap_length = 0;
			break;
		}

		if(regulation_overlap_length > longest_regulation_overlap_length) {
			longest_regulation_overlap_length = regulation_overlap_length;
		}
	}

	return longest_regulation_overlap_length;
}

} // namespace vq_regulation

} // namespace graph

} // namespace rns

#endif // RNS_VQ_REGULATION_GRAPH_DETAIL_OVERLAP_LENGTH_H_
