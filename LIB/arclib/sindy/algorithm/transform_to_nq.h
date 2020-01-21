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
 * @brief �����N�̕��т��m�[�h�̕��тɕϊ�����A���S���Y���B
 *
 * @note
 * �m�[�hID�͐��̐����łȂ���΂Ȃ�Ȃ��B
 *
 * @note
 * @a result �ɓn��v�f�̐��́A�S�Ẵ����N�𐳂����ϊ��ł����ꍇ�A <b>std::distance(first, last) + 1</b> �ł���B
 *
 * @pre
 * -# <b>arctl::field::get<sindy::road_link::kFromNodeId>(*it)</b>���\�ł���B
 * -# <b>arctl::field::get<sindy::road_link::kToNodeId>(*it)</b>���\�ł���B
 * -# <b>std::distance(first, last) == 1</b> �ł��邩�A�e���v���[�g�̕������ꉻ�ɑΉ����Ă��Ȃ��ꍇ�A<b>arctl::field::get<sindy::link_queue::kLinkDir>(*it)</b>���\�ł���B
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
 * @brief �����N�̕��т��m�[�h�̕��тɕϊ�����A���S���Y���B
 *
 * @note
 * �e���v���[�g�̕������ꉻ�ɑΉ����Ă��Ȃ��R���p�C���p�B
 *
 * @pre
 * -# arctl::field::get<sindy::road_link::kFromNodeId>(*it) ���\�ł���B
 * -# arctl::field::get<sindy::road_link::kToNodeId>(*it) ���\�ł���B
 * -# std::distance(first, last) <> 1 �ł���B
 *
 * @retval true �ϊ������B
 * @retval fales �����N�̕��т��������Ȃ��B
 */
template <typename TInputIterator, typename TOutputIterator>
bool transform_to_nq_with_no_dir(TInputIterator first, TInputIterator last, TOutputIterator result)
{
	return detail::transform_to_nq_t<false>(first, last, result);
}
#endif

} // namespace sindy


#endif // SINDY_ALGORITHM_TRANSFORM_TO_NQ_H_
