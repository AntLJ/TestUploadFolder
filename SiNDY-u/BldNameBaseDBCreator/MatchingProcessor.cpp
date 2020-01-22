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
#include "MatchingProcessor.h"

using namespace sindy::schema::sj;
using namespace common_proc;

MatchingProcessor::MatchingProcessor(
	const std::vector<long>& srcPriListPrior,
	const std::vector<long>& srcPriListMatch,
	const std::vector<CString>& poiPriList,
	const std::vector<CString>& supPoiPriList,
	const std::vector<StrPair>& replaceList,
	const std::map<long, long>& condList,
	const std::map<CString, long>& bldSrcIndeces,
	const std::map<CString, long>& poiPtOrgIndeces,
	const std::map<CString, long>& mapAddrCodeCounter,
	bool isSurvey,
	SurveyPointManager& surveyPtMgr
	)
	: m_srcPriListPrior(srcPriListPrior), m_srcPriListMatch(srcPriListMatch), m_poiPriList(poiPriList), m_supPoiPriList(supPoiPriList),
	m_replaceList(replaceList), m_condList(condList), m_bldSrcIndeces(bldSrcIndeces), m_poiPtOrgIndeces(poiPtOrgIndeces),
	m_mapAddrCodeCounter(mapAddrCodeCounter), m_isSuvey(isSurvey), m_surveyPtMgr( surveyPtMgr )
{
}

MatchingProcessor::~MatchingProcessor(void)
{
}

bool MatchingProcessor::MatchingProc(
	IDFeatureMap& tgtSrcPtFeatures,
	const IDFeatureMap& tgtPoiPtFeatures,
	long bldID,
	IPolygonPtr ipFilledBldPolygon,
	BldNamePtInfo *bldNamePtInfo,
	std::set<long> *relIDs,
	bool *result,
	bool *excepted,
	bool *selected
	)
{
	*result = false;
	*excepted = false;
	*selected = false;

	// 対象素材ポイントが存在する場合に素材ポイントを利用した処理を実施
	// （１つもない場合でも補完用注記POIによる補完は実施する）
	if (tgtSrcPtFeatures.size() > 0)
	{
		// 「除外建物」「独自調査」の素材ポイントが存在するかどうかチェック
		if (! CheckSpecialSrcPt(tgtSrcPtFeatures, bldID, bldNamePtInfo, result, excepted, selected))
			return false;
		if (*result || *excepted) {
			return true;
		}

		// マッチング対象素材ポイント群から「独自調査」のものを削除
		DeleteOwnSurveyPts(tgtSrcPtFeatures);

		if (tgtSrcPtFeatures.size() > 0)
		{
			// 注記POI（素材ポイント）が存在するかチェックし、存在したらこれを登録
			if (! GetPoiSrcPtID(tgtSrcPtFeatures, bldID, bldNamePtInfo, result))
				return false;
			if (*result) {
				// POIが存在した時点で代表点、rel_buildingnameに登録する代表点ID群を登録して終了
				long headID = bldNamePtInfo->e_srcOID;
				SetRelIDs(tgtSrcPtFeatures, headID, relIDs);
				return true;
			}

			// その他の素材ポイントによるマッチング
			long headID = -1;
			if (1 == tgtSrcPtFeatures.size()) {
				// 家形上に1ポイントしか存在しない場合の処理
				IDFeatureMap::const_iterator srcPtItr = tgtSrcPtFeatures.begin();
				if (! MatchingOnePoint(srcPtItr->first, srcPtItr->second.m_T, bldID, ipFilledBldPolygon, false, bldNamePtInfo, result))
					return false;
				// 1ポイントしか存在しない場合は、rel_buildingnameには何も登録しない
			} else {
				// 文字列マッチングを行う
				if (! MatchingPoints(tgtSrcPtFeatures, bldID, bldNamePtInfo, result))
					return false;
				if (*result) {
					headID = bldNamePtInfo->e_srcOID;
				} else {
					// 同じ種別のポイントが複数存在するはずなので、各ポイントの信頼性をチェックする。
					std::map<long, BldNamePtInfo> sameSrcBldNamPtInfos;
					for (auto& tgtSrcPt : tgtSrcPtFeatures) {
						BldNamePtInfo tmpInfo;
						bool priorityChk = false;
						if (! MatchingOnePoint(tgtSrcPt.first, tgtSrcPt.second, bldID, ipFilledBldPolygon, true, &tmpInfo, &priorityChk))
							return false;
						if (priorityChk)
							sameSrcBldNamPtInfos[tgtSrcPt.first] = tmpInfo;
					}
					if (sameSrcBldNamPtInfos.size() > 0) {
						headID = SelectSameSrcBldNamePtInfo(sameSrcBldNamPtInfos);
						*bldNamePtInfo = sameSrcBldNamPtInfos.find(headID)->second;
						*result = true;
					}
				}
			}

			if (*result) {
				// 採用条件記述リストによるフィルタリング
				// 採用条件記述リストにない信頼度種別コードの場合、もしくはマッチング率が指定された閾値未満の場合は、建物名称ポイントレコード情報をリセット
				std::map<long, long>::const_iterator condItr = m_condList.find(bldNamePtInfo->e_priority);
				if (condItr == m_condList.end() || (bldNamePtInfo->e_matchPer != -1 && bldNamePtInfo->e_matchPer < condItr->second)) {
					bldNamePtInfo->Reset();
					*result = false;
				} else if (headID > 0) {
					SetRelIDs(tgtSrcPtFeatures, headID, relIDs);
				}
			}
		}
	}

	// 素材ポイントでは採用ポイントが決まらなかった場合、補完用注記POIによる補完を実施
	if (! *result) {
		if (! SupplyWithPoiPtOrg(tgtPoiPtFeatures, bldID, bldNamePtInfo, result))
			return false;
	}

	// 調査対象ポイントリスト出力モードで尚且つ代表点が取得できなかった場合は、調査対象ポイント情報登録
	if (tgtSrcPtFeatures.size() > 0 && (! *result) && m_isSuvey && (! *selected))
	{
		long tgtSrcID = -1;
		IFeaturePtr ipTgtSrcFeature;
		if (! ::common_proc::SelectHeadNameFeature(tgtSrcPtFeatures, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &tgtSrcID, ipTgtSrcFeature))
			return false;
		if(! m_surveyPtMgr.addFromBldgNameSrc( tgtSrcID, bldID, ipTgtSrcFeature ) )
			return false;
		*selected = true;
	}

	return true;
}

bool MatchingProcessor::CheckSpecialSrcPt(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result, bool *excepted, bool *selected)
{
	PriorFeatureMap priorFeatureMap;	// 優先度決定マップ
	for (IDFeatureMap::const_iterator itrSrcPt = tgtSrcPtFeatures.begin(); itrSrcPt != tgtSrcPtFeatures.end(); ++itrSrcPt)
	{
		// 出典種別コード取得
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			return false;
		// 出典種別コードが「除外建物」の場合
		if (::buildingname_src_point::source_type::kExclusion == srcType.lVal)
		{
			LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("「除外建物」ポイントが存在します"), sindy::schema::building::kTableName, bldID);
			*excepted = true;
			return true;	// 即抜ける
		}
		// 採用種別コード取得
		CComVariant adptCode;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAdoption, m_bldSrcIndeces, &adptCode))
			return false;
		// 採用種別コードのチェック（「独自調査」ポイントの場合）
		if (::buildingname_src_point::source_type::kOwnSurvey == srcType.lVal)
		{
			switch(adptCode.lVal)
			{
			case ::buildingname_src_point::adoption_code::kAdopt:			// 採用
			case ::buildingname_src_point::adoption_code::kRejectClosed:	// 現調により不採用（閉鎖・民家）
			case ::buildingname_src_point::adoption_code::kRejectUnkown:	// 現調により不採用（不明）
				break;
			default:
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::err, _T("不正な「独自調査」ポイントが存在します"), ::buildingname_src_point::kTableName, itrSrcPt->first);
				continue;
			}
		}
		// 名称取得
		CComVariant name;
		if (! ::GetValue((_IRowPtr)itrSrcPt->second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// 「優先利用」種別の場合、優先度決定マップに格納
		if (std::find(m_srcPriListPrior.begin(), m_srcPriListPrior.end(), srcType.lVal) != m_srcPriListPrior.end())
		{
			SrcPriorKey srcPriorKey(m_srcPriListPrior, srcType.lVal, adptCode.lVal, CString(name.bstrVal).GetLength(), itrSrcPt->first);
			priorFeatureMap.insert(PriorFeatureMap::value_type(srcPriorKey, itrSrcPt->second.m_T));
		}
	}

	// 「優先利用」種別の素材ポイントがない場合は抜ける
	if (0 == priorFeatureMap.size()) return true;

	// 優先度1位のレコードについて処理
	PriorFeatureMap::const_iterator itr = priorFeatureMap.begin();
	if (::buildingname_src_point::source_type::kOwnSurvey == itr->first.e_srcType && ::buildingname_src_point::adoption_code::kRejectClosed == itr->first.e_adptCode)
	{
		// 「現調により不採用（閉鎖・民家）」の場合
		// 「除外建物」と同等の扱い
		LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("「31: 現調により不採用（閉鎖・民家）」により除外"), sindy::schema::building::kTableName, bldID);
		*excepted = true;
	}
	else if (::buildingname_src_point::source_type::kOwnSurvey == itr->first.e_srcType && ::buildingname_src_point::adoption_code::kRejectUnkown == itr->first.e_adptCode)
	{
		// 「現調により不採用（不明）」
		// 調査対象ポイント生成を抑制
		*selected = true;
	}
	else
	{
		// その他 → 優先的にベースDBに登録
		IFeaturePtr ipPrior = itr->second;
		// NAME取得
		CComVariant name;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// SOURCENAME取得
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		// SOURCEID取得
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		// ANYPLACE_ID取得
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)ipPrior, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		// 形状取得
		IGeometryPtr ipGeometry;
		if (FAILED(ipPrior->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), ::buildingname_src_point::kTableName, itr->first.e_objectID);
			return false;
		}
		bldNamePtInfo->e_name		= (VT_BSTR == name.vt) ? (CString)name : _T("");
		bldNamePtInfo->e_bldOID		= bldID;
		bldNamePtInfo->e_srcType	= itr->first.e_srcType;
		bldNamePtInfo->e_srcName	= (VT_BSTR == srcName.vt) ? (CString)srcName : _T("");
		bldNamePtInfo->e_srcID		= (VT_BSTR == srcID.vt) ? (CString)srcID : _T("");
		bldNamePtInfo->e_srcOID		= itr->first.e_objectID;
		bldNamePtInfo->e_anyPlaceID	= anyplaceID.bstrVal;
		bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kEdit;
		bldNamePtInfo->e_matchPer	= -1;
		bldNamePtInfo->e_ipGeometry	= ipGeometry;
		*result = true;
	}

	// 残りのレコードについて採用されない旨をエラーログに出力
	++itr;
	for (; itr != priorFeatureMap.end(); ++itr)
	{
		CString errMsg;
		errMsg.Format(_T("「優先利用」のポイントが同一建物内に複数存在するため不採用\t%d"),  itr->first.e_srcType);
		LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, errMsg, ::buildingname_src_point::kTableName, itr->first.e_objectID);
	}

	return true;
}

void MatchingProcessor::DeleteOwnSurveyPts(IDFeatureMap& tgtSrcPtFeatures)
{
	// 一旦削除対象ID（キー）を取得する
	std::set<long> deleteIDs;
	for (auto rec : tgtSrcPtFeatures)
	{
		// 出典種別コード取得
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)rec.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			continue;
		// 「独自調査」の場合は削除対象IDとして追加
		if (buildingname_src_point::source_type::kOwnSurvey == srcType.lVal)
			deleteIDs.insert( rec.first );
	}
	// 削除対象IDに該当するものを削除
	for (auto delID : deleteIDs)
	{
		tgtSrcPtFeatures.erase(delID);
	}
}

bool MatchingProcessor::GetPoiSrcPtID(const IDFeatureMap& tgtSrcPtFeatures, long bldID, BldNamePtInfo* bldNamePtInfo, bool* existPOI)
{
	*existPOI = false;
	// 出典コードがPOIである素材のCONTENTS_CODE(BUILDING1_DIV1)、名称(NAME)の長さ・objectidを取得する。
	std::map<CString, std::set<POIINFO>> poiInfos; // キー:CONTENTS_CODE 要素 名称の長さ&objectid&名称(名称の長さが短い順に格納・名称の長さが同じの場合はobjectid順)
	for (const auto& tgtSrcPt : tgtSrcPtFeatures) {
		// 出典コード取得
		CComVariant srcType;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType))
			return false;
		if (::buildingname_src_point::source_type::kAnnoPOI != srcType.lVal)
			continue;		// 注記POI以外は飛ばす
		// CONTENTS_CODE取得
		CComVariant contentsCode;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kBuilding1Div1, m_bldSrcIndeces, &contentsCode))
			return false;
		// CONTENTS_CODEがNULLの場合、エラーを出力する。
		if ( contentsCode.vt == VT_NULL || contentsCode.vt == VT_EMPTY) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("注記POI素材にコンテンツコードが存在しません"), ::buildingname_src_point::kTableName, tgtSrcPt.first);
			return false;
		}
		// NAME取得
		CComVariant name;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		// SOURCEID取得
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		// 形状取得
		IGeometryPtr ipGeometry;
		if (FAILED(tgtSrcPt.second.m_T->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), ::buildingname_src_point::kTableName, tgtSrcPt.first);
			return false;
		}
		// POI情報のキーセット
		POIINFO poiInfo;
		poiInfo.e_nameLength	= (VT_BSTR == name.vt) ? ((CString)name).GetLength() : 0;
		poiInfo.e_objectid		= tgtSrcPt.first;
		poiInfo.e_name			= (VT_BSTR == name.vt) ? (CString)name : _T("");
		poiInfo.e_srcID			= (VT_BSTR == srcID.vt) ? (CString)srcID : _T("");
		poiInfo.e_ipGeometry	= ipGeometry;
		CString strContentsCode	= (VT_BSTR == contentsCode.vt) ? (CString)contentsCode : _T("");
		poiInfos[strContentsCode].insert(poiInfo);
	}
	
	// 対象となる注記POIポイントが存在しない場合は終了
	if (0 == poiInfos.size())
		return true;

	// 複数存在するので、優先度チェック
	for (const auto& poiCode : m_poiPriList) {
		// m_poiPriListは優先度順に格納されているので、見つかり次第終了
		std::map<CString, std::set<POIINFO>>::const_iterator tgtSrcItr = poiInfos.find(poiCode);
		if (tgtSrcItr != poiInfos.end()) {
			// std::set<POIINFO>は文字列の短い順（文字列の長さが一緒の場合はobjectid順）で格納されているので1個目の要素が対象
			std::set<POIINFO>::const_iterator poiInfoItr = tgtSrcItr->second.begin();
			bldNamePtInfo->e_name		= poiInfoItr->e_name;
			bldNamePtInfo->e_bldOID		= bldID;
			bldNamePtInfo->e_srcType	= ::buildingname_point::source_type::kAnnoPOI;
			bldNamePtInfo->e_srcID		= poiInfoItr->e_srcID;
			bldNamePtInfo->e_srcOID		= poiInfoItr->e_objectid;
			bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kAnnoPoi;
			bldNamePtInfo->e_matchPer	= -1;
			bldNamePtInfo->e_ipGeometry	= poiInfoItr->e_ipGeometry.m_T;
			*existPOI = true;
			// 残りのobjectidをログに出力する。
			++poiInfoItr;
			for (; poiInfoItr != tgtSrcItr->second.end(); ++poiInfoItr) {
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("同一POIジャンルが複数存在した為、文字列の長さ or objectid順で対象外になりました"), ::buildingname_src_point::kTableName, poiInfoItr->e_objectid);
			}
			break;
		}
	}
	if (! *existPOI) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("建物上に注記POI優先度種別リストに存在しない注記POI素材が存在します"), sindy::schema::building::kTableName, bldID);
		return false;
	} 

	return true;
}

bool MatchingProcessor::MatchingOnePoint(
	long tgtSrcID,
	const IFeaturePtr& ipTgtSrcFeature,
	long bldID,
	IPolygonPtr ipFilledBldPolygon,
	bool isPart,
	BldNamePtInfo* bldNamePtInfo,
	bool* result
	)
{
	*result = false;
	// 防火対象物・不動産情報・GSのチェック
	CComVariant sourceType;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &sourceType))
		return false;
	switch(sourceType.lVal) {
	case ::buildingname_src_point::source_type::kFirePreventive:	// 防火対象物の場合
		if (! AdoptFirePrevChk(ipTgtSrcFeature, result))
			return false;
		if (*result) {
			if (! isPart) {
				// マッチング対象建物上に本素材しか存在しない場合
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kOneFirePreventive;
			} else {
				// マッチング対象建物上に同種別素材が複数ある場合
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kNPartFirePreventive;
			}
		}
		break;
	case ::buildingname_src_point::source_type::kImmovables:		// 不動産情報の場合
		if (! AdoptImmovablesChk(tgtSrcID, ipTgtSrcFeature, ipFilledBldPolygon, bldID, result))
			return false;
		if (*result) {
			if (! isPart) {
				// マッチング対象建物上に本素材しか存在しない場合
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kOneImmovalbles;
			} else {
				// マッチング対象建物上に同種別素材が複数ある場合
				bldNamePtInfo->e_priority = ::buildingname_point::priority_code::kNPartImmovalbles;
			}
		}
		break;
	case ::buildingname_src_point::source_type::kGeoSpace:			// GSの場合
		break;	// GSだけの場合は何もしない(登録しない)
	default:
		CString errMsg;
		errMsg.Format(_T("%sが不正です"), ::buildingname_src_point::kSourceType);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	if (*result) {
		// 代表点に選ばれた場合のみ更新
		bldNamePtInfo->e_bldOID = bldID;
		bldNamePtInfo->e_matchPer = -1;
		bldNamePtInfo->e_srcOID = tgtSrcID;
		bldNamePtInfo->e_srcType = sourceType.lVal;
		// NAME取得
		CComVariant name;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		bldNamePtInfo->e_name = name.bstrVal;
		// SOURCENAME取得
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		bldNamePtInfo->e_srcName = srcName.bstrVal;
		// SOURCEID取得
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		bldNamePtInfo->e_srcID = srcID.bstrVal;
		// ANYPLACE_ID取得
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		bldNamePtInfo->e_anyPlaceID = anyplaceID.bstrVal;
		// 形状取得
		IGeometryPtr ipTempGeo;
		if (FAILED(ipTgtSrcFeature->get_ShapeCopy(&ipTempGeo))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), ::buildingname_src_point::kTableName, tgtSrcID);
			return false;
		}
		bldNamePtInfo->e_ipGeometry = ipTempGeo;
	}
	return true;
}

bool MatchingProcessor::MatchingPoints(const IDFeatureMap& tgtSrcPtFeatures, 
									   long bldID,
									   BldNamePtInfo *bldNamePtInfo,
									   bool *result)
{
	*result = false;
	std::map<long, MATCHINGINFO> resultMatch;	// ① キー：素材のobjecttid 要素：マッチング結果
	std::map<long, std::map<long, MATCHINGINFO*>> resultMatchSortPerfectMatch; // キー：完全一致個数 要素：①のマップ
	std::map<long, std::map<long, MATCHINGINFO*>> resultMatchSortReplacePerfectMatch; // キー：加工後完全一致個数 要素：①のマップ
	std::map<double, std::map<long, MATCHINGINFO*>> resultMatchSortMaxMatchPer; // キー：最大マッチング率 要素：①のマップ
	// 各素材のマッチング情報取得
	if (! GetEachMatchingInfo(tgtSrcPtFeatures,
							  &resultMatch,
							  &resultMatchSortPerfectMatch,
							  &resultMatchSortReplacePerfectMatch,
							  &resultMatchSortMaxMatchPer))
		return false;
	// マッチング情報が無い場合は全て同じ種別なので、終わらせる
	if (resultMatch.size() == 0) 
		return true;
	// 代表点決定処理
	long headID = -1;
	long priority = -1;
	if (! GetHeadPt(resultMatchSortPerfectMatch, resultMatchSortReplacePerfectMatch, resultMatchSortMaxMatchPer, &headID, &priority)) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("代表点決定処理に失敗しました"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	// 登録情報取得
	// 属性登録
	std::map<long, MATCHINGINFO>::iterator headItr = resultMatch.find(headID);
	bldNamePtInfo->e_name = headItr->second.e_name;
	bldNamePtInfo->e_bldOID = bldID;
	bldNamePtInfo->e_srcType = headItr->second.e_srcType;
	bldNamePtInfo->e_srcName = headItr->second.e_srcName;
	bldNamePtInfo->e_srcID = headItr->second.e_srcID;
	bldNamePtInfo->e_srcOID = headID;
	bldNamePtInfo->e_anyPlaceID = headItr->second.e_anyplaceID;
	bldNamePtInfo->e_priority = priority;
	bldNamePtInfo->e_matchPer = headItr->second.e_maxMatchPer;
	// 形状登録
	IGeometryPtr ipTempGeo;
	IDFeatureMap::const_iterator featureItr = tgtSrcPtFeatures.find(headID);
	if (FAILED(featureItr->second->get_ShapeCopy(&ipTempGeo))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), ::buildingname_src_point::kTableName, headID);
		return false;
	}
	bldNamePtInfo->e_ipGeometry = ipTempGeo;
	*result = true;
	return true;
}

long MatchingProcessor::SelectSameSrcBldNamePtInfo(const std::map<long, BldNamePtInfo>& sameSrcBldNamePtInfo)
{
	// 文字列の長さを比較して、短い
	std::map<long, BldNamePtInfo> shortStrBldNamePtInfos;
	long length = -1;
	for (const auto& bldNamePt :sameSrcBldNamePtInfo) {
		if (length == -1) {
			shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			length = bldNamePt.second.e_name.GetLength();
		} else {
			long tgtLength = bldNamePt.second.e_name.GetLength();
			if (length == tgtLength) {
				shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			} else if (length > tgtLength) {
				shortStrBldNamePtInfos.clear();
				shortStrBldNamePtInfos[bldNamePt.first] = bldNamePt.second;
			}
		}
	}
	// 一番短い文字列の中で、素材IDが一番若いものが代表
	return shortStrBldNamePtInfos.begin()->first;
}

bool MatchingProcessor::AdoptFirePrevChk(const IFeaturePtr& ipTgtSrcFeature, bool* result)
{
	// 最初はfalseに
	*result = false;
	// 自分自身と同じ20桁コードを持つ防火対象素材があるかチェック
	CComVariant addrCode;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrCode, m_bldSrcIndeces, &addrCode))
		return false;
	const auto itr = m_mapAddrCodeCounter.find(CString(addrCode));
	if (m_mapAddrCodeCounter.end() != itr && 1 < (*itr).second) {
		// 同じ20桁コードの防火対象物素材が複数存在するので採用外
		return true;
	}
	// 素材が保持する20桁コードがiPC住所の号レベルで1つだけかチェック
	CComVariant sameAddr, sameAddrCount, pinpoint;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSameAddr, m_bldSrcIndeces, &sameAddr) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSameAddrCount, m_bldSrcIndeces, &sameAddrCount) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kPinPoint, m_bldSrcIndeces, &pinpoint))
		return false;
	if ((0 == sameAddr.lVal) && (1 == sameAddrCount.lVal) && (1 == pinpoint.lVal))
		*result = true;		// sameaddr_f = 0, sameaddr_count=1, pinpoin_f=1であればiPC住所の号レベルで一つ
	return true;
}

bool MatchingProcessor::AdoptImmovablesChk(long tgtSrcID, const IFeaturePtr& ipTgtSrcFeature, IPolygonPtr ipFilledBldPolygon, long bldID, bool *result)
{
	*result = false;
	// iPC住所の緯度経度取得
	CComVariant addrLon, addrLat;
	if (! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrLon, m_bldSrcIndeces, &addrLon) ||
		! ::GetValue((_IRowPtr)ipTgtSrcFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrLat, m_bldSrcIndeces, &addrLat))
		return false;
	// 取得した緯度経度からポイント作成
	IPointPtr ipPoint(CLSID_Point);
	if (FAILED(ipPoint->put_X(addrLon.dblVal)) || FAILED(ipPoint->put_Y(addrLat.dblVal))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("iPC住所緯度経度からのポイント作成失敗しました"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	// 作成したポイントに穴埋め建物形状の空間参照を付与
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipFilledBldPolygon->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照取得に失敗しました"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	if (FAILED(ipPoint->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("iPC住所緯度経度ポイントへの空間参照付与に失敗しました"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}

	// 作成したポイントが穴埋め建物形状内に存在するかチェック
	IRelationalOperatorPtr ipRel(ipFilledBldPolygon);
	VARIANT_BOOL containChk = VARIANT_FALSE;
	if (FAILED(ipRel->Contains((IGeometryPtr)ipPoint, &containChk))) {
		CString errMsg;
		errMsg.Format(_T("建物形状(objectid=%d)とのcontainチェックに失敗しました"), ::buildingname_src_point::kTableName, tgtSrcID);
		return false;
	}
	if (VARIANT_TRUE == containChk) 
		*result = true;	// 建物に含まれていれば採用
	return true;
}

bool MatchingProcessor::GetEachMatchingInfo(const IDFeatureMap& tgtSrcPtFeatures, 
											std::map<long, MATCHINGINFO>* resultMatch, 
											std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortPerfectMatch,
											std::map<long, std::map<long, MATCHINGINFO*>>* resultMatchSortReplacePerfectMatch,
											std::map<double, std::map<long, MATCHINGINFO*>>* resultMatchSortMaxMatchPer)
{
	for (const auto& tgtSrcPt : tgtSrcPtFeatures) {
		MATCHINGINFO matchingInfo;
		// 比較元の素材種類取得
		CComVariant srcType1;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType1))
			return false;
		if (::buildingname_src_point::source_type::kGeoSpace == srcType1.lVal)
			continue;		// 素材がGSの場合は代表点にはならないので飛ばす
		// 素材種類登録
		matchingInfo.e_srcType = srcType1.lVal;
		// 比較元名称取得
		CComVariant name;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name))
			return false;
		CString orgName1 = name.bstrVal;
		// 比較元名称登録
		matchingInfo.e_name = orgName1;
		// 加工文字列取得
		CString replaceName1 = ReplaceStr(orgName1);
		// SOURCENAME取得
		CComVariant srcName;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceName, m_bldSrcIndeces, &srcName))
			return false;
		matchingInfo.e_srcName = srcName.bstrVal;
		// SOURCEID取得
		CComVariant srcID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceID, m_bldSrcIndeces, &srcID))
			return false;
		matchingInfo.e_srcID = srcID.bstrVal;
		// AnyPlaceID取得
		CComVariant anyplaceID;
		if (! ::GetValue((_IRowPtr)tgtSrcPt.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAnyPlaceID, m_bldSrcIndeces, &anyplaceID))
			return false;
		matchingInfo.e_anyplaceID = anyplaceID.bstrVal;

		// 自分と同じ素材以外とのマッチング処理
		bool doMatching = false;
		for (const auto& tgtSrcPt2 : tgtSrcPtFeatures) {
			// 比較先の素材種類取得
			CComVariant srcType2;
			if (! ::GetValue((_IRowPtr)tgtSrcPt2.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &srcType2))
				return false;
			// 比較元種別と比較先種別が一致した場合は、処理を飛ばす
			if (srcType1.lVal == srcType2.lVal)
				continue;
			doMatching = true;
			// 比較先名称取得
			CComVariant name2;
			if (! ::GetValue((_IRowPtr)tgtSrcPt2.second.m_T, ::buildingname_src_point::kTableName, ::buildingname_src_point::kName, m_bldSrcIndeces, &name2))
				return false;
			CString orgName2 = name2.bstrVal;
			// 比較先加工文字取得
			CString replaceName2 = ReplaceStr(orgName2);
			// マッチング処理
			// 元文字列一致チェック
			if (orgName1 == orgName2)
				++(matchingInfo.e_perfectMatchCount);
			// 加工後文字列チェック
			if (replaceName1 == replaceName2)
				++(matchingInfo.e_replacePerfectMatchCount);
			// マッチング率チェック
			double matchPer = MatchingStr(replaceName1, replaceName2);
			if (matchPer > matchingInfo.e_maxMatchPer)
				matchingInfo.e_maxMatchPer = matchPer;		// 最大マッチング率登録
			// マッチング率登録
			matchingInfo.e_allMatchPer[tgtSrcPt2.first] = matchPer;
		}
		if (doMatching) {
			(*resultMatch)[tgtSrcPt.first] = matchingInfo;
			(*resultMatchSortPerfectMatch)[matchingInfo.e_perfectMatchCount].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
			(*resultMatchSortReplacePerfectMatch)[matchingInfo.e_replacePerfectMatchCount].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
			(*resultMatchSortMaxMatchPer)[matchingInfo.e_maxMatchPer].insert(std::pair<long,MATCHINGINFO*>(tgtSrcPt.first, &((*resultMatch)[tgtSrcPt.first])));
		}
	}
	return true;
}

CString MatchingProcessor::ReplaceStr(const CString& orgName)
{
	CString replaceName = orgName;
	for (const auto& strPair :m_replaceList) {
		replaceName.Replace(strPair.e_orgStr, strPair.e_replaceStr);
	}
	return replaceName;
}

double MatchingProcessor::MatchingStr(const CString& str1, const CString& str2)
{
	double matchPer = 0.0;
	CString baseStr, tgtStr;
	long length = 0;
	if (str1.GetLength() > str2.GetLength()) {
		baseStr = str1;
		tgtStr = str2;
		length = _tcsclen(str1);
	} else {
		baseStr = str2;
		tgtStr = str1;
		length = _tcsclen(str2);
	}
	// 一文字づつ分解
	std::set<CString> baseCharSet = Str2Pieces(baseStr);
	std::set<CString> tgtCharSet = Str2Pieces(tgtStr);
	// 一致文字数取得
	long sameCount = 0;
	for (const auto& str : baseCharSet) {
		if (tgtCharSet.find(str) != tgtCharSet.end())
			++sameCount;
	}
	matchPer = (sameCount*100.0)/length;
	return matchPer;
}

bool MatchingProcessor::GetHeadPt(const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortPerfectMatch,
								  const std::map<long, std::map<long, MATCHINGINFO*>>& resultMatchingSortReplacePerfectMatch,
				                  const std::map<double, std::map<long, MATCHINGINFO*>>& resultMatchingSortMaxMatchPer,
								  long *headID,
								  long *priority)
{
	long maxPerfectMatchCount = resultMatchingSortPerfectMatch.rbegin()->first;
	long maxReplacePefectMatchCount = resultMatchingSortReplacePerfectMatch.rbegin()->first;
	std::map<long, MATCHINGINFO*> maxPerfectMatchInfo = resultMatchingSortPerfectMatch.rbegin()->second;
	std::map<long, MATCHINGINFO*> maxReplacePerfectMatchInfo = resultMatchingSortReplacePerfectMatch.rbegin()->second;
	//　加工後完全一致がある場合
	if (maxReplacePefectMatchCount > 0) {
		if (! GetHeadPtByPerfectMatch(maxPerfectMatchInfo, maxReplacePerfectMatchInfo, maxPerfectMatchCount, headID, priority))
			return false;
	} else {
		// 加工後完全一致が無い場合
	std::map<long, MATCHINGINFO*> maxMatchPerInfo = resultMatchingSortMaxMatchPer.rbegin()->second;
	if (! GetHeadPtByMatchPer(maxMatchPerInfo, headID, priority))
		return false;
	}
	return true;
}

bool MatchingProcessor::GetHeadPtByPerfectMatch(const std::map<long, MATCHINGINFO*>& maxPerfectMatchInfo,
												const std::map<long, MATCHINGINFO*>& maxReplacePerfectMatchInfo,
												long maxPerfectMatchCount,
												long *headID,
												long *priority)
{
	bool existPerfectMatch = false;
	if (maxPerfectMatchCount > 0)
		existPerfectMatch = true;
	// 信頼度コード3:完全一致個数が一番多い素材が一つの場合
	if (maxPerfectMatchInfo.size() == 1) {
		*headID = maxPerfectMatchInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri3;
		return true;
	}
	// 信頼度コード4:加工後完全一致個数が一番多い素材が一つの場合
	if (maxReplacePerfectMatchInfo.size() == 1) {
		*headID = maxReplacePerfectMatchInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri4;
		return true;
	}
	// 信頼度コード5:完全一致個数が一番多い素材の中で優先度が高いもの
	std::map<long, MATCHINGINFO*> perfectMatchPriInfo = GetMatchInfoBySrcPri(maxPerfectMatchInfo);
	if (existPerfectMatch) {
		if (perfectMatchPriInfo.size() == 1) {
			*headID = perfectMatchPriInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri5;
			return true;
		}
		if (perfectMatchPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("素材優先度種別リストに存在しない素材があります"), ::buildingname_src_point::kTableName);
			return false;
		}
	}
	// 信頼度コード6:加工後完全一致個数が一番多い存在の中で優先度が高いもの
	std::map<long, MATCHINGINFO*> replacePerfectMatchPriInfo = GetMatchInfoBySrcPri(maxReplacePerfectMatchInfo);
	if (replacePerfectMatchPriInfo.size() == 1) {
		*headID = replacePerfectMatchPriInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri6;
		return true;
	}
	if (replacePerfectMatchPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("素材優先度種別リストに存在しない素材があります"), ::buildingname_src_point::kTableName);
			return false;
	}
	// 信頼度コード7:信頼度コード5のチェックの際に取得した素材のうち文字列が一番短いもの
	std::map<long, MATCHINGINFO*> perfectMatchLenInfo = GetMatchInfoByStrLen(perfectMatchPriInfo);
	if (existPerfectMatch) {
		if (perfectMatchLenInfo.size() == 1) {
			*headID = perfectMatchLenInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri7;
			return true;
		}
	}
	// 信頼度コード8:信頼度コード6のチェックの際に取得した素材のうち文字列が一番短いもの
	std::map<long, MATCHINGINFO*> replacePerfectMatchLenInfo = GetMatchInfoByStrLen(replacePerfectMatchPriInfo);
	if (replacePerfectMatchLenInfo.size() == 1) {
		*headID = replacePerfectMatchLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri8;
		return true;
	}
	// 信頼度コード9:信頼度コード7のチェックの際に取得した素材のうち、objectidが一番若いもの
	if (existPerfectMatch) {
		if (perfectMatchLenInfo.size() >= 0) {
			*headID = perfectMatchLenInfo.begin()->first;
			*priority = ::buildingname_point::priority_code::kPri9;
			return true;
		}
	}
	// 信頼度コード10:信頼度コード8のチェックの際に取得した素材のうち、objectidが一番若いもの
	if (replacePerfectMatchLenInfo.size() >= 0) {
		*headID = replacePerfectMatchLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri10;
		return true;
	}
	// ここまで来ることはないはずなので、最後falseにしておく
	LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("プログラムが不正です(代表点決定部)。開発者にお問い合わせください。"));
	return false;
}

bool MatchingProcessor::GetHeadPtByMatchPer(const std::map<long, MATCHINGINFO*>& maxMatchPerInfo,
											long *headID,
											long *priority)
{
	// 信頼度コード11:マッチング率が最大の素材が一つ
	if (maxMatchPerInfo.size() == 1) {
		*headID = maxMatchPerInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri11;
		return true;
	}
	// 信頼度コード12:信頼度コード11で取得した素材のうち、優先度が一番高いもの
	std::map<long, MATCHINGINFO*> matchPerPriInfo = GetMatchInfoBySrcPri(maxMatchPerInfo);
	if (matchPerPriInfo.size() == 1) {
		*headID = matchPerPriInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri12;
		return true;
	}
	if (matchPerPriInfo.size() == 0) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("素材優先度種別リストに存在しない素材があります"), ::buildingname_src_point::kTableName);
			return false;
	}
	// 信頼度コード13:信頼度コード12で取得した素材のうち、文字列の長さが一番短いもの
	std::map<long, MATCHINGINFO*> matchPerLenInfo = GetMatchInfoByStrLen(matchPerPriInfo);
	if (matchPerLenInfo.size() == 1) {
		*headID = matchPerLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri13;
		return true;
	}
	// 信頼度コード14:信頼度コード13で取得した素材のうち、objectIDが一番若いもの
	if (matchPerLenInfo.size() > 0) {
		*headID = matchPerLenInfo.begin()->first;
		*priority = ::buildingname_point::priority_code::kPri14;
		return true;
	}
	// ここまで来ることはないはずなので、最後falseにしておく
	LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("プログラムが不正です(代表点決定部)。開発者にお問い合わせください。"));
	return false;
}

std::map<long, MATCHINGINFO*> MatchingProcessor::GetMatchInfoBySrcPri(const std::map<long, MATCHINGINFO*>& matchingInfos)
{
	std::map<long, MATCHINGINFO*> retMatchingInfos;
	for (const auto & srcPri : m_srcPriListMatch) {
		for (const auto & matchingInfo: matchingInfos) {
			if (srcPri == matchingInfo.second->e_srcType)
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
		}
		// 優先度順にfor文は回しているので、最初にretMatchingInfosの要素数が正になった時点で終了
		if (0 < retMatchingInfos.size()) 
			break;
	}
	return retMatchingInfos;
}

std::map<long, MATCHINGINFO*> MatchingProcessor::GetMatchInfoByStrLen(const std::map<long, MATCHINGINFO*>& matchingInfos)
{
	std::map<long, MATCHINGINFO*> retMatchingInfos;
	long length = -1;
	for (const auto & matchingInfo : matchingInfos) {
		if (length == -1) {
			retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			length = matchingInfo.second->e_name.GetLength();
		} else {
			long tgtLength = matchingInfo.second->e_name.GetLength();
			if (length == tgtLength) {
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			} else if (length > tgtLength) {
				retMatchingInfos.clear();
				retMatchingInfos[matchingInfo.first] = matchingInfo.second;
			}
		}
	}
	return retMatchingInfos;
}

void MatchingProcessor::SetRelIDs(const IDFeatureMap& tgtSrcPtFeatures, long headID, std::set<long>* relIDs)
{
	for (const auto& tgtSrcPt : tgtSrcPtFeatures){
		if (tgtSrcPt.first != headID)
			relIDs->insert(tgtSrcPt.first);
	}
}

std::set<CString> MatchingProcessor::Str2Pieces(const CString& str)
{
	std::set<CString> strSet;
	long num = str.GetLength();
	for (long i = 0; i < num; ++i) {
		CString temp = str.Mid(i, 1);
		if (strSet.find(temp) == strSet.end())
			strSet.insert(temp);
	}
	return strSet;
}

bool MatchingProcessor::SupplyWithPoiPtOrg(const IDFeatureMap& tgtPoiPtFeatures, long bldID, BldNamePtInfo *bldNamePtInfo, bool *result)
{
	using namespace sindy::schema::sindyk;

	*result = false;

	// マッチング対象注記POIポイントがない場合は終了
	if (0 == tgtPoiPtFeatures.size())
		return true;

	// 注記POIポイントのリストを、コンテンツコードをキーとしたリストに変換
	std::map<CString, std::set<POIINFO>> poiInfos;
	for (const auto& tgtPoiPt : tgtPoiPtFeatures) {
		// コンテンツコード取得
		CComVariant contentsCode;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kContentsCode, m_poiPtOrgIndeces, &contentsCode))
			return false;
		// コンテンツ内シーケンス取得
		CComVariant contentsSeq;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kContentsSeq, m_poiPtOrgIndeces, &contentsSeq))
			return false;
		// 名称取得
		CComVariant name;
		if (! ::common_proc::GetValue((_IRowPtr)tgtPoiPt.second.m_T, poi_point_org::kTableName, poi_point_org::kName, m_poiPtOrgIndeces, &name))
			return false;
		IGeometryPtr ipGeometry;
		if (FAILED((tgtPoiPt.second.m_T)->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), poi_point_org::kTableName, tgtPoiPt.first);
			return false;
		}
		// POI情報マップへのレコード追加
		POIINFO poiInfo;
		poiInfo.e_objectid		= tgtPoiPt.first;
		poiInfo.e_name			= name.bstrVal;
		poiInfo.e_nameLength	= poiInfo.e_name.GetLength();
		poiInfo.e_srcID.Format(_T("%ld-%ld-%s"), (contentsCode.lVal / 100000), (contentsCode.lVal % 100000), contentsSeq.bstrVal);	// コンテンツコードを下位5桁と上位桁（4～5桁）に分離
		poiInfo.e_ipGeometry	= ipGeometry;
		poiInfos[std::to_string(contentsCode.lVal).c_str()].insert(poiInfo);
	}

	// 対象となる注記POIポイントが存在しない場合は終了
	if (0 == poiInfos.size())
		return true;

	// 優先度リストをもとに、１つの補完用注記POIポイントを選択
	for (const auto& poiCode : m_supPoiPriList) {
		// m_poiPriListは優先度順に格納されているので、見つかり次第終了
		std::map<CString, std::set<POIINFO>>::const_iterator tgtSrcItr = poiInfos.find(poiCode);
		if (tgtSrcItr != poiInfos.end()) {
			// std::set<POIINFO>は文字列の短い順（文字列の長さが一緒の場合はobjectid順）で格納されているので1個目の要素が対象
			std::set<POIINFO>::const_iterator poiInfoItr = tgtSrcItr->second.begin();
			bldNamePtInfo->e_name		= poiInfoItr->e_name;
			bldNamePtInfo->e_bldOID		= bldID;
			bldNamePtInfo->e_srcType	= ::buildingname_point::source_type::kCompAnnoPOI;
			bldNamePtInfo->e_srcID		= poiInfoItr->e_srcID;
			bldNamePtInfo->e_srcOID		= poiInfoItr->e_objectid;
			bldNamePtInfo->e_priority	= ::buildingname_point::priority_code::kCompAnnoPoi;
			bldNamePtInfo->e_matchPer	= -1;
			bldNamePtInfo->e_ipGeometry	= poiInfoItr->e_ipGeometry.m_T;
			*result = true;
			// 残りのobjectidをログに出力する。
			++poiInfoItr;
			for (; poiInfoItr != tgtSrcItr->second.end(); ++poiInfoItr) {
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, _T("同一POIジャンルが複数存在した為、文字列の長さ or objectid順で対象外になりました"), poi_point_org::kTableName, poiInfoItr->e_objectid);
			}
			break;
		}
	}

	return true;
}
