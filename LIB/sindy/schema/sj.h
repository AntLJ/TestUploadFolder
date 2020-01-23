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

#ifndef SINDY_SCHEMA_SJ_H_
#define SINDY_SCHEMA_SJ_H_

#include <sindy/schema/common.h>
#include <sindy/schema/map.h>
#include <sindy/schema/annotation.h>

namespace sindy {

namespace schema {

namespace sj{


extern const _TCHAR kRoofTopTableName[];     //!< rooftop�e�[�u����

extern const _TCHAR kWalkwayTableName[];     //!< walkway�e�[�u����

extern const _TCHAR kOtherLCTableName[];     //!< other_lc�e�[�u����

extern const _TCHAR kRailwayTableName[];     //!< railway_ground�e�[�u����

extern const _TCHAR kRoadTableName[];        //!< road_polygon�e�[�u����

extern const _TCHAR kPredominantTableName[]; //!< predominant_lc�e�[�u����

extern const _TCHAR kRoadF[];  //!< ���H�t���O
extern const _TCHAR kLCCLS[];  //!< Land Cover
extern const _TCHAR kLUCLS[];  //!< Land Use

namespace land_cover {
	enum ECode {
		kUndefined               = 0,
		kUnknown_Improved        = 10,
		kPaved                   = 11,
		kCobblestone             = 12,
		kBrick                   = 13,
		kMetal                   = 14,
		kWood                    = 15,
		kRailway_ground          = 16,
		kUnknown_Crushed_ground  = 20,
		kSand                    = 21,
		kClay                    = 22,
		kDirt                    = 23,
		kStone                   = 24,
		kGravel                  = 25,
		kUnknown_Vegetation      = 30,
		kGrass_Meadow            = 31,
		kCropland                = 32,
		kMarshland               = 33,
		kTrees_Forest_Coniferous = 34,
		kTrees_Forest_Deciduous  = 35,
		kTrees_Forest_Mixed      = 36,
		kTrees_Forest_Unknwon    = 37,
		kWater                   = 40,
		kUnknown                 = 50,
	};
} // land_cover

namespace land_use1 {
	enum ECode {
		kNotpplicable       = 0,
		kDiviver            = 11,
		kSidewalks          = 12,
		kOffroad_pathways   = 13,
		kParking            = 14,
		kWater_residential  = 21,
		kWater_recreation   = 22,
		kWater_natural      = 23,
		kWater_industrial   = 24,
		kWater_unknown      = 25,
		kIce                = 26,
		kBaseball           = 31,
		kBasketball         = 32,
		kFootball           = 33,
		kGolf               = 34,
		kSoccer             = 35,
		kTennis             = 36,
		kTrack              = 37,
		kSports_unknown     = 38,
		kUnder_construction = 41,
		kPier               = 51,
	};
} // land_use1

namespace land_use2 {
	enum ECode {
		kNotapplicable     = 0,
		kSidewalks         = 12,
		kOffroad_pathways  = 13,
	};
} // land_use2

namespace land_use3 {
	enum ECode {
		kNotapplicable  = 0,
		kDivider        = 11,
	};
} // land_use3


/// �n���S�����N
namespace subway_link {
	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kFromNodeID[];    //!< �n�_�m�[�hID
	extern const _TCHAR kToNodeID[];      //!< �I�_�m�[�hID
	extern const _TCHAR kRailwayLineID[]; //!< �S���H��ID
}

/// �n���S�m�[�h
namespace subway_node {
	extern const _TCHAR kTableName[];     //!< �e�[�u����

	extern const _TCHAR kNodeClassC[];    //!< �m�[�h���
	extern const _TCHAR kJoinLineInfo[];  //!< ������H��ID
	extern const _TCHAR kStationNumber[]; //!< �w�ԍ�
	
	/// �n���S�m�[�h���
	namespace node_class {
		enum ECode {
			kPublic		= 0,	///< ��ʃm�[�h
			kTerminal	= 1,	///< �[�_�m�[�h
			kStation    = 2,	///< �wPOI�m�[�h
			kOutline	= 99,	///< �}�s��m�[�h
		};
	}
}

/// �����E��������|���S��
namespace nationalpark_site {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kNparkID[];   //!< �����֘A�e�[�u��ID
}

///�����E��������֘A�e�[�u��
namespace nationalpark_table {
	extern const _TCHAR kTableName[];  //!< �e�[�u����

	extern const _TCHAR kParkTypeC[];  //!< ������ʃR�[�h
	extern const _TCHAR kParkNumber[]; //!< �����ԍ�
	extern const _TCHAR kName[];       //!< ����

	namespace park_class {
		enum ECode {
			kUnknown = 0,        ///<�s��
			kNational = 1,       ///<��������
			kQuasiNational = 2,  ///<�������
		};
	}
}

// �s�s�n�}���L�w�i�|���S��
namespace city_anno_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];  //!< �e�[�u����

	extern const _TCHAR kCityAnnoID[]; //!< �s�s���LObjectID
	extern const _TCHAR kAnnoClassC[]; //!< �s�s���L��ʃR�[�h
	extern const _TCHAR kCitySiteID[]; //!< �s�s�w�iObjectID
	extern const _TCHAR kBGClassC[];   //!< �s�s�w�i��ʃR�[�h

	namespace bg_class {
		using namespace sindy::schema::city_site::bg_class;
	} // bg_class

	namespace annotation_code {
		using namespace sindy::schema::city_annotation::annotation_code;
	} // annotation_code

} // city_anno_site

// ���k���L�w�i�R�t���e�[�u��
namespace rel_base_anno_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];  //!< �e�[�u����

	extern const _TCHAR kBaseAnnoID[]; //!< ���k���LID
	extern const _TCHAR kAnnoClassC[]; //!< ���k���L��ʃR�[�h
	extern const _TCHAR kBaseSiteID[]; //!< ���k�w�iObjectID
	extern const _TCHAR kBGClassC[];   //!< ���k�w�i��ʃR�[�h

	namespace bg_class {
		using namespace sindy::schema::base_site::bg_class;
	} // bg_class

	namespace annotation_code {
		using namespace sindy::schema::base_annotation::annotation_code;
	} // annotation_code

} // rel_base_anno_site

// �����r�����̋��ʕ���
namespace buildingname_common {
	extern const _TCHAR kName[];			//!< ����
	extern const _TCHAR kSourceType[];		//!< �o�T�R�[�h

	namespace source_type {
		enum ECode {
			kOwnSurvey		= 0,	//!< �Ǝ�����
			kImmovables		= 1,	//!< �s���Y���
			kFirePreventive	= 2,	//!< �h�ΑΏە�
			kGeoSpace		= 3,	//!< GEOSPACE
			kKankosha		= 4,	//!< ���L��
			kGSSurvey		= 5,	//!< GS���n����
			kAnnoPOI		= 101,	//!< POI���L
			kCompAnnoPOI	= 102,	//!< �⊮�pPOI���L
			kTownPage		= 201,	//!< �^�E���y�[�W
			kExclusion		= 999,	//!< ���O����
		};
	}
} // buildingname_common

// �����r�����̋��ʕ����i�t�����j
namespace buildingname_common_addinfo {
	using namespace sindy::schema::sj::buildingname_common;
	extern const _TCHAR kNameSeparated[];	//!< ���́i�����j
	extern const _TCHAR kYomiSeparated[];	//!< ���~�i�����j
	extern const _TCHAR kBldgClass1[];		//!< ������ʃR�[�h1
	extern const _TCHAR kBldgClass2[];		//!< ������ʃR�[�h2
	extern const _TCHAR kBldgClass3[];		//!< ������ʃR�[�h3
	extern const _TCHAR kFloors[];			//!< �K��
	extern const _TCHAR kSuitable[];		//!< �r�����̑Ó��t���O
	extern const _TCHAR kFixClass[];		//!< ��ʊm��R�[�h

	namespace bldg_class {
		enum ECode {
			kMansion		= 101,		//!< �}���V����
			kApartment		= 102,		//!< �A�p�[�g
			kHousingComplex	= 103,		//!< �c�n
			kDormitory		= 104,		//!< ���E�Б�
			kMiscHousing	= 199,		//!< ���̑��Z��
			kRestaurant		= 201,		//!< ���H�X
			kRetailStore	= 202,		//!< �����X
			kFinancial		= 203,		//!< ���Z�@�ցE�X�֋�
			kAutomotive		= 204,		//!< �����Ԋ֘A�X��
			kMiscStore		= 299,		//!< �T�[�r�X�X�E���̑��X��
			kCommercial		= 301,		//!< �������Ǝ{�݁E���ƃr��
			kOfficeBldg		= 302,		//!< �I�t�B�X�r���E�I�t�B�X�n����
			kGovernmental	= 303,		//!< �������E���I�@��
			kWelfare		= 304,		//!< ��Î{�݁E�����{��
			kEducational	= 305,		//!< ����E�ۈ�{��
			kLodging		= 306,		//!< �h���{��
			kReligious		= 307,		//!< �@���֘A�{��
			kCultural		= 308,		//!< �����{�݁E�ό��E�C�x���g�֘A
			kAmusement		= 309,		//!< ��y�E�X�|�[�c�֘A
			kTransportation	= 310,		//!< ������ʋ@��
			kIndustrial		= 311,		//!< �H��E�q�ɁE������
			kParkingGarage	= 312,		//!< ���̒��ԏ�
			kMiscellaneous	= 999,		//!< ���̑��{�݁E����
		};
	}

	namespace fix_class {
		enum ECode {
			kUnconfirmed		= 0,		//!< �s��
			kFixedByKeyword		= 1,		//!< ��ʊm��i�L�[���[�h��v�j
			kFixedByAgreement	= 2,		//!< ��ʊm��i���S2����v�i���̑���ʈȊO�j�j
			kFixedByManualCheck	= 3,		//!< ��ʊm��i�L�[���[�h�s��v�E�ڎ����O�m�FOK�j
			kRejected			= 9,		//!< �s�̗p�i���ԍ��݂̂̂��̂Ȃǁj
		};
	}
} // buildingname_common_addinfo

// �����r�����̑f�ރ|�C���g
namespace buildingname_src_point {
	using namespace sindy::schema::feature;
	using namespace sindy::schema::sj::buildingname_common;
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kOrgName1[];		//!< ���H�O����1
	extern const _TCHAR kOrgName2[];		//!< ���H�O����2
	extern const _TCHAR kKana[];			//!< �J�i
	extern const _TCHAR kAddr[];			//!< �Z��
	extern const _TCHAR kOrgAddr[];			//!< ���`�O�Z��
	extern const _TCHAR kAddrCode[];		//!< �Z���R�[�h
	extern const _TCHAR kAddrLon[];			//!< �Z���o�x
	extern const _TCHAR kAddrLat[];			//!< �Z���ܓx
	extern const _TCHAR kRepType[];			//!< �n�ԑ�\�_�����R�[�h
	extern const _TCHAR kAddrLevel[];		//!< �Z�����x���R�[�h
	extern const _TCHAR kPinPoint[];		//!< �s���|�C���g�t���O
	extern const _TCHAR kMultiSameBldg[];	//!< ����ƌ`�������Z�����݃t���O
	extern const _TCHAR kSameAddr[];		//!< ����iPC�Z���������݃t���O
	extern const _TCHAR kSameAddrCount[];	//!< ����iPC�Z����
	extern const _TCHAR kAddrVer[];			//!< �Z����͌�XYDB��
	extern const _TCHAR kSourceName[];		//!< �o�T��
	extern const _TCHAR kSourceDate[];		//!< �o�T�������莞��
	extern const _TCHAR kSourceID[];		//!< �o�T����ID
	extern const _TCHAR kBuilding1Div1[];	//!< �p�r�敪1
	extern const _TCHAR kBuilding1Div2[];	//!< �p�r�敪2
	extern const _TCHAR kBuilding2Div1[];	//!< �������p�r�敪1
	extern const _TCHAR kBuilding2Div2[];	//!< �������p�r�敪2
	extern const _TCHAR kHeight[];			//!< ��������
	extern const _TCHAR kFloors[];			//!< �n��K��
	extern const _TCHAR kBasementFloors[];	//!< �n���K��
	extern const _TCHAR kBuildingArea[];	//!< �����ʐ�
	extern const _TCHAR kAllArea[];			//!< �q�׏��ʐ�
	extern const _TCHAR kSrcStartDate[];	//!< �f�ގg�p�J�n�N����
	extern const _TCHAR kSrcUpdateDate[];	//!< �f�ލX�V�N����
	extern const _TCHAR kAnyPlaceID[];		//!< AnyPlaceID
	extern const _TCHAR kUpdateDate[];		//!< �X�V�N����
	extern const _TCHAR kAdoption[];		//!< �̗p�R�[�h

	namespace adoption_code {
		enum ECode {
			kAdopt = 0,						//!< �̗p
			kRejectNoSurvey = 11,			//!< �Ԉ����ɂ��s�̗p(������)
			kRejectUnnecessary = 12,		//!< �Ԉ����ɂ��s�̗p(�s�v)
			kRejectChain = 13,				//!< �Ԉ����ɂ��s�̗p(�`�F�[���n)
			kRejectAddrAnalyze = 21,		//!< �Z����̓��x���s�\���ɂ��s�̗p
			kRejectClosed = 31,				//!< �����ɂ��s�̗p(���E����)
			kRejectSrcDeleted = 32,			//!< �f�ލ폜�ɂ��s�̗p
			kRejectUnkown = 33				//!< �����ɂ��s�̗p(�s��)
		};
	}
	
	namespace chiban_fukugou {
		enum ECode {
			kIndefinite = 0,				//!< �s��
			kAreaOff = 1,					//!< �G���A/OFF
			kOffOn = 2,						//!< OFF/ON
			kOffOff = 3,					//!< OFF/OFF
			kMultiOff = 4,					//!< ����/OFF
			kMultiOn = 5					//!< ����/ON
		};
	}

	namespace addr_level {
		enum ECode {
			kUnconverted = 0,               //!< ���ϊ�
			kPref        = 1,               //!< �s���{�����x��
			kCity        = 2,               //!< �s�撬�����x��
			kChomoku     = 3,               //!< ���ڃ��x��
			kChiban      = 4,               //!< �n�ԃ��x��
			kGou         = 5,               //!< �����x��
		};
	}
	
} // buildingname_src_point

// �����r�����̑f�ޕt�����
namespace buildingname_src_addinfo {
	using namespace sindy::schema::object;
	using namespace sindy::schema::sj::buildingname_common_addinfo;
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kSourceName[];		//!< �o�T��
	extern const _TCHAR kSourceID[];		//!< �o�T����ID
} // buildingname_src_addinfo

// �����r�����̃|�C���g
namespace buildingname_point {
	using namespace sindy::schema::ipc_feature;
	using namespace sindy::schema::sj::buildingname_common_addinfo;
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kBuildingOID[];		//!< �����|���S��OID
	extern const _TCHAR kSrcOID[];			//!< �Q�Ƒf��OID
	extern const _TCHAR kSurveyDate[];		//!< ������
	extern const _TCHAR kAnyPlaceID[];		//!< AnyPlaceID
	extern const _TCHAR kPriority[];		//!< �M���x�R�[�h
	extern const _TCHAR kMatchPer[];		//!< �}�b�`���O��
	extern const _TCHAR kContentsCode[];	//!< ���LPOI�R���e���c�R�[�h
	extern const _TCHAR kContentsSeq[];		//!< ���LPOI�R���e���c���V�[�P���X

	namespace priority_code {
		enum ECode {
			kUnknown				= 0,		//!< �s��
			kEdit					= 1,		//!< �����ɂ���ĕҏW���ꂽ����
			kAnnoPoi				= 2,		//!< ���LPOI���̗p���ꂽ�ꍇ
			kPri3					= 3,		//!< ���S��v�L��E��v������ԑ�������
			kPri4					= 4,		//!< ���H�㊮�S��v�L��E��v������ԑ�������
			kPri5					= 5,		//!< ���S��v����E�D��x����������
			kPri6					= 6,		//!< ���H���v����E�D��x����������
			kPri7					= 7,		//!< ���S��v����E����������������
			kPri8					= 8,		//!< ���H���v�L��E����������������
			kPri9					= 9,		//!< ���S��v����E��������\�[�g���ĎႢ����
			kPri10					= 10,		//!< ���H���v����E��������\�[�g���ĎႢ����
			kPri11					= 11,		//!< ���S��v�����E�}�b�`���O������������
			kPri12					= 12,		//!< ���S��v�����E�D��x����������
			kPri13					= 13,		//!< ���S��v�����E����������������
			kPri14					= 14,		//!< ���S��v�����E��������\�[�g���ĎႢ��
			kOneFirePreventive		= 15,		//!< ������ɖh�ΑΏۏ�񂪃��j�[�N�ɑ��݂��A�̗p���ꂽ����
			kOneImmovalbles			= 16,		//!< ������ɕs���Y��񂪃��j�[�N�ɑ��݂��A�̗p���ꂽ����
			kNPartFirePreventive	= 17,		//!< ������ɖh�ΑΏۏ��̂ݕ������݂��A���̒��ň�̗p���ꂽ����
			kNPartImmovalbles		= 18,		//!< ������ɕs���Y���̂ݕ������݂��A���̒��ň�̗p���ꂽ����
			kCompAnnoPoi			= 19,		//!< ���LPOI�i�⊮�p�j���̗p���ꂽ�ꍇ
		};
	}
} // buildingname_point

// �����r�����̕R�Â��e�[�u��
namespace rel_buildingname {
	using namespace sindy::schema::object;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBuildingNameOID[];	//!< �����r������OID
	extern const _TCHAR kSrcOID[];
} // rel_buildingname

// �S�������N
namespace railway_link {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kFromNodeOID[];			//!< �n�_�m�[�h��OID
	extern const _TCHAR kToNodeOID[];			//!< �I�_�m�[�h��OID
	extern const _TCHAR kRailwayClass_C[];		//!< �S�����
	extern const _TCHAR kDispScale_C[];			//!< �X�P�[���\�����
	extern const _TCHAR kUnderGround_F[];		//!< �n���t���O

	// �S�������N��ʃR�[�h
	namespace link_class{
		enum ECode{
			kDivideByPoi = 0,		//!< �S��POI��ʂŕ���
			kFreightLine = 1,		//!< �ݕ���
			kSideTrack   = 2		//!< �S��������
		};
	}// node_code

	// �X�P�[���\����ʃR�[�h
	namespace disp_scale{
		enum ECode{
			kS1toS4 = 0,		//!< S1�`S4�ŕ\��
			kS2toS4 = 1,		//!< S2�`S4�ŕ\��
			kS3toS4 = 2,		//!< S3�`S4�ŕ\��
			k4      = 3			//!< S4�ŕ\��
		};
	}// disp_scale

} // railway_link

// �S�������N�R�t���e�[�u��
namespace rel_railway_link {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kRwLinkID[];			//!< �S�������N��OID
	extern const _TCHAR kRailwayLineID[];		//!< �S���H����OID
} // rel_railway_link

// �S���m�[�h
namespace railway_node {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kNodeClass_C[];			//!< �m�[�h���

	// �S���m�[�h��ʃR�[�h
	namespace node_class{
		enum ECode{
			kNone      = 0,		//!< �@�\�Ȃ��m�[�h
			kTerminal  = 1,		//!< �[�_�m�[�h
			kStation   = 2,		//!< �wPOI�m�[�h
			kAlter     = 3,		//!< �����ω��_�m�[�h
			kBranch    = 4,		//!< ����m�[�h
			kOutline   = 99		//!< �}�s��m�[�h
		};
	}// node_code

} // railway_node

// �S���m�[�h�R�t���e�[�u��
namespace rel_railway_node {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kRwNodeID[];			//!< �S���m�[�h��OID
	extern const _TCHAR kJoinLineInfoID[];		//!< ������H����OID
	extern const _TCHAR kStationNo[];			//!< �w�ԍ�
} // rel_railway_node


// �S���J���[
namespace railway_color {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kRailwayLineID[];		//!< �S���H����OID
	extern const _TCHAR kColorName[];			//!< �F����
	extern const _TCHAR kColorNameEng[];		//!< �F����(�p��) 
	extern const _TCHAR kColorCode[];			//!< �J���[�R�[�h
} // railway_color


// �ڍ׃|���S��
namespace station_detail {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kPolygonType_C[];		//!< �|���S�����

	// �w�|���S����ʃR�[�h
	namespace polygontype_class{
		enum ECode{
			kMainFacility       = 1,	//!< �w��v�{��
			kCommercialFacility = 2,	//!< �w�֘A���Ǝ{��
			kPlatform           = 3		//!< �w�v���b�g�t�H�[��
		};
	} // polygontype_class

} // station_detail


// �v���b�g�t�H�[���|���S��
namespace midzoom_platform {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // midzoom_platform


// �w�ڍ׃|���S��
namespace highzoom_station {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // highzoom_station


// �w�ȈՃ|���S��
namespace midzoom_station {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // midzoom_station


// �v���b�g�t�H�[���ȈՃ|���S��
namespace lowzoom_platform {
	using namespace sindy::schema::ipc_feature;
	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // lowzoom_platform


// �w�|���S���R�t���e�[�u��
namespace rel_station_site {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kGroupID[];			//!< �wID
	extern const _TCHAR kLayerNo_C[];		//!< ���C���[�ԍ�
	extern const _TCHAR kLayerOID[];		//!< ���C���[��OID

	// �w�|���S�����C���[�ԍ��R�[�h
	namespace layerno_class{
		enum ECode{
			kStationDetail    = 1,		//!< �ڍ׃|���S��
			kMidzoomPlatform  = 2,		//!< �v���b�g�t�H�[���|���S��
			kHighzoomStation  = 3,		//!< �w�ڍ׃|���S��
			kMidzoomStation   = 4,		//!< �w�ȈՃ|���S��
			kLowzoomPlatform  = 5		//!< �v���b�g�t�H�[���S�̃|���S��
		}; // layerno_class
	}
} // rel_station_site


// �O���[�vID�}�X�^�[
namespace groupid_master {
	using namespace sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kStationName[];			//!< �w����
} // groupid_master

// ���H���̃e�[�u��
namespace road_name_list {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kNameClass[];	//!< ���H���̎��
	extern const _TCHAR kNameKanji[];	//!< ����
	extern const _TCHAR kNameYomi[];	//!< �ǂ�

	// ���H�o�H�p��ʃR�[�h
	namespace name_class {
		enum ECode {
			kNone = 0, //!< ���ݒ�
			kShieldRoadNo = 1,		//!< �W�����H�ԍ�
			kShieldHwyNo = 2,		//!< �W�������ԍ�
			kNationalRoadNo = 3,	//!< �����ԍ�
			kPrefRoadNoName = 4,	//!< �����ԍ��E����
			kPrefRoadNo = 5,		//!< �����ԍ�
			kPrefRoadNoAbbr = 6,	//!< �����ԍ�����
			kPrefRoadName = 7,		//!< ��������
			kRoadName = 8,			//!< �H������
			kRoadHwyAbbr = 9,		//!< �������H����
			kRoadCityHwyAbbr = 10,	//!< �s�s��������
			kRoadCityHwyAbbr2 = 11,	//!< �s�s��������2
			kSectionName = 12		//!< ��Ԗ���
		};
	}
} // road_name_list

} // sj

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_SJ_H_
