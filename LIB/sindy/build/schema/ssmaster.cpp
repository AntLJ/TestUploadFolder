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
#include <sindy/schema/ssmaster.h>

namespace sindy {
namespace schema {
namespace ssmaster {

	// DTMDSP(�f�[�^�d�l�Ǘ�)
	namespace dtmdsp {
		const _TCHAR kTableName[] = _T("DTMDSP");

		const _TCHAR kGrpCode[]       = _T("GRPCODE");        //!< �f�[�^���ރR�[�h
		const _TCHAR kChainCode[]     = _T("CHAINCODE");      //!< �X�܃R�[�h
		const _TCHAR kIdnCode[]       = _T("IDNCODE");        //!< �f�[�^���ʃR�[�h
		const _TCHAR kFeatureItem1[]  = _T("FEATURE_ITEM1");  //!< �Љ����1
		const _TCHAR kFeatureItem2[]  = _T("FEATURE_ITEM2");  //!< �Љ����2
		const _TCHAR kFeatureItem3[]  = _T("FEATURE_ITEM3");  //!< �Љ����3
		const _TCHAR kFeatureItem4[]  = _T("FEATURE_ITEM4");  //!< �Љ����4
		const _TCHAR kFeatureItem5[]  = _T("FEATURE_ITEM5");  //!< �Љ����5
		const _TCHAR kFeatureItem6[]  = _T("FEATURE_ITEM6");  //!< �Љ����6
		const _TCHAR kFeatureItem7[]  = _T("FEATURE_ITEM7");  //!< �Љ����7
		const _TCHAR kFeatureItem8[]  = _T("FEATURE_ITEM8");  //!< �Љ����8
		const _TCHAR kFeatureItem9[]  = _T("FEATURE_ITEM9");  //!< �Љ����9
		const _TCHAR kFeatureItem10[] = _T("FEATURE_ITEM10"); //!< �Љ����10
		const _TCHAR kInfoFilePath[]  = _T("INFOFILE_PATH");  //!< ���t�@�C���i�[��
		const _TCHAR kInfoFileseq[]   = _T("INFOFILE_SEQ");   //!< ���t�@�C���V�[�P���X
	}

	// DTMIDN(�f�[�^���ʃ}�X�^)
	namespace dtmidn {
		const _TCHAR kTableName[] = _T("DTMIDN");

		const _TCHAR kIdnCode[]        = _T("IDNCODE");    //!< �f�[�^���ʃR�[�h
		const _TCHAR kName[]           = _T("NAME");       //!< ����
		const _TCHAR kTableNameField[] = _T("TABLE_NAME"); //!< �e�[�u������
		const _TCHAR kTableCode[]      = _T("TABLE_CODE"); //!< �i�[�e�[�u���R�[�h
	}

	// DTMGLC(�f�[�^���ޓX�܃}�X�^)
	namespace dtmglc {
		const _TCHAR kTableName[] = _T("DTMGLC");

		const _TCHAR kGrpCode[]      = _T("GRPCODE");        //!< �f�[�^���ރR�[�h
		const _TCHAR kChainCode[]    = _T("CHAINCODE");      //!< �X�܃R�[�h
		const _TCHAR kName[]         = _T("NAME");           //!< ����
		const _TCHAR kMntGroupCode[] = _T("MNT_GROUP_CODE"); //!< ���������e�O���[�v�R�[�h
		const _TCHAR kCatGroupCode[] = _T("CAT_GROUP_CODE"); //!< �啪�ރO���[�v�R�[�h
		const _TCHAR kContentsType[] = _T("CONTENTS_TYPE");  //!< �R���e���c�^�C�v
	}

	// MSTGRP(�f�[�^���ރ}�X�^)
	namespace mstgrp {
		const _TCHAR kTableName[] = _T("MSTGRP");

		const _TCHAR kGrpCode[]    = _T("GRPCODE");    //!< �f�[�^���ރR�[�h
		const _TCHAR kName[]       = _T("NAME");       //!< ����
		const _TCHAR kMjrGrpCode[] = _T("MJRGRPCODE"); //!< �f�[�^�啪�ރR�[�h
	}

	// MSTIRP(���񋟌��}�X�^)
	namespace mstipr {
		const _TCHAR kTableName[] = _T("MSTIPR");

		const _TCHAR kIPCode[]      = _T("IPCODE");       //!< ���񋟌��R�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
	}

	// MSTACC(���x�}�X�^)
	namespace mstacc {
		const _TCHAR kTableName[] = _T("MSTACC");
				
		const _TCHAR kAccCode[]     = _T("ACCCODE");      //!< ���x�R�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
	}

	// MASTCLS(�����R�}�X�^)
	namespace mstcls {
		const _TCHAR kTableName[] = _T("MSTCLS");

		const _TCHAR kCloseCode[] = _T("CLOSECODE"); //!< �����R�R�[�h
		const _TCHAR kName[]      = _T("NAME");      //!< ����
	}

	// MSTFTM(�Љ���o���}�X�^)
	namespace mstftm {
		const _TCHAR kTableName[] = _T("MSTFTM");

		const _TCHAR kFeatureMark[] = _T("FEATURE_MARK"); //!< �Љ���o���R�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
	}

	// MSTDAY(�j���}�X�^)
	namespace mstday {
		const _TCHAR kTableName[] = _T("MSTDAY");

		const _TCHAR kDayCode[] = _T("DAYCODE"); //!< �j���R�[�h
		const _TCHAR kName[]    = _T("NAME");    //!< ����
	}

	// MSTPSA(���W�l��ʃ}�X�^)
	namespace mstpsa {
		const _TCHAR kTableName[] = _T("MSTPSA");

		const _TCHAR kPstAttCode[]  = _T("PST_ATTCODE");  //!< ���W�l��ʃR�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
		const _TCHAR kPsaPriority[] = _T("PSA_PRIORITY"); //!< ���W�l��ʊԗD�揇��
		const _TCHAR kMntPriority[] = _T("MNT_PRIORITY"); //!< �����D�揇��
	}

	// MSTPRA(���W�l��ʓ��D�揇�ʃ}�X�^)
	namespace mstpra {
		const _TCHAR kTableName[] = _T("MSTPRA");

		const _TCHAR kPriorityAttCode[] = _T("PRIORITY_ATTCODE"); //!< ���W�l��ʓ��D�揇�ʎ�ʃR�[�h
		const _TCHAR kName[]            = _T("NAME");             //!< ����
	}

	// MSTSLS(�̎Ѓ}�X�^)
	namespace mstsls {
		const _TCHAR kTableName[] = _T("MSTSLS");

		const _TCHAR kSlsCode[]   = _T("SLSCODE");   //!< �̎ЃR�[�h
		const _TCHAR kName[]      = _T("NAME");      //!< ����
		const _TCHAR kYomi[]      = _T("YOMI");      //!< �ǂ�
		const _TCHAR kGrpCode[]   = _T("GRPCODE");   //!< �f�[�^���ރR�[�h
		const _TCHAR kChainCode[] = _T("CHAINCODE"); //!< �X�܃R�[�h
	}

	// MSTTST(���o�}�X�^)
	namespace msttst {
		const _TCHAR kTableName[] = _T("MSTTST");

		const _TCHAR kNo[]    = _T("NO");    //!< ���o�R�[�h
		const _TCHAR kTaste[] = _T("TASTE"); //!< ���o
	}

	// MSTPCA(POI���ʎq�R�t��ʃ}�X�^)
	namespace mstpca {
		const _TCHAR kTableName[] = _T("MSTPCA");

		const _TCHAR kPoiCncAttCode[]    = _T("POI_CNC_ATTCODE");    //!< POI���ʎq�R�t��ʃR�[�h
		const _TCHAR kName[]             = _T("NAME");               //!< ����
		const _TCHAR kBankContentsCode[] = _T("BANK_CONTENTS_CODE"); //!< �R���e���c�R�[�h(��s)
		const _TCHAR kAtmContentsCode[]  = _T("ATM_CONTENTS_CODE");  //!< �R���e���c�R�[�h(ATM)
	}

	// DTMDTS(�ڍ׏��d�l�Ǘ�)
	namespace dtmdts {
		const _TCHAR kTableName[] = _T("DTMDTS");

		const _TCHAR kGrpCode[]     = _T("GRPCODE");       //!< �啪�ރR�[�h
		const _TCHAR kChainCode[]   = _T("CHAINCODE");     //!< �X�܃R�[�h
		const _TCHAR kItemCode[]    = _T("ITEMCODE");      //!< ���ڃR�[�h
		const _TCHAR kItemMngFlag[] = _T("ITEM_MNG_FLAG"); //!< ���ڎ戵���Ǘ��t���O
		const _TCHAR kSortOrder[]   = _T("SORT_ORDER");    //!< ���я�
	}

	// MSTDTI(�ڍ׏�񍀖ڃ}�X�^)
	namespace mstdti {
		const _TCHAR kTableName[] = _T("MSTDTI");

		const _TCHAR kItemCode[] = _T("ITEMCODE"); //!< ���ڃR�[�h
		const _TCHAR kName[]     = _T("NAME");     //!< ����
	}

	// MSTATT(��ʃ}�X�^)
	namespace mstatt {
		const _TCHAR kTableName[] = _T("MSTATT");

		const _TCHAR kAttCode[] = _T("ATTCODE"); //!< ��ʃR�[�h
		const _TCHAR kName[]    = _T("NAME");    //!< ����
	}

	// MSTCST(�n�ԏ�ԃ}�X�^)
	namespace mstcst {
		const _TCHAR kTableName[] = _T("MSTCST");

		const _TCHAR kChibanStatus[] = _T("CHIBAN_STATUS"); //!< �n�ԏ�ԃR�[�h
		const _TCHAR kName[]         = _T("NAME");          //!< ����
		const _TCHAR kPnpSetFlag[]   = _T("PNP_SET_FLAG");  //!< �s���|�C���g�ݒ�Ώۃt���O
	}

	// MSTDIR(���ʃ}�X�^)
	namespace mstdir {
		const _TCHAR kTableName[] = _T("MSTDIR");

		const _TCHAR kDirectionCode[] = _T("DIRECTIONCODE"); //!< ���ʃR�[�h
		const _TCHAR kName[]          = _T("NAME");          //!< ����
		const _TCHAR kYomi[]          = _T("YOMI");          //!< �ǂ�
	}

	// MSTFCA(���H�{�ݎ�ʃ}�X�^)
	namespace mstfca {
		const _TCHAR kTableName[] = _T("MSTFCA");

		const _TCHAR kFclAttCode[] = _T("FCL_ATTCODE"); //!< ���H�{�ݎ�ʃR�[�h
		const _TCHAR kName[]       = _T("NAME");        //!< ����
		const _TCHAR kYomi[]       = _T("YOMI");        //!< �ǂ�
	}

	// MSTKBN(�f�[�^�擾�敪�}�X�^)
	namespace mstkbn {
		const _TCHAR kTableName[] = _T("MSTKBN");

		const _TCHAR kDataKubun[] = _T("DATA_KUBUN"); //!< �f�[�^�敪�擾�R�[�h
		const _TCHAR kName[]      = _T("NAME");       //!< ����
	}

	// MSTMSH(2�����b�V���R�[�h�}�X�^)
	namespace mstmsh {
		const _TCHAR kTableName[] = _T("MSTMSH");

		const _TCHAR kMeshCode[] = _T("MESHCODE"); //!< 2�����b�V���R�[�h
	}

	// MSTTEL(�d�ԗ������}�X�^)
	namespace msttel {
		const _TCHAR kTableName[] = _T("MSTTEL");

		const _TCHAR kTelCode[]    = _T("TELCODE");      //!< �d�ԗ�����
		const _TCHAR kName[]       = _T("NAME");         //!< ����
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE");  //!< �d�b�ԍ���ʃR�[�h
		const _TCHAR kNacSetFlag[] = _T("NAC_SET_FLAG"); //!< �g�p�֎~�ݒ�Ώۃt���O
	}

	// MSTTKB(�d�b�敪�}�X�^)
	namespace msttkb {
		const _TCHAR kTableName[] = _T("MSTTKB");

		const _TCHAR kTelKubun[]   = _T("TEL_KUBUN");   //!< �d�b�敪�R�[�h
		const _TCHAR kName[]       = _T("NAME");        //!< ����
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< �d�b�ԍ���ʃR�[�h
	}

	// CNGCID(�R���e���c���ʎq�ϊ�)
	namespace cngcid {
		const _TCHAR kTableName[] = _T("CNGCID");

		const _TCHAR kGrpCode[]     = _T("GRPCODE");      //!< �f�[�^���ރR�[�h
		const _TCHAR kChainCode[]   = _T("CHAINCODE");    //!< �X�܃R�[�h
		const _TCHAR kIPCode[]      = _T("IPCODE");       //!< ���񋟌��R�[�h
		const _TCHAR kDataKubun[]   = _T("DATA_KUBUN");   //!< �f�[�^�擾�敪�R�[�h
		const _TCHAR kAttCode[]     = _T("ATTCODE");      //!< ��ʃR�[�h
		const _TCHAR kChainCode8d[] = _T("CHAINCODE_8D"); //!< �X�܃R�[�h(8��)
	}

	// CNGDAY(�j���ϊ�)
	namespace cngday {
		const _TCHAR kTableName[] = _T("CNGDAY");

		const _TCHAR kDayCode[]     = _T("DAYCODE");      //!< �j���R�[�h
		const _TCHAR kDayCodeBit[]  = _T("DAYCODE_BIT");  //!< �j���R�[�h(�r�b�g)
		const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< ���Ԏ�ʃR�[�h
	}

	// CNGDVL(�폜�f�[�^�ϊ�)
	namespace cngdvl {
		const _TCHAR kTableName[] = _T("CNGDVL");

		const _TCHAR kTableNameField[] = _T("TABLE_NAME");    //!< �e�[�u������
		const _TCHAR kColumnName[]     = _T("COLUMN_NAME");   //!< �J��������
		const _TCHAR kFieldsNum[]      = _T("FIELDS_NUM");    //!< �t�@�C�����ڈʒu
		const _TCHAR kDeleteValue[]    = _T("DELETE_VALUE");  //!< �폜������
		const _TCHAR kMatchMtdCode[]   = _T("MATCH_MTDCODE"); //!< ��v���@�R�[�h
	}

	// CNGPST(���W�l��ʃR�[�h�ϊ�)
	namespace cngpst {
		const _TCHAR kTableName[] = _T("CNGPST");

		const _TCHAR kPstAttCode[]    = _T("PST_ATTCODE");     //!< ���W�l��ʃR�[�h
		const _TCHAR kOrgPstAttCode[] = _T("ORG_PST_ATTCODE"); //!< ���W�l��ʃR�[�h(�h����)
		const _TCHAR kTollRoadFlag[]  = _T("TOLLROAD_FLAG");   //!< �������H�t���O
	}

	// CNGTRC(�S�����ޕϊ�)
	namespace cngtrc {
		const _TCHAR kTableName[] = _T("CNGTRC");

		const _TCHAR kGrpCode[]          = _T("GRPCODE");           //!< �f�[�^���ރR�[�h
		const _TCHAR kRecordSeq[]        = _T("RECORD_SEQ");        //!< �R���e���c���V�[�P���X
		const _TCHAR kTrainTypeCode[]    = _T("TRAINTYPECODE");     //!< �S�����ރR�[�h
		const _TCHAR kTrainCorpCode[]    = _T("TRAINCORPCODE");     //!< �S����ЃR�[�h
		const _TCHAR kTrainTypeCodeOld[] = _T("TRAINTYPECODE_OLD"); //!< �S����ЃR�[�h(��)
		const _TCHAR kTrcDplFlag[]       = _T("TRC_DPL_FLAG");      //!< �S�����ޏd���t���O
	}

	// CNGVAL(�i�[������ϊ�)
	namespace cngval {
		const _TCHAR kTableName[] = _T("CNGVAL");

		const _TCHAR kTableNameField[] = _T("TABLE_NAME");  //!< �e�[�u������
		const _TCHAR kColumnName[]     = _T("COLUMN_NAME"); //!< �J��������
		const _TCHAR kValueOld[]       = _T("VALUE_OLD");   //!< �ύX�O������
		const _TCHAR kValueNew[]       = _T("VALUE_NEW");   //!< �ύX�㕶����
	}

	// MSTA1A(�厚�E����ʃ}�X�^)
	namespace msta1a {
		const _TCHAR kTableName[] = _T("MSTA1A");

		const _TCHAR kAdr1AttCode[] = _T("ADR1_ATTCODE"); //!< �厚�E����ʃR�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
	}

	// MSTA2A(���E������ʃ}�X�^)
	namespace msta2a {
		const _TCHAR kTableName[] = _T("MSTA2A");

		const _TCHAR kAdr2AttCode[] = _T("ADR2_ATTCODE"); //!< ���E������ʃR�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
	}

	// MSTAFA(�t���@�\��ʃ}�X�^)
	namespace mstafa {
		const _TCHAR kTableName[] = _T("MSTAFA");

		const _TCHAR kAddFuncAttCode[] = _T("ADD_FUNC_ATTCODE"); //!< �t���@�\��ʃR�[�h
		const _TCHAR kName[]           = _T("NAME");             //!< ����
	}

	// MSTAFS(�t���@�\��ԃ}�X�^)
	namespace mstafs {
		const _TCHAR kTableName[] = _T("MSTAFS");

		const _TCHAR kAddFuncStatus[] = _T("ADD_FUNC_STATUS"); //!< �t���@�\��ԃR�[�h
		const _TCHAR kName[]          = _T("NAME");            //!< ����
	}

	// MSTCHG(�X�ܕ��ރ}�X�^)
	namespace mstchg {
		const _TCHAR kTableName[] = _T("MSTCHG");

		const _TCHAR kChnGrpCode[] = _T("CHN_GRPCODE"); //!< �X�ܕ��ރR�[�h
		const _TCHAR kName[]       = _T("NAME");       //!< ����
	}

	// MSTCPC(�@�l�i�}�X�^)
	namespace mstcpc {
		const _TCHAR kTableName[] = _T("MSTCPC");

		const _TCHAR kCorporateClass[] = _T("CORPORATE_CLASS"); //!< �@�l�i�R�[�h
		const _TCHAR kName[]          = _T("NAME");             //!< ����
	}

	// MSTCPJ(�@�l�i�ڑ��ʒu�}�X�^)
	namespace mstcpj {
		const _TCHAR kTableName[] = _T("MSTCPJ");

		const _TCHAR kCorporateJoint[] = _T("CORPORATE_JOINT"); //!< �@�l�i�ڑ��ʒu�R�[�h
		const _TCHAR kName[]           = _T("NAME");            //!< ����
	}

	// MSTFRA(���H�{�ݑ�\�_��ʃ}�X�^)
	namespace mstfra {
		const _TCHAR kTableName[] = _T("MSTFRA");

		const _TCHAR kFclRepAttCode[] = _T("FCL_REP_ATTCODE"); //!< ���H�{�ݑ�\�_��ʃR�[�h
		const _TCHAR kName[]          = _T("NAME");            //!< ����
	}

	// MSTGND(���ʃ}�X�^)
	namespace mstgnd {
		const _TCHAR kTableName[] = _T("MSTGND");

		const _TCHAR kGenderCode[] = _T("GENDERCODE"); //!< ���ʃR�[�h
		const _TCHAR kName[]       = _T("NAME");       //!< ����
	}

	// MSTLNG(����}�X�^)
	namespace mstlng {
		const _TCHAR kTableName[] = _T("MSTLNG");

		const _TCHAR kLangCode[] = _T("LANGCODE"); //!< ����R�[�h
		const _TCHAR kName[]     = _T("NAME");     //!< ����
	}

	// MSTMGR(�f�[�^�啪�ރ}�X�^)
	namespace mstmgr {
		const _TCHAR kTableName[] = _T("MSTMGR");

		const _TCHAR kMjrGrpCode[] = _T("MJRGRPCODE"); //!< �f�[�^�啪�ރR�[�h
		const _TCHAR kName[]       = _T("NAME");       //!< ����
	}

	// MSTMMD(��v���@�}�X�^)
	namespace mstmmd {
		const _TCHAR kTableName[] = _T("MSTMMD");

		const _TCHAR kMatchMtdCode[] = _T("MATCH_MTDCODE"); //!< ��v���@�R�[�h
		const _TCHAR kName[]         = _T("NAME");          //!< ����
	}

	// MSTNAV(�g�p�֎~���R�}�X�^)
	namespace mstnav {
		const _TCHAR kTableName[] = _T("MSTNAV");

		const _TCHAR kNaCode[] = _T("NACODE"); //!< �g�p�֎~���R�R�[�h
		const _TCHAR kName[]   = _T("NAME");   //!< ����
	}

	// MSTSTF(�X�܌`�ԃ}�X�^)
	namespace mststf {
		const _TCHAR kTableName[] = _T("MSTSTF");

		const _TCHAR kStoreForm[] = _T("STORE_FORM"); //!< �X�܌`�ԃR�[�h
		const _TCHAR kName[]      = _T("NAME");       //!< ����
	}

	// MSTTLA(�d�b�ԍ���ʃ}�X�^)
	namespace msttla {
		const _TCHAR kTableName[] = _T("MSTTLA");

		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< �d�b�ԍ���ʃR�[�h
		const _TCHAR kName[]       = _T("NAME");        //!< ����
	}

	// MSTTMA(���Ԏ�ʃ}�X�^)
	namespace msttma {
		const _TCHAR kTableName[] = _T("MSTTMA");

		const _TCHAR kTermAttCode[] = _T("TERM_ATTCODE"); //!< ���Ԏ�ʃR�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
	}

	// MSTUDA(�㉺����ʃ}�X�^)
	namespace mstuda {
		const _TCHAR kTableName[] = _T("MSTUDA");

		const _TCHAR kUpDownAttCode[] = _T("UPDOWN_ATTCODE"); //!< �㉺����ʃR�[�h
		const _TCHAR kName[]          = _T("NAME");           //!< ����
	}

	// MSTZPA(�X�֔ԍ���ʃ}�X�^)
	namespace mstzpa {
		const _TCHAR kTableName[] = _T("MSTZPA");

		const _TCHAR kZipAttCode[] = _T("ZIP_ATTCODE"); //!< �X�֔ԍ���ʃR�[�h
		const _TCHAR kName[]       = _T("NAME");        //!< ����
	}

	// CNGACC(���x�ϊ�TB)
	namespace cngacc {
		const _TCHAR kTableName[] = _T("CNGACC");

		const _TCHAR kAccCodeDB[]   = _T("ACCCODE_DB");   //!< ���x�R�[�h(DB�i�[�l)
		const _TCHAR kAccCodeAddr[] = _T("ACCCODE_ADDR"); //!< ���x�R�[�h(�Z��)
	}

	// MSTFCD(�@�\�ڍ�)
	namespace mstfcd {
		const _TCHAR kTableName[] = _T("MSTFCD");
				
		const _TCHAR kIdnCode[]          = _T("IDNCODE");            //!< �f�[�^���ʃR�[�h
		const _TCHAR kSortOrder[]        = _T("SORT_ORDER");         //!< ���я�
		const _TCHAR kColumnName[]       = _T("COLUMN_NAME");        //!< ��
		const _TCHAR kSiriusColumnName[] = _T("SIRIUS_COLUMN_NAME"); //!< SS-Format���`
	}

	// MSTREF(�Q�ƃo�[�W�����}�X�^)
	namespace mstref {
		const _TCHAR kTableName[] = _T("MSTREF");

		const _TCHAR kMasterCode[]    = _T("MASTER_CODE");     //!< �}�X�^�R�[�h
		const _TCHAR kDataLot[]       = _T("DATALOT");         //!< �f�[�^���b�g
		const _TCHAR kWorkName[]      = _T("WORK_NAME");       //!< ��Ɩ�
		const _TCHAR kAddrVersion[]   = _T("ADDR_VERSION");    //!< �Z�������[�X���[�U
		const _TCHAR kDBConnectInfo[] = _T("DB_CONNECT_INFO"); //!< SiNDYDB�ڑ����
		const _TCHAR kVersionCode[]   = _T("VERSION_CODE");    //!< �}�X�^�o�[�W�����R�[�h
	}

	// MSTMNT(���������e�O���[�v�}�X�^)
	namespace mstmnt {
		const _TCHAR kTableName[] = _T("MSTMNT");

		const _TCHAR kMntCode[] = _T("MNT_CODE"); //!< ���������e�O���[�v�R�[�h
		const _TCHAR kName[]    = _T("NAME");     //!< ����
	}

	// MSTCAT(�啪�ރO���[�v�}�X�^)
	namespace mstcat {
		const _TCHAR kTableName[] = _T("MSTCAT");

		const _TCHAR kCatCode[] = _T("CAT_CODE"); //!< �啪�ރR�[�h
		const _TCHAR kName[]    = _T("NAME");     //!< ����
	}

	// MSTTBL(�@�\�i�[��e�[�u���}�X�^)
	namespace msttbl {
		const _TCHAR kTableName[] = _T("MSTTBL");

		const _TCHAR kTblCode[] = _T("TBL_CODE"); //!< �@�\�i�[��e�[�u���R�[�h
		const _TCHAR kName[]    = _T("NAME");     //!< ����
	}

	// CNGTLA(����s�O�ǔԃ}�X�^)
	namespace cngtla {
		const _TCHAR kTableName[] = _T("CNGTLA");

		const _TCHAR kTel[]        = _T("TEL_C");       //!< �s�O�ǔ�
		const _TCHAR kTelAttCode[] = _T("TEL_ATTCODE"); //!< ����s�O�ǔԃR�[�h
	}

	// CNGADR(�Z���R�[�h�ϊ�)
	namespace cngadr {
		const _TCHAR kTableName[] = _T("CNGADR");

		const _TCHAR kPrefCodeOld[]  = _T("PREFCODE_OLD");  //!< ��:�s���{���R�[�h
		const _TCHAR kCityCodeOld[]  = _T("CITYCODE_OLD");  //!< ��:�s�撬���R�[�h
		const _TCHAR kPrefNameOld[]  = _T("PREFNAME_OLD");  //!< ��:�s���{������
		const _TCHAR kCityNameOld[]  = _T("CITYNAME_OLD");  //!< ��:�s�撬������
		const _TCHAR kPrefCodeNew[]  = _T("PREFCODE_NEW");  //!< �V:�s���{���R�[�h
		const _TCHAR kCityCodeNew[]  = _T("CITYCODE_NEW");  //!< �V:�s�撬���R�[�h
		const _TCHAR kPrefNameNew[]  = _T("PREFNAME_NEW");  //!< �V:�s���{������
		const _TCHAR kCityNameNew[]  = _T("CITYNAME_NEW");  //!< �V:�s�撬������
		const _TCHAR kSheetSeq[]     = _T("SHEET_SEQ");     //!< EXCEL�V�[�g�V�[�P���X
		const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< ���{���t
	}

	// MSTCHN(�X�܃}�X�^)
	namespace mstchn {
		const _TCHAR kTableName[] = _T("MSTCHN");

		const _TCHAR kChainCode[]   = _T("CHAINCODE");    //!< �X�܃R�[�h
		const _TCHAR kChaincode8d[] = _T("CHAINCODE_8D"); //!< �X�܃R�[�h(8��)
		const _TCHAR kChnGrpCode[]  = _T("CHN_GRPCODE");  //!< �X�ܕ��ރR�[�h
		const _TCHAR kName[]        = _T("NAME");         //!< ����
		const _TCHAR kYomi[]        = _T("YOMI");         //!< �ǂ�
		const _TCHAR kTwnSetFlag[]  = _T("TWN_SET_FLAG"); //!< �^�E���y�[�W�ݒ�Ώۃt���O
		const _TCHAR kGnrSetFlag[]  = _T("GNR_SET_FLAG"); //!< ���POI�ݒ�Ώۃt���O
	}

	// MSTNSS
	namespace mstnss {
		const _TCHAR kTableName[] = _T("MSTNSS");

		const _TCHAR kNssCode[] = _T("NSSCODE"); //!< NSS�Ǝ�R�[�h
		const _TCHAR kName[]    = _T("NAME");    //!< ����
	}

	// MSTNTT
	namespace mstntt {
		const _TCHAR kTableName[] = _T("MSTNTT");

		const _TCHAR kNttCode[]    = _T("NTTCODE");    //!< NTT�Ǝ�R�[�h
		const _TCHAR kName[]       = _T("NAME");       //!< NTT�Ǝ햼��
		const _TCHAR kNttGrpCode[] = _T("NTTGRPCODE"); //!< NTT��{���ރR�[�h
		const _TCHAR kGrpCode[]    = _T("GRPCODE");    //!< �f�[�^���ރR�[�h
	}

	// CNGTEL(�ǔԕϊ�)
	namespace cngtel {
		const _TCHAR kTableName[] = _T("CNGTEL");

		const _TCHAR kTelInfOld[]    = _T("TEL_INF_OLD");   //!< �ԍ��̈�
		const _TCHAR kTelCOld[]      = _T("TEL_C_OLD");     //!< �ύX�O�s�O�ǔ�
		const _TCHAR kTelCNew[]      = _T("TEL_C_NEW");     //!< �ύX��s�O�ǔ�
		const _TCHAR kTelAddNew[]    = _T("TEL_ADD_NEW");   //!< �ύX��t�����
		const _TCHAR kDigitOld[]     = _T("DIGIT_OLD");     //!< �ύX�O�ԍ�����
		const _TCHAR kDigitNew[]     = _T("DIGIT_NEW");     //!< �ύX��ԍ�����
		const _TCHAR kTgtArea[]      = _T("TGT_AREA");      //!< �Ώےn��
		const _TCHAR kCngTelFlag[]   = _T("CNG_TEL_FLAG");  //!< �ǔԕϊ��Ώۃt���O
		const _TCHAR kEnforcedDate[] = _T("ENFORCED_DATE"); //!< ���{���t
	}

	// GNM001
	namespace gnm001 {
		const _TCHAR kTableName[] = _T("GNM001");

		const _TCHAR kIPCode[]        = _T("IPCODE");         //!< ���񋟌��R�[�h
		const _TCHAR kDataKubun[]     = _T("DATA_KUBUN");     //!< �f�[�^�擾�敪�R�[�h
		const _TCHAR kAttCode[]       = _T("ATTCODE");        //!< ��ʃR�[�h
		const _TCHAR kFeatureItem1[]  = _T("FEATURE_ITEM1");  //!< �Љ����1
		const _TCHAR kFeatureItem2[]  = _T("FEATURE_ITEM2");  //!< �Љ����2
		const _TCHAR kFeatureItem3[]  = _T("FEATURE_ITEM3");  //!< �Љ����3
		const _TCHAR kFeatureItem4[]  = _T("FEATURE_ITEM4");  //!< �Љ����4
		const _TCHAR kFeatureItem5[]  = _T("FEATURE_ITEM5");  //!< �Љ����5
		const _TCHAR kFeatureItem6[]  = _T("FEATURE_ITEM6");  //!< �Љ����6
		const _TCHAR kFeatureItem7[]  = _T("FEATURE_ITEM7");  //!< �Љ����7
		const _TCHAR kFeatureItem8[]  = _T("FEATURE_ITEM8");  //!< �Љ����8
		const _TCHAR kFeatureItem9[]  = _T("FEATURE_ITEM9");  //!< �Љ����9
		const _TCHAR kFeatureItem10[] = _T("FEATURE_ITEM10"); //!< �Љ����10
		const _TCHAR kFeatureItem11[] = _T("FEATURE_ITEM11"); //!< �Љ����11
		const _TCHAR kFeatureItem12[] = _T("FEATURE_ITEM12"); //!< �Љ����12
		const _TCHAR kFeatureItem13[] = _T("FEATURE_ITEM13"); //!< �Љ����13
		const _TCHAR kFeatureItem14[] = _T("FEATURE_ITEM14"); //!< �Љ����14
		const _TCHAR kFeatureItem15[] = _T("FEATURE_ITEM15"); //!< �Љ����15
		const _TCHAR kFeatureItem16[] = _T("FEATURE_ITEM16"); //!< �Љ����16
		const _TCHAR kFeatureItem17[] = _T("FEATURE_ITEM17"); //!< �Љ����17
		const _TCHAR kFeatureItem18[] = _T("FEATURE_ITEM18"); //!< �Љ����18
		const _TCHAR kFeatureItem19[] = _T("FEATURE_ITEM19"); //!< �Љ����19
		const _TCHAR kFeatureItem20[] = _T("FEATURE_ITEM20"); //!< �Љ����20
	}

	// MSTEMS(�ً}���R�[�h�}�X�^)
	namespace mstems {
		const _TCHAR kTableName[] = _T("MSTEMS");

		const _TCHAR kCode[] = _T("EMS_CODE"); //!< �ً}���R�[�h
		const _TCHAR kName[] = _T("NAME"); //!< ����
	}
} // ssmaster
} // schema
} //sindy
