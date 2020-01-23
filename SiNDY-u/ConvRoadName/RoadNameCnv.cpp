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
#include "RoadNameCnv.h"

using namespace sindy::schema;

bool RoadNameCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kNameKanji), &m_roadCodeListNameKanjiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayKanji);
		return false;
	}
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kNameYomi), &m_roadCodeListNameYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kDisplayYomi);
		return false;
	}
	return SplitBasicTokenRuleCnv::setSubTblFieldIndex();
}

bool RoadNameCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// �֘A�t�����H�H�����X�g�擾
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;
	if (!ipCode) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	// ���H�H�����X�g����LINE_CLASS_C, NAME_KANJI, NAME_YOMI���擾���A���̃f�[�^����
	CComVariant roadCodeListLineClsVal, roadCodeListNameKanjiVal, roadCodeListNameYomiVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListLineClsIndex, &roadCodeListLineClsVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kLineClass);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListNameKanjiIndex, &roadCodeListNameKanjiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kNameKanji);
		return false;
	}
	if (S_OK != ipCode->get_Value(m_roadCodeListNameYomiIndex, &roadCodeListNameYomiVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kNameYomi);
		return false;
	}
	// ���́A�ǂ݂̐ݒ�
	CString roadCodeListNameKanji = roadCodeListNameKanjiVal.bstrVal,
		roadCodeListNameYomi = roadCodeListNameYomiVal.bstrVal;
	if ((::road_code_list::line_class::kFerry != roadCodeListLineClsVal.lVal) &&
		(::road_code_list::line_class::kNormal != roadCodeListLineClsVal.lVal)) {
		// �����̐����ƁA���ʁ{���ʓ��������폜
		roadCodeListNameKanji = roadCodeListNameKanji.TrimRight(_T("�O�P�Q�R�S�T�U�V�W�X"));
		roadCodeListNameYomi = roadCodeListNameYomi.TrimRight(_T("�O�P�Q�R�S�T�U�V�W�X"));
		roadCodeListNameKanji = removeBracketedPhrase(roadCodeListNameKanji);
		roadCodeListNameYomi = removeBracketedPhrase(roadCodeListNameYomi);
	}
	RoadNameKey roadNameKey(roadCodeListNameKanji, roadCodeListNameYomi);
	std::map<RoadNameKey, RoadNameRecord>::const_iterator roadNameItr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == roadNameItr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_code_list::kTableName, codeOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = roadNameItr->second;
	return true;
}


CString RoadNameCnv::removeBracketedPhrase(CString strRoadName)
{
	// ���ʂ͐������ݒ肳��Ă���O��̏���
	// ���W�b�N�F
	//    1.�擪����u�j�v��T��
	//    2.1�Ō��������ʒu����t�����Ɂu�i�v��T��
	//    3.1��2�̊Ԃ��폜
	//    4.���ʂ��Ȃ��Ȃ�܂ŌJ��Ԃ�
	while (1) {
		int iEnd = strRoadName.Find(_T("�j"));
		if (iEnd < 0) {
			// ���ʂ��Ȃ���ΏI��
			break;
		}
		CString strRoadNameTemp = strRoadName.Left(iEnd);
		int iStart = strRoadNameTemp.ReverseFind(_T('�i'));
		if (iStart < 0) {
			// ���ʂ��Ȃ���ΏI��
			break;
		}
		// ���ʓ�������폜�i���ʂ��܂ށj
		strRoadName.Delete(iStart, iEnd - iStart + 1);
	}
	return strRoadName;
}