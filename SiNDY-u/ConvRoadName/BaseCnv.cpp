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
	// 初期化
	if (!init())
		return false;
	// 道路リンクのQueryFilter作成
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if (S_OK != ipQFilter->put_WhereClause(CComBSTR(m_roadLinkWhere))) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgSetFilter, sindy::schema::road_link::kTableName);
		return false;
	}
	// 道路リンクから件数+カーソル取得
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
	// 取得した道路リンクカーソルを回し、変換処理を実施
	long counter = 0;
	IFeaturePtr ipLink;
	while (S_OK == ipLinkCursor->NextFeature(&ipLink) && ipLink) {
		++counter;
		if (counter%100 == 0) {
			std::cout << "\t" << counter << " / " << size << " 件\r";
		}
		// 道路リンクOID取得
		long linkOID = -1;
		if (FAILED(ipLink->get_OID(&linkOID))) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgGetOID, sindy::schema::road_link::kTableName);
			return false;
		}
		// サブテーブルの関連Row取得
		long subOID = -1;
		_IRowPtr ipSubRow;
		if (!findSubRow(linkOID, ipLink, subOID, ipSubRow))
			return false;
		// 変換対象かチェック
		bool chkCnv = false;
		if (!chkProcRec(linkOID, ipLink, subOID, ipSubRow, chkCnv))
			return false;
		if (!chkCnv)
			continue;
		// リンクに紐づく道路名称データ取得
		RoadNameRecord roadNameRecord;
		if (!findRoadName(linkOID, ipLink, subOID, ipSubRow, roadNameRecord))
			return false;
		// 都市高速略称・都市高速略称2変換処理のみ、文字加工の結果、名称がNULLになるケースがあるので
		// NULLの場合に飛ばす処理を入れる
		if (roadNameRecord.m_name.IsEmpty() && roadNameRecord.m_yomi.IsEmpty())
			continue;
		// 道路リンクに紐づく道路名称データ変換
		if (!cnvRelNameLink(linkOID, roadNameRecord, rlNameLnkRecs))
			return false;
		// 既に道路名称・道路名称分割データが変換済みの場合はcontinue
		if (isConverted(roadNameRecord, rdNameRecs))
			continue;
		// 道路名称データ変換
		if (!cnvRoadName(roadNameRecord, rdNameRecs))
			return false;
		// 道路名称分割データ変換
		if (!cnvSplitName(linkOID, ipLink, subOID, ipSubRow, roadNameRecord, spltRdNameRecs))
			return false;
	}
	std::cout << "\t" << counter << " / " << size << " 件" << std::endl;
	return true;
}

bool BaseCnv::init()
{
	// 道路名称テーブルフィールドインデックス取得
	if (!setRoadNameFieldIndex() ||
		!setRoadLinkFieldIndex() ||
		!setSubTblFieldIndex())
		return false;

	// 道路名称レコード取得
	if (!loadRoadNameList())
		return false;

	// サブテーブル取得
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
	// 継承先で必要に応じて実施
	return true;
}

// 変換対象road_name_listの取得
bool BaseCnv::loadRoadNameList()
{
	// 出力名称種別を持つレコードを取得する検索フィルタ作成
	CString nameListWhere;
	nameListWhere.Format(_T("%s=%ld"), ::sj::road_name_list::kNameClass, m_tgtNameClass);
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	if (S_OK != ipQFilter->put_WhereClause(CComBSTR(nameListWhere))) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::fatal, err_code::NgSetFilter, ::sj::road_name_list::kTableName);
		return false;
	}
	// カーソル取得
	_ICursorPtr ipNameListCursor;
	if (S_OK != m_ipRoadNameList->Search(ipQFilter, VARIANT_FALSE, &ipNameListCursor)) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetCursor, ::sj::road_name_list::kTableName);
		return false;
	}
	// 変換対象道路名称レコード取得
	_IRowPtr ipName;
	while (S_OK == ipNameListCursor->NextRow(&ipName) && ipName) {
		// OBJECTID取得
		long nameOID = -1;
		if (S_OK != ipName->get_OID(&nameOID)) {
			Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, ::sj::road_name_list::kTableName);
			return false;
		}
		// 名称、読み取得
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

// 変換対象サブテーブルの取得
bool BaseCnv::loadSubTable()
{
	// 必要に応じて継承先で定義
	return true;
}

// サブテーブルの関連Row取得
bool BaseCnv::findSubRow(const long& linkOID, const IFeaturePtr& ipLink, long& subOID, _IRowPtr& ipSubRow)
{
	// 必要に応じて継承先で定義
	return true;
}

// 処理対象レコードかチェック
bool BaseCnv::chkProcRec(const long& linkOID, const IFeaturePtr& ipLink, const long& subOID, const _IRowPtr& ipSubRow, bool& chkCnv)
{
	// 必要に応じて継承先で定義
	chkCnv = true;
	return true;
}

// 道路リンクに紐づく道路名称データ変換
bool BaseCnv::cnvRelNameLink(const long& linkOID, const RoadNameRecord& roadNameRecord, std::set<RelNameLinkRecord>& rlNameLnkRecs)
{
	RelNameLinkRecord relNameLinkRecord(linkOID, roadNameRecord.m_nameID, roadNameRecord.m_nameCls);

	if (!rlNameLnkRecs.insert(relNameLinkRecord).second) {
		Log::GetInstance().PrintLog1(false, true, false, true, err_type::error, err_code::NgGetOID, linkOID, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}

// 道路名称・道路名称分割データ変換済みチェック
bool BaseCnv::isConverted(const RoadNameRecord& roadNameRecord, const std::set<RoadNameRecord>& rdNameRecs)
{
	if (rdNameRecs.find(roadNameRecord) != rdNameRecs.end())
		return true;
	return false;
}

// 道路名称データ変換
bool BaseCnv::cnvRoadName(const RoadNameRecord& roadNameRecord, std::set<RoadNameRecord>& rdNameRecs)
{
	if (!rdNameRecs.insert(roadNameRecord).second) {
		Log::GetInstance().PrintLog2(false, true, false, true, err_type::fatal, err_code::DuplicateData, ::sj::road_name_list::kTableName, roadNameRecord.m_nameID);
		return false;
	}
	return true;
}