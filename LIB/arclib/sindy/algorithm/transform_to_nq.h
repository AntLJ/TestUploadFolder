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

#ifndef SINDY_ALGORITHM_TRANSFORM_TO_NQ_H_
#define SINDY_ALGORITHM_TRANSFORM_TO_NQ_H_

#include <arctl/field.h>
//#include <SiNDYAttrCommunity.h>

namespace sindy {

namespace detail {

template <bool THaveLinkDir>
struct transform_to_nq_t
{
	template <typename TInputIterator, typename TOutputIterator>
	void operator()(TInputIterator first, TInputIterator last, TOutputIterator result)
	{
		using namespace road_link;
		using namespace arctl::field;

		long nNodeIds[2][2];

		nNodeIds[0][0] = get<kFromNodeId>(*first);
		nNodeIds[0][1] = get<kToNodeId>(*first);

		bool bFirst = true;
		for(++first != last; ++result) {
			long nNodeIds[1][0] = get<kFromNodeId>(*first);
			long nNodeIds[1][1] = get<kToNodeId>(*first);

			bool bFores[2];
			bool bJoint = false;
			for(int i = 0; ! bJoint && i < 2; i++) {
				for(int j = 0; ! bJoint && j < 2; j++) {
					if(nNodeIds[0][i] == nNodeIds[1][j]) {
						bFores[0] = i != 0;
						bFores[1] = j == 0;
						break;
					}
				}
			}

			if(! bJoint)
				return false;

			if(bFirst) {
				bFirst = false;
				*result = bFores[0] ? nNodeIds[0][0] : nNodeIds[0][1];
				*++result = bFores[0] ? nNodeIds[0][1] : nNodeIds[0][0];
				++result;
			}

			*result = bFores[1] ? nNodeIds[1][1] : nNodeIds[1][0];
			++result;
		}

		return ! bFirst;
	}
};

template <>
struct transform_to_nq_t<true>
{
	ARCTL_FIELD_VALUE_CLASS(
		LNq,
		(2(schema::road_link::kFromNodeID, unsigned long))
		(2(schema::road_link::kToNodeID, unsigned long))
		(2(schema::link_queue::kLinkDir, long))
	);

	template <typename TInputIterator, typename TOutputIterator>
	TOutputIterator operator()(TInputIterator first, TInputIterator last, TOutputIterator result)
	{
		using namespace arctl::field;
		using namespace schema::road_link;
		using namespace link_queue;

		long nPrevId = -1;
		for(; first != last; ++first, ++result) {
			LNq aLNq(*first);

			long nFromNodeId = get<kFromNodeID>(aLNq);
			long nToNodeId = get<kToNodeID>(aLNq);
			long nLinkDir = get<kLinkDir>(aLNq);

			if(nFromNodeId == -1 || nToNodeId == -1)
				return result;

			if(nPrevId == -1) {
				switch(nLinkDir) {
				case link_dir::kFore:
					*result = nPrevId = nFromNodeId;
					break;
				case link_dir::kReverse:
					*result = nPrevId = nToNodeId;
					break;
				default:
					return result;
				}
			}
			switch(nLinkDir) {
			case link_dir::kFore:
				if(nPrevId != nFromNodeId)
					return result;
				*result = nPrevId = nToNodeId;
				break;
			case link_dir::kReverse:
				if(nPrevId != nToNodeId)
					return result;
				*result = nPrevId = nFromNodeId;
				break;
			default:
				return result;
			}
		}

		return result;
	}
};

} // namespace detail

/**
 * @brief リンクの並びをノードの並びに変換するアルゴリズム。
 *
 * @note
 * ノードIDは正の整数でなければならない。
 *
 * @note
 * @a result に渡る要素の数は、全てのリンクを正しく変換できた場合、 <b>std::distance(first, last) + 1</b> である。
 *
 * @pre
 * -# <b>arctl::field::get<sindy::road_link::kFromNodeId>(*it)</b>が可能である。
 * -# <b>arctl::field::get<sindy::road_link::kToNodeId>(*it)</b>が可能である。
 * -# <b>std::distance(first, last) == 1</b> であるか、テンプレートの部分特殊化に対応していない場合、<b>arctl::field::get<sindy::link_queue::kLinkDir>(*it)</b>が可能である。
 */
template <typename TInputIterator, typename TOutputIterator>
TOutputIterator transform_to_nq(TInputIterator first, TInputIterator last, TOutputIterator result)
{
#if(0)
	return detail::transform_to_nq_t<
		boost::is_convertible<
			std::iterator_traits<TInputIterator>::value_type,
			const arctl::field::value_base_t<link_queue::kLinkDir>&
		>::value
	>
#else
	return detail::transform_to_nq_t<true>
#endif
	()(first, last, result);
}

#if(0)
/**
 * @brief リンクの並びをノードの並びに変換するアルゴリズム。
 *
 * @note
 * テンプレートの部分特殊化に対応していないコンパイラ用。
 *
 * @pre
 * -# arctl::field::get<sindy::road_link::kFromNodeId>(*it) が可能である。
 * -# arctl::field::get<sindy::road_link::kToNodeId>(*it) が可能である。
 * -# std::distance(first, last) <> 1 である。
 *
 * @retval true 変換成功。
 * @retval fales リンクの並びが正しくない。
 */
template <typename TInputIterator, typename TOutputIterator>
bool transform_to_nq_with_no_dir(TInputIterator first, TInputIterator last, TOutputIterator result)
{
	return detail::transform_to_nq_t<false>(first, last, result);
}
#endif

} // namespace sindy


#endif // SINDY_ALGORITHM_TRANSFORM_TO_NQ_H_
