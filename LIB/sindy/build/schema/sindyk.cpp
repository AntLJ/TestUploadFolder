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

#include "stdafx.h"
#include <sindy/schema/sindyk.h>

namespace sindy {

namespace schema {

namespace sindyk {

namespace master_history {
	const _TCHAR kAddMasterCode[] = _T("ADD_MASTER_CODE"); //!< �ǉ��}�X�^�R�[�h
	const _TCHAR kDelMasterCode[] = _T("DEL_MASTER_CODE"); //!< �p�~�}�X�^�R�[�h 
}

// �f�[�^���ރ}�X�^
namespace group_master {
	const _TCHAR kTableName[] = _T("GROUP_MASTER"); //!< �e�[�u����

	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< �f�[�^���ރR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kMajorGroupCode[] = _T("MJRGRPCODE"); //!< �f�[�^�啪�ރR�[�h
}

// �X�܃}�X�^
namespace chain_master {
	const _TCHAR kTableName[] = _T("CHAIN_MASTER"); //!< �e�[�u����

	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< �X�܃R�[�h
	const _TCHAR kChainCode8[] = _T("CHAINCODE_8D"); //!< �X�܃R�[�h�i8���j
	const _TCHAR kChainGrpCode[] = _T("CHN_GRPCODE"); //!< �X�ܕ��ރR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kYomi[] = _T("YOMI"); //!< �ǂ�
	const _TCHAR kTwnSet[] = _T("TWN_SET_F"); //!< �^�E���y�[�W�ݒ�Ώۃt���O
	const _TCHAR kGnrSet[] = _T("GNR_SET_F"); //!< ���POI�ݒ�Ώۃt���O
}

// �R���e���c�}�X�^
namespace contents_master {
	const _TCHAR kTableName[] = _T("CONTENTS_MASTER"); //!< �e�[�u����

	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< �f�[�^���ރR�[�h
	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< �X�܃R�[�h
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kContentsType[] = _T("CONTENTS_TYPE"); //!< �R���e���c�^�C�v
	const _TCHAR kCATGroupCode[] = _T("CAT_GROUP_CODE"); //!< �啪�ރO���[�v�R�[�h
	const _TCHAR kMNTGroupCode[] = _T("MNT_GROUP_CODE"); //!< ���������e�O���[�v�R�[�h
	const _TCHAR kName[] = _T("NAME"); //!< �ǂ�
	const _TCHAR kInfoFilePath[] = _T("INFOFILE_PATH"); //!< ���t�@�C���i�[��
	const _TCHAR kInfoFileSeq[] = _T("INFOFILE_SEQ"); //!< ���t�@�C���V�[�P���X
}

// �̎Ѓ}�X�^
namespace sls_master {
	const _TCHAR kTableName[] = _T("SALES_COMPANY_MASTER"); //!< �e�[�u����

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kSLSCode[] = _T("SLSCODE"); //!< �̎ЃR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kYomi[] = _T("YOMI"); //!< �ǂ�
}

// �R���e���c�@�\
namespace contents_function {
	const _TCHAR kTableName[] = _T("CONTENTS_FUNCTION");		 //!< �e�[�u����

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< �@�\�R�[�h
	const _TCHAR kLock[] = _T("LOCK_F"); //!< ���b�N�t���O
}

// �R���e���c�Љ
namespace contents_introduction {
	const _TCHAR kTableName[] = _T("CONTENTS_INTRODUCTION"); // !< �e�[�u����

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kFeatureItemSeq[] = _T("FEATURE_ITEM_SEQ"); //!< �Љ���ڃV�[�P���X
	const _TCHAR kFeatureMarkCode[] = _T("FEATURE_MARK_CODE"); //!< �Љ���o���R�[�h
	const _TCHAR kFeatureMarkName[] = _T("FEATURE_MARK_NAME"); //!< �Љ���o������
	const _TCHAR kUpdate[] = _T("UPDATE_C"); //!< �X�V�R�[�h
}

// �R���e���c�ڍ׏��
namespace contents_detailinfo {
	const _TCHAR kTableName[] = _T("CONTENTS_DETAILINFO"); // !< �e�[�u����

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kSortOrder[] = _T("SORT_ORDER"); //!< ���я�
	const _TCHAR kFeatureCode[] = _T("FEATURE_CODE"); //!< �ڍ׏�񍀖ڃR�[�h
	const _TCHAR kExist[] = _T("EXIST_F"); //!< ����Ȃ��t���O
}

// ���W�l��ʃ}�X�^
namespace xy_assortment_convert {
	extern const _TCHAR kTableName[] = _T("XY_ASSORTMENT_CONVERT"); //!< �e�[�u����

	extern const _TCHAR kPSTAttCode[] = _T("PST_ATTCODE"); //!< ���W�l��ʃR�[�h
	extern const _TCHAR kOrgPSTAttCode[] = _T("ORG_PST_ATTCODE"); //!< ���W�l��ʃR�[�h�i�h�����j
	extern const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //! �������H�t���O
}

// ���W�l��ʃ}�X�^
namespace xy_assortment_master {
	extern const _TCHAR kTableName[] = _T("XY_ASSORTMENT_MASTER"); //!< �e�[�u����

	extern const _TCHAR kPSTAttCode[] = _T("PST_ATTCODE"); //!< ���W�l��ʃR�[�h
	extern const _TCHAR kName[] = _T("NAME"); //!< ����
	extern const _TCHAR kPSAPriority[] = _T("PSA_PRIORITY"); //!< ���W�l��ʗD�揇��
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
}

// POI���ʎq�R�t����ʃ}�X�^
namespace poiid_connect_master {
	extern const _TCHAR kTableName[] = _T("POIID_CONNECT_MASTER"); //!< �e�[�u����
  
	extern _TCHAR kPCACode[] = _T("POI_CNC_ATTCODE"); //!< POI���ʎq�R�t��ʃR�[�h
	extern _TCHAR kName[] = _T("NAME"); //!< ����
	extern _TCHAR kBankContentsCode[] = _T("BANK_CONTENTS_CODE"); //!< �R���e���c�R�[�h�i��s�j
	extern _TCHAR kATMContentsCode[] = _T("ATM_CONTENTS_CODE"); //!< �R���e���c�R�[�h�iATM�j
}

// ���񋟌��}�X�^
namespace infoprovide_master {
	extern const _TCHAR kTableName[] = _T("INFOPROVIDE_MASTER"); //!< �e�[�u����

	extern const _TCHAR kIPCode[] = _T("IPCODE"); //!< ���񋟌��R�[�h
	extern const _TCHAR kName[] = _T("NAME"); //!< ����
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
}

// ���x�}�X�^
namespace accuracy_master {
	extern const _TCHAR kTableName[] = _T("ACCURACY_MASTER"); //!< �e�[�u����

	extern const _TCHAR kACCCode[] = _T("ACCCODE"); //!< ���x�R�[�h
	extern const _TCHAR kName[] = _T("NAME"); //!< ����
	extern const _TCHAR kMNTPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
}

// �Q�ƃo�[�W�����}�X�^
namespace refversion_master {
	extern const _TCHAR kTableName[] = _T("REFVERSION_MASTER"); //!< �e�[�u����

	extern const _TCHAR kMasterCode[] = _T("MASTER_CODE"); //!< �}�X�^�R�[�h
	extern const _TCHAR kDataLot[] = _T("DATALOT"); //!< �f�[�^���b�g
	extern const _TCHAR kWorkName[] = _T("WORK_NAME"); //!< ��Ɩ�
	extern const _TCHAR kAddr[] = _T("ADDR_VERSION"); //!< �Z�������[�X���[�U
	extern const _TCHAR kAdmin[] = _T("DB_CONNECT_INFO"); //!< SiNDYDB�ڑ����
	extern const _TCHAR kVersionCode[] = _T("VERSION_CODE"); //!< �}�X�^�o�[�W�����R�[�h
}

// �@�\�}�X�^
namespace function_master {
	const _TCHAR kTableName[] = _T("FUNCTION_MASTER"); //!< �e�[�u����

	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< �f�[�^���ʃR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kSiRiUSTableName[] = _T("SIRIUS_TABLE_NAME"); //!< SiRiUS�e�[�u����
	const _TCHAR kTableCode[] = _T("TABLE_CODE"); //!< �i�[�e�[�u���R�[�h
}

// �@�\�ڍג�`
namespace function_detail {
	const _TCHAR kTableName[] = _T("FUNCTION_DETAIL"); //!< �e�[�u����

	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< �f�[�^���ʃR�[�h
	const _TCHAR kSortOrder[] = _T("SORT_ORDER"); //!< �\�[�g��
	const _TCHAR kColumnName[] = _T("COLUMN_NAME"); //!< �t�B�[���h��
	const _TCHAR kSiRiUSColumnName[] = _T("SIRIUS_COLUMN_NAME"); //!< SiRiUS�J������
}

// POI�|�C���g
namespace _poi_point {
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ"); //!< �R���e���c���V�[�P���X
	const _TCHAR kWorker[] = _T("WORKER"); //!< �ҏW��
	const _TCHAR kWorkDate[] = _T("WORK_DATE"); //!< �ҏW������
	const _TCHAR kVerifier[] = _T("VERIFIER"); //!< ���؎�
	const _TCHAR kVerifyDate[] = _T("VERIFY_DATE"); //!< ���؊�����
	const _TCHAR kUPDAvailDate[] = _T("UPD_AVAIL_DATE"); //!< �X�V���L�����t
	const _TCHAR kDelete[] = _T("DELETE_C"); //!< �폜�R�[�h
	const _TCHAR kErrorFixed[] = _T("ERROR_F"); //!< �G���[�C���t���O
	const _TCHAR kPSTIPCode[] = _T("PST_IP_C"); //!< ���W�l�񋟌��R�[�h�i���񋟌��R�[�h�j
	const _TCHAR kACCCode[] = _T("ACC_C"); //!< ���x�R�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kYomi[] = _T("YOMI_D"); //!< �i��؂蕶���t���j�ǂ�
	const _TCHAR kCloseCode[] = _T("CLOSE_C"); //!< �����R�R�[�h
	const _TCHAR kCloseMemo[] = _T("CLOSE_MEMO"); //!< �����R����
	const _TCHAR kMNTMemoAdmin[] = _T("MNT_MEMO_ADMIN"); //!< ���������i�Ǘ��ҁj
	const _TCHAR kMNTMemoWork[] = _T("MNT_MEMO_WORK"); //!< ���������i��Ǝҁj
	const _TCHAR kBGInfo1[] = _T("BG_INFO1"); //!< �Q�l�����P
	const _TCHAR kBGInfo2[] = _T("BG_INFO2"); //!< �Q�l�����Q
	const _TCHAR kURL[] = _T("URL"); //!< URL
	const _TCHAR kIPUseID[] = _T("IP_USE_ID"); //!< ���񋟌����pID
	const _TCHAR kFeature[] = _T("FEATURE"); //!< [�Љ]�Љ
	const _TCHAR kDetailInfo[] = _T("DETAILINFO"); //!< [�ڍ׏��]�ڍ׏��
	const _TCHAR kTel[] = _T("TEL"); //!< [�d�b�ԍ�]�d�b�ԍ�
	const _TCHAR kPrefCode[] = _T("PREFCODE"); //!< [�\���p�s�撬��] �s���{���R�[�h
	const _TCHAR kCityCode[] = _T("CITYCODE"); //!< [�\���p�s�撬��] �s�撬���R�[�h
	const _TCHAR kAddress[] = _T("ADDRESS"); //!< [�Z��]�����ڈȉ��Z��
	const _TCHAR kLiquor[] = _T("LIQUOR_F"); //!< [���戵��]���戵���t���O
	const _TCHAR kCigar[] = _T("CIGAR_F"); //!< [���΂��戵��]���΂��戵���t���O
	const _TCHAR kCRSLM[] = _T("CRS_LM_F"); //!< [�����h�}�[�N]�����_�g�厞�����h�}�[�N�t���O
	const _TCHAR kHeightRST[] = _T("HEIGHT_RST"); //!< [���ԏꐧ��]��������
	const _TCHAR kWidthRST[] = _T("WIDTH_RST"); //!< [���ԏꐧ��]������
	const _TCHAR kLengthRST[] = _T("LENGTH_RST"); //!< [���ԏꐧ��]��������
	const _TCHAR kWeightRST[] = _T("WEIGHT_RST"); //!< [���ԏꐧ��]�d�ʐ���
	const _TCHAR kParkingRSTBit[] = _T("PARKING_RST_B"); //!< [���ԏꐧ��]���ԏꐧ���r�b�g�i3���ځj
	const _TCHAR kInfoFileID[] = _T("INFOFILE_ID"); //!< [�f���t�@�C��]���t�@�C��ID
	const _TCHAR kSLS[] = _T("SLS_C"); //!< [�̎�]�̎ЃR�[�h
	const _TCHAR kCSID[] = _T("CS_ID"); //!< AnyPlace ID
	const _TCHAR kInfoID[] = _T("INFO_ID"); //!< SiNDY-i ID
	const _TCHAR kLMID[] = _T("LM_ID"); //!< �����h�}�[�NID
	const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //!< [�L�����H]�L�����H�t���O
	// ���L�p
#ifdef _SINDYK_SUPPORT_COMMON_RELATION // �e�q�֌W�̃T�|�[�g
	const _TCHAR kAnnoGUID[] = _T("ANNO_DISP_ID"); //!< ���L�\���pGUID
#endif // ifdef _SINDYK_SUPPORT_COMMON_RELATION
	const _TCHAR kAnnoName1[] = _T("ANNO_DISP_NAME1"); //!< ���L�\���p������P
	const _TCHAR kAnnoName2[] = _T("ANNO_DISP_NAME2"); //!< ���L�\���p������Q
	const _TCHAR kAnnoHyoukou[] = _T("ANNO_DISP_HYOUKOU"); //!< ���L�\���p�W���l
	const _TCHAR kAnnoMntMemo[] = _T("MNT_MEMO_ANNO"); //!< ���L��������
	const _TCHAR kAnnoDispCode[] = _T("ANNO_DISP_C"); //!< ���L�\���R�[�h
	const _TCHAR kAnnoDispLevel[] = _T("ANNO_DISP_LEVEL"); //!< ���L�\�����x��
	const _TCHAR kAnnoDispNL[] = _T("ANNO_DISP_NL"); //!< ���L���s�ʒu
	const _TCHAR kAnnoDispYomi[] = _T("ANNO_DISP_YOMI"); //!< ���L���~
	const _TCHAR kSerialID[] = _T("SERIAL_ID"); //!< �V���A��ID
	const _TCHAR kPartialNameD[] = _T("PARTIALNAME_D"); //!< [������v]������v���́i��؂蕶���t���j
	const _TCHAR kPartialYomiD[] = _T("PARTIALYOMI_D"); //!< [������v]������v��݁i��؂蕶���t���j
	const _TCHAR kTempleSeq[] = _T("TEMPLE_SEQ"); //!< [�D��]�D���ԍ�
	const _TCHAR kEditStatusCode[] = _T("EDT_STATUS_C"); //! �ҏW��ԃR�[�h
	const _TCHAR kEmergencyShelterC[] = _T("EMERGENCY_SHELTER_C"); //!< [�ً}��]�ً}���R�[�h
}

namespace poi_point {
	const _TCHAR kTableName[] = _T("POI_POINT"); //!< �e�[�u����
}

namespace poi_point_org {
	const _TCHAR kTableName[] = _T("POI_POINT_ORG"); //!< �e�[�u����
}

// POI�T�u�|�C���g
namespace _poi_sub_point {
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID"); //!< POI�|�C���gID
	const _TCHAR kIDNCode[] = _T("IDNCODE"); //!< �f�[�^���ʃR�[�h
	const _TCHAR kFuncSeq[] = _T("FUNCTION_SEQ"); //!< �@�\���V�[�P���X
	const _TCHAR kDelete[] = _T("DELETE_C"); //!< �폜�R�[�h
	const _TCHAR kPSTAttCode[] = _T("PST_ATT_C"); //!< [���ԏ�/���ԏ����/���B]���W�l��ʃR�[�h
	const _TCHAR kPriority[] = _T("PRIORITY"); //!< [���ԏ�/���ԏ����/���B]���W�l��ʓ��D�揇��
	const _TCHAR kPriorityAttCode[] = _T("PRIORITY_ATT_C"); //!< [���ԏ�/���ԏ����/���B]���W�l��ʓ��D�揇�ʃR�[�h
	const _TCHAR kMNTInfo[] = _T("MNT_INFO"); //!< [���ԏ���/���B���] ������Ə��
	const _TCHAR kTollRoad[] = _T("TOLLROAD_F"); //!< [�L�����H]�L�����H�t���O
}

namespace poi_sub_point {
	const _TCHAR kTableName[] = _T("POI_SUB_POINT"); //!< �e�[�u����
}

namespace poi_sub_point_org {
	const _TCHAR kTableName[] = _T("POI_SUB_POINT_ORG"); //!< �e�[�u����
}

// POI�T�u����
namespace _poi_sub_attr {
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID");         //!< POI�|�C���gID
	const _TCHAR kIDNCode[] = _T("IDNCODE");     //!< �f�[�^���ʃR�[�h
	const _TCHAR kStartDate[] = _T("START_DATE"); //!< [�����c�Ǝ���]�J�n��
	const _TCHAR kEndDate[] = _T("END_DATE"); //!< [�����c�Ǝ���]�I����
	const _TCHAR kOpenTime[] = _T("OPEN_TIME"); //!< [�����c�Ǝ���]�c�ƊJ�n����
	const _TCHAR kCloseTime[] = _T("CLOSE_TIME"); //!< [�����c�Ǝ���]�c�ƏI������
	const _TCHAR kDayCode[] = _T("DAY_C"); //!< [�����c�Ǝ���]�j���R�[�h
	const _TCHAR kMultiChainCode[] = _T("MULTI_CHAIN_C"); //!< [�����X��]�����X�܃R�[�h
}

namespace poi_sub_attr {
	const _TCHAR kTableName[] = _T("POI_SUB_ATTR"); //!< �e�[�u����
}

namespace poi_sub_attr_org {
	const _TCHAR kTableName[] = _T("POI_SUB_ATTR_ORG"); //!< �e�[�u����
}

// POI�֘A
namespace _poi_relation {
	const _TCHAR kTableName[] = _T("POI_RELATION"); //!< �e�[�u����

	const _TCHAR kRelatedKey[] = _T("RELATED_KEY"); //!< �֘A�L�[
	const _TCHAR kPOIPointID[] = _T("POI_POINT_ID"); //!< POI�|�C���gID
	const _TCHAR kFuncSeq[] = _T("FUNCTION_SEQ"); //!< �@�\���V�[�P���X
	const _TCHAR kTmpParkingID[] = _T("TMP_PARKING_ID_D"); //!< [�f�p�[�g��g���ԏ�] �i��؂蕶���t�����ԏ�ID
	const _TCHAR kPOICNCATT[] = _T("POI_CNC_ATT_C"); //!< [POI���ʎq�R�t] POI���ʎq�R�t��ʃR�[�h
	const _TCHAR kParkingID[] = _T("PARKING_ID"); //!< [��g�Ǝ����ԏ�] �Ǝ����ԏ�ID
}

namespace poi_relation {
	const _TCHAR kTableName[] = _T("POI_RELATION"); //!< �e�[�u����
}

namespace poi_relation_org {
	const _TCHAR kTableName[] = _T("POI_RELATION_ORG"); //!< �e�[�u����
}

/// �X�ܕ�����ƍ�����
namespace dicchn {
	const _TCHAR kTableName[] = _T("DICCHN"); //!< �e�[�u����

	const _TCHAR kInitDate[] = _T("INIT_DATE"); //!< ����o�^��
	const _TCHAR kDeleteDate[] = _T("DELETE_DATE"); //!< �폜��
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< �f�[�^���ރR�[�h
	const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< �f�[�^�X�܃R�[�h
	const _TCHAR kChkDataSeq[] = _T("CHK_DATA_SEQ"); //!< �`�F�b�N�f�[�^�V�[�P���X
	const _TCHAR kFwdName[] = _T("FWD_NAME"); //!< ���́i�O����v�j
	const _TCHAR kMidName[] = _T("MID_NAME"); //!< ���́i���Ԉ�v�j
	const _TCHAR kBwdName[] = _T("BWD_NAME"); //!< ���́i�����v�j
	const _TCHAR kFwdYomi[] = _T("FWD_YOMI"); //!< �ǂ݁i�O����v�j
	const _TCHAR kMidYomi[] = _T("MID_YOMI"); //!< �ǂ݁i���Ԉ�v�j
	const _TCHAR kBwdYomi[] = _T("BWD_YOMI"); //!< �ǂ݁i�����v�j
} // dicchn

/// �V���A��ID�쐬�e�[�u��
namespace create_serial_id {
	const _TCHAR kTableName[] = _T("CREATE_SERIALID");	//!< �e�[�u����
}

/// id_table��id_addr_table�̋��ʕ���
namespace id_table_base {
	const _TCHAR kAddIDType[] = _T("ADDIDTYPE_C");	//!< ID�t�^���
} // namespace id_table_base

/// �V���A��ID�Ή��e�[�u��
namespace id_table {
	using namespace id_table_base;

	const _TCHAR kTableName[] = _T("IDTABLE");		//!< �e�[�u����

	const _TCHAR kMeshCode[] = _T("MESHCODE");		//!< �Q�����b�V���R�[�h
	const _TCHAR kX[] = _T("X");				//!< �Q�����b�V�������K��X���W
	const _TCHAR kY[] = _T("Y");				//!< �Q�����b�V�������K��Y���W
	const _TCHAR kCrossName[] = _T("CRS_NAME");		//!< �����_����
	const _TCHAR kRoadTypeCode[] = _T("ROADTYPECODE");	//!< �Ǘ��c�̃R�[�h
	const _TCHAR kRoadTypeCodeSeq[] = _T("ROADTYPECODE_SEQ");	//!< �H���V�[�P���X�ԍ�
	const _TCHAR kRoadSeq[] = _T("ROAD_SEQ");			//!< �H�����V�[�P���X�ԍ�
	const _TCHAR kFacilName[] = _T("FCL_NAME");		//!< ���H�{�ݖ���
	const _TCHAR kFacilAttrCode[] = _T("FCL_ATTCODE");	//!< ���H�{�ݎ�ʃR�[�h
	const _TCHAR kDirectionCode[] = _T("DIRECTIONCODE");	//!< ���ʃR�[�h
	const _TCHAR kDirectionName[] = _T("DIRECTIONNAME");	//!< ���ʖ���
	const _TCHAR kTelCode[] = _T("TEL_CODE");			//!< �ǔ�
}

/// �Z���R���e���c���V�[�P���X�Ή��e�[�u��
namespace id_addr_table
{
	using namespace id_table_base;

	const _TCHAR kTableName[] = _T("IDADDRTABLE");	//!< �e�[�u����

	const _TCHAR kZipCode[] = _T("ZIPCODE");		//!< �X�֔ԍ�
	const _TCHAR kName[] = _T("NAME");		//!< ����
	const _TCHAR kPrefCode[] = _T("PREFCODE");	//!< �s���{���R�[�h
	const _TCHAR kCityCode[] = _T("CITYCODE");	//!< �s�撬���R�[�h
	const _TCHAR kAdrCode1[] = _T("ADRCODE1");	//!< �厚�E�ʏ̃R�[�h
	const _TCHAR kAdrCode2[] = _T("ADRCODE2");	//!< ���E���ڃR�[�h
	const _TCHAR kChibanCode[] = _T("CHIBANCODE");	//!< �n�ԃR�[�h
	const _TCHAR kJukyoCode[] = _T("JUKYOCODE");	//!< �Z���ԍ��R�[�h
}

/// �R���e���c���V�[�P���X�Ǘ��e�[�u��
namespace dtm_seq {
	const _TCHAR kTableName[] = _T("DTMSEQ");		//!< �e�[�u����

	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ");		//!< �R���e���c���V�[�P���X�̍ő�l
	const _TCHAR kLockFlag[] = _T("LOCK_F");		//!< ���b�N�t���O
}

/// �R���e���c���ʎq�ϊ��e�[�u��
namespace contents_convert {
	const _TCHAR kTableName[] = _T("CONTENTS_CONVERT"); //!< �e�[�u����

	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE"); //!< �R���e���c�R�[�h
	const _TCHAR kIPCode[] = _T("IPCODE"); //!< IP�R�[�h
	const _TCHAR kDataKubunCode[] = _T("DATA_KUBUN_CODE"); //!< �f�[�^�敪�R�[�h
	const _TCHAR kAttCode[] = _T("ATTCODE"); //!< ��ʃR�[�h
	const _TCHAR kChainCode8D[] = _T("CHAINCODE_8D"); //!< �X�܃R�[�h(8��)
}

/// NTT�Ǝ�}�X�^
namespace ntt_master {
	const _TCHAR kTableName[] = _T("NTT_MASTER"); //!< �e�[�u����

	const _TCHAR kNTTCode[] = _T("NTTCODE"); //!< NTT�Ǝ�R�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
	const _TCHAR kNTTGroupCode[] = _T("NTTGRPCODE"); //!< NTT��{���ރR�[�h
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< �f�[�^���ރR�[�h
}

/// �f�[�^�擾�敪�}�X�^
namespace data_kubun_master {
	const _TCHAR kTableName[] = _T("DATA_KUBUN_MASTER"); //!< �e�[�u����

	const _TCHAR kDataKubunCode[] = _T("DATA_KUBUN_CODE");; //!< �f�[�^�擾�敪�R�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
}

/// ��ʃ}�X�^
namespace assortment_master {
	const _TCHAR kTableName[] = _T("ASSORTMENT_MASTER"); //!< �e�[�u����

	const _TCHAR kATTCode[] = _T("ATTCODE"); //!< ��ʃR�[�h
	const _TCHAR kName[] = _T("NAME"); //!< ����
}

/// �ǔԕϊ��e�[�u��
namespace tel_convert {
	const _TCHAR kTableName[] = _T("TEL_CONVERT"); //!< �e�[�u����

	const _TCHAR kTelInfOld[] = _T("TEL_INF_OLD"); //!< �ԍ��̈�
	const _TCHAR kTelCodeOld[] = _T("TELCODE_OLD"); //!< �ύX�O�s�O�ǔ�
	const _TCHAR kTelCode[] = _T("TELCODE");//!< �ύX��s�O�ǔ�
	const _TCHAR kTelAdd[] = _T("TEL_ADD"); //!< �ύX��t�����
	const _TCHAR kDigitOld[] = _T("DIGIT_OLD"); //!< �ύX�O�ԍ�����
	const _TCHAR kDigit[] = _T("DIGIT"); //!< �ύX��ԍ�����
	const _TCHAR kCngTelF[] = _T("CNG_TEL_F"); //!< �ǔԕύX�Ώۃt���O
	const _TCHAR kTgtArea[] = _T("TGT_AREA"); //!< �Ώےn��
	const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< ���{���t
}

/// �j���R�[�h�ϊ��e�[�u��
namespace day_convert{
	const _TCHAR kTableName[] = _T("DAY_CONVERT"); //!< �e�[�u����
	
	const _TCHAR kDayCode[] = _T("DAYCODE"); //!< �j���R�[�h
	const _TCHAR kDayCodeBit[] = _T("DAYCODE_BIT"); //!< �j���R�[�h�i�r�b�g�j
	const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< ���Ԏ�ʃR�[�h
}

/// �S�����ޕϊ��e�[�u��
namespace traintype_convert{
	const _TCHAR kTableName[] = _T("TRAINTYPE_CONVERT"); //!< �e�[�u����
	
	const _TCHAR kGroupCode[] = _T("GRPCODE"); //!< �f�[�^���ރR�[�h
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ"); //!< �R���e���c���V�[�P���X
	const _TCHAR kTrainTypeCode[] = _T("TRAINTYPECODE"); //!< �S�����ރR�[�h
	const _TCHAR kTrainCorpCode[] = _T("TRAINCORPCODE"); //!< �S����ЃR�[�h
	const _TCHAR kTrainTypeCodeOld[] = _T("TRAINTYPECODE_OLD"); //!< �S�����ރR�[�h�i���j
	const _TCHAR kTrcDplF[] = _T("TRC_DPL_F"); //!< �S�����ޏd���t���O
}

} // sindyk

} // schema

} // sindy
