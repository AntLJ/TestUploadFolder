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

#include "StdAfx.h"
#include <sindy/schema/toll.h>

namespace sindy {

namespace schema {

/// 片側の料金額テーブル
namespace onesidetoll_table {
	const _TCHAR kKei_Toll[]		= _T("K_TOLL");
	const _TCHAR kHutuu_Toll[]		= _T("S_TOLL");
	const _TCHAR kChuu_Toll[]		= _T("M_TOLL");
	const _TCHAR kDai_Toll[]		= _T("L_TOLL");
	const _TCHAR kTokudai_Toll[]	= _T("VL_TOLL");
}

/// 上下別の料金額テーブル
namespace updowntoll_table {
	const _TCHAR kKei_Toll_Up[]			= _T("K_TOLL_UP");
	const _TCHAR kHutuu_Toll_Up[]		= _T("S_TOLL_UP");
	const _TCHAR kChuu_Toll_Up[]		= _T("M_TOLL_UP");
	const _TCHAR kDai_Toll_Up[]			= _T("L_TOLL_UP");
	const _TCHAR kTokudai_Toll_Up[]		= _T("VL_TOLL_UP");
	
	const _TCHAR kKei_Toll_Down[]		= _T("K_TOLL_DOWN");
	const _TCHAR kHutuu_Toll_Down[]		= _T("S_TOLL_DOWN");
	const _TCHAR kChuu_Toll_Down[]		= _T("M_TOLL_DOWN");
	const _TCHAR kDai_Toll_Down[]		= _T("L_TOLL_DOWN");
	const _TCHAR kTokudai_Toll_Down[]	= _T("VL_TOLL_DOWN");
}

/// FromTo施設テーブル
namespace fromto_tolltable {
	const _TCHAR kRoadCode1[]		= _T("ROAD_CODE1");
	const _TCHAR kRoadSeq1[]		= _T("ROAD_SEQ1");
	const _TCHAR kRoadCode2[]		= _T("ROAD_CODE2");
	const _TCHAR kRoadSeq2[]		= _T("ROAD_SEQ2");
}

/// JH←→独自整備施設コード変換テーブル
namespace facil_conv_table {
	const _TCHAR kTableName[]	= _T("TOLL_FACIL_CONV_TABLE");

	const _TCHAR kJHRoadCode[]	= _T("JH_ROAD_CODE");
	const _TCHAR kJHRoadSeq[]	= _T("JH_ROAD_SEQ");
	const _TCHAR kMemo[]		= _T("TOLL_MEMO");
}

/// JH路線コードテーブル
namespace jh_road_code_list_table {
	const _TCHAR kJHDispCode[]		= _T("JH_DISP_CODE");
	const _TCHAR kJHRoadCode[]		= _T("JH_ROAD_CODE");
	const _TCHAR kJHRoadName[]		= _T("JH_ROAD_NAME");
}

/// JH施設コードテーブル
namespace jh_facil_code_list_table {
	const _TCHAR kJHRoadCode[]		= _T("JH_ROAD_CODE");
	const _TCHAR kJHRoadSeq[]		= _T("JH_ROAD_SEQ");
	const _TCHAR kJHFacilName[]		= _T("JH_FACIL_NAME");
}

namespace jh_toll_table {
	const _TCHAR kJHRoadCode1[]		= _T("JH_ROAD_CODE1");
	const _TCHAR kJHRoadSeq1[]		= _T("JH_ROAD_SEQ1");
	const _TCHAR kJHRoadCode2[]		= _T("JH_ROAD_CODE2");
	const _TCHAR kJHRoadSeq2[]		= _T("JH_ROAD_SEQ2");
	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");

	const _TCHAR kReserved1[]		= _T("RESERVED1");
	const _TCHAR kDistance[]		= _T("DISTANCE");
}

namespace jh_patch_table {
	const _TCHAR kTableName[]		= _T("TOLL_JH_PATCH_TABLE");

	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");
}

namespace other_table {
	const _TCHAR kTableName[]		= _T("TOLL_OTHER_TABLE");

	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");

	const _TCHAR kNormal_F[]		= _T("NORMAL_F");
	const _TCHAR kUpdown_C1[]		= _T("UPDOWN_C1");
	const _TCHAR kUpdown_C2[]		= _T("UPDOWN_C2");
}

namespace alone_table {
	const _TCHAR kTableName[]		= _T("TOLL_ALONE_TABLE");

	const _TCHAR kLineClass_C[]		= _T("LINECLASS_C");
	const _TCHAR kRoadCode[]		= _T("ROAD_CODE");
	const _TCHAR kRoadSeq[]			= _T("ROAD_SEQ");
	
	const _TCHAR kUpdown_C[]		= _T("UPDOWN_C");
	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");
	const _TCHAR kStdLinkClass_C[]	= _T("STD_LINECLASS_C");
	const _TCHAR kStdRoadCode[]		= _T("STD_ROAD_CODE");

	const _TCHAR kNormal_F[]		= _T("NORMAL_F");
}

namespace nested_table {
	const _TCHAR kTableName[]		= _T("TOLL_NESTED_TABLE");

	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");
}

namespace samefacil_table {
	const _TCHAR kTableName[]		= _T("TOLL_SAME_FACIL_TABLE");

	const _TCHAR kAlone_F[]			= _T("ALONE_F");
}
namespace free_table {
	const _TCHAR kTableName[]		= _T("TOLL_FREE_TABLE");
}

namespace temp_facil_table {
	const _TCHAR kTableName[]		= _T("TOLL_TEMP_FACIL_TABLE");

	const _TCHAR kRoadCode[]		= _T("ROAD_CODE");
	const _TCHAR kRoadSeq[]			= _T("ROAD_SEQ");
	const _TCHAR kDiscount_C[]		= _T("ETC_DISCOUNT_C");

	const _TCHAR kTempTollClass[]	= _T("TEMPTOLLCLASS_C");

	const _TCHAR kTollMemo[]		= _T("TOLL_MEMO");
}

namespace road_code_list {
	const _TCHAR kLandClass[]		= _T("LANDCLASS_C");
}

namespace toll_discount_common {
	const _TCHAR kEtcDiscount[]		= _T("ETC_DISCOUNT_C");
	const _TCHAR kTollMemo[]		= _T("TOLL_MEMO");
}

namespace toll_discount_table {
	const _TCHAR kTableName[]		= _T("TOLL_DISCOUNT_TABLE");

	const _TCHAR kDiscountName[]	= _T("DISCOUNT_NAME");
	const _TCHAR kManagerName[]		= _T("MANAGER_NAME");
	const _TCHAR kEtc[]				= _T("ETC_F");
	const _TCHAR kTypeKei[]			= _T("TYPE_KEI_F");
	const _TCHAR kTypeHutuu[]		= _T("TYPE_HUTUU_F");
	const _TCHAR kTypeChuu[]		= _T("TYPE_CHUU_F");
	const _TCHAR kTypeDai[]			= _T("TYPE_DAI_F");
	const _TCHAR kTypeTokuDai[]		= _T("TYPE_TOKUDAI_F");
	const _TCHAR kOverStart[]		= _T("OVER_START_F");
	const _TCHAR kOverEnd[]			= _T("OVER_END_F");
	const _TCHAR kOverAll[]			= _T("OVER_ALL_F");
	const _TCHAR kRestriction[]		= _T("RESTRICTION");
	const _TCHAR kCloseDate[]		= _T("CLOSE_DATE");
}

namespace toll_discounttime_table {
	const _TCHAR kTableName[]		= _T("TOLL_DISCOUNTTIME_TABLE");

	const _TCHAR kWeekWeekday[]		= _T("WEEK_WEEKDAY_F");
	const _TCHAR kWeekSaturday[]	= _T("WEEK_SATURDAY_F");
	const _TCHAR kWeekSunday[]		= _T("WEEK_SUNDAY_F");
	const _TCHAR kWeekHoliday[]		= _T("WEEK_HOLIDAY_F");
	const _TCHAR kTermStartMonth[]	= _T("TERM_START_MONTH");
	const _TCHAR kTermStartDay[]	= _T("TERM_START_DAY");
	const _TCHAR kTermEndMonth[]	= _T("TERM_END_MONTH");
	const _TCHAR kTermEndDay[]		= _T("TERM_END_DAY");
	const _TCHAR kStartHour[]		= _T("START_HOUR");
	const _TCHAR kStartMin[]		= _T("START_MIN");
	const _TCHAR kEndHour[]			= _T("END_HOUR");
	const _TCHAR kEndMin[]			= _T("END_MIN");
}

} // schema

} // sindy
