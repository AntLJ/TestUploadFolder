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
#include "BaseCnv.h"

using namespace sindy::schema;

bool BaseCnv::cnv(std::set<RoadNameRecord>& rdNameRecs, std::set<RelNameLinkRecord>& rlNameLnkRecs, std::set<SpltNameRecord>& spltRdNameRecs)
{
	// ������
	if (!init())
		return false;
	// ���H�����N��QueryFilter�쐬
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if (S_OK != ipQFilter->put_WhereClause(CComBSTR(m_roadLinkWhere))) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgSetFilter, sindy::schema::road_link::kTableName);
		return false;
	}
	// ���H�����N���猏��+�J�[�\���擾
	long size = 0;
	if (S_OK != m_ipRoadLink->FeatureCount(ipQFilter, &size)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetRowCount, sindy::schema::road_link::kTableName);
		return false;
	}
	IFeatureCursorPtr ipLinkCursor;
	if (S_OK != m_ipRoadLink->Search(ipQFilter, VARIANT_FALSE, &ipLinkCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetCursor, sindy::schema::road_link::kTableName);
		return false;
	}
	// �擾�������H�����N�J�[�\�����񂵁A�ϊ����������{
	long counter = 0;
	IFeaturePtr ipLink;
	while (S_OK == ipLinkCursor->NextFeature(&ipLink) && ipLink) {
		++counter;
		if (counter%100 == 0) {
			std::cout << "\t" << counter << " / " << size << " ��\r";
		}
		// ���H�����NOID�擾
		long linkOID = -1;
		if (FAILED(ipLink->get_OID(&linkOID))) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetOID, sindy::schema::road_link::kTableName);
			return false;
		}
		// �T�u�e�[�u���̊֘ARow�擾
		long subOID = -1;
		_IRowPtr ipSubRow;
		if (!findSubRow(linkOID, ipLink, subOID, ipSubRow))
			return false;
		// �ϊ��Ώۂ��`�F�b�N
		bool chkCnv = false;
		if (!chkProcRec(linkOID, ipLink, subOID, ipSubRow, chkCnv))
			return false;
		if (!chkCnv)
			continue;
		// �����N�ɕR�Â����H���̃f�[�^�擾
		RoadNameRecord roadNameRecord;
		if (!findRoadName(linkOID, ipLink, subOID, ipSubRow, roadNameRecord))
			return false;
		// �s�s�������́E�s�s��������2�ϊ������̂݁A�������H�̌��ʁA���̂�NULL�ɂȂ�P�[�X������̂�
		// NULL�̏ꍇ�ɔ�΂�����������
		if (roadNameRecord.m_name.IsEmpty() && roadNameRecord.m_yomi.IsEmpty())
			continue;
		// ���H�����N�ɕR�Â����H���̃f�[�^�ϊ�
		if (!cnvRelNameLink(linkOID, roadNameRecord, rlNameLnkRecs))
			return false;
		// ���ɓ��H���́E���H���̕����f�[�^���ϊ��ς݂̏ꍇ��continue
		if (isConverted(roadNameRecord, rdNameRecs))
			continue;
		// ���H���̃f�[�^�ϊ�
		if (!cnvRoadName(roadNameRecord, rdNameRecs))
			return false;
		// ���H���̕����f�[�^�ϊ�
		if (!cnvSplitName(linkOID, ipLink, subOID, ipSubRow, roadNameRecord, spltRdNameRecs))
			return false;
	}
	std::cout << "\t" << counter << " / " << size << " ��" << std::endl;
	return true;
}

bool BaseCnv::init()
{
	// ���H���̃e�[�u���t�B�[���h�C���f�b�N�X�擾
	if (!setRoadNameFieldIndex() ||
		!setRoadLinkFieldIndex() ||
		!setSubTblFieldIndex())
		return false;

	// ���H���̃��R�[�h�擾
	if (!loadRoadNameList())
		return false;

	// �T�u�e�[�u���擾
	if (!loadSubTable())
		return false;

	return true;
}

bool BaseCnv::setRoadNameFieldIndex()
{
	if (S_OK != m_ipRoadNameList->FindField(CComBSTR(::sj::road_name_list::kNameClass), &m_nameListClsIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::sj::road_name_list::kTableName, ::sj::road_name_list::kNameClass);
		return false;
	}
	if (S_OK != m_ipRoadNameList->FindField(CComBSTR(::sj::road_name_list::kNameKanji), &m_nameListNameIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::sj::road_name_list::kTableName, ::sj::road_name_list::kNameKanji);
		return false;
	}
	if (S_OK != m_ipRoadNameList->FindField(CComBSTR(::sj::road_name_list::kNameYomi), &m_nameListYomiIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::sj::road_name_list::kTableName, ::sj::road_name_list::kNameYomi);
		return false;
	}
	return true;
}

bool BaseCnv::setSubTblFieldIndex()
{
	// �p����ŕK�v�ɉ����Ď��{
	return true;
}

// �ϊ��Ώ�road_name_list�̎擾
bool BaseCnv::loadRoadNameList()
{
	// �o�͖��̎�ʂ������R�[�h���擾���錟���t�B���^�쐬
	CString nameListWhere;
	nameListWhere.Format(_T("%s=%ld"), ::sj::road_name_list::kNameClass, m_tgtNameClass);
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if (S_OK != ipQFilter->put_WhereClause(CComBSTR(nameListWhere))) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgSetFilter, ::sj::road_name_list::kTableName);
		return false;
	}
	// �J�[�\���擾
	_ICursorPtr ipNameListCursor;
	if (S_OK != m_ipRoadNameList->Search(ipQFilter, VARIANT_FALSE, &ipNameListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::sj::road_name_list::kTableName);
		return false;
	}
	// �ϊ��Ώۓ��H���̃��R�[�h�擾
	_IRowPtr ipName;
	while (S_OK == ipNameListCursor->NextRow(&ipName) && ipName) {
		// OBJECTID�擾
		long nameOID = -1;
		if (S_OK != ipName->get_OID(&nameOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::sj::road_name_list::kTableName);
			return false;
		}
		// ���́A�ǂݎ擾
		CComVariant nameVal, yomiVal;
		if (S_OK != ipName->get_Value(m_nameListNameIndex, &nameVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::sj::road_name_list::kTableName, nameOID, ::sj::road_name_list::kNameKanji);
			return false;
		}
		if (S_OK != ipName->get_Value(m_nameListYomiIndex, &yomiVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::sj::road_name_list::kTableName, nameOID, ::sj::road_name_list::kNameYomi);
			return false;
		}
		RoadNameKey roadNameKey(nameVal.bstrVal, yomiVal.bstrVal);
		RoadNameRecord roadNameRecord(nameOID, m_tgtNameClass, nameVal.bstrVal, yomiVal.bstrVal);
		if (!m_tgtRdNameRecs.insert(std::make_pair(roadNameKey, roadNameRecord)).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, ::sj::road_name_list::kTableName, nameOID);
			return false;
		}
	}
	return true;
}

// �ϊ��ΏۃT�u�e�[�u���̎擾
bool BaseCnv::loadSubTable()
{
	// �K�v�ɉ����Čp����Œ�`
	return true;
}

// �T�u�e�[�u���̊֘ARow�擾
bool BaseCnv::findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow)
{
	// �K�v�ɉ����Čp����Œ�`
	return true;
}

// �����Ώۃ��R�[�h���`�F�b�N
bool BaseCnv::chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv)
{
	// �K�v�ɉ����Čp����Œ�`
	chkCnv = true;
	return true;
}

// ���H�����N�ɕR�Â����H���̃f�[�^�ϊ�
bool BaseCnv::cnvRelNameLink(const long& linkOID, const RoadNameRecord& roadNameRecord, std::set<RelNameLinkRecord>& rlNameLnkRecs)
{
	RelNameLinkRecord relNameLinkRecord(linkOID, roadNameRecord.m_nameID, roadNameRecord.m_nameCls);

	if (!rlNameLnkRecs.insert(relNameLinkRecord).second) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, linkOID, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}

// ���H���́E���H���̕����f�[�^�ϊ��ς݃`�F�b�N
bool BaseCnv::isConverted(const RoadNameRecord& roadNameRecord, const std::set<RoadNameRecord>& rdNameRecs)
{
	if (rdNameRecs.find(roadNameRecord) != rdNameRecs.end())
		return true;
	return false;
}

// ���H���̃f�[�^�ϊ�
bool BaseCnv::cnvRoadName(const RoadNameRecord& roadNameRecord, std::set<RoadNameRecord>& rdNameRecs)
{
	if (!rdNameRecs.insert(roadNameRecord).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::DuplicateData, ::sj::road_name_list::kTableName, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}