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
#include <sindy/schema/multi_language.h>

namespace sindy
{
namespace schema
{
namespace multi_language
{
	namespace ipc_mlang_table
	{
		extern const _TCHAR kDeleteF[] = _T("DELETE_F");
		extern const _TCHAR kLangCode[] = _T("LANGCODE");
	}

	namespace partial_name_table
	{
		extern const _TCHAR kSortOrder[] = _T("SORT_ORDER");
		extern const _TCHAR kPartialName[] = _T("PARTIAL_NAME");
	}

	namespace annotation
	{
		const _TCHAR kTableName[] = _T("MLANG_ANNOTATION");

		extern const _TCHAR kAnnotationScale[] = _T("ANNOTATION_SCALE");
		extern const _TCHAR kAnnotationID[] = _T("ANNOTATION_ID");
		extern const _TCHAR kWorkName[] = _T("WORK_NAME");
		extern const _TCHAR kOrgName[] = _T("ORG_NAME");
		extern const _TCHAR kOrgYomi[] = _T("ORG_YOMI");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace poi
	{
		const _TCHAR kTableName[] = _T("MLANG_POI");

		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace poi_partial
	{
		const _TCHAR kTableName[] = _T("MLANG_POI_PARTIAL");
	}

	namespace railway
	{
		const _TCHAR kTableName[] = _T("MLANG_RAILWAY");

		extern const _TCHAR kTrainTypeCode[] = _T("TRAINTYPECODE");
		extern const _TCHAR kTrainCorpCode[] = _T("TRAINCORPCODE");
		extern const _TCHAR kTrainLineCode[] = _T("TRAINLINECODE");
		extern const _TCHAR kLineSeq[] = _T("LINESEQ");
		extern const _TCHAR kStationSeq[] = _T("STATION_SEQ");
		extern const _TCHAR kName[] = _T("NAME");
		extern const _TCHAR kSubName[] = _T("SUBNAME");
	}

	namespace railway_partial
	{
		const _TCHAR kTableName[] = _T("MLANG_RAILWAY_PARTIAL");
	}

	namespace addr
	{
		const _TCHAR kTableName[] = _T("MLANG_ADDR");

		extern const _TCHAR kAddrCode[] = _T("ADDRCODE");
		extern const _TCHAR kName[] = _T("NAME");
		extern const _TCHAR kSubName[] = _T("SUBNAME");
	}

	namespace road
	{
		const _TCHAR kTableName[] = _T("MLANG_ROAD");

		extern const _TCHAR kRoadTypeCode[]  = _T("ROADTYPECODE");
		extern const _TCHAR kRoadLineCode[]  = _T("ROADLINECODE");
		extern const _TCHAR kRoadSeq[]       = _T("ROAD_SEQ");
		extern const _TCHAR kFclAttCode[]    = _T("FCL_ATTCODE");
		extern const _TCHAR kDirectionCode[] = _T("DIRECTIONCODE");
		extern const _TCHAR kName[]      = _T("NAME");
		extern const _TCHAR kSubName[]   = _T("SUBNAME");
		extern const _TCHAR kShortName[] = _T("SHORTNAME");
		extern const _TCHAR kDisplayName[] = _T("DISPLAYNAME");
	}

	namespace road_partial
	{
		const _TCHAR kTableName[] = _T("MLANG_ROAD_PARTIAL");
	}

	namespace master
	{
		namespace chain_master
		{
			const _TCHAR kTableName[] = _T("MLANG_CHAIN_MASTER");

			extern const _TCHAR kChainCode[] = _T("CHAINCODE");
			extern const _TCHAR kName[] = _T("NAME");
		}

		namespace group_master
		{
			const _TCHAR kTableName[] = _T("MLANG_GROUP_MASTER");

			extern const _TCHAR kGroupCode[] = _T("GRPCODE");
			extern const _TCHAR kName[] = _T("NAME");
		}
	}

	namespace pref_point
	{
		const _TCHAR kTableName[] = _T("MLANG_PREF_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace city_point
	{
		const _TCHAR kTableName[] = _T("MLANG_CITY_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kCityCode[] = _T("CITYCODE");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace oaza_point
	{
		const _TCHAR kTableName[] = _T("MLANG_OAZA_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kCityCode[] = _T("CITYCODE");
		extern const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace aza_point
	{
		const _TCHAR kTableName[] = _T("MLANG_AZA_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kCityCode[] = _T("CITYCODE");
		extern const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
		extern const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace main_no_point
	{
		const _TCHAR kTableName[] = _T("MLANG_MAIN_NO_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kCityCode[] = _T("CITYCODE");
		extern const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
		extern const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
		extern const _TCHAR kChibanCode[] = _T("CHIBANCODE");
		extern const _TCHAR kName[] = _T("NAME");
	}

	namespace sub_no_point
	{
		const _TCHAR kTableName[] = _T("MLANG_SUB_NO_POINT");

		extern const _TCHAR kPrefCode[] = _T("PREFCODE");
		extern const _TCHAR kCityCode[] = _T("CITYCODE");
		extern const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
		extern const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
		extern const _TCHAR kChibanCode[] = _T("CHIBANCODE");
		extern const _TCHAR kJukyoCode[] = _T("JUKYOCODE");
		extern const _TCHAR kName[] = _T("NAME");
	}
} // namespace multi_language
} // namespace schema
} // namespace sindy
