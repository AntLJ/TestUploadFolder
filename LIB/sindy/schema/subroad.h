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

#ifndef SINDY_SCHEMA_SUBROAD_H_
#define SINDY_SCHEMA_SUBROAD_H_

#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace subroad {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kNameKanji[]; ///< 開通予定路線漢字名称
	extern const _TCHAR kNameYomi[]; ///< 開通予定路線カナ名称
	extern const _TCHAR kGeneralRoad[]; ///< 一般道フラグ（[Bug7906]で追加）
} // namespace subroad

namespace subroad_section {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSubRoadID[]; ///< 開通予定路線ID
	extern const _TCHAR kSectionName[]; ///< 開通予定区間名称
	extern const _TCHAR kSchedule[]; ///< 開通予定日時
	extern const _TCHAR kSupplement[]; ///< 開示情報
	extern const _TCHAR kSectionComment[]; ///< コメント
	extern const _TCHAR kExplicitSectionID[]; ///< 明示的区間ID（[Bug7906]で追加）
} // namespace subroad_section

namespace subroad_section_sequence {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[];	///< 開通区間ID
	extern const _TCHAR kPartID[];		///< 開通単位ID
} // namespace subroad_section_sequence

namespace subroad_part {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];
	
	extern const _TCHAR kClose[];		///< 初期状態
	extern const _TCHAR kSectionSeq[];	///< 区間内シーケンス（[Bug6163]で追加）
} // namespace subroad_part

namespace subroad_link {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::from_to_node_relational_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPartID[]; ///< 開通単位ID
} // namespace subroad_link

namespace subroad_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[]; ///< 開通区間ID
} // namespace subroad_line

namespace subroad_facil {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[]; ///< 開通区間ID
	extern const _TCHAR kClose[]; ///< 初期状態
} // namespace subroad_facil

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_SUBROAD_H_
