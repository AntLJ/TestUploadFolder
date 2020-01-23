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
#include "Param.h"

CParam::CParam(void) {
}

CParam::~CParam(void) {
}

bool CParam::checkParam(int argc, _TCHAR* argv[]) {
	po::options_description opt("�I�v�V����");

	// �������`
	opt.add_options()
		("help,h", "�w���v�\��")
		(OPT_INI_FILE, uh::tvalue<uh::tstring>(&m_tstrIniFile), "[�K�{]�ݒ�t�@�C��")
		(OPT_RUN_LOG,  uh::tvalue<uh::tstring>(&m_tstrRunLog),	"[�I��]���s���O�t�@�C��")
		(OPT_ERR_LOG,  uh::tvalue<uh::tstring>(&m_tstrErrLog),	"[�I��]�G���[���O�t�@�C��");

	// �I�v�V�����̒l��ǂݍ��ޕϐ�
	po::variables_map vmOpt;

	// �R�}���h���C���������p�[�X�������ʂ� variable_maps�Ɋi�[
	po::store(po::parse_command_line(argc, argv, opt), vmOpt);
	po::notify(vmOpt);

	// �K�{�p�����[�^�`�F�b�N
	if (vmOpt.count("help,h") ||
		!vmOpt.count(g_strIniFile)) {
		std::cerr << opt << std::endl;
		return false;
	}

	// �ݒ�t�@�C�����݃`�F�b�N
	if (!PathFileExists(CA2CT(getIniFile().c_str()))) {
		std::cerr << LOG_ERROR << "\t" << OPT_INI_FILE << " �I�v�V�����s���i�t�@�C�����݂Ȃ��j" << std::endl;
		return false;
	}

	// �ݒ�t�@�C���`�F�b�N
	if (!checkIni()) {
		return false;
	}

	return true;
}

bool CParam::checkIni() {
	bool bRet = true;
	std::string sMsg;

	po::options_description ini("�ݒ�t�@�C������");

	CString csDiscDistance;
	csDiscDistance.Format(_T("[�I��]�����`�F�b�N�p臒l (default:%s)"), _T(DISTANCE_DEF));

	ini.add_options()
		(INI_REL_TBL,	po::value<std::string>(&m_strRelTbl),		"[�K�{]�`�F�b�N�Ώۊ֘A�e�[�u��")
		(INI_WN_DIR,	po::value<std::string>(&m_strWnDir),		"[�K�{]���H�E���s�҃m�[�h�i�[Dir")
		(INI_POI_LIST,	po::value<std::string>(&m_strPoiList),		"[�K�{]POI�t�@�C�����X�g")
		(INI_MESHLIST,	po::value<std::string>(&m_strMeshList),		"[�K�{]�`�F�b�N�Ώۃ��b�V�����X�g")
		(INI_CODE_LIST,	po::value<std::string>(&m_strCodeList),		"[�K�{]�`�F�b�N�ΏۃR���e���c�R�[�h���X�g")
		(INI_NODECLASS,	po::value<std::string>(&m_strNodeClass),	"[�K�{]�`�F�b�N�Ώە��s�҃m�[�h���")
		(INI_DISTANCE,	po::value<std::string>(&m_strDistance),		CT2CA(csDiscDistance))
		(INI_PARAM,		po::value<std::string>(&m_strParam),		"[�K�{]���E���n�n�p�����[�^�t�@�C��");

	// �ݒ荀�ڂ̒l��ǂݍ��ޕϐ�
	po::variables_map vmIni;

	// �O���t�@�C������ݒ荀�ړǂݍ���
	std::ifstream iniFile(getIniFile().c_str());
	po::store(po::parse_config_file(iniFile, ini), vmIni);
	po::notify(vmIni);

	// �K�{���ڃ`�F�b�N
	if (!vmIni.count(INI_REL_TBL) ||
		!vmIni.count(INI_WN_DIR) ||
		!vmIni.count(INI_POI_LIST) ||
		!vmIni.count(INI_MESHLIST) ||
		!vmIni.count(INI_CODE_LIST) ||
		!vmIni.count(INI_NODECLASS) ||
		!vmIni.count(INI_PARAM)) {
		// �ݒ荀�ڐ����\������"--"��"arg"������
		std::stringstream	ini_tmp;
		ini_tmp << ini << std::endl;
		auto ini_kai = boost::algorithm::replace_all_copy(ini_tmp.str(), " arg ", "     ");
		ini_kai = boost::algorithm::replace_all_copy(ini_kai, "--", "");
		std::cerr << ini_kai << std::endl;
		return false;
	}

	// �֘A�e�[�u���ݒ�`�F�b�N
	if (m_strRelTbl.empty()) {
		outputCerrItem(LOG_ERROR, INI_REL_TBL, "���ݒ�");
		bRet = false;
	}
	// �֘A�e�[�u���t�@�C�����݃`�F�b�N
	else if (!PathFileExists(CA2CT(getRelTbl().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_REL_TBL, "�t�@�C�����݂Ȃ�");
		bRet = false;
	}

	// ���H�E���s�҃m�[�h�i�[Dir�ݒ�`�F�b�N
	if (m_strWnDir.empty()) {
		outputCerrItem(LOG_ERROR, INI_WN_DIR, "���ݒ�");
		bRet = false;
	}
	// ���H�E���s�҃m�[�h�i�[Dir���݃`�F�b�N
	else if (!PathIsDirectory(CA2CT(getWnDir().c_str()))) {
		sMsg = "Dir���݂Ȃ�";
		outputCerrItem(LOG_ERROR, INI_WN_DIR, "Dir���݂Ȃ�");
		bRet = false;
	}

	// POI�t�@�C�����X�g�ݒ�`�F�b�N
	if (m_strPoiList.empty()) {
		outputCerrItem(LOG_ERROR, INI_POI_LIST, "���ݒ�");
		bRet = false;
	}
	// POI�t�@�C�����X�g���݃`�F�b�N
	else if (!PathFileExists(CA2CT(getPoiList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_POI_LIST, "�t�@�C�����݂Ȃ�");
		bRet = false;
	}

	// ���b�V�����X�g�ݒ�`�F�b�N
	if (m_strMeshList.empty()) {
		outputCerrItem(LOG_ERROR, INI_MESHLIST, "���ݒ�");
		bRet = false;
	}
	// ���b�V�����X�g���݃`�F�b�N
	else if (!PathFileExists(CA2CT(getMeshList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_MESHLIST, "�t�@�C�����݂Ȃ�");
		bRet = false;
	}

	// �R���e���c�R�[�h���X�g���݃`�F�b�N
	if (m_strCodeList.empty()) {
		outputCerrItem(LOG_ERROR, INI_CODE_LIST, "���ݒ�");
		bRet = false;
	}
	// �R���e���c�R�[�h���X�g���݃`�F�b�N
	else if (!PathFileExists(CA2CT(getCodeList().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_CODE_LIST, "�t�@�C�����݂Ȃ�");
		bRet = false;
	}

	// ���s�҃m�[�h��ʐݒ�`�F�b�N
	if (m_strNodeClass.empty()) {
		outputCerrItem(LOG_ERROR, INI_NODECLASS, "���ݒ�");
		bRet = false;
	}
	// ���s�҃m�[�h��ʐ��l�`�F�b�N
	else {
		CString csWalkNodeKind(getNodeClass().c_str());
		if (csWalkNodeKind != csWalkNodeKind.SpanIncluding(CHECK_INTEGER_STR)) {
			outputCerrItem(LOG_ERROR, INI_NODECLASS, "���l�ȊO");
			bRet = false;
		}
	}

	// �����`�F�b�N�p臒l�ݒ�`�F�b�N
	if (m_strDistance.empty()) {
		// 臒l�f�t�H���g�l�ݒ�
		m_strDistance = DISTANCE_DEF;
	}
	CString csDistance(getDistance().c_str());
	// �����`�F�b�N�p臒l���l�`�F�b�N
	if (csDistance != csDistance.SpanIncluding(CHECK_DOUBLE_STR)) {
		outputCerrItem(LOG_ERROR, INI_DISTANCE, "���l�ȊO");
		bRet = false;
	}

	// ���E���n�n�p�����[�^�t�@�C���ݒ�`�F�b�N
	if (m_strParam.empty()) {
		outputCerrItem(LOG_ERROR, INI_PARAM, "���ݒ�");
		bRet = false;
	}
	// ���E���n�n�p�����[�^�t�@�C�����݃`�F�b�N
	else if (!PathFileExists(CA2CT(getParam().c_str()))) {
		outputCerrItem(LOG_ERROR, INI_PARAM, "�t�@�C�����݂Ȃ�");
		bRet = false;
	}

	return bRet;
}

std::string CParam::getOption() {
	std::string strOption = "  option\n";
	strOption.append(getOptionLine(g_strIniFile, uh::toStr(m_tstrIniFile)));

	if (!m_tstrRunLog.empty()) {
		strOption.append(getOptionLine(g_strRunLog, uh::toStr(m_tstrRunLog)));
	}

	if (!m_tstrErrLog.empty()) {
		strOption.append(getOptionLine(g_strErrLog, uh::toStr(m_tstrErrLog)));
	}
	
	strOption.append("\n");
	// ini�t�@�C���ݒ���e�o��
	strOption.append(getIniItem());

	return strOption;
}

std::string CParam::getOptionLine(const char* const cParam, std::string strValue) {
	return (boost::format("%4s--%-10s %s\n") % " " % cParam % strValue).str();
}

std::string CParam::getIniItem() {
	std::string strItem = "  iniFile Item\n";
	strItem.append(getIniItemLine(INI_REL_TBL, uh::toStr(m_strRelTbl)));
	strItem.append(getIniItemLine(INI_WN_DIR, uh::toStr(m_strWnDir)));
	strItem.append(getIniItemLine(INI_POI_LIST, uh::toStr(m_strPoiList)));
	strItem.append(getIniItemLine(INI_MESHLIST, uh::toStr(m_strMeshList)));
	strItem.append(getIniItemLine(INI_CODE_LIST, uh::toStr(m_strCodeList)));
	strItem.append(getIniItemLine(INI_NODECLASS, uh::toStr(m_strNodeClass)));
	strItem.append(getIniItemLine(INI_DISTANCE, uh::toStr(m_strDistance)));
	strItem.append(getIniItemLine(INI_PARAM, uh::toStr(m_strParam)));

	return strItem;
}

std::string CParam::getIniItemLine(const char* const cParam, std::string strValue) {
	return (boost::format("%4s%-20s = %s\n") % " " % cParam % strValue).str();
}

void CParam::outputCerrItem(std::string errKind, std::string itemName, std::string msg) {
	std::cerr << errKind <<  "\t" << itemName << " ���ڕs���i" << msg << "�j" << std::endl;
}
