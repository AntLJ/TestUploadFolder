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
#include <sindy/schema/category.h>

namespace sindy {

namespace schema {

namespace sindyk {

/// �f�[�^���ʃR�[�h
namespace idncode {
	enum ECode {
#define IDN_CODE(code,name,id) name = id,
#include "idncode.h"
#undef IDN_CODE
	};
} // idncode

namespace master_history {
	extern const _TCHAR kAddMasterCode[]; //!< �ǉ��}�X�^�R�[�h
	extern const _TCHAR kDelMasterCode[]; //!< �p�~�}�X�^�R�[�h 
}

/// �f�[�^���ރ}�X�^
namespace group_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kMajorGroupCode[]; //!< �f�[�^�啪�ރR�[�h
	using namespace master_history;
}

/// �X�܃}�X�^
namespace chain_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kChainCode[]; //!< �X�܃R�[�h
	extern const _TCHAR kChainCode8[]; //!< �X�܃R�[�h�i8���j
	extern const _TCHAR kChainGrpCode[]; //!< �X�ܕ��ރR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kYomi[]; //!< �ǂ�
	extern const _TCHAR kTwnSet[]; //!< �^�E���y�[�W�ݒ�Ώۃt���O
	extern const _TCHAR kGnrSet[]; //!< ���POI�ݒ�Ώۃt���O
	using namespace master_history;
}

/// �R���e���c�}�X�^
namespace contents_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
	extern const _TCHAR kChainCode[]; //!< �X�܃R�[�h
	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kContentsType[]; //!< �R���e���c�^�C�v
	extern const _TCHAR kCATGroupCode[]; //!< �啪�ރO���[�v�R�[�h
	extern const _TCHAR kMNTGroupCode[]; //!< ���������e�O���[�v�R�[�h
	extern const _TCHAR kName[]; //!< �ǂ�
	extern const _TCHAR kInfoFilePath[]; //!< ���t�@�C���i�[��
	extern const _TCHAR kInfoFileSeq[]; //!< ���t�@�C���V�[�P���X
	using namespace master_history;

	/// �R���e���c�^�C�v�R�[�h
	namespace contents_type_code {
		enum ECode {
			kPOI = 0, //!< POI��p
			kCommon = 1, //!< ���LPOI���p
			kAnno = 2, //!< ���L��p
		};
	}
}

/// �̎Ѓ}�X�^
namespace sls_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kSLSCode[]; //!< �̎ЃR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kYomi[]; //!< �ǂ�
	using namespace master_history;
}

/// �R���e���c�@�\
namespace contents_function {
	extern const _TCHAR kTableName[];		 //!< �e�[�u����

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kIDNCode[]; //!< �@�\�R�[�h
	extern const _TCHAR kLock[]; //!< ���b�N�t���O
	using namespace master_history;

	/// �X�V�R�[�h
	namespace update_code {
		enum ECode {
			kDelete = 1, //!< �폜
			kAdd = 2, //!< �ǉ�
		};
	}
}

/// �R���e���c�Љ
namespace contents_introduction {
	extern const _TCHAR kTableName[]; // !< �e�[�u����

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kFeatureItemSeq[]; //!< �Љ���ڃV�[�P���X
	extern const _TCHAR kFeatureMarkCode[]; //!< �Љ���o���R�[�h
	extern const _TCHAR kFeatureMarkName[]; //!< �Љ���o������
	using namespace master_history;
}

/// �R���e���c�ڍ׏��
namespace contents_detailinfo {
	extern const _TCHAR kTableName[]; // !< �e�[�u����

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kSortOrder[]; //!< ���я�
	extern const _TCHAR kFeatureCode[]; //!< �ڍ׏�񍀖ڃR�[�h
	extern const _TCHAR kExist[]; //!< ����Ȃ��t���O
	using namespace master_history;

	/// �ڍ׏�񍀖ڃR�[�h
	namespace dinfo_code {
		enum ECode { // TODO: �}�X�^�X�V�ŃC���|�[�g�������̂Ȃ̂ŁA�Ƃ肠�����K�v�Ȃ��̂�����`
			kOpenHours = 1, //!< �c�Ǝ���
			kHolidays = 2, //!< �x�Ɠ�
			kRapid = 4, //!< �}��
			k100v = 5, //!< 100V
			k200v = 6 //!< 200V
		};
	}
}

/// ���W�l��ʃ}�X�^
namespace xy_assortment_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kPSTAttCode[]; //!< ���W�l��ʃR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kPSAPriority[]; //!< ���W�l��ʗD�揇��
	extern const _TCHAR kMNTPriority[]; //!< �����D�揇��
}

/// ���W�l��ʕϊ��}�X�^
namespace xy_assortment_convert {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kPSTAttCode[]; //!< ���W�l��ʃR�[�h
	extern const _TCHAR kOrgPSTAttCode[]; //!< ���W�l��ʃR�[�h�i�h�����j
	extern const _TCHAR kTollRoad[]; //!< �������H�t���O
}

/// POI���ʎq�R�t����ʃ}�X�^
namespace poiid_connect_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
  
	extern _TCHAR kPCACode[]; //!< POI���ʎq�R�t��ʃR�[�h
	extern _TCHAR kName[]; //!< ����
	extern _TCHAR kBankContentsCode[]; //!< �R���e���c�R�[�h�i��s�j
	extern _TCHAR kATMContentsCode[]; //!< �R���e���c�R�[�h�iATM�j
}

/// ���񋟌��}�X�^
namespace infoprovide_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kIPCode[]; //!< ���񋟌��R�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kMNTPriority[]; //!< �����D�揇��
}

/// ���x�}�X�^
namespace accuracy_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kACCCode[]; //!< ���x�R�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kMNTPriority[]; //!< �����D�揇��
}

/// �Q�ƃo�[�W�����}�X�^
namespace refversion_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kMasterCode[]; //!< �}�X�^�R�[�h
	extern const _TCHAR kDataLot[]; //!< �f�[�^���b�g
	extern const _TCHAR kWorkName[]; //!< ��Ɩ�
	extern const _TCHAR kAddr[]; //!< �Z�������[�X���[�U
	extern const _TCHAR kAdmin[]; //!< SiNDYDB�ڑ����
	extern const _TCHAR kVersionCode[]; //!< �}�X�^�o�[�W�����R�[�h
}

/// �@�\�}�X�^
namespace function_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kIDNCode[]; //!< �f�[�^���ʃR�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kSiRiUSTableName[]; //!< SiRiUS�e�[�u����
	extern const _TCHAR kTableCode[]; //!< �i�[�e�[�u���R�[�h
}

/// �@�\�ڍ�
namespace function_detail {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kIDNCode[]; //!< �f�[�^���ʃR�[�h
	extern const _TCHAR kSortOrder[]; //!< �\�[�g��
	extern const _TCHAR kColumnName[]; //!< �t�B�[���h��
	extern const _TCHAR kSiRiUSColumnName[]; //!< SiRiUS�J������
}

/// POI�|�C���g
namespace _poi_point {
	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kContentsSeq[]; //!< �R���e���c���V�[�P���X
	extern const _TCHAR kWorker[]; //!< �ҏW��
	extern const _TCHAR kWorkDate[]; //!< �ҏW������
	extern const _TCHAR kVerifier[]; //!< ���؎�
	extern const _TCHAR kVerifyDate[]; //!< ���؊�����
	extern const _TCHAR kUPDAvailDate[]; //!< �X�V���L�����t
	extern const _TCHAR kDelete[]; //!< �폜�R�[�h
	extern const _TCHAR kErrorFixed[]; //!< �G���[�C���t���O
	extern const _TCHAR kPSTIPCode[]; //!< ���W�l�񋟌��R�[�h�i���񋟌��R�[�h�j
	extern const _TCHAR kACCCode[]; //!< ���x�R�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kYomi[]; //!< �i��؂蕶���t���j�ǂ�
	extern const _TCHAR kCloseCode[]; //!< �����R�R�[�h
	extern const _TCHAR kCloseMemo[]; //!< �����R����
	extern const _TCHAR kMNTMemoAdmin[]; //!< ���������i�Ǘ��ҁj
	extern const _TCHAR kMNTMemoWork[]; //!< ���������i��Ǝҁj
	extern const _TCHAR kBGInfo1[]; //!< �Q�l�����P
	extern const _TCHAR kBGInfo2[]; //!< �Q�l�����Q
	extern const _TCHAR kURL[]; //!< URL
	extern const _TCHAR kIPUseID[]; //!< ���񋟌����pID
	extern const _TCHAR kFeature[]; //!< [�Љ]�Љ
	extern const _TCHAR kDetailInfo[]; //!< [�ڍ׏��]�ڍ׏��
	extern const _TCHAR kTel[]; //!< [�d�b�ԍ�]�d�b�ԍ�
	extern const _TCHAR kPrefCode[]; //!< [�\���p�s�撬��] �s���{���R�[�h
	extern const _TCHAR kCityCode[]; //!< [�\���p�s�撬��] �s�撬���R�[�h
	extern const _TCHAR kAddress[]; //!< [�Z��]�����ڈȉ��Z��
	extern const _TCHAR kLiquor[]; //!< [���戵��]���戵���t���O
	extern const _TCHAR kCigar[]; //!< [���΂��戵��]���΂��戵���t���O
	extern const _TCHAR kCRSLM[]; //!< [�����h�}�[�N]�����_�g�厞�����h�}�[�N�t���O
	extern const _TCHAR kHeightRST[]; //!< [���ԏꐧ��]��������
	extern const _TCHAR kWidthRST[]; //!< [���ԏꐧ��]������
	extern const _TCHAR kLengthRST[]; //!< [���ԏꐧ��]��������
	extern const _TCHAR kWeightRST[]; //!< [���ԏꐧ��]�d�ʐ���
	extern const _TCHAR kParkingRSTBit[]; //!< [���ԏꐧ��]���ԏꐧ���r�b�g�i3���ځj
	extern const _TCHAR kInfoFileID[]; //!< [�f���t�@�C��]���t�@�C��ID
	extern const _TCHAR kSLS[]; //!< [�̎�]�̎ЃR�[�h
	extern const _TCHAR kCSID[]; //!< AnyPlace ID
	extern const _TCHAR kInfoID[]; //!< SiNDY-i ID
	extern const _TCHAR kLMID[]; //!< �����h�}�[�NID
	extern const _TCHAR kTollRoad[]; //!< [�L�����H]�L�����H�t���O
	extern const _TCHAR kSerialID[]; //!< �V���A��ID
	extern const _TCHAR kPartialNameD[]; //!< [������v]������v���́i��؂蕶���t���j
	extern const _TCHAR kPartialYomiD[]; //!< [������v]������v��݁i��؂蕶���t���j
	extern const _TCHAR kTempleSeq[]; //! [�D��]�D���ԍ�
	extern const _TCHAR kEmergencyShelterC[]; //!< [�ً}��]�ً}���R�[�h

	// ���L�p
#ifdef _SINDYK_SUPPORT_COMMON_RELATION // �e�q�֌W�̃T�|�[�g
	extern const _TCHAR kAnnoGUID[]; //!< ���L�\���pGUID
#endif // ifdef _SINDYK_SUPPORT_COMMON_RELATION
	extern const _TCHAR kAnnoName1[]; //!< ���L�\���p������P
	extern const _TCHAR kAnnoName2[]; //!< ���L�\���p������Q
	extern const _TCHAR kAnnoHyoukou[]; //!< ���L�\���p�W���l
	extern const _TCHAR kAnnoMntMemo[]; //!< ���L��������
	extern const _TCHAR kAnnoDispCode[]; //!< ���L�\���R�[�h
	extern const _TCHAR kAnnoDispLevel[]; //!< ���L�\�����x��
	extern const _TCHAR kAnnoDispNL[]; //!< ���L���s�ʒu
	extern const _TCHAR kAnnoDispYomi[]; //!< ���L�ǂ�
	extern const _TCHAR kEditStatusCode[]; //! �ҏW��ԃR�[�h

	/// �폜�R�[�h
	namespace delete_code {
		enum ECode {
			kAlive = 0, //!< ����
			kDead = 1, //!< �폜
			kReserve = 2, //!< �폜�ۗ�
		};
	} // deletecode

	/// SiNDY-k�@�\�i�[��e�[�u���R�[�h
	namespace sindyktable_code {
		enum ECode {
			kPoi_point =		1, //!< POI_POINT
			kPoi_sub_point =	2, //!< POI_SUB_POINT
			kPoi_sub_attr =		3, //!< POI_SUB_ATTR
			kPoi_reration =		4, //!< POI_RERATION
		};
	} // sindyktable_code
	
	/// ���L�\���R�[�h
	namespace annodisp_code {
		enum ECode {
			kNone = 0, //!< ��\��
			kNomal = 1, //!< �W��
			kForce = 2, //!< �����\��
			kUnconfirm = 9 //!< ���m�F
		};
	} // deletecode

	/// �ҏW�X�e�[�^�X�R�[�h
	namespace editstatus_code {
		enum ECode {
			kNone           = 0, //!< ���ҏW
			kWorking        = 1, //!< �ҏW��
			kWorkedNoverify = 2, //!< �ҏW�ς݁i�����؁j
			kVerified      = 3  //!< ���؍ς�
		};
	} // editstatus_code
}

namespace poi_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_point;
}

namespace poi_point_org {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_point;
}

/// POI�T�u�|�C���g
namespace _poi_sub_point {
	extern const _TCHAR kPOIPointID[]; //!< POI�|�C���gID
	extern const _TCHAR kIDNCode[]; //!< �f�[�^���ʃR�[�h
	extern const _TCHAR kFuncSeq[]; //!< �@�\���V�[�P���X
	extern const _TCHAR kDelete[]; //!< �폜�R�[�h
	extern const _TCHAR kPSTAttCode[]; //!< [���ԏ�/���ԏ����/���B]���W�l��ʃR�[�h
	extern const _TCHAR kPriority[]; //!< [���ԏ�/���ԏ����/���B]���W�l��ʓ��D�揇��
	extern const _TCHAR kPriorityAttCode[]; //!< [���ԏ�/���ԏ����/���B]���W�l��ʓ��D�揇�ʎ�ʃR�[�h
	extern const _TCHAR kMNTInfo[]; //!< [���ԏ���/���B���]�Љ
	extern const _TCHAR kTollRoad[]; //!< [�L�����H]�L�����H�t���O

	/// �폜�R�[�h
	namespace delete_code {
		enum ECode {
			kAlive = 0, //!< ����
			kDead = 1, //!< �폜
			kReserve = 2, //!< �폜�ۗ�
		};
	} // deletecode
}

namespace poi_sub_point {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_sub_point;
}

namespace poi_sub_point_org {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_sub_point;
}

/// POI�T�u����
namespace _poi_sub_attr {
	extern const _TCHAR kPOIPointID[]; //!< POI�|�C���gID
	extern const _TCHAR kIDNCode[]; //!< �f�[�^���ʃR�[�h
	extern const _TCHAR kStartDate[]; //!< [�����c�Ǝ���]�J�n��
	extern const _TCHAR kEndDate[]; //!< [�����c�Ǝ���]�I����
	extern const _TCHAR kOpenTime[]; //!< [�����c�Ǝ���]�c�ƊJ�n����
	extern const _TCHAR kCloseTime[]; //!< [�����c�Ǝ���]�c�ƏI������
	extern const _TCHAR kDayCode[]; //!< [�����c�Ǝ���]�j���R�[�h
	extern const _TCHAR kMultiChainCode[]; //!< [�����X��]�����X�܃R�[�h
}

namespace poi_sub_attr {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_sub_attr;
}

namespace poi_sub_attr_org {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_sub_attr;
}

/// POI�֘A
namespace _poi_relation {
	extern const _TCHAR kRelatedKey[]; //!< �֘A�L�[
	extern const _TCHAR kPOIPointID[]; //!< POI�|�C���gID
	extern const _TCHAR kFuncSeq[]; //!< �@�\���V�[�P���X
	extern const _TCHAR kTmpParkingID[]; //!< [�f�p�[�g��g���ԏ�] ��g���ԏ�E���ԏ�ID
	extern const _TCHAR kPOICNCATT[]; //!< [POI���ʎq�R�t] POI���ʎq�R�t��ʃR�[�h
	extern const _TCHAR kParkingID[]; //!< [��g�Ǝ����ԏ�] �Ǝ����ԏ�ID
}

namespace poi_relation {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_relation;
}

namespace poi_relation_org {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	using namespace _poi_relation;
}

/// �X�ܕ�����ƍ�����
namespace dicchn {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kInitDate[]; //!< ����o�^��
	extern const _TCHAR kDeleteDate[]; //!< �폜��
	extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
	extern const _TCHAR kChainCode[]; //!< �f�[�^�X�܃R�[�h
	extern const _TCHAR kChkDataSeq[]; //!< �`�F�b�N�f�[�^�V�[�P���X
	extern const _TCHAR kFwdName[]; //!< ���́i�O����v�j
	extern const _TCHAR kMidName[]; //!< ���́i���Ԉ�v�j
	extern const _TCHAR kBwdName[]; //!< ���́i�����v�j
	extern const _TCHAR kFwdYomi[]; //!< �ǂ݁i�O����v�j
	extern const _TCHAR kMidYomi[]; //!< �ǂ݁i���Ԉ�v�j
	extern const _TCHAR kBwdYomi[]; //!< �ǂ݁i�����v�j
} // dicchn

/// �V���A��ID�쐬�e�[�u��
namespace create_serial_id {
	extern const _TCHAR kTableName[];	//!< �e�[�u����
}

/// id_table��id_addr_table�̋��ʕ���
namespace id_table_base {
	using namespace category::contents_seq_table;
	using namespace category::group_code_table;
	using namespace category::chain_code_table;

	extern const _TCHAR kAddIDType[];	//!< ID�t�^���

	/// ID�t�^��ʃR�[�h
	namespace add_id_type
	{
		enum ECode
		{
			kCross						= 1,	//!< �����_
			kRoadType					= 2,	//!< ���H����
			kRoadLine					= 3,	//!< ���H�H��
			kRoadFacilRepPoint			= 4,	//!< ���H�{�ݑ�\�_
			kRoadFacilPoint				= 5,	//!< ���H�{�݃|�C���g
			kRoadFacilBranchPoint		= 6,	//!< ���H�{�ݕ���|�C���g
			kRailwayType				= 7,	//!< �S������
			kRailwayCompany				= 8,	//!< �S�����
			kRailwayLine				= 9,	//!< �S���H��
			kRailwayStation				= 10,	//!< �S���w
			kRailwayInOut				= 11,	//!< �S���w�o����
			kGeneralPOI					= 12,	//!< ���POI
			kTownPage					= 14,	//!< �^�E���y�[�W         // ssmaster.dll�Ŏg�p����̂Ŕԍ��ύX���Ȃ����ƁI�I
			kBellemax					= 15,	//!< Bellemax             // ssmaster.dll�Ŏg�p����̂Ŕԍ��ύX���Ȃ����ƁI�I
			kPhoneCodePointOut			= 16,	//!< �ǔԑ�\�_�i�s�O�j   // ssmaster.dll�Ŏg�p����̂Ŕԍ��ύX���Ȃ����ƁI�I
			kPhoneCodePointCenter		= 17,	//!< �ǔԑ�\�_�i�s���j   // ssmaster.dll�Ŏg�p����̂Ŕԍ��ύX���Ȃ����ƁI�I
			kZipCode					= 18,	//!< �X�֔ԍ�
			kPrefecture					= 19,	//!< �s���{��
			kMunicipality				= 20,	//!< �s����
			kTown						= 21,	//!< ������	// TODO: kTownPage�ƕ���킵���̂ŕς������c
			kChiban						= 22,	//!< �n��
			kChibanWithCharacter		= 23,	//!< �����t�n��
			kGou						= 24,	//!< ��
			kGouWithCharacter			= 25,	//!< �����t��
		};
	}

	/**k
	 * @brief �f�[�^���ރR�[�h
	 * @note	�R���e���c�R�[�h�̍\���v�f�̈ꕔ�B�u�R���e���c�R�[�h = �f�[�^���ރR�[�h * 100000 + �X�܃R�[�h�v�ł���B
	 *			�R�[�h�l�h���C���ł͂Ȃ����A����ɗނ�����̂Ȃ̂Œ�`���Ă����B
	 * @note	���ނ�ID�t�^��ʂƂقړ��������A���L�̓_���قȂ�B
	 *			�E�u�X�֔ԍ��v���X�ɍו�������Ă���B
	 *			�E���POI�͉ςȂ̂Œ�`�Ȃ��B
	 *			�E�^�E���y�[�W�͎��ۂ̉^�p�ł͉ς����ADTMSEQ�ł̊Ǘ��̂��߁A��\�ƂȂ�l�����߂Ă���B
	 */
	namespace group_code
	{
		enum ECode
		{
			kCross						= 22,	//!< �����_
			kRoadType					= 12,	//!< ���H����
			kRoadLine					= 13,	//!< ���H�H��
			kRoadFacilRepPoint			= 14,	//!< ���H�{�ݑ�\�_
			kRoadFacilPoint				= 15,	//!< ���H�{�݃|�C���g
			kRoadFacilBranchPoint		= 16,	//!< ���H�{�ݕ���|�C���g
			kRailwayType				= 17,	//!< �S������
			kRailwayCompany				= 18,	//!< �S�����
			kRailwayLine				= 19,	//!< �S���H��
			kRailwayStation				= 20,	//!< �S���w
			kRailwayInOut				= 21,	//!< �S���w�o����
			kTownPage					= 2885,	//!< �^�E���y�[�W
			kBellemax					= 1,	//!< Bellemax
			kPhoneCodePointOut			= 23,	//!< �ǔԑ�\�_�i�s�O�j
			kPhoneCodePointCenter		= 24,	//!< �ǔԑ�\�_�i�s���j
			kPostCode_Address			= 8,	//!< �X�֔ԍ��i�Z���j
			kPostCode_Building			= 9,	//!< �X�֔ԍ��i�r���j
			kPostCode_Office			= 10,	//!< �X�֔ԍ��i������Ə��j
			kPostCode_PostOfficeBox		= 11,	//!< �X�֔ԍ��i�������j
			kPrefecture					= 2,	//!< �s���{��
			kMunicipality				= 3,	//!< �s����
			kTown						= 4,	//!< ������	// TODO: kTownPage�ƕ���킵���̂ŕς������c
			kChiban						= 5,	//!< �n��
			kChibanWithCharacter		= 2453,	//!< �����t�n��
			kGou						= 6,	//!< ��
			kGouWithCharacter			= 2454,	//!< �����t��
		};
	}

} // namespace id_table_base

/// �V���A��ID�Ή��e�[�u��
namespace id_table {
	using namespace id_table_base;
	using namespace category::serial_id_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kMeshCode[];		//!< �Q�����b�V���R�[�h
	extern const _TCHAR kX[];				//!< �Q�����b�V�������K��X���W
	extern const _TCHAR kY[];				//!< �Q�����b�V�������K��Y���W
	extern const _TCHAR kCrossName[];		//!< �����_����
	extern const _TCHAR kRoadTypeCode[];	//!< �Ǘ��c�̃R�[�h
	extern const _TCHAR kRoadTypeCodeSeq[];	//!< �H���V�[�P���X�ԍ�
	extern const _TCHAR kRoadSeq[];			//!< �H�����V�[�P���X�ԍ�
	extern const _TCHAR kFacilName[];		//!< ���H�{�ݖ���
	extern const _TCHAR kFacilAttrCode[];	//!< ���H�{�ݎ�ʃR�[�h
	extern const _TCHAR kDirectionCode[];	//!< ���ʃR�[�h
	extern const _TCHAR kDirectionName[];	//!< ���ʖ���
	extern const _TCHAR kTelCode[];			//!< �ǔ�
}

/// �Z���R���e���c���V�[�P���X�Ή��e�[�u��
namespace id_addr_table
{
	using namespace id_table_base;

	extern const _TCHAR kTableName[];	//!< �e�[�u����

	extern const _TCHAR kZipCode[];		//!< �X�֔ԍ�
	extern const _TCHAR kName[];		//!< ����
	extern const _TCHAR kPrefCode[];	//!< �s���{���R�[�h
	extern const _TCHAR kCityCode[];	//!< �s�撬���R�[�h
	extern const _TCHAR kAdrCode1[];	//!< �厚�E�ʏ̃R�[�h
	extern const _TCHAR kAdrCode2[];	//!< ���E���ڃR�[�h
	extern const _TCHAR kChibanCode[];	//!< �n�ԃR�[�h
	extern const _TCHAR kJukyoCode[];	//!< �Z���ԍ��R�[�h
}

/// �R���e���c���V�[�P���X�Ǘ��e�[�u��
namespace dtm_seq {
	using namespace category::contents_code_table;

	extern const _TCHAR kTableName[];		//!< �e�[�u����

	extern const _TCHAR kContentsSeq[];		//!< �R���e���c���V�[�P���X�̍ő�l
	extern const _TCHAR kLockFlag[];		//!< ���b�N�t���O
}

/// �R���e���c���ʎq�ϊ��e�[�u��
namespace contents_convert {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kContentsCode[]; //!< �R���e���c�R�[�h
	extern const _TCHAR kIPCode[]; //!< IP�R�[�h
	extern const _TCHAR kDataKubunCode[]; //!< �f�[�^�敪�R�[�h
	extern const _TCHAR kAttCode[]; //!< ��ʃR�[�h
	extern const _TCHAR kChainCode8D[]; //!< �X�܃R�[�h(8��)
	using namespace master_history;
}

/// NTT�Ǝ�}�X�^
namespace ntt_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kNTTCode[]; //!< NTT�Ǝ�R�[�h
	extern const _TCHAR kName[]; //!< ����
	extern const _TCHAR kNTTGroupCode[]; //!< NTT��{���ރR�[�h
	extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
	using namespace master_history;
}

/// �f�[�^�擾�敪�}�X�^
namespace data_kubun_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kDataKubunCode[]; //!< �f�[�^�擾�敪�R�[�h
	extern const _TCHAR kName[]; //!< ����
	using namespace master_history;
}

/// ��ʃ}�X�^
namespace assortment_master {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kATTCode[]; //!< ��ʃR�[�h
	extern const _TCHAR kName[]; //!< ����
	using namespace master_history;
}

/// �ǔԕϊ��e�[�u��
namespace tel_convert {
	extern const _TCHAR kTableName[]; //!< �e�[�u����

	extern const _TCHAR kTelInfOld[]; //!< �ԍ��̈�
	extern const _TCHAR kTelCodeOld[]; //!< �ύX�O�s�O�ǔ�
	extern const _TCHAR kTelCode[]; //!< �ύX��s�O�ǔ�
	extern const _TCHAR kTelAdd[]; //!< �ύX��t�����
	extern const _TCHAR kDigitOld[]; //!< �ύX�O�ԍ�����
	extern const _TCHAR kDigit[]; //!< �ύX��ԍ�����
	extern const _TCHAR kCngTelF[]; //!< �ǔԕύX�Ώۃt���O
	extern const _TCHAR kTgtArea[]; //!< �Ώےn��
	extern const _TCHAR kEnforcedDate[]; //!< ���{���t
}

/// �j���R�[�h�ϊ��e�[�u��
namespace day_convert{
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	
	extern const _TCHAR kDayCode[]; //!< �j���R�[�h
	extern const _TCHAR kDayCodeBit[]; //!< �j���R�[�h�i�r�b�g�j
	extern const _TCHAR kTermAttCode[]; //!< ���Ԏ�ʃR�[�h
}

/// �S�����ޕϊ��e�[�u��
namespace traintype_convert {
	extern const _TCHAR kTableName[]; //!< �e�[�u����
	
	extern const _TCHAR kGroupCode[]; //!< �f�[�^���ރR�[�h
	extern const _TCHAR kContentsSeq[]; //!< �R���e���c���V�[�P���X
	extern const _TCHAR kTrainTypeCode[]; //!< �S�����ރR�[�h
	extern const _TCHAR kTrainCorpCode[]; //!< �S����ЃR�[�h
	extern const _TCHAR kTrainTypeCodeOld[]; //!< �S�����ރR�[�h�i���j
	extern const _TCHAR kTrcDplF[]; //!< �S�����ޏd���t���O
}

} // sindyk

} // schema

} // sindy
