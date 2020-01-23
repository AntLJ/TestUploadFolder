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
#include <sindy/schema/subroad.h>

namespace sindy {

namespace schema {

namespace subroad {
	const _TCHAR kTableName[] = _T("SUBROAD");

	const _TCHAR kNameKanji[] = _T("NAME_KANJI");
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");
	const _TCHAR kGeneralRoad[] = _T("GENERALROAD_F");		// [Bug7906]‚Å’Ç‰Á
} // namespace subroad

namespace subroad_section {
	const _TCHAR kTableName[] = _T("SUBROAD_SECTION");

	const _TCHAR kSubRoadID[] = _T("SUBROAD_ID");
	const _TCHAR kSectionName[] = _T("SECTION_NAME");
	const _TCHAR kSchedule[] = _T("SCHEDULE");
	const _TCHAR kSupplement[] = _T("SUPPLEMENT");
	const _TCHAR kSectionComment[] = _T("SECTION_COMMENT");
	const _TCHAR kExplicitSectionID[] = _T("EXPLICIT_SECTION_ID");	// [Bug7906]‚Å’Ç‰Á
} // namespace subroad_section

namespace subroad_section_sequence {
	const _TCHAR kTableName[] = _T("SUBROAD_SECTION_SEQUENCE");

	const _TCHAR kSectionID[] = _T("SECTION_ID");
	const _TCHAR kPartID[] = _T("PART_ID");
} // namespace subroad_section_sequence

namespace subroad_part {
	const _TCHAR kTableName[] = _T("SUBROAD_PART");

	const _TCHAR kClose[] = _T("CLOSE_F");
	const _TCHAR kSectionSeq[] = _T("SECTION_SEQ");
} // namespace subroad_part

namespace subroad_link {
	const _TCHAR kTableName[] = _T("SUBROAD_LINK");

	const _TCHAR kPartID[] = _T("PART_ID");
	const _TCHAR kFromNodeID[] = _T("FROM_NODE_ID");
	const _TCHAR kToNodeID[] = _T("TO_NODE_ID");
} // namespace subroad_link

namespace subroad_line {
	const _TCHAR kTableName[] = _T("SUBROAD_LINE");

	const _TCHAR kSectionID[] = _T("SECTION_ID");
} // namespace subroad_line

namespace subroad_facil {
	const _TCHAR kTableName[] = _T("SUBROAD_FACIL");
	
	const _TCHAR kSectionID[] = _T("SECTION_ID");
	const _TCHAR kClose[] = _T("CLOSE_F");
} // namespace subroad_facil

} // namespace schema

} // namespace sindy
