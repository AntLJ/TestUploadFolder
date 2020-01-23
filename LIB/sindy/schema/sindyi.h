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

#pragma once

namespace sindy {

namespace schema {

namespace sindyi {

namespace object_locks {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
};

namespace info_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
};
namespace info_polyline {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
};
namespace info_polygon {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
};

namespace shape{
	namespace jump_type{
		enum ECode{
			eCenter = 0,
			eStartPoint = 1,
			eEndPoint = 2,
			eAllView = 3
		};
	}// jump_type
}// shape

namespace info_master {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kRegistrant[];		//!< �o�^��
	extern const _TCHAR kRegistDate[];		//!< �o�^��
	extern const _TCHAR kTitle[];			//!< �^�C�g��
	extern const _TCHAR kInfoType[];		//!< �����
	extern const _TCHAR kDetails[];			//!< �ڍ׏��
	extern const _TCHAR kMotoID[];			//!< �n�}�̑fID
	extern const _TCHAR kOldInfoID[];		//!< �����DBID
	extern const _TCHAR kRoadManageID[];	//!< ���H�Ǘ�ID
	extern const _TCHAR kMipss[];			//!< ��MIPSSID

	//!< ����ʃR�[�h
	namespace info_type {
		enum ECode {
			kDefault   = 0,		//!< �ڍs������
			kInquiry   = 1,		//!< SiNDY-i����
			kAdjust    = 2,		//!< SiNDY-i����
			kMotoiPC   = 7221,	//!< �n�}�̑f�iiPC�j
			kMotoNEW   = 7309,	//!< �n�}�̑f�i�V���Ј��j
			kMotoPEC   = 8102,	//!< �n�}�̑f�iPEC�j
			kMotoMFW   = 9001,	//!< �n�}�̑f�iMFW�j
			kMotoINV   = 1060,	//!< �n�}�̑f�iINV�j
		};
	} // info_type
} // info_master

namespace info_relation {
	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kRelatedKey[];	//!< �֘A�L�[
	extern const _TCHAR kMasterID[];	//!< �}�X�^�[ID
	extern const _TCHAR kConstSeq[];	//!< ����
	extern const _TCHAR kRelatedType[]; //!< �֘A�ڍ׃R�[�h

	//!< �֘A�ڍ׃R�[�h
	namespace related_type {
		enum ECode {
			kAdjust = 1,				//!< �ҏW��Ɛ���
			kSameReference = 2,			//!< ����₢���킹��
			kComplexInstitution = 3,	//!< �����{��
			kDuplication = 4,			//!< �d��
		};
	} // related_type

} // info_relation

namespace info_src {
	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kMasterID[];	//!< �}�X�^�[ID
	extern const _TCHAR kInfoName[];	//!< ��񌹖���
	extern const _TCHAR kInfoSrc[];		//!< ��񌹃R�[�h
	extern const _TCHAR kInfoURL[];		//!< ���URL
	extern const _TCHAR kRegistDate[];	//!< �o�^��

	//!< ��񌹃R�[�h
	namespace infosrc_code {
		enum ECode {
			kDefault = 0,                               //!< -
			kPublicInfomation = 100,                    //!< ���I���
			kOfficialGazette = 101,                     //!< �i���j����
			kOfficialReport = 102,                      //!< �i���j����
			kPublicRelations = 103,                     //!< �i���j�L��
			kOfficialDietProceedings = 104,             //!< �i���j�c��c���^
			kBudget = 105,                              //!< �i���j�\�Z
			kAnnual = 106,                              //!< �i���j�N��
			kCS = 200,                                  //!< CS
			kCSCommonPress = 201,                       //!< �iCS�j��ʎ�
			kCSBuildingPaper = 202,                     //!< �iCS�j���݌n�V��
			kCSCommercialFacilitiesPaler = 203,         //!< �iCS�j���Ǝ{�ݐV��
			kCSSpetializedMagazine = 204,               //!< �iCS�j��厏
			kCSUrbanDevelopment = 205,                  //!< �iCS�j�s�s�J��
			kLocalNews = 300,                           //!< �n����j���[�X
			kLCommonPress = 301,                        //!< �i�n�j��ʎ�
			kLSpetializedMagazine = 302,                //!< �i�n�j��厏
			kLPrefReport = 303,                         //!< �i�n�j����
			kLOther = 304,                              //!< �i�n�j���̑�
			kWireReport = 400,                          //!< �z�M�j���[�X
			kWRYahoo = 401,                             //!< �i�j�jYahoo!���f����
			kWRGoogle = 402,                            //!< �i�j�jGoogle�A���[�g
			kWebInformation = 500,                      //!< Web
			kWMLITHP = 501,                             //!< �i�v�j�Ȓ�HP
			kWPrefHP = 502,                             //!< �i�v�j�s���{��HP
			kWCityHP = 503,                             //!< �i�v�j�s�撬��HP
			kWCompanyHP = 504,                          //!< �i�v�j��ƒc��HP
			kWWebNews = 505,                            //!< �i�v�j�j���[�X�T�C�g
			kWOtherHP = 506,                            //!< �i�v�j���̑�HP
			kWebPatrol = 600,                           //!< Web����
			kWPWebNews = 601,                           //!< �i�v���j�j���[�X�T�C�g
			kWPJapanPost = 603,                         //!< �i�v���j�X������
			kWPPrefHP = 604,                            //!< �i�v���j�s���{��HP
			kWPCityHP = 605,                            //!< �i�v���j�s�撬��HP
			kWPCompanyHP = 606,                         //!< �i�v���j��ƒc��HP
			kWPCommonHP = 607,                          //!< �i�v���j���HP
            kWPOther = 608,                             //!< �i�v���j���̑�
			kHatena = 700,                              //!< �͂ĂȃA���e�i
			kHMLITHP = 701,                             //!< �i�́j�Ȓ�HP
			kHPrefHP = 702,                             //!< �i�́j�s���{��HP
			kHCityHP = 703,                             //!< �i�́j�s�撬��HP
			kHCompanyHP = 704,                          //!< �i�́j��ƒc��HP
			kHWebNews = 705,                            //!< �i�́j�j���[�X�T�C�g
			kHOtherHP = 706,                            //!< �i�́j���̑�HP
			kOutHouseInformation = 800,                 //!< �O���񋟏��
			kOShopList = 801,                           //!< �i�O�j�X�܃��X�g
			kODRMA = 802,                               //!< �i�O�jDRMA
			kOPasco = 803,                              //!< �i�O�j�p�X�R
			kOTellInterview = 804,                      //!< �i�O�j��ދL�^�i�d�b�j
			kOVisitInterview = 805,                     //!< �i�O�j��ދL�^�i�K��j
			kODispatchInfomation = 806,                 //!< �i�O�j���M���
			kOGeographicDataCenter = 807,               //!< �i�O�j���n��
			kOPostCode = 808,                           //!< �i�O�j�X�֔ԍ�
			kOKajo = 809,                               //!< �i�O�j����
			kContribution = 900,                        //!< ���e���
			kMIPSS = 901,                               //!< �i���jMIPSS
			kMoto = 902,                                //!< �i���j�n�}�̑f
			kInHouseInformation = 1000,                 //!< �Г����
			kClaim = 1001,                              //!< �i�Ёj�N���[��
			kFieldInvestigationPicture = 1002,          //!< �i�Ёj���s�摜
			kNewRoadFieldInvestigationPicture = 1003,   //!< �i�Ёj�V�K���H�����摜
			kOrtho = 1004,                              //!< �i�Ёj�I���\
			kFieldInvestigation = 1005,                 //!< �i�Ёj���n����
			kAddrRepresentivePointInvestigation = 1006, //!< �i�Ёj�Z����\�_����
			kSendingUpInformation = 1100,               //!< �Г����i�ł��グ�j
			kSUMap = 1101,                              //!< �i�БŁj�n�}�ł��グ
			kSURoad = 1102,                             //!< �i�БŁj���H�ł��グ
			kSUAddr = 1103,                             //!< �i�БŁj�Z���ł��グ
			kSUPOI = 1104,                              //!< �i�БŁjPOI�ł��グ
			kSUOther = 1105,                            //!< �i�БŁj���̑��ł��グ
		};
	} // info_src
} // info_src

namespace info_submaster {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kMasterID[];		//!< �}�X�^�[ID
	extern const _TCHAR kEditionNo[];		//!< �Ŕԍ�
	extern const _TCHAR kLock[];			//!< ���b�N�t���O
	extern const _TCHAR kValidDate[];		//!< ���p��
	extern const _TCHAR kValidDateCode[];	//!< ���p���R�[�h
	extern const _TCHAR kOwner[];			//!< �����ӔC��
	extern const _TCHAR kTimeLimit[];		//!< ��������
	extern const _TCHAR kComplete[];		//!< �������SF
	extern const _TCHAR kStatus[];			//!< �X�e�[�^�X
	extern const _TCHAR kRank[];			//!< �����N
	extern const _TCHAR kNoOperation[];		//!< ��ƕs�v���R
	extern const _TCHAR kResult[];			//!< ��������
	extern const _TCHAR kSurvey[];			//!< �v���n����
	extern const _TCHAR kPaper[];			//!< ������F
	extern const _TCHAR kShereder[];		//!< �v�p������

	//!< �}�X�^�[�X�e�[�^�X�R�[�h
	namespace master_status_type {
		enum ECode {
			kRegist		= 0,  //!< �o�^
			kInquiry	= 10, //!< ������
			kEarly		= 11, //!< ��������
			kConfirm	= 12, //!< �����m�F�҂�
			kComplete	= 20, //!< ����
			kReInquiry	= 30, //!< �Ē����˗�
			kDuplicate	= 50, //!< �d��
		};
	}

	//!< ���p���R�[�h
	namespace validate_type {
		enum ECode {
			kUnknown    = 0,	//!< ���m�F
			kBackward	= 1,	//!< �ߋ�
			kInterim	= 2,	//!< �b��
			kFixed	    = 3,	//!< �m��
		};
	}

	//!< �����N
	namespace rank_type {
		enum ECode {
			kAAA    = 11,	//!< AAA
			kAA	    = 12,	//!< AA
			kA		= 13,	//!< A
			kB	    = 20,	//!< B
			kC	    = 30,	//!< C
			kD	    = 40,	//!< D
		};
	}

	//!< ��ƕs�v���R
	namespace no_operation_type {
		enum ECode {
			kUnknown	= 0,			//!< -
			kReflected	= 1,			//!< ���f�ς�
			kCourtesyDenied	= 2,		//!< �����񋟋���
			kCourtesyNone	= 3,		//!< �����񋟂Ȃ�
			kContactNG	= 4,			//!< �₢���킹NG
			kExcludedFromWork	= 5,	//!< ��ƑΏۊO
			kNoSpaces	= 6,			//!< �X�y�[�X�Ȃ�
			kSourceUnknown	= 7,		//!< ��񌹕s��
			kOnlyRegistered	= 8,		//!< �o�^�̂�
		};
	}

	//!< �v���n����
	namespace survey_type {
		enum ECode {
			kUnknown		= 0,	//!< -
			kRequestPlan	= 1,	//!< �˗��\��
			kPreRequest		= 2,	//!< �˗��ς�
			kDone			= 3,	//!< �����ς�
			kNoSurvey		= 4,	//!< �����s��
		};
	}

	//!< �v�p������
	namespace shereder_type {
		enum ECode {
			kNoMaterial   = 0,	//!< �v�p�������Ȃ�
			kAvailable    = 1,  //!< �v�p����������
			kPreDisposal  = 2,	//!< �p���ς�
		};
	}

} // info_submaster

namespace feature_rel {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kInfoSubID[];		//!< �T�u�}�X�^�[ID
	extern const _TCHAR kGeometryID[];		//!< �W�I���g��ID
	extern const _TCHAR kGeometryType[];	//!< �W�I���g���^�C�v
	extern const _TCHAR kMeshCode[];		//!< ���b�V���R�[�h
	extern const _TCHAR kInquiryClass[];	//!< �������
	extern const _TCHAR kNameFixed[];		//!< �m��R�[�h�i���́j
	extern const _TCHAR kName[];			//!< ���̓�
	extern const _TCHAR kAddressFixed[];	//!< �m��R�[�h�i�Z���j
	extern const _TCHAR kGeometryFixed[];	//!< �m��R�[�h�i�`��E�ʒu�j
	extern const _TCHAR kAddress[];			//!< �Z��
	extern const _TCHAR kAddressCode[];		//!< 20���Z���R�[�h
	extern const _TCHAR kMaxFloor[];		//!< �K�����
	extern const _TCHAR kWideSite[];		//!< 10000�u�t���O
	extern const _TCHAR kRoadNo[];			//!< ���H�ԍ�
	extern const _TCHAR kOpen[];			//!< �J�ʓ��e
	extern const _TCHAR kDistance[];		//!< ����[km]
	extern const _TCHAR kWidth[];			//!< ����
	extern const _TCHAR kRefShape[];		//!< �Q�ƌ`��
	extern const _TCHAR kChangeAccuracy[];	//!< �ύX���x
	extern const _TCHAR kDataAccuracy[];	//!< �������x
	extern const _TCHAR kDataObtain[];		//!< ���񂹌�
	extern const _TCHAR kModifyLv[];		//!< ��ƃ��x��
	extern const _TCHAR kFollowup[];		//!< �ǐ�F
	extern const _TCHAR kPOINotUse[];		//!< POI���̗p�t���O
	extern const _TCHAR kPOISequences[];	//!< POI�V�[�P���X
	extern const _TCHAR kMainF[];			//!< ��\�t���O
	extern const _TCHAR kSiteArea[];		//!< �~�n�ʐ�
	extern const _TCHAR kSiteAreaCode[];	//!< �~�n�ʐσR�[�h
	extern const _TCHAR kBuildArea[];		//!< ���z�ʐ�
	extern const _TCHAR kBuildAreaCode[];	//!< ���z�ʐσR�[�h

	//!< �W�I���g���^�C�v
	namespace geometry_type {
		enum ECode {
			kUnknown = 0,	//!< �s��
			kPoint = 1,		//!< �|�C���g
			kLine = 3,		//!< ���C��
			kPolygon = 4,	//!< �|���S��
		};
	}

	//!< �m��R�[�h�i���́j
	namespace name_fixed_type {
		enum ECode {
			kUnknown = 0,	//!< -
			kInterim = 1,	//!< �b��
			kFixed = 2,		//!< �m��
			kDone = 4,		//!< ������
		};
	}

	//!< �m��R�[�h�i�`��E�ʒu�j
	namespace geometry_fixed_type {
		enum ECode {
			kUnknown = 0,			//!< -
			kFixedPosition = 1,		//!< �ʒu�m��
			kEndFixedPosition = 2,	//!< �ʒu�m��I��
			kEndFixedShape = 3,		//!< �`��m��I��
			kDone = 4,				//!< ������
		};
	}

	//!< �m��R�[�h�i�Z���j
	namespace address_fixed {
		enum ECode {
			kDefault = 0, //�����l
			kInquiry = 1, //������
			kFixed   = 2, //�m��
			kNoReflect = 3, // ���f�s��
		};
	}

	//!< �J�ʓ��e
	namespace open_type {
		enum ECode {
			kUnknown = 0,			//!< -
			kOpenRoad = 1,			//!< �J��
			kExtension = 2,			//!< ���L
			kWidening = 3,			//!< �g��
			kOpen = 4,				//!< �J��
			kChange = 5,			//!< �ύX
			kLinearImprovement = 6, //!< ���`����
			kAbolition	 = 7,		//!< �p�~
			kRegulation = 8,		//!< �K��
			kFreeOf = 9,			//!< ������
			kNameChange = 10,		//!< ���̕ύX
			kInduction = 11,		//!< �U��
			kCorrection = 12,		//!< �C��
		};
	}

	//!< �Q�ƌ`��
	namespace refshape_type {
		enum ECode {
			kUnknown = 0,				//!< �|
			kTopographicMap = 1,		//!< �n�`�}
			kDRMA = 2,					//!< DRMA
			kCityMap = 3,				//!< �s�s�n�}
			kTotalCapitalFigure = 4,	//!< �s�v�}
			kPreviousDrawing = 5,		//!< ��s�}��
			kConstructionDrawings = 6,	//!< �H���}��
			kDrawingNotice = 7,			//!< �����}��
			kLocationMap = 8,			//!< �ʒu�}
			kLog = 9,					//!< ���s���O
			kOrtho  = 10,				//!< �I���\�摜
			kOther = 11,				//!< ���̑�
			kUnnecessary = 12,			//!< �s�v
			kAddedEdition = 13,			//!< �ʔ�
		};
	}

	//!< �Z�����x
	namespace accuracy_type {
		enum ECode {
			kUnknown = 0,				//!< �|
			kGou = 1,					//!< ���E�n��
			kBlock = 2,					//!< �X��
			kCityChome = 3,				//!< ������
			kPlaceName = 4,				//!< �n��
			kRepresentativePoint = 5,	//!< ��\�_
		};
	}

	//!< ���񂹌�
	namespace obtain_type {
		enum ECode {
			kUnknown = 0,			//!< �|
			kMoriokaCoverage = 1,	//!< �����i��ށj
			kMoriokaOther = 2,		//!< �����i���̑��j
			kMeguro = 3,			//!< �ڍ�
			kKajo = 4,				//!< ����
		};
	}

	//!< �~�n�ʐσR�[�h
	namespace sitearea_type {
		enum ECode {
			kUnknown = 0,	//!< ���m�F
			kInterim = 1,	//!< �b��
			kFixed = 2,		//!< �m��
		};
	}

	//!< ���z�ʐσR�[�h
	namespace buildarea_type {
		enum ECode {
			kUnknown = 0,	//!< ���m�F
			kInterim = 1,	//!< �b��
			kFixed = 2,		//!< �m��
		};
	}

} // feature_rel

namespace work_target {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kInfoSubID[];		//!< �T�u�}�X�^�[ID
	extern const _TCHAR kEditType[];		//!< �ҏW�^�C�v
	extern const _TCHAR kWorkName[];		//!< ��Ɩ��R�[�h
	extern const _TCHAR kRegistrant[];		//!< �o�^��
	extern const _TCHAR kRegistDate[];		//!< �o�^��
	extern const _TCHAR kTimeLimit[];		//!< �Ή�����
	extern const _TCHAR kDept[];			//!< �Ή�����
	extern const _TCHAR kReflect[];			//!< ���f�m�F
	extern const _TCHAR kDifficultyLevel[]; //!< ��Ɠ�Փx

	//!< �ҏW�^�C�v
	namespace edit_type {
		enum ECode {
			kMap = 1,	//!< �n�}
			kRoad = 2,	//!< ���H
			kAddr = 3,	//!< �Z��
			kPOI = 4,	//!< POI
		};
	} // edit_type

	//!< �Ή�����
	namespace dept {
		enum ECode {
			kDefault = 0,				//!< -
			kMoto = 21,					//!< �n�}�̑f
			kNewRoad = 22,				//!< �V�K���H
			kAdjust = 23,				//!< �����ҏW
			kDRMA = 24,					//!< DRMA����
			kFieldInvestigation = 25,	//!< �V���X�V
			kComplain  = 26,			//!< �R���v���C���Ή�
			kError = 27,				//!< �G���[�C��
			kSurvey = 28,				//!< ���s����
			kSurveyNewRoad = 29,		//!< �V�K������
			kRepairAddr = 31,			//!< �Z������
		};
	} // dept

	//!< ���f�m�F
	namespace reflect {
		enum ECode {
			kCancel = 0,			//!< �L�����Z��
			kCanReflect = 1,		//!< ���f��
			kAlreadyReflected = 2,	//!< ���f��
			kCantReflect = 3,		//!< ���f�s��
		};
	} // reflect

	//!< ��Ɠ�Փx
	namespace difficulty_level {
		enum ECode {
			kLV0 = 0,
			kLV1 = 1,
			kLV2 = 2,
			kLV3 = 3,
			kLV4 = 4,
			kLV5 = 5,
		};
	} // difficulty_level
} // work_target

namespace work_progress {
	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kTargetID[];	//!< �^�[�Q�b�gID
	extern const _TCHAR kStatus[];		//!< �X�e�[�^�X
	extern const _TCHAR kWorker[];		//!< ��Ǝ�
	extern const _TCHAR kStartTime[];	//!< �J�n����
	extern const _TCHAR kEndTime[];		//!< �I������
	extern const _TCHAR kComment[];		//!< �R�����g

	//!< ��ƃX�e�[�^�X�R�[�h
	namespace work_status {
		enum ECode {
			kOpen      = 0, //!< �I�[�v��
			kDraft     = 1, //!< ���e
			kProof     = 2, //!< �Z��
			kEdit      = 3, //!< �ҏW
			kCheck     = 4, //!< ����
			kAdjust    = 5, //!< �����m�F
			kClose     = 6, //!< �N���[�Y
			kReInquiry = 7, //!< �Ē����m�F
		};
	};

} // work_progress

namespace work_add_item {
	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kProgressID[];	//!< �i��ID
	extern const _TCHAR kColID[];		//!< �ǉ����ڗ�
	extern const _TCHAR kColValue[];	//!< �ǉ����ڗ�l

	//!< SiNDY-i��ƕs�v�R�[�h
	namespace no_work {
		enum ECode {
			kWorked = 1, //!< ���f�ς�
			kNoWork = 2, //!< ���f�s��
		};
	} // no_work

	namespace adjust_status{
		enum ECode{
			kOK			= 1,	//!< OK
			kNGMap		= 11,	//!< �n�}NG
			kNGRoad		= 12,	//!< ���HNG
			kNGPOI		= 13,	//!< POING
			kNGMapRoad	= 21,	//!< �n�}�E���HNG
			kNGRoadPOI	= 22,	//!< ���H�EPOING
			kNGPOIMap	= 23,	//!< POI�E�n�}NG
		};
	}
} // work_add_item

namespace contact_master {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kIndustry_Name_C[]; //!< �Ǝ햼�R�[�h
	extern const _TCHAR kHead_Org[];		//!< ��\����
	extern const _TCHAR kHead_Yomi[];		//!< ��\���~
	extern const _TCHAR kMain_Num[];		//!< ��\�d�b�ԍ�
	extern const _TCHAR kPost_Code[];		//!< �X�֔ԍ�
	extern const _TCHAR kAddress[];			//!< �Z��
	extern const _TCHAR kURL[];				//!< URL
	extern const _TCHAR kBiz_Brand[];		//!< ���ƃu�����h
	extern const _TCHAR kRemarks[];			//!< ���l
	extern const _TCHAR kPerson[];			//!< �Г��S����
	extern const _TCHAR kClose_F[];			//!< ���t���O

	namespace industry_name_c {
		enum ECode {
			kUnknown = 0,			//!< -
			kPublicOffice = 1,		//!< ������
			kHospital = 2,			//!< �a�@
			kRailway = 3,			//!< �S��
			kAviation = 4,			//!< �q��
			kTransportation = 5,	//!< �^�A�E���s
			kAccommodation = 6,		//!< �h���{��
			kCommerce = 7,			//!< ����
			kService = 8,			//!< �T�[�r�X
			kEntertainment = 9,		//!< �G���^�[�e�C�������g
			kInformation = 10,		//!< ���E�ʐM
			kFinance = 11,			//!< ���Z
			kBuilding = 12,			//!< �r���E�Z��
			kManufacture = 13,		//!< ����
			kEnergy = 14,			//!< �G�l���M�[
			kEducation = 15,		//!< ����E�����@��
			kProduction = 16,		//!< �H�����Y
		};
	}

} // contact_master

namespace contact_submaster {
	extern const _TCHAR kTableName[];			//!< �e�[�u����

	extern const _TCHAR kHeadId[];				//!< ��\ID
	extern const _TCHAR kBranch_Org[];			//!< �����x�X����
	extern const _TCHAR kBranch_Yomi[];			//!< �����x�X���~
	extern const _TCHAR kContact_Person[];		//!< �⍇����S����
	extern const _TCHAR kContact_Person_Yomi[];	//!< �⍇����S���҃��~
	extern const _TCHAR kPost_Code[];			//!< �X�֔ԍ�
	extern const _TCHAR kAddress[];				//!< �Z��
	extern const _TCHAR kPhone[];				//!< ���ʓd�b�ԍ�
	extern const _TCHAR kExtension_Num[];		//!< ����
	extern const _TCHAR kFax[];					//!< FAX�ԍ�
	extern const _TCHAR kRemarks[];				//!< ���l
	extern const _TCHAR kPermit_C[];			//!< ���󋵃R�[�h
	extern const _TCHAR kRegistrant[];			//!< �o�^��
	extern const _TCHAR kRegist_Date[];			//!< �o�^��

	//!< ���󋵃R�[�h
	namespace permit_c {
		enum ECode {
			kConfirming = 0,				//!< �����ۊm�F��
			kInvestigable = 10,				//!< ������
			kHeadofficeInquiryContact = 11,	//!< �����i�S���ҌŒ�j
			kUncertainMaterialOffer = 12,	//!< �����i�����񋟂܂ŉj
			kHeadofficeNG_OutletOK = 13,	//!< �����i�{��NG/�e�X��OK�j
			kPoiLicensedCompany = 20,		//!< POI����
			kUnauthorized = 30,				//!< �����s��
		};
	}
} // contact_submaster

namespace inquiry_history {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kInfoSubId[];		//!< �T�u�}�X�^�[ID
	extern const _TCHAR kContactSId[];		//!< �₢���킹��ID
	extern const _TCHAR kUpdater[];			//!< �X�V��
	extern const _TCHAR kUpdate_Date[];		//!< �X�V��
	extern const _TCHAR kUpdate_Date_C[];	//!< �X�V���R�[�h
	extern const _TCHAR kNext_Date[];		//!< ���񒲍���
	extern const _TCHAR kComments[];		//!< �R�����g

	//!< �X�V���R�[�h
	namespace update_date_c {
		enum ECode {
			kUnknown = 1,					//!< �|
			kContact = 2,					//!< �⍇��
			kRequest = 3,					//!< �˗�
			kArrival = 4,					//!< ����
			kChangestatus = 10,				//!< �X�e�[�^�X�ύX
			kChangeowner = 11,				//!< �����ӔC�ҕύX
			kAftercompletingUpdate = 20,	//!< ������X�V
		};
	}

} // inquiry_history

namespace work_item_registry {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kEditType[];		//!< �ҏW�^�C�v
	extern const _TCHAR kStatus[];			//!< �X�e�[�^�X
	extern const _TCHAR kDispOrder[];		//!< �\����
	extern const _TCHAR kColUse[];			//!< ��p�r
	extern const _TCHAR kColName[];			//!< �񖼏�
	extern const _TCHAR kColType[];			//!< ��^�C�v
	extern const _TCHAR kColDomain[];		//!< �R�[�h�l�h���C����

	namespace edit_type {
		enum ECode {
			kMap = 1,	//!< �n�}
			kRoad = 2,	//!< ���H
			kAddr = 3,	//!< �Z��
			kPOI = 4,	//!< POI
		};
	}
	namespace work_status {
		enum ECode {
			kOpen      = 0, //!< �I�[�v��
			kDraft     = 1, //!< ���e
			kProof     = 2, //!< �Z��
			kEdit      = 3, //!< �ҏW
			kCheck     = 4, //!< ����
			kAdjust    = 5, //!< �����m�F
			kClose     = 6, //!< �N���[�Y
			kReInquiry = 7, //!< �Ē����m�F
		};
	}
	namespace col_use {
		enum ECode {
			kProgress = 1, //!< �i������
			kCheck	  = 2, //!< �������ځi�ڍׁj
			kNGRegist = 3, //!< NG�o�^�p
		};
	}

	namespace col_type {
		enum ECode {
			kFlag	= 1, //!< �t���O
			kCode	= 2, //!< �R�[�h�l
			kCount	= 3, //!< ���� 
		};
	}
} // work_item_registry


namespace work_target_master {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kWorkCode[]; 		//!< ��ƃR�[�h
	extern const _TCHAR kWorkName[]; 		//!< ��Ɩ�
	extern const _TCHAR kBaseDate[]; 		//!< ���
	extern const _TCHAR kRelease_Codes[]; 	//!< �����[�X�ԍ�
	extern const _TCHAR kDispF[]; 			//!< �\���t���O
	extern const _TCHAR kDataType_C[]; 		//!< �f�[�^�^�C�v

	//!< �f�[�^�^�C�v
	namespace data_type_c {
		enum ECode {
			kMainstream = 1,		//!< ���W���[
			kMonth = 2,				//!< ���x
			kOpen = 3,				//!< �J�ʓ��J��
		};
	}

} // work_target_master

namespace work_indication {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kInfoSubID[]; 		//!< �T�u�}�X�^�[ID
	extern const _TCHAR kFromDept[]; 		//!< ���M����
	extern const _TCHAR kToDept[]; 			//!< ��M����
	extern const _TCHAR kRelated[]; 		//!< �֘A���
	extern const _TCHAR kIndication[]; 		//!< ��Ǝw��
	extern const _TCHAR kRegistrant[]; 		//!< �o�^��
	extern const _TCHAR kRegistDate[]; 		//!< �o�^��

	//!< ���M����
	namespace from_dept {
		enum ECode {
			kMap		= 1,	//!< �n�}
			kRoad		= 2,	//!< ���H
			kAddr		= 3,	//!< �Z��
			kPOI		= 4,	//!< POI
			kInfoMap	= 10,	//!< ����(�n�})
			kInfoRoad	= 20,	//!< ����(���H)
			kInfoAddr	= 30,	//!< ����(�Z��)
			kInfoPOI	= 40,	//!< ����(POI)
		};
	}

	//!< ��M����
	namespace to_dept {
		enum ECode {
			kMap		= 1,	//!< �n�}
			kRoad		= 2,	//!< ���H
			kAddr		= 3,	//!< �Z��
			kPOI		= 4,	//!< POI
			kInfoMap	= 10,	//!< ����(�n�})
			kInfoRoad	= 20,	//!< ����(���H)
			kInfoAddr	= 30,	//!< ����(�Z��)
			kInfoPOI	= 40,	//!< ����(POI)
		};
	}

	//!< �֘A���
	namespace related_type {
		enum ECode {
			kUnknown			  = 0,	//!< -
			kCheckRequest		  = 1,	//!< �m�F�˗�
			kConfirmed_NoSupport  = 2,	//!< �m�F�ρi�Ή��s�v�j
			kConfirmed_Support    = 3,	//!< �m�F�ρi�v�Ή��j
		};
	}

} // work_indication

namespace info_tag {
	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kInfoSubID[]; 		//!< �T�u�}�X�^�[ID
	extern const _TCHAR kTagName[]; 		//!< �^�O

	//!< �^�O
	namespace tag_type {
		enum ECode {
			kClaim      = 1,	//!< �N���[��
			kWatch      = 2,	//!< WATCH
			kY          = 11,	//!< Y!
			kHwy		= 21,	//!< HWY
			kOpen		= 22,	//!< �J�J
			kNewRoad	= 23,	//!< �V�K��
		};
	}

} // info_tag

} // sindyi

} // schema

} // sindy
