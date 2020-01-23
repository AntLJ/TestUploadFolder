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
#include <boost/preprocessor.hpp>
#include <sindy/schema/globalshp.h>


namespace sindy {

namespace schema {

namespace globalshp{

namespace poi_info {
	const _TCHAR kTableName[]   = _T("POI_INFO");	//!< テーブル名
	const _TCHAR kFID[]         = _T("FID");		//!< FID
	const _TCHAR kShape[]       = _T("SHAPE");

	const _TCHAR kOfficial[]    = _T("Official");
	const _TCHAR kOnTrans[]     = _T("OnTrans");
	const _TCHAR kAlter[]       = _T("Alter");
	const _TCHAR kAnTrans[]     = _T("AnTrans");
	const _TCHAR kCategory[]    = _T("Category");
	const _TCHAR kSubCategory[] = _T("SubCat");
	const _TCHAR kErrCat[]      = _T("ErrCat");
	const _TCHAR kMainPhone[]   = _T("MainPhone");
	const _TCHAR kMainPhoneType[]  = _T("MainPhoneC");

#define DECL(z, n, data)                                              \
	const _TCHAR kSubPhoneType##n[] = _T("SubPhone") _T(#n) _T("C"); \
	const _TCHAR kSubPhone##n[]     = _T("SubPhone") _T(#n);
	BOOST_PP_REPEAT_FROM_TO(1, 6, DECL, _)
#undef DECL
	const LPCTSTR kSubPhoneType[5] = { BOOST_PP_ENUM_SHIFTED_PARAMS(5, kSubPhoneType) };
	const LPCTSTR kSubPhone[5]     = { BOOST_PP_ENUM_SHIFTED_PARAMS(5, kSubPhone) };

	const _TCHAR kSubdist[]     = _T("Subdist");
	const _TCHAR kRoadName[]    = _T("RoadName");
	const _TCHAR kRnTrans[]     = _T("RnTrans");
	const _TCHAR kHouseNum[]    = _T("HouseNum");
	const _TCHAR kActAddr1[]    = _T("ActAddr1");
	const _TCHAR kActAddr2[]    = _T("ActAddr2");
	const _TCHAR kAccuracyC[]   = _T("AccuracyC");
	const _TCHAR kDelCode[]     = _T("DelCode");
	const _TCHAR kChainCode[]   = _T("ChainCode");
	const _TCHAR kFuelType[]    = _T("FuelType");
	const _TCHAR kOrgOID[]      = _T("OrgOID");
	const _TCHAR kWorkResult[]  = _T("WorkResult");
	const _TCHAR kWww[]         = _T("www");
	const _TCHAR kQcDate[]      = _T("Qcdate");
	const _TCHAR kSource[]		= _T("SOURCE");
	const _TCHAR kLanguageC[]	= _T("LANGUAGE_C");
	const _TCHAR kLangTrns[]	= _T("LANG_TRNS");
	const _TCHAR kProductC[]	= _T("PRODUCT_C");
	const _TCHAR kDetlInfo[]	= _T("DETLINFO");
} // poi_info

namespace poi_entrypoint {
	const _TCHAR kTableName[]   = _T("POI_ENTRYPOINT");	//!< テーブル名
	const _TCHAR kFID[]         = _T("FID");		//!< FID
	const _TCHAR kShape[]       = _T("SHAPE");

	const _TCHAR kPOIInfoFID[]  = _T("POIInfoFID");
	const _TCHAR kPriorityF[]   = _T("PriorityF");
	const _TCHAR kAccuracyC[]   = _T("AccuracyC");
	const _TCHAR kEntryC[]      = _T("EntryC");
	const _TCHAR kOrgOID[]      = _T("OrgOID");
	const _TCHAR kOrgPOIInfo[]  = _T("OrgPOIInfo");
	const _TCHAR kQcDate[]      = _T("Qcdate");
} // poi_entrypoint

namespace aoi {
	const _TCHAR kTableName[]   = _T("AOI");	//!< テーブル名
	const _TCHAR kFID[]         = _T("FID");		//!< FID
	const _TCHAR kShape[]       = _T("SHAPE");

	const _TCHAR kLayerName[]	= _T("LayerName");
	const _TCHAR kOrgOID[]      = _T("OrgOID");
	const _TCHAR kName[]		= _T("Name");
	const _TCHAR kOnTrans[]		= _T("OnTrans");
} // aoi


}

}

}
