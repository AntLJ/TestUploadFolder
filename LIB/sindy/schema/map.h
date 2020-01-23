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

#ifndef SINDY_SCHEMA_MAP_H_
#define SINDY_SCHEMA_MAP_H_

#include <sindy/schema/common.h>
#include <sindy/schema/reference.h>

namespace sindy {

namespace schema {

namespace scaledisp_flag {
	extern const _TCHAR kSC1Disp[];			//!< �X�P�[��1�\���t���O
	extern const _TCHAR kSC2Disp[];			//!< �X�P�[��2�\���t���O
	extern const _TCHAR kSC3Disp[];			//!< �X�P�[��3�\���t���O
	extern const _TCHAR kSC4Disp[];			//!< �X�P�[��4�\���t���O
} // scaledisp_flag

namespace scale_class {
	extern const _TCHAR kSC1BgClass[];		//!< �X�P�[��1�w�i��ʃR�[�h
	extern const _TCHAR kSC2BgClass[];		//!< �X�P�[��2�w�i��ʃR�[�h
	extern const _TCHAR kSC3BgClass[];		//!< �X�P�[��3�w�i��ʃR�[�h
	extern const _TCHAR kSC4BgClass[];		//!< �X�P�[��4�w�i��ʃR�[�h
} // scale_class

// �N���[���|���S��
namespace claimpoly {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kClaimType[];		//!< �N���[���^�C�v�R�[�h
	extern const _TCHAR kClaimNo[];			//!< �N���[���ԍ�
	extern const _TCHAR kComFirm[];			//!< �펞�m�F�t���O
	extern const _TCHAR kUseLess[];			//!< �s�v�t���O
	extern const _TCHAR kRemarks[];			//!< �R�����g�A���l

	namespace claim_type {
		enum ECode {
			kBackward			=        1, //!< ���ߋ��g��
			kCSDB				=        2, //!< CSDB
			kCQ					=        3, //!< CQ
			kTopic				=        4, //!< �g�s�b�N
			kTDC				=        5, //!< TDC�f����
			kOther				=        9, //!< ���̑�
		};
	} // claim_type
} // claimpoly

/// �����|���S���e�[�u��
namespace building {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];  //!< �e�[�u����

	extern const _TCHAR kBldClass[];   //!< ������ʃR�[�h
	extern const _TCHAR kCorridor[];   //!< �n��L���t���O
	extern const _TCHAR kNoWall[];     //!< ���ǎɃt���O
	extern const _TCHAR kGeospaceID[]; //!< GEOSPACE_ID

	namespace bld_class {
		enum ECode {
			kNone							=        0, //!< �������E��ƒ�
			kOtherStation					=        1, //!< �n���S�ȊO�w�i�n��j
			kSubwayStation					=        2, //!< �n���S�w�i�n��j
			kOtherStationHome				=        3, //!< �n���S�ȊO�w�z�[���i�n��j
			kSubwayStationHome				=        4, //!< �n���S�w�z�[���i�n��j
			kGovmentOffice					=        5, //!< ������
			kHospital						=        6, //!< �a�@
			kSchool							=        7, //!< �w�Z
			KHighRiseComplexHousing			=        8, //!< ���w�W���Z��
			kPlant							=        9, //!< �H��E��s�E�r��
			kTank							=       10, //!< �^���N
			kHotel							=       11, //!< �z�e��
			kDepartmentStroe				=       12, //!< ��y�E�f�p�[�g
			kRailroadBuilding 				=       13, //!< �w�r��
			kPedestrianDeck 				=       15, //!< �y�f�X�g���A���f�b�L�i���ʁj
			kMarket							=		16, //!< �s��
			kFactory						=		17,	//!< �H��
			kStoreHouse						=		18, //!< �q��
			kSurfaceRailwayStation			=		19,	//!< �H�ʓS���w(�n��)
			kFootBridge						=		20,	//!< ������
			kPedestrianDeckStairsEscalator	=		23,	//!< �؃f�X�g���A���f�b�L�i�K�i�E�G�X�J���[�^�j
			kSiteStairs						=		25,	//!< �~�n�K�i
			kArcade							=		26,	//!< �A�[�P�[�h
			kTollGate						=		27,	//!< ������
			kUniversalTrafficStation		=		28,	//!< �V��ʉw�i�n��j
			kMonorailStation				=		29,	//!< ���m���[���w�i�n��j
			kCableCarStation				=		30,	//!< �P�[�u���J�[�w�i�n��j
			kForce							=		31,	//!< ���q���E�ČR
			kGasStation						=		32, //!< �K�\�����X�^���h
		};
	} // bld_class
} // building

/// �������C���e�[�u��
namespace building_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBldClass[];			//!< �������C����ʃR�[�h

	namespace bld_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kDecoration				=        1, //!< ������
			kStairsDecoration		=		 2,	//!< �K�i������
			kParent					=		 3, //!< �e��
			kStairsTread			=		 4, //!< �K�i���ʃ��C���i�~�n�K�i�E�y�f�K�i�j
		};
	} // bld_class
} // building_line

/// �����i���|���S���e�[�u��
namespace building_step {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kFloors[];				//!< �K��
	extern const _TCHAR kCheck[];				//!< �K�����m�F�t���O
	extern const _TCHAR kBuildingID[];          //!< ����ID
} // building_step

/// �s�s�n�}�w�i�|���S���e�[�u��
namespace city_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBgClass[];				//!< �w�i��ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kBlock						=        1, //!< �X��
			kSands						=       10, //!< ���n
			kMarsh						=       11, //!< ���n
			kForest						=       12, //!< �R��
			kLawnOther					=       13, //!< �Ő������̑��Βn
			kPark						=       14, //!< �A�͒n
			kRiver						=		20,	//!< �͐�
			kLevee						=       21, //!< ��O�n
			kPool						=       22, //!< �v�[��
			kSea						=		23, //!< �C
			kLake						=		24,	//!< �r�E��
			kInterCityTollExpressway	=		30,	//!< �s�s�ԍ������i�L���j
			kNationalHighway			=       31, //!< ����
			kPrincipalPrefecturalRoad 	=       32, //!< ��v�n����
			kPrefectureNormalRoad		=		33,	//!< ��ʓs���{���� 
			kSideway					=       34, //!< ����
			kGardenPath					=       35, //!< �뉀�H
			kCenterDivider				=       37, //!< ���������сi�W���j
			kInterCityFreeExpressway	=		38,	//!< �s�s�ԍ������i�����j
			kPublicRoadStairs			=		39,	//!< �����K�i
			kHospital					=       40, //!< �a�@
			kSchool						=       41, //!< �w�Z
			kPlant						=       42, //!< �H��
			kDamGuideNotExist			=       45, //!< �_���i�֗��L�ڂȂ��j
			kDamGuideExist				=       46, //!< �_���i�֗��L�ڂ���j
			kStationSite				=       50, //!< �S���~�n
			kOtherSite					=       51, //!< �ܑ��n���̑��~�n
			kBreakwater					=       52, //!< �h�g��
			kForceManeuveringGround		=		55,	//!< ���q���E�ČR�i���K��j
			kForce						=		56,	//!< ���q���E�ČR�i���Ԓn�ق��j
			kIncompleteArea 			=       60, //!< �s���S�G���A
			kUrbanTollExpressway		=		70,	//!< �s�s�����i�L���j
			kUrbanFreeExpressway		=		71,	//!< �s�s�����i�����j
			kTollRoad					=		72,	//!< �L�����H
			kOtherArterialRoad			=		73,	//!< ���̑��������H
			kCenterDividerPark			=       75, //!< ���������сi�A�͒n�j
			kRunway						=		76,	//!< �����H
			kUnderRepairRoad			=		77, //!< �H�������H
			kTollGateLane				=		78, //!< ���������[��
			kNarrow						=		79, //!< �׊X�H
		};
	} // bg_class
} // city_site

/// �s�s�n�}�w�i���C���e�[�u��
namespace city_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBgClass[];				//!< �w�i���C����ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kSolid						=        1, //!< �����\��
			kDashe		 				=        2, //!< �j���\��
			kTollbooth					=        3, //!< ������
			kWatergate 					=        4, //!< ����
			kErosionControlDam			=		 5,	//!< ���h�_��
			kFerrySeaway 				=       10, //!< �t�F���[�q�H
			kPassengerShipSeaway		=       11, //!< ���̑����q�D�q�H
			kStairsDecoration			=		15,	//!< �K�i������
			kParent						=		16, //!< �e��
			kStairsTread				=		17, //!< �K�i���ʃ��C���i�����K�i�j
			kDivider					=		20,	//!< ���������сi���C���j
			kRoadSite					=		21,	//!< ���H�w�i
			kExpressway					=		22,	//!< �������H
			kOverheadRoad				=		23,	//!< ���H����
			kOverheadRailway			=		24,	//!< �S������
			kChockLine					=		25, //!< �Ԏ~�߃��C��
		};
	} // bg_class
} // city_line

/// �s�s�n�}�w�Ƀ|���S���e�[�u��
namespace city_station {
	using namespace ::sindy::schema::ipc_table;

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
} // city_station

/// �s�s�n�}�S�����C���e�[�u��
namespace city_railway {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kRailwayClass[];		//!< �S����ʃR�[�h

	namespace railway_class {
		extern LPCTSTR kFieldName; //!< �t�B�[���h��
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kNormalRailway			=		 1,	//!< ���ʓS��
			kSubway					=        2, //!< �n���S
			kSurfaceRailway			=		 3, //!< �H�ʓS��
			kRollerCoster			=		 4,	//!< �W�F�b�g�R�[�X�^�[��
			kUniversalTraffic		=		 5,	//!< �V��ʃV�X�e��
			kMonorail				=		 6,	//!< ���m���[��
			kCableCar				=		 7,	//!< �P�[�u���J�[
		};
	} // railway_class
} // city_railway

/// �n���|���S��(���s�҃i�r����)
namespace underground_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kUgClass[];			//!< �n���w�i���

	namespace ug_class {
		enum ECode {
			kUnder			=	1,			//!< �n��
			kStairs			=	2			//!< �n���K�i
		};
	} // ug_class

} // underground

/// �n�����C��(���s�҃i�r����)
namespace underground_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kUgClass[];			//!< �n���w�i���C�����

	namespace ug_class {
		enum ECode {
			kStairsDecoration		=		 2,	//!< �K�i������
			kParent					=		 3	//!< �e��
		};
	} // ug_class

} // underground_line

/// �s�s�n�}�s���E�|���S���e�[�u��
namespace city_admin {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
	extern const _TCHAR kAddrCode[];			//!< �Z���R�[�h
	extern const _TCHAR kAddrCode2[];			//!< �Z���R�[�h2
	extern const _TCHAR kTmpCode[];				//!< ���Z���R�[�h�t���O
	extern const _TCHAR kGaikuFugo[];			//!< �X�敄��
	extern const _TCHAR kExtGaikuFugo[];		//!< �g���X�敄��
	extern const _TCHAR kAddrClass[];			//!< �Z���������
	extern const _TCHAR kAreaClass[];			//!< �G���A���ʎ��
	extern const _TCHAR kManage[];				//!< �Ǘ��t���O
	extern const _TCHAR kDetach[];				//!< ��n�t���O
	extern const _TCHAR kColorCode[];			//!< �X��F�t�^�p�F����
	extern const _TCHAR kWorkColor[];			//!< �X��F�t�^��Ɨp
	extern const _TCHAR kConvType[];			//!< �Z���ϊ����[�h

	namespace addr_class {
		enum ECode {
			kAzaUndeveloped								=        0, //!< �����ڊE������
			kResidenceIndicationMaintenance 		 	=        1, //!< �Z���\�������ς�
			kChibanMaintenance							=        2, //!< �n�Ԑ����ς�
			kResidenceIndicationAndChibanUndeveloped	=        3, //!< �Z���\���E�n�Ԗ�����
		};
	} // addr_class

	namespace area_class {
		enum ECode {
			kOutArea				=        0, //!< �G���A�O
			kInArea					=        1, //!< �G���A��
			kSea					=        2, //!< �C
		};
	} // area_class

	namespace conv_type {
		enum ECode {
			kNormal					=        0, //!< �ʏ�
			kOazaGeneration			=        1, //!< �厚������
			kAzaGeneration			=        2, //!< ��������
		};
	} // conv_type
} // city_admin

namespace city_rep_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kChibanNo[];			//!< �n��
	extern const _TCHAR kAnno[];				//!< �n�Ԓ��L�t���O
	extern const _TCHAR kPointClass[];			//!< �|�C���g���
	extern const _TCHAR kAddrCode[];			//!< �Z���P�P���R�[�h
	extern const _TCHAR kRepType[];				//!< ��\�_�^�n�Ԏ~�܂�t���O

	namespace point_class {
		enum ECode {
			kChiban						=        0, //!< �n�ԁi16���j
			kAza						=        1, //!< �����ځE���i11���j
			kOaza						=        2, //!< �厚�i8���j
		};
	} // point_class

	namespace rep_type {
		enum ECode {
			kIncertitude 				=        0, //!< �s��i�厚�E�����ڑ�\�_�̎��̂݁j
			kArea_OFF					=        1, //!< �G���A/OFF
			kOFF_ON						=        2, //!< OFF/ON
			kOFF_OFF					=        3, //!< OFF/OFF
			kMultiple_OFF				=        4, //!< ����/OFF
			kMultiple_ON				=        5, //!< ����/ON
		};
	} // rep_type
} // city_rep_point

namespace cityplanmap {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kMapID[];				//!< �}�tID
	extern const _TCHAR kCityCode[];			//!< �s�����R�[�h
	extern const _TCHAR kCityName[];			//!< �s����
	extern const _TCHAR kMapName[];				//!< �}�t��
	extern const _TCHAR kMapNo[];				//!< �}�tNO
	extern const _TCHAR kPubDate[];				//!< �쐬�N�x
} // cityplanmap

/// ���k�w�i�|���S���e�[�u��
namespace base_site {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scale_class;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kNoDisp						=        1, //!< ��\��
			kSea						=   101001, //!< �C
			kRiver						=   101002, //!< �͐�E��E�J
			kLake						=   101003, //!< �΁E�r�E��
			kSands						=   102001, //!< ���n
			kMarsh						=   102002, //!< ���n
			kLevee						=   102003, //!< ��O�n
			kGround						=   102005, //!< �n�ʁi�|���S�����n�ʁj
			kParking					=   201001, //!< ���ԏ�
			kRoadStation				=   201002, //!< ���̉w
			kTrafficHome				=   201003, //!< ���̑���ʊ֘A�{��
			kRailwayHome				=   202001, //!< ���̑��̓S���{��
			kFerryTerminal				=   203001, //!< �t�F���[�^�[�~�i���̕~�n
			kAirport					=   203002, //!< ��`�̕~�n
			kPrivateAirport				=   203003, //!< ���Ԕ�s��̕~�n
			kHeliport					=   203004, //!< �w���|�[�g�̕~�n
			kMarina						=   203005, //!< ���b�g�n�[�o�[�E�}���[�i
			kRunway						=	203006,	//!< �����H
			kPref						=   301001, //!< �s���{����
			kCityHall					=   301002, //!< �s����
			kWardOffice					=   301003, //!< �����
			kTownOffice					=   301004, //!< ��������
			kBranchOffice				=   301005, //!< �s��������o����
			kGovmentOffice				=   302001, //!< ������
			kCourt						=   302002, //!< �ٔ���
			kPrison						=   302003, //!< �Y����
			kHealth						=   302004, //!< �ی���
			kPoliceOffice				=   302005, //!< �x�@��
			kFireOffice					=   302006, //!< ���h��
			kTaxOffice					=   302007, //!< �Ŗ���
			kPostOffice					=   302008, //!< �X�֋�
			kForestManagedSite			=   302009, //!< �X�ъǗ����̕~�n
			kSelfDifenceForce			=   303001, //!< ���q��
			kUnitedStatesArmy			=   303002, //!< �ČR
			kPublicHall					=   304001, //!< ������
			kImperialFamily				=   304002, //!< �c���{��
			kPublicInstitution			=   304003, //!< ���̑��̌����{��
			kElementarySchool			=   401001, //!< ���w�Z
			kJuniorHighSchool			=   401002, //!< ���w�Z
			kSeniorHighSchool			=   401003, //!< ���Z
			kSpecialHighSchool			=   401004, //!< ����
			kJuniorCollege				=   401005, //!< �Z����w
			kUniversityCollege			=   401006, //!< ��w�E��w�Z
			kSpecialSchool				=   401007, //!< ��C�w�Z�E�e��w�Z
			kNurseSchool				=   401008, //!< �ӁE�W�E�{��w�Z
			kDriversSchool				=   401009, //!< �����Ԋw�Z
			kSchool						=   401010, //!< ���̑��̊w�Z
			kHospital					=   402001, //!< �a�@
			kPark						=   501001, //!< �����E�뉀�E�Βn�E�A�͒n
			kAmusementPark				=   501002, //!< �V���n
			kThemePark					=   501003, //!< �e�[�}�p�[�N
			kResort						=   501004, //!< �������]�[�g
			kZoo						=   501005, //!< ������
			kAquarium					=   501006, //!< ������
			kBotanicalGarden			=   501007, //!< �A����
			kArtMuseum					=   501008, //!< ���p��
			kMuseum						=   501009, //!< �����فE���w�فE�Ȋw�فE������
			kLibrary					=   501010, //!< �����}����
			kStudyMuseum				=   501011, //!< �̌��E�w�K��
			kPlayhouse					=   501012, //!< �z�[���E����E�������
			kRaceTrack					=   501013, //!< ���n��
			kBicycleRaceTrack			=   501014, //!< ���֏�
			kMotorbortRaceTrack			=   501015, //!< ������
			kMotorcycleRaceTrack		=   501016, //!< �I�[�g���[�X��
			kSwimmingPool				=   501017, //!< �v�[��
			kSpaHouse					=   501018, //!< ���N�����h
			kAmusementFacilities		=   501019, //!< ���̑��̌�y�{��
			kFamilyRestaurant			=   502001, //!< �t�@�~���[���X�g����
			kFastFood					=   502002, //!< �t�@�[�X�g�t�[�h
			kRestaurant					=   502003, //!< ���H�X�E���X�g����
			kDepartmentStore			=   502004, //!< �S�ݓX
			kShoppingCenter				=   502005, //!< �V���b�s���O�Z���^�[
			kSupermarket				=   502006, //!< �X�[�p�[
			kCommerseFacillities		=   502007, //!< �������Ǝ{��
			kDIY						=   502008, //!< �c�h�x
			kCarDealer					=   502009, //!< �J�[�f�B�[���[
			kCarShop					=   502010, //!< �����ԗp�i�X
			kSpatialiryStore			=   502011, //!< ���X
			kTouristMarket				=   502012, //!< ���Y�فE�ό��s��
			kMarket						=   502013, //!< �s��
			kWeddingHall				=   502014, //!< ��������
			kAthleticGround				=   503001, //!< ���㋣�Z��
			kGym						=   503002, //!< �̈��
			kBaseballGround				=   503003, //!< �싅��
			kSkiingGround				=   503004, //!< �X�L�[��
			kSoccerGround				=   503005, //!< �T�b�J�[��
			kRugbyGround				=   503006, //!< ���O�r�[��
			kPublicGolfLinks			=   503007, //!< �p�u���b�N�S���t��
			kSemipublicGolfLinks		=   503008, //!< �Z�~�p�u���b�N�S���t��
			kMembersGolfLinks			=   503009, //!< �����o�[�Y�S���t��
			kGolfLinks					=   503010, //!< ���̑��̃S���t��
			kDrivingRange				=   503011, //!< �S���t���K��
			kTennisCourt				=   503012, //!< �e�j�X�R�[�g
			kPlayPark					=   503013, //!< �����X�|�[�c����
			kMotorSportsPark			=   503014, //!< ���[�^�[�X�|�[�c�{��
			kCyclingPark				=   503015, //!< �T�C�N���X�|�[�c�Z���^�[
			kRidingTrack				=   503016, //!< ��n
			kAutoCampingSite			=   503017, //!< �I�[�g�L�����v��
			kCampingSite				=   503018, //!< �ʏ�L�����v��
			kBowlingAlley				=   503019, //!< �{�E�����O��
			kShootingGallery			=   503020, //!< �ˌ���
			kOtherSportsPark			=   503021, //!< ���̑��̃X�|�[�c�{��
			kCastle						=   504001, //!< ��E���
			kHistoricSite				=   504002, //!< �j�ՁE�����E�V�R�L�O��
			kObservatory				=   504003, //!< �W�]��
			kSpa						=   504004, //!< ����
			kRanch						=   504005, //!< �q��
			kTuristRanch				=   504006, //!< �ό��q��
			kTuristFarm					=   504007, //!< �ό��_��
			kTuristOrchard				=   504008, //!< �ό��ʎ���
			kPleasureBoat				=   504009, //!< �ό��V���D���
			kPlant						=   601001, //!< �H��
			kLaboratory					=   601002, //!< ������
			kPublicLabo					=   601003, //!< ���݂̌�����
			kDispositionPlant			=   601004, //!< �򐅏�E�I��������
			kPowerPlant					=   601005, //!< ���d���E�ϓd��
			kStoreHouse					=   601006, //!< �q��
			kBank						=   602001, //!< ��s
			kNTT						=   602002, //!< �m�s�s
			kOtherPrincipalCorporate	=   602003, //!< ���̑��̏d�v��Ǝ{��
			kShintoShrine				=   603001, //!< �_��
			kBuddhistTemple				=   603002, //!< ���@
			kChurch						=   603003, //!< �L���X�g����E�V�哰
			kCemetery					=   603004, //!< ��n
			kReligionSite				=   603005, //!< ���̑��̏@���{��
			kYouthHouse					=   604001, //!< �N�̉�
			kMine						=   604002, //!< �z�R
			kFuneralHall				=   604003, //!< �֏�
			kInstitution				=   604004, //!< ���̑��{��
			kHotel						=   701001, //!< �z�e��
			kPublicInn					=   701002, //!< �����h���{��
			kDam						=   801001, //!< �_��
			kMausoleum					=   801002, //!< �˕�
			kBuilding					=   900001, //!< �����i�|���S�����{�݁j
		};
	} // bg_class
} // base_site

/// ���k�w�i���C���e�[�u��
namespace base_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;


	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kBgClass[];				//!< �w�i���C����ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kRiver					=        1, //!< �͐�E��E�J
			kBreakwater				=        2, //!< �h�g��
			kPrefecturesBorder		=       11, //!< �s���{���E
			kMunicipalityBorder		=       12, //!< �s�撬���E
		};
	} // bg_class
} // base_line

/// ���k�w�i�w�Ƀ|���S���e�[�u��
namespace base_station {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kStationClass[];		//!< �S���w��ʃR�[�h

	namespace station_class {
		enum ECode {
			kNone							=        0, //!< �������E��ƒ�
			kBulletTrainStation 			=        1, //!< �V�����w
			kJRStation						=        2, //!< �i�q�w
			kThirdSectorCorporationStation	=        3, //!< ��R�Z�N�^�[�w
			kPrivateRailwayStation			=        4, //!< ���S�w
			kSurfaceRailwayStation			=        5, //!< �H�ʓS���w
			kSubwayStation					=        6, //!< �n���S�w
			kMonorailStation				=        7, //!< ���m���[���w
			kUniversalTrafficStation		=        8, //!< �V��ʉw
			kCableCarStation				=       10, //!< �P�[�u���J�[�w
			kGoodsLineStation				=       11, //!< �ݕ����w
		};
	} // station_class
} // base_station

/// ���k�w�i�S�����C���e�[�u��
namespace base_railway {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kRailwayClass[];		//!< �S����ʃR�[�h

	namespace railway_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kBulletTrain			=        1, //!< �V����
			kJR						=        2, //!< �i�q
			kThirdSectorCorporation	=        3, //!< ��R�Z�N�^�[
			kPrivateRailway			=        4, //!< ���S
			kSurfaceRailway			=        5, //!< �H�ʓS��
			kSubway					=        6, //!< �n���S
			kMonorail				=        7, //!< ���m���[��
			kUniversalTraffic		=        8, //!< �V���
			kAerialCableway			=        9, //!< ���[�v�E�F�C
			kCableCar				=       10, //!< �P�[�u���J�[
			kGoodsLine				=       11, //!< �ݕ���
			kRailwaySideTrack		=       20, //!< �S��������
		};
	} // railway_class
} // base_railway

/// ���k�w�i�n�ʃe�[�u��
namespace base_land {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // base_land

/// ���k�s���E�|���S���e�[�u��
namespace base_admin {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
} // base_admin

namespace base_rep_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAddrCode[];			//!< �Z���P�P���R�[�h
	extern const _TCHAR kAdminName[];			//!< ��\�_����
	extern const _TCHAR kZipCode[];				//!< �X�֔ԍ�
} // base_rep_point

/// ���k�i�ʃ|���S���e�[�u��
namespace base_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCntClass[];			//!< �i�ʎ�ʃR�[�h

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			k500_1000					=        2, //!< �i��500m�ȏ�1000m����
			k1000_1500					=        3, //!< �i��1000m�ȏ�1500m����
			k1500_2000					=        4, //!< �i��1500m�ȏ�2000m����
			k2000_2500					=        5, //!< �i��2000m�ȏ�2500m����
			k2500_3000					=        6, //!< �i��2500m�ȏ�3000m����
			k3000_3500					=        7, //!< �i��3000m�ȏ�3500m����
			k3500_4000					=        8, //!< �i��3500m�ȏ�4000m����
		};
	} // cnt_class
} // base_contour

/// �~�h���w�i�|���S���e�[�u��
namespace middle_site {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scale_class;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kNoDisp					=        1, //!< ��\��
			kSea					=   101001, //!< �C
			kRiver					=   101002, //!< �͐�E��E�J
			kLake					=   101003, //!< �΁E�r�E��
			kAirport				=   203002, //!< ��`�̕~�n
			kRunway					=	203006,	//!< �����H
			kUrbanArea				=   300000, //!< �s�X�n
		};
	} // bg_class
} // middle_site

/// �~�h���w�i���C���e�[�u��
namespace middle_line {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::scaledisp_flag;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kUnderGround[];			//!< �n���t���O
	extern const _TCHAR kBgClass[];				//!< �w�i���C����ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kRiver						=        1, //!< �͐�E��E�J
		};
	} // bg_class
} // middle_line

/// �~�h���i�ʃe�[�u��
namespace middle_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCntClass[];			//!< �i�ʎ�ʃR�[�h

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			k0_500						=        1, //!< �i��0���ȏ�500m����
			k500_1000					=        2, //!< �i��500m�ȏ�1000m����
			k1000_1500					=        3, //!< �i��1000m�ȏ�1500m����
			k1500_2000					=        4, //!< �i��1500m�ȏ�2000m����
			k2000_2500					=        5, //!< �i��2000m�ȏ�2500m����
			k2500_3000					=        6, //!< �i��2500m�ȏ�3000m����
			k3000_3500					=        7, //!< �i��3000m�ȏ�3500m����
			k3500_4000					=        8, //!< �i��3500m�ȏ�4000m����
		};
	} // cnt_class
} // middle_contour

/// �g�b�v�w�i�|���S���e�[�u��
namespace top_site {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBgClass[];				//!< �g�b�v�w�i��ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kSea					=   101001, //!< �C
			kRiver					=   101002, //!< �͐�E��E�J
			kLake					=   101003, //!< �΁E�r�E��
			kSands					=   102001, //!< ���n
		};
	} // bg_class
} // top_site

/// �g�b�v�w�i���C���e�[�u��
namespace top_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kBgClass[];				//!< �w�i���C����ʃR�[�h

	namespace bg_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kRiver						=        1, //!< �͐�E��E�J
			kNationalBorderLine			=       10, //!< ������
			kPrefecturesBorder			=       11, //!< �s���{���E
		};
	} // bg_class
} // top_line

/// �g�b�v�i�ʃe�[�u��
namespace top_contour {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCntClass[];			//!< �i�ʎ�ʃR�[�h

	namespace cnt_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			k0_500						=        1, //!< �i��0���ȏ�500m����
			k500_1000					=        2, //!< �i��500m�ȏ�1000m����
			k1000_1500					=        3, //!< �i��1000m�ȏ�1500m����
			k1500_2000					=        4, //!< �i��1500m�ȏ�2000m����
			k2000_2500					=        5, //!< �i��2000m�ȏ�2500m����
			k2500_3000					=        6, //!< �i��2500m�ȏ�3000m����
			k3000_3500					=        7, //!< �i��3000m�ȏ�3500m����
			k3500_4000					=        8, //!< �i��3500m�ȏ�4000m����
			k4000_4500					=        9, //!< �i��4000m�ȏ�4500m����
		};
	} // cnt_class
} /// top_contour

/// �I���\�G���A�e�[�u��
namespace ortho_area {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kTileName[];			//!< 
} // orth_area

namespace adminarea {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
	extern const _TCHAR kCityName[];			//!< �s�撬����
} // adminarea

namespace aerial_photo {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kMeshCode[];			//!< �s�s�n�}���b�V���R�[�h
	extern const _TCHAR kBinderName[];			//!< �o�C���_�[��
	extern const _TCHAR kCourse[];				//!< �B�e�R�[�X
	extern const _TCHAR kPhotoNo[];				//!< �ʐ^�ԍ�
	extern const _TCHAR kShootYear[];			//!< �ʐ^�B�e�N
	extern const _TCHAR kShootDate[];			//!< �ʐ^�B�e�N����
	extern const _TCHAR kOwnerCode[];			//!< ���ݒn

	namespace owner {
		enum ECode {
			kMorioka						=        1, //!< ����
			kShanghai						=        2, //!< ��C
		};
	} // owner
}

namespace daikei_point {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAddrCode[];			//!< �Z���R�[�h
	extern const _TCHAR kAddrStr[];				//!< �Z��������
	extern const _TCHAR kSrcType[];				//!< �f�[�^�\�[�X�^�C�v
	extern const _TCHAR kDupType[];				//!< �d���^�C�v
} // daikei_point

namespace parea {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAddrCode[];			//!< PAREA�Z���R�[�h
	extern const _TCHAR kRepPointCode[];		//!< ��\�_�R�[�h
	extern const _TCHAR kUpdateCode[];			//!< �X�V�R�[�h
	extern const _TCHAR kTmpCode[];				//!< ���R�[�h�t���O

	namespace update {
		enum ECode {
			kMasterCode						=        0, //!< �}�X�^�[�R�[�h
			kTmpCode						=        1, //!< ���R�[�h
			kFixCode						=        2, //!< �C���R�[�h
		};
	} // update

	namespace rep_oint {
		enum ECode {
			kOazaRepPoint				=        0, //!< �厚��\�_
			kTmpAzaRepPoint				=        1, //!< �������ڑ�\�_
		};
	} // rep_point
} // parea

/// ���e�[�u��
namespace gou_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kGouNo[];				//!< ���ԍ�
	extern const _TCHAR kAddrCode[];			//!< 20���Z���R�[�h
	extern const _TCHAR kBuildRF[];				//!< �����֘A�t���O
	extern const _TCHAR kBuildID[];				//!< �Ή�����ID
	extern const _TCHAR kExpGouNo[];			//!< �g���ԍ��t���O
	extern const _TCHAR kTmpCheck[];			//!< �m�F�t���O
	extern const _TCHAR kTmpFloors[];			//!< �K�����
	extern const _TCHAR kGouType[];				//!< ���^�C�v
	extern const _TCHAR kPriority[];			//!< �D��t���O
	extern const _TCHAR kPrecisionCode[];		//!< �Z����v����
	extern const _TCHAR kConfirm[];				//!< �Z���m�F�X�e�[�^�X�R�[�h
	extern const _TCHAR kHimawari[];			//!< �Ђ܂��t���O

	namespace confirm {
		enum ECode {
			kNone					=        0, //!< ���m�F
			kUndevelopedAreaNG		=        1, //!< �������G���ANG
			kProgramNG				=		 2,	//!< �v���O�����m�FNG
			kWatchingNG				=		 3,	//!< �ڎ��m�F�m�FNG
			kNeedPublicChart		=		 4,	//!< �v���}�m�F
			kProgramOK16			=		 5,	//!< �v���O�����m�FOK�i16���j
			kProgramOK				=		 6,	//!< �v���O�����m�FOK
			kWatchingOK				=		 7,	//!< �ڎ��m�FOK
			kResearchReferenceOK	=		 8,	//!< ��������OK
			kPublicReferenceOK		=		 9,	//!< ���I����OK
		};
	} // confirm

} // gou_point

// �ƌ`�����N�e�[�u��
namespace klink_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAddrCodeOrg[];			//!< ����22���R�[�h
	extern const _TCHAR kAddrCode[];			//!< �ŐV22���R�[�h
	extern const _TCHAR kKatagakiName[];		//!< �^�E���y�[�W��������
	extern const _TCHAR kKeisaiName[];			//!< �^�E���y�[�W�f�ږ���
	extern const _TCHAR kInfoSrc[];				//!< ���񋟌��R�[�h
	extern const _TCHAR kInputSeido[];			//!< ���W���x��ʃR�[�h
	extern const _TCHAR kReleaseSeido[];		//!< ���W���x��ʃR�[�h
	extern const _TCHAR kBuildName[];			//!< �r������
	extern const _TCHAR kBuildInfoSrc[];		//!< �r�����񋟌��R�[�h
	extern const _TCHAR kMaxFloors[];			//!< �ŏ�K��
	extern const _TCHAR kFloor[];				//!< �K��
	extern const _TCHAR kNequal[];				//!< 11���s��v�t���O

	namespace info_src {
		enum ECode {
			kUnlooked				=        0, //!< �������i���W�����j
			kWallLinkData			=        1, //!< �ƌ`�����N�f�[�^
			kGouMaintenanceData 	=        2, //!< �������f�[�^
			kWorking				=       10, //!< ��ƒ�
		};
	} // info_src

	namespace input_seido {
		enum ECode {
			kNone						=        0, //!< ������
			kNoVerification 			=        1, //!< ������
			kMultipleWallCounterData	=        2, //!< �Ή��ƌ`�����i�f�[�^�j
			kNotWallCounterData			=        3, //!< �Ή��ƌ`�Ȃ��i�f�[�^�j
			kAddrClericalMistake 		=        4, //!< �Z������L
			kUnmatchedName				=        5, //!< ���̕s��v
			kUnmatchedShape				=        6, //!< �`��s��v
			kMultipleWallCounterSrc		=        7, //!< �Ή��ƌ`�����i�����j
			kNotWallCounterSrc			=        8, //!< �Ή��ƌ`�Ȃ��i�����j
			kUnmatchedBlock				=        9, //!< �X��s��v
			kOther						=       10, //!< ���̑�
		};
	} // input_seido

	namespace release_seido {
		enum ECode {
			kNone					=        0, //!< ������
			kMunicipalityBorder		=        1, //!< �s��S�����E
			kOazaBorder				=        2, //!< ���E�厚�E
			kAzaBorder				=        3, //!< ���ځE���i�厚�j�E
			kBlock					=        4, //!< �X��i�n�ԁj
			kPinpoint 				=        5, //!< �s���|�C���g
			kWall					=        6, //!< �ƌ`
			kUnmatchedBlock			=        9, //!< �X��s��v
			kOther					=       10, //!< ���̑�
		};
	} // release_seido

	namespace buildinfo_src {
		enum ECode {
			kUnknown				=        0, //!< ���̕s��
			kAutoGeneration 		=        1, //!< ��������
			kPhoneSurvey 			=        2, //!< �d�b����
			kReference				=        3, //!< �Q�l����
		};
	} // buildinfo_src
} // klink_point



// �Z���N���[���|���S���e�[�u��
namespace claimpoly_addr {
	using namespace ::sindy::schema::claimpoly;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // claimpoly_addr

// �n�}�N���[���|���S���e�[�u��
namespace claimpoly_map {
	using namespace ::sindy::schema::claimpoly;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
} // claimpoly_map

// �Z���N���[���|�C���g�e�[�u��
namespace cs_addr_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kAddrCode[];			//!< �Q�O���Z���R�[�h
} // cs_addr_point

namespace city_admin_c {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
} // city_admin_c

namespace city_admin_pref {

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kCityCode[];			//!< �s�撬���R�[�h
} // city_admin_pref

// ExTP�|�C���g�e�[�u��
namespace extp_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR	kAddrCode[];			//!< 20���Z���R�[�h
	extern const _TCHAR	kAddrName[];			//!< �Z����
	extern const _TCHAR	kKatagakiName[];		//!< �^�E���y�[�W��������
	extern const _TCHAR	kKeisaiName[];			//!< �^�E���y�[�W�f�ږ���
	extern const _TCHAR	kAddPoint[];			//!< �Z�������|�C���g���t���O
	extern const _TCHAR	kInfoSrc[];				//!< �������W�񋟌��R�[�h
	extern const _TCHAR kPointStat[];			//!< �������ʃR�[�h

	namespace info_src {
		enum ECode {
			kiPC	= 1,	//!< iPC
			kAlps	= 2,	//!< �A���v�X
			kiPC_4	= 3,	//!< iPC(���x4)
		};
	} // info_src

	namespace point_stat {
		enum ECode {
			kNone					= 0,	//!< ������
			kUnknownPos				= 1,	//!< �ʒu�s��
			kGou					= 2,	//!< GOU����
			kGouEx					= 3,	//!< GOU�g������
			kTPG					= 4,	//!< TPG����
			kUndergroundNo			= 5,	//!< �n���X�ԍ�
			kReferenceNequal		= 6,	//!< �����ԍ��ƕs��v
			kAdminNequal			= 7,	//!< �s���E�s��v
			kNoNotKeisai			= 8,	//!< �ԍ����f��
			kInstitutionNotKeisai	= 9,	//!< �{�ݖ��f��
		};
	} // point_stat

} // extp_point

// TelTP�|�C���g�e�[�u��
namespace teltp_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR	kTelNum[];				//!< �d�b�ԍ�
	extern const _TCHAR	kKatagakiName[];		//!< �^�E���y�[�W��������
	extern const _TCHAR	kKeisaiName[];			//!< �^�E���y�[�W�f�ږ���
	extern const _TCHAR	kAddrCode[];			//!< 11���Z���R�[�h
	extern const _TCHAR	kAddrName[];			//!< �Z����
	extern const _TCHAR kPointStat[];			//!< �������ʃR�[�h
	extern const _TCHAR kSequence[];			//!< �V�[�P���X�ԍ�
	extern const _TCHAR kAddXY[];				//!< ���W�t�^���

	namespace point_stat {
		enum ECode {
			kNone					= 0,	//!< ������
			kUnknownPos				= 1,	//!< �ʒu�s��
			kInstitutionOK			= 2,	//!< �Y���{�݂���
			kReserved				= 3,	//!< �ۗ�
		};
	} // point_stat

	namespace add_xy {
		enum ECode {
			kNormal					= 0,	//!< �ʏ�
			kClaim					= 1,	//!< �N���[��
			kSameAddr				= 2,	//!< ����Z��
			kNormalRPoint			= 3,	//!< �i�ؗU���|�C���g(��ʓ��H)
			kHighwayRPoint			= 4,	//!< �i�ؗU���|�C���g(�������H)
			kTWNPinPoint			= 5,	//!< TWN�s���|�C���g�������(�����ڍs)
			kHLPPinPoint			= 6,	//!< HLP�s���|�C���g�������(�����ڍs)
		};
	} // add_xy

} // teltp_point

// �s�s��������{��
namespace building_roof {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kRfClass[];			//!< ����{�ݎ��

	namespace rf_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kOtherSite				=        1, //!< �ܑ��n���̑��~�n�i����{�݁j
			kPlanting				=		 2,	//!< �A�͒n�i����{�݁j
			kPool					=		 3, //!< �v�[���i����{�݁j
		};
	} // rf_class
} // building_roof

// �s�s�G���A
namespace city_area {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kAreaClass[];			//!< �G���A���

	namespace area_class {
		enum ECode {
			kNone					=        0, //!< �������E��ƒ�
			kPark					=        1, //!< ����
		};
	} // area_class
} // city_area

// �s�s�g���l���B��
namespace city_tunnel {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kTunnelClass[];			//!< �g���l�����
} // city_tunnel

// �s�s���H�|���S��
namespace city_road {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kRoadClass[];			//!< ���H�|���S�����
	
	namespace road_class {
		enum ECode {
			kNone						=        0, //!< �������E��ƒ�
			kInterCityTollExpressway	=		 1,	//!< �s�s�ԍ������i�L���j
			kInterCityFreeExpressway	=		 2,	//!< �s�s�ԍ������i�����j
			kUrbanTollExpressway		=		 3,	//!< �s�s�����i�L���j
			kUrbanFreeExpressway		=		 4,	//!< �s�s�����i�����j
			kNationalHighway			=        5, //!< ����
			kPrincipalPrefecturalRoad 	=        6, //!< ��v�n����
			kPrefectureNormalRoad		=		 7,	//!< ��ʓs���{����
			kOtherArterialRoad			=		 8,	//!< ���̑��������H
			kOtherRoad					=		 9,	//!< ���̑���
			kTollRoad					=		10,	//!< �L�����H
		};
	} // road_class
} // city_road

// �s�s�����������у|���S��
namespace city_divider {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];           //!< �e�[�u����
	extern const _TCHAR kDividerClass[];        //!< �����������ю��

} // city_divider

// ���ˉ��w�i���C��
namespace city_line_under {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kBgClass[];				//!< ���H�|���S�����

} // city_line_under

// ���ˉ��w�i�|���S��
namespace city_site_under {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];			//!< �e�[�u����
	extern const _TCHAR kBgClass[];				//!< ���H�|���S�����

} // city_site_under

// �n���ʘH�E�n���X�|���S��
namespace underground_area
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];   //!< �e�[�u����

} // underground_area

// �X�֔ԍ��|���S��
namespace zipcode_area
{
	using namespace ::sindy::schema::ipc_feature;

	extern const _TCHAR kTableName[]; //!< �e�[�u����
	extern const _TCHAR kZipCode1[]; //!< �X�֔ԍ�1
	extern const _TCHAR kZipCode2[]; //!< �X�֔ԍ�2
}

// �X�֔ԍ��|���S���Z���R�t���e�[�u��
namespace rel_addr
{
	extern const _TCHAR kTableName20[]; //!< 20���R�[�h�Ńe�[�u����
	extern const _TCHAR kTableName23[]; //!< 23���R�[�h�Ńe�[�u����
	extern const _TCHAR kZipCode[]; //!< �X�֔ԍ�
	extern const _TCHAR kAddrCode[]; //!< �Z���R�[�h
	extern const _TCHAR kModifyStatus[]; //!< �������

	namespace ModifyStatusCode {
		enum ECode {
			None = 0, //!< ���f�O
			Used11digit = 1, //!< �g�p���i11���ȍ~�j
			UnUsed = 2, //!< ���g�p
			Obsolete = 3, //!< �p�~
			Used5digit = 4, //!< �g�p���i5���j
		};
	}
}


} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_MAP_H_
