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
#include "GsSchema.h"

namespace gs {

namespace schema {

// 整備用住所ポイント
namespace gs_addr_point
{
	const _TCHAR kTableName[] = _T("ADDR_POINT");

	const _TCHAR kGeoBFid[] = _T("GEOBFID");
	const _TCHAR kIpcObjId[] = _T("IPCOBJID");
	const _TCHAR kPlaceCd[] = _T("PLACE_CD");
	const _TCHAR kStreetCd[] = _T("STREET_CD");
	const _TCHAR kAddress1[] = _T("ADDRESS1");
	const _TCHAR kAddress2[] = _T("ADDRESS2");
	const _TCHAR kAddress3[] = _T("ADDRESS3");
	const _TCHAR kSourceCd[] = _T("SOURCE_CD");
	const _TCHAR kPtn[] = _T("PTN_F");
	const _TCHAR kOrderCd[] = _T("ORDER_CD");
	const _TCHAR kJudgCd[] = _T("JUDG_CD");
	const _TCHAR kGeoSf[] = _T("GEO_SF");
	const _TCHAR kIpcSf[] = _T("IPC_SF");
	const _TCHAR kShinrai[] = _T("SHINRAI_F");
	const _TCHAR kHyoki[] = _T("HYOKI");
	const _TCHAR kAdopt[] = _T("ADOPT_F");
	const _TCHAR kPriority[] = _T("PRIORITY_F");
}

// 整備用字ポリゴン
namespace gs_addr_polygon
{
	const _TCHAR kTableName[] = _T("ADDR_POLYGON");

	const _TCHAR kCityCode[] = _T("CITYCODE");
	const _TCHAR kAddrCode1[] = _T("ADDRCODE1");
	const _TCHAR kKCode1[] = _T("K_CODE1");
	const _TCHAR kAddrCode2[] = _T("ADDRCODE2");
	const _TCHAR kKCode2[] = _T("K_CODE2");
	const _TCHAR kGaikuFugo[] = _T("GAIKUFUGO");
	const _TCHAR kAddrClass[] = _T("ADDRCLASS");
}

} // schema

} // sindy