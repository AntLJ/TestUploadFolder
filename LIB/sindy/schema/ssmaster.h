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
namespace ssmaster {

	// DTMDSP(�f�[�^�d�l�Ǘ�)
	namespace dtmdsp {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];       //!< �f�[�^���ރR�[�h
		extern const _TCHAR kChainCode[];     //!< �X�܃R�[�h
		extern const _TCHAR kIdnCode[];       //!< �f�[�^���ʃR�[�h
		extern const _TCHAR kFeatureItem1[];  //!< �Љ����1
		extern const _TCHAR kFeatureItem2[];  //!< �Љ����2
		extern const _TCHAR kFeatureItem3[];  //!< �Љ����3
		extern const _TCHAR kFeatureItem4[];  //!< �Љ����4
		extern const _TCHAR kFeatureItem5[];  //!< �Љ����5
		extern const _TCHAR kFeatureItem6[];  //!< �Љ����6
		extern const _TCHAR kFeatureItem7[];  //!< �Љ����7
		extern const _TCHAR kFeatureItem8[];  //!< �Љ����8
		extern const _TCHAR kFeatureItem9[];  //!< �Љ����9
		extern const _TCHAR kFeatureItem10[]; //!< �Љ����10
		extern const _TCHAR kInfoFilePath[];  //!< ���t�@�C���i�[��
		extern const _TCHAR kInfoFileseq[];   //!< ���t�@�C���V�[�P���X
	}

	// DTMIDN(�f�[�^���ʃ}�X�^)
	namespace dtmidn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIdnCode[];        //!< �f�[�^���ʃR�[�h
		extern const _TCHAR kName[];           //!< ����
		extern const _TCHAR kTableNameField[]; //!< �e�[�u������
		extern const _TCHAR kTableCode[];      //!< �i�[�e�[�u���R�[�h
	}

	// DTMGLC(�f�[�^���ޓX�܃}�X�^)
	namespace dtmglc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];      //!< �f�[�^���ރR�[�h
		extern const _TCHAR kChainCode[];    //!< �X�܃R�[�h
		extern const _TCHAR kName[];         //!< ����
		extern const _TCHAR kMntGroupCode[]; //!< ���������e�O���[�v�R�[�h
		extern const _TCHAR kCatGroupCode[]; //!< �啪�ރO���[�v�R�[�h
		extern const _TCHAR kContentsType[]; //!< �R���e���c�^�C�v
	}

	// MSTGRP(�f�[�^���ރ}�X�^)
	namespace mstgrp {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];    //!< �f�[�^���ރR�[�h
		extern const _TCHAR kName[];       //!< ����
		extern const _TCHAR kMjrGrpCode[]; //!< �f�[�^�啪�ރR�[�h
	}

	// MSTIRP(���񋟌��}�X�^)
	namespace mstipr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIPCode[];      //!< ���񋟌��R�[�h
		extern const _TCHAR kName[];        //!< ����
		extern const _TCHAR kMntPriority[]; //!< �����D�揇��
	}

	// MSTACC(���x�}�X�^)
	namespace mstacc {
		extern const _TCHAR kTableName[];
				
		extern const _TCHAR kAccCode[];     //!< ���x�R�[�h
		extern const _TCHAR kName[];        //!< ����
		extern const _TCHAR kMntPriority[]; //!< �����D�揇��
	}

	// MASTCLS(�����R�}�X�^)
	namespace mstcls {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCloseCode[]; //!< �����R�R�[�h
		extern const _TCHAR kName[];      //!< ����
	}

	// MSTFTM(�Љ���o���}�X�^)
	namespace mstftm {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFeatureMark[]; //!< �Љ���o���R�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// MSTDAY(�j���}�X�^)
	namespace mstday {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDayCode[]; //!< �j���R�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// MSTPSA(���W�l��ʃ}�X�^)
	namespace mstpsa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPstAttCode[];  //!< ���W�l��ʃR�[�h
		extern const _TCHAR kName[];        //!< ����
		extern const _TCHAR kPsaPriority[]; //!< ���W�l��ʊԗD�揇��
		extern const _TCHAR kMntPriority[]; //!< �����D�揇��
	}

	// MSTPRA(���W�l��ʓ��D�揇�ʃ}�X�^)
	namespace mstpra {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPriorityAttCode[]; //!< ���W�l��ʓ��D�揇�ʎ�ʃR�[�h
		extern const _TCHAR kName[];            //!< ����
	}

	// MSTSLS(�̎Ѓ}�X�^)
	namespace mstsls {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kSlsCode[];   //!< �̎ЃR�[�h
		extern const _TCHAR kName[];      //!< ����
		extern const _TCHAR kYomi[];      //!< �ǂ�
		extern const _TCHAR kGrpCode[];   //!< �f�[�^���ރR�[�h
		extern const _TCHAR kChainCode[]; //!< �X�܃R�[�h
	}

	// MSTTST(���o�}�X�^)
	namespace msttst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNo[];    //!< ���o�R�[�h
		extern const _TCHAR kTaste[]; //!< ���o
	}

	// MSTPCA(POI���ʎq�R�t��ʃ}�X�^)
	namespace mstpca {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPoiCncAttCode[];    //!< POI���ʎq�R�t��ʃR�[�h
		extern const _TCHAR kName[];             //!< ����
		extern const _TCHAR kBankContentsCode[]; //!< �R���e���c�R�[�h(��s)
		extern const _TCHAR kAtmContentsCode[];  //!< �R���e���c�R�[�h(ATM)
	}

	// DTMDTS(�ڍ׏��d�l�Ǘ�)
	namespace dtmdts {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];     //!< �啪�ރR�[�h
		extern const _TCHAR kChainCode[];   //!< �X�܃R�[�h
		extern const _TCHAR kItemCode[];    //!< ���ڃR�[�h
		extern const _TCHAR kItemMngFlag[]; //!< ���ڎ戵���Ǘ��t���O
		extern const _TCHAR kSortOrder[];   //!< ���я�
	}

	// MSTDTI(�ڍ׏�񍀖ڃ}�X�^)
	namespace mstdti {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kItemCode[]; //!< ���ڃR�[�h
		extern const _TCHAR kName[];     //!< ����
	}

	// MSTATT(��ʃ}�X�^)
	namespace mstatt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAttCode[]; //!< ��ʃR�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// MSTCST(�n�ԏ�ԃ}�X�^)
	namespace mstcst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChibanStatus[]; //!< �n�ԏ�ԃR�[�h
		extern const _TCHAR kName[];         //!< ����
		extern const _TCHAR kPnpSetFlag[];   //!< �s���|�C���g�ݒ�Ώۃt���O
	}

	// MSTDIR(���ʃ}�X�^)
	namespace mstdir {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDirectionCode[]; //!< ���ʃR�[�h
		extern const _TCHAR kName[];          //!< ����
		extern const _TCHAR kYomi[];          //!< �ǂ�
	}

	// MSTFCA(���H�{�ݎ�ʃ}�X�^)
	namespace mstfca {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFclAttCode[]; //!< ���H�{�ݎ�ʃR�[�h
		extern const _TCHAR kName[];       //!< ����
		extern const _TCHAR kYomi[];       //!< �ǂ�
	}

	// MSTKBN(�f�[�^�擾�敪�}�X�^)
	namespace mstkbn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDataKubun[]; //!< �f�[�^�敪�擾�R�[�h
		extern const _TCHAR kName[];      //!< ����
	}

	// MSTMSH(2�����b�V���R�[�h�}�X�^)
	namespace mstmsh {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMeshCode[]; //!< 2�����b�V���R�[�h
	}

	// MSTTEL(�d�ԗ������}�X�^)
	namespace msttel {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelCode[];    //!< �d�ԗ�����
		extern const _TCHAR kName[];       //!< ����
		extern const _TCHAR kTelAttCode[]; //!< �d�b�ԍ���ʃR�[�h
		extern const _TCHAR kNacSetFlag[]; //!< �g�p�֎~�ݒ�Ώۃt���O
	}

	// MSTTKB(�d�b�敪�}�X�^)
	namespace msttkb {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelKubun[];   //!< �d�b�敪�R�[�h
		extern const _TCHAR kName[];       //!< ����
		extern const _TCHAR kTelAttCode[]; //!< �d�b�ԍ���ʃR�[�h
	}

	// CNGCID(�R���e���c���ʎq�ϊ�)
	namespace cngcid {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];     //!< �f�[�^���ރR�[�h
		extern const _TCHAR kChainCode[];   //!< �X�܃R�[�h
		extern const _TCHAR kIPCode[];      //!< ���񋟌��R�[�h
		extern const _TCHAR kDataKubun[];   //!< �f�[�^�擾�敪�R�[�h
		extern const _TCHAR kAttCode[];     //!< ��ʃR�[�h
		extern const _TCHAR kChainCode8d[]; //!< �X�܃R�[�h(8��)
	}

	// CNGDAY(�j���ϊ�)
	namespace cngday {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kDayCode[];     //!< �j���R�[�h
		extern const _TCHAR kDayCodeBit[];  //!< �j���R�[�h(�r�b�g)
		extern const _TCHAR kTermAttCode[]; //!< ���Ԏ�ʃR�[�h
	}

	// CNGDVL(�폜�f�[�^�ϊ�)
	namespace cngdvl {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTableNameField[]; //!< �e�[�u������
		extern const _TCHAR kColumnName[];     //!< �J��������
		extern const _TCHAR kFieldsNum[];      //!< �t�@�C�����ڈʒu
		extern const _TCHAR kDeleteValue[];    //!< �폜������
		extern const _TCHAR kMatchMtdCode[];   //!< ��v���@�R�[�h
	}

	// CNGPST(���W�l��ʃR�[�h�ϊ�)
	namespace cngpst {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPstAttCode[];    //!< ���W�l��ʃR�[�h
		extern const _TCHAR kOrgPstAttCode[]; //!< ���W�l��ʃR�[�h(�h����)
		extern const _TCHAR kTollRoadFlag[];  //!< �������H�t���O
	}

	// CNGTRC(�S�����ޕϊ�)
	namespace cngtrc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGrpCode[];          //!< �f�[�^���ރR�[�h
		extern const _TCHAR kRecordSeq[];        //!< �R���e���c���V�[�P���X
		extern const _TCHAR kTrainTypeCode[];    //!< �S�����ރR�[�h
		extern const _TCHAR kTrainCorpCode[];    //!< �S����ЃR�[�h
		extern const _TCHAR kTrainTypeCodeOld[]; //!< �S����ЃR�[�h(��)
		extern const _TCHAR kTrcDplFlag[];       //!< �S�����ޏd���t���O
	}

	// CNGVAL(�i�[������ϊ�)
	namespace cngval {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTableNameField[]; //!< �e�[�u������
		extern const _TCHAR kColumnName[];     //!< �J��������
		extern const _TCHAR kValueOld[];       //!< �ύX�O������
		extern const _TCHAR kValueNew[];       //!< �ύX�㕶����
	}

	// MSTA1A(�厚�E����ʃ}�X�^)
	namespace msta1a {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAdr1AttCode[]; //!< �厚�E����ʃR�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// MSTA2A(���E������ʃ}�X�^)
	namespace msta2a {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAdr2AttCode[]; //!< ���E������ʃR�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// MSTAFA(�t���@�\��ʃ}�X�^)
	namespace mstafa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAddFuncAttCode[]; //!< �t���@�\��ʃR�[�h
		extern const _TCHAR kName[];           //!< ����
	}

	// MSTAFS(�t���@�\��ԃ}�X�^)
	namespace mstafs {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAddFuncStatus[]; //!< �t���@�\��ԃR�[�h
		extern const _TCHAR kName[];          //!< ����
	}

	// MSTCHG(�X�ܕ��ރ}�X�^)
	namespace mstchg {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChnGrpCode[]; //!< �X�ܕ��ރR�[�h
		extern const _TCHAR kName[];       //!< ����
	}

	// MSTCPC(�@�l�i�}�X�^)
	namespace mstcpc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCorporateClass[]; //!< �@�l�i�R�[�h
		extern const _TCHAR kName[];          //!< ����
	}

	// MSTCPJ(�@�l�i�ڑ��ʒu�}�X�^)
	namespace mstcpj {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCorporateJoint[]; //!< �@�l�i�ڑ��ʒu�R�[�h
		extern const _TCHAR kName[];           //!< ����
	}

	// MSTFRA(���H�{�ݑ�\�_��ʃ}�X�^)
	namespace mstfra {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kFclRepAttCode[]; //!< ���H�{�ݑ�\�_��ʃR�[�h
		extern const _TCHAR kName[];         //!< ����
	}

	// MSTGND(���ʃ}�X�^)
	namespace mstgnd {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kGenderCode[]; //!< ���ʃR�[�h
		extern const _TCHAR kName[];       //!< ����
	}

	// MSTLNG(����}�X�^)
	namespace mstlng {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kLangCode[]; //!< ����R�[�h
		extern const _TCHAR kName[];     //!< ����
	}

	// MSTMGR(�f�[�^�啪�ރ}�X�^)
	namespace mstmgr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMjrGrpCode[]; //!< �f�[�^�啪�ރR�[�h
		extern const _TCHAR kName[];       //!< ����
	}

	// MSTMMD(��v���@�}�X�^)
	namespace mstmmd {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMatchMtdCode[]; //!< ��v���@�R�[�h
		extern const _TCHAR kName[];         //!< ����
	}

	// MSTNAV(�g�p�֎~���R�}�X�^)
	namespace mstnav {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNaCode[]; //!< �g�p�֎~���R�R�[�h
		extern const _TCHAR kName[];   //!< ����
	}

	// MSTSTF(�X�܌`�ԃ}�X�^)
	namespace mststf {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kStoreForm[]; //!< �X�܌`�ԃR�[�h
		extern const _TCHAR kName[];      //!< ����
	}

	// MSTTLA(�d�b�ԍ���ʃ}�X�^)
	namespace msttla {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelAttCode[]; //!< �d�b�ԍ���ʃR�[�h
		extern const _TCHAR kName[];       //!< ����
	}

	// MSTTMA(���Ԏ�ʃ}�X�^)
	namespace msttma {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTermAttCode[]; //!< ���Ԏ�ʃR�[�h
		extern const _TCHAR kName[];        //!< ����
	}

	// MSTUDA(�㉺����ʃ}�X�^)
	namespace mstuda {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kUpDownAttCode[]; //!< �㉺����ʃR�[�h
		extern const _TCHAR kName[];          //!< ����
	}

	// MSTZPA(�X�֔ԍ���ʃ}�X�^)
	namespace mstzpa {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kZipAttCode[]; //!< �X�֔ԍ���ʃR�[�h
		extern const _TCHAR kName[];       //!< ����
	}

	// CNGACC(���x�ϊ�TB)
	namespace cngacc {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kAccCodeDB[];   //!< ���x�R�[�h(DB�i�[�l)
		extern const _TCHAR kAccCodeAddr[]; //!< ���x�R�[�h(�Z��)
	}

	// MSTFCD(�@�\�ڍ�)
	namespace mstfcd {
		extern const _TCHAR kTableName[];
				
		extern const _TCHAR kIdnCode[];          //!< �f�[�^���ʃR�[�h
		extern const _TCHAR kSortOrder[];        //!< ���я�
		extern const _TCHAR kColumnName[];       //!< ��
		extern const _TCHAR kSiriusColumnName[]; //!< SS-Format���`
	}

	// MSTREF(�Q�ƃo�[�W�����}�X�^)
	namespace mstref {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMasterCode[];    //!< �}�X�^�R�[�h
		extern const _TCHAR kDataLot[];       //!< �f�[�^���b�g
		extern const _TCHAR kWorkName[];      //!< ��Ɩ�
		extern const _TCHAR kAddrVersion[];   //!< �Z�������[�X���[�U
		extern const _TCHAR kDBConnectInfo[]; //!< SiNDYDB�ڑ����
		extern const _TCHAR kVersionCode[];   //!< �}�X�^�o�[�W�����R�[�h
	}

	// MSTMNT(���������e�O���[�v�}�X�^)
	namespace mstmnt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kMntCode[]; //!< ���������e�O���[�v�R�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// MSTCAT(�啪�ރO���[�v�}�X�^)
	namespace mstcat {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCatCode[]; //!< �啪�ރR�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// MSTTBL(�@�\�i�[��e�[�u���}�X�^)
	namespace msttbl {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTblCode[]; //!< �@�\�i�[��e�[�u���R�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// CNGTLA(����s�O�ǔԃ}�X�^)
	namespace cngtla {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTel[];        //!< �s�O�ǔ�
		extern const _TCHAR kTelAttCode[]; //!< ����s�O�ǔԃR�[�h
	}

	// CNGADR(�Z���R�[�h�ϊ�)
	namespace cngadr {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kPrefCodeOld[];  //!< ��:�s���{���R�[�h
		extern const _TCHAR kCityCodeOld[];  //!< ��:�s�撬���R�[�h
		extern const _TCHAR kPrefNameOld[];  //!< ��:�s���{������
		extern const _TCHAR kCityNameOld[];  //!< ��:�s�撬������
		extern const _TCHAR kPrefCodeNew[];  //!< �V:�s���{���R�[�h
		extern const _TCHAR kCityCodeNew[];  //!< �V:�s�撬���R�[�h
		extern const _TCHAR kPrefNameNew[];  //!< �V:�s���{������
		extern const _TCHAR kCityNameNew[];  //!< �V:�s�撬������
		extern const _TCHAR kSheetSeq[];     //!< EXCEL�V�[�g�V�[�P���X
		extern const _TCHAR kEnforcedDate[]; //!< ���{���t
	}

	// MSTCHN(�X�܃}�X�^)
	namespace mstchn {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kChainCode[];   //!< �X�܃R�[�h
		extern const _TCHAR kChaincode8d[]; //!< �X�܃R�[�h(8��)
		extern const _TCHAR kChnGrpCode[];  //!< �X�ܕ��ރR�[�h
		extern const _TCHAR kName[];        //!< ����
		extern const _TCHAR kYomi[];        //!< �ǂ�
		extern const _TCHAR kTwnSetFlag[];  //!< �^�E���y�[�W�ݒ�Ώۃt���O
		extern const _TCHAR kGnrSetFlag[];  //!< ���POI�ݒ�Ώۃt���O
	}

	// MSTNSS
	namespace mstnss {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNssCode[]; //!< NSS�Ǝ�R�[�h
		extern const _TCHAR kName[];    //!< ����
	}

	// MSTNTT
	namespace mstntt {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kNttCode[];    //!< NTT�Ǝ�R�[�h
		extern const _TCHAR kName[];       //!< NTT�Ǝ햼��
		extern const _TCHAR kNttGrpCode[]; //!< NTT��{���ރR�[�h
		extern const _TCHAR kGrpCode[];    //!< �f�[�^���ރR�[�h
	}

	// CNGTEL(�ǔԕϊ�)
	namespace cngtel {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kTelInfOld[];    //!< �ԍ��̈�
		extern const _TCHAR kTelCOld[];      //!< �ύX�O�s�O�ǔ�
		extern const _TCHAR kTelCNew[];      //!< �ύX��s�O�ǔ�
		extern const _TCHAR kTelAddNew[];    //!< �ύX��t�����
		extern const _TCHAR kDigitOld[];     //!< �ύX�O�ԍ�����
		extern const _TCHAR kDigitNew[];     //!< �ύX��ԍ�����
		extern const _TCHAR kTgtArea[];      //!< �Ώےn��
		extern const _TCHAR kCngTelFlag[];   //!< �ǔԕϊ��Ώۃt���O
		extern const _TCHAR kEnforcedDate[]; //!< ���{���t
	}

	// GNM001
	namespace gnm001 {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kIPCode[];        //!< ���񋟌��R�[�h
		extern const _TCHAR kDataKubun[];     //!< �f�[�^�擾�敪�R�[�h
		extern const _TCHAR kAttCode[];       //!< ��ʃR�[�h
		extern const _TCHAR kFeatureItem1[];  //!< �Љ����1
		extern const _TCHAR kFeatureItem2[];  //!< �Љ����2
		extern const _TCHAR kFeatureItem3[];  //!< �Љ����3
		extern const _TCHAR kFeatureItem4[];  //!< �Љ����4
		extern const _TCHAR kFeatureItem5[];  //!< �Љ����5
		extern const _TCHAR kFeatureItem6[];  //!< �Љ����6
		extern const _TCHAR kFeatureItem7[];  //!< �Љ����7
		extern const _TCHAR kFeatureItem8[];  //!< �Љ����8
		extern const _TCHAR kFeatureItem9[];  //!< �Љ����9
		extern const _TCHAR kFeatureItem10[]; //!< �Љ����10
		extern const _TCHAR kFeatureItem11[]; //!< �Љ����11
		extern const _TCHAR kFeatureItem12[]; //!< �Љ����12
		extern const _TCHAR kFeatureItem13[]; //!< �Љ����13
		extern const _TCHAR kFeatureItem14[]; //!< �Љ����14
		extern const _TCHAR kFeatureItem15[]; //!< �Љ����15
		extern const _TCHAR kFeatureItem16[]; //!< �Љ����16
		extern const _TCHAR kFeatureItem17[]; //!< �Љ����17
		extern const _TCHAR kFeatureItem18[]; //!< �Љ����18
		extern const _TCHAR kFeatureItem19[]; //!< �Љ����19
		extern const _TCHAR kFeatureItem20[]; //!< �Љ����20
	}

	// MSTEMS(�ً}���R�[�h�}�X�^)
	namespace mstems {
		extern const _TCHAR kTableName[];

		extern const _TCHAR kCode[];  //!< �ً}���R�[�h
		extern const _TCHAR kName[];  //!< ����
	}
} // ssmaster
} // schema
} //sindy
