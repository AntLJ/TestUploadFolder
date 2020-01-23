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
#include "NationalRoadNoCnv.h"

using namespace sindy::schema;

bool NationalRoadNoCnv::setRoadLinkFieldIndex()
{
	// RoadNo�t�B�[���h��index�擾
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadNo), &m_roadLinkRoadNoIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadNo);
		return false;
	}
	return true;
}

bool NationalRoadNoCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// ���H�ԍ��擾
	CComVariant vaRoadNo;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &vaRoadNo)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName , linkOID, ::road_link::kRoadNo);
		return false;
	}
	
	// ���H�ԍ����疼�́E�ǂ݂��쐬
	RoadNameKey roadNameKey(_T(""), _T(""));
	roadNameKey.m_name.Format(_T("����%s��"), num2ZenStr(vaRoadNo.lVal));
	roadNameKey.m_yomi.Format(_T("�R�N�h�E%s�S�E"), num2YomiStr(vaRoadNo.lVal));

	// ���H���̃f�[�^����
	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == itr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, road_link::kTableName, linkOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = itr->second;
	return true;
}

bool NationalRoadNoCnv::cnvSplitName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, const RoadNameRecord& roadNameRecord, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// ���H���̕����f�[�^ �V�[�P���X1�o�^
	SpltNameRecord spltNameRecord1(roadNameRecord.m_nameID, ::split_class::kBody, 1, _T("����"), _T("�R�N�h�E"));
	if (!spltRdNameRecs.insert(spltNameRecord1).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	// ���H���̕����f�[�^ �V�[�P���X2�o�^
	// ���̍쐬
	CStringW orgName = CT2CW(roadNameRecord.m_name), orgYomi = CT2CW(roadNameRecord.m_yomi);
	CStringW name2nd, yomi2nd;
	name2nd = orgName.Mid(roadNameRecord.m_name.Find(_T('��'))+1);
	yomi2nd = orgYomi.Mid(roadNameRecord.m_yomi.Find(_T('�E'))+1);
	SpltNameRecord spltNameRecord2(roadNameRecord.m_nameID, ::split_class::kBody, 2, CString(name2nd), CString(yomi2nd));
	if (!spltRdNameRecs.insert(spltNameRecord2).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, SPLIT_ROAD_NAME, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}