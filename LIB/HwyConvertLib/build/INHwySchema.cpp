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

#include "INHwySchema.h"
#include "SindyAttrDef.h"
#include <boost/assign.hpp>

using namespace boost::assign;

/**
 * @brief 与えられたSiNDYのコード値に対応するI/Nのコード値を返す
 * @param[in]	cCodeTable	コード値対応テーブル
 * @param[in]	cSindyCode	SiNDYのコード値
 * @return	I/Nのコード値
 * @note	I/Nのコード値を求めることに失敗した場合はstd::runtime_error例外を送出する
 * @note	SINDY2INCODE.hに定義されているSINDY2INCODE名前空間とは無関係なので要注意（対応テーブルの記述方法の参考にはした）
 */
INCode Sindy2INCode(const CodeTable& cCodeTable, SindyCode cSindyCode)
{
	CodeTable::const_iterator aIt = cCodeTable.find(cSindyCode);
	if (aIt == cCodeTable.end())
		throw std::runtime_error("SiNDYのコード値からI/Nのコード値を求めるのに失敗");
	return aIt->second;
}

namespace in {

namespace schema {

namespace hwy_common_field
{
	namespace facil_class
	{
		CodeTable tFacilClass = map_list_of
			(sindy::facil_info_point::facilclass_sa,		sa)
			(sindy::facil_info_point::facilclass_pa,		pa)
			(sindy::facil_info_point::facilclass_jct,		jct)
			(sindy::facil_info_point::facilclass_ramp,		ramp)
			(sindy::facil_info_point::facilclass_ic,		ic)
			(sindy::facil_info_point::facilclass_smartic,	smartic)
			(sindy::facil_info_point::facilclass_tollgate,	tollbooth);
	}

	namespace inout
	{
		CodeTable tInOut = map_list_of
			(sindy::inout::inout_no,	no_inout)
			(sindy::inout::inout_enter,	in)
			(sindy::inout::inout_exit,	out);
	}

}	// namespace hwy_common_field

namespace hwynode
{
	namespace direction
	{
		CodeTable tDirection = map_list_of
			(sindy::highway_node::direction_no,		nodir)
			(sindy::highway_node::direction_up,		up)
			(sindy::highway_node::direction_down,	down)
			(sindy::highway_node::direction_common,	common);
	}

}	// namespace hwynode

namespace fclpoint
{
	namespace direction
	{
		CodeTable tDirection = map_list_of
			(sindy::access_point::direction_no,		nodir)
			(sindy::access_point::direction_up,		up)
			(sindy::access_point::direction_down,	down)
			(sindy::access_point::direction_common,	common);
	}

}	// namespace fclpoint

namespace fclrep
{
	namespace searchclass
	{
		CodeTable tSearchClass = map_list_of
			(sindy::facil_info_point::searchclass_not_for_search,	not_for_search)
			(sindy::facil_info_point::searchclass_sa,				sa)
			(sindy::facil_info_point::searchclass_pa,				pa)
			(sindy::facil_info_point::searchclass_jct,				jct)
			(sindy::facil_info_point::searchclass_ramp,				ramp)
			(sindy::facil_info_point::searchclass_ic,				ic)
			(sindy::facil_info_point::searchclass_tollgate,			tollbooth)
			(sindy::facil_info_point::searchclass_smartic,			smartic);
	}

}	// namespace fclrep

namespace road_code_doc
{
	namespace line_class
	{
		CodeTable tLineClass = map_list_of
			(sindy::road_code_list::lineclass_ferry,		ferry)
			(sindy::road_code_list::lineclass_street,		street)
			(sindy::road_code_list::lineclass_tfspecial,	street)				// 「一般有料道路」を「街道名」と同一視する
			(sindy::road_code_list::lineclass_highway,		highway)
			(sindy::road_code_list::lineclass_tollroad,		tollroad)
			(sindy::road_code_list::lineclass_metropolis,	metropolis)
			(sindy::road_code_list::lineclass_hanshin,		hanshin)
			(sindy::road_code_list::lineclass_nagoya,		nagoya)
			(sindy::road_code_list::lineclass_fukuoka,		fukuoka)
			(sindy::road_code_list::lineclass_kitakyushu,	kitakyushu)
			(sindy::road_code_list::lineclass_hiroshima,	hiroshima)
			(sindy::road_code_list::lineclass_kyouto,		kyouto);

		CodeTable tLineClassForSiRiUS = map_list_of
			(sindy::road_code_list::lineclass_ferry,		ferry)
			(sindy::road_code_list::lineclass_street,		street)
			(sindy::road_code_list::lineclass_tfspecial,	ordinary_tollroad)	// tLineClassとはここが違う
			(sindy::road_code_list::lineclass_highway,		highway)
			(sindy::road_code_list::lineclass_tollroad,		tollroad)
			(sindy::road_code_list::lineclass_metropolis,	metropolis)
			(sindy::road_code_list::lineclass_hanshin,		hanshin)
			(sindy::road_code_list::lineclass_nagoya,		nagoya)
			(sindy::road_code_list::lineclass_fukuoka,		fukuoka)
			(sindy::road_code_list::lineclass_kitakyushu,	kitakyushu)
			(sindy::road_code_list::lineclass_hiroshima,	hiroshima)
			(sindy::road_code_list::lineclass_kyouto,		kyouto);
	}

}	// namespace road_code_doc

}	// namespace schema

}	// namespace in
