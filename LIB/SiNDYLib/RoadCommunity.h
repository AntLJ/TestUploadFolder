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

/**
 * @file RoadCommunity.h
 * @brief ���H�e�[�u���t�B�[���h�E�h���C����`
 * @author 
 * $Id$
 */
#ifndef _ROADCOMMUNITY_H_
#define _ROADCOMMUNITY_H_

namespace sindy {
	/**
	 * \namespace sindy::direction_code
	 * @brief �n�C�E�F�C�����R�[�h�p�l�[���X�y�[�X
	 */	
	namespace direction_code {
		/**
		 * \enum sindy::direction_code::ECode
		 * @brief �n�C�E�F�C�����R�[�h�h���C��
		 */
		enum ECode
		{
			kNoDirection,			//!< �����Ȃ�
			kUpInnerWest,			//!< ���E�����E���s��
			kDownOuterEast,			//!< ����E�O���E���s��
			kBoth					//!< �o��������
		};
	}

	/**
	 * \namespace sindy::inout_code
	 * @brief �n�C�E�F�CINOUT�R�[�h�p�l�[���X�y�[�X
	 */	
	namespace inout_code {
		/**
		 * \enum sindy::inout_code::ECode
		 * @brief �n�C�E�F�CINOUT�R�[�h�h���C��
		 */
		enum ECode
		{
			kNoDistinction,			//!< ��ʂȂ�
			kEntranceConfluence,	//!< �����E����
			kExitDivergence			//!< �o���E����
		};
	}

	/**
	 * \namespace sindy::gs_code
	 * @brief �K�\�����X�^���h���ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace gs_code {
		/**
		 * \enum sindy::gs_code::ECode
		 * @brief �K�\�����X�^���h���ʃR�[�h�h���C��
		 */
		enum ECode
		{
			kNoGS,					//!< GS����
			kOther,					//!< ���̑��E����
			kiEsso,					//!< �G�b�\
			kMobil,					//!< ���[�r��
			kKygnus,				//!< �L�O�i�X
			kCosmo,					//!< �R�X��
			kShell,					//!< �V�F��
			kJomo,					//!< �W����
			kGeneral,				//!< �[�l����
			kStork,					//!< ��B
			kIdemitsu,				//!< �o��
			kTaiyo,					//!< ���z
			kEneos					//!< ENEOS
		};
	}

	/**
	 * \namespace sindy::facilclass_code
	 * @brief Highway�{�ݎ�ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace facilclass_code {
		/**
		 * \enum sindy::facilclass_code::ECode
		 * @brief Highway�{�ݎ�ʃR�[�h�h���C��
		 */
		enum ECode
		{
			kSA,					//!< SA
			kPA,					//!< PA
			kJCT,					//!< JCT
			kRamp,					//!< �����v
			kIC,					//!< IC
			kTollgate				//!< ������
		};
	}

	/**
	 * \namespace sindy::tollclass_code
	 * @brief ��������ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace tollclass_code {
		/**
		 * \enum sindy::tollclass_code::ECode
		 * @brief ��������ʃR�[�h�h���C��
		 */
		enum ECode
		{
			kOther,					//!< �������ȊO
			kMainTollgate,			//!< �{���㗿����
			kRampTollgate,			//!< �����v�㗿����
			kBarrier				//!< �o���A
		};
	}

	/**
	 * \namespace sindy::tollfunc_code
	 * @brief �������@�\�R�[�h2�p�l�[���X�y�[�X
	 */	
	namespace tollfunc_code {
		/**
		 * \enum sindy::tollfunc_code::ECode
		 * @brief �������@�\�R�[�h2�h���C��
		 */
		enum ECode
		{
			kNoFunction,							//!< �@�\����
			kTicketIssue,							//!< �����@�\
			kAdjustment,							//!< ���Z�@�\
			kStandalone,							//!< �P�Ɨ�����
			kStandaloneWithTicketIssue,				//!< �P�Ɨ������{�����@�\
			kStandaloneWithAdjustment,				//!< �P�Ɨ������{���Z�@�\
			kUTurnTicketInspectionIssue,			//!< U�^�[���p���D�@�\
			kStandaloneChargeInvalidTicketIssue,	//!< �P�Ɨ������������@�\
		};
	}

	/**
	 * \namespace sindy::roadclass_code
	 * @brief ���H��ʃR�[�h3�p�l�[���X�y�[�X
	 */	
	namespace roadclass_code {
		/**
		 * \enum sindy::roadclass_code::ECode
		 * @brief ���H��ʃR�[�h3�h���C��
		 */
		enum ECode
		{
			kNoDisplayKind = 0,				//!< �\����ʂȂ�
			kExpressway = 101,				//!< ���������ԓ��H
			kCityExpressway = 102,			//!< �s�s�������H
			kNational = 1,					//!< ��ʍ���
			kTurnpike = 103,				//!< ��ʍ����i�L���j
			kMainLocalPrefRoad = 2,			//!< ��v�n�����i�s�{���j
			kMainLocalPrefTollRoad = 104,	//!< ��v�n�����i�s�{���j�i�L���j
			kMainLocalRoad = 3,				//!< ��v�n�����i�w��s�j
			kMainLocalTollRoad = 105,		//!< ��v�n�����i�w��s�j�i�L���j
			kPrefecturesRoad = 4,			//!< ��ʓs���{����
			kPrefecturesTollRoad = 106,		//!< ��ʓs���{�����i�L���j
			kCityRoad = 5,					//!< �w��s�̈�ʎs��
			kCityTollRoad = 107,			//!< �w��s�̈�ʎs���i�L���j
			kOtherRoad = 6,					//!< ���̑����H
			kOtherTollRoad = 108,			//!< ���̑����H�i�L���j
			kOtherCullRoad = 7,				//!< ���̑����H�i�Ԉ��Ώہj
			kNarrowPathL1 = 8,				//!< �׊X�HL1
			kNarrowPathL2 = 9,				//!< �׊X�HL2
			kNarrowPathL3 = 10,				//!< �׊X�HL3
			kFerryLineS1 = 201,				//!< �t�F���[�q�H(��\����)
			kFerryLineS2 = 202,				//!< �t�F���[�q�H(S2�j���\����)
			kFerryLineS3 = 203,				//!< �t�F���[�q�H(S3�j���\����)
			kFerryLineS4 = 204,				//!< �t�F���[�q�H(S4�j���\����)
			kVirtualLink = 301,				//!< ���z�����N
			kGardenCourse = 302,			//!< �뉀�H
			kBridgeLink = 303,				//!< �u���b�W�����N
			kInstituteGateway = 304,		//!< �{�ݏo���������N
			kParkingPremisesLink = 401,		//!< ���ԏ�\�����H�����N�iPEC�j
			kMatchingLink = 402,			//!< �}�b�`���O�����N�iPEC�j
			kInstituteLink = 305			//!< �{�ݓ������N
		};
	}

	/**
	 * \namespace sindy::naviclass_code
	 * @brief ���H�o�H�p��ʃR�[�h2�p�l�[���X�y�[�X
	 */	
	namespace naviclass_code {
		/**
		 * \enum sindy::naviclass_code::ECode
		 * @brief ���H�o�H�p��ʃR�[�h2�h���C��
		 */
		enum ECode
		{
			kNotCourse = 0,					//!< �o�H�O���H
			kExpressway = 101,				//!< ���������ԓ��H
			kCityExpressway = 102,			//!< �s�s�������H
			kNational = 1,					//!< ��ʍ���
			kTurnpike = 103,				//!< ��ʍ����i�L���j
			kMainLocalPrefRoad = 2,			//!< ��v�n�����i�s�{���j
			kMainLoaclPrefTollRoad = 104,	//!< ��v�n�����i�s�{���j�i�L���j
			kMainLocalRoad = 3,				//!< ��v�n�����i�w��s�j
			kMainLocalTollRoad = 105,		//!< ��v�n�����i�w��s�j�i�L���j
			kPrefecturesRoad = 4,			//!< ��ʓs���{����
			kPrefecturesTollRoad = 106,		//!< ��ʓs���{�����i�L���j
			kCityRoad = 5,					//!< �w��s�̈�ʎs��
			kCityTollRoad = 107,			//!< �w��s�̈�ʎs���i�L���j
			kOtherRoad = 6,					//!< ���̑����H
			kOtherTollRoad = 108,			//!< ���̑����H�i�L���j
			kConnectNational = 403,			//!< �ڑ�����
			kFerryLine = 201				//!< �t�F���[�q�H
		};
	}

	/**
	 * \namespace sindy::linkclass_code
	 * @brief �����N��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace linkclass_code {
		/**
		 * \enum sindy::linkclass_code::ECode
		 * @brief �����N��ʃR�[�h�h���C��
		 */
		enum ECode
		{
			kUnInvestigated = 0,			//!< �������E��ƒ�
			kMainLine = 1,					//!< �{��(�㉺���񕪗�)�����N
			kMainLineWithMedian = 2,		//!< �{��(�㉺����)�����N
			kConnection = 3,				//!< �A���H(�{���Ԃ̓n���)
			kIntersection = 4,				//!< �����_�������N
			kRamp = 5,						//!< �A���H(�����v)�����N
			kSideLine = 6,					//!< �{���Ɠ���H���̑��������N
			kSA = 7,						//!< �r�`�ȂǑ��������N
			kCyclingRoad = 8,				//!< ���]�ԓ���
			kConnectSideLine = 9,			//!< �{�������ڑ������N
		};
	}

	/**
	 * \namespace sindy::roadmanager_code
	 * @brief ���H�Ǘ��҃R�[�h�p�l�[���X�y�[�X
	 */	
	namespace roadmanager_code {
		/**
		 * \enum sindy::roadmanager_code::ECode
		 * @brief ���H�Ǘ��҃R�[�h�h���C��
		 */
		enum ECode
		{
			kUnInvestigated = 0,			//!< �������E��ƒ�
			kJH = 1,						//!< ���{���H���c
			kRoadPublicCorp3 = 2,			//!< ���H�R���c
			kRoadPuclicCorp = 3,			//!< ���H����
			kNational = 4,					//!< ��
			kPrefectures = 5,				//!< �s���{��
			kCity = 6,						//!< �w��s
			kMunicipality = 7,				//!< ���̎s����
			kOthers = 8,					//!< ���̑��̊Ǘ���
		};
	}
	
	/**
	 * \namespace sindy::roadwidth_code
	 * @brief ���H�����敪�R�[�h2�p�l�[���X�y�[�X
	 */	
	namespace roadwidth_code {
		/**
		 * \enum sindy::roadwidth_code::ECode
		 * @brief ���H�����敪�R�[�h2�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< ������
			kOver130 = 1,					//!< ����13.0m�ȏ�
			kFrom55To130 = 2,				//!< ����5.5m�ȏ�13.0m����
			kFrom30To55 = 3,				//!< ����3.0m�ȏ�5.5m����
			kLess30 = 4,					//!< ����3.0m����
		};
	}

	/**
	 * \namespace sindy::widearea_code
	 * @brief �L�摶�݃R�[�h2�p�l�[���X�y�[�X
	 */	
	namespace widearea_code {
		/**
		 * \enum sindy::widearea_code::ECode
		 * @brief �L�摶�݃R�[�h2�h���C��
		 */
		enum ECode 
		{
			kNoWideArea = 0,				//!< �L��Ȃ�
			kAvoidCharge = 1,				//!< �L�����
			kBasic = 2,						//!< �W��
		};
	}

	/**
	 * \namespace sindy::updownclass_code
	 * @brief �㉺����ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace updownclass_code {
		/**
		 * \enum sindy::updownclass_code::ECode
		 * @brief �㉺����ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< ������
			kInner = 1,						//!< ���E�����E���s��
			kOuter = 2,						//!< ����E�O���E���s��
			kCommon = 3,					//!< �㉺������
		};
	}

	/**
	 * \namespace sindy::nopassage_code
	 * @brief �ʍs�֎~��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace nopassage_code {
		/**
		 * \enum sindy::nopassage_code::ECode
		 * @brief �ʍs�֎~��ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kNothing = 0,					//!< �ʍs�֎~�Ȃ�
			kNoPassage = 1,					//!< �����Ȃ��ʍs�֎~
			kNoPassageWithCondition = 2,	//!< �����t���ʍs�֎~
		};
	}

	/**
	 * \namespace sindy::oneway_code
	 * @brief ����ʍs��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace oneway_code {
		/**
		 * \enum sindy::oneway_code::ECode
		 * @brief ����ʍs��ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kNothing = 0,					//!< ����ʍs�Ȃ�
			kOneway = 1,					//!< �����Ȃ�����ʍs�i�������j
			kOnewayRev = 2,					//!< �����Ȃ�����ʍs�i�t�����j
			kOnewayWithCond = 3,			//!< �����t������ʍs�i�������j
			kOnewayRevWithCond = 4,			//!< �����t������ʍs�i�t�����j
			kOnwwayBothWithCond = 5,		//!< �����t������ʍs�i���t����j
		};
	}

	/**
	 * \namespace sindy::limitspeed_code
	 * @brief �K�����x�R�[�h2�p�l�[���X�y�[�X
	 */	
	namespace limitspeed_code {
		/**
		 * \enum sindy::limitspeed_code::ECode
		 * @brief �K�����x�R�[�h2�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< ������
			kLess30 = 1,					//!< �R�Okm/h����
			k40 = 2,						//!< �S�Okm/h
			k50 = 3,						//!< �T�Okm/h
			k60 = 4,						//!< �U�Okm/h
			k70 = 5,						//!< �V�Okm/h
			k80 = 6,						//!< �W�Okm/h
			k100 = 7,						//!< �P�O�Okm/h
		};
	}

	/**
	 * \namespace sindy::kubun_code
	 * @brief ��]���H�敪�R�[�h�p�l�[���X�y�[�X
	 */	
	namespace kubun_code {
		/**
		 * \enum sindy::kubun_code::ECode
		 * @brief ��]���H�敪�R�[�h�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,			//!< ������
			kMainRoad = 1,					//!< �哹�H
			kSubRoad = 2,					//!< �]���H
		};
	}

	/**
	 * \namespace sindy::alleymainte_code
	 * @brief �׊X�H��ƔN�x�R�[�h�p�l�[���X�y�[�X
	 */	
	namespace alleymainte_code {
		/**
		 * \enum sindy::alleymainte_code::ECode
		 * @brief �׊X�H��ƔN�x�R�[�h�h���C��
		 */
		enum ECode 
		{
			kNoData = 0,					//!< �Y���Ȃ�
			k1998 = 1,						//!< 1998�N�x
			k1999 = 2,						//!< 1999�N�x
			k2000Spring = 3,				//!< 2000�N�x�i�t�j
			k2000Autumn = 4,				//!< 2000�N�x�i�H�j
			k2001 = 5,						//!< 2001�N�x
			k2002 = 6,						//!< 2002�N�x
			k2003 = 7,						//!< 2003�N�x
		};
	}

	/**
	 * \namespace sindy::parkingclass_code
	 * @brief ���ԏ��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace parkingclass_code {
		/**
		 * \enum sindy::parkingclass_code::ECode
		 * @brief ���ԏ��ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,				//!< ������
			kParkingStructure = 1,				//!< ���̒��ԏ�
			kUndergroundParking = 2,			//!< �n�����ԏ�
			kUndergroundParkingStructure = 3,	//!< ���̂��n�����ԏ�
			kTowerParking = 4,					//!< �^���[�p�[�L���O
			kAutoRotateTowerParking = 5,		//!< ������]�^���[�p�[�L���O
			kParkingLot = 6,					//!< ���ʒ��ԏ�
		};
	}

	/**
	 * \namespace sindy::parkinglinkclass_code
	 * @brief ���ԏꃊ���N��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace parkinglinkclass_code {
		/**
		 * \enum sindy::parkinglinkclass_code::ECode
		 * @brief ���ԏꃊ���N��ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kUnInvestigated = 0,				//!< ������
			kEntrance = 1,						//!< ���������N
			kExit = 2,							//!< �o�������N
			kGateway = 3,						//!< �o���������N
			kParkingPrecinct = 4,				//!< ���ԏ�\�������N
			kLevelConnect = 5,					//!< ���x���Ԃ̓n�胊���N
			kParkingConnect = 6,				//!< ���ԏ�ڑ������N
		};
	}

	/**
	 * \namespace sindy::nodeclass_code
	 * @brief �m�[�h��ʃR�[�h3�p�l�[���X�y�[�X
	 */	
	namespace nodeclass_code {
		/**
		 * \enum sindy::nodeclass_code::ECode
		 * @brief �m�[�h��ʃR�[�h3�h���C��
		 */
		enum ECode 
		{
			kEmptyNode = 0,					//!< �@�\�Ȃ��m�[�h
			kIntersectionNode = 1,			//!< �����_�m�[�h
			kEndNode = 2,					//!< �[�_�m�[�h
			kAttributeChangeNode = 3,		//!< �����ω��_�m�[�h
			kAttributeNode = 4,				//!< �����t���m�[�h
			kPECLinkNode = 5,				//!< PEC�����N�ڑ��m�[�h
			kLinkQueueNode = 6,				//!< �����N��֌W�m�[�h
			kTollGateNode = 7,				//!< �������m�[�h
			kHighwayNode = 8,				//!< �n�C�E�F�C�֌W�m�[�h
			kMeshNode = 9,					//!< �Q�����b�V�����ӏ�m�[�h
			kIdentityCheckNode = 10,		//!< ���ꃊ���N�h�c�h�~�m�[�h
			kPECNode = 11,					//!< PEC�m�[�h
			kMeshPECNode = 12,				//!< �Q�����b�V�����ӏ�PEC�m�[�h
			kDummyNode = 13,				//!< �_�~�[�{�݃m�[�h
		};
	}
	 
	/**
	 * \namespace sindy::height_code
	 * @brief �m�[�h���΍����R�[�h�p�l�[���X�y�[�X
	 */	
	namespace height_code {
		/**
		 * \enum sindy::height_code::ECode
		 * @brief �m�[�h���΍����R�[�h�h���C��
		 */
		enum ECode 
		{
			kDisabled = 0,					//!< 	���΍����Ȃ�
			kEnabled = 1,					//!< 	���΍����L��
			kAuto = 2,						//!< 	���΍��������t�^
		};
	}

	/**
	 * \namespace sindy::signal_code
	 * @brief �M���@��ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace signal_code {
		/**
		 * \enum sindy::signal_code::ECode
		 * @brief �M���@��ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kNoSignal = 0,					//!< 		�M���@�Ȃ�
			kSignal = 1,					//!< 		�M���@����
			kUnInvestigated = 2,			//!< 		������
		};
	}

	/**
	 * \namespace sindy::linkattr_code
	 * @brief �����N��������ʃR�[�h2�p�l�[���X�y�[�X
	 */	
	namespace linkattr_code {
		/**
		 * \enum sindy::linkattr_code::ECode
		 * @brief �����N��������ʃR�[�h2�h���C��
		 */
		enum ECode 
		{
			kBridge = 1,					//!< ���E����
			kTunnel = 2,					//!< �g���l��
			kCave = 3,						//!< ����
			kRailroadCrossing = 4,			//!< ����
			kUnderPass = 5,					//!< �A���_�[�p�X
			kUpDownRelationship = 6,		//!< �㉺�֌W
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief �n���ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief �n���ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kGeneral = 1,					//!< �n��i��ʁj
			kToll = 2,						//!< �n��i�L���j
			kNationalWayNo = 3,				//!< �����ԍ�
			kPrefWayNo = 4,					//!< �����ԍ�
			kOther = 5,						//!< ���̑�
			kTollString = 12,				//!< �L���i������j
			kTollNationalWayNo = 13,		//!< �L���i�����ԍ��j
			kTollPrefWayNo = 14,			//!< �L���i�����ԍ��j
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief �n���ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief �n���ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kAccessRoad = 1,				//!< �A�N�Z�X���H
			kConnectRoad = 2,				//!< ���p����
			kInfomation = 3,				//!< �ό��ē�
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief �n���ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief �n���ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			kHoliday = 1,					//!< ���E�x��
			kExceptHolidayAndSunday = 2,	//!< ���E�x�����̂���
			kHolidayExceptSunday = 3,		//!< �x��(���j�͂̂���)
			kExceptHoliday = 4,				//!< �x�����̂���
			kOrdinaryDays = 5,				//!< ����
			kSunday = 6,					//!< ���j
			kMonday = 7,					//!< ��
			kTuesday = 8.					//!< ��
			kWednesday = 9,					//!< ��
			kThursday = 10,					//!< ��
			kFriday = 11,					//!< ��
			kSaturday = 12,					//!< �y
			kWeekday = 13,					//!< �y���E�x��
			kExceptWeekday = 14,			//!< �y���E�x�����̂���
			kAlldays = 15,					//!< �S�j��
			kExceptSunday = 16,				//!< ���j�����̂���
			kBeforeHoliday = 17,			//!< ���E�x���̑O��
		};
	}

	/**
	 * \namespace sindy::signage_code
	 * @brief �n���ʃR�[�h�p�l�[���X�y�[�X
	 */	
	namespace signage_code {
		/**
		 * \enum sindy::signage_code::ECode
		 * @brief �n���ʃR�[�h�h���C��
		 */
		enum ECode 
		{
			case kUnsurveyed:					///< ���ݒ�
			case kNoGuide:						///< ���Ȃ�i�ē��Ȃ��j
			case k12:							///< ���i�i�P�Q�������j
			case k1:							///< �E�O�����i�P�������j
			case k2:							///< �E��O�����i�Q�������j
			case k3:							///< �E�܁i�R�������j
			case k4:							///< �E�΂ߌ������i�S�������j
			case k5:							///< �E�߂�����i�T�������j
			case k6:							///< �߂�����i�U�������j
			case k7:							///< ���߂�����i�V�������j
			case k8:							///< ���΂ߌ������i�W�������j
			case k9:							///< ���܁i�X�������j
			case k10:							///< ����O�����i�P�O�������j
			case k11:							///< ���O�����i�P�P�������j
};

#endif // _ROADCOMMUNITY_H_
