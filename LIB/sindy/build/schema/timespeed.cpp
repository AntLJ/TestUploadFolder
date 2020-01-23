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
#include "sindy/schema/timespeed.h"

namespace sindy {

namespace schema {

namespace timespeed_table {
	const _TCHAR kLinkID[] = _T("LINK_ID");
	const _TCHAR kLinkDir[] = _T("LINK_DIR_F");
	const _TCHAR kHoliday[] = _T("HOLIDAY_F");
	const _TCHAR kTimeGroup[] = _T("TIME_GROUP_C");
}

namespace timezone_speed {
	const _TCHAR kTableName[] = _T("TIMEZONE_SPEED");

	const _TCHAR kTripSpeed_M[] = _T("TRIP_SPEED_M");
	const _TCHAR kTripSpeed_R[] = _T("TRIP_SPEED_R");
	const _TCHAR kSampleCount[] = _T("SAMPLE_COUNT");
	const _TCHAR kNewestDate[] = _T("NEWEST_DATE");
	const _TCHAR kOldestDate[] = _T("OLDEST_DATE");
	const _TCHAR kRenewCount[] = _T("RENEW_COUNT");
	const _TCHAR kAdopt[] = _T("ADOPT_C");
}

namespace timezone_speed_probe {
	const _TCHAR kTableName[] = _T("TIMEZONE_SPEED_PROBE");

	const _TCHAR kTripSpeed_M[] = _T("TRIP_SPEED");
	const _TCHAR kSampleCount[] = _T("SAMPLE_COUNT");
}

}

}
