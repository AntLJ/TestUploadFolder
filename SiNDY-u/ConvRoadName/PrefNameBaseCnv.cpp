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
#include "PrefNameBaseCnv.h"

using namespace sindy::schema;

bool PrefNameBaseCnv::setRoadLinkFieldIndex()
{
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kPrefCode), &m_roadLinkPrefCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kPrefCode);
		return false;
	}
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadNo), &m_roadLinkRoadNoIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadNo);
		return false;
	}
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadClass), &m_roadLinkRoadClsIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadClass);
		return false;
	}
	return SplitBasicTokenRuleCnv::setRoadLinkFieldIndex();
}

bool PrefNameBaseCnv::setSubTblFieldIndex()
{
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::pref_road_list::kPrefCode), &m_prefRdListPrefCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::pref_road_list::kTableName, ::pref_road_list::kPrefCode);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::pref_road_list::kRoadNo), &m_prefRdListRoadNoIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::pref_road_list::kTableName, ::pref_road_list::kRoadNo);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::pref_road_list::kRoadClass), &m_prefRdListRoadClsIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::pref_road_list::kTableName, ::pref_road_list::kRoadClass);
		return false;
	}
	return SplitBasicTokenRuleCnv::setSubTblFieldIndex();
}

bool PrefNameBaseCnv::loadSubTable()
{
	// 全てのレコードを取得するが、レプリカでも動くようIQueryPtrを定義(検索時にIQueryPtrを与えないとレプリカで動作しない)
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// カーソル取得
	_ICursorPtr ipPrefRoadListCursor;
	if (S_OK != m_ipSubNameTable->Search(ipQFilter, VARIANT_FALSE, &ipPrefRoadListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::road_code_list::kTableName);
		return false;
	}
	// 変換対象PrefCodeList取得
	_IRowPtr ipPref;
	while (S_OK == ipPrefRoadListCursor->NextRow(&ipPref) && ipPref) {
		// OBJECTID取得
		long prefOID = -1;
		if (S_OK != ipPref->get_OID(&prefOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::pref_road_list::kTableName);
			return false;
		}
		//  PREF_CODE, ROAD_NO, ROAD_CLASS_C取得し、コンテナに挿入
		CComVariant prefCodeVal, roadNoVal, roadClsVal;
		if (S_OK != ipPref->get_Value(m_prefRdListPrefCodeIndex, &prefCodeVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, prefOID, ::pref_road_list::kPrefCode);
			return false;
		}
		if (S_OK != ipPref->get_Value(m_prefRdListRoadNoIndex, &roadNoVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, prefOID, ::pref_road_list::kRoadNo);
			return false;
		}
		if (S_OK != ipPref->get_Value(m_prefRdListRoadClsIndex, &roadClsVal)) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::pref_road_list::kTableName, prefOID, ::pref_road_list::kRoadClass);
			return false;
		}
		PrefCodeKey prefCodeKey(roadNoVal.lVal, roadClsVal.lVal, prefCodeVal.lVal);
		if (!m_tgtPrefRdListRecs.insert(std::make_pair(prefCodeKey, ipPref)).second) {
			Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::DuplicateData, ::pref_road_list::kTableName, prefOID);
			return false;
		}
	}
	return true;
}

bool PrefNameBaseCnv::findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow)
{
	// 道路リンクよりPREF_CODE, ROAD_NO, ROAD_CLASS_Cを取得し、検索
	CComVariant linkPrefCode, linkRoadNo, linkRoadCls;
	if (S_OK != ipLink->get_Value(m_roadLinkPrefCodeIndex, &linkPrefCode)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kPrefCode);
		return false;
	}
	if (S_OK != ipLink->get_Value(m_roadLinkRoadNoIndex, &linkRoadNo)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadNo);
		return false;
	}
	if (S_OK != ipLink->get_Value(m_roadLinkRoadClsIndex, &linkRoadCls)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadClass);
		return false;
	}
	PrefCodeKey prefCodeKey(linkRoadNo.lVal, linkRoadCls.lVal, linkPrefCode.lVal);
	std::map<PrefCodeKey, _IRowPtr>::const_iterator itr = m_tgtPrefRdListRecs.find(prefCodeKey);
	if (itr == m_tgtPrefRdListRecs.end()) {
		// 取得できない場合もあるので、エラーにはしない
		ipSubRow = nullptr;
		subOID = -1;
		return true;
	}
	ipSubRow = itr->second;
	if (S_OK != ipSubRow->get_OID(&subOID)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::road_code_list::kTableName);
		return false;
	}
	return true;
}

bool PrefNameBaseCnv::chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv)
{
	chkCnv = true;
	// ipSubRowがNULLの場合は変換対象外
	if (!ipSubRow) {
		chkCnv = false;
		return true;
	}
	return true;
}