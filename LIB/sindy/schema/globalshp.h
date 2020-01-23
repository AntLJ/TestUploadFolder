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

#ifndef SINDY_SCHEMA_GLOBAL_SHAPE_H_
#define SINDY_SCHEMA_GLOBAL_SHAPE_H_

#include <iostream>

namespace sindy {

namespace schema {

namespace globalshp{

namespace poi_info{

	extern const _TCHAR kTableName[];
	extern const _TCHAR kFID[];
	extern const _TCHAR kShape[];

	extern const _TCHAR kOfficial[];
	extern const _TCHAR kOnTrans[];
	extern const _TCHAR kAlter[];
	extern const _TCHAR kAnTrans[];
	extern const _TCHAR kCategory[];
	extern const _TCHAR kSubCategory[];
	extern const _TCHAR kErrCat[];
	extern const _TCHAR kMainPhone[];
	extern const _TCHAR kMainPhoneType[];
	extern const _TCHAR kSubPhone1[];
	extern const _TCHAR kSubPhoneType1[];
	extern const _TCHAR kSubPhone2[];
	extern const _TCHAR kSubPhoneType2[];
	extern const _TCHAR kSubPhone3[];
	extern const _TCHAR kSubPhoneType3[];
	extern const _TCHAR kSubPhone4[];
	extern const _TCHAR kSubPhoneType4[];
	extern const _TCHAR kSubPhone5[];
	extern const _TCHAR kSubPhoneType5[];
	extern const _TCHAR kSubdist[];
	extern const _TCHAR kRoadName[];
	extern const _TCHAR kRnTrans[];
	extern const _TCHAR kHouseNum[];
	extern const _TCHAR kActAddr1[];
	extern const _TCHAR kActAddr2[];
	extern const _TCHAR kAccuracyC[];
	extern const _TCHAR kDelCode[];
	extern const _TCHAR kChainCode[];
	extern const _TCHAR kFuelType[];
	extern const _TCHAR kOrgOID[];
	extern const _TCHAR kWorkResult[];
	extern const _TCHAR kWww[];
	extern const _TCHAR kQcDate[];
	extern const _TCHAR kSource[];
	extern const _TCHAR kLanguageC[];
	extern const _TCHAR kLangTrns[];
	extern const _TCHAR kProductC[];
	extern const _TCHAR kDetlInfo[];

	// TODO: 各フィールドの列幅をglobalshp内で定義する。
}

namespace poi_entrypoint{

	extern const _TCHAR kTableName[];
	extern const _TCHAR kFID[];
	extern const _TCHAR kShape[];

	extern const _TCHAR kPOIInfoFID[];
	extern const _TCHAR kPriorityF[];
	extern const _TCHAR kAccuracyC[];
	extern const _TCHAR kEntryC[];
	extern const _TCHAR kOrgOID[];
	extern const _TCHAR kOrgPOIInfo[];
	extern const _TCHAR kQcDate[];

	// TODO: 各フィールドの列幅をglobalshp内で定義する。
}

namespace aoi{
	extern const _TCHAR kTableName[];
	extern const _TCHAR kFID[];
	extern const _TCHAR kShape[];
	
	extern const _TCHAR kOrgOID[];
	extern const _TCHAR kLayerName[];
	extern const _TCHAR kName[];
	extern const _TCHAR kOnTrans[];

	namespace columnlength
	{
		const long kLayerName = 254;
		const long kOrgOID = 10;
		const long kName = 254;
		const long kOnTrans = 254;
	}
}

}

}

}
#endif
