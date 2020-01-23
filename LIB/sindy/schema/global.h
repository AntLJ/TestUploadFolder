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

#ifndef SINDY_SCHEMA_GLOBAL_H_
#define SINDY_SCHEMA_GLOBAL_H_

#include <iostream>

namespace sindy {

namespace schema {

namespace global{

namespace middlemesh {
	extern const _TCHAR kTableName[];      //!< �e�[�u����
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< ���b�V���R�[�h
	extern const _TCHAR kShape[];
} // middlemesh

namespace basemesh {
	extern const _TCHAR kTableName[];      //!< �e�[�u����
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< ���b�V���R�[�h
	extern const _TCHAR kShape[];
} // basemesh

namespace citymesh {
	extern const _TCHAR kTableName[];      //!< �e�[�u����
	extern const _TCHAR kObjectID[];       //!< ObjectID
	extern const _TCHAR kMeshCode[];       //!< ���b�V���R�[�h
	extern const _TCHAR kCreateYear[];     //!< �쐬�N�x
	extern const _TCHAR kModifyYear[];     //!< �X�V�N�x
	extern const _TCHAR kCompleteF[];      //!< ���S���t���O
	extern const _TCHAR kShape[];
} // citymesh

namespace object {
	extern const _TCHAR kObjectID[]; ///< �I�u�W�F�N�gID
} // object

namespace common_table {
	using namespace ::sindy::schema::global::object;
	extern const _TCHAR kOperator[];       //!< �ŏI�X�V��
	extern const _TCHAR kPurpose[];        //!< ��ƖړI
	extern const _TCHAR kModifyDate[];     //!< �ŏI�X�V����
	extern const _TCHAR kUpdateType[];     //!< �ŏI�X�V���e
	extern const _TCHAR kProgModifyDate[]; //!< �ŏI�v���O�����X�V����
	extern const _TCHAR kModifyProgName[]; //!< �ŏI�X�V�v���O������
	extern const _TCHAR kUserClaim[];      //!< ���[�U�N���[���t���O
	extern const _TCHAR kSource[];         //!< ���\�[�X

	namespace gl_chg_code {
		enum ECode {
			kExistData   = 0,              //!< ����
			kNew         = 1,              //!< �V�K�쐬
			kDelete      = 2,              //!< �폜
			kShapeModify = 3,              //!< �`��ύX
			kAttrModify  = 4,              //!< �����ύX
			kWorkImport  = 5,              //!< ��ƃC���|�[�g
			kDataImport  = 6,              //!< �f�[�^�C���|�[�g
		};
	} // gl_chg_code

	namespace gl_layer_code{
		enum ECode{
			kBuilding			= 0,
			kCityStation		= 1,
			kBaseLine			= 2,
			kBaseRailway		= 3,
			kLandUse			= 4,
			kBaseStation		= 5,
			kWaterway			= 6,
			kLandCover			= 7,
			kIsland				= 8,
			kRoadLink			= 9,
			kCountry			= 10,
			kProvince			= 11,
			kDistrict			= 12,
			kSubDistrict		= 13,
			kNamedArea			= 14,
			kPoiInfo			= 15,
			kWaterwayLine		= 16,
			kRoadNode			= 17,
			kSegmentAttr		= 18,
			kBuiltUpArea		= 19,
			kTopCountry			= 20,
			kTopProvince		= 21,
			kTopWaterwayLine	= 22,
			kTopWaterwayPolygon	= 23,
			kTopAnnotation		= 24,
			kVillage            = 25,
			kSeaPolygon			= 26,
			kBuildingRoof		= 27,
			kCityDivider		= 28,
			kCityLine			= 29,
			kCityRailWay		= 30,
			kCitySite			= 31,
			kCityTunnel			= 32,
			kPoiInfoRoadNameID	= 33,
			kHnpRoadNameID		= 34,
			kSeaPolygonName		= 35,
			kPoiEntryPoint		= 36,
			kAnnoRoadName		= 1000,
			kAnnoRoadNo			= 1001,
		};
	} // gl_layer_code

	namespace gl_country_code {
		enum ECode {
			kUndefined						= 0,
			kAustralia						= 36,
			kBangladesh						= 50,
			kBhutan							= 64,
			kBruneiDarussalam				= 96,
			kMyanmar						= 104,
			kCambodia						= 116,
			kChina							= 156,
			kTaiwan							= 158,
			kHongKong						= 344,
			kIndia							= 356,
			kIndonesia						= 360,
			kJapan							= 392,
			kLaos							= 418,
			kMacao							= 446,
			kMalaysia						= 458,
			kFederatedStatesofMicronesia	= 583,
			kPalau							= 585,
			kPapuaNewGuinea					= 598,
			kPhilippines					= 608,
			kTimorLeste						= 626,
			kSingapore						= 702,
			kVietnam						= 704,
			kThailand						= 764,
		};
	} // gl_country_code

} // common_table

namespace country_table {
	using namespace ::sindy::schema::global::common_table;

	extern const _TCHAR kCountryCode[];	//!< ���R�[�h�i����DB��p�j
} // contry_table

namespace common_feature {
	using namespace ::sindy::schema::global::common_table;

	extern const _TCHAR kShape[];        //!< SHAPE
} // common_feature

namespace country_feature {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kShape[];        //!< SHAPE
} // country_feature

namespace scaledisp_flag {
	extern const _TCHAR kSC1Disp[];      //!< �X�P�[��1�\���t���O
	extern const _TCHAR kSC2Disp[];      //!< �X�P�[��2�\���t���O
	extern const _TCHAR kSC3Disp[];      //!< �X�P�[��3�\���t���O
	extern const _TCHAR kSC4Disp[];      //!< �X�P�[��4�\���t���O
} // scaledisp_flag

namespace scale_class {
	extern const _TCHAR kSC1BgClass[];   //!< �X�P�[��1�w�i��ʃR�[�h
	extern const _TCHAR kSC2BgClass[];   //!< �X�P�[��2�w�i��ʃR�[�h
	extern const _TCHAR kSC3BgClass[];   //!< �X�P�[��3�w�i��ʃR�[�h
	extern const _TCHAR kSC4BgClass[];   //!< �X�P�[��4�w�i��ʃR�[�h
} // scale_class

namespace name_class {
	extern const _TCHAR kNameID[];       //!< ����ID
} // name_class

// �����|���S���e�[�u��
namespace building {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kBldClass[];     //!< ������ʃR�[�h
	extern const _TCHAR kCorridor[];     //!< �n��L���t���O
	extern const _TCHAR kNoWall[];       //!< ���ǎɃt���O
	extern const _TCHAR kGeospaceID[];   //!< GEOSPACE_ID

	namespace gl_bld_class {
		enum ECode {
			kUndefined					= 0,     //!< ����`
			kTrain						= 1,     //!< �n���S�ȊO�w�i�n��j
			kSubway						= 2,     //!< �n���S�w�i�n��j
			kGovernmentPublic			= 5,     //!< ������
			kHospital					= 6,     //!< �a�@
			kSchool						= 7,     //!< �w�Z
			kResidential				= 8,     //!< �Z��
			kOtherFacility				= 9,     //!< ���̑��{��
			kHotel						= 11,    //!< �z�e��
			kPedestrianDeck				= 15,    //!< ���s�җp�f�b�L
			kIndustrialBusiness			= 17,    //!< �H��
			kTramwayStation				= 19,    //!< �H�ʓS���w(�n��)
			kFootbridge					= 20,    //!< ������
			kStairsPedestrianDeck		= 23,    //!< �؃f�X�g���A���f�b�L�i�K�i�E�G�X�J���[�^�j
			kStairsInsideThePremises	= 25,    //!< �~�n�K�i
			kTollGate					= 27,    //!< ������
			kNewTransportSystem			= 28,    //!< �V��ʉw�i�n��j
			kMonorailStation			= 29,    //!< ���m���[���w�i�n��j
			kCablecarStation			= 30,    //!< �P�[�u���J�[�w�i�n��j
			kTransportation				= 100,   //!< �^��
			kCulturalLeisure			= 101,   //!< �������W���[
			kCommercial					= 102,   //!< ����
			kReligious					= 103,   //!< �@��
			kBank						= 104,   //!< ��s
		};
	} // bld_class
} // building

// �����i���|���S���e�[�u��
namespace building_step {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kFloors[];       //!< �K��
	extern const _TCHAR kCheck[];        //!< �K�����m�F�t���O
	extern const _TCHAR kBuildingID[];   //!< ����ID
} // building_step

// ��������{�݃|���S���e�[�u��
namespace building_roof {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kRfClassC[];     //!< ��������{�ݎ�ʃR�[�h

	namespace gl_bldroof_class {
		enum ECode {
			kUndefined		= 0,	//!< ����`
			kPravedArea		= 1,	//!< �ܑ��n���̑��~�n�i����{�݁j
			kGreenUrbanArea = 2,	//!< �A�͒n�i����{�݁j
			kPool			= 3,	//!< �v�[���i����{�݁j
		};
	} // gl_bldroof_class

} // building_roof

  // �������C���e�[�u��
namespace building_line {
	using namespace sindy::schema::global::common_feature;
	using namespace sindy::schema::global::building::gl_bld_class;
	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kBldClass[];     //!< ������ʃR�[�h
} // building_line


// �s�s�n�}�w�i�|���S���e�[�u��
namespace city_site {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kBgClass[];      //!< �w�i�|���S����ʃR�[�h

	namespace gl_bgpolygon_class{
		enum ECode {
			kUndefined				= 0,	//!< ����`
			kUnspecified			= 1,	//!< �X��
			kSandySoil				= 10,	//!< ���n
			kSwamp					= 11,	//!< ���n
			kForest					= 12,	//!< �R��
			kGrass					= 13,	//!< �Ő������̑��Βn
			kGreenUrbanArea			= 14,	//!< �A�͒n
			kRiver					= 20,	//!< �͐�
			kRiverside				= 21,	//!< ��O�n
			kPool					= 22,	//!< �v�[��
			kOcean					= 23,	//!< �C
			kPondOrLake				= 24,	//!< �r�E��
			kExpressway				= 30,	//!< �s�s�ԍ������H�i�L���j
			kMainRoad				= 31,	//!< �ŏd�v���H
			kSidewalk				= 34,	//!< ����
			kPathway				= 35,	//!< ��ʏd�v���H
			kDividerNomal			= 37,	//!< ���������сi�W���j
			kStairs					= 39,	//!< �����K�i
			kHospital				= 40,	//!< �a�@
			kSchool					= 41,	//!< �w�Z
			kFactory				= 42,	//!< �H��
			kDam					= 45,	//!< �_��
			kRailwayGround			= 50,	//!< �S���~�n
			kPavedArea				= 51,	//!< �ܑ��n���̑��~�n
			kBreakwater				= 52,	//!< �h�g��
			kMilitaryGround			= 55,	//!< �R�p�n
			kIncompleteArea			= 60,	//!< �s���S�G���A
			kDividerGreen			= 75,	//!< ���������сi�A�͒n�j
			kRunway					= 76,	//!< �����H
			kUnderConstructionRoad	= 77,	//!< �H�������H
			kOrdinaryRoad			= 79,	//!< �׊X�H
		};
	} // gl_bgpolygon_class
} // city_site

// �s�s�n�}�w�i���C���e�[�u��
namespace city_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBgClass[];				//!< �w�i���C����ʃR�[�h

	namespace gl_bgline_class {
		enum ECode {
			kUndefined					=        0, //!< �������E��ƒ�
			kDecorative					=        1, //!< ������
			kTunnel		 				=        2, //!< �g���l��
			kTollGate					=        3, //!< ������
			kFloodGate 					=        4, //!< ����
			kFerryRoute 				=       10, //!< �t�F���[�q�H
			kOtherPassengerBoatRoute	=       11, //!< ���̑����q�D�q�H
			kStairsDecorative			=		15,	//!< �K�i������
			kInitial					=		16, //!< �e��
			kStairsTread				=		17, //!< �K�i���ʃ��C���i�����K�i�j
			kDivider					=		20,	//!< ���������сi���C���j
			kRoadBackground				=		21,	//!< ���H�w�i
			kExpressway					=		22,	//!< �������H
			kRoadOverpass				=		23,	//!< ���H����
			kRailwayOverpass			=		24,	//!< �S������
			kVehicleStopBarrier			=		25, //!< �Ԏ~�߃��C��
		};
	} // gl_bgline_class
} // city_line

// �s�s�g���l���B��
namespace city_tunnel {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kTunnelClass[];			//!< �g���l����ʃR�[�h
} // city_tunnel

// �s�s�S�����C���e�[�u��
namespace city_railway {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kRailWayClassC[];		//!< �S�����

	namespace gl_ug_railway_class {
		enum ECode {
			kUndefined				=        0, //!< �������E��ƒ�
			kTrain					=        1, //!< ���ʓS��
			kMetroSubway			=        2, //!< �n���S
			kTramway				=        3, //!< �H�ʓS��
			kRollercoaster 			=        4, //!< �W�F�b�g�R�[�X�^�[��
			kNewTransportSystem		=        5, //!< �V��ʃV�X�e��
			kMonorail				=        6, //!< ���m���[��
			Cablecar				=        7, //!< �P�[�u���J�[
		};
	} // gl_ug_railway_class
} // city_railway

// �s�s�����������у|���S���e�[�u��
namespace city_divider {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kDividerClassC[];		//!< �����������ю�ʃR�[�h

} // city_divider

// �s�s�n�}�w�Ƀ|���S���e�[�u��
namespace city_station {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kUnderGround[];  //!< �n���t���O
	extern const _TCHAR kStationClass[]; //!< �S���w��ʃR�[�h

	namespace gl_ug_station_class {
		enum ECode {
			kUndefined   = 0,            //!< ����`
			kNotSubwayUG = 1,            //!< �n���S�ȊO�w
			kSubwayUG    = 2,            //!< �n���S�w
		};
	} // gl_ug_station_class
} // city_station

/// ���k�w�i���C���e�[�u��
namespace base_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;
	
	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kUnderGround[];  //!< �n���t���O
	extern const _TCHAR kBgClass[];      //!< �w�i���C����ʃR�[�h

	namespace gl_bgline_class {
		enum ECode {
			kUndefined    = 0,           //!< ����`
			kNoDisplay    = 1,           //!< ��\��
			kRiver        = 2,           //!< �͐�
			kOcean        = 3,           //!< �C
			kCanal        = 4,           //!< ���H
			kLake         = 5,           //!< �r
			kWaterUnknown = 9,           //!< �s�����n
		};
	} // gl_bgline_class
} // base_line

/// ���k�w�i�S�����C���e�[�u��
namespace base_railway {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kUnderGround[];  //!< �n���t���O
	extern const _TCHAR kRailwayClass[]; //!< �S����ʃR�[�h

	namespace gl_railway_class {
		enum ECode {
			kUndefined          = 0,     //!< ����`
			kTrain              = 1,     //!< ���ʓS��
			kMetroSubway        = 2,     //!< �n���S
			kNewTransportSystem = 3,     //!< �V���
		};
	} // gl_railway_class
} // base_railway

/// LAND USE�|���S���e�[�u��
namespace land_use {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	namespace gl_use_class {
		enum ECode {
			kUndefined          = 0,      //!< ����`
			kNoDisplay          = 1,      //!< ��\��
			kLC7110             = 100,    //!< LC7110(MPA��p)
			kPavedArea          = 201001, //!< �ܑ��H
			kRailwayGround      = 202001, //!< �S���~�n
			kAirportGround      = 203002, //!< ��`�~�n
			kAirportRunway      = 203006, //!< �����H
			kMilitaryGround     = 303002, //!< �R���{��
			kPublicFacility     = 304003, //!< �����{��
			kSchoolGround       = 401010, //!< �w�Z
			kHospitalGround     = 402001, //!< �a�@
			kPark               = 501001, //!< ����
			kRaceCourse         = 501013, //!< ���[�X��
			kLeisureFacility    = 501019, //!< ��y�{��
			kCommercialFacility = 502005, //!< ���Ǝ{��
			kFootballStadium    = 503001, //!< �T�b�J�[��
			kGolfCourseGround   = 503007, //!< �S���t�R�[�X
			kGolfDrivingRange   = 503011, //!< �S���t���K��
			kSportsFacility     = 503021, //!< ���̑��X�|�[�c�{��
			kFarm               = 504005, //!< �_��
			kIndustrialBusiness = 601001, //!< �H��
			kCemetery           = 603004, //!< ��n
			kReligiousFacility  = 603005, //!< �@���{��
			kOtherFacility      = 604004, //!< ���̑��{��
			kHotel              = 701001, //!< �z�e��
			kResidential        = 701101, //!< �Z��
		};
	} // gl_use_class
} // land_use

/// ���k�w�i�w�Ƀ|���S���e�[�u��
namespace base_station {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scaledisp_flag;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kUnderGround[];   //!< �n���t���O
	extern const _TCHAR kStationClass[];  //!< �S���w��ʃR�[�h

	namespace gl_station_class {
		enum ECode {
			kUndefined          = 0,      //!< ����`
			kTrain              = 1,      //!< ���ʓS��
			kMetroSubway        = 2,      //!< �n���S
			kNewTransportSystem = 3,      //!< �V���
		};
	} // gl_station_class
} // base_station

/// WATER WAY�|���S���e�[�u��
namespace water_way {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	namespace gl_water_class {
		enum ECode {
			kUndefined  = 0,              //!< ����`
			kNoDisplay  = 1,              //!< ��\��
			kOcean		= 101001,		  //!< �C�m
			kRiver      = 101002,         //!< �͐�
			kLake       = 101003,         //!< �r
			kCanal      = 102003,         //!< ���H
			kHarborPort = 203005,         //!< �`
 		};
	} // gl_water_class
} // water_way

/// WATER WAY���C���e�[�u��
namespace waterway_line {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����
	
	extern const _TCHAR kBgClass[];       //!< �w�i���
	namespace gl_bgline_class{
		using namespace ::sindy::schema::global::base_line::gl_bgline_class;
	}
} // waterway_line

/// LAND COVER�|���S���e�[�u��
namespace land_cover {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	namespace gl_cover_class {
		enum ECode {
			kUndefined       = 0,         //!< ����`
			kNoDisplay       = 1,         //!< ��\��
			kSandySoil       = 102001,    //!< ���n
			kWetland         = 102002,    //!< ���n
			kGround          = 102005,    //!< �O���E���h
			kForest          = 102101,    //!< �X��
			kNationalPark    = 102102,    //!< ��������
			kForestPark      = 102103,    //!< �X�ь���
			kWildLifeReserve = 102104,    //!< ���R�ی��
			kNoHuntingArea   = 102105,    //!< �֗�
		};
	} // gl_cover_class
} // land_cover

/// ISLAND�|���S���e�[�u��
namespace island {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::scale_class;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	namespace gl_island_class {
		enum ECode {
			kUndefined = 0,               //!< ����`
			kNoDisplay = 1,               //!< ��\��
			kIsland    = 102201,          //!< ��
		};
	} // gl_island_class
} // island

/// ���b�V���ɂЂ��Â��e�[�u��
namespace mesh_code_table {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kMeshCode[];      //!< ���b�V���R�[�h
} // mesh_code_table

// �n�I�_���m�[�h�ƂЂ��Â��e�[�u��
namespace from_to_node_relational_table {
	extern const _TCHAR kFromNodeID[];    //!< �n�_���m�[�h��OBJECTID
	extern const _TCHAR kToNodeID[];      //!< �I�_���m�[�h��OBJECTID
} // from_to_node_relational_table

// �n�C�E�F�C�̘H���Ɋ֘A�t���e�[�u��
namespace highway_road_relational_table {
	extern const _TCHAR kRoadCode[];      //!< �H���R�[�h
} // highway_road_relational_table

// ����
namespace fieldsurvey_table {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kFieldSurvey[];   //!< ���n�����t���O
	extern const _TCHAR kWorkYear[];      //!< ���n�����N�x
} // fieldsurvey_table

// �����N�ɂЂ��Â��e�[�u��
namespace link_relational_table {
	extern const _TCHAR kLinkID[];        //!< �����NID
} // link_relational_table

/// �����N�Ƃ��̌����ɂЂ��Â��e�[�u��
namespace directional_link_relational_table {
	using namespace ::sindy::schema::global::link_relational_table;

	extern const _TCHAR kLinkDir[];       //!< �����N�����R�[�h

	/// �����N�����R�[�h
	namespace gl_link_dir {
		enum ECode {
			kPositive = 1, //!< ������
			kNegative = 2, //!< �t����
		};
	}
} // directional_link_relational_table

// �����N��
namespace link_queue {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::directional_link_relational_table;

	extern const _TCHAR kInfID[];      //!< �����N��ID
	extern const _TCHAR kSequence[];   //!< �����N�񏇔�
	extern const _TCHAR kLastLink[];   //!< �I�[�����N�t���O
} // link_queue

/// ���ԋK�������e�[�u��
namespace time_condition_table {
	extern const _TCHAR kStartMonth[]; //!< �K������:�J�n��
	extern const _TCHAR kStartDay[];   //!< �K������:�J�n��
	extern const _TCHAR kStartHour[];  //!< �K������:�J�n����
	extern const _TCHAR kStartMin[];   //!< �K������:�J�n��
	extern const _TCHAR kEndMonth[];   //!< �K������;�I����
	extern const _TCHAR kEndDay[];     //!< �K������:�I����
	extern const _TCHAR kEndHour[];    //!< �K������:�I������
	extern const _TCHAR kEndMin[];     //!< �K������:�I����
	extern const _TCHAR kDayOfWeek[];  //!< �K������:�j���w��
} // time_condition_table

// ���H�����N�t�B�[�`���N���X
namespace road_link {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::from_to_node_relational_table;
	using namespace ::sindy::schema::global::highway_road_relational_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];
	
	extern const _TCHAR kRoadClass[];     //!< ���H�\���p���
	extern const _TCHAR kNaviClass[];     //!< ���H�o�H�p���
	extern const _TCHAR kMainLinkClass[]; //!< �����N��ʁi��j
	extern const _TCHAR kAdminCode[];     //!< �s�����R�[�h
	extern const _TCHAR kRoadManager[];   //!< �Ǘ��҃R�[�h
	extern const _TCHAR kRoadWidth[];     //!< ���H�����敪
	extern const _TCHAR kBypass[];        //!< �o�C�p�X�t���O
	extern const _TCHAR kCarOnly[];       //!< �����Ԑ�p���H�t���O
	extern const _TCHAR kWideArea[];      //!< �L�摶�݃t���O
	extern const _TCHAR kIsland[];        //!< �����t���O
	extern const _TCHAR kRoadNo[];        //!< ���H�ԍ�(�����p)
	extern const _TCHAR kRoadNoString[];  //!< ���H�ԍ�(�C�O�p)
	extern const _TCHAR kRoadNoString2[]; //!< ���H�ԍ�2(�C�O�p)
	extern const _TCHAR kUpDownClass[];   //!< �㉺����ʃR�[�h
	extern const _TCHAR kNoPassage[];     //!< �ʍs�֎~��ʃR�[�h
	extern const _TCHAR kOneway[];        //!< ����ʍs��ʃR�[�h
	extern const _TCHAR kKubun[];         //!< ��]�敪�R�[�h
	extern const _TCHAR kTripSpeed[];     //!< ���s���x
	extern const _TCHAR kLimitSpeed[];    //!< �K�����x�R�[�h
	extern const _TCHAR kLaneCount[];     //!< �Ԑ���
	extern const _TCHAR kLaneValid[];     //!< �Б��Ԑ����L���t���O
	extern const _TCHAR kRightChange[];   //!< �Ԑ����E����
	extern const _TCHAR kLeftChange[];    //!< �Ԑ���������
	extern const _TCHAR kAlleyMainte[];   //!< ���׊X�H��ƔN�x���ID
	extern const _TCHAR kNoPassageRF[];   //!< �����t�ʍs�֎~�֌W�t���O
	extern const _TCHAR kOnewayRF[];      //!< �����t����ʍs�֌W�t���O
	extern const _TCHAR kDupliLinkRF[];   //!< �d�p�H�����֌W�t���O
	extern const _TCHAR kTurnRegLQRF[];   //!< �E���܋֎~�����N��֌W�t���O
	extern const _TCHAR kDirGuideLQRF[];  //!< ���ʈē������N��֌W�t���O
	extern const _TCHAR kGuideLQRF[];     //!< �����U�������N��֌W�t���O
	extern const _TCHAR kLaneLQRF[];      //!< ���[����񃊃��N��֌W�t���O
	extern const _TCHAR kVICSLQRF[];      //!< VICS�����N��֌W�t���O
	extern const _TCHAR kBywayLQRF[];     //!< ��������񃊃��N��֌W�t���O
	extern const _TCHAR kRouteLQRF[];     //!< ���[�g��񃊃��N��֌W�t���O
	extern const _TCHAR kUturnLQRF[];     //!< U�^�[���\�ӏ���񃊃��N��֌W�t���O
	extern const _TCHAR kInternaviLQRF[]; //!< �C���^�[�i�rVICS�����N��֌W�t���O
	extern const _TCHAR kIpcVicsLQRF[];   //!< iPC-VICS�����N��֌W�t���O
	extern const _TCHAR kIntersectionLQRF[]; //!< �i�������ʌ����_�����N��֌W�t���O

	extern const _TCHAR kParkingLinkClass[]; //!< ���ԏꃊ���N���
	extern const _TCHAR kFloorLevel[];       //!< ���ԏ�K�w���x��
	extern const _TCHAR kMiddleFloorLevel[]; //!< ���ԏꒆ�K�w���x��

	extern const _TCHAR kLegalSpeed[];    //!< �ō����x�R�[�h
	extern const _TCHAR kLegalSpeedVerified[]; //!< �ō����x�m�F�R�[�h

	extern const _TCHAR kVehicleTurnRegLQRF[]; //!< ��蕨�ʉE���܋֎~�����N��֌W�t���O
	extern const _TCHAR kVehicleRegRF[];       //!< ��蕨�ʋK���t���O

	extern const _TCHAR kFromHNLeft[];    //!< LeftFrom�̃n�E�X�i���o�[
	extern const _TCHAR kToHNLeft[];      //!< LeftTo�̃n�E�X�i���o�[
	extern const _TCHAR kFromHNRight[];   //!< RightFrom�̃n�E�X�i���o�[
	extern const _TCHAR kToHNRight[];     //!< RightTo�̃n�E�X�i���o�[
	extern const _TCHAR kHNSchemeLeft[];  //!< Left�̃n�E�X�i���o�[�X�L�[��
	extern const _TCHAR kHNSchemeRight[]; //!< Right�̃n�E�X�i���o�[�X�L�[��
	extern const _TCHAR kHNType[];        //!< �n�E�X�i���o�[���

	// ���H�\���p��ʃR�[�h
	namespace gl_disp_class {
		enum ECode {
			kHighImportanceMajorRoads = 1,   //!< �ŏd�v���H
			kOtherMajorRoads          = 2,   //!< ���̑��d�v���H
			kSecondaryRoads           = 4,   //!< �T�u���H
			kLocalConnectingRoads     = 5,   //!< ���̑��ڑ����H
			kLocalRoads               = 6,   //!< ���̑����H
			kLocalRoadsMinor          = 7,   //!< ���̑��i�����H
			kNarrowRoads1             = 8,   //!< �׊X�HL1
			kNarrowRoads2             = 9,   //!< �׊X�HL2
			kMotorways                = 11,  //!< �������H
			kTollMotorways            = 101, //!< �������H�i�L���j
			kFerry                    = 202, //!< �t�F���[
			kFerryNonRoutable         = 205, //!< �t�F���[�i�o�H�O�j
			kPrivateRoads             = 302, //!< ����
			kInSiteRoads              = 305, //!< �{�ݓ����H
			kWalkway                  = 601, //!< ����

		};
	} // gl_disp_class

	// ���H�o�H�p��ʃR�[�h
	namespace gl_navi_class {
		enum ECode {
			kNonNavigableRoads        = 0,   //!< �o�H�O���H�iL1�A�������j
			kHighImportanceMajorRoads = 1,   //!< �ŏd�v���C�����H
			kOtherMajorRoads          = 2,   //!< ���̑����C�����H
			kSecondaryRoads           = 4,   //!< �T�u���H
			kLocalConnectingRoads     = 5,   //!< ���̑��ڑ����H
			kLocalRoads               = 6,   //!< ���̑����H
			kNonRoutableRoads         = 99,  //!< �o�H�O���H�iL2�A�����j
			kMotorways                = 101, //!< ����
		};
	} // gl_navi_class

	// �����N��ʃR�[�h
	namespace gl_link_class {
		enum ECode {
			kUndefined          = 0,   //!< ����`
			kMainSingleRoadLink = 1,   //!< �{���i�㉺���񕪗��j�����N
			kMainMultiRoadLink  = 2,   //!< �{���i�㉺�����j�����N
			kJunctionLink       = 3,   //!< �W�����N�V���������N
			kIntersectionLink   = 4,   //!< �����_�������N
			kRampLink           = 5,   //!< �A���H�i�����v�j�����N
			kFrontageRoadLink   = 6,   //!< ���������N
			kSAetcFrontageLink  = 7,   //!< �r�`�ȂǑ��������N
			kConnectionLink     = 9,   //!< �ڑ������N
			kRoundAbout         = 101, //!< ���E���h�A�o�E�g
			kEntranceRoad       = 102, //!< ���������N
			kPartOfParking      = 103, //!< ���ԏꃊ���N
			kPartOfWalkway      = 104, //!< ���������N
		};
	} // gl_link_class

	// ���H�����敪�R�[�h
	namespace gl_road_width {
		enum ECode {
			kUndefined       = 0, //!< ������
			kOver130         = 1, //!< ����13.0m�ȏ�
			kBetween55And130 = 2, //!< ����5.5m�ȏ�13.0m����
			kBetween30And55  = 3, //!< ����3.0m�ȏ�5.5m����
			kUnder30         = 4, //!< ����3.0m����
		};
	} // gl_road_width

	// �ʍs�֎~��ʃR�[�h
	namespace gl_no_passage {
		enum ECode {
			kNotclosed                   = 0, //!< �ʍs�֎~�Ȃ�
			kClosedNoCond                = 1, //!< �����Ȃ��ʍs�֎~
			kClosedWithCond              = 2, //!< �����t���ʍs�֎~
			kClosedNoCondExcAuthorized   = 4, //!< �����Ȃ��ʍs�֎~(�֌W�҈ȊO)
			kClosedWithCondExcAuthorized = 5, //!< �����t���ʍs�֎~(�֌W�҈ȊO)
		};
	} // gl_no_passage

	// ����ʍs��ʃR�[�h
	namespace gl_oneway {
		enum ECode {
			kNoOneway                   = 0, //!< ����ʍs�Ȃ�
			kOnewayNoCondOpenPositive   = 1, //!< �����Ȃ�����ʍs(������)
			kOnewayNoCondOpenNegative   = 2, //!< �����Ȃ�����ʍs(�t����)
			kOnewayWithCondOpenPositive = 3, //!< �����t������ʍs(������)
			kOnewayWithCondOpenNegative = 4, //!< �����t������ʍs(�t����)
			kOnewayWithCondDependOnCond = 5, //!< �����t������ʍs(���t)
		};
	} // gl_oneway

	// �ō����x�R�[�h
	namespace gl_max_speed {
		enum ECode
		{
			kUndefined = 0,
			k5         = 5,
			k10        = 10,
			k15        = 15,
			k20        = 20,
			k25        = 25,
			k30        = 30,
			k35        = 35,
			k40        = 40,
			k45        = 45,
			k50        = 50,
			k55        = 55,
			k60        = 60,
			k65        = 65,
			k70        = 70,
			k75        = 75,
			k80        = 80,
			k85        = 85,
			k90        = 90,
			k95        = 95,
			k100       = 100,
			k105       = 105,
			k110       = 110,
			k115       = 115,
			k120       = 120,
			kNon       = 999,
		};
	} // gl_max_speed

	// �ō����x�m�F�R�[�h
	namespace gl_speed_verified {
		enum ECode
		{
			kUnverified = 0, // �ō����x�W���@���m�F
			kVerified   = 1, // �ō����x�W���@�L��m�F
			kLegal      = 2, // �ō����x�W���@�����m�F(�@�葬�x����)
		};
	} // gl_verified

	// �n�E�X�i���o�[�����W�X�L�[�}
	namespace hnr_scheme
	{
		enum ECode
		{
			kUndefined = 0,  //!< Undefined
			kEven      = 1,  //!< Even
			kOdd       = 2,  //!< Odd
			kMixed     = 3,  //!< Mixed
		};
	}

	// �n���X�i���o�[�^�C�v
	namespace hn_type
	{
		enum ECode
		{
			kBase = 0,  //!< Base
			kOld  = 1,  //!< Old
		};
	}

} // road_link

// ���H�m�[�h�e�[�u��
namespace road_node {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];  //!< �e�[�u����

	extern const _TCHAR kNodeClass[];  ///< �m�[�h���
	extern const _TCHAR kSignal[];     //!< �M���@�R�[�h
	extern const _TCHAR kNameKanji[];  //!< �����_��������
	extern const _TCHAR kNameYomi[];   //!< �����_�J�i����
	extern const _TCHAR kHeightCode[]; //!< �����R�[�h 
	extern const _TCHAR kHeight[];     //!< �������
	extern const _TCHAR kParkingID[];  //!< ���ԏ�h�c

	/// �m�[�h��ʃR�[�h
	namespace gl_node_class {
		enum ECode {
			kNoFunction           = 0,  //!< �@�\�����m�[�h
			kCross                = 1,  //!< �����_�m�[�h
			kEdge                 = 2,  //!< �[�_�m�[�h
			kAttributeChangePoint = 3,  //!< �����ω��_�m�[�h
			kAttributed           = 4,  //!< �����t���m�[�h
			kOnSecondaryMesh      = 9,  //!< 2�����b�V������m�[�h
			kAvoidDuplicateLink   = 10, //!< ���[�v�����N����m�[�h
			KBorderPoint          = 999,//!< �����m�[�h
		};
	} // gl_node_class

	/// �M���@�R�[�h
	namespace gl_signal_class {
		enum ECode {
			kNotPresent = 0,  //!< �M���@�Ȃ�
			kPresent    = 1,  //!< �M���@����
		};
	} // gl_signal_class

} // road_node

// �����N�������t�B�[�`���N���X
namespace segment_attr {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::link_relational_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kUnderOver[]; ///< �㉺�֌W
	extern const _TCHAR kLinkAttr[]; ///< �����N��������ʃR�[�h

	// �����N�������R�[�h
	namespace gl_link_attr {
		enum ECode {
			kUndefined         = 0, //!< ����`
			kBridge            = 1, //!< ���E����
			kTunnel            = 2, //!< �g���l��
			kCave              = 3, //!< ������
			kRailroadCrossings = 4, //!< ����
			kUnderpass         = 5, //!< �A���_�[�p�X
			kOverpass          = 6, //!< �I�[�o�[�p�X
			kUnpaved           = 7, //!< ���ܑ�
		};
	} // gl_link_attr
} // segment_attr

// �j���w��R�[�h�h���C��
namespace day_code {
	enum ECode {
		kSunHolidays         =  1, //!< ���E�x��
		kNotSunHolidays      =  2, //!< ���E�x�����̂���
		kHolidaysExcSun      =  3, //!< �x��(���j�͂̂���)
		kNotHolidays         =  4, //!< �x�����̂���
		kWeekday             =  5, //!< ����
		kSun                 =  6, //!< ��
		kMon                 =  7, //!< ��
		kTue                 =  8, //!< ��
		kWed                 =  9, //!< ��
		kThu                 = 10, //!< ��
		kFri                 = 11, //!< ��
		kSat                 = 12, //!< �y
		kSatandSun           = 13, //!< �y��
		kNotSatSunHoliday    = 14, //!< �y���E�x�����̂���
		kAllDays             = 15, //!< �S�j��
		kNotSun              = 16, //!< ���j�����̂���
		kDaybeforeSunHoliday = 17, //!< ���E�x���̑O��
	};
} // day_code

// �E���܋֎~�����N��Inf�e�[�u��
namespace inf_turnreg {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kStartMonth0[];  //!< �K������:�J�n��
	extern const _TCHAR kStartDay0[];    //!< �K������:�J�n��
	extern const _TCHAR kStartHour0[];   //!< �K������:�J�n����
	extern const _TCHAR kStartMin0[];    //!< �K������:�J�n��
	extern const _TCHAR kEndMonth0[];    //!< �K������;�I����
	extern const _TCHAR kEndDay0[];      //!< �K������:�I����
	extern const _TCHAR kEndHour0[];     //!< �K������:�I������
	extern const _TCHAR kEndMin0[];      //!< �K������:�I����
	extern const _TCHAR kDayOfWeek0[];   //!< �K������:�j���w��

	extern const _TCHAR kStartMonth1[];  //!< �K������1:�J�n��
	extern const _TCHAR kStartDay1[];    //!< �K������1:�J�n��
	extern const _TCHAR kStartHour1[];   //!< �K������1:�J�n����
	extern const _TCHAR kStartMin1[];    //!< �K������1:�J�n��
	extern const _TCHAR kEndMonth1[];    //!< �K������1;�I����
	extern const _TCHAR kEndDay1[];      //!< �K������1:�I����
	extern const _TCHAR kEndHour1[];     //!< �K������1:�I������
	extern const _TCHAR kEndMin1[];      //!< �K������1:�I����
	extern const _TCHAR kDayOfWeek1[];   //!< �K������1:�j���w��

	extern const _TCHAR kStartMonth2[];  //!< �K������2:�J�n��
	extern const _TCHAR kStartDay2[];    //!< �K������2:�J�n��
	extern const _TCHAR kStartHour2[];   //!< �K������2:�J�n����
	extern const _TCHAR kStartMin2[];    //!< �K������2:�J�n��
	extern const _TCHAR kEndMonth2[];    //!< �K������2;�I����
	extern const _TCHAR kEndDay2[];      //!< �K������2:�I����
	extern const _TCHAR kEndHour2[];     //!< �K������2:�I������
	extern const _TCHAR kEndMin2[];      //!< �K������2:�I����
	extern const _TCHAR kDayOfWeek2[];   //!< �K������2:�j���w��

	extern const _TCHAR kStartMonth3[];  //!< �K������3:�J�n��
	extern const _TCHAR kStartDay3[];    //!< �K������3:�J�n��
	extern const _TCHAR kStartHour3[];   //!< �K������3:�J�n����
	extern const _TCHAR kStartMin3[];    //!< �K������3:�J�n��
	extern const _TCHAR kEndMonth3[];    //!< �K������3;�I����
	extern const _TCHAR kEndDay3[];      //!< �K������3:�I����
	extern const _TCHAR kEndHour3[];     //!< �K������3:�I������
	extern const _TCHAR kEndMin3[];      //!< �K������3:�I����
	extern const _TCHAR kDayOfWeek3[];   //!< �K������3:�j���w��

	extern const _TCHAR kStartMonth4[];  //!< �K������4:�J�n��
	extern const _TCHAR kStartDay4[];    //!< �K������4:�J�n��
	extern const _TCHAR kStartHour4[];   //!< �K������4:�J�n����
	extern const _TCHAR kStartMin4[];    //!< �K������4:�J�n��
	extern const _TCHAR kEndMonth4[];    //!< �K������4;�I����
	extern const _TCHAR kEndDay4[];      //!< �K������4:�I����
	extern const _TCHAR kEndHour4[];     //!< �K������4:�I������
	extern const _TCHAR kEndMin4[];      //!< �K������4:�I����
	extern const _TCHAR kDayOfWeek4[];   //!< �K������4:�j���w��

	extern const LPCTSTR kStartMonth[5]; //!< �K������:�J�n���ꗗ
	extern const LPCTSTR kStartDay[5];   //!< �K������:�J�n���ꗗ
	extern const LPCTSTR kStartHour[5];  //!< �K������:�J�n���Ԉꗗ
	extern const LPCTSTR kStartMin[5];   //!< �K������:�J�n���ꗗ
	extern const LPCTSTR kEndMonth[5];   //!< �K������;�I�����ꗗ
	extern const LPCTSTR kEndDay[5];     //!< �K������:�I�����ꗗ
	extern const LPCTSTR kEndHour[5];    //!< �K������:�I�����Ԉꗗ
	extern const LPCTSTR kEndMin[5];     //!< �K������:�I�����ꗗ
	extern const LPCTSTR kDayOfWeek[5];  //!< �K������:�j���w��ꗗ

	using namespace ::sindy::schema::global::day_code;

	struct where_all_year_all_days {};

	template <typename CharT>
	inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<CharT>& os, where_all_year_all_days)
	{
		using namespace atl2::io::compatible;
		return os << kStartMonth1 << L" IS NULL";
	}
} // inf_turnreg

// �E���܋֎~�����N��LQ�e�[�u��
namespace lq_turnreg {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // lq_turnreg

// �Ԏ�ʉE���܋֎~�����N��Inf�e�[�u��
namespace inf_vehicle_turnreg {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< �e�[�u����
}; // inf_vehicle_turnreg

// �Ԏ�ʉE���܋֎~�����N��LQ�e�[�u��
namespace lq_vehicle_turnreg {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // lq_vehicle_turnreg

// �Ԏ�ʉE���܋֎~�����N��K���e�[�u��
namespace vehicle_turnreg {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kInfID[];        //!< �Ԏ�ʉE���܋֎~ID
	extern const _TCHAR kHeight[];       //!< ��������
	extern const _TCHAR kWidth[];        //!< �ő啝
	extern const _TCHAR kWeight[];       //!< �d�ʐ���
	extern const _TCHAR kLength[];       //!< �Ԓ�
	extern const _TCHAR kLoadAge[];      //!< �ύڗ�
	extern const _TCHAR kDangerF[];      //!< �댯���t���O

	extern const _TCHAR kStartMonth[];   //!< �K������:�J�n��
	extern const _TCHAR kStartDay[];     //!< �K������:�J�n��
	extern const _TCHAR kStartHour[];    //!< �K������:�J�n����
	extern const _TCHAR kStartMin[];     //!< �K������:�J�n��
	extern const _TCHAR kEndMonth[];     //!< �K������;�I����
	extern const _TCHAR kEndDay[];       //!< �K������:�I����
	extern const _TCHAR kEndHour[];      //!< �K������:�I������
	extern const _TCHAR kEndMin[];       //!< �K������:�I����
	extern const _TCHAR kDayOfWeek[];    //!< �K������:�j���w��
} // vehicle_turnreg

// �����t���ʍs�֎~�e�[�u��
namespace time_nopassage {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::link_relational_table;
	using namespace ::sindy::schema::global::time_condition_table;

	extern const _TCHAR kSeasonReg[]; ///< �G�ߋK���t���O

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // time_nopassage

// �����t������ʍs�e�[�u���t�B�[���h
namespace time_oneway {
	using namespace ::sindy::schema::global::mesh_code_table;
	using namespace ::sindy::schema::global::directional_link_relational_table;
	using namespace ::sindy::schema::global::time_condition_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // time_oneway

// �Ԏ�ʋK���e�[�u��
namespace vehicle_reg {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kLinkID[];       //!< RoadLinkID
	extern const _TCHAR kRegTypeClass[]; //!< �K���^�C�v�R�[�h
	extern const _TCHAR kHeight[];       //!< ��������
	extern const _TCHAR kWidth[];        //!< �ő啝
	extern const _TCHAR kWeight[];       //!< �d�ʐ���
	extern const _TCHAR kLength[];       //!< �Ԓ�
	extern const _TCHAR kLoadAge[];      //!< �ύڗ�
	extern const _TCHAR kDangerF[];      //!< �댯���t���O

	extern const _TCHAR kStartMonth[];   //!< �K������:�J�n��
	extern const _TCHAR kStartDay[];     //!< �K������:�J�n��
	extern const _TCHAR kStartHour[];    //!< �K������:�J�n����
	extern const _TCHAR kStartMin[];     //!< �K������:�J�n��
	extern const _TCHAR kEndMonth[];     //!< �K������;�I����
	extern const _TCHAR kEndDay[];       //!< �K������:�I����
	extern const _TCHAR kEndHour[];      //!< �K������:�I������
	extern const _TCHAR kEndMin[];       //!< �K������:�I����
	extern const _TCHAR kDayOfWeek[];    //!< �K������:�j���w��

	// �K���^�C�v�R�[�h
	namespace gl_reg_type_class {
		enum ECode {
			kOnewayOpenPositive = 0,     //!< ����ʍs�i�������j
			kOnewayOpenNegative = 1,     //!< ����ʍs�i�t�����j
			kClosed             = 2,     //!< �ʍs�֎~
		};
	} // gl_reg_type_class
} // vehicle_reg

namespace vehicle_type {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];      //!< �e�[�u����

	extern const _TCHAR kRegID[];          //!< VehicleRegID
	extern const _TCHAR kVehicleRegCode[]; //!< �Ԏ�ʋK���R�[�h
	extern const _TCHAR kExcludeF[];       //!< ���O�t���O
} // vehicle_type

namespace vehicle_reg_master {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< �e�[�u����

	extern const _TCHAR kVehicleRegCode[];    //!< �K���R�[�h
	extern const _TCHAR kVehicleRegName[];    //!< �K������
	extern const _TCHAR kPassengerF[];        //!< ��p�ԃt���O
	extern const _TCHAR kTaxiF[];             //!< �^�N�V�[�t���O
	extern const _TCHAR kPublicBusF[];        //!< �����o�X
	extern const _TCHAR kPrivateBusF[];       //!< ���p�o�X
	extern const _TCHAR kTransportTruckF[];   //!< �g���b�N
	extern const _TCHAR kSignageIllustFile[]; //!< �K���摜��
} // vehicle_reg_master


/// ���ʈē������N��Inf�e�[�u��
namespace inf_dirguide {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];   //!< �e�[�u����

	extern const int kMaxNumber;        //!< �ő���ʈē���

	extern const _TCHAR kGuideClass0[]; //!< ���ʎ��
	extern const _TCHAR kNameKanji0[];  //!< ���ʖ���
	extern const _TCHAR kNameYomi0[];   //!< ���ʃ��~

	extern const _TCHAR kGuideClass1[]; //!< ���ʎ��1
	extern const _TCHAR kNameKanji1[];  //!< ���ʖ���1
	extern const _TCHAR kNameYomi1[];   //!< ���ʃ��~1

	extern const _TCHAR kGuideClass2[]; //!< ���ʎ��2
	extern const _TCHAR kNameKanji2[];  //!< ���ʖ���2
	extern const _TCHAR kNameYomi2[];   //!< ���ʃ��~2

	extern const _TCHAR kGuideClass3[]; //!< ���ʎ��3
	extern const _TCHAR kNameKanji3[];  //!< ���ʖ���3
	extern const _TCHAR kNameYomi3[];   //!< ���ʃ��~3

	extern const _TCHAR kGuideClass4[]; //!< ���ʎ��4
	extern const _TCHAR kNameKanji4[];  //!< ���ʖ���4
	extern const _TCHAR kNameYomi4[];   //!< ���ʃ��~4

	extern const _TCHAR kGuideClass5[]; //!< ���ʎ��5
	extern const _TCHAR kNameKanji5[];  //!< ���ʖ���5
	extern const _TCHAR kNameYomi5[];   //!< ���ʃ��~5

	extern const _TCHAR kGuideClass6[]; //!< ���ʎ��6
	extern const _TCHAR kNameKanji6[];  //!< ���ʖ���6
	extern const _TCHAR kNameYomi6[];   //!< ���ʃ��~6

	extern const _TCHAR kGuideClass7[]; //!< ���ʎ��7
	extern const _TCHAR kNameKanji7[];  //!< ���ʖ���7
	extern const _TCHAR kNameYomi7[];   //!< ���ʃ��~7

	extern const _TCHAR kGuideClass8[]; //!< ���ʎ��8
	extern const _TCHAR kNameKanji8[];  //!< ���ʖ���8
	extern const _TCHAR kNameYomi8[];   //!< ���ʃ��~8

	extern const _TCHAR kGuideClass9[]; //!< ���ʎ��9
	extern const _TCHAR kNameKanji9[];  //!< ���ʖ���9
	extern const _TCHAR kNameYomi9[];   //!< ���ʃ��~9

	extern const _TCHAR kGuideClass10[]; //!< ���ʎ��10
	extern const _TCHAR kNameKanji10[];  //!< ���ʖ���10
	extern const _TCHAR kNameYomi10[];   //!< ���ʃ��~10

	extern const LPCTSTR kGuideClass[11]; //!< ���ʎ�ʈꗗ
	extern const LPCTSTR kNameKanji[11];  //!< ���ʖ��̈ꗗ
	extern const LPCTSTR kNameYomi[11];   //!< ���ʃ��~���̈ꗗ

	extern const _TCHAR kGuidePattern[]; //!< ���ʈē��p�^�[��
	extern const _TCHAR kSequence[];     //!< �V�[�P���X

	/// ���ʈē���ʃR�[�h
	namespace guide_class {
		enum ECode {
			kGuide     = 51, //!< Guide
			kRouteNum  = 52, //!< RouteNumber
			kExitNum   = 53, //!< ExitNumber
			kPictogram = 54, //!< Pictogram
		};
	}; // guide_class

	/// ���ʈē��p�^�[���R�[�h
	namespace guide_pattern {
		enum ECode {
			kNoPattern  =  0, //!< �p�^�[������
			kGreen      = 51, //!< Green
			kBlue       = 52, //!< Blue
			kWhite      = 53, //!< White
			kOrange     = 54, //!< Orange
			kUndefined  = 99, //!< ������
		};
	}; // guide_pattern

	/// ���ʎ�ʃR�[�h
	namespace destination_type {
		enum ECode {
			kGuideLocal                   = 1,  //!< Guide(Local)
			kGuideToll                    = 2,  //!< Guide(Toll)
			kNationalRoadRouteNumber      = 3,  //!< NationalRoadRouteNumber 
			kPrefecturalHighwayNumber     = 4,  //!< PrefecturalHighwayNumber
			kOther                        = 5,  //!< Other
			kTollCharacterString          = 12, //!< Toll(Character String)
			kTollNationalRoadNumber       = 13, //!< Toll(National Road Number)
			kTollPrefecturalHighwayNumber = 14, //!< Toll(Prefectural Highway Number)
		};
	} // destination_type

} // inf_dirguide

/// �����U�������N��Inf�e�[�u��
namespace inf_guide {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kGuideType[]; //!< �����U���ē��R�[�h
	extern const _TCHAR kDelete[];    //!< �����U���폜�t���O

	/// �����U���ē��R�[�h
	namespace guide_type {
		enum ECode {
			kNotSet     =  0, ///< ���ݒ�
			kNoGuide    =  1, ///< ���Ȃ�i�ē��Ȃ��j
			k12         =  2, ///< ���i�i�P�Q�������j
			k1          =  3, ///< �E�O�����i�P�������j
			k2          =  4, ///< �E��O�����i�Q�������j
			k3          =  5, ///< �E�܁i�R�������j
			k4          =  6, ///< �E�΂ߌ������i�S�������j
			k5          =  7, ///< �E�߂�����i�T�������j
			k6          =  8, ///< �߂�����i�U�������j
			k7          =  9, ///< ���߂�����i�V�������j
			k8          = 10, ///< ���΂ߌ������i�W�������j
			k9          = 11, ///< ���܁i�X�������j
			k10         = 12, ///< ����O�����i�P�O�������j
			k11         = 13, ///< ���O�����i�P�P�������j
		};
	} // guide_type

} // inf_guide

/// ���[�������N��Inf�e�[�u��
namespace inf_lane {
	using namespace ::sindy::schema::global::fieldsurvey_table;

	extern const _TCHAR kTableName[];    //!< �e�[�u����

	extern const _TCHAR kLaneDir[];      //!< ���[�������R�[�h
	extern const _TCHAR kApproachLane[]; //!< �����_���ߎԐ���
	extern const _TCHAR kRightChange[];  //!< �E�������Ԑ���
	extern const _TCHAR kLeftChange[];   //!< ���������Ԑ���
	extern const _TCHAR kFlowLane[];     //!< ���o�Ԑ���
	extern const _TCHAR kLanePassable[]; //!< �Ԑ��ʍs���

	/// ���[�������R�[�h
	namespace lane_direction {
		enum ECode {
			kUndefined = 0, //!< ���ݒ�
			k0         = 1, //!< �O����(0)
			k45        = 2, //!< �E�΂ߑO����(45)
			k90        = 3, //!< �E����(90)
			k135       = 4, //!< �E�΂ߌ�����(135)
			k180       = 5, //!< ������(180)
			k225       = 6, //!< ���΂ߌ�����(225)
			k270       = 7, //!< ������(270)
			k315       = 8, //!< ���΂ߑO����(315)
		};
	} // lane_direction

} // inf_lane

/// �����U�������N��LQ�e�[�u��
namespace lq_guide {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // lq_guide

/// ���ʈē������N��LQ�e�[�u��
namespace lq_dirguide {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // lq_dirguide

/// ���[�������N��LQ�e�[�u��
namespace lq_lane {
	using namespace ::sindy::schema::global::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // lq_lane

/// �H���R�[�h���X�g�e�[�u��
namespace road_code_list {
	using namespace ::sindy::schema::global::object;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kOperator[];    //!< �ŏI�X�V��
	extern const _TCHAR kModifyDate[];  //!< �ŏI�X�V����
	extern const _TCHAR kRoadCode[];    //!< �H������ID
	extern const _TCHAR kNameKanji[];   //!< �H����������
	extern const _TCHAR kNameYomi[];    //!< �H�����~����
	extern const _TCHAR kWorkKanji[];   //!< �H����������(Hwy���[�h�p)
	extern const _TCHAR kWorkYomi[];    //!< �H�����~����(Hwy���[�h�p)
	extern const _TCHAR kLineClass[];   //!< �H����ʃR�[�h
	extern const _TCHAR kLineDirType[]; //!< �H�������^�C�v�R�[�h
	extern const _TCHAR kLandClass[];   //!< �n���ʃR�[�h

	/// �H����ʃR�[�h��`
	namespace line_class {
		enum ECode {
			kHighway     =  1, ///< �������H
			kToll        =  2, ///< �L�����H
			kCityHighway =  3, ///< ��s�������H
			kHanshin     =  4, ///< ��_�������H
			kNagoya      =  5, ///< ���É��s�s�������H
			kFukuoka     =  6, ///< �����s�s�������H
			kKitakyu     =  7, ///< �k��B�s�s�������H
			kHiroshima   =  8, ///< �L���������H
			kFerry       =  9, ///< �t�F���[�q�H
			kNormal      = 10, ///< ��ʊX������
			kTF          = 11, ///< TF�����Ή��H��
			kKyoto		 = 12, ///< ���s�������H
		};
	} // line_class

	/// �H�������^�C�v�R�[�h��`
	namespace line_dir_type {
		enum ECode {
			kNoType		= 0,	///< �^�C�v�Ȃ�
			kUpDown		= 1,	///< ���E����
			kInOut		= 2,	///< �����E�O���
			kEastWest	= 3,	///< ���s���E���s��
			kOther		= 4,	///< ���̑�
		};
	} // line_dir_type

} // road_code_list

// �s���E�̃x�[�X
namespace admin_class {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kAdminCode[];       //!< �s���E�R�[�h
	extern const _TCHAR kPopulation[];      //!< �l���l
} // admin_class

// built up area�|���S��
namespace built_up_area {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kAdminCode[];       //!< �s���E�R�[�h
	extern const _TCHAR kPopulation[];      //!< �l���l
} // built_up_area

namespace common_lang_type{
	namespace gl_lang_type {
		enum ECode {
			kUndefined = 0,                 //!< ����`
			kENG = 1,                       //!< �p��
			kTHA = 2,                       //!< �^�C��
			kIND = 3,                       //!< �C���h�l�V�A��
			kMAY = 4,                       //!< �}���[�V�A��
			kVIE = 5,                       //!< �x�g�i����
			kLAO = 6,						//!< ���I�X��
			kKHM = 7,                       //!< �J���{�W�A��
			kJPN = 99,                      //!< ���{��
		};
	} // gl_lang_type
} // common_lang_type

// ���|���S��
namespace country {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::common_lang_type;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kAdminCode[];       //!< �s���E�R�[�h
	extern const _TCHAR kLangC[];           //!< ���p��
	extern const _TCHAR kDriveSide[];       //!< �i�s�Ԑ��w��
	extern const _TCHAR kSpeedLimitUnit[];  //!< �ō����x�P��
	extern const _TCHAR kTimeZone[];        //!< �^�C���]�[��
	extern const _TCHAR kPhoneCountryC[];   //!< ���d�b�ԍ�
	extern const _TCHAR kPhonePrefix[];     //!< �d�b�ԍ��v���t�B�b�N�X
	extern const _TCHAR kPOIAddrDispType[]; //!< �\���^�C�v

	namespace gl_drive_side {
		enum ECode {
			kLeftSide  = 0,                 //!< ���Ԑ�
			kRightSide = 1,                 //!< �E�Ԑ�
		};
	} // gl_drive_side

	// �ō����x�P��
	namespace gl_speed_limit {
		enum ECode {
			kKPH = 0,                       //!< km/h
			kMPH = 1,                       //!< mile/h
		};
	} // gl_speed_limit

} // country

// PROVINCE�|���S��
namespace province {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
} // province

// DISTRICT�|���S��
namespace district {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
} // district

// SUBDISTRICT�|���S��
namespace sub_district {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
} // subdistrict

// VILLAGE�|���S��
namespace village {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
} // Village

// NAMEDAREA�|���S��
namespace named_area {
	using namespace ::sindy::schema::global::admin_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
} // namedarea

// POSTALDISTRICT
namespace postal_district {
	using namespace ::sindy::schema::global::country_feature;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kPostalCode[];      //!< �X�֔ԍ�
} // postal_district

// �^�C�v�R�[�h
namespace hn_common_domain{
	namespace gl_hn_type {
		enum ECode {
			kBase = 0,                          //!< �W��
			kOld  = 1,                          //!< ���W��
		};
	} // gl_hn_type
} // hn_common_domain

// HNP
namespace hnp {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::hn_common_domain;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kHn[];              //!< �n�E�X�i���o�[
	extern const _TCHAR kHnType[];          //!< �n�E�X�i���o�[�^�C�v
	extern const _TCHAR kLinkID[];          //!< ���H�����NID
	extern const _TCHAR kRoadNameID[];      //!< ���H��ID
	// TODO 2015�N��4���ɂ͕s�v�ɂȂ錩���݂Ȃ̂ŁA�s�v�ɂȂ��������
	extern const _TCHAR kHnRangeLineID[];   //!< �n�E�X�i���o�[�����W���C��ID
} // hnp

// GL_POIAccuracy
namespace gl_poi_accuracy{
	enum ECode {
		kUndefined  ,                   //!< ����`
		kVerified   ,                   //!< ���؍�
		kSpecialized,                   //!< Specialized
		kAuto       ,                   //!< ����
	};
} // gl_poi_accuracy

// HNP_ENTRYPOINT
namespace hnp_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::gl_poi_accuracy;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kHnpPointID[];      //!< HNP��ID
	extern const _TCHAR kAccuracyCode[];      //!< ���x�R�[�h

} // hnp_entry_point

// POSTALPOINT
 namespace postal_point{
 	using namespace ::sindy::schema::global::country_feature;
 
 	extern const _TCHAR kTableName[];       //!< �e�[�u����
 
 	extern const _TCHAR kPostalCode[];		//!< POSTALPOINT(�X�֔ԍ��|�C���g)�̓V���K�|�[���݂̂Ŏg�p�B�V���K�|�[����6���̗X�֔ԍ��ƌ��܂��Ă���
 	extern const _TCHAR kRoadLinkID[];		//!< ���H�����NID
	extern const _TCHAR kRoadNameID[];		//!< ���H��ID
 }// postal_point

// POSTALENTRYPOINT
namespace postal_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::gl_poi_accuracy;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kPostalPointID[];   //!< POSTALPOINT��ID
	extern const _TCHAR kAccuracyCode[];    //!< ���x�R�[�h

} // postal_entry_point

 // HNR
namespace hnr {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::hn_common_domain;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kFromHnLeft[];      //!< �����̊J�n�n�E�X�i���o�[
	extern const _TCHAR kToHnLeft[];        //!< �����̏I���n�E�X�i���o�[
	extern const _TCHAR kFromHnRight[];     //!< �E���̊J�n�n�E�X�i���o�[
	extern const _TCHAR kToHnRight[];       //!< �E���̏I���n�E�X�i���o�[
	extern const _TCHAR kHnSchemeLeft[];    //!< �����̃n�E�X�i���o�[��`
	extern const _TCHAR kHnSchemeRight[];   //!< �E���̃n�E�X�i���o�[��`
	extern const _TCHAR kHnType[];          //!< �n�E�X�i���o�[�^�C�v
	extern const _TCHAR kLinkID[];          //!< �����NID

	// ��`�R�[�h
	namespace gl_hnr_scheme {
		enum ECode {
			kEven  = 0,                     //!< ����
			kOdd   = 1,                     //!< �
			kMixed = 2,                     //!< ����
		};
	} // gl_hnr_scheme
} // hnr

// POI_INFO
namespace poi_info {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::common_lang_type;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];         //!< �e�[�u����

	extern const _TCHAR kUpdAvailDate[];      //!< ���
	extern const _TCHAR kAccuracyLevel[];     //!< ���x���m���x��
	extern const _TCHAR kAdminCode[];         //!< �Z���R�[�h
	extern const _TCHAR kRoadNameID[];        //!< ���H�̖��̃e�[�u��ID
	extern const _TCHAR kProductType[];       //!< ���i���
	extern const _TCHAR kHouseNumber[];       //!< �n�E�X�i���o�[
	extern const _TCHAR kLanguageCode[];      //!< Actual Address�̌���R�[�h
	extern const _TCHAR kActualAddress[];     //!< Actual Address
	extern const _TCHAR kLanguageTransCode[]; //!< Actual Address Translation�̌���R�[�h
	extern const _TCHAR kActualAddressTrans[];//!< Actual Address Translation
	extern const _TCHAR kCategoryCode[];      //!< �J�e�S���[�R�[�h
	extern const _TCHAR kSubCategoryCode[];   //!< �T�u�J�e�S���[�R�[�h
	extern const _TCHAR kChainMasterCode[];   //!< �`�F�[���R�[�h
	extern const _TCHAR kDetailInfo[];        //!< �ڍ׏��
	extern const _TCHAR kDeletionCode[];      //!< �폜�R�[�h
	extern const _TCHAR kFuelType[];          //!< �^�C�v�R�[�h

	namespace gl_poi_accuracy {
		enum ECode {
			kUndefined, // =0             //!< ����`
			kVerified,                    //!< ���؍�
			kSpecialized,                 //!< Specialized
			kAuto                         //!< ����
		};
	} // gl_poi_accuracy

	namespace gl_product_type {
		enum ECode {
			kOriginal	= 0,              //!< �I���W�i��
			k3rdParty	= 1,              //!< �T�[�h�p�[�e�B
			kOutsource	= 2,              //!< �A�E�g�\�[�X
		};
	} // gl_product_type
	
	namespace gl_poi_deletion {
		enum ECode {
			kInUse            = 0,              
			TemporaryClosure  = 1,              
			kSuveyImpossible  = 2,
			kDependOnList     = 3,
			kDeletion         = 99
		};
	} // gl_product_type

	// TODO:�g�p����Ă��Ȃ��̂ŁA�������č폜
	namespace gl_phon_type {
		enum ECode {
			kPhoneNumber     = 0,         //!< �ʏ�
			kTollFreeNumber  = 1,         //!< �t���[�_�C�A��
			kMobilePhone     = 2,         //!< �g�ѓd�b
			kEtc             = 3          //!< ���̑�
		};
	} // gl_phon_type

	namespace gl_category {					
		enum ECode {
			kFood                     = 10, //!< ���H
			kBank_ATM                 = 11, //!< ��s
			kBusiness                 = 12, //!< ���
			kEducation                = 13, //!< ����
			kReligion                 = 14, //!< �@��
			kHospital_Healthcare      = 15, //!< �a�@
			kConference_Community     = 16, //!< �A��
			kAutomobile               = 17, //!< ������
			kTransportation           = 18, //!< �^��
			kCityCenter               = 22, //!< �V�e�B�Z���^�[
			kService                  = 19, //!< �T�[�r�X
			kHotel                    = 20, //!< �z�e��
			kInfrastructure_Hydro     = 21, //!< �C���t���E����
			kResidence                = 30, //!< �Z��
			kShops_Store              = 40, //!< �V���b�v
			kTourism                  = 50, //!< �ό�
			kArt_Culture              = 60, //!< �|�p
			kSport                    = 70, //!< �X�|�[�c
			kEntertainment            = 80, //!< �G���^�[�e�C�����g
			kGovernment               = 90, //!< �s��
			kUndefined                = 0   //!< ����`
		};				
	} // gl_category
	
	// TODO: �R�����g�L��
	namespace gl_sub_category {
		enum ECode {
			kThai                            = 1001,
			kChinese                         = 1002,
			kJapanese                        = 1003,
			kKorean                          = 1004,
			kVietnam                         = 1005,
			kMuslim_Indian                   = 1006,
			kItalian                         = 1007,
			kFrench                          = 1008,
			kGerman                          = 1009,
			kWesternOthers                   = 1010,
			kSeafood                         = 1011,
			kFoodCenter                      = 1012,
			kSuki                            = 1013,
			kFastFood                        = 1014,
			kSteak                           = 1015,
			kBarbecue                        = 1016,
			kCoffeeShop                      = 1017,
			kBakery_IceCream                 = 1018,
			kFood_Others                     = 1099,
			kBanks_FinancialServices         = 1101,
			kCurrencyExchange                = 1102,
			kATM                             = 1103,
			kBank_Others                     = 1199,
			kOfficeBuilding                  = 1201,
			kBuisness_Others                 = 1299,
			kUniversity_College              = 1301,
			kSchool_Kindergarten_Vocation    = 1302,
			kTutoring_Computer_LanguageIns   = 1303,
			kLibrary                         = 1304,
			kNon_formalEducationCenter       = 1305,
			kEducation_Others                = 1399,
			kBuddhistTemple                  = 1401,
			kChurch                          = 1402,
			kMosque                          = 1403,
			kCemetery                        = 1404,
			kBureauOfMonks                   = 1405,
			kShrine                          = 1406,
			kReligion_Others                 = 1499,
			kHospital                        = 1501,
			kPolyClinic                      = 1502,
			kPublicHealthCenter              = 1503,
			kDentalClinic                    = 1504,
			kEyeEarNoseThroatClinic          = 1505,
			kSkin_SurgeryClinic              = 1506,
			kPharmacy                        = 1507,
			kSpas_Massage_BodyCare           = 1508,
			kBone_ArticulationClinic         = 1509,
			kChildClinic                     = 1510,
			kChildrenDevelopmentCenter       = 1511,
			kClinicOthers                    = 1512,
			kCommunityHealthCenter           = 1513,
			kFamilyPlanningCenter            = 1514,
			kRehabilitateMedicalClinic       = 1515,
			kRehabilitationClinic            = 1516,
			kSinusClinic                     = 1517,
			kThaiMassageClinic               = 1518,
			kX_rayLab_LabClinic              = 1519,
			kHospital_Others                 = 1599,
			kExhibition_Conference           = 1601,
			kCommunityCentre                 = 1602,
			kMeetingHall_AssocClubHose       = 1603,
			kConference_Others               = 1699,
			kCarDealership                   = 1701,
			kCarRental                       = 1702,
			kCarRepair                       = 1703,
			kAutoParts_AutoAccessories       = 1704,
			kCarCare                         = 1705,
			kPetrolStations                  = 1706,
			kNGV_LPGStation                  = 1707,
			kParkingGarage                   = 1708,
			kParkingLot                      = 1709,
			kRestArea                        = 1710,
			kTankFarm                        = 1711,
			kAutomobile_Others               = 1799,
			kAirport                         = 1801,
			kAirportPassengerTerminal        = 1802,
			kBusStation                      = 1803,
			kBTS_MRTStation                  = 1804,
			kFerryTerminal                   = 1805,
			kPier                            = 1806,
			kMarina                          = 1807,
			kAirlines                        = 1808,
			kRailwayStation                  = 1809,
			kTransportCompany                = 1810,
			kTollPlaza                       = 1811,
			kPort                            = 1820,
			kTransport_Others                = 1899,
			kAir_conditioning                = 1902,
			kAntiques                        = 1903,
			kBeautySalon_Barber              = 1904,
			kBeautyServiceCenter_Cosmetic    = 1905,
			kBuddha_ReligiousArticleFoundry  = 1906,
			kBicycle                         = 1907,
			kBugExterminator                 = 1909,
			kCargoCentre                     = 1911,
			kCarpet_Curtain_Blinds_Flooring  = 1912,
			kCircle_ClockTower               = 1915,
			kClothesSewingShop               = 1917,
			kClothes_Leather_HandBag_Shoe    = 1918,
			kConstruction_LandFill           = 1920,
			kCookingGas                      = 1921,
			kCooperativeSociety              = 1922,
			kDrinkingWaterBottler_IceFactory = 1923,
			kDryGoods                        = 1924,
			kDryRice_AnimalFeed              = 1925,
			kDutyFreeShop                    = 1926,
			kElectricalAppliance             = 1927,
			kElectricalRepair                = 1928,
			kFactory_Industry                = 1929,
			kFlorist_FlowerNursery           = 1932,
			kFoundation                      = 1933,
			kFreePort                        = 1934,
			kFrontierCrossing                = 1935,
			kGift_Souvenir                   = 1936,
			kGlassware                       = 1937,
			kGun_Ammunition                  = 1938,
			kHandcraftHousing                = 1939,
			kHardware_ConstructionMaterial   = 1940,
			kHornSculpture                   = 1941,
			kHousePet_Fish_Aquarium          = 1942,
			kInsurance                       = 1943,
			kInteriorDesign                  = 1944,
			kInternetService_InternetCafe    = 1945,
			kJewelryShop                     = 1946,
			kKitchenware                     = 1947,
			kLaundry                         = 1949,
			kLeatherHandBagMfg               = 1950,
			kLocksmith                       = 1951,
			kLuggage                         = 1952,
			kModelingAgency                  = 1954,
			kMusicalInstruments              = 1955,
			kMusical_Arts_DanceLesson        = 1956,
			kOffice_CompanyOffice            = 1958,
			kPawnShop                        = 1959,
			kPaymentService                  = 1960,
			kPC_VideoGames                   = 1961,
			kPetsClinic                      = 1962,
			kPhotoLab_Camera_Film            = 1963,
			kPictureFrame                    = 1964,
			kPlasticGoodsMfg                 = 1965,
			kPoliticalPartyOffice            = 1967,
			kPrintingShop                    = 1968,
			kRubberStamp_Chops               = 1973,
			kShoeRepair                      = 1974,
			kSignBoard                       = 1976,
			kTape_VDO_VCD                    = 1979,
			kTelevision_RadioStation         = 1980,
			kToys                            = 1982,
			kTVAntenna_SatelliteDish         = 1984,
			kCableTVAgency                   = 1985,
			kUpholstery                      = 1986,
			kWarehouse                       = 1987,
			kWatches_Glasses_Optics          = 1988,
			kWeddingService                  = 1989,
			kWindowFrame_Aluminum_Screen_Alloy = 1990,
			kWine_Beer_AlcoholicBeverage     = 1991,
			kWinery                          = 1992,
			kService_Others                  = 1999,
			kHotel_Motel                     = 2001,
			kResort                          = 2002,
			kBungalow                        = 2003,
			kGuestHouse                      = 2004,
			kHotel_Others                    = 2099,
			kJunction_Bridge                 = 2101,
			kHydro                           = 2103,
			kInfra_Others                    = 2199,
			kCountry                         = 2201,
			kCapital                         = 2202,
			kProvince                        = 2203,
			kDistrict                        = 2204,
			kSubDistrict                     = 2205,
			kCityCenter_Others               = 2299,
			kCondominium                     = 3001,
			kApartment                       = 3002,
			kFlat                            = 3003,
			kDormitory                       = 3004,
			kHousing                         = 3005,
			kResidence_Others                = 3099,
			kShoppingCentre_Bazaar           = 4001,
			kDepartmentStore                 = 4002,
			kSupermarket                     = 4003,
			kMarket                          = 4004,
			kBooks_OfficeEquipment           = 4005,
			kFurniture_HomeRepair            = 4006,
			kComputer_Software               = 4007,
			kSportingGoods                   = 4008,
			kMobileShop                      = 4009,
			kMinimart_ConvenienceStore       = 4010,
			kShops_Others                    = 4099,
			kNationalPark_ForestPark         = 5001,
			kMountain                        = 5002,
			kBeach                           = 5003,
			kIsland                          = 5004,
			kWaterfall                       = 5005,
			kReservoir_Dam                   = 5006,
			kCave                            = 5007,
			kHotSpring                       = 5008,
			kVantagePoint                    = 5009,
			kTourismSite                     = 5010,
			kFarm_Orchard_Winery             = 5011,
			kCityPark                        = 5012,
			kZoo                             = 5013,
			kElephantRide                    = 5014,
			kRafting                         = 5015,
			kCamping                         = 5016,
			kTouristOffice                   = 5017,
			kTravelAgency                    = 5018,
			kTourism_Others                  = 5099,
			kPalace                          = 6001,
			kHistoricalMonument              = 6002,
			kCulturalCentre                  = 6003,
			kMuseum                          = 6004,
			kArtGallery                      = 6005,
			kArt_Others                      = 6099,
			kStadium                         = 7001,
			kGolfCourse                      = 7002,
			kGolfDrivingRange                = 7003,
			kHorseracing_Racecourses         = 7004,
			kSports_Recreation               = 7005,
			kFitnessCenter                   = 7006,
			kSwimmingPool                    = 7007,
			kBowling                         = 7008,
			kSpot_Others                     = 7099,
			kAmusementPark_ThemePark         = 8001,
			kMovieTheater                    = 8002,
			kTheaters                        = 8003,
			kAssociation_Club                = 8004,
			kPub_Bar                         = 8005,
			kDiscotheque_NightClub           = 8006,
			kCafe                            = 8007,
			kKaraoke                         = 8008,
			kSnookerClub                     = 8009,
			kMassageParlor                   = 8010,
			kEntertainment_Others            = 8099,
			kGovernmentOffices_Military      = 9001,
			kEmbassy_Consulate               = 9002,
			kCityHall                        = 9003,
			kCourtHouse                      = 9004,
			kPoliceDepartment                = 9005,
			kPostOffice                      = 9006,
			kFireStation                     = 9007,
			kLandOffice                      = 9008,
			kPrison                          = 9009,
			kRevenueDepartmentOffice         = 9010,
			kGovernmentOther                 = 9011,
			kGovernment_Others               = 9099,
			kUndefined                       = 0   ,
		};				
	} // gl_sub_category

} // poi_position

// POI_CHAIN_MASTER
namespace poi_chain_master {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kChainCode[];		//!< �`�F�[���R�[�h
	extern const _TCHAR kNameLatin[];		//!< �p�ꖼ�i���e�����j
	extern const _TCHAR kNameLocal[];		//!< ���n�ꖼ
	extern const _TCHAR kLocalLanguageC[];	//!< ���n��R�[�h

} // poi_chain_master

// POI_ENTRYPOINT
namespace poi_entry_point {
	using namespace ::sindy::schema::global::country_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];         //!< �e�[�u����

	extern const _TCHAR kPoiInfoID[];         //!< POI_POSITION��ID
	extern const _TCHAR kPriorityF[];         //!< �D��x�t���O(0:Main, 1:Sub)
	extern const _TCHAR kEntryPointCode[];    //!< �U���|�C���g�̎�ʁi���ԏ����/�{�ݓ����j
	extern const _TCHAR kAccuracyCode[];      //!< ���x�R�[�h

	// �U���|�C���g��ʃR�[�h
	namespace gl_entrypoint_code{
		enum ECode {
			kUndefined            = 0,        //!< ����`
			kEntrance_of_parking  = 1,        //!< ���ԏ����
			kEntrance_of_facility = 2,        //!< �{�ݓ���
			kEntrance_for_pedestrian = 3,     //!< ���s��
		};
	} // gl_entrypoint_code

	// ���x�R�[�h
	namespace gl_coordinate_code {
		enum ECode {
			kUndefined   = 0,                 //!< ����`
			kVerified    = 1,                 //!< ���؍�
			kSpecialized = 2,                 //!< Specialized
			kAuto        = 3,                 //!< ����
		};
	} // gl_coordinate_code
} // poi_entry_point

// POI_PHONE
namespace poi_phone {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< �e�[�u����

	extern const _TCHAR kPoiInfoID[];         //!< POI_POSITION��ID
	extern const _TCHAR kPhonePriorityF[];    //!< �D��x�t���O(1: Main, 0: Sub)
	extern const _TCHAR kPhoneType[];         //!< ��ʃ^�C�v
	extern const _TCHAR kPhoneNumber[];       //!< �d�b�ԍ�

	// ��ʃ^�C�v�R�[�h
	namespace gl_phone_type {
		enum ECode {
			kPhoneNumber    = 0,              //!< �ʏ�
			kTollFreeNumber = 1,              //!< �t���[�_�C����
			kMobilePhone    = 2,              //!< �g�ѓd�b
			kEtc            = 3,              //!< ���̑�
		};
	} // gl_phone_type
} // poi_phone

// POI_ASSOCIATION
namespace poi_association {
	using namespace ::sindy::schema::global::country_table;

	extern const _TCHAR kTableName[];         //!< �e�[�u����

	extern const _TCHAR kParentID[];          //!< �eID(POI_POSITION��ID)
	extern const _TCHAR kChildID[];           //!< �qID(POI_POSITION��ID)
} // poi_association

// TOP_ANNOTATION
namespace top_annotation{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kAnnoClassC[];		//!< ���L��ʃR�[�h
}

// TOP_COUNTRY
namespace top_country{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kCountryC[];		//!< ���R�[�h
}

// TOP_PROVINCE
namespace top_province{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
}

// TOP_WATERWAY_LINE
namespace top_waterway_line{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kBgClassC[];		//!< �w�i��ʃR�[�h
}

// TOP_WATERWAY_POLYGON
namespace top_waterway_polygon{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kBgClassC[];		///!< �w�i��ʃR�[�h
}

// TOP_PROVINCE
namespace top_province{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
}

// AUTO_ANNOTATION
namespace auto_annotation{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];       //!< �e�[�u����

	extern const _TCHAR kOrgID[];			//!< OriginalObjectID
	extern const _TCHAR kMainName[];		//!< MainName
	extern const _TCHAR kMainLanguageC[];	//!< MainLanguageCode
	extern const _TCHAR kSubName[];			//!< SubName
	extern const _TCHAR kSubLanguageC[];	//!< SubLanguageCode
	extern const _TCHAR kLayerC[];			//!< CorrespondingLayerCode
	extern const _TCHAR kAnnoCode[];		//!< OriginalAnnotationCode
	extern const _TCHAR kAnnoAngle[];       //!< AnnotationAngle

}

// Annotation common object
namespace anno_common{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kOrgID[];			//!< OriginalObjectID			
	extern const _TCHAR kRoadNoString[];	//!< RoadNoString
	extern const _TCHAR kLayerC[];			//!< CorrespondingLayerCode
	extern const _TCHAR kAnnoCode[];		//!< OriginalAnnotationCode
}

// ANNO_BASE_RAILWAY
namespace anno_base_railway{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
}

// ANNO_ROAD_NAME
namespace anno_road_name{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
}

// ANNO_ROAD_NO
namespace anno_road_no{
	using namespace anno_common;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
}

/// OFFICIAL_NAME�e�[�u��
namespace official_name {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kLayerCode[];     //!< ���C���R�[�h
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kLanguageCode[];  //!< ����R�[�h
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // official_name

/// ALTERNATIVE_NAME�e�[�u��
namespace alternative_name {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kOfficialID[];    //!< �I�t�B�V����ID
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kLanguageCode[];  //!< ����R�[�h
	extern const _TCHAR kSequence[];      //!< �V�[�P���X
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // alternative_name

/// TRANSLATION�e�[�u��
namespace translation {
	using namespace ::sindy::schema::global::country_table;
	using namespace ::sindy::schema::global::common_lang_type;

	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kTypeF[];         //!< 0:Official, 1:Alternative
	extern const _TCHAR kID[];            //!< [Official or Alternative] ObjectID
	extern const _TCHAR kName[];          //!< ����
	extern const _TCHAR kLanguageCode[];  //!< ����R�[�h
	extern const _TCHAR kDisplayF[];      //!< DisplayFlag

} // translation

namespace base_border_line {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
	extern const _TCHAR kCountryType[];		//!< CountryType
	extern const _TCHAR kOceanF[];			//!< OceanFlag
} // base_border_line

namespace border_point {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[]    ;       //!< �e�[�u����

	extern const _TCHAR kBorderPointCode[];		//!< BORDER_POINT���������A�t�F���[�q�H�݂̂Ȃ����������������
	extern const _TCHAR kCountryCode1[] ;       //!< ���ԍ�1
	extern const _TCHAR kOriginalObjectID1[] ;  //!< �I�u�W�F�N�gID1
	extern const _TCHAR kCountryCode2[] ;       //!< ���ԍ�2
	extern const _TCHAR kOriginalObjectID2[] ;  //!< �I�u�W�F�N�gID2

	namespace gl_borderpoint_code {
		enum ECode {
			kUndefined		= 0,             //!< Undefined
			kOfficial		= 1,             //!< Official
			kNonOfficial	= 2,             //!< NonOfficial
		};
	} // gl_borderpoint_code

} // border_point

namespace release_country_zone {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;
	using namespace ::sindy::schema::global::border_point::gl_country_code;

	extern const _TCHAR kTableName[];            //!< �e�[�u����

	extern const _TCHAR kCountryCode[];          //!< ���ԍ�
} // release_country_zone

namespace sea_polygon {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];            //!< �e�[�u����
} // sea_polygon

namespace sea_polygon_name {
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::name_class;

	extern const _TCHAR kTableName[];            //!< �e�[�u����
	extern const _TCHAR kUnitF[];				 //!< �C���̃|���S�����j�b�g�t���O
} // sea_polygon_name

namespace land_complement {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];            //!< �e�[�u����
} // land_complement

namespace top_border_line {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[];       //!< �e�[�u����
	extern const _TCHAR kCountryType[];		//!< CountryType
	extern const _TCHAR kOceanF[];			//!< OceanFlag
} // top_border_line

/// diffattribute
namespace ipc_diff_feature{
    using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kCountry[];           //!< country�R�[�h
	extern const _TCHAR kOrgId[];             //!< �I�u�W�F�N�gID
	extern const _TCHAR kChangeType[];        //!< change�R�[�h
	extern const _TCHAR kCountryAlias[];      //!< country�R�[�h�ʖ�
	extern const _TCHAR kObjectIdAlias[];     //!< �I�u�W�F�N�gID�ʖ�
	extern const _TCHAR kChangeTypeAlias[];   //!< change�R�[�h�ʖ�
}// diffattribute


/// �C���X�g�����N�t�B�[�`���N���X
namespace illust_link {
	using namespace ::sindy::schema::global::common_feature;

	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kIllustClass[];           //!< �C���X�g���
	extern const _TCHAR kCustomerCode[];          //!< �d������R�[�h
	extern const _TCHAR kIllustFileName[];        //!< �摜�t�@�C����
	extern const _TCHAR kGuideSeq[];              //!< �ē��V�[�P���X
	extern const _TCHAR kDirectionCode[];         //!< �ē������R�[�h
	extern const _TCHAR kUTurnFlag[];             //!< U�^�[���t���O
	extern const _TCHAR kRoundaboutFlag[];        //!< ���E���h�A�o�E�g

	extern const int    kMaxNodeNumber;// �ő�m�[�h��
	extern const _TCHAR kNode0[];   //!< ���H�m�[�h0(�g��Ȃ����֋X��)
	extern const _TCHAR kNode1[];   //!< ���H�m�[�h1
	extern const _TCHAR kNode2[];   //!< ���H�m�[�h2
	extern const _TCHAR kNode3[];   //!< ���H�m�[�h3
	extern const _TCHAR kNode4[];   //!< ���H�m�[�h4
	extern const _TCHAR kNode5[];   //!< ���H�m�[�h5
	extern const _TCHAR kNode6[];   //!< ���H�m�[�h6
	extern const _TCHAR kNode7[];   //!< ���H�m�[�h7
	extern const _TCHAR kNode8[];   //!< ���H�m�[�h8
	extern const _TCHAR kNode9[];   //!< ���H�m�[�h9
	extern const _TCHAR kNode10[];  //!< ���H�m�[�h10
	extern const _TCHAR kNode11[];  //!< ���H�m�[�h11
	extern const _TCHAR kNode12[];  //!< ���H�m�[�h12
	extern const _TCHAR kNode13[];  //!< ���H�m�[�h13
	extern const _TCHAR kNode14[];  //!< ���H�m�[�h14
	extern const _TCHAR kNode15[];  //!< ���H�m�[�h15
	extern const _TCHAR kNode16[];  //!< ���H�m�[�h16
	extern const _TCHAR kNode17[];  //!< ���H�m�[�h17
	extern const _TCHAR kNode18[];  //!< ���H�m�[�h18
	extern const _TCHAR kNode19[];  //!< ���H�m�[�h19
	extern const _TCHAR kNode20[];  //!< ���H�m�[�h20

	extern const LPCTSTR kNode[21]; //!< ���H�m�[�h�ꗗ

	// �C���X�g�d������R�[�h
	namespace customer {
		enum ECode {
			kUndefined     = 0,   //!< ���ݒ�
			kCommon        = 1,   //!< ����
		};
	} // illust_customer

	// �C���X�g��ʃR�[�h
	namespace illust_class {
		enum ECode {
			kUndefined         = 0,   //!< ���ݒ�
			kJunction          = 1,   //!< (����)�W�����N�V��������
			kExitFromMain      = 2,   //!< (����)�{���o������
			kBranchOnRamp      = 3,   //!< (����)�����E�o������
			kEntrance          = 4,   //!< (����)����
			kBranchFront       = 5,   //!< ��������
			kIntersection      = 6,   //!< ��ʓ������_
			kCmplIntersection  = 7,   //!< ���G�����_
		};
	} // illust_code
	
	// �C���X�g�ē������R�[�h
	namespace direction_code {
		enum ECode {
			kUndefined     = 0,   //!< ���ݒ�
			kA             = 1,   //!< A
			kB             = 2,   //!< B
			kC             = 3,   //!< C
			kD             = 4,   //!< D
			kE             = 5,   //!< E
			kF             = 6,   //!< F
			kG             = 7,   //!< G
			kH             = 8,   //!< H
			kI             = 9,   //!< I
			kJ             = 10,  //!< J
			kK             = 11,  //!< K
			kL             = 12,  //!< L
			kM             = 13,  //!< M
			kN             = 14,  //!< N
			kO             = 15,  //!< O
		};
	} // direction_code
} // illust_link

/// �X�΃t�B�[�`���N���X
namespace grad{
	using namespace ::sindy::schema::global::common_feature;
	using namespace ::sindy::schema::global::link_relational_table;

	extern const _TCHAR kTableName[]; //!< �e�[�u����
	extern const _TCHAR kGradValue[]; //!< �X�Βl
}


} // global

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_SJ_H_
