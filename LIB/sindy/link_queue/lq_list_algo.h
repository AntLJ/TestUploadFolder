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

#ifndef SINDY_LQ_LIST_ALGO_H_
#define SINDY_LQ_LIST_ALGO_H_

#include <rns/functional.h>
#include <arctl/field/value.h>
#include <arctl/iterator/add_iterator.h>
#include <sindy/schema/common.h>
#include <sindy/link_queue/lq_list.h>

namespace sindy {

namespace lq_list_algo_detail {

template <
	typename T,
	bool HasInfIDField = boost::is_convertible<T, arctl::field::value_base_t<sindy::schema::link_queue::kInfID> >::value
>
struct add_inf_id_field
{
	typedef T base_type;
	ARCTL_FIELD_VALUE_CLASS(type, ((base_type)) (2(schema::link_queue::kInfID, long)));
};

template <typename T>
struct add_inf_id_field<T, true>
{
	typedef T type;
};

} // namespace lq_list_algo_detail

template <typename Key, typename LqList>
void assign_lq([SA_Pre(Null=SA_No)] ITable* ipLqTable, std::map<Key, LqList>& rLqMap)
{
	typedef typename lq_list_algo_detail::add_inf_id_field<LqList::lq_field_value_type>::type field_value_type;

	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
	ipQueryFilter->SubFields = L"";
	field_value_type::index_type::copy_name(ipLqTable, arctl::adder(ipQueryFilter, &IQueryFilter::_AddField));
	
	const field_value_type::index_type aIndex(ipLqTable);

	atl2::transform(
		ipLqTable->_Search(ipQueryFilter, VARIANT_TRUE),
		make_indirect_lq_insert_iterator(rns::make_mapped_value_referer(rLqMap)),
		arctl::field::interpreter<field_value_type>(&aIndex)
	);
}

} // anonymous namespace

#endif // SINDY_LQ_LIST_ALGO_H_
