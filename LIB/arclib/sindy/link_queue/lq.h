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

#ifndef SINDY_LQ_H_
#define SINDY_LQ_H_

#include <arctl/field.h>
#include <../../LIB/sindy/schema/common.h>

namespace sindy {

namespace detail {

/// @brief リンク列構成要素LQクラス定義。
ARCTL_FIELD_VALUE_CLASS(lq_base_t, (2(link_queue::kLinkId, long)) (2(link_queue::kLinkDir, long)));

} // namespace detail

/**
 * @brief リンク列構成要素LQクラス。
 */
class lq_t : public detail::lq_base_t
{
	typedef detail::lq_base_t super;
public:
	lq_t()
	{
	}

	template <typename Row>
	lq_t(Row const& row) : super(row)
	{
	}

	template <typename Index>
	lq_t(IRowBuffer* row, Index const& index) : super(row, index)
	{
	}

	lq_t(long link_id, long link_dir)
	{
		arctl::field::get<link_queue::kLinkId>(*this) = link_id;
		arctl::field::get<link_queue::kLinkDir>(*this) = link_dir;
	}
};

ARCTL_FIELD_VALUE_CLASS(lq3_t, ((lq_t)) (2(link_queue::kMeshCode, long)));

} // namespace sindy

#endif // SINDY_LQ_H_
