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

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kNameString1[];		//!< ���L������1
	extern const _TCHAR kNameString2[];		//!< ���L������2
	extern const _TCHAR kStringNum1[];		//!< ���L1������
	extern const _TCHAR kStringNum2[];		//!< ���L2������
	extern const _TCHAR kAnnoClass[];		//!< ���L���
	extern const _TCHAR kNameYomi[];		//!< ���L�ǂ�
	extern const _TCHAR kEngDefault[];		//!< �f�t�H���g�p�꒍�L
	extern const _TCHAR kEngName[];			//!< �ʉp�꒍�L
	extern const _TCHAR kEngOut[];			//!< �p�꒍�L�o�̓t���O
	extern const _TCHAR kDispType[];		//!< �\���^�C�v
	extern const _TCHAR kHimawari[];		//!< �Ђ܂��t���O
	extern const _TCHAR kContentsCode[];	//!< �R���e���c�R�[�h
	extern const _TCHAR kContentsSeq[];		//!< �R���e���c�V�[�P���X
	extern const _TCHAR kAnnoDisp[];		//!< ���L�\���R�[�h

	//// CDS���L��ʃR�[�h
	//namespace annotation_code {
	//	enum ECode {
	//	};
	//} // annotation_code

	// CDS���L�\���^�C�v
	namespace disp_type {
		enum ECode {
			kStr	= 1,	//!< ������̂�
			kSymStr	= 2,	//!< �L���{������
		};
	} // disp_type

} // city_annotation_gs

namespace building_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kBldClass[];		//!< �������
	extern const _TCHAR kCorridor[];		//!< �n��L���t���O
	extern const _TCHAR kNoWall[];			//!< ���ǃt���O
	extern const _TCHAR kBldName[];			//!< ��������
	extern const _TCHAR kBldAddr[];			//!< �����Z��
	extern const _TCHAR kGeospaceID[];		//!< GEOSPACE�ƌ`ID

	// CDS�s�s������ʃR�[�h
	namespace bld_class {
		enum ECode {
			kNone							=        0, //!< �������E��ƒ�
			kNormalRailwayStation			=        1, //!< ���ʓS���w(�n��)
			kSubwayStation					=        2, //!< �n���S�w(�n��)
			kOtherStationHome				=        3, //!< �n���S�ȊO�w�z�[���i�n��j
			kSubwayStationHome				=        4, //!< �n���S�w�z�[���i�n��j
			kGovmentOffice					=        5, //!< ������
			kHospital						=        6, //!< �a�@
			kSchool							=        7, //!< �w�Z
			KHighRiseComplexHousing			=        8, //!< ���w�W���Z��
			kPlant							=        9, //!< �H��E��s�E�r��
			kHotel							=       11, //!< �z�e��
			kDepartmentStore				=       12, //!< ��y�E�f�p�[�g
			kRailroadBuilding 				=       13, //!< �w�r��
			kMarket							=		16, //!< �s��
			kFactory						=		17,	//!< �H��
			kStoreHouse						=		18, //!< �q��
			kSurfaceRailwayStation			=		19,	//!< �H�ʓS���w(�n��)
			kArcade							=		26,	//!< �A�[�P�[�h(��)
			kTollGate						=		27,	//!< ������(��)
			kUniversalTrafficStation		=		28,	//!< �V��ʉw(�n��)
			kMonorailStation				=		29,	//!< ���m���[���w(�n��)
			kCableCarStation				=		30,	//!< �P�[�u���J�[�w(�n��)
			kForce							=		31,	//!< ���q���E�ČR
			kGasStation						=		32, //!< �K�\�����X�^���h
			kConfirmationTenantAttr			=		99, //!< �e�i���g�����m�F
		};
	} // bld_class

} // building_gs

namespace city_line_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kBgClass[];			//!< �w�i���C�����

	// CDS�s�s�w�i���C����ʃR�[�h
	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kDashe		 				=        2, //!< �j���\��
			kWatergate 					=        4, //!< ����
			kErosionControlDam			=		 5,	//!< ���h�_��
			kStairsDecoration			=		15,	//!< �K�i������
			kDivider					=		20,	//!< ���������сi���C���j
			kRoadSite					=		21,	//!< ���H�w�i
			kContour					=      101, //!< ������
			kPrefecturesBorder			=      111, //!< �s���{���E
			kMunicipalityBorder			=      112, //!< �s�����E
			kWardBorder					=      113, //!< ��E
			kOazaBorder					=      114, //!< �厚�E
			kKoazaBorder				=      115, //!< �������E
			kRiver						=      121, //!< �͐�
			kLake                       =      122, //!< ���r�E��
			kCoastline                  =      123, //!< �C�ݐ�
			kLineRoad                   =      131, //!< �����H
			kGardenPath                 =      132, //!< �뉀�H
			kFootBridge                 =      133, //!< ������
			kTank                       =      141, //!< �^���N
		};
	} // bg_class

} // city_line_gs

namespace city_railway_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kRailwayClass[];	//!< �S�����
	extern const _TCHAR kUnderground[];		//!< �n���t���O

	// CDS�s�s�S����ʃR�[�h
	namespace railway_class {
		enum ECode {
			kNone						=         0, //!< �������E��ƒ�
			kNormalRailwayStation		=         1, //!< ���ʓS���w(�n��)
			kSubway 					=         2, //!< �n���S
			kSurfaceRailway			    =		  3, //!< �H�ʓS��
			kSpecialOrbit		    	=		101, //!< ����O��
		};
	} // railway_class

} // city_railway_gs

namespace city_site_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kBgClass[];			//!< �w�i���

	// CDS�s�s�w�i�|���S����ʃR�[�h
	namespace bg_class {
		enum ECode {
			kNone								=         0, //!< �������E��ƒ�
			kPark								=        14, //!< �A�͒n
			kRiver								=		 20, //!< �͐�
			kSea								=		 23, //!< �C
			kLake								=		 24, //!< �r�E��
			kInterCityTollExpressway			=		 30, //!< �s�s�ԍ���(�L��)
			kNationalHighway					=        31, //!< ����
			kPrincipalPrefecturalRoad 			=        32, //!< ��v�n����
			kPrefectureNormalRoad				=		 33, //!< ��ʓs���{���� 
			kSideway							=        34, //!< ����
			kGardenPath							=        35, //!< �뉀�H
			kCenterDivider						=        37, //!< ����������(�W��)
			kInterCityFreeExpressway			=		 38, //!< �s�s�ԍ���(����)
			kOtherSite							=        51, //!< �ܑ��n���̑��~�n
			kUrbanTollExpressway				=		 70, //!< �s�s����(�L��)
			kUrbanFreeExpressway				=		 71, //!< �s�s����(����)
			kTollRoadPolygon					=		 72, //!< �L�����H�|���S��
			kOtherArterialRoad					=		 73, //!< ���̑��������H
			kOtherRoad							=		 74, //!< ���̑���
			kHighwayPolygon						=		101, //!< �������H�|���S���i���̌����j
			kUrbanExpresswayPolygon				=		111, //!< �s�s�������H�|���S���i���̌����j
			kNationalHighwayPolygon				=		121, //!< �����|���S���i���̌����j
			kPrincipalPrefecturalRoadPolygon	=		131, //!< ��v�n�����|���S���i���̌����j
			kPrefectureNormalRoadPolygon		=		141, //!< ��ʓs���{�����|���S���i���̌����j
			kSidewayPolygon						=		161, //!< �����|���S���i�S���H�j
			kTollRoadPolygonCross				=		171, //!< �L�����H�|���S���i���̌����j
		};
	} // bg_class

} // city_site_gs

namespace land_cover_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

} // land_cover_gs

namespace underground_area_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

} // underground_area_gs

namespace bld_tenant_gs {
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kGeospaceID[];		//!< �Ɖ��`��n��ID
	extern const _TCHAR kLClass[];			//!< �啪�ރR�[�h
	extern const _TCHAR kMClass[];			//!< �����ރR�[�h
	extern const _TCHAR kSClass[];			//!< �����ރR�[�h
	extern const _TCHAR kPopularName[];		//!< �ʏ̖���
	extern const _TCHAR kSignageName[];		//!< �\�L����
	extern const _TCHAR kNameKana[];		//!< �J�i����
	extern const _TCHAR kIconClass[];		//!< �A�C�R�����
	extern const _TCHAR kReserved[];		//!< reserved
	extern const _TCHAR kAddrCode11[];		//!< �Z���R�[�h11��
	extern const _TCHAR kPrefKanji[];		//!< ��������
	extern const _TCHAR kCityKanji[];		//!< �����s������
	extern const _TCHAR kOazaKanji[];		//!< �����厚��
	extern const _TCHAR kAzaKanji[];		//!< �������E����
	extern const _TCHAR kBlockKanji[];		//!< �Ԓn
	extern const _TCHAR kGoKanji[];			//!< ��
	extern const _TCHAR kBranchNum[];		//!< �}��
	extern const _TCHAR kPrefKana[];		//!< ���p�J�i����
	extern const _TCHAR kCityKana[];		//!< ���p�J�i�s������
	extern const _TCHAR kOazaKana[];		//!< ���p�J�i�厚��
	extern const _TCHAR kAzaKana[];			//!< ���p�J�i���E����
	extern const _TCHAR kPostNum[];			//!< �X�֔ԍ�

} // bld_tenant_gs

/// �s�s�n�}�w�Ƀ|���S���e�[�u��
namespace city_station_gs {
	using namespace ::sindy::schema::feature;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kStationClass[];		//!< �S���w��ʃR�[�h

	namespace station_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kOtherStation				=        1, //!< �n���S�ȊO�w�i�n���j
			kSubwayStation				=        2, //!< �n���S�w�i�n���j
			kUniversalTrafficStation	=		 3,	//!< �V��ʃV�X�e���i�n���j
			kMonorailStation			=		 4,	//!< ���m���[���i�n���j
			kCableCarStation			=		 5,	//!< �P�[�u���J�[�i�n���j
		};
	} // station_class
} // city_station_gs

} // schema

} // sindy

#endif // SINDY_SCHEMA_CDS_H_
