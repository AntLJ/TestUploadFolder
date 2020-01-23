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

#ifndef SINDY_SCHEMA_ROAD_H_
#define SINDY_SCHEMA_ROAD_H_

#include <iostream>
#include <sindy/schema/common.h>
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

extern const _TCHAR kObjectID[];		///< �I�u�W�F�N�gID�J������

/// ���H�����N�t�B�[�`���N���X
namespace road_link {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::time_regulation_relational_table;
	using namespace ::sindy::schema::category::parking_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::section_code_relational_table; ///< [bug 12036]
	using namespace ::sindy::schema::category::pref_road_relational_table;    ///< [bug 12036]

	extern const _TCHAR kTableName[];

	extern const _TCHAR kNaviClass[]; ///< ���H�o�H�p���
	extern const _TCHAR kMainLinkClass[]; ///< �����N��ʁi��j
	extern const _TCHAR kSubLinkClass[]; ///< �����N��ʁi���j
	extern const _TCHAR kAdminCode[]; ///< �s�����R�[�h
	extern const _TCHAR kRoadManager[]; ///< �Ǘ��҃R�[�h
	extern const _TCHAR kRoadWidth[]; ///< ���H�����敪
	extern const _TCHAR kBypass[]; ///< �o�C�p�X�t���O
	extern const _TCHAR kCarOnly[]; ///< �����Ԑ�p���H�t���O
	extern const _TCHAR kWideArea[]; ///< �L�摶�݃t���O
	extern const _TCHAR kIsland[]; ///< �����t���O
	extern const _TCHAR kUpDownClass[]; ///< �㉺����ʃR�[�h
	extern const _TCHAR kKubun[]; ///< ��]�敪�R�[�h
	extern const _TCHAR kTripSpeed[]; ///< ���s���x
	extern const _TCHAR kLimitSpeed[]; ///< �K�����x�R�[�h
	extern const _TCHAR kLaneCount[]; ///< �Ԑ���
	extern const _TCHAR kLaneValid[]; ///< �Б��Ԑ����L���t���O
	extern const _TCHAR kRightChange[]; ///< �Ԑ����E����
	extern const _TCHAR kLeftChange[]; ///< �Ԑ���������
	extern const _TCHAR kAlleyMainte[]; ///< ���׊X�H��ƔN�x���ID
	extern const _TCHAR kDupliLinkRF[]; ///< �d�p�H�����֌W�t���O
	extern const _TCHAR kTurnRegLQRF[]; ///< �E���܋֎~�����N��֌W�t���O
	extern const _TCHAR kDirGuideLQRF[]; ///< ���ʈē������N��֌W�t���O
	extern const _TCHAR kGuideLQRF[]; ///< �����U�������N��֌W�t���O
	extern const _TCHAR kLaneLQRF[]; ///< ���[����񃊃��N��֌W�t���O
	extern const _TCHAR kVICSLQRF[]; ///< VICS�����N��֌W�t���O
	extern const _TCHAR kBywayLQRF[]; ///< ��������񃊃��N��֌W�t���O
	extern const _TCHAR kRouteLQRF[]; ///< ���[�g��񃊃��N��֌W�t���O
	extern const _TCHAR kUturnLQRF[]; ///< U�^�[���\�ӏ���񃊃��N��֌W�t���O
	extern const _TCHAR kInternaviLQRF[]; ///< �C���^�[�i�rVICS�����N��֌W�t���O
	extern const _TCHAR kIpcVicsLQRF[]; ///< iPC-VICS�����N��֌W�t���O
	extern const _TCHAR kIntersectionLQRF[]; ///< �i�������ʌ����_�����N��֌W�t���O
	extern const _TCHAR kVehicleRegRF[];     //!< �Ԏ�ʋK���֘A�t���O
	extern const _TCHAR kVehicleTurnRegRF[]; //!< �Ԏ�ʉE���܋֎~�����N��֘A�t���O
	extern const _TCHAR kWalkable[]; //!< ���s�\��ʃR�[�h
	extern const _TCHAR kPedestrianZone[]; //!< ���s�ғV���t���O
	extern const _TCHAR kWheelChair[]; //!< �Ԉ֎q�ʍs�R�[�h

	extern const _TCHAR kParkingLinkClass[]; ///< ���ԏꃊ���N���
	extern const _TCHAR kFloorLevel[]; ///< ���ԏ�K�w���x��
	extern const _TCHAR kMiddleFloorLevel[]; ///< ���ԏꒆ�K�w���x��

	extern const _TCHAR kLegalSpeed[]; ///< �@�葬�x�R�[�h(�{���ɂ͖��K�p 2010.2.25)

	/// ���H�o�H�p��ʃR�[�h
	namespace navi_class {
		enum ECode {
			kNoNavi               =   0, ///< �o�H�O���H
			kCountry              =   1, ///< ��ʍ���
			kPrefectureMain       =   2, ///< ��v�n�����i�s�{���j
			kCityMain             =   3, ///< ��v�n�����i�w��s�j
			kPrefectureNormal     =   4, ///< ��ʓs���{����
			kCityNormal           =   5, ///< �w��s�̈�ʎs��
			kOther                =   6, ///< ���̑����H
			kHighway              = 101, ///< ���������ԓ��H
			kCityHighway          = 102, ///< �s�s�������H
			kTollCountry          = 103, ///< ��ʍ����i�L���j
			kTollPrefectureMain   = 104, ///< ��v�n�����i�s�{���j�i�L���j
			kTollCityMain         = 105, ///< ��v�n�����i�w��s�j�i�L���j
			kTollPrefectureNormal = 106, ///< ��ʓs���{�����i�L���j
			kTollCityNormal       = 107, ///< �w��s�̈�ʎs���i�L���j
			kTollOther            = 108, ///< ���̑����H�i�L���j
			kFerry                = 201, ///< �t�F���[�q�H
			kLinkCountry          = 403, ///< �ڑ�����
		};
	}

	/// ���H�����敪�R�[�h
	namespace road_width {
		enum ECode {
			kUnsurveyed      = 0, ///< ������
			kOver130         = 1, ///< ����13.0m�ȏ�
			kBetween55And130 = 2, ///< ����5.5m�ȏ�13.0m����
			kBetween30And55  = 3, ///< ����3.0m�ȏ�5.5m����
			kUnder30         = 4, ///< ����3.0m����
		};
	}

	/// �����N��ʃR�[�h
	namespace link_class {
		enum ECode {
			kUnsurveyed  = 0,   //!< �������E��ƒ�
			kMain        = 1,   //!< �{���i�㉺���񕪗��j�����N
			kMainUpDown  = 2,   //!< �{���i�㉺�����j�����N
			kConnect     = 3,   //!< �A���H�i�{���Ԃ̓n����j
			kCross       = 4,   //!< �����_�������N
			kRamp        = 5,   //!< �A���H�i�����v�j�����N
			kByway       = 6,   //!< �{���Ɠ���H���̑��������N
			kSA          = 7,   //!< �r�`�ȂǑ��������N
			kBicycle     = 8,   //!< ���]�ԓ���
			kBywayToMain = 9,   //!< �{�������ڑ������N
			kRoundAbout  = 101, //!< ���E���h�A�o�E�g
		};
	}

	/// �僊���N��ʃR�[�h
	namespace main_link_class {
		using namespace ::sindy::schema::road_link::link_class;
	}

	/// �������N��ʃR�[�h
	namespace sub_link_class {
		using namespace ::sindy::schema::road_link::link_class;
	}

	/// �Ǘ��҃R�[�h
	namespace road_manager {
		enum ECode {
			kUnsurveyed = 0,
			kJH         = 1, ///< ���{���H���c
			kPublic3    = 2, ///< ���H�R���c
			kPublic     = 3, ///< ���H����
			kCountry    = 4, ///< ��
			kPrefecture = 5, ///< �s���{��
			kCity       = 6, ///< �w��s
			kOtherTown  = 7, ///< ���̎s�������i�ܓ���23��j
			kOther      = 8, ///< ���̑��̊Ǘ���
		};
	}

	/// �㉺����ʃR�[�h�i�n�C�E�F�C�H���R�[�h�j
	namespace up_down_class {
		enum ECode {
			kNoHighway   = 0, ///< �ΏۊO
			kUndirected  = 1, ///< HWY(�㉺�Ȃ�)
			kUpInWest    = 2, ///< HWY���(���E��)
			kDownOutEast = 3, ///< HWY����(�O�E��)
			kCommon      = 4, ///< HWY�㉺������
			kEtcRamp	 = 5, ///< ETC��p�����v�����N
			kEtcAccess	 = 6, ///< ETC�ڑ���ʓ������N
		};
	}

	/// �ʍs�֎~��ʃR�[�h
	namespace no_passage {
		enum ECode {
			kPassable          = 0, ///< �ʍs�֎~�Ȃ�
			kNoPassage         = 1, ///< �����Ȃ��ʍs�֎~
			kNoPassageWithReg  = 2, ///< �����t���ʍs�֎~
			kInpassable        = 3, ///< �ʍs�s��
			kRestricted        = 4, ///< �����Ȃ��ʍs�֎~(�֌W�҈ȊO)
			kRestrictedWithReg = 5, ///< �����t���ʍs�֎~(�֌W�҈ȊO)
		};
	}

	/// ����ʍs��ʃR�[�h
	namespace oneway {
		enum ECode {
			kNoOneway           = 0, ///< ����ʍs�Ȃ�
			kFore               = 1, ///< �����Ȃ�����ʍs(������)
			kReverse            = 2, ///< �����Ȃ�����ʍs(�t����)
			kForeWithReg        = 3, ///< �����t������ʍs(������)
			kReverseWithReg     = 4, ///< �����t������ʍs(�t����)
			kForeReverseWithReg = 5, ///< �����t������ʍs(���t)
		};
	}

	/// ��]���H�敪�R�[�h
	namespace kubun {
		enum ECode {
			kUnsurveyed = 0, ///< ������
			kMain       = 1, ///< �哹�H
			kSub        = 2, ///< �]���H
		};
	}

	/// ���ԏꃊ���N��ʃR�[�h
	namespace parking_link_class {
		enum ECode {
			kUnsurveyed   = 0, ///< ������
			kEntrance     = 1, ///< ���������N
			kExit         = 2, ///< �o�������N
			kExitEntrance = 3, ///< �o���������N
			kParkingRoad  = 4, ///< �\�����H�����N
			kLevelRamp    = 5, ///< ���x���Ԃ̓n�胊���N
		};
	}

	/// DRMA�K�����x�R�[�h
	namespace limit_speed {
		enum ECode {
			kUnInvestigated = 0, ///< ������
			kLess30         = 1, ///< �R�Okm/h����
			k40             = 2, ///< �S�Okm/h
			k50             = 3, ///< �T�Okm/h
			k60             = 4, ///< �U�Okm/h
			k70             = 5, ///< �V�Okm/h
			k80             = 6, ///< �W�Okm/h
			k100            = 7, ///< �P�O�Okm/h
		};
	}

	/// �׊X�H��ƔN�x�R�[�h
	namespace alley_mainte {
		enum ECode {
			kNoData     =  0, ///< �Y���Ȃ�
			k1998       =  1, ///< 1998�N�x
			k1999       =  2, ///< 1999�N�x
			k2000Spring =  3, ///< 2000�N�x�i�t�j
			k2000Autumn =  4, ///< 2000�N�x�i�H�j
			k2001       =  5, ///< 2001�N�x
			k2002       =  6, ///< 2002�N�x
			k2003       =  7, ///< 2003�N�x
			k2004       =  8, ///< 2004�N�x
			k2005       =  9, ///< 2005�N�x
			k2006       = 10, ///< 2006�N�x
		};
	}

	namespace updown_class {
		enum ECode
		{
			kNonHwy		= 0,	///< �ΏۊO
			kNoDir		= 1,	///< HWY�i�㉺�Ȃ��j
			kUp			= 2,	///< HWY���i���E���j
			kDown		= 3,	///< HWY����i�O�E���j
			kUpDown		= 4,	///< HWY�㉺������
			kEtcRamp	= 5,	///< ETC��p�����v�����N
			kEtcAccess	= 6,	///< ETC�ڑ���ʓ������N
		};
	}

	namespace walkable_class {
		enum ECode
		{
			kNotWalk  = 0,  ///< �����Ȃ��i���s�֎~�j
			kWalkable = 1,  ///< �����Ȃ��i�l�ԋ��p�j
			kWalkOnly = 2,  ///< ���s�Ґ�p
		};
	} // walkable_class

	/**
	 * @brief �ō����x�R�[�h
	 * @note	[Bug9294]�Ή�����[Bug9293]�Łi[Bug8924]���܂ށj�̃X�L�[�}�ɍ��킹�ĕύX�B�C���O��[Bug7486]�̎b��X�L�[�}�̒ʂ�ɂȂ��Ă����B
	 *			�C���O�̂�����g�����R�[�h���r���h���悤�Ƃ������Ƀr���h�ł��Ă��܂��ƃo�O�̌��ɂȂ肻���Ȃ̂ŁA
	 *			�r���h�����s���Ă����̕ω��ɋC�t����悤�ɁA�e�R�[�h�l�ɑΉ�����񋓌^�v�f����S�ĕς��Ă����i��F�uk40�v���uk40km�v�j�B
	 */
	namespace legal_speed {
		enum ECode
		{
			kUnInvestigated = 0,	///< ������
			k30kmOrLess		= 1,	///< 30km/h�ȉ�
			k20km			= 2,	///< 20km/h
			k30km			= 3,	///< 30km/h
			k40km			= 4,	///< 40km/h
			k50km			= 5,	///< 50km/h
			k60km			= 6,	///< 60km/h
			k70km			= 7,	///< 70km/h
			k80km			= 8,	///< 80km/h
			k90km			= 9,	///< 90km/h
			k100km			= 10,	///< 100km/h
			k110km			= 11,	///< 110km/h [bug12460]
			k120km			= 12,	///< 120km/h [bug12460]
			kZone30km		= 30,	///< �]�[��30
			kNone			= 99,	///< �Ȃ�
		};
	}
}

/// ���H�m�[�h�e�[�u��
namespace road_node {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kNodeClass[];///< �m�[�h���
	extern const _TCHAR kSignal[]; ///< �M���@�R�[�h
	extern const _TCHAR kNameKanji[]; ///< �����_��������
	extern const _TCHAR kNameYomi[]; ///< �����_�J�i����
	extern const _TCHAR kHeightCode[]; ///< �����R�[�h 
	extern const _TCHAR kHeight[]; ///< �������
	extern const _TCHAR kParkingID[]; ///< ���ԏ�h�c

	/// �m�[�h��ʃR�[�h
	namespace node_class {
		enum ECode {
			kNoFunction =  0, ///< �@�\�Ȃ��m�[�h
			kCross      =  1, ///< �����_�m�[�h
			kTerminal   =  2, ///< �[�_�m�[�h
			kAlter      =  3, ///< �����ω��_�m�[�h
			kAttr       =  4, ///< �����t���m�[�h
			kParking    =  5, ///< ���ԏꃊ���N�ڑ��m�[�h
			kLQBase     =  6, ///< �m�[�h���_�m�[�h
			kTollbooth  =  7, ///< �������m�[�h
			kHighway    =  8, ///< �n�C�E�F�C�֌W�m�[�h
			kEdge       =  9, ///< �Q�����b�V���̋�؂��̃m�[�h
			kEvasion    = 10, ///< ���ꃊ���NID�h�~�m�[�h
			kPec        = 11, ///< PEC�m�[�h
			kPecEdge    = 12, ///< �Q�����b�V�����ӏ�PEC�m�[�h
			kDummyFacil = 13, ///< �_�~�[�{�݃m�[�h
		};
	}

	/// �M���@�R�[�h
	namespace signal {
		enum ECode {
			kSignal     = 1, ///< �M���@����
			kNoSignal   = 0, ///< �M���@�Ȃ�
			kUnsurveyed = 2, ///< ������
		};
	}

	/// �����R�[�h
	namespace height_code {
		enum ECode {
			kInvalid = 0, ///< ���΍�������
			kValid   = 1, ///< ���΍����L��
			kAuto    = 2, ///< ���������v�Z
		};
	}
}

/// ���������N�e�[�u��
namespace walk_link {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::from_to_node_relational_table;
	using namespace ::sindy::schema::category::time_regulation_relational_table;

	extern const _TCHAR kTableName[];			///< �e�[�u����

	extern const _TCHAR kWalkClass[];			///< ���s�Ҏ��
	extern const _TCHAR kFloorLevel[];			///< �K�w���x��
	extern const _TCHAR kFloorMoveClass[];		///< �K�w�ړ����
	extern const _TCHAR kAutoWalkFlag[];		///< �I�[�g�E�H�[�N�t���O
	extern const _TCHAR kStairFlag[];			///< �K�i�t���O
	extern const _TCHAR kSlopeFlag[];			///< �X���[�v�t���O
	extern const _TCHAR kEscalatorFlag[];		///< �G�X�J���[�^�t���O
	extern const _TCHAR kArcadeFlag[];			///< �����t���t���O
	extern const _TCHAR kOpenAreaFlag[];		///< �L��iOpen Traffic Area�j�t���O�ibug 11187�j
	extern const _TCHAR kTunnelFlag[];			///< �g���l���t���O
	extern const _TCHAR kRoadSurface[];			///< �H�ʑf�ރR�[�h
	extern const _TCHAR kLinkRelationRF[];		///< �����N�R�t�e�[�u���֌W�t���O
	extern const _TCHAR kWalkCode[];			///< ���s�ҘH���R�[�h
	extern const _TCHAR kWheelChair[];			///< �Ԉ֎q�ʍs�R�[�h
	extern const _TCHAR kPlatformNumber[];		///< �̂�Δԍ�
	extern const _TCHAR kScrambleFlag[];		///< �X�N�����u���t���O

	/// ���s�ҕ\����ʃR�[�h
	namespace walk_class {
		enum ECode {
			kNone                 = 0,    ///< �ݒ�Ȃ�
			kWalkRoad             = 1,    ///< ����
			kInferredCrossRoad    = 2,    ///< �݂Ȃ����f����
			kCrossRoad            = 3,    ///< ���f����
			kOtherWalkRoad        = 99,   ///< ���̑�����
			kWalkableRoad         = 100,  ///< �l�ԋ��p�����N
			kConnect              = 101,  ///< �ڑ������N
			kWalkBridge           = 200,  ///< ������
			kBridge               = 201,  ///< ��
			kRailCrossing         = 202,  ///< ����
			kOutdoor              = 300,  ///< �{�ݕ~�n���i���O�j
			kIndoor               = 301,  ///< �{�ݕ~�n���i�����j
			kInternalTicketGate   = 302,  ///< �n����D��
			kPlatForm             = 303,  ///< �n��v���b�g�t�H�[��
			kUnderGround          = 400,  ///< �n���ʘH
			kUnderGroundPath      = 401,  ///< ���f�n�����ibug 11187�j
			kInternalTicketGateUG = 402,  ///< �n�����D��
			kPlatFormUG           = 403,  ///< �n���v���b�g�t�H�[��
			kWalkFerry            = 500,  ///< ���s�Ґ�p�t�F���[
			
		};
	} // walk_class

	/// �K�w�ړ���ʃR�[�h
	namespace floor_move_class {
		enum ECode {
			kNone      = 0,  ///< �㉺����
			kForeUp    = 1,  ///< ���(������)
			kReverseUp = 2,  ///< ���(�t����)
		};
	} // floor_move_class

	/// ����ʍs��ʃR�[�h
	namespace oneway {
		using namespace ::sindy::schema::road_link::oneway;
	} // oneway

	/// �ʍs�֎~��ʃR�[�h
	namespace nopassage {
		enum ECode {
			kPassable          = 0, ///< �ʍs�֎~�Ȃ�
			kNoPassageWithReg  = 2, ///< �����t���ʍs�֎~
		};
	} // nopassage

	/// �H�ʑf�ރR�[�h
	namespace road_surface_class {
		enum ECode {
			kPaved   = 1,  ///< �ܑ�
			kUnpaved = 99, ///< ���ܑ�
		};
	} // road_surface_class

	/// �Ԉ֎q�ʍs�R�[�h
	namespace wheel_chair {
		enum ECode {
			kUnInvestigated       = 0,    ///< ������
			kPassable             = 1,    ///< �ʍs�\
			kNoPassable           = 2,    ///< �ʍs�s�\
		};
	} // walk_class
} // walk_link

/// �����m�[�h�e�[�u��
namespace walk_node {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];              ///< �e�[�u����

	extern const _TCHAR kNodeClass[];              ///< ���s�҃m�[�h���
	extern const _TCHAR kRoadNodeID[];             ///< ���H�m�[�h�Ή�ID
	extern const _TCHAR kStationGatePointID[];     ///< �w�o�����|�C���gID
	extern const _TCHAR kUndergroundGatePointID[]; ///< �n���X�o�����|�C���gID

	/// ���s�҃m�[�h���
	namespace node_class {
		enum ECode {
			kPublic           = 0,  ///< ��ʃm�[�h
			kTerminal         = 1,  ///< �[�_�m�[�h
			kAlter            = 2,  ///< �����J�ړ_�m�[�h
			kRoadNW           = 3,  ///< ���H�m�[�h�ڑ��m�[�h
			kGate             = 4,  ///< �w�o�����m�[�h
			kUndergroundGate  = 5,  ///< �n���X�o�����m�[�h
			kElevator         = 6,  ///< �G���x�[�^�m�[�h
			kKaisatsu         = 7,  ///< ���D�m�[�h
			kCarPratform      = 8,  ///< ��ʎԏ�~��m�[�h
			kBusPratform      = 9,  ///< �o�X��~��m�[�h
			kTaxiPratform     = 10, ///< �^�N�V�[��~��m�[�h
			kParking          = 11, ///< ���ԏ�o�����m�[�h
			kDepartureGate    = 12, ///< ��`�o���Q�[�g�m�[�h
			kArrivalGate      = 13, ///< ��`�����Q�[�g�m�[�h
			kTicketGate       = 14, ///< �w���D�m�[�h
			kOutline          = 99, ///< �}�s��m�[�h
		};
	}
} // walk_node

/// �j���w��R�[�h�h���C���iwalk_nopassage, walk_oneway�ł��g�p����̂ŁAinf_turnreg���番���j
namespace day_code {
	enum ECode {
		kHoliday                =  1, ///< ���E�x��
		kExceptHolidayAndSunday =  2, ///< ���E�x�����̂���
		kHolidayExceptSunday    =  3, ///< �x��(���j�͂̂���)
		kExceptHoliday          =  4, ///< �x�����̂���
		kOrdinaryDays           =  5, ///< ����
		kSunday                 =  6, ///< ���j
		kMonday                 =  7, ///< ��
		kTuesday                =  8, ///< ��
		kWednesday              =  9, ///< ��
		kThursday               = 10, ///< ��
		kFriday                 = 11, ///< ��
		kSaturday               = 12, ///< �y
		kWeekday                = 13, ///< �y���E�x��
		kExceptWeekday          = 14, ///< �y���E�x�����̂���
		kAlldays                = 15, ///< �S�j��
		kExceptSunday           = 16, ///< ���j�����̂���
		kBeforeHoliday          = 17, ///< ���E�x���̑O��
	};
}


/// �����t���ʍs�֎~�e�[�u��
namespace time_nopassage {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kSeasonReg[]; ///< �G�ߋK���t���O

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �����t������ʍs�e�[�u���t�B�[���h
namespace time_oneway {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/**
 * @defgroup sindy_schema_road_link_segment_sharing_feature_classes
 * @brief ���H�����N�������L�t�B�[�`���N���X
 */
//@{
/// �����N�������t�B�[�`���N���X
namespace segment_attr {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kUnderOver[]; ///< �㉺�֌W
	extern const _TCHAR kLinkAttr[]; ///< �����N��������ʃR�[�h
	extern const _TCHAR kName[]; ///< ����

	/// �����N�������R�[�h
	namespace link_attr {
		enum ECode {
			kBridge            = 1, ///< ���E����
			kTunnel            = 2, ///< �g���l��
			kDoumon            = 3, ///< ����
			kRailCross         = 4, ///< ����
			kUnderPass         = 5, ///< �A���_�[�p�X
			kUnderOver         = 6, ///< �㉺�֌W
			kDirtRoad          = 7, ///< ���ܑ����H
			kFlooding          = 8, ///< �������Ӓn�_
			kTunnelSquare      = 100, ///< �g���l���i�l�p�j
			kTunnelCircle      = 101, ///< �g���l���i�ہj
			kRailCrossKamaboko = 102, ///< ���؁i���܂ڂ��^�j
		};
	}
}

/// �X�΃t�B�[�`���N���X
namespace grad {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kGradValue[]; ///< �X�Βl

}

/// �ȈՌX�΃t�B�[�`���N���X
namespace simple_grad {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::link_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kNodeID[]; ///< ����m�[�hID
	extern const _TCHAR kGrad[]; ///< �ȈՌX�΃R�[�h
	extern const _TCHAR kLinkType[]; ///< �����N�^�C�v�R�[�h

	/// �ȈՌX�΃R�[�h
	namespace grad {
		enum ECode {
			kUnknown   = 0, ///< UNKNOWN
			kFlat      = 1, ///< FLAT
			kUp        = 2, ///< UP
			kDown      = 3, ///< Down
		};
	}
	/// �����N�^�C�v�R�[�h
	namespace link_type {
		enum ECode {
			kUnknown   = 0, ///< ����`
			kFromLink  = 1, ///< From�����N
			kToLink    = 2, ///< To�����N
		};
	}
}

//@}

/// ���ԏ�O�`�t�B�[�`���N���X
namespace parking_shape {
	using namespace ::sindy::schema::category::parking_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kParkingOrgID[]; ///< ���ԏ�ID
	extern const _TCHAR kNameKanji[]; ///< ���ԏꖼ��
}

/// ���s�҃i�r�G���A�|���S��
namespace edit_area {
	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kAreaName[];	///< �����G���A
	extern const _TCHAR kCreateYear[];	///< �쐬�N�x
	extern const _TCHAR kPriority[];	///< �D��x
}

/// ���s�җp�����t���ʍs�֎~�e�[�u��
namespace walk_nopassage {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // walk_nopassage

/// ���s�җp�����t������ʍs�e�[�u���t�B�[���h
namespace walk_oneway {
	using namespace ::sindy::schema::meshcode_class;
	using namespace ::sindy::schema::category::directional_link_relational_table;
	using namespace ::sindy::schema::category::time_condition_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
} // walk_oneway

/// ���s�҃^�C�v�e�[�u��
namespace walk_type{
	using namespace ::sindy::schema::object;

	extern const _TCHAR kTableName[];  ///< �e�[�u����

	extern const _TCHAR kWalkTypeClass[];  ///< ���s�Ҏ�ʃR�[�h

	namespace type_class {
		enum ECode {
			kGeneral     = 1, ///< ��������i�ėp�Łj
			kDownConvert = 2, ///< �����Ȃ�
		};
	} // type_class
} // walk_type

/**
 * @defgroup sindy_schema_inf
 * @brief �����N��Inf�e�[�u���O���[�v
 */
//@{
/// �E���܋֎~�����N��Inf�e�[�u��
namespace inf_turnreg {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
	extern const int    kMaxNumber; ///< �ő�E���܋֎~��

	extern const _TCHAR kStartMonth0[]; ///< �K������:�J�n��
	extern const _TCHAR kStartDay0[]; ///< �K������:�J�n��
	extern const _TCHAR kStartHour0[]; ///< �K������:�J�n����
	extern const _TCHAR kStartMin0[]; ///< �K������:�J�n��
	extern const _TCHAR kEndMonth0[]; ///< �K������;�I����
	extern const _TCHAR kEndDay0[]; ///< �K������:�I����
	extern const _TCHAR kEndHour0[]; ///< �K������:�I������
	extern const _TCHAR kEndMin0[]; ///< �K������:�I����
	extern const _TCHAR kDayOfWeek0[]; ///< �K������:�j���w��

	extern const _TCHAR kStartMonth1[]; ///< �K������1:�J�n��
	extern const _TCHAR kStartDay1[]; ///< �K������1:�J�n��
	extern const _TCHAR kStartHour1[]; ///< �K������1:�J�n����
	extern const _TCHAR kStartMin1[]; ///< �K������1:�J�n��
	extern const _TCHAR kEndMonth1[]; ///< �K������1;�I����
	extern const _TCHAR kEndDay1[]; ///< �K������1:�I����
	extern const _TCHAR kEndHour1[]; ///< �K������1:�I������
	extern const _TCHAR kEndMin1[]; ///< �K������1:�I����
	extern const _TCHAR kDayOfWeek1[]; ///< �K������1:�j���w��

	extern const _TCHAR kStartMonth2[]; ///< �K������2:�J�n��
	extern const _TCHAR kStartDay2[]; ///< �K������2:�J�n��
	extern const _TCHAR kStartHour2[]; ///< �K������2:�J�n����
	extern const _TCHAR kStartMin2[]; ///< �K������2:�J�n��
	extern const _TCHAR kEndMonth2[]; ///< �K������2;�I����
	extern const _TCHAR kEndDay2[]; ///< �K������2:�I����
	extern const _TCHAR kEndHour2[]; ///< �K������2:�I������
	extern const _TCHAR kEndMin2[]; ///< �K������2:�I����
	extern const _TCHAR kDayOfWeek2[]; ///< �K������2:�j���w��

	extern const _TCHAR kStartMonth3[]; ///< �K������3:�J�n��
	extern const _TCHAR kStartDay3[]; ///< �K������3:�J�n��
	extern const _TCHAR kStartHour3[]; ///< �K������3:�J�n����
	extern const _TCHAR kStartMin3[]; ///< �K������3:�J�n��
	extern const _TCHAR kEndMonth3[]; ///< �K������3;�I����
	extern const _TCHAR kEndDay3[]; ///< �K������3:�I����
	extern const _TCHAR kEndHour3[]; ///< �K������3:�I������
	extern const _TCHAR kEndMin3[]; ///< �K������3:�I����
	extern const _TCHAR kDayOfWeek3[]; ///< �K������3:�j���w��

	extern const _TCHAR kStartMonth4[]; ///< �K������4:�J�n��
	extern const _TCHAR kStartDay4[]; ///< �K������4:�J�n��
	extern const _TCHAR kStartHour4[]; ///< �K������4:�J�n����
	extern const _TCHAR kStartMin4[]; ///< �K������4:�J�n��
	extern const _TCHAR kEndMonth4[]; ///< �K������4;�I����
	extern const _TCHAR kEndDay4[]; ///< �K������4:�I����
	extern const _TCHAR kEndHour4[]; ///< �K������4:�I������
	extern const _TCHAR kEndMin4[]; ///< �K������4:�I����
	extern const _TCHAR kDayOfWeek4[]; ///< �K������4:�j���w��

	extern const LPCTSTR kStartMonth[5]; ///< �K������:�J�n���ꗗ
	extern const LPCTSTR kStartDay[5]; ///< �K������:�J�n���ꗗ
	extern const LPCTSTR kStartHour[5]; ///< �K������:�J�n���Ԉꗗ
	extern const LPCTSTR kStartMin[5]; ///< �K������:�J�n���ꗗ
	extern const LPCTSTR kEndMonth[5]; ///< �K������;�I�����ꗗ
	extern const LPCTSTR kEndDay[5]; ///< �K������:�I�����ꗗ
	extern const LPCTSTR kEndHour[5]; ///< �K������:�I�����Ԉꗗ
	extern const LPCTSTR kEndMin[5]; ///< �K������:�I�����ꗗ
	extern const LPCTSTR kDayOfWeek[5]; ///< �K������:�j���w��ꗗ

	using namespace ::sindy::schema::day_code;

	struct where_all_year_all_days {};

	template <typename CharT>
	inline std::basic_ostream<wchar_t>& operator<<(std::basic_ostream<CharT>& os, where_all_year_all_days)
	{
		using namespace atl2::io::compatible;
		return os << kStartMonth1 << L" IS NULL";
	}
}

/// �����U�������N��Inf�e�[�u��
namespace inf_guide {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
	
	extern const _TCHAR kGuideType[]; ///< �����U���ē��R�[�h
	extern const _TCHAR kDelete[]; ///< �����U���폜�t���O

	/// �����U���ē��R�[�h
	namespace guide_type {
		enum ECode {
			kUnsurveyed =  0, ///< ���ݒ�
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
	}
}

/// ���ʈē������N��Inf�e�[�u��
namespace inf_dirguide {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const int kMaxNumber; ///< �ő���ʈē���

	extern const _TCHAR kGuidePattern[]; ///< ���ʊŔp�^�[���ԍ�
	extern const _TCHAR kSequence[]; ///< ����V�[�P���X�ԍ�

	extern const _TCHAR kGuideClass0[]; ///< ���ʈē�:�n����
	extern const _TCHAR kNameKanji0[]; ///< ���ʈē�:��������
	extern const _TCHAR kNameYomi0[]; ///< ���ʈē�:�J�i����

	extern const _TCHAR kGuideClass1[]; ///< ���ʈē�1:�n����
	extern const _TCHAR kNameKanji1[]; ///< ���ʈē�1:��������
	extern const _TCHAR kNameYomi1[]; ///< ���ʈē�1:�J�i����

	extern const _TCHAR kGuideClass2[]; ///< ���ʈē�2:�n����
	extern const _TCHAR kNameKanji2[]; ///< ���ʈē�2:��������
	extern const _TCHAR kNameYomi2[]; ///< ���ʈē�2:�J�i����

	extern const _TCHAR kGuideClass3[]; ///< ���ʈē�3:�n����
	extern const _TCHAR kNameKanji3[]; ///< ���ʈē�3:��������
	extern const _TCHAR kNameYomi3[]; ///< ���ʈē�3:�J�i����

	extern const _TCHAR kGuideClass4[]; ///< ���ʈē�4:�n����
	extern const _TCHAR kNameKanji4[]; ///< ���ʈē�4:��������
	extern const _TCHAR kNameYomi4[]; ///< ���ʈē�4:�J�i����

	extern const _TCHAR kGuideClass5[]; ///< ���ʈē�5:�n����
	extern const _TCHAR kNameKanji5[]; ///< ���ʈē�5:��������
	extern const _TCHAR kNameYomi5[]; ///< ���ʈē�5:�J�i����

	extern const _TCHAR kGuideClass6[]; ///< ���ʈē�6:�n����
	extern const _TCHAR kNameKanji6[]; ///< ���ʈē�6:��������
	extern const _TCHAR kNameYomi6[]; ///< ���ʈē�6:�J�i����

	extern const _TCHAR kGuideClass7[]; ///< ���ʈē�7:�n����
	extern const _TCHAR kNameKanji7[]; ///< ���ʈē�7:��������
	extern const _TCHAR kNameYomi7[]; ///< ���ʈē�7:�J�i����

	extern const _TCHAR kGuideClass8[]; ///< ���ʈē�8:�n����
	extern const _TCHAR kNameKanji8[]; ///< ���ʈē�8:��������
	extern const _TCHAR kNameYomi8[]; ///< ���ʈē�8:�J�i����

	extern const _TCHAR kGuideClass9[]; ///< ���ʈē�9:�n����
	extern const _TCHAR kNameKanji9[]; ///< ���ʈē�9:��������
	extern const _TCHAR kNameYomi9[]; ///< ���ʈē�9:�J�i����

	extern const _TCHAR kGuideClass10[]; ///< ���ʈē�10:�n����
	extern const _TCHAR kNameKanji10[]; ///< ���ʈē�10:��������
	extern const _TCHAR kNameYomi10[]; ///< ���ʈē�10:�J�i����

	extern const LPCTSTR kGuideClass[11]; ///< ���ʈē�:�n���ʈꗗ
	extern const LPCTSTR kNameKanji[11]; ///< ���ʈē�:�������̈ꗗ
	extern const LPCTSTR kNameYomi[11]; ///< ���ʈē�:�J�i���̈ꗗ

	/// ���ʊŔp�^�[���ԍ�
	namespace guide_pattern {
		enum ECode {
			kNoPattern  =  0, ///< �p�^�[������
			kOverpass   =  1, ///< ���̂`�i�I�[�o�[�p�X�j
			kUnderpass  =  2, ///< ���̂a�i�A���_�[�p�X�j
			kCrossroads =  3, ///< �\���H
			kTJunctionL =  4, ///< �s���`�i�E�܂Ȃ��j
			kTJunctionR =  5, ///< �s���a�i���܂Ȃ��j
			kYJunction  =  6, ///< �x��
			kTJunction  =  7, ///< �s��
			kInitial    = 99, ///< ���ݒ�
		};
	};

	/// ���ʈē���ʃR�[�h2
	namespace guide_class {
		enum ECode {
			kNormal            =  1, ///< �n��i��ʁj
			kToll              =  2, ///< �n��i�L���j
			kCountry           =  3, ///< ����
			kPrefecture        =  4, ///< ����
			kOther             =  5, ///< ���̑�
			kTollString        = 12, ///< �L���i������j
			kTollNationalWayNo = 13, ///< �L���i�����ԍ��j
			kTollPrefWayNo     = 14, ///< �L���i�����ԍ��j
		};
	}
}

/// ���[�������N��Inf�e�[�u��
namespace inf_lane {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kLaneDir[]; ///< ���[�������R�[�h
	extern const _TCHAR kApproachLane[]; ///< �����_���ߎԐ���
	extern const _TCHAR kRightChange[]; ///< �E�������Ԑ���
	extern const _TCHAR kLeftChange[]; ///< ���������Ԑ���
	extern const _TCHAR kFlowLane[]; ///< ���o�Ԑ���
	extern const _TCHAR kLanePassable[]; ///< �Ԑ��ʍs���

	/// ���[�������R�[�h
	namespace lane_dir {
		enum ECode {
			kUnsurveyed = 0, ///< �������E��ƒ�
			k0   = 1, ///< �O����(0)
			k45  = 2, ///< �E�΂ߑO����(45)
			k90  = 3, ///< �E����(90)
			k135 = 4, ///< �E�΂ߌ�����(135)
			k180 = 5, ///< ������(180)
			k225 = 6, ///< ���΂ߌ�����(225)
			k270 = 7, ///< ������(270)
			k315 = 8, ///< ���΂ߑO����(315)
		};
	}
}

/// VICS�����N��Inf�e�[�u��
namespace inf_vics {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �C���^�[�i�rVICS�����N��Inf�e�[�u��
namespace inf_internavi {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// iPC-VICS�����N��Inf�e�[�u��
namespace inf_ipcvics {
	using namespace ::sindy::schema::fieldsurvey_table;
	using namespace ::sindy::schema::category::vics_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// iPC-VICS��ID�Ǘ��e�[�u��(�ő�l�ێ�)
namespace ipcvics_idtable {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
	extern const _TCHAR kMeshCode[];	///< ���b�V���R�[�h
	extern const _TCHAR kMainNumber[];	///< �D��g�p�ς�ID�ԍ�
	extern const _TCHAR kSubNumber[];	///< ��D��g�p�ς�ID�ԍ�
	extern const _TCHAR kSubVicsClass[];	///< ��D��VICS�敪
}

/// �i�������ʌ����_�����N��Inf�e�[�u��
namespace inf_intersection {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kNameKanji[]; ///< �����_����
	extern const _TCHAR kNameYomi[]; ///< ���~
}

/// �����������N��Inf�e�[�u��
namespace inf_byway {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kByway[]; ///< �������t���O
	extern const _TCHAR kBackRoad[]; ///< �����t���O
	extern const _TCHAR kSourceName[]; ///< �o�T
}


/// ���[�g�����N��Inf�e�[�u��
namespace inf_route {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kRouteClass[]; ///< ���[�g��ʃR�[�h
	extern const _TCHAR kSourceName[]; ///< �o�T
	extern const _TCHAR kRankMeshCode[]; ///< ����₷�����b�V��
	extern const _TCHAR kRankLinkID[]; ///< ����₷�������N�ԍ�
	extern const _TCHAR kDriveMapRank[]; ///< ����₷���]��

	/// ���[�g��ʃR�[�h
	namespace route_class {
		enum ECode {
			kAccessRoad       = 1,  //!< �A�N�Z�X���H	
			kConnectRoad      = 2,  //!< ���p����
			kInfomation       = 3,  //!< �ό��ē�
			kHighAccidentArea = 4,  //!< ���̑����n�_
		};
	}

	/// �]�������N��ʃR�[�h
	namespace rank_class {
		enum ECode {
			kSRank = 1,
			kARank = 2,
			kBRank = 3,
			kCRank = 4,
			kDRank = 5,
			kMRank = 11,
			kYRAnk = 12,
			kRRAnk = 13,
		};
	}
}

/// �t�^�[���\�ӏ������N��Inf�e�[�u��
namespace inf_uturn {
	using namespace ::sindy::schema::fieldsurvey_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kSourceName[]; ///< �o�T
}
//@}

/**
 * @defgroup sindy_schema_link_queue
 * @brief �����N��LQ�e�[�u���O���[�v
 */
//@{
/// �E���܋֎~�����N��LQ�e�[�u��
namespace lq_turnreg {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �����U�������N��LQ�e�[�u��
namespace lq_guide {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// ���ʈē������N��LQ�e�[�u��
namespace lq_dirguide {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// ���[�������N��LQ�e�[�u��
namespace lq_lane {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// VICS�����N��LQ�e�[�u��
namespace lq_vics {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �C���^�[�i�rVICS�����N��LQ�e�[�u��
namespace lq_internavi {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// iPC-VICS�����N��LQ�e�[�u��
namespace lq_ipcvics {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �i�������ʌ����_�����N��LQ�e�[�u��
namespace lq_intersection {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// �����������N��LQ�e�[�u��
namespace lq_byway {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}

/// ���[�g�����N��LQ�e�[�u��
namespace lq_route {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}
/// �t�^�[���\�ӏ������N��LQ�e�[�u��
namespace lq_uturn {
	using namespace ::sindy::schema::link_queue;

	extern const _TCHAR kTableName[]; ///< �e�[�u����
}
//@}

/// �{�ݏ��|�C���g�t�B�[�`���N���X
namespace facil_info_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[];		///< �e�[�u���i���C���j��

	extern const _TCHAR kNameKanji[];		///< �{�݊�������
	extern const _TCHAR kNameYomi[];		///< �{�݃J�i����
	extern const _TCHAR kDummyName[];		///< ���̃t���O
	extern const _TCHAR kFacilClass[];		///< �{�ݎ�ʃR�[�h
	extern const _TCHAR kAutoPosition[];	///< ��\���W�����v�Z�t���O
	extern const _TCHAR kSearchClass[];		///< �����p�{�ݎ�ʃR�[�h
	extern const _TCHAR kDummyFacil[];		///< �_�~�[�{�݃t���O�i[Bug9140]�Œǉ��j

	/// �{�ݎ�ʃR�[�h��`
	namespace facil_class {
		enum ECode {
			kSA			= 1, ///< �r�`
			kPA			= 2, ///< �o�`
			kJCT		= 3, ///< �i�b�s
			kRamp		= 4, ///< �����v
			kIC			= 5, ///< �h�b
			kTollbooth	= 6, ///< ������
			kSmartIC	= 7, ///< �X�}�[�g�h�b
		};
	}

	/// �����p�{�ݎ�ʃR�[�h��`
	namespace search_class {
		enum ECode {
			kNotForSearch	= 0, ///< �ΏۊO
			kSA				= 1, ///< �r�`
			kPA				= 2, ///< �o�`
			kJCT			= 3, ///< �i�b�s
			kRamp			= 4, ///< �����v
			kIC				= 5, ///< �h�b
			kTollbooth		= 6, ///< ������
			kSmartIC		= 7, ///< �X�}�[�g�h�b
		};
	}
}

/// �A�N�Z�X�|�C���g�t�B�[�`���N���X
namespace access_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u���i���C���j��

	extern const _TCHAR kHwyMode[]; ///< �n�C�E�F�C���[�h�Ή��t���O
	extern const _TCHAR kDirectionKanji[]; ///< ���ʊ�������
	extern const _TCHAR kDirectionYomi[]; ///< ���ʃJ�i����
	extern const _TCHAR kGS[]; ///< �K�\�����X�^���h���ʃR�[�h
	extern const _TCHAR kToilet[]; ///< �g�C���t���O
	extern const _TCHAR kHandicap[]; ///< �g��҃g�C���t���O
	extern const _TCHAR kRestaurant[]; ///< ���X�g�����t���O
	extern const _TCHAR kCafeteria[]; ///< �y�H�t���O
	extern const _TCHAR kSouvenir[]; ///< ���X�t���O
	extern const _TCHAR kResting[]; ///< �x�e���t���O
	extern const _TCHAR kNap[]; ///< �����x�e�{�݃t���O
	extern const _TCHAR kInfoDesk[]; ///< �ē���(�ΐl)�t���O
	extern const _TCHAR kInformation[]; ///< �C���t�H���[�V�����t���O
	extern const _TCHAR kShower[]; ///< �V�����[���[���t���O
	extern const _TCHAR kLaundry[]; ///< �R�C�������h���[�t���O
	extern const _TCHAR kSpa[]; ///< �����C�t���O
	extern const _TCHAR kFax[]; ///< FAX�t���O
	extern const _TCHAR kMailbox[]; ///< �X�փ|�X�g�t���O
	extern const _TCHAR kCashService[]; ///< �L���b�V���T�[�r�X�t���O
	extern const _TCHAR kHwyOasis[]; ///< �n�C�E�F�C�I�A�V�X�t���O
	extern const _TCHAR kCarWash[]; ///< �R�C����ԏ�t���O
	extern const _TCHAR kDogRun[];	//!< �h�b�O�����t���O

	namespace GS {
		enum ECode {
			kNoGS = 0,						//!< GS����
			kOther = 1,					//!< ���̑��E����
			kEsso = 2,						//!< �G�b�\
			kMobil = 3,					//!< ���[�r��
			kKygnus = 4,					//!< �L�O�i�X
			kCosmo = 5,					//!< �R�X��
			kShell = 6,					//!< �V�F��
			kJomo = 7,						//!< �W����
			kGeneral = 8,					//!< �[�l����
			kStork = 9,					//!< ��B
			kIdemitsu = 10,					//!< �o��
			kTaiyo = 11,					//!< ���z
			kEneos = 12,					//!< ENEOS
		};
	}
};

/// �n�C�E�F�C�m�[�h�t�B�[�`���N���X
namespace highway_node {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u���i���C���j��

	extern const _TCHAR kHwyMode[];			///< �n�C�E�F�C���[�h�Ή��t���O
	extern const _TCHAR kTollClass[];		///< ��������ʃR�[�h

	// ��ʎԗ��i��ETC�񓋍ڎԗ��j�p
	extern const _TCHAR kNoEtcCar[];		///< ��ʎԗ��Ή��t���O
	extern const _TCHAR kTollFunc[];		///< �������@�\�R�[�h
	extern const _TCHAR kDummyToll[];		///< ��U���������t���O

	// ETC���ڎԗ��p
	extern const _TCHAR kEtcFacil[];		///< ETC�{�݃R�[�h
	extern const _TCHAR kEtcTollFunc[];		///< �������@�\�R�[�h(ETC)
	extern const _TCHAR kEtcNoGuideToll[];	///< ��U���������t���O(ETC)
	extern const _TCHAR kTollgateID[];		///< ������ID
	
	extern const _TCHAR kGuide[];			///< �ē��������t���O
	extern const _TCHAR kRoadSeq2[];		///< tollnode�p�H�����V�[�P���X
	extern const _TCHAR kApproachNodeID[];	///< �����m�[�h�h�c
	extern const _TCHAR kFlowNodeID[];		///< ���o�m�[�h�h�c

	extern const _TCHAR kTollGateLane[];	///< ���������[�����

	extern const _TCHAR kHWYGuideNumber[];  //!< �����{�݈ē��ԍ��ibug 10205�j

	/// ��������ʃR�[�h��`
	namespace toll_class {
		enum ECode {
			kNoTollbooth   = 0, ///< ���򍇗��m�[�h
			kMainTollbooth = 1, ///< �{���㗿����
			kRampTollbooth = 2, ///< �����v�㗿����
			kBarrier       = 3, ///< �o���A
			kGateway       = 4, ///< �o�����m�[�h
		};
	}

	/// �������@�\�R�[�h��`
	namespace toll_func {
		enum ECode {
			kNoFunc             = 0, ///< �@�\����
			kTicket             = 1, ///< �����@�\
			kAdjust             = 2, ///< ���Z�@�\
			kAlone              = 3, ///< �P�Ɨ�����
			kAloneTicket        = 4, ///< �P�Ɨ������{�����@�\
			kAloneAdjust        = 5, ///< �P�Ɨ������{���Z�@�\
			kCheckTicket        = 6, ///< U�^�[���p���D�@�\
			kAloneInvalidTicket = 7, ///< �P�Ɨ������������@�\
			kAdjustTicket		= 8, ///< ���Z�@�\�{�����@�\
		};
	}

	/// ETC�{�݃R�[�h��`
	namespace etc_facil {
		enum ECode {
			kNoEtcFacil	= 0,	///< ETC��Ή�
			kGate		= 1,	///< �Q�[�g
			kAntenna	= 2,	///< �A���e�i
		};
	}

	/// ���������[�����
	namespace tollgate_lane {
		enum ECode {
			kUnusable		= 0,	///< �g�p�s��
			kGeneralLane	= 1,	///< ��ʃ��[��
			kEtcLane		= 2,	///< ETC��p���[��
			kMixedLane		= 3,	///< ���݃��[��
			kSwitchingLane	= 4,	///< �؂�ւ����[��
			kUnknownLane	= 5,	///< �s�����[��
		};
	}
}

/// �n�C�E�F�C�e�L�X�g�e�[�u��
namespace highway_text {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::highway_facil_relational_table;
	using namespace ::sindy::schema::category::highway_in_out_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kPointID[]; ///< �n�C�E�F�C�|�C���g�h�c
	extern const _TCHAR kOrderSeq[]; ///< �o�����V�[�P���X
	extern const _TCHAR kUpDown[]; ///< �����R�[�h
	extern const _TCHAR kNameKanji[]; ///< �|�C���g����
	extern const _TCHAR kPrevID[]; ///< �O�̎{�݂ւ̃|�C���g�h�c
	extern const _TCHAR kNextID[]; ///< ���̎{�݂ւ̃|�C���g�h�c
	extern const _TCHAR kParentID[]; ///< ���݂̐e�{�݂̃|�C���g�h�c
	extern const _TCHAR kForeCount[]; ///< �O������
	extern const _TCHAR kForeID0[];
	extern const _TCHAR kForeID1[]; ///< �O������h�c�P
	extern const _TCHAR kForeID2[]; ///< �O������h�c�Q
	extern const _TCHAR kForeID3[]; ///< �O������h�c�R
	extern const _TCHAR kForeID4[]; ///< �O������h�c�S
	extern const _TCHAR kForeDir1[]; ///< �O����������R�[�h�P
	extern const _TCHAR kForeDir2[]; ///< �O����������R�[�h�Q
	extern const _TCHAR kForeDir3[]; ///< �O����������R�[�h�R
	extern const _TCHAR kForeDir4[]; ///< �O����������R�[�h�S
	extern const _TCHAR kBackCount[]; ///< �������
	extern const _TCHAR kBackID0[];
	extern const _TCHAR kBackID1[]; ///< �������h�c�P
	extern const _TCHAR kBackID2[]; ///< �������h�c�Q
	extern const _TCHAR kBackID3[]; ///< �������h�c�R
	extern const _TCHAR kBackID4[]; ///< �������h�c�S
	extern const _TCHAR kBackDir1[]; ///< �����������R�[�h�P
	extern const _TCHAR kBackDir2[]; ///< �����������R�[�h�Q
	extern const _TCHAR kBackDir3[]; ///< �����������R�[�h�R
	extern const _TCHAR kBackDir4[]; ///< �����������R�[�h�S

	extern const LPCTSTR kForeID[5]; ///< �O������h�c�ꗗ
	extern const LPCTSTR kBackID[5]; ///< �������h�c�ꗗ

	/// �����R�[�h�Q�R�[�h��`
	namespace up_down {
		enum ECode {
			kUp   = 1, ///< ���E�����E���s��
			kDown = 2, ///< ����E�O���E���s��
		};
	}

	/// ��������R�[�h��`
	namespace dircode {
		enum ECode {
			kStraight = 0, //!< ���i����
			kRight    = 1, //!< �E����
			kLeft     = 2, //!< ������
			kUnknown  = 3, //!< �s��
		};
	}
}

/// �H���R�[�h���X�g�e�[�u��
namespace road_code_list {
	using namespace ::sindy::schema::category::link_relational_table;
	using namespace ::sindy::schema::category::highway_road_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kLineClass[]; ///< �H����ʃR�[�h
	extern const _TCHAR kLineDirType[]; ///< �H�������^�C�v�R�[�h
	extern const _TCHAR kNameKanji[]; ///< �H�����̊���
	extern const _TCHAR kNameYomi[]; ///< �H�����̓ǂ�
	extern const _TCHAR kWorkKanji[]; ///< �H����������(Hwy���[�h�p)
	extern const _TCHAR kWorkYomi[]; ///< �H���J�i����(Hwy���[�h�p)
	extern const _TCHAR kDisplayKanji[]; ///< �H�����̊���(�\���p)
	extern const _TCHAR kDisplayYomi[]; ///< �H�����̓ǂ�(�\���p)
	extern const _TCHAR kShieldNoStr[]; ///< �W���ԍ��E�L�� [bug12036]

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
	}

	/// �H�������^�C�v�R�[�h��`
	namespace line_dir_type {
		enum ECode {
			kNoType		= 0,	///< �^�C�v�Ȃ�
			kUpDown		= 1,	///< ���E����
			kInOut		= 2,	///< �����E�O���
			kEastWest	= 3,	///< ���s���E���s��
			kOther		= 4,	///< ���̑�
		};
	}
}

/// �s���{�������X�g [bug 12036]
namespace pref_road_list {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::pref_road_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kNameKanji[];	///< �H����������
	extern const _TCHAR kNameYomi[];	///< �H�����~����
} // namespace pref_road_list

/// ��ԕʏ̃R�[�h���X�g [bug 12036]
namespace section_code_list {
	using namespace ::sindy::schema::ipc_table;
	using namespace ::sindy::schema::category::section_code_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kNameKanji[];	///< �H����������
	extern const _TCHAR kNameYomi[];	///< �H�����~����
} // namespace section_code_list

/// �����W�|�C���g
namespace distance_post_point {
	using namespace ::sindy::schema::ipcfeature;
	using namespace ::sindy::schema::category::highway_road_relational_table;
	using namespace ::sindy::schema::category::highway_direction_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kDistance[];		///< �����W�l
	extern const _TCHAR kDistanceMark[];	///< �����W�L��
}

/// HWY�X�܏��e�[�u��
namespace access_store {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kAccessPointID[];	///< �Ή�����A�N�Z�X�|�C���g�̃I�u�W�F�N�gID
	extern const _TCHAR kMultipleStore[];	///< �`�F�[���X�}�X�^�[�R�[�h
	extern const _TCHAR kStartHour[];		///< �c�ƊJ�n����
	extern const _TCHAR kStartMin[];		///< �c�ƊJ�n��
	extern const _TCHAR kEndHour[];			///< �c�ƏI������
	extern const _TCHAR kEndMin[];			///< �c�ƏI����
	extern const _TCHAR kSunday[];			///< ���j���c�ƃt���O
	extern const _TCHAR kMonday[];			///< ���j���c�ƃt���O
	extern const _TCHAR kTuesday[];			///< �Ηj���c�ƃt���O
	extern const _TCHAR kWednesday[];		///< ���j���c�ƃt���O
	extern const _TCHAR kThursday[];		///< �ؗj���c�ƃt���O
	extern const _TCHAR kFriday[];			///< ���j���c�ƃt���O
	extern const _TCHAR kSaturday[];		///< �y�j���c�ƃt���O
	extern const _TCHAR kBon[];				///< ���~�c�ƃt���O
	extern const _TCHAR kYearEnd[];			///< �N���c�ƃt���O
	extern const _TCHAR kNewYear[];			///< �N�n�c�ƃt���O
	extern const _TCHAR kGoldenWeek[];		///< �S�[���f���E�B�[�N�c�ƃt���O
	extern const _TCHAR kHoliday[];			///< �j�Փ��c�ƃt���O
	extern const _TCHAR kStoreType[];		///< �`�F�[���X��ʃR�[�h

	namespace store_type {
		enum ECode {
			kGS			= 0,	///< �K�\�����X�^���h
			kChainStore	= 1,	///< �`�F�[���X
		};
	}
}

/// iPC�����e�[�u��
namespace ipc_toll_table {

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kRoadCode1[]; ///< �H���R�[�h
	extern const _TCHAR kRoadSeq1[]; ///< �H�����V�[�P���X
	extern const _TCHAR kRoadCode2[]; ///< �H���R�[�h
	extern const _TCHAR kRoadSeq2[]; ///< �H�����V�[�P���X
	extern const _TCHAR kKToll[]; ///< ��ցE�y����
	extern const _TCHAR kSToll[]; ///< ���ʎԗ���
	extern const _TCHAR kMToll[]; ///< ���^�ԗ���
	extern const _TCHAR kLToll[]; ///< ��^�ԗ���
	extern const _TCHAR kVLToll[]; ///< ����ԗ���
	extern const _TCHAR kNormal[]; ///< ��ʗL���������t���O
}

/// �n�C�E�F�C��Ή��H�������e�[�u��
namespace toll_other_table {
	using namespace ipc_toll_table;
}

/// �P�Ɨ����e�[�u��
namespace toll_alone_table {
	using namespace ::sindy::schema::category::highway_facil_relational_table;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kLineClass[]; ///< �H����ʃR�[�h
	extern const _TCHAR kKTollUp[]; ///< ���:��ցE�y����
	extern const _TCHAR kSTollUp[]; ///< ���:���ʎԗ���
	extern const _TCHAR kMTollUp[]; ///< ���:���^�ԗ���
	extern const _TCHAR kLTollUp[]; ///< ���:��^�ԗ���
	extern const _TCHAR kVLTollUp[]; ///< ���:����ԗ���
	extern const _TCHAR kKTollDown[]; ///< ����:��ցE�y����
	extern const _TCHAR kSTollDown[]; ///< ����:���ʎԗ���
	extern const _TCHAR kMTollDown[]; ///< ����:���^�ԗ���
	extern const _TCHAR kLTollDown[]; ///< ����:��^�ԗ���
	extern const _TCHAR kVLTollDown[]; ///< ����:����ԗ���
	extern const _TCHAR kNormal[]; ///< ��ʗL���������t���O

	/// �H����ʃR�[�h
	namespace line_class {
		/// �H����ʃR�[�h�ꗗ
		typedef ::sindy::schema::road_code_list::line_class::ECode ECode;
	}
}

//�ꎞ��~�|�C���g(�{���ɂ͖��K�p 2010.2.25)
namespace stop_point {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[]; ///< �e�[�u����

	extern const _TCHAR kLinkID[]; ///< �����NID
	extern const _TCHAR kNodeID[]; ///< �m�[�hID
}

/// �Ԏ�ʋK���}�X�^
namespace vehicle_reg_master {
	extern const _TCHAR kTableName[];  //!< �e�[�u���i���C���j��

	extern const _TCHAR kVehicleRegCode[];    //!< �Ԏ�ʋK���R�[�h
	extern const _TCHAR kVehicleRegName[];    //!< �K���̎��
	extern const _TCHAR kOgataJoyoF[];        //!< ���
	extern const _TCHAR kTokuteiChuJoF[];     //!< ���蒆��
	extern const _TCHAR kChugataJoyoF[];      //!< ����
	extern const _TCHAR kFutsuJoyoF[];        //!< ����
	extern const _TCHAR kFutsuJoyoKeiF[];     //!< ����i�y�j
	extern const _TCHAR kOgataKamotsuF[];     //!< ���
	extern const _TCHAR kTokuteiChuKaF[];     //!< ���蒆��
	extern const _TCHAR kChugataKamotsuF[];   //!< ����
	extern const _TCHAR kFutsuKamotsuF[];     //!< ����
	extern const _TCHAR kFutsuKamotsuKeiF[];  //!< ���݁i�y�j
	extern const _TCHAR kOgataTokushuF[];     //!< ���
	extern const _TCHAR kKogataTokushuF[];    //!< ����
	extern const _TCHAR kOgataBikeF[];        //!< ��^���
	extern const _TCHAR kFutsuBikeF[];        //!< ���ʓ��
	extern const _TCHAR kKogataBikeF[];       //!< ���^���
	extern const _TCHAR kTaxiF[];             //!< �^�N�V�[
	extern const _TCHAR kBusF[];              //!< �H���o�X
	extern const _TCHAR kMiniBikeF[];         //!< �����@�t���]��
	extern const _TCHAR kTrolleyF[];          //!< �g�����[
	extern const _TCHAR kBicycleF[];          //!< ���]��
	extern const _TCHAR kExceptBicycleF[];    //!< ���]�ԈȊO�̌y�ԗ�
	extern const _TCHAR kSignageIllustFile[]; //!< �K���摜��
}

/// �Ԏ�ʋK���e�[�u��
namespace vehicle_reg {
	extern const _TCHAR kTableName[];      //!< �e�[�u����
	
	extern const _TCHAR kLinkID[];         //!< ���H�����N��OID

	using namespace category::time_condition_table;

	extern const _TCHAR kRegTypeC[];       //!< �K���^�C�v
	extern const _TCHAR kHeight[];         //!< �ԍ�
	extern const _TCHAR kWidth[];          //!< �ԕ�
	extern const _TCHAR kWeight[];         //!< �ԏd
	extern const _TCHAR kLength[];         //!< �Ԓ�
	extern const _TCHAR kLoadage[];        //!< �ύڗ�
	extern const _TCHAR kDangerF[];        //!< �댯���t���O

	/// �K���^�C�v�R�[�h��`
	namespace reg_type {
		enum ECode {
			kOneway    = 0,            //!< ����ʍs�i�������j
			kROneway   = 1,            //!< ����ʍs�i�t�����j
			kNoPassage = 2,            //!< �ʍs�֎~
		};
	}
	
	/// �j���R�[�h
	using namespace ::sindy::schema::day_code;
}

/// �Ԏ�e�[�u��
namespace vehicle_type{
	extern const _TCHAR kTableName[];      //!< �e�[�u����

	extern const _TCHAR kRegID[];           //!< VEHICLE_REG��OID
	extern const _TCHAR kVehicleRegCode[];  //!< �Ԏ�ʋK���R�[�h
	extern const _TCHAR kExcludeF[];        //!< ���O�t���O
}

/// �d�Ȃ胊���N�̍����m�[�h
namespace height_node
{
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];      ///< �e�[�u����

	extern const _TCHAR kLinkID1[];        ///< �����N1�̃����NID
	extern const _TCHAR kLinkID1Height[];  ///< �����N1�̑��΍���
	extern const _TCHAR kLinkID1Table[];   ///< �����N1�̃����N�e�[�u��
	extern const _TCHAR kLinkID2[];        ///< �����N2�̃����NID
	extern const _TCHAR kLinkID2Height[];  ///< �����N2�̑��΍���
	extern const _TCHAR kLinkID2Table[];   ///< �����N2�̃����N�e�[�u��

	namespace link_table{
		enum ECode {
			kNone     = 0,    ///< ������
			kWalkLink = 1,    ///< ���s�҃����N
			kRoadLink = 2,    ///< ���H�����N
		};
	}

} // namespace height_node

/// ���s�ҁE���H�����N�R�Â��e�[�u��
namespace link_relation
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];       ///< �e�[�u����

	extern const _TCHAR kBaseLinkID[];      ///< ������NID
	extern const _TCHAR kRelatedLinkID[];   ///< �֘A�����NID
	extern const _TCHAR kLinkSide[];   ///< �����N���Έʒu

	namespace link_side_class{
		enum ECode{
			kRight    = 1,   ///< �E
			kLeft     = 2,   ///< ��
		};
	}

} // namespace link_relation

/// ���s�ҘH���R�[�h���X�g
namespace walk_code_list {
	extern const _TCHAR kTableName[];   ///< �e�[�u����

	extern const _TCHAR kOperator[];    ///< �ŏI�X�V��
	extern const _TCHAR kModifyDate[];  ///< �ŏI�X�V����
	extern const _TCHAR kWalkCode[];    ///< ���s�ҘH���R�[�h
	extern const _TCHAR kNameKanji[];   ///< ���s�ҘH������
	extern const _TCHAR kNameYomi[];    ///< ���s�ҘH���ǂ�

} // namespace walk_code_list

/// �x���W��
namespace warning_sign {
	using namespace ::sindy::schema::ipcfeature;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kLinkID[];      //!< �����NID
	extern const _TCHAR kWarningSign[]; //!< �x���W�����
	extern const _TCHAR kPicturePath[]; //!< ���s�摜�t�@�C���p�X

	/// �x���W����ʃR�[�h
	namespace warning_sign_class {
		enum ECode {
			kNone                =   0, //!< ���ݒ�
			kFallingLock         =   1, //!< ���΂̋��ꂠ��
			kSlippy              =   2  //!< ���ׂ�₷��
		};
	}
} // warning_sign

	
  /// ETC������
namespace etc_tollgate {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];

	extern const _TCHAR kTollgateID[];			//!< ������ID
	extern const _TCHAR kTollgateParentID[];	//!< �������d���Ǘ�ID
	extern const _TCHAR kTollgateOfficeID[];	//!< ���Ə�ID
	extern const _TCHAR kTollgateOfficeName[];	//!< ���Ə���
	extern const _TCHAR kTollgateRoadNumber[];	//!< �H���ԍ�
	extern const _TCHAR kTollgateRoadName[];	//!< �H����
	extern const _TCHAR kTollgateICNumber[];	//!< IC�ԍ�
	extern const _TCHAR kTollgateICName[];		//!< IC��
	extern const _TCHAR kTollgateICNameYomi[];	//!< IC�����~
	extern const _TCHAR kTollgateFlag[];		//!< �������t���O
	extern const _TCHAR kTollgateOther[];		//!< ���̑�

} // etc_tollgate


} // namespace schema

} // namespace sindy

#endif // SINDY_SCHEMA_ROAD_H_
