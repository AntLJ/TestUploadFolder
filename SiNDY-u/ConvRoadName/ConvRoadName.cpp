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
#include "ConvRoadName.h"
#include "ShieldRoadNoCnv.h"
#include "ShieldHwyNoCnv.h"
#include "NationalRoadNoCnv.h"
#include "PrefRoadNoNameCnv.h"
#include "PrefRoadNoCnv.h"
#include "PrefRoadNoAbbrCnv.h"
#include "PrefRoadNameCnv.h"
#include "RoadNameCnv.h"
#include "RoadHwyAbbrCnv.h"
#include "RoadCityHwyAbbrCnv.h"
#include "RoadCityHwyAbbr2Cnv.h"
#include "SectionNameCnv.h"
#include "OutputFile.h"
#include <WinLib/CStringTokenizer.h>

// �������s
bool ConvRoadName::run(const Arguments& args)
{
	CString strMsg;
	// �o�̓N���X������
	OutputFile outputFile(args.m_outDir.c_str());
	if (!outputFile.init())
		return false;
	// �ϊ����e�[�u���擾
	strMsg.Format(_T("[%s] �ϊ����f�[�^�擾"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	if (!getTgtTbl(args))
		return false;
	// �����擾
	if (!getDictionary(args.m_dicPath.c_str()))
		return false;
	// �e�햼�̕ϊ�
	strMsg.Format(_T("[%s] �e�햼�̕ϊ�"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	std::set<RoadNameRecord> rdNameRecs;
	std::set<RelNameLinkRecord> rlNameLnkRecs;
	std::set<SpltNameRecord> spltRdNameRecs;
	if (!cnvEachName(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	// �e�L�X�g�o��
	strMsg.Format(_T("[%s] �t�@�C���o��"), getNowTime());
	Log::GetInstance().PrintLog(true, false, true, false, strMsg);
	if (!outputFile.write(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	return true;
}

// �ϊ����e�[�u���擾
bool ConvRoadName::getTgtTbl(const Arguments& args)
{
	// ���HDB�ڑ�
	m_ipRoadWork = sindy::create_workspace(args.m_roadDB.c_str());
	if (!m_ipRoadWork) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDBConnect, args.m_roadDB.c_str());
		return false;
	}
	// ���HDB�p�̃I�[�i��
	CString roadOwner = args.m_roadOwner.c_str();
	// ���H����DB�ڑ�
	m_ipNameWork = sindy::create_workspace(args.m_nameDB.c_str());
	if (!m_ipNameWork) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDBConnect, args.m_nameDB.c_str());
		return false;
	}
	// ���H����DB�p�̃I�[�i��
	CString nameOwner = args.m_nameOwner.c_str();
	// ���H�����N���C���擾
	CString roadLinkTableName = sindy::schema::road_link::kTableName;
	if (!roadOwner.IsEmpty())
		roadLinkTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::road_link::kTableName);
	if (S_OK != m_ipRoadWork->OpenFeatureClass(CComBSTR(roadLinkTableName), &m_ipRoadLink)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadLinkTableName);
		return false;
	}
	// �H�����̃e�[�u���擾
	CString roadCodeListTableName = sindy::schema::road_code_list::kTableName;
	if (!roadOwner.IsEmpty())
		roadCodeListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::road_code_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(roadCodeListTableName), &m_ipRoadCodeList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadCodeListTableName);
		return false;
	}
	// �������̃e�[�u���擾
	CString prefRoadListTableName = sindy::schema::pref_road_list::kTableName;
	if (!roadOwner.IsEmpty())
		prefRoadListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::pref_road_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(prefRoadListTableName), &m_ipPrefRoadList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, prefRoadListTableName);
		return false;
	}
	// ��Ԗ��̃e�[�u���擾
	CString sectionCodeListTableName = sindy::schema::section_code_list::kTableName;
	if (!roadOwner.IsEmpty())
		sectionCodeListTableName.Format(_T("%s.%s"), roadOwner, sindy::schema::section_code_list::kTableName);
	if (S_OK != m_ipRoadWork->OpenTable(CComBSTR(sectionCodeListTableName), &m_ipSectionCodeList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, sectionCodeListTableName);
		return false;
	}
	// ���H���̃e�[�u���擾
	CString roadNameListTableName = sindy::schema::sj::road_name_list::kTableName;
	if (!nameOwner.IsEmpty())
		roadNameListTableName.Format(_T("%s.%s"), nameOwner, sindy::schema::sj::road_name_list::kTableName);
	if (S_OK != m_ipNameWork->OpenTable(CComBSTR(roadNameListTableName), &m_ipRoadNameList)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgOpenTable, roadNameListTableName);
		return false;
	}
	return true;
}

bool ConvRoadName::getDictionary(const CString& dicPath)
{
	// �R���e�i������
	m_dictionary.clear();
	// �t�@�C���I�[�v��
	std::ifstream dicFile(dicPath);
	if (!dicFile.is_open()) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::FailOpenInputFile, dicPath);
		return false;
	}
	// �����擾
	std::string temp;
	int lineNum = 0;		// �s��(�G���[�o�͗p)
	while (std::getline(dicFile, temp)) {
		// �^�u�ŋ�؂�
		CString record = temp.c_str();
		CStringTokenizer cTokenizer(record, _T("\t"), TOKEN_RET_EMPTY_ALL);
		int fieldCount = cTokenizer.CountTokens();
		if (1 > fieldCount) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryFieldNum, lineNum);
			return false;
		}
		// ������������擾
		CString orgStr = cTokenizer.GetNextToken();
		// �����敶����擾
		std::vector<CString> splitStrs;
		while (cTokenizer.HasMoreTokens()) {
			CString partStr = cTokenizer.GetNextToken();
			if (partStr.IsEmpty() || (partStr == _T(""))) {
				Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryFieldVal, lineNum);
				return false;
			}
			partStr.Trim();
			splitStrs.push_back(partStr);
		}
		// �������ƕ�������Ȃ���ƈ�v���邩�`�F�b�N
		CString connectStr;
		for (const auto partStr : splitStrs) {
			connectStr.AppendFormat(partStr);
		}
		if (0 != connectStr.Compare(orgStr)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgDictionaryRecord, lineNum);
			return false;
		}
		if (!m_dictionary.insert(std::make_pair(orgStr, splitStrs)).second) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::DuplicateDictionary, lineNum);
			return false;
		}
	}
	return true;
}

bool ConvRoadName::cnvEachName(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// �����G���[���ݔ���p�ϐ�
	bool splitResult = true;
	// �W�����H�ԍ�
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �W�����H�ԍ�"));
	ShieldRoadNoCnv shieldRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!shieldRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= shieldRoadNoCnv.GetSplitResult();

	// �W�������ԍ�
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �W�������ԍ�"));
	ShieldHwyNoCnv shieldHwyNoCnv(m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!shieldHwyNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= shieldHwyNoCnv.GetSplitResult();

	// �����ԍ�
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �����ԍ�"));
	NationalRoadNoCnv naRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!naRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= naRoadNoCnv.GetSplitResult();

	// �����ԍ��E����
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �����ԍ��E����"));
	PrefRoadNoNameCnv prefRoadNoNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipPrefRoadList);
	if (!prefRoadNoNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoNameCnv.GetSplitResult();

	// �����ԍ�
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �����ԍ�"));
	PrefRoadNoCnv prefRoadNoCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!prefRoadNoCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoCnv.GetSplitResult();
	
	// �����ԍ�����
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �����ԍ�����"));
	PrefRoadNoAbbrCnv prefRoadNoAbbrCnv(m_ipRoadLink, m_ipRoadNameList);
	if (!prefRoadNoAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNoAbbrCnv.GetSplitResult();

	// ��������
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� ��������"));
	PrefRoadNameCnv prefRoadNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipPrefRoadList);
	if (!prefRoadNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= prefRoadNameCnv.GetSplitResult();

	// �H������
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �H������"));
	RoadNameCnv roadNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadNameCnv.GetSplitResult();

	// �������H����
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �������H����"));
	RoadHwyAbbrCnv roadHwyAbbrCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadHwyAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadHwyAbbrCnv.GetSplitResult();

	// �s�s��������
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �s�s��������"));
	RoadCityHwyAbbrCnv roadCityHwyAbbrCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadCityHwyAbbrCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadCityHwyAbbrCnv.GetSplitResult();
	
	// �s�s��������2
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� �s�s��������2"));
	RoadCityHwyAbbr2Cnv roadCityHwyAbbr2Cnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList);
	if (!roadCityHwyAbbr2Cnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= roadCityHwyAbbr2Cnv.GetSplitResult();

	// ��Ԗ���
	Log::GetInstance().PrintLog(true, false, true, false, _T("\t�� ��Ԗ���"));
	SectionNameCnv sectionNameCnv(m_dictionary, m_ipRoadLink, m_ipRoadNameList, m_ipRoadCodeList, m_ipSectionCodeList);
	if (!sectionNameCnv.cnv(rdNameRecs, rlNameLnkRecs, spltRdNameRecs))
		return false;
	splitResult &= sectionNameCnv.GetSplitResult();

	return splitResult;
}