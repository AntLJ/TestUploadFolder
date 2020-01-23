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
#include <sindy/schema/sindym.h>

namespace sindy {

namespace schema {

namespace sindym {

namespace spot_survey_point {
	const _TCHAR kTableName[] = _T("SPOT_SURVEY_POINT");

	const _TCHAR kObjectidPostgres[] = _T("OBJECTID_POSTGRES");
	const _TCHAR kCompanyName[] = _T("COMPANY_NAME");
	const _TCHAR kAccountName[] = _T("ACCOUNT_NAME");
	const _TCHAR kPermissionLevel[] = _T("PERMISSION_LEVEL");
	const _TCHAR kModifyDatetime[] = _T("MODIFY_DATETIME");
	const _TCHAR kCitymesh[] = _T("CITYMESH");
	const _TCHAR kCreatedBySurveyorF[] = _T("CREATED_BY_SURVEYOR_F");
	const _TCHAR kManagementId[] = _T("MANAGEMENT_ID");
	const _TCHAR kTargetType[] = _T("TARGET_TYPE");
	const _TCHAR kSeason[] = _T("SEASON");
	const _TCHAR kSurveyType[] = _T("SURVEY_TYPE");
	const _TCHAR kAllDisplayF[] = _T("ALL_DISPLAY_F");
	const _TCHAR kSurveyRank[] = _T("SURVEY_RANK");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kNameOrg[] = _T("NAME_ORG");
	const _TCHAR kAddress11[] = _T("ADDRESS11");
	const _TCHAR kSurveyAttribute[] = _T("SURVEY_ATTRIBUTE");
	const _TCHAR kSurveyRequestType[] = _T("SURVEY_REQUEST_TYPE");
	const _TCHAR kCompanyComment[] = _T("COMPANY_COMMENT");
	const _TCHAR kChangedCode[] = _T("CHANGED_CODE");
	const _TCHAR kSurveyComment[] = _T("SURVEY_COMMENT");
	const _TCHAR kPictureImpossibleF[] = _T("PICTURE_IMPOSSIBLE_F");
	const _TCHAR kCreatedByIPCF[] = _T("CREATED_BY_IPC_F");
	const _TCHAR kSurveyDate[] = _T("SURVEY_DATE");
	const _TCHAR kRemarks1[] = _T("REMARKS1");
	const _TCHAR kRemarks2[] = _T("REMARKS2");
	const _TCHAR kFirstCheckCompleteDate[] = _T("FIRST_CHECK_COMPLETE_DATE");
	const _TCHAR kNgAttribute[] = _T("NG_ATTRIBUTE");
	const _TCHAR kNgComment[] = _T("NG_COMMENT");
}

namespace spot_survey_point_relational_table {
	const _TCHAR kSurveyPointId[] = _T("SURVEY_POINT_ID");
}

namespace spot_survey_point_org {
	const _TCHAR kTableName[] = _T("SPOT_SURVEY_POINT_ORG");

	const _TCHAR kCitymeshOrg[] = _T("CITYMESH_ORG");
	const _TCHAR kMeshOid[] = _T("MESH_OID");
}

namespace picture_point {
	const _TCHAR kTableName[] = _T("PICTURE_POINT");

	const _TCHAR kPictureSeq[] = _T("PICTURE_SEQ");
	const _TCHAR kDirection[] = _T("DIRECTION");
}

/// イラストポイント
namespace illustration_point {
	const _TCHAR kTableName[] = _T("ILLUSTRATION_POINT");
}

/// イラストライン
namespace illustration_line {
	const _TCHAR kTableName[] = _T("ILLUSTRATION_LINE");
}

/// イラストポリゴン
namespace illustration_polygon {
	const _TCHAR kTableName[] = _T("ILLUSTRATION_POLYGON");
}
} // sindym

} // schema

} // sindy
