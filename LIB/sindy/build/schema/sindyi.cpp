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
#include <sindy/schema/sindyi.h>

namespace sindy {

namespace schema {

namespace sindyi {

namespace object_locks {
	const _TCHAR kTableName[]			= _T("RECORD_LOCKS");				//!< ƒe[ƒuƒ‹–¼
} // object_locks

namespace info_point {
	const _TCHAR kTableName[]	= _T("INFO_POINT");
} // info_point

namespace info_polyline {
	const _TCHAR kTableName[]	= _T("INFO_LINE");
} // info_polyline

namespace info_polygon {
	const _TCHAR kTableName[]	= _T("INFO_POLYGON");
} // info_polygon

namespace info_master {
	const _TCHAR kTableName[] = _T("INFO_MASTER");
	const _TCHAR kRegistrant[] = _T("REGISTRANT");
	const _TCHAR kRegistDate[] = _T("REGIST_DATE");
	const _TCHAR kTitle[] = _T("TITLE");
	const _TCHAR kInfoType[] = _T("INFOTYPE_C");
	const _TCHAR kDetails[] = _T("DETAILS");
	const _TCHAR kMotoID[] = _T("MOTOID");
	const _TCHAR kOldInfoID[] = _T("OLD_INFOID");
	const _TCHAR kRoadManageID[] = _T("ROADMANAGEID");
	const _TCHAR kMipss[] = _T("MIPSSID");

} // info_master

namespace info_relation {
	const _TCHAR kTableName[] = _T("INFO_RELATION");
	const _TCHAR kRelatedKey[] = _T("RELATED_KEY");
	const _TCHAR kMasterID[] = _T("MASTERID");
	const _TCHAR kConstSeq[] = _T("CONSTSEQ");
	const _TCHAR kRelatedType[] = _T("RELATEDTYPE_C");
} // info_relation

namespace info_src {
	const _TCHAR kTableName[] = _T("INFO_SRC");
	const _TCHAR kMasterID[] = _T("MASTERID");
	const _TCHAR kInfoName[] = _T("INFONAME");
	const _TCHAR kInfoSrc[] = _T("INFOSRC_C");
	const _TCHAR kInfoURL[] = _T("INFOURL");
	const _TCHAR kRegistDate[] = _T("REGIST_DATE");
} // info_src

namespace info_submaster {
	const _TCHAR kTableName[] = _T("INFO_SUBMASTER");
	const _TCHAR kMasterID[] = _T("MASTERID");
	const _TCHAR kEditionNo[] = _T("EDITIONNO");
	const _TCHAR kLock[] = _T("LOCK_F");
	const _TCHAR kValidDate[] = _T("VALID_DATE");
	const _TCHAR kValidDateCode[] = _T("VALID_DATE_C");
	const _TCHAR kOwner[] = _T("OWNER");
	const _TCHAR kTimeLimit[] = _T("TIME_LIMIT");
	const _TCHAR kComplete[] = _T("COMPLETE_F");
	const _TCHAR kStatus[] = _T("STATUS_C");
	const _TCHAR kRank[] = _T("RANK_C");
	const _TCHAR kNoOperation[] = _T("NOOPERATION_C");
	const _TCHAR kResult[] = _T("RESULT");
	const _TCHAR kSurvey[] = _T("SURVEY_C");
	const _TCHAR kPaper[] = _T("PAPER_F");
	const _TCHAR kShereder[] = _T("SHEREDER_C");
} // info_submaster

namespace feature_rel {
	const _TCHAR kTableName[] = _T("FEATURE_REL");
	const _TCHAR kInfoSubID[] = _T("INFOSUBID");
	const _TCHAR kGeometryID[] = _T("GEOMETRYID");
	const _TCHAR kGeometryType[] = _T("GEOMETRYTYPE_C");
	const _TCHAR kMeshCode[] = _T("MESHCODE");
	const _TCHAR kInquiryClass[] = _T("INQUIRYCLASS_C");
	const _TCHAR kNameFixed[] = _T("NAME_C");
	const _TCHAR kName[] = _T("NAME");
	const _TCHAR kAddressFixed[] = _T("ADDRESS_C");
	const _TCHAR kGeometryFixed[] = _T("GEOMETRY_C");
	const _TCHAR kAddress[] = _T("ADDRESS");
	const _TCHAR kAddressCode[] = _T("ADDRESS_CODE");
	const _TCHAR kMaxFloor[] = _T("MAX_FLOOR");
	const _TCHAR kWideSite[] = _T("WIDESITE_F");
	const _TCHAR kRoadNo[] = _T("ROAD_NO");
	const _TCHAR kOpen[] = _T("OPEN_C");
	const _TCHAR kDistance[] = _T("DISTANCE");
	const _TCHAR kWidth[] = _T("WIDTH_C");
	const _TCHAR kRefShape[] = _T("REFSHAPE_C");
	const _TCHAR kChangeAccuracy[] = _T("CHANGE_ACCURACY_C");
	const _TCHAR kDataAccuracy[] = _T("DATA_ACCURACY_C");
	const _TCHAR kDataObtain[] = _T("DATA_OBTAIN_C");
	const _TCHAR kModifyLv[] = _T("MODIFY_LV_C");
	const _TCHAR kFollowup[] = _T("FOLLOWUP_F");
	const _TCHAR kPOINotUse[] = _T("POI_NOTUSE_F");
	const _TCHAR kPOISequences[] = _T("POI_SEQUENCES");
	const _TCHAR kMainF[] = _T("MAIN_F");
	const _TCHAR kSiteArea[] = _T("SITE_AREA");
	const _TCHAR kSiteAreaCode[] = _T("SITE_AREA_C");
	const _TCHAR kBuildArea[] = _T("BUILD_AREA");
	const _TCHAR kBuildAreaCode[] = _T("BUILD_AREA_C");
} // feature_rel

namespace work_target {
	const _TCHAR kTableName[] = _T("WORK_TARGET");
	const _TCHAR kInfoSubID[] = _T("INFOSUBID");
	const _TCHAR kEditType[] = _T("EDITTYPE_C");
	const _TCHAR kWorkName[] = _T("WORK_NAME_C");
	const _TCHAR kRegistrant[] = _T("REGISTRANT");
	const _TCHAR kRegistDate[] = _T("REGIST_DATE");
	const _TCHAR kTimeLimit[] = _T("TIME_LIMIT");
	const _TCHAR kDept[] = _T("DEPT_C");
	const _TCHAR kReflect[] = _T("REFLECT_C");
	const _TCHAR kDifficultyLevel[] = _T("DIFFICULTY_LV_C");
} // work_target

namespace work_progress {
	const _TCHAR kTableName[] = _T("WORK_PROGRESS");
	const _TCHAR kTargetID[] = _T("TARGETID");
	const _TCHAR kStatus[] = _T("STATUS_C");
	const _TCHAR kWorker[] = _T("WORKER");
	const _TCHAR kStartTime[] = _T("START_TIME");
	const _TCHAR kEndTime[] = _T("END_TIME");
	const _TCHAR kComment[] = _T("COMMENTS");
} // work_progress

namespace work_add_item {
	const _TCHAR kTableName[] = _T("WORK_ADD_ITEM");
	const _TCHAR kProgressID[] = _T("PROGRESSID");
	const _TCHAR kColID[] = _T("COLID");
	const _TCHAR kkColValue[] = _T("COLVALUE");
} // work_add_item

namespace contact_master {
	const _TCHAR kTableName[] = _T("CONTACT_MASTER");

	const _TCHAR kIndustry_Name_C[] = _T("INDUSTRY_NAME_C");
	const _TCHAR kHead_Org[] = _T("HEAD_ORG");
	const _TCHAR kHead_Yomi[] = _T("HEAD_YOMI");
	const _TCHAR kMain_Num[] = _T("MAIN_NUM");
	const _TCHAR kPost_Code[] = _T("POST_CODE");
	const _TCHAR kAddress[] = _T("ADDRESS");
	const _TCHAR kURL[] = _T("URL");
	const _TCHAR kBiz_Brand[] = _T("BIZ_BRAND");
	const _TCHAR kRemarks[] = _T("REMARKS");
	const _TCHAR kPerson[] = _T("PERSON");
	const _TCHAR kClose_F[] = _T("CLOSE_F");
} // contact_master

namespace contact_submaster {
	const _TCHAR kTableName[] = _T("CONTACT_SUBMASTER");

	const _TCHAR kHeadId[] = _T("HEADID");
	const _TCHAR kBranch_Org[] = _T("BRANCH_ORG");
	const _TCHAR kBranch_Yomi[] = _T("BRANCH_YOMI");
	const _TCHAR kContact_Person[] = _T("CONTACT_PERSON");
	const _TCHAR kContact_Person_Yomi[] = _T("CONTACT_PERSON_YOMI");
	const _TCHAR kPost_Code[] = _T("POST_CODE");
	const _TCHAR kAddress[] = _T("ADDRESS");
	const _TCHAR kPhone[] = _T("PHONE");
	const _TCHAR kExtension_Num[] = _T("EXTENSION_NUM");
	const _TCHAR kFax[] = _T("FAX");
	const _TCHAR kRemarks[] = _T("REMARKS");
	const _TCHAR kPermit_C[] = _T("PERMIT_C");
	const _TCHAR kRegistrant[] = _T("REGISTRANT");
	const _TCHAR kRegist_Date[] = _T("REGIST_DATE");
}

namespace inquiry_history {
	const _TCHAR kTableName[] = _T("INQUIRY_HISTORY");

	const _TCHAR kInfoSubId[] = _T("INFOSUBID");
	const _TCHAR kContactSId[] = _T("CONTACTSID");
	const _TCHAR kUpdater[] = _T("UPDATER");
	const _TCHAR kUpdate_Date[] = _T("UPDATE_DATE");
	const _TCHAR kUpdate_Date_C[] = _T("UPDATE_DATE_C");
	const _TCHAR kNext_Date[] = _T("NEXT_DATE");
	const _TCHAR kComments[] = _T("COMMENTS");
}

namespace work_item_registry {
	const _TCHAR kTableName[] = _T("WORK_ITEM_REGISTRY");
	const _TCHAR kEditType[] = _T("EDITTYPE_C");
	const _TCHAR kStatus[] = _T("STATUS_C");
	const _TCHAR kDispOrder[] = _T("DISP_ORDER");
	const _TCHAR kColUse[] = _T("COL_USE_C");
	const _TCHAR kColName[] = _T("COL_NAME");
	const _TCHAR kColType[] = _T("COLTYPE_C");
	const _TCHAR kColDomain[] = _T("COLDOMAIN");
}

namespace work_target_master {
	const _TCHAR kTableName[] = _T("WORK_TARGET_MASTER");

	const _TCHAR kWorkCode[] = _T("WORK_CODE");
	const _TCHAR kWorkName[] = _T("WORK_NAME");
	const _TCHAR kBaseDate[] = _T("BASE_DATE");
	const _TCHAR kRelease_Codes[] = _T("RELEASE_CODES");
	const _TCHAR kDispF[] = _T("DISP_F");
	const _TCHAR kDataType_C[] = _T("DATATYPE_C");
}

namespace work_indication {
	const _TCHAR kTableName[] = _T("WORK_INDICATION");

	const _TCHAR kInfoSubId[] = _T("INFOSUBID");
	const _TCHAR kFromDept[] = _T("FROM_DEPT_C");
	const _TCHAR kToDept[] = _T("TO_DEPT_C");
	const _TCHAR kRelated[] = _T("RELATED_C");
	const _TCHAR kIndication[] = _T("INDICATION");
	const _TCHAR kRegistrant[] = _T("REGISTRANT");
	const _TCHAR kRegistDate[] = _T("REGIST_DATE");
}

namespace info_tag {
	const _TCHAR kTableName[] = _T("INFO_TAG");

	const _TCHAR kInfoSubId[] = _T("INFOSUBID");
	const _TCHAR kTagName[] = _T("TAG_NAME_C");

}

} // sindyi

} // schema

} // sindy
