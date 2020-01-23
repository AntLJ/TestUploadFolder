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
#include "SplitBasicTokenRuleCnv.h"
#include <cwctype>

using namespace sindy::schema;

bool SplitBasicTokenRuleCnv::setRoadLinkFieldIndex()
{
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadCode), &m_roadLinkRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadCode);
		return false;
	}
	return true;
}

bool SplitBasicTokenRuleCnv::setSubTblFieldIndex()
{
	
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kRoadCode), &m_roadCodeListRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kRoadCode);
		return false;
	}
	if (S_OK != m_ipRoadCodeList->FindField(CComBSTR(::road_code_list::kLineClass), &m_roadCodeListLineClsIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kLineClass);
		return false;
	}
	return true;
}

bool SplitBasicTokenRuleCnv::loadSubTable()
{
	// �S�Ẵ��R�[�h���擾���邪�A���v���J�ł������悤IQueryPtr���`(��������IQueryPtr��^���Ȃ��ƃ��v���J�œ��삵�Ȃ�)
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// �J�[�\���擾
	_ICursorPtr ipCodeListCursor;
	if (S_OK != m_ipRoadCodeList->Search(ipQFilter, VARIANT_FALSE, &ipCodeListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::road_code_list::kTableName);

	}
	// �ϊ��Ώۓ��H�H���R�[�h���R�[�h�擾
	_IRowPtr ipCode;
	while (S_OK == ipCodeListCursor->NextRow(&ipCode) && ipCode) {
		// OBJECTID�擾
		long codeOID = -1;
		if (S_OK != ipCode->get_OID(&codeOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::road_code_list::kTableName);
			return false;
		}
		// �H���R�[�h�擾
		CComVariant codeVal;
		if (S_OK != ipCode->get_Value(m_roadCodeListRoadCodeIndex, &codeVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kRoadCode);
			return false;
		}
		if (!m_tgtRdCodeListRecs.insert(std::make_pair(codeVal.lVal, ipCode)).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, ::road_code_list::kTableName, codeOID);
			return false;
		}
	}
	return true;
}

bool SplitBasicTokenRuleCnv::cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// �����ɓo�^����Ă��閼�̂̏ꍇ�A�����ɏ]���ēo�^���Ċ���
	if (chkDictionary(roadNameRecord, spltRdNameRecs))
		return true;
	
	// ���H���̃f�[�^�̖��́E�ǂ݂𕪊����ēo�^
	// ���̕����p�ɑΏۓ��H�����N�̘H����ʎ擾
	long lineCls = -1;
	if (!getLineCls(linkOID, ipLink, lineCls))
		return false;
	// �����񕪊�
	std::list<SplitElement> mainNameSpltElem, mainYomiSpltElem;
	basicTokenRuleCnv(roadNameRecord.m_nameID, ::sj::road_name_list::kTableName, lineCls, roadNameRecord.m_name, roadNameRecord.m_yomi, mainNameSpltElem, mainYomiSpltElem);
	// �o�͗p�R���e�i�ɓo�^
	long seq = 1;
	std::list<SplitElement>::const_iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// ���́E�ǂ݂�NULL�̏ꍇ�͔�΂�
		if (nameItr->m_str.IsEmpty() || yomiItr->m_str.IsEmpty()) {
			if (!(nameItr->m_str.IsEmpty() && yomiItr->m_str.IsEmpty())) {
				Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::UnsymmetricalWord, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			}
			++nameItr;
			++yomiItr;
			continue;
		}
		SpltNameRecord spltNameRecord(roadNameRecord.m_nameID, nameItr->m_class, seq, nameItr->m_str, yomiItr->m_str);
		if (!spltRdNameRecs.insert(spltNameRecord).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			return false;
		}
		++seq;
		++nameItr;
		++yomiItr;
	} 
	// �ǂ��炩�̃C�e���[�^��end()���w���Ă��Ȃ��ꍇ�́A���������Ⴄ�̂Ń��b�Z�[�W�o��
	if ((mainNameSpltElem.end() != nameItr) || (mainYomiSpltElem.end() != yomiItr)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		m_isErr = true;
	}
	
	return true;
}

bool SplitBasicTokenRuleCnv::chkDictionary(const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	bool existDic = false;
	std::map<CString, std::vector<CString>>::const_iterator nameItr = m_dictionary.find(roadNameRecord.m_name),
		yomiItr = m_dictionary.find(roadNameRecord.m_yomi);
	long seq = 1;
	if ((m_dictionary.end() != nameItr) && (m_dictionary.end() != yomiItr)) {
		std::vector<CString>::const_iterator splitNameItr = nameItr->second.begin(),
			splitYomiItr = yomiItr->second.begin();
		long seq = 1;
		while ((splitNameItr != nameItr->second.end()) || (splitYomiItr != yomiItr->second.end())) {
			split_class::ECode spltCls = getSplitClass(*splitNameItr);
			SpltNameRecord spltNameRecord(roadNameRecord.m_nameID, spltCls, seq, *splitNameItr, *splitYomiItr);
			spltRdNameRecs.insert(spltNameRecord);
			++seq;
			++splitNameItr;
			++splitYomiItr;
		}
		if (splitNameItr != nameItr->second.end() || splitYomiItr != yomiItr->second.end()) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
			m_isErr = true;
		}
		return true;
	}
	return false;
}

bool SplitBasicTokenRuleCnv::getLineCls(const long& linkOID, const IFeaturePtr& ipLink, long& lineCls)
{
	long codeOID = -1;
	_IRowPtr ipCode;
	if (!getLinkRelRoadCode(linkOID, ipLink, codeOID, ipCode))
		return false;

	if (!ipCode) {
		// �֘A�t��road_code_list�������ꍇ�́AlineCls��-1�ɂ���B
		lineCls = -1;
		return true;
	}
	// �H����ʎ擾
	CComVariant lineClsVal;
	if (S_OK != ipCode->get_Value(m_roadCodeListLineClsIndex, &lineClsVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, codeOID, ::road_code_list::kLineClass);
		return false;
	}
	lineCls = lineClsVal.lVal;
	return true;
}

void SplitBasicTokenRuleCnv::basicTokenRuleCnv(const long& tgtID,
	const CString& tgtTblName,
	const long& lineCls,
	const CString& nameStr,
	const CString& yomiStr,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	// ���������̂��߂ɖ��́E�ǂ݂�mainNameSpltElem, mainYomiSpltElem�ɒǉ�����B
	CStringW nameStrW = CT2CW(nameStr), yomiStrW = CT2CW(yomiStr);
	SplitElement nameElem(nameStrW, split_class::kBody, false), yomiElem(yomiStrW, split_class::kBody, false);
	mainNameSpltElem.push_back(nameElem);
	mainYomiSpltElem.push_back(yomiElem);
	// ���́E�ǂ݁u�i�j�v����
	splitNameYomi(split_func::kParen, tgtID, tgtTblName, L"", L"", mainNameSpltElem, mainYomiSpltElem);

	// ���́E�ǂ݁u�`�v����
	splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"�`", L"�`", mainNameSpltElem, mainYomiSpltElem);

	// �H����ʂ�1,2,3,4,5,6,7,8,11�̏ꍇ�݂̂̕���
	if (((::road_code_list::line_class::kHighway <= lineCls) && (::road_code_list::line_class::kHiroshima >= lineCls)) ||
		(::road_code_list::line_class::kTF == lineCls)) {
		// ���́u�����ԓ��v�ǂ݁u�W�h�E�V���h�E�v�ŕ���
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"�����ԓ�", L"�W�h�E�V���h�E", mainNameSpltElem, mainYomiSpltElem);
		// ���́u�L�����H�v�ǂ݁u���E�����E�h�E���v�ŕ���
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"�L�����H", L"���E�����E�h�E��", mainNameSpltElem, mainYomiSpltElem);
		// ���́u���H�v�ǂ݁u�h�E���v�ŕ���
		splitNameYomi(split_func::kNormal, tgtID, tgtTblName, L"���H", L"�h�E��", mainNameSpltElem, mainYomiSpltElem);
	}

	// ���́u�~�~�����v�ǂ݁u�~�~�R�E�\�N�v����
	splitNameYomi(split_func::kHwy, tgtID, tgtTblName, L"����", L"�R�E�\�N", mainNameSpltElem, mainYomiSpltElem);
	// ���́u�~�~�����v�ǂ݁u�~�~�S�E�Z���v����
	long preRoadNoSpltCount = mainNameSpltElem.size();
	splitNameYomiByRoadNo(tgtID, tgtTblName, L"����", L"�S�E�Z��", mainNameSpltElem, mainYomiSpltElem);
	// �u�����v�u�S�E�Z���v�ɂ�镪�����s���Ȃ������ꍇ�̕���
	if (mainNameSpltElem.size() == preRoadNoSpltCount) {
		splitNameYomiByRoadNo(tgtID, tgtTblName, L"��", L"�S�E", mainNameSpltElem, mainYomiSpltElem);
	}
	return;
}

bool SplitBasicTokenRuleCnv::getLinkRelRoadCode(const long& linkOID, const IFeaturePtr& ipLink, long& codeOID, _IRowPtr& ipCode)
{
	// �����N�̘H���R�[�h�擾���Aroad_code_list����
	CComVariant linkRoadCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadCodeIndex, &linkRoadCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadCode);
		return false;
	}
	long linkRoadCode = linkRoadCodeVal.lVal;
	// 40���ԑ�ȏ�̏ꍇ�A�n��� or ������Ԃł���A�����N����RoadCode�͒l���ǉ�����Ă���̂ŁA������l������B
	if (CROSS_OVER_CODE_LOW <= linkRoadCode) {
		// 40���ԑ� or 50���ԑ�̏ꍇ�́A�n����ł���A20�������K�v����
		if (CROSS_OVER_CODE_HIG >= linkRoadCode) {
			linkRoadCode = linkRoadCode - CROSS_OVER_PLUS_VALUE;
		}
		else if ((FREE_HWY_CODE_LOW <= linkRoadCode) && (FREE_HWY_CODE_HIG >= linkRoadCode)) {
			linkRoadCode = linkRoadCode - FREE_HWY_PLUS_VALUE;
		}
	}
	std::map<long, _IRowPtr>::const_iterator itr = m_tgtRdCodeListRecs.find(linkRoadCode);
	if (itr == m_tgtRdCodeListRecs.end()) {
		// �擾�ł��Ȃ��ꍇ������̂ŁA�G���[�ɂ͂��Ȃ�
		ipCode = nullptr;
		codeOID = -1;
		return true;
	}
	ipCode = itr->second;
	if (S_OK != ipCode->get_OID(&codeOID)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName);
		return false;
	}
	return true;
}

split_class::ECode SplitBasicTokenRuleCnv::getSplitClass(const CString& splitStr)
{
	if (_T("�`") == splitStr) return split_class::kConnector;
	if (_T("�����ԓ�") == splitStr || _T("�W�h�E�V���h�E") == splitStr ||
		_T("�L�����H") == splitStr || _T("���E�����E�h�E��") == splitStr ||
		_T("���H") == splitStr || _T("�h�E��") == splitStr)
		return split_class::kClassifier;
	// ��L�ȊO��Body
	return split_class::kBody;
}

void SplitBasicTokenRuleCnv::splitNameYomi(const split_func::ECode& funcCode,
	const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltNameWord,
	const CStringW& spltYomiWord,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	std::list<SplitElement>::iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// ���́E�ǂ݂�()���������ꂳ��Ă��邱�Ƃ��`�F�b�N
		if ((nameItr->m_isParen && !yomiItr->m_isParen) ||
			(!nameItr->m_isParen && yomiItr->m_isParen)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgParenConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ()�t���̕�����́A�����ΏۊO
		if (nameItr->m_isParen && yomiItr->m_isParen) {
			++nameItr;
			++yomiItr;
		}
		// �������������ꂳ��Ă��邱�Ƃ��`�F�b�N
		if (nameItr->m_class != yomiItr->m_class) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgSpClsConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ����������Body�ȊO�́A�����ΏۊO
		if ((nameItr->m_class != split_class::kBody) && (yomiItr->m_class != split_class::kBody)) {
			++nameItr;
			++yomiItr;
		}

		std::vector<SplitElement> nameSpltElem, yomiSpltElem;
		// ���̕�������
		splitStr(funcCode, tgtID, tgtTblName, spltNameWord, *nameItr, nameSpltElem);
		if (1 < nameSpltElem.size()) {
			// �ǂݕ�������
			splitStr(funcCode, tgtID, tgtTblName, spltYomiWord, *yomiItr, yomiSpltElem);
		} else {
			// ���̑��ŕ���������Ȃ��ꍇ�́A�ǂ݂��������Ȃ�
			yomiSpltElem.push_back(*yomiItr);
		}
		// ���̂Ɠǂ݂ŕ��������Ⴄ�ꍇ�́A���b�Z�[�W���o�͂���.
		if (nameSpltElem.size() != yomiSpltElem.size()) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DiffSplitCount, tgtTblName, tgtID);
			m_isErr = true;
		}
		// �������ʂ�S�Ă̕������ʂɃ}�[�W
		mergeElementsToMain(nameSpltElem, nameItr, mainNameSpltElem);
		mergeElementsToMain(yomiSpltElem, yomiItr, mainYomiSpltElem);
	}
	// ������������Ă��Ȃ����̃`�F�b�N�͍Ō�ɂ��̂ŁA�����ł͂��Ȃ��B
	return;
}

void SplitBasicTokenRuleCnv::splitNameYomiByRoadNo(const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltBaseNameWord,
	const CStringW& spltBaseYomiWord,
	std::list<SplitElement>& mainNameSpltElem,
	std::list<SplitElement>& mainYomiSpltElem)
{
	std::list<SplitElement>::iterator nameItr = mainNameSpltElem.begin(),
		yomiItr = mainYomiSpltElem.begin();
	while ((mainNameSpltElem.end() != nameItr) && (mainYomiSpltElem.end() != yomiItr)) {
		// ���́E�ǂ݂�()���������ꂳ��Ă��邱�Ƃ��`�F�b�N
		if ((nameItr->m_isParen && !yomiItr->m_isParen) ||
			(!nameItr->m_isParen && yomiItr->m_isParen)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgParenConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ()�t���̕�����́A�����ΏۊO
		if (nameItr->m_isParen && yomiItr->m_isParen) {
			++nameItr;
			++yomiItr;
		}
		// �������������ꂳ��Ă��邱�Ƃ��`�F�b�N
		if (nameItr->m_class != yomiItr->m_class) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgSpClsConsis, tgtTblName, tgtID);
			m_isErr = true;
		}
		// ����������Body�ȊO�́A�����ΏۊO
		if ((nameItr->m_class != split_class::kBody) && (yomiItr->m_class != split_class::kBody)) {
			++nameItr;
			++yomiItr;
		}
		std::vector<SplitElement> nameSpltElem, yomiSpltElem;
		// ���̕�������
		std::vector<CStringW> spltNo;
		splitStrByRoadNo(spltBaseNameWord, *nameItr, spltNo, nameSpltElem);
		if (1 < nameSpltElem.size()) {
			// spltNo�̊e�v�f�̐�����ǂ݂ɂ������� �{ spltBaseYomiWord�Œʏ�̕��������{
			for (const auto strNo : spltNo) {
				long no = _wtol(strNo);
				CString spltYomiWord;
				spltYomiWord.Format(_T("%s%s"), num2YomiStr(no), spltBaseYomiWord);
				splitStrNormal(spltYomiWord, *yomiItr, yomiSpltElem);
			}
		} else {
			// ���̑��ŕ���������Ȃ��ꍇ�́A�ǂ݂��������Ȃ�
			yomiSpltElem.push_back(*yomiItr);
		}
		// �������ʂ�S�Ă̕������ʂɃ}�[�W
		mergeElementsToMain(nameSpltElem, nameItr, mainNameSpltElem);
		mergeElementsToMain(yomiSpltElem, yomiItr, mainYomiSpltElem);
	}
	return;
}

void SplitBasicTokenRuleCnv::splitStr(const split_func::ECode& funcCode, 
	const long& tgtID,
	const CString& tgtTblName,
	const CStringW& spltWord,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	switch (funcCode) {
	case split_func::kNormal:
		splitStrNormal(spltWord, tgtElem, spltElem);
		break;
	case split_func::kParen:
		splitStrByParen(tgtID, tgtTblName, tgtElem, spltElem);
		break;
	case split_func::kHwy:
		splitStrByHwy(spltWord, tgtElem, spltElem);
		break;
	default:
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgSplitFunc);
		m_isErr = true;
	}
	return;
}

void SplitBasicTokenRuleCnv::splitStrNormal(const CStringW& spltWord,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltWord);
	if (-1 == spltPos) {
		// ���������񂪑��݂��Ȃ��ꍇ�́Avector�ɑ}�����Ċ���
		spltElem.push_back(tgtElem);
		return;
	}
	// �����񕪊����{
	CStringW leftStr, midStr, rightStr;
	leftStr = tgtElem.m_str.Left(spltPos);
	midStr = tgtElem.m_str.Mid(spltPos, spltWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltWord.GetLength());
	// vector�ւ̓o�^
	SplitElement leftElem(leftStr, split_class::kBody, false),
		midElem(midStr, getSplitClass(midStr), false), rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	spltElem.push_back(midElem);
	// rightElem�̂݁A���������񂪑��݂���\��������̂ŁA�ċA����
	splitStrNormal(spltWord, rightElem, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::splitStrByParen(const long& tgtID,
	const CString& tgtTblName,
	const SplitElement& tgtElem,
	std::vector<SplitElement>& spltElem)
{
	long frontParenPos = tgtElem.m_str.Find(L"�i");
	if (-1 == frontParenPos) {
		// "("�����݂��Ȃ��ꍇ�́Avector�ɑ}�����Ċ���
		spltElem.push_back(tgtElem);
		return;
	}
	long frontParenNum = 0, behindParenNum = 0;
	long behindParenPos = 0;
	long tgtStrLen = tgtElem.m_str.GetLength();
	for (long i = 0; i < tgtStrLen; ++i) {
		CStringW temp = tgtElem.m_str[i];
		if (L"�i" == temp)
			++frontParenNum;
		if (L"�j" == temp)
			++behindParenNum;
		// ()������܂�continue
		if ((0 == frontParenNum) || (frontParenNum != behindParenNum))
			continue;
		// ��������
		behindParenPos = i;
		CStringW leftStr, midStr, rightStr;
		leftStr = tgtElem.m_str.Left(frontParenPos);
		midStr = tgtElem.m_str.Mid(frontParenPos, behindParenPos - frontParenPos + 1);
		rightStr = tgtElem.m_str.Mid(behindParenPos + 1);
		// vector�ւ̓o�^
		SplitElement leftElem(leftStr, split_class::kBody, false),
			midElem(midStr, split_class::kBody, true), rightElem(rightStr, split_class::kBody, false);
		spltElem.push_back(leftElem);
		spltElem.push_back(midElem);
		// rightElem�̂�()�����݂���\��������̂ŁA�ċA����
		splitStrByParen(tgtID, tgtTblName, rightElem, spltElem);
		return;
	}
	// �����܂ŗ����ꍇ�́A���������Ȃ̂�("�i"�̂ݑ��݂��邱�ƂȂ�)�Avector�ɂ��̂܂ܑ}��(error�͏o��)
	spltElem.push_back(tgtElem);
	Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::UnsymmetricalParen, tgtTblName, tgtID);
	m_isErr = true;
	return;
}

void SplitBasicTokenRuleCnv::splitStrByHwy(const CStringW& spltWord, const SplitElement& tgtElem, std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltWord);
	if (-1 == spltPos) {
		// ���������񂪑��݂��Ȃ��ꍇ�́Avector�ɑ}�����Ċ���
		spltElem.push_back(tgtElem);
		return;
	}
	// �����񕪊������{
	// �������������荶�ƉE�ɕ���
	CStringW leftStr, rightStr;
	leftStr = tgtElem.m_str.Left(spltPos + spltWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltWord.GetLength());
	// vector�ւ̓o�^
	SplitElement leftElem(leftStr, split_class::kBody, false),
		rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	// rightelem�͂܂������ł���\��������̂ŁA�ċA����
	splitStrByHwy(spltWord, rightElem, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::splitStrByRoadNo(const CStringW& spltBaseWord, const SplitElement& tgtElem, std::vector<CString>& spltNo, std::vector<SplitElement>& spltElem)
{
	long spltPos = tgtElem.m_str.Find(spltBaseWord);
	if (-1 == spltPos) {
		// ���������񂪑��݂��Ȃ��ꍇ�́Avector�ɑ}�����Ċ���
		spltElem.push_back(tgtElem);
		return;
	}
	// ���������̑O�̐��������`�F�b�N
	long numberStartPos = -1;
	bool isPreNumber = false;
	for (long pos = 0; pos < spltPos; ++pos) {
		wchar_t oneWord = tgtElem.m_str[pos];
		if (std::iswdigit(oneWord)) {
			if (!isPreNumber)
				numberStartPos = pos;
			isPreNumber = true;
		}
		else {
			isPreNumber = false;
		}
	}
	// �ŏI�I��isPreNumber��false�̏ꍇ�́Avector�ɑ}�����Ċ���
	if (!isPreNumber) {
		spltElem.push_back(tgtElem);
		return;
	}
	// �����񕪊�
	CStringW leftStr, midStr, rightStr;
	leftStr = tgtElem.m_str.Left(numberStartPos);
	midStr = tgtElem.m_str.Mid(numberStartPos, spltPos - numberStartPos + spltBaseWord.GetLength());
	rightStr = tgtElem.m_str.Mid(spltPos + spltBaseWord.GetLength());
	// midStr�̐���������o�^
	CStringW numStr = tgtElem.m_str.Mid(numberStartPos, spltPos - numberStartPos);
	spltNo.push_back(numStr);
	// vector�ւ̓o�^
	SplitElement leftElem(leftStr, split_class::kBody, false),
		midElem(midStr, split_class::kBody, false), rightElem(rightStr, split_class::kBody, false);
	spltElem.push_back(leftElem);
	spltElem.push_back(midElem);
	// rightElem�̂݁A���������񂪑��݂���\��������̂ŁA�ċA����
	splitStrByRoadNo(spltBaseWord, rightElem, spltNo, spltElem);
	return;
}

void SplitBasicTokenRuleCnv::mergeElementsToMain(const std::vector<SplitElement>& spltElem, std::list<SplitElement>::iterator& itr, std::list<SplitElement>& mainSpltElem)
{
	// �����������ǉ�
	for (const auto add : spltElem) {
		itr = mainSpltElem.insert(itr, add);
		++itr;
	}
	// ��������̍폜
	itr = mainSpltElem.erase(itr);
	return;
}