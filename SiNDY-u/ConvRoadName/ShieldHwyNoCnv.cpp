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
#include "ShieldHwyNoCnv.h"

using namespace sindy::schema;

bool ShieldHwyNoCnv::setRoadLinkFieldIndex()
{
	// RoadNoフィールドのindex取得
	if (S_OK != m_ipRoadLink->FindField(CComBSTR(::road_link::kRoadCode), &m_roadLinkRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_link::kTableName, ::road_link::kRoadCode);
		return false;
	}
	return true;
}

bool ShieldHwyNoCnv::setSubTblFieldIndex()
{
	// 必要indexの取得
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::road_code_list::kRoadCode), &m_roadCodeListRoadCodeIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kRoadCode);
		return false;
	}
	if (S_OK != m_ipSubNameTable->FindField(CComBSTR(::road_code_list::kShieldNoStr), &m_roadCodeListShieldNoStrIndex)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetIndex, ::road_code_list::kTableName, ::road_code_list::kShieldNoStr);
		return false;
	}
	return true;
}

bool ShieldHwyNoCnv::loadSubTable()
{
	// 全てのレコードを取得するが、レプリカでも動くようIQueryPtrを定義(検索時にIQueryPtrを与えないとレプリカで動作しない)
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	// カーソル取得
	_ICursorPtr ipCodeListCursor;
	if (S_OK != m_ipSubNameTable->Search(ipQFilter, VARIANT_FALSE, &ipCodeListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::road_code_list::kTableName);
		return false;
	}
	// 変換対象道路路線コードレコード取得
	_IRowPtr ipCode;
	while (S_OK == ipCodeListCursor->NextRow(&ipCode) && ipCode) {
		// OBJECTID取得
		long codeOID = -1;
		if (S_OK != ipCode->get_OID(&codeOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::road_code_list::kTableName);
			return false;
		}
		// 路線コード取得
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

bool ShieldHwyNoCnv::findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow)
{
	// 道路リンクよりRoadCode取得し検索
	CComVariant linkRoadCodeVal;
	if (S_OK != ipLink->get_Value(m_roadLinkRoadCodeIndex, &linkRoadCodeVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_link::kTableName, linkOID, ::road_link::kRoadCode);
		return false;
	}
	long linkRoadCode = linkRoadCodeVal.lVal;
	// 40万番台以上の場合、渡り線 or 無料区間であり、リンク側のRoadCodeは値が追加されているので、それを考慮する。
	if (CROSS_OVER_CODE_LOW <= linkRoadCode) {
		// 40万番台 or 50万番台の場合は、渡り線であり、20万引く必要あり
		if (CROSS_OVER_CODE_HIG >= linkRoadCode) {
			linkRoadCode = linkRoadCode - CROSS_OVER_PLUS_VALUE;
		} else if ((FREE_HWY_CODE_LOW <= linkRoadCode) && (FREE_HWY_CODE_HIG >= linkRoadCode)) {
			linkRoadCode = linkRoadCode - FREE_HWY_PLUS_VALUE;
		}
	}
		
	std::map<long, _IRowPtr>::const_iterator itr = m_tgtRdCodeListRecs.find(linkRoadCode);
	if (itr == m_tgtRdCodeListRecs.end()) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, ::road_link::kTableName, linkOID, ::road_code_list::kTableName);
		return false;
	}
	ipSubRow = itr->second;
	if (S_OK != ipSubRow->get_OID(&subOID)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::road_code_list::kTableName);
		return false;
	}
	return true;
}

bool ShieldHwyNoCnv::chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv)
{
	chkCnv = true;
	// SHIELD_NO_STRINGに値があるかチェック
	CComVariant shieldNoStrVal;
	if (S_OK != ipSubRow->get_Value(m_roadCodeListShieldNoStrIndex, &shieldNoStrVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, subOID, ::road_code_list::kShieldNoStr);
		return false;
	}
	if (shieldNoStrVal.vt == VT_NULL || shieldNoStrVal.bstrVal == _T(""))
		chkCnv = false;
	return true;
}

bool ShieldHwyNoCnv::findRoadName(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, RoadNameRecord& roadNameRecord)
{
	// SHIELD_NO_STRING取得
	CComVariant shieldNoStrVal;
	if (S_OK != ipSubRow->get_Value(m_roadCodeListShieldNoStrIndex, &shieldNoStrVal)) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NgGetVal, ::road_code_list::kTableName, subOID, ::road_code_list::kShieldNoStr);
		return false;
	}
	
	// 道路名称データ検索
	RoadNameKey roadNameKey(shieldNoStrVal.bstrVal, _T(""));
	std::map<RoadNameKey, RoadNameRecord>::const_iterator itr = m_tgtRdNameRecs.find(roadNameKey);
	if (m_tgtRdNameRecs.end() == itr) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::error, err_code::NoRelData, road_code_list::kTableName, subOID, ::sj::road_name_list::kTableName);
		return false;
	}
	roadNameRecord = itr->second;
	return true;
}