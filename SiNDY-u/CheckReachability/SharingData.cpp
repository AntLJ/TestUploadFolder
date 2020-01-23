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

#include "stdafx.h"
#include <boost/format.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/casts.hpp>
#include <boost/lexical_cast.hpp>
#include <atl2/io.h>
#include <atl2/algorithm.h>
#include <arctl/iterator.h>
#include <sindy/link_queue/lq_list_algo.h>
#include "SharingData.h"

namespace {

std::auto_ptr<LQMap> create_turnreg_map(const sindy::link_queue_table& rTurnregTable)
{
	using namespace boost;
	using namespace boost::lambda;
	using namespace arctl;
	using namespace arctl::field;
	using namespace sindy;
	using namespace sindy::schema::inf_turnreg;

	const ITablePtr ipInfTable(rTurnregTable.get_inf_table());

	// ‹ó”’•¶Žš‚ðŠÜ‚Þ‚½‚ß lexical_cast<_bstr_t>(where_all_year_all_days()) ‚Í•s‰ÂB
	const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
	ipQueryFilter->WhereClause = _bstr_t(lexical_cast<std::wstring>(where_all_year_all_days()).c_str());
	ipQueryFilter->SubFields = ipInfTable->OIDFieldName;

	std::auto_ptr<LQMap> pLQMap(new LQMap);
	atl2::for_each(
		ipInfTable->_Search(ipQueryFilter, VARIANT_TRUE),
		var(*pLQMap)[bind(&_IRow::GetOID, ll_static_cast<_IRow*>(_1))]
	);

	assign_lq(rTurnregTable.get_lq_table(), *pLQMap);

	return pLQMap;
}

} // anonymous namespace

std::auto_ptr<SharingData> create_sharing_data(const sindy::link_queue_table& rTurnregTable)
{
	std::auto_ptr<SharingData> pSharingData(new SharingData);
	pSharingData->m_pLQMap = create_turnreg_map(rTurnregTable);
	return pSharingData;
}
