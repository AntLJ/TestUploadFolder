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

	extern const _TCHAR kNameKanji[]; ///< �J�ʗ\��H����������
	extern const _TCHAR kNameYomi[]; ///< �J�ʗ\��H���J�i����
	extern const _TCHAR kGeneralRoad[]; ///< ��ʓ��t���O�i[Bug7906]�Œǉ��j
} // namespace subroad

namespace subroad_section {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSubRoadID[]; ///< �J�ʗ\��H��ID
	extern const _TCHAR kSectionName[]; ///< �J�ʗ\���Ԗ���
	extern const _TCHAR kSchedule[]; ///< �J�ʗ\�����
	extern const _TCHAR kSupplement[]; ///< �J�����
	extern const _TCHAR kSectionComment[]; ///< �R�����g
	extern const _TCHAR kExplicitSectionID[]; ///< �����I���ID�i[Bug7906]�Œǉ��j
} // namespace subroad_section

namespace subroad_section_sequence {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[];	///< �J�ʋ��ID
	extern const _TCHAR kPartID[];		///< �J�ʒP��ID
} // namespace subroad_section_sequence

namespace subroad_part {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];
	
	extern const _TCHAR kClose[];		///< �������
	extern const _TCHAR kSectionSeq[];	///< ��ԓ��V�[�P���X�i[Bug6163]�Œǉ��j
} // namespace subroad_part

namespace subroad_link {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::from_to_node_relational_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kPartID[]; ///< �J�ʒP��ID
} // namespace subroad_link

namespace subroad_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[]; ///< �J�ʋ��ID
} // namespace subroad_line

namespace subroad_facil {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kSectionID[]; ///< �J�ʋ��ID
	extern const _TCHAR kClose[]; ///< �������
} // namespace subroad_facil

} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_SUBROAD_H_
