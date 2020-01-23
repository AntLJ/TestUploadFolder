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
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace category {

const _TCHAR mesh_code_table::kMeshCode[] = _T("MESHCODE");

const _TCHAR link_relational_table::kLinkID[] = _T("LINK_ID");
const _TCHAR directional_link_relational_table::kLinkDir[] = _T("LINKDIR_C");

namespace from_to_node_relational_table {
	const _TCHAR kFromNodeID[] = _T("FROM_NODE_ID");
	const _TCHAR kToNodeID[] = _T("TO_NODE_ID");
}

namespace time_regulation_relational_table {
	const _TCHAR kOneway[]      = _T("ONEWAY_C");
	const _TCHAR kOnewayRF[]    = _T("ONEWAY_RF");
	const _TCHAR kNoPassage[]   = _T("NOPASSAGE_C");
	const _TCHAR kNoPassageRF[] = _T("NOPASSAGE_RF");
} // time_regulation_relational_table

namespace time_condition_table {
	const _TCHAR kStartMonth[] = _T("START_MONTH");
	const _TCHAR kStartDay[] = _T("START_DAY");
	const _TCHAR kStartHour[] = _T("START_HOUR");
	const _TCHAR kStartMin[] = _T("START_MIN");
	const _TCHAR kEndMonth[] = _T("END_MONTH");
	const _TCHAR kEndDay[] = _T("END_DAY");
	const _TCHAR kEndHour[] = _T("END_HOUR");
	const _TCHAR kEndMin[] = _T("END_MIN");
	const _TCHAR kDayOfWeek[] = _T("DAYOFWEEK_C");
}

namespace vics_table {
	const _TCHAR kVICSMeshCode[] = _T("VICS_MESHCODE");
	const _TCHAR kVICSClass[] = _T("VICSCLASS_C");
	const _TCHAR kVICSLinkID[] = _T("VICS_LINK_ID");
}

namespace original_vics_table {
	const _TCHAR kMeshCode[] = _T("MESHCODE");
	const _TCHAR kVICSClass[] = _T("VICS_CLASS");
	const _TCHAR kVICSLinkID[] = _T("VICS_ID");
}

namespace drive_map_table {
	const _TCHAR kMeshCode[] = _T("MeshCode");
	const _TCHAR kDriveMapRank[] = _T("Rank");
	const _TCHAR kRankLinkID[] = _T("Rank_Link_ID");
}

const _TCHAR parking_table::kParkingClass[] = _T("PARKINGCLASS_C");

namespace highway_road_relational_table {
	const _TCHAR kRoadCode[] = _T("ROAD_CODE");
}

namespace highway_facil_relational_table {
	const _TCHAR kRoadSeq[] = _T("ROAD_SEQ");
}

namespace highway_direction_table {
	const _TCHAR kDirection[] = _T("DIRECTION_C");
}

namespace highway_in_out_table {
	const _TCHAR kInOut[] = _T("INOUT_C");
}

namespace sindyc_table {
	const _TCHAR kOrgOperator[] = _T("ORG_OPERATOR");
	const _TCHAR kOrgModifyDate[] = _T("ORG_MODIFYDATE");
	const _TCHAR kOrgOBJID[] = _T("ORG_OBJID");
	const _TCHAR kUpdateC[] = _T("UPDATE_C");
	const _TCHAR kCheckC[] = _T("CHECK_C");
}

namespace serial_id_table
{
	const _TCHAR kSerialID[] = _T("SERIALID");
}

namespace contents_seq_table
{
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ");
}

namespace group_code_table
{
	const _TCHAR kGroupCode[] = _T("GRPCODE");
}

namespace chain_code_table
{
	const _TCHAR kChainCode[] = _T("CHAINCODE");
}

namespace contents_code_table
{
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE");
}

/// 区間別称に関連付くテーブル [bug 12036]
namespace section_code_relational_table {
	const _TCHAR kSectionCode[] = _T("SECTION_CODE");
}

/// 都道府県道名称に関連付くテーブル [bug 12036]
namespace pref_road_relational_table {
	const _TCHAR kRoadClass[] = _T("ROADCLASS_C");
	const _TCHAR kPrefCode[]  = _T("PREF_CODE");
	const _TCHAR kRoadNo[]    = _T("ROAD_NO");
}

/// ADAM v2用
namespace adam_v2_from_to_node_relational_table_gid {
	const _TCHAR kFromNodeGID[] = _T("FROM_NODE_GID");
	const _TCHAR kToNodeGID[] = _T("TO_NODE_GID");
}

/// ノード
namespace adam_v2_node {
	const _TCHAR kNodeClassC[] = _T("NODECLASS_C");		///< ノード種別コード
}

namespace adam_v2_group{
	extern const _TCHAR kGroupC[] = _T("GROUP_C");		///< グループ種別コード
}

} // namespace category

} // namespace schema

} // namespace sindy
