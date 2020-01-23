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

#ifndef SINDY_FIELDS_HIGHWAY_FACIL_ID_H_
#define SINDY_FIELDS_HIGHWAY_FACIL_ID_H_

#include <arctl/field/value.h>
#include <sindy/schema/category.h>

namespace sindy {

/**
 * @class highway_facil_id
 * @brief ハイウェイ施設識別子
 */
ARCTL_FIELD_VALUE_CLASS(
	facil_id,
	(2(schema::category::highway_facil_relational_table::kRoadCode, long))
	(2(schema::category::highway_facil_relational_table::kRoadSeq, long))
);

/**
 * @brief ハイウェイ識別子の有効性判定
 *
 * 路線コードと路線内シーケンス、どちらか一方でも0だったら無効。
 */
inline bool operator!(const facil_id& rhs)
{
	using namespace arctl::field;
	using namespace schema::category::highway_facil_relational_table;

	return get<kRoadCode>(rhs) == 0 || get<kRoadSeq>(rhs) == 0;
}

} // namespace sindy

#endif // SINDY_FIELDS_HIGHWAY_FACIL_ID_H_
