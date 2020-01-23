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

/**
 * @brief �o�̓t�@�C������`
 */
const LPCTSTR ROAD_NAME = _T("road_name");
const LPCTSTR REL_ROAD_NAME_LINK = _T("rel_road_name_link");
const LPCTSTR SPLIT_ROAD_NAME = _T("split_road_name");


// �n����̏ꍇ�ɁA����ROAD_CODE�Ƀv���X�����l
const long CROSS_OVER_PLUS_VALUE = 200000;
// �n�����ROAD_CODE(CROSS_OVER_PLUS_VALUE�𑫂����l)�͈̔�
const long CROSS_OVER_CODE_LOW = 400000;
const long CROSS_OVER_CODE_HIG = 599999;
// ����������Ԃ̏ꍇ�ɁA����ROAD_CODE�Ƀv���X�����l
const long FREE_HWY_PLUS_VALUE = 400000;
// ����������Ԃ�ROAD_CODE�͈̔�
const long FREE_HWY_CODE_LOW = 600000;
const long FREE_HWY_CODE_HIG = 799999;

/**
 * @brief ���H���̃f�[�^���R�[�h�i�[�p
 */
class RoadNameRecord {
public:
	RoadNameRecord()
		:m_nameID(-1), m_nameCls(-1) {}
	RoadNameRecord(const long& nameID, const long& nameCls, const CString& name, const CString& yomi)
		:m_nameID(nameID), m_nameCls(nameCls), m_name(name), m_yomi(yomi) {}
	~RoadNameRecord() {}
public:
	bool operator < (const RoadNameRecord& roadNameRecord) const
	{
		return this->m_nameID < roadNameRecord.m_nameID;
	}
	bool operator == (const RoadNameRecord& roadNameRecord) const
	{
		return this->m_nameID == roadNameRecord.m_nameID;
	}
public:
	long m_nameID;
	long m_nameCls;
	CString m_name;
	CString m_yomi;
};

/**
 * @brief ���H�����N�ɕR�t�����H���̃��R�[�h�i�[�p
 */
class RelNameLinkRecord {
public:
	RelNameLinkRecord()
		:m_roadOID(-1), m_nameID(-1), m_nameCls(-1) {}
	RelNameLinkRecord(const long& roadOID, const long& nameID, const long& nameCls)
		:m_roadOID(roadOID), m_nameID(nameID), m_nameCls(nameCls) {}
	~RelNameLinkRecord() {}
public:
	bool operator < (const RelNameLinkRecord& relNameLinkRecord) const
	{
		return this->m_roadOID != relNameLinkRecord.m_roadOID ? this->m_roadOID < relNameLinkRecord.m_roadOID :
			this->m_nameID < relNameLinkRecord.m_nameID;
	}
	bool operator == (const RelNameLinkRecord& relNameLinkRecord) const
	{
		return this->m_roadOID == relNameLinkRecord.m_roadOID &&
			this->m_nameID == relNameLinkRecord.m_nameID;
	}
public:
	long m_roadOID;
	long m_nameID;
	long m_nameCls;
};

/**
 * @brief ���H���̕����f�[�^���R�[�h�i�[�p
 */
class SpltNameRecord {
public:
	SpltNameRecord()
		:m_nameID(-1), m_spltCls(-1), m_spltSeq(-1) {}
	SpltNameRecord(const long& nameID, const long& spltCls, const long& spltSeq,
		const CString& spltName, const CString& spltYomi)
		:m_nameID(nameID), m_spltCls(spltCls), m_spltSeq(spltSeq), m_spltName(spltName), m_spltYomi(spltYomi) {}
	~SpltNameRecord() {}
public:
	bool operator < (const SpltNameRecord& spltNameRecord) const
	{
		return this->m_nameID != spltNameRecord.m_nameID ? this->m_nameID < spltNameRecord.m_nameID :
			this->m_spltSeq < spltNameRecord.m_spltSeq;
	}
	bool operator == (const SpltNameRecord& spltNameRecord) const
	{
		return this->m_nameID == spltNameRecord.m_nameID &&
			this->m_spltSeq == spltNameRecord.m_spltSeq;
	}
public:
	long m_nameID;
	long m_spltCls;
	long m_spltSeq;
	CString m_spltName;
	CString m_spltYomi;
};

/**
 * @brief ����������擾
 * @return ����������
 */
inline CString getNowTime()
{
	CString strTime;
	
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

	return strTime;
}

/**
 * @brief OID������擾
 * @param ipFeature [in] �t�B�[�`��
 * @return OID������
 */
inline CString getID(const IFeaturePtr& ipFeature)
{
	CString strOID;
	long lOID = 0;
	if(S_OK != ipFeature->get_OID(&lOID)){
		// �G���[�Ȃ��O�ŁA�A�A
		throw std::runtime_error("OID�擾���s");
	}
	strOID.Format(_T("%ld"), lOID);

	return strOID;
}

/**
 * @brief ���p������S�p����������ɕϊ�
 * @param lNumber [in] ���p����
 * @return �S�p����������
 */
inline CString num2ZenStr(long lNumber)
{
	// �����擾
	long digit = log10(lNumber) + 1;

	CString strZenNumber;
	for(long n = digit; n > 0; --n){
		long perNum = pow(10,(n-1));
		long digNum = long(lNumber / perNum);
		lNumber = lNumber % perNum;
		CString strDigNum;

		switch(digNum){
		case 0: strDigNum = _T("�O");	break;
		case 1: strDigNum = _T("�P");	break;
		case 2: strDigNum = _T("�Q");	break;
		case 3: strDigNum = _T("�R");	break;
		case 4: strDigNum = _T("�S");	break;
		case 5:	strDigNum = _T("�T");	break;
		case 6:	strDigNum = _T("�U");	break;
		case 7:	strDigNum = _T("�V");	break;
		case 8:	strDigNum = _T("�W");	break;
		case 9:	strDigNum = _T("�X");	break;
		default:
			// �����ɂ͂��Ȃ��͂��A�A
			throw std::runtime_error("����NG");
			break;
		}
		strZenNumber += strDigNum;
	}
	return strZenNumber;
}

/**
 * @brief ���p�������J�i�ǂ݂ɕϊ��i5���܂Łj
 * @param lNumber [in] ���p����
 * @return �����̃J�i�ǂ�
 */
inline CString num2YomiStr(long lNumber)
{
	// �����擾
	long digit = log10(lNumber) + 1;

	CString strYomi;
	for(long n = digit; n > 0; --n){
		long perNum = pow(10,(n-1));
		long digNum = long(lNumber / perNum);
		lNumber = lNumber % perNum;
		CString strDigYomi;

		// �w�茅�̐��l�̓ǂݐݒ�
		switch(digNum){
		case 1:
			// ���ꏈ��
			// ��A���̈ʂ̏ꍇ�͓ǂ݂Ɂu�C�`�v���t�^����邪�A�\�A�S�A��̈ʂ̏ꍇ�͓ǂݕs�v
			// ��j11111���u�C�`�}���Z���q���N�W���E�C�`�v
			switch(n){
			case 1:
			case 5:
				strDigYomi = _T("�C�`");
				break;
			default:
				break;
			}
			break;
		case 2:
			strDigYomi = _T("�j");
			break;
		case 3:
			strDigYomi = _T("�T��");
			break;
		case 4:
			strDigYomi = _T("����");
			break;
		case 5:
			strDigYomi = _T("�S");
			break;
		case 6:
			// ���ꏈ��
			// �S�̈ʂ̏ꍇ�͓ǂ݂��ς��
			// ��j66666���u���N�}�����N�Z�����b�s���N���N�W���E���N�v
			if(3 == n){
				strDigYomi = _T("���b");
			} else {
				strDigYomi = _T("���N");
			}
			break;
		case 7:
			strDigYomi = _T("�i�i");
			break;
		case 8:
			// ���ꏈ��
			// �S�A��̈ʂ̏ꍇ�͓ǂ݂��ς��
			// ��j88888���u�n�`�}���n�b�Z���n�b�s���N�n�`�W���E�n�`�v
			switch(n){
			case 3:
			case 4:
				strDigYomi = _T("�n�b");
				break;
			default:
				strDigYomi = _T("�n�`");
				break;
			}
			break;
		case 9:
			strDigYomi = _T("�L���E");
			break;
		default:
			break;
		}
		// ��`���̈ʂ̓ǂݐݒ�
		if(0 != digNum){
			switch(n){
			case 0:
				break;
			case 1:
				break;
			case 2:
				strDigYomi += _T("�W���E");
				break;
			case 3:
				// �S�̈ʂ̓��ꏈ��
				// 3,6,8�̏ꍇ�͓ǂ݂�����
				switch(digNum){
				case 3:
					strDigYomi += _T("�r���N");
					break;
				case 6:
				case 8:
					strDigYomi += _T("�s���N");
					break;
				default:
					strDigYomi += _T("�q���N");
					break;
				}
				break;
			case 4:
				// ��̈ʂ̓��ꏈ��
				// 3�̏ꍇ�͓ǂ݂�����
				switch(digNum){
				case 3:
					strDigYomi += _T("�[��");
					break;
				default:
					strDigYomi += _T("�Z��");
					break;
				}
				break;
			case 5:
				strDigYomi += _T("�}��");
				break;
			default:
				// �����ɂ͂��Ȃ��͂��A�A
				throw std::runtime_error("����NG");
				break;
			}
		}
		strYomi += strDigYomi;
	}
	return strYomi;
}

/**
 * @brief ���R�[�h���猧���擾
 * @param lPrefCode [in] ���R�[�h
 * @return ������
 */
inline CString prefCode2PrefName(long lPrefCode)
{
	CString strPrefName;
	switch(lPrefCode){
	case 1:		strPrefName = _T("�k�C��");		break;
	case 2:		strPrefName = _T("�X��");		break;
	case 3:		strPrefName = _T("��茧");		break;
	case 4:		strPrefName = _T("�{�錧");		break;
	case 5:		strPrefName = _T("�H�c��");		break;
	case 6:		strPrefName = _T("�R�`��");		break;
	case 7:		strPrefName = _T("������");		break;
	case 8:		strPrefName = _T("��錧");		break;
	case 9:		strPrefName = _T("�Ȗ،�");		break;
	case 10:	strPrefName = _T("�Q�n��");		break;
	case 11:	strPrefName = _T("��ʌ�");		break;
	case 12:	strPrefName = _T("��t��");		break;
	case 13:	strPrefName = _T("�����s");		break;
	case 14:	strPrefName = _T("�_�ސ쌧");	break;
	case 15:	strPrefName = _T("�V����");		break;
	case 16:	strPrefName = _T("�x�R��");		break;
	case 17:	strPrefName = _T("�ΐ쌧");		break;
	case 18:	strPrefName = _T("���䌧");		break;
	case 19:	strPrefName = _T("�R����");		break;
	case 20:	strPrefName = _T("���쌧");		break;
	case 21:	strPrefName = _T("�򕌌�");		break;
	case 22:	strPrefName = _T("�É���");		break;
	case 23:	strPrefName = _T("���m��");		break;
	case 24:	strPrefName = _T("�O�d��");		break;
	case 25:	strPrefName = _T("���ꌧ");		break;
	case 26:	strPrefName = _T("���s�{");		break;
	case 27:	strPrefName = _T("���{");		break;
	case 28:	strPrefName = _T("���Ɍ�");		break;
	case 29:	strPrefName = _T("�ޗǌ�");		break;
	case 30:	strPrefName = _T("�a�̎R��");	break;
	case 31:	strPrefName = _T("���挧");		break;
	case 32:	strPrefName = _T("������");		break;
	case 33:	strPrefName = _T("���R��");		break;
	case 34:	strPrefName = _T("�L����");		break;
	case 35:	strPrefName = _T("�R����");		break;
	case 36:	strPrefName = _T("������");		break;
	case 37:	strPrefName = _T("���쌧");		break;
	case 38:	strPrefName = _T("���Q��");		break;
	case 39:	strPrefName = _T("���m��");		break;
	case 40:	strPrefName = _T("������");		break;
	case 41:	strPrefName = _T("���ꌧ");		break;
	case 42:	strPrefName = _T("���茧");		break;
	case 43:	strPrefName = _T("�F�{��");		break;
	case 44:	strPrefName = _T("�啪��");		break;
	case 45:	strPrefName = _T("�{�茧");		break;
	case 46:	strPrefName = _T("��������");	break;
	case 47:	strPrefName = _T("���ꌧ");		break;
	default:
		// �����ɂ͂��Ȃ��͂��A�A
		throw std::runtime_error("���R�[�hNG");
		break;
	}
	return strPrefName;
}

/**
 * @brief ���R�[�h���猧�ǂݎ擾
 * @param lPrefCode [in] ���R�[�h
 * @return ���ǂ�
 */
inline CString prefCode2PrefYomi(long lPrefCode)
{
	CString strPrefYomi;
	switch(lPrefCode){
	case 1:		strPrefYomi = _T("�z�b�J�C�h�E");	break;
	case 2:		strPrefYomi = _T("�A�I�����P��");	break;
	case 3:		strPrefYomi = _T("�C���e�P��");		break;
	case 4:		strPrefYomi = _T("�~���M�P��");		break;
	case 5:		strPrefYomi = _T("�A�L�^�P��");		break;
	case 6:		strPrefYomi = _T("���}�K�^�P��");	break;
	case 7:		strPrefYomi = _T("�t�N�V�}�P��");	break;
	case 8:		strPrefYomi = _T("�C�o���L�P��");	break;
	case 9:		strPrefYomi = _T("�g�`�M�P��");		break;
	case 10:	strPrefYomi = _T("�O���}�P��");		break;
	case 11:	strPrefYomi = _T("�T�C�^�}�P��");	break;
	case 12:	strPrefYomi = _T("�`�o�P��");		break;
	case 13:	strPrefYomi = _T("�g�E�L���E�g");	break;
	case 14:	strPrefYomi = _T("�J�i�K���P��");	break;
	case 15:	strPrefYomi = _T("�j�C�K�^�P��");	break;
	case 16:	strPrefYomi = _T("�g���}�P��");		break;
	case 17:	strPrefYomi = _T("�C�V�J���P��");	break;
	case 18:	strPrefYomi = _T("�t�N�C�P��");		break;
	case 19:	strPrefYomi = _T("���}�i�V�P��");	break;
	case 20:	strPrefYomi = _T("�i�K�m�P��");		break;
	case 21:	strPrefYomi = _T("�M�t�P��");		break;
	case 22:	strPrefYomi = _T("�V�Y�I�J�P��");	break;
	case 23:	strPrefYomi = _T("�A�C�`�P��");		break;
	case 24:	strPrefYomi = _T("�~�G�P��");		break;
	case 25:	strPrefYomi = _T("�V�K�P��");		break;
	case 26:	strPrefYomi = _T("�L���E�g�t");		break;
	case 27:	strPrefYomi = _T("�I�I�T�J�t");		break;
	case 28:	strPrefYomi = _T("�q���E�S�P��");	break;
	case 29:	strPrefYomi = _T("�i���P��");		break;
	case 30:	strPrefYomi = _T("���J���}�P��");	break;
	case 31:	strPrefYomi = _T("�g�b�g���P��");	break;
	case 32:	strPrefYomi = _T("�V�}�l�P��");		break;
	case 33:	strPrefYomi = _T("�I�J���}�P��");	break;
	case 34:	strPrefYomi = _T("�q���V�}�P��");	break;
	case 35:	strPrefYomi = _T("���}�O�`�P��");	break;
	case 36:	strPrefYomi = _T("�g�N�V�}�P��");	break;
	case 37:	strPrefYomi = _T("�J�K���P��");		break;
	case 38:	strPrefYomi = _T("�G�q���P��");		break;
	case 39:	strPrefYomi = _T("�R�E�`�P��");		break;
	case 40:	strPrefYomi = _T("�t�N�I�J�P��");	break;
	case 41:	strPrefYomi = _T("�T�K�P��");		break;
	case 42:	strPrefYomi = _T("�i�K�T�L�P��");	break;
	case 43:	strPrefYomi = _T("�N�}���g�P��");	break;
	case 44:	strPrefYomi = _T("�I�I�C�^�P��");	break;
	case 45:	strPrefYomi = _T("�~���U�L�P��");	break;
	case 46:	strPrefYomi = _T("�J�S�V�}�P��");	break;
	case 47:	strPrefYomi = _T("�I�L�i���P��");	break;
	default:
		// �����ɂ͂��Ȃ��͂��A�A
		throw std::runtime_error("���R�[�hNG");
		break;
	}
	return strPrefYomi;
}

/**
 * @brief ���R�[�h����ڔ����i�s���{���j���擾
 * @param lPrefCode [in] ���R�[�h
 * @return �ڔ�������
 */
inline CString prefCode2PrefSuffixName(long lPrefCode)
{
	CString strPrefSuffixName;
	switch(lPrefCode){
	case 1:
		strPrefSuffixName = _T("��");
		break;
	case 13:
		strPrefSuffixName = _T("�s");
		break;
	case 26:
	case 27:
		strPrefSuffixName = _T("�{");
		break;
		break;
	default:
		strPrefSuffixName = _T("��");
		break;
	}
	return strPrefSuffixName;
}

/**
 * @brief ���R�[�h����ڔ����i�s���{���j�ǂݎ擾
 * @param lPrefCode [in] ���R�[�h
 * @return �ڔ����ǂ�
 */
inline CString prefCode2PrefSuffixYomi(long lPrefCode)
{
	CString strPrefSuffixYomi;
	switch(lPrefCode){
	case 1:
		strPrefSuffixYomi = _T("�h�E");
		break;
	case 13:
		strPrefSuffixYomi = _T("�g");
		break;
	case 26:
	case 27:
		strPrefSuffixYomi = _T("�t");
		break;
		break;
	default:
		strPrefSuffixYomi = _T("�P��");
		break;
	}
	return strPrefSuffixYomi;
}
