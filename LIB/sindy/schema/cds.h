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

#ifndef SINDY_SCHEMA_CDS_H_
#define SINDY_SCHEMA_CDS_H_

#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

namespace city_annotation_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kNameString1[];		//!< ’‹L•¶Žš—ñ1
	extern const _TCHAR kNameString2[];		//!< ’‹L•¶Žš—ñ2
	extern const _TCHAR kStringNum1[];		//!< ’‹L1•¶Žš”
	extern const _TCHAR kStringNum2[];		//!< ’‹L2•¶Žš”
	extern const _TCHAR kAnnoClass[];		//!< ’‹LŽí•Ê
	extern const _TCHAR kNameYomi[];		//!< ’‹L“Ç‚Ý
	extern const _TCHAR kEngDefault[];		//!< ƒfƒtƒHƒ‹ƒg‰pŒê’‹L
	extern const _TCHAR kEngName[];			//!< ŒÂ•Ê‰pŒê’‹L
	extern const _TCHAR kEngOut[];			//!< ‰pŒê’‹Lo—Íƒtƒ‰ƒO
	extern const _TCHAR kDispType[];		//!< •\Ž¦ƒ^ƒCƒv
	extern const _TCHAR kHimawari[];		//!< ‚Ð‚Ü‚í‚èƒtƒ‰ƒO
	extern const _TCHAR kContentsCode[];	//!< ƒRƒ“ƒeƒ“ƒcƒR[ƒh
	extern const _TCHAR kContentsSeq[];		//!< ƒRƒ“ƒeƒ“ƒcƒV[ƒPƒ“ƒX
	extern const _TCHAR kAnnoDisp[];		//!< ’‹L•\Ž¦ƒR[ƒh

	//// CDS’‹LŽí•ÊƒR[ƒh
	//namespace annotation_code {
	//	enum ECode {
	//	};
	//} // annotation_code

	// CDS’‹L•\Ž¦ƒ^ƒCƒv
	namespace disp_type {
		enum ECode {
			kStr	= 1,	//!< •¶Žš—ñ‚Ì‚Ý
			kSymStr	= 2,	//!< ‹L†{•¶Žš—ñ
		};
	} // disp_type

} // city_annotation_gs

namespace building_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kBldClass[];		//!< Œš•¨Ží•Ê
	extern const _TCHAR kCorridor[];		//!< “n‚è˜L‰ºƒtƒ‰ƒO
	extern const _TCHAR kNoWall[];			//!< –³•Çƒtƒ‰ƒO
	extern const _TCHAR kBldName[];			//!< Œš•¨–¼Ì
	extern const _TCHAR kBldAddr[];			//!< Œš•¨ZŠ
	extern const _TCHAR kGeospaceID[];		//!< GEOSPACE‰ÆŒ`ID

	// CDS“sŽsŒš•¨Ží•ÊƒR[ƒh
	namespace bld_class {
		enum ECode {
			kNone							=        0, //!< –¢’²¸Eì‹Æ’†
			kNormalRailwayStation			=        1, //!< •’Ê“S“¹‰w(’nã)
			kSubwayStation					=        2, //!< ’n‰º“S‰w(’nã)
			kOtherStationHome				=        3, //!< ’n‰º“SˆÈŠO‰wƒz[ƒ€i’nãj
			kSubwayStationHome				=        4, //!< ’n‰º“S‰wƒz[ƒ€i’nãj
			kGovmentOffice					=        5, //!< Š¯Œö’¡
			kHospital						=        6, //!< •a‰@
			kSchool							=        7, //!< ŠwZ
			KHighRiseComplexHousing			=        8, //!< ‚‘wW‡Z‘î
			kPlant							=        9, //!< HêE‹âsEƒrƒ‹
			kHotel							=       11, //!< ƒzƒeƒ‹
			kDepartmentStore				=       12, //!< ŒâŠyEƒfƒp[ƒg
			kRailroadBuilding 				=       13, //!< ‰wƒrƒ‹
			kMarket							=		16, //!< Žsê
			kFactory						=		17,	//!< Hê
			kStoreHouse						=		18, //!< ‘qŒÉ
			kSurfaceRailwayStation			=		19,	//!< ˜H–Ê“S“¹‰w(’nã)
			kArcade							=		26,	//!< ƒA[ƒP[ƒh(–Ê)
			kTollGate						=		27,	//!< —¿‹àŠ(–Ê)
			kUniversalTrafficStation		=		28,	//!< VŒð’Ê‰w(’nã)
			kMonorailStation				=		29,	//!< ƒ‚ƒmƒŒ[ƒ‹‰w(’nã)
			kCableCarStation				=		30,	//!< ƒP[ƒuƒ‹ƒJ[‰w(’nã)
			kForce							=		31,	//!< Ž©‰q‘àE•ÄŒR
			kGasStation						=		32, //!< ƒKƒ\ƒŠƒ“ƒXƒ^ƒ“ƒh
			kConfirmationTenantAttr			=		99, //!< ƒeƒiƒ“ƒg‘®«Šm”F
		};
	} // bld_class

} // building_gs

namespace city_line_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kBgClass[];			//!< ”wŒiƒ‰ƒCƒ“Ží•Ê

	// CDS“sŽs”wŒiƒ‰ƒCƒ“Ží•ÊƒR[ƒh
	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< –¢’²¸Eì‹Æ’†
			kDashe		 				=        2, //!< ”jü•\Ž¦
			kWatergate 					=        4, //!< …–å
			kErosionControlDam			=		 5,	//!< »–hƒ_ƒ€
			kStairsDecoration			=		15,	//!< ŠK’i‘•üü
			kDivider					=		20,	//!< ’†‰›•ª—£‘Ñiƒ‰ƒCƒ“j
			kRoadSite					=		21,	//!< “¹˜H”wŒi
			kContour					=      101, //!< “™‚ü
			kPrefecturesBorder			=      111, //!< “s“¹•{Œ§ŠE
			kMunicipalityBorder			=      112, //!< Žs’¬‘ºŠE
			kWardBorder					=      113, //!< ‹æŠE
			kOazaBorder					=      114, //!< ‘åŽšŠE
			kKoazaBorder				=      115, //!< ü¬ŽšŠE
			kRiver						=      121, //!< ‰Íì
			kLake                       =      122, //!< ü’rEŒÎ
			kCoastline                  =      123, //!< ŠCŠÝü
			kLineRoad                   =      131, //!< ü“¹˜H
			kGardenPath                 =      132, //!< ’ë‰€˜H
			kFootBridge                 =      133, //!< •à“¹‹´
			kTank                       =      141, //!< ƒ^ƒ“ƒN
		};
	} // bg_class

} // city_line_gs

namespace city_railway_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kRailwayClass[];	//!< “S“¹Ží•Ê
	extern const _TCHAR kUnderground[];		//!< ’n‰ºƒtƒ‰ƒO

	// CDS“sŽs“S“¹Ží•ÊƒR[ƒh
	namespace railway_class {
		enum ECode {
			kNone						=         0, //!< –¢’²¸Eì‹Æ’†
			kNormalRailwayStation		=         1, //!< •’Ê“S“¹‰w(’nã)
			kSubway 					=         2, //!< ’n‰º“S
			kSurfaceRailway			    =		  3, //!< ˜H–Ê“S“¹
			kSpecialOrbit		    	=		101, //!< “ÁŽê‹O“¹
		};
	} // railway_class

} // city_railway_gs

namespace city_site_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kBgClass[];			//!< ”wŒiŽí•Ê

	// CDS“sŽs”wŒiƒ|ƒŠƒSƒ“Ží•ÊƒR[ƒh
	namespace bg_class {
		enum ECode {
			kNone								=         0, //!< –¢’²¸Eì‹Æ’†
			kPark								=        14, //!< AÍ’n
			kRiver								=		 20, //!< ‰Íì
			kSea								=		 23, //!< ŠC
			kLake								=		 24, //!< ’rEŒÎ
			kInterCityTollExpressway			=		 30, //!< “sŽsŠÔ‚‘¬(—L—¿)
			kNationalHighway					=        31, //!< ‘“¹
			kPrincipalPrefecturalRoad 			=        32, //!< Žå—v’n•û“¹
			kPrefectureNormalRoad				=		 33, //!< ˆê”Ê“s“¹•{Œ§“¹ 
			kSideway							=        34, //!< •à“¹
			kGardenPath							=        35, //!< ’ë‰€˜H
			kCenterDivider						=        37, //!< ’†‰›•ª—£‘Ñ(•W€)
			kInterCityFreeExpressway			=		 38, //!< “sŽsŠÔ‚‘¬(–³—¿)
			kOtherSite							=        51, //!< •Ü‘•’n‚»‚Ì‘¼•~’n
			kUrbanTollExpressway				=		 70, //!< “sŽs‚‘¬(—L—¿)
			kUrbanFreeExpressway				=		 71, //!< “sŽs‚‘¬(–³—¿)
			kTollRoadPolygon					=		 72, //!< —L—¿“¹˜Hƒ|ƒŠƒSƒ“
			kOtherArterialRoad					=		 73, //!< ‚»‚Ì‘¼Š²ü“¹˜H
			kOtherRoad							=		 74, //!< ‚»‚Ì‘¼“¹
			kHighwayPolygon						=		101, //!< ‚‘¬“¹˜Hƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
			kUrbanExpresswayPolygon				=		111, //!< “sŽs‚‘¬“¹˜Hƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
			kNationalHighwayPolygon				=		121, //!< ‘“¹ƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
			kPrincipalPrefecturalRoadPolygon	=		131, //!< Žå—v’n•û“¹ƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
			kPrefectureNormalRoadPolygon		=		141, //!< ˆê”Ê“s“¹•{Œ§“¹ƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
			kSidewayPolygon						=		161, //!< •à“¹ƒ|ƒŠƒSƒ“i‘S“¹˜Hj
			kTollRoadPolygonCross				=		171, //!< —L—¿“¹˜Hƒ|ƒŠƒSƒ“i—§‘ÌŒð·j
		};
	} // bg_class

} // city_site_gs

namespace land_cover_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

} // land_cover_gs

namespace underground_area_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

} // underground_area_gs

namespace bld_tenant_gs {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];		//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kGeospaceID[];		//!< ‰Æ‰®Œ`ó’n•¨ID
	extern const _TCHAR kLClass[];			//!< ‘å•ª—ÞƒR[ƒh
	extern const _TCHAR kMClass[];			//!< ’†•ª—ÞƒR[ƒh
	extern const _TCHAR kSClass[];			//!< ¬•ª—ÞƒR[ƒh
	extern const _TCHAR kPopularName[];		//!< ’ÊÌ–¼Ì
	extern const _TCHAR kSignageName[];		//!< •\‹L–¼Ì
	extern const _TCHAR kNameKana[];		//!< ƒJƒi–¼Ì
	extern const _TCHAR kIconClass[];		//!< ƒAƒCƒRƒ“Ží•Ê
	extern const _TCHAR kReserved[];		//!< reserved
	extern const _TCHAR kAddrCode11[];		//!< ZŠƒR[ƒh11Œ…
	extern const _TCHAR kPrefKanji[];		//!< Š¿ŽšŒ§–¼
	extern const _TCHAR kCityKanji[];		//!< Š¿ŽšŽs’¬‘º–¼
	extern const _TCHAR kOazaKanji[];		//!< Š¿Žš‘åŽš–¼
	extern const _TCHAR kAzaKanji[];		//!< Š¿ŽšŽšE’š–Ú
	extern const _TCHAR kBlockKanji[];		//!< ”Ô’n
	extern const _TCHAR kGoKanji[];			//!< †
	extern const _TCHAR kBranchNum[];		//!< Ž}”Ô
	extern const _TCHAR kPrefKana[];		//!< ”¼ŠpƒJƒiŒ§–¼
	extern const _TCHAR kCityKana[];		//!< ”¼ŠpƒJƒiŽs’¬‘º–¼
	extern const _TCHAR kOazaKana[];		//!< ”¼ŠpƒJƒi‘åŽš–¼
	extern const _TCHAR kAzaKana[];			//!< ”¼ŠpƒJƒiŽšE’š–Ú
	extern const _TCHAR kPostNum[];			//!< —X•Ö”Ô†

} // bld_tenant_gs

/// “sŽs’n}‰wŽÉƒ|ƒŠƒSƒ“ƒe[ƒuƒ‹
namespace city_station_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];			//!< ƒe[ƒuƒ‹–¼

	extern const _TCHAR kUnderGround[];			//!< ’n‰ºƒtƒ‰ƒO
	extern const _TCHAR kStationClass[];		//!< “S“¹‰wŽí•ÊƒR[ƒh

	namespace station_class {
		enum ECode {
			kNone						=        0, //!< –¢’²¸Eì‹Æ’†
			kOtherStation				=        1, //!< ’n‰º“SˆÈŠO‰wi’n‰ºj
			kSubwayStation				=        2, //!< ’n‰º“S‰wi’n‰ºj
			kUniversalTrafficStation	=		 3,	//!< VŒð’ÊƒVƒXƒeƒ€i’n‰ºj
			kMonorailStation			=		 4,	//!< ƒ‚ƒmƒŒ[ƒ‹i’n‰ºj
			kCableCarStation			=		 5,	//!< ƒP[ƒuƒ‹ƒJ[i’n‰ºj
		};
	} // station_class
} // city_station_gs

} // schema

} // sindy

#endif // SINDY_SCHEMA_CDS_H_
