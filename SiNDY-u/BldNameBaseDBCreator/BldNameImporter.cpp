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
#include "BldNameImporter.h"
#include "LogSys.h"
#include "common.h"
#include "ctime.h"

using namespace sindy::schema::sj;

BldNameImporter::BldNameImporter(
	const std::vector<long>& srcPriListPrior,
	const std::vector<long>& srcPriListMatch,
	const std::vector<CString>& poiPriList,
	const std::vector<CString>& supPoiPriList,
	const std::vector<StrPair>& replaceList,
	const std::set<long>& bldExcList,
	const std::map<long, long>& condList,
	const std::map<CString, long>& bldSrcIndeces,
	const std::map<CString, long>& bldAddInfoIndeces,
	const std::map<CString, long>& bldIndeces,
	const std::map<CString, long>& twnpageIndeces,
	const std::map<CString, long>& poiPtOrgIndeces,
	const std::map<CString, long>& bldNamePtIndeces,
	const std::map<CString, long>& relBldNameIndeces,
	const IFeatureClassPtr& ipBldSrcPt,
	const ITablePtr& ipBldAddInfo,
	const IFeatureClassPtr& ipBld,
	const IFeatureClassPtr& ipCityAdm,
	const IFeatureClassPtr& ipTwnpage,
	const IFeatureClassPtr& ipPoiPtOrg,
	const IFeatureClassPtr& ipBldNamePt,
	const ITablePtr& ipRelBldName,
	bool isUpdate,
	bool isSurvey,
	SurveyPointManager& surveyPtMgr
	)
	: m_bldExcList(bldExcList), m_bldIndeces(bldIndeces), m_twnpageIndeces(twnpageIndeces), m_poiPtOrgIndeces(poiPtOrgIndeces),
	m_bldSrcIndeces(bldSrcIndeces), m_bldAddInfoIndeces(bldAddInfoIndeces), m_bldNamePtIndeces(bldNamePtIndeces), m_relBldNameIndeces(relBldNameIndeces),
	m_ipBldSrcPt(ipBldSrcPt), m_ipBldAddInfo(ipBldAddInfo), m_ipBld(ipBld), m_ipCityAdm(ipCityAdm),
	m_ipTwnpage(ipTwnpage), m_ipPoiPtOrg(ipPoiPtOrg), m_ipBldNamePt(ipBldNamePt), m_ipRelBldName(ipRelBldName),
	m_isUpdate(isUpdate), m_isSuvey(isSurvey), m_surveyPtMgr( surveyPtMgr )
{
	m_matchingProcessor = new MatchingProcessor(srcPriListPrior, srcPriListMatch, poiPriList, supPoiPriList, replaceList, condList, bldSrcIndeces, poiPtOrgIndeces, m_mapAddrCodeCounter, isSurvey, surveyPtMgr);
}

BldNameImporter::~BldNameImporter(void)
{
	delete m_matchingProcessor;
}

bool BldNameImporter::Init()
{
	// 出典種別コード値リストを取得
	if (! ::common_proc::GetCodedValues(m_ipBldSrcPt, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, m_srcTypeCodedValues))
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("出典種別コード値リストの作成に失敗"), ::buildingname_src_point::kTableName); 
		return false;
	}

	// 「防火対象物情報」ポイント・20桁住所コードカウントリストの作成
	// Where句設定
	CString strWhere;
	strWhere.Format(_T("%s = %d"), ::buildingname_src_point::kSourceType, ::buildingname_src_point::source_type::kFirePreventive);
	IFeatureCursorPtr ipCurSrcPt;
	if (! ::common_proc::SearchByWhereClause(m_ipBldSrcPt, ipCurSrcPt, strWhere, true))
	{
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("20桁住所コードカウントリストの作成に失敗"), ::buildingname_src_point::kTableName); 
		return false;
	}
	IFeaturePtr ipSrcPt;
	while (SUCCEEDED(ipCurSrcPt->NextFeature(&ipSrcPt)) && ipSrcPt)
	{
		// 20桁住所コードを取得
		CComVariant addrCode;
		if (! ::common_proc::GetValue((_IRowPtr)ipSrcPt, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAddrCode, m_bldSrcIndeces, &addrCode))
		{
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, _T("20桁住所コードカウントリストの作成に失敗"), ::buildingname_src_point::kTableName); 
			return false;
		}
		CString strAddrCode(addrCode.bstrVal);
		// 当該20桁住所コードのカウンタを+1
		if (!strAddrCode.IsEmpty()) ++m_mapAddrCodeCounter[strAddrCode];
	}

	return true;
}

bool BldNameImporter::Import(const CString& addrCode)
{
	// インポート用各種レコード情報をクリア
	m_bldNamePtInfos.clear();
	m_relBldNameInfos.clear();
	m_deleteBldNamePtIDs.clear();

	// 5桁コードより対象行政界取得し、マージ形状取得
	std::cout << CT2A(addrCode) << "\t当該市区町村行政界マージ中" << std::endl;
	IGeometryPtr tgtAdmGeometry;
	if (! GetTgtCityAdms(addrCode, tgtAdmGeometry))
		return false;
	if (!tgtAdmGeometry) {
		CString errMsg;
		errMsg.Format(_T("処理対象行政界が存在しません。5桁コード：%s"), addrCode);
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::warning, errMsg, sindy::schema::city_admin::kTableName);
		return false;
	}

	// 行政界にintersectする建物とoverlapする建物のoidリスト取得
	std::cout << CT2A(addrCode) << "\t当該市区町村内建物ポリゴン取得中" << std::endl;
	IDFeatureMap intersectBldFeatures;
	std::set<long> overlapBldIDs;
	if (! GetTgtBldInfos(tgtAdmGeometry, &intersectBldFeatures, &overlapBldIDs))
		return false;

	// 建物分繰り返しし、DB更新情報取得
	long i = 0;
	for (const auto& bldFeatureInfo : intersectBldFeatures) {
		if (0 == i % 100)
			std::cout << CT2A(addrCode) << "\t名称ポイント処理中 [ " << std::setw(6) << i << " / " << std::setw(6) << intersectBldFeatures.size() << " ]\r";
		++i;
		if( !GetImportRecordInfos(bldFeatureInfo.first, bldFeatureInfo.second.m_T, overlapBldIDs))
			return false;
		// DB更新
		if (! DBUpdate(false))
			return false;
	}
	std::cout << CT2A(addrCode) << "\t名称ポイント処理中 [ " << std::setw(6) << i << " / " << std::setw(6) << intersectBldFeatures.size() << " ]" << std::endl;

	// DB更新（全件反映）
	if (! DBUpdate(true))
		return false;

	// 調査ポイントリストは行政界毎の処理の最後で出力する。
	if (m_isSuvey) {
		std::cout << CT2A(addrCode) << "\t調査対象ポイントリスト出力中" << std::endl;
		m_surveyPtMgr.output();
	}

	return true;
}

bool BldNameImporter::GetTgtCityAdms(const CString& addrCode, IGeometryPtr& tgtAdmGeometry)
{
	CString errMsg;

	// Where句作成
	CString whereClause;
	whereClause.Format(_T("%s = '%s'"), sindy::schema::city_admin::kCityCode, addrCode);
	// Cursor取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByWhereClause(m_ipCityAdm, ipFeatureCursor, whereClause))
		return false;

	// ジオメトリバッグ準備（空間参照付与）
	IGeometryCollectionPtr ipGeomColl(CLSID_GeometryBag);
	ISpatialReferencePtr ipSpRef;
	if(FAILED(IGeoDatasetPtr(m_ipCityAdm)->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の取得に失敗しました"), sindy::schema::city_admin::kTableName);
		return false;
	}
	if(FAILED(IGeometryPtr(ipGeomColl)->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の付与(GeometryBag)に失敗しました"), sindy::schema::city_admin::kTableName);
		return false;
	}

	// フィーチャ取得
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid取得
		long oid = -1;
		ipFeature->get_OID(&oid);
		// geometry取得
		IGeometryPtr ipGeometry;
		if (FAILED(ipFeature->get_Shape(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), sindy::schema::city_admin::kTableName, oid);
			return false;
		}
		if (FAILED(ipGeomColl->AddGeometry(ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の追加に失敗しました"), sindy::schema::city_admin::kTableName, oid);
			return false;
		}
	}

	// geometryが存在しなかった場合、NULLを渡して終了
	long countGeom = 0;
	IEnumGeometryPtr(ipGeomColl)->get_Count(&countGeom);
	if (0 == countGeom) {
		tgtAdmGeometry = nullptr;
		return true;
	}

	// geometriesマージ
	IPolygonPtr ipUnion(CLSID_Polygon);
	if(FAILED(ipUnion->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の付与(Polygon)に失敗しました"), sindy::schema::city_admin::kTableName);
		return false;
	}
	if (FAILED(ITopologicalOperatorPtr(ipUnion)->ConstructUnion(IEnumGeometryPtr(ipGeomColl)))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の統合に失敗しました"), sindy::schema::city_admin::kTableName);
		return false;
	}
	tgtAdmGeometry = ipUnion;

	return true;
}

bool BldNameImporter::GetTgtBldInfos(const IGeometryPtr& ipAdmGeometry, IDFeatureMap* intersectBldFeatures, std::set<long>* overlapBldIDs)
{
	CString errMsg;

	// overlap調査用にIRelationalOperatorを準備
	IRelationalOperatorPtr ipRel(ipAdmGeometry);

	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipBld, ipFeatureCursor, ipAdmGeometry, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// objectid取得
		long oid = -1;
		ipFeature->get_OID(&oid);
		// geometryの取得
		IGeometryPtr ipGeometry;
		if (FAILED(ipFeature->get_Shape(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), sindy::schema::building::kTableName, oid);
			return false;
		}
		// 行政界にcontainsしているか調べる
		VARIANT_BOOL containsChk = VARIANT_FALSE;
		if (FAILED(ipRel->Contains(ipGeometry, &containsChk))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("行政界とのContains判定に失敗しました"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (containsChk == VARIANT_TRUE) {
			// コンテナに登録
			intersectBldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
		} else {
			// 行政界にoverlapしているかチェック
			VARIANT_BOOL overlapChk = VARIANT_FALSE;
			if (FAILED(ipRel->Overlaps(ipGeometry, &overlapChk))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("行政界とのOverlaps判定に失敗しました"), sindy::schema::building::kTableName, oid);
				return false;
			}
			if (overlapChk == VARIANT_TRUE) {
				// overlapしていた場合、リストに登録してコンテナに登録
				overlapBldIDs->insert(oid);
				intersectBldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
			}
			// overlapもしていない場合は処理対象外(タッチしているということになる)
		}
	}
	return true;
}

bool BldNameImporter::GetImportRecordInfos(long bldID, const IFeaturePtr& ipBldFeature, const std::set<long>& overlapBldIDs)
{
	// 処理除外対象建物かチェックをする
	CComVariant bldClass;
	if (! ::common_proc::GetValue((_IRowPtr)ipBldFeature, sindy::schema::building::kTableName, sindy::schema::building::kBldClass, m_bldIndeces, &bldClass))
		return false;
	if (m_bldExcList.find(bldClass.lVal) != m_bldExcList.end())
		return true;	// 処理対象外

	// 行政界にoverlapした建物か調べる
	if (overlapBldIDs.find(bldID) != overlapBldIDs.end()) {
		// 変換済か調べる
		if (m_cnvdOverlapBldIDs.find(bldID) != m_cnvdOverlapBldIDs.end())
			return true;	// 変換済なので処理しない
		// 変換済IDとして登録する。
		m_cnvdOverlapBldIDs.insert(bldID);
		// 出力先テーブル(建物ビル名称ポイント)に現在処理している建物に紐づくポイントが存在するか調べる
		// 存在した場合、削除する。
		bool existEditPoint = false;
		if (! ChkExistBldNamePt(bldID, &existEditPoint))
			return false;
		if (existEditPoint)
			return true;	// 手作業で編集した建物ビル名称ポイントが存在する場合は、処理自体行わない
	}

	// 建物フィーチャから穴埋め済ポリゴンを取得
	IPolygonPtr ipFilledBldPolygon;
	if (! GetFilledPolygon(ipBldFeature, ipFilledBldPolygon))
		return false;

	// 建物形状内に存在する素材ポイント取得
	IDFeatureMap tgtSrcPtFeatures;
	if (! GetTgtSrcPt(ipFilledBldPolygon, &tgtSrcPtFeatures))
		return false;

	// 建物形状内に存在する注記POIポイント取得
	IDFeatureMap tgtPoiPtFeatures;
	if (! GetTgtPoiPt(ipFilledBldPolygon, &tgtPoiPtFeatures))
		return false;

	// マッチング処理
	BldNamePtInfo bldNamePtInfo;
	std::set<long> relIDs;

	bool result = false;
	bool excepted = false;
	bool selected = false;
	if (! m_matchingProcessor->MatchingProc(tgtSrcPtFeatures,
											tgtPoiPtFeatures,
											bldID,
											ipFilledBldPolygon,
											&bldNamePtInfo,
											&relIDs,
											&result,
											&excepted,
											&selected))
		return false;
	if (result)
	{
		m_bldNamePtInfos[bldNamePtInfo.e_srcOID] = bldNamePtInfo;
		if (relIDs.size() > 0)
			m_relBldNameInfos[bldNamePtInfo.e_srcOID] = relIDs;
	}
	else
	{
		// 調査対象ポイントリスト出力モードの場合、TOWNPAGEデータが存在するかチェックし、あれば調査対象ポイントとして取得する
		// 「除外建物」にマッチした場合は調査対象ポイントは生成しない
		// 素材ポイントを元に調査対象ポイント設定済みの場合も生成しない
		if (m_isSuvey && !excepted && !selected)
		{
			// 建物上のtownpageオブジェクト取得
			IDFeatureMap tgtTWNPt;
			if (! GetTgtAtInfillGeo(bldID, ipFilledBldPolygon, m_ipTwnpage, sindy::schema::townpage::kTableName, &tgtTWNPt))
				return false;
			// 一つも取得できなければ終了
			if (!tgtTWNPt.size())
				return true;
			// 取得したオブジェクトのうち代表ポイントを取得する。
			long headTwnID = -1;
			IFeaturePtr ipHeadTwnFeature;
			if (! ::common_proc::SelectHeadNameFeature(tgtTWNPt, sindy::schema::townpage::kTableName, sindy::schema::townpage::kName, m_twnpageIndeces, &headTwnID, ipHeadTwnFeature))
				return false;
			// townpageオブジェクトから調査ポイント情報取得
			if( !m_surveyPtMgr.addFromTownpage( headTwnID, bldID, ipHeadTwnFeature ) )
				return false;
		}
	}

	return true;
}

bool BldNameImporter::DBUpdate(bool flushAll)
{
	// DB更新モードでない場合はコンテナをクリアしてすぐ戻る
	if (!m_isUpdate) {
		m_bldNamePtInfos.clear();
		m_relBldNameInfos.clear();
		m_deleteBldNamePtIDs.clear();
		return true;
	}

	// 全件反映モードではなく、いずれのコンテナも一定の件数に満たない場合はまだ溜め込む
	if (!flushAll && ((m_bldNamePtInfos.size() < FETCHNUM) && (m_relBldNameInfos.size() < FETCHNUM) && (m_deleteBldNamePtIDs.size() < FETCHNUM))) {
		return true;
	}

	// 建物名称削除
	if (m_deleteBldNamePtIDs.size() > 0) {
		if (! DeleteExistRecord())
			return false;
	}

	// 建物名称・建物ビル名称紐づきレコード登録
	IFeatureCursorPtr ipBldNameCursor;
	if (FAILED(m_ipBldNamePt->Insert(VARIANT_TRUE, &ipBldNameCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("インポート用フィーチャカーソル取得に失敗しました"), ::buildingname_point::kTableName);
		return false;
	}
	_ICursorPtr ipRelBldNameCursor;
	if (FAILED(m_ipRelBldName->Insert(VARIANT_TRUE, &ipRelBldNameCursor))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("インポート用カーソル取得に失敗しましました"), ::rel_buildingname::kTableName);
		return false;
	}

	for (const auto& bldNamePtInfo : m_bldNamePtInfos) {
		// 付加情報レコードの取得
		BldNameAddInfo bldNameAddInfo;
		if(! GetAddInfo(bldNamePtInfo.second, bldNameAddInfo))
			return false;
		// 建物ビル名称へのオブジェクト挿入
		long bldNamePtID = -1;
		if (! InsertBldNamePt(bldNamePtInfo.second, bldNameAddInfo, ipBldNameCursor, &bldNamePtID))
			return false;
		// 建物ビル名称紐づきテーブルへのオブジェクト挿入
		if (! InsertRelBldName(bldNamePtID, bldNamePtInfo.first, ipRelBldNameCursor))
			return false;
	}

	// フラッシュ
	if (FAILED(ipBldNameCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フラッシュに失敗しました"), ::buildingname_point::kTableName);
		return false;
	}
	if (FAILED(ipRelBldNameCursor->Flush())) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("フラッシュに失敗しました"), ::rel_buildingname::kTableName);
		return false;
	}

	m_bldNamePtInfos.clear();
	m_relBldNameInfos.clear();
	m_deleteBldNamePtIDs.clear();

	return true;
}

bool BldNameImporter::ChkExistBldNamePt(long bldID, bool* existEditPoint)
{
	CString errMsg;
	*existEditPoint = false;

	// Where句作成
	CString whereClause;
	whereClause.Format(_T("%s = %d"), ::buildingname_point::kBuildingOID, bldID);
	// Cursor取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByWhereClause(m_ipBldNamePt, ipFeatureCursor, whereClause))
		return false;

	// フィーチャ取得
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		// 信頼コードが1かチェック
		CComVariant priority;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_point::kTableName, ::buildingname_point::kPriority, m_bldNamePtIndeces, &priority))
			return false;
		if (priority.lVal == ::buildingname_point::priority_code::kEdit) {
			// 信頼コードが1の場合は削除しない。
			*existEditPoint = true;
			continue;
		}
		// objectid取得
		long oid;
		ipFeature->get_OID(&oid);
		// 削除IDとして登録
		m_deleteBldNamePtIDs.insert(oid);
	}
	return true;
}

bool BldNameImporter::GetFilledPolygon(IFeaturePtr ipTgtFeature, IPolygonPtr& ipFilledPolygon)
{
	// OID取得
	long tgtID;
	ipTgtFeature->get_OID(&tgtID);

	// テーブル名取得
	ITablePtr ipTable;
	ipTgtFeature->get_Table(&ipTable);
	CComBSTR bstrName;
	((IDatasetPtr)ipTable)->get_Name(&bstrName);
	CString strTableName(bstrName);

	// ジオメトリ取得
	IGeometryPtr ipTgtGeometry;
	if (FAILED(ipTgtFeature->get_ShapeCopy(&ipTgtGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), strTableName, tgtID);
		return false;
	}
	// ExteriorRing数の取得
	long exteriorCount = -1;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->get_ExteriorRingCount(&exteriorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing数の取得に失敗しました"), strTableName, tgtID);
		return false;
	}
	// ExteriorRingが1でなかったらエラー
	if (exteriorCount != 1) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRingが複数存在します"), strTableName, tgtID);
		return false;
	}
	// ExteriorRingの取得
	IRingPtr ipRing;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->QueryExteriorRings(&ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRingの取得に失敗しました"), strTableName, tgtID);
		return false;
	}
	// InteriorRing数の取得
	long interiorCount = -1;
	if (FAILED(((IPolygonPtr)ipTgtGeometry)->get_InteriorRingCount(ipRing, &interiorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("InteriorRing数の取得に失敗しました"), strTableName, tgtID);
		return false;
	}
	// InteriorRingが0の場合、穴埋めの必要がないので元の形状を返す
	if (0 == interiorCount) {
		ipFilledPolygon = ipTgtGeometry;
		return true;
	}
	// リングが閉じているか・右回りかをチェック
	VARIANT_BOOL isClosed, isExterior;
	if (FAILED(ipRing->get_IsClosed(&isClosed)) || isClosed == VARIANT_FALSE ||
		FAILED(ipRing->get_IsExterior(&isExterior) || isExterior == VARIANT_FALSE)){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング形状が不正です"), strTableName, tgtID);
		return false;
	}
	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipTgtGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の取得に失敗しました"), strTableName, tgtID);
		return false;
	}
	IPolygonPtr ipNewPolygon(CLSID_Polygon);
	// 穴埋め済ポリゴンに空間参照を付与
	if (FAILED(ipNewPolygon->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング→ポリゴンの際の空間参照付与に失敗しました"), strTableName, tgtID);
		return false;
	}
	// 穴埋め済ポリゴンにリングを追加
	if (FAILED(IGeometryCollectionPtr(ipNewPolygon)->AddGeometry(ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ポリゴンへのリングの追加に失敗しました"), strTableName, tgtID);
		return false;
	}
	ipFilledPolygon = ipNewPolygon;

	return true;
}

bool BldNameImporter::GetTgtSrcPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtSrcPtFeatures)
{
	CString errMsg;

	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipBldSrcPt, ipFeatureCursor, ipFilledBldPolygon, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid取得
		long oid = -1;
		ipFeature->get_OID(&oid);
		// 出典種別コードと採用種別コードを取得
		CComVariant sourceType, adoption;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kSourceType, m_bldSrcIndeces, &sourceType))
			return false;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, ::buildingname_src_point::kTableName, ::buildingname_src_point::kAdoption, m_bldSrcIndeces, &adoption))
			return false;
		// 出典種別コードに不正のものが無いかチェック
		// （コード値ドメインに含まれないか、「102: 補完用注記POI」の場合は不正）
		if (m_srcTypeCodedValues.end() == m_srcTypeCodedValues.find(sourceType.lVal) || ::buildingname_src_point::source_type::kCompAnnoPOI == sourceType.lVal)
		{
			errMsg.Format(_T("%sが不正です : %ld"), ::buildingname_src_point::kSourceType, sourceType.lVal);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::buildingname_src_point::kTableName, oid);
			return false;
		}
		// 出典種別コードが「0: 独自調査」 or 採用種別コードが「0: 採用」の場合に、マッチング対象として素材ポイント群に挿入
		if( buildingname_src_point::source_type::kOwnSurvey == sourceType.lVal || buildingname_src_point::adoption_code::kAdopt == adoption.lVal )
		{
			tgtSrcPtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
		}
	}

	return true;
}

bool BldNameImporter::GetTgtPoiPt(IPolygonPtr ipFilledBldPolygon, IDFeatureMap* tgtPoiPtFeatures)
{
	CString errMsg;

	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(m_ipPoiPtOrg, ipFeatureCursor, ipFilledBldPolygon, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		using namespace sindy::schema::sindyk;

		// objectid取得
		long oid = -1;
		ipFeature->get_OID(&oid);

		// 名称取得
		CComVariant varName;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kName, m_poiPtOrgIndeces, &varName))
			return false;
		// 読み取得
		CComVariant varYomi;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kYomi, m_poiPtOrgIndeces, &varYomi))
			return false;
		// 閉鎖理由コード取得
		CComVariant varCloseC;
		if (! ::common_proc::GetValue((_IRowPtr)ipFeature, poi_point_org::kTableName, poi_point_org::kCloseCode, m_poiPtOrgIndeces, &varCloseC))
			return false;

		// 名称がNULL、もしくは名称が「ヌル」かつ読みが「ヨミヌル」の場合は除外する
		CString strName(varName.bstrVal), strYomi(varYomi.bstrVal);
		if (strName.IsEmpty() || (strName == _T("ヌル") && strYomi == _T("ヨミヌル"))) continue;

		// 閉鎖理由コードが「NULL」or「b（休止中）」でない場合は除外する
		CString strCloseC(varCloseC.bstrVal);
		if (!strCloseC.IsEmpty() && CA2T(CLOSE_SUSPENDED) != strCloseC) continue;

		tgtPoiPtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
	}

	return true;
}

bool BldNameImporter::GetTgtAtInfillGeo(long bldID, const IGeometryPtr& ipInfillBldGeometry, const IFeatureClassPtr& ipFeatureClass, LPCTSTR tableName, IDFeatureMap* tgtFeatures)
{
	CString errMsg;

	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! ::common_proc::SearchByGeometry(ipFeatureClass, ipFeatureCursor, ipInfillBldGeometry, esriSpatialRelIntersects))
		return false;

	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature) {
		long oid = -1;
		ipFeature->get_OID(&oid);
		// コンテナに格納
		tgtFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));
	}
	return true;
}

bool BldNameImporter::DeleteExistRecord()
{
	CString errMsg;
	CString whereClause;

	// あまり量が無いはずなので、一個ずつ消す
	for (const auto& bldNamePtID : m_deleteBldNamePtIDs) {

		// カーソル生成
		whereClause.Format(_T("%s=%d"), ::buildingname_point::kObjectID, bldNamePtID);
		IFeatureCursorPtr ipBldNamePtCursor;
		if (! ::common_proc::SearchByWhereClause(m_ipBldNamePt, ipBldNamePtCursor, whereClause))
			return false;

		// buildingname_pointの削除
		IFeaturePtr ipBldNameFeature;
		while (SUCCEEDED(ipBldNamePtCursor->NextFeature(&ipBldNameFeature)) && ipBldNameFeature) {
			// 建物ID取得
			CComVariant bldID;
			if (! ::common_proc::GetValue(ipBldNameFeature, ::buildingname_point::kTableName, ::buildingname_point::kBuildingOID, m_bldNamePtIndeces, &bldID))
				return false;
			if (FAILED(ipBldNameFeature->Delete()) /*|| FAILED(ipBldNameFeature->Store())*/) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("オブジェクト削除に失敗しました"), ::buildingname_point::kTableName, bldNamePtID);
				return false;
			} else {
				// 削除メッセージを念のため出しておく
				errMsg.Format(_T("処理対象建物上にオブジェクトが存在した為、削除しました。:bldid = %d"), bldID.lVal);
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, errMsg, ::buildingname_point::kTableName, bldNamePtID);
			}
		}

		// カーソル生成
		whereClause.Format(_T("%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
		_ICursorPtr ipRelBldNameCursor;
		if (! ::common_proc::SearchByWhereClause(m_ipRelBldName, ipRelBldNameCursor, whereClause))
			return false;

		_IRowPtr ipRelBldNameRow;
		while (SUCCEEDED(ipRelBldNameCursor->NextRow(&ipRelBldNameRow)) && ipRelBldNameRow) {
			if (FAILED(ipRelBldNameRow->Delete()) /*|| FAILED(ipRelBldNameRow->Store())*/) {
				errMsg.Format(_T("オブジェクト削除に失敗しました：%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, ::rel_buildingname::kTableName);
				return false;
			} else {
				// 削除メッセージを念のため出しておく
				CString procMsg;
				procMsg.Format(_T("処理対象建物上に建物名称オブジェクトが存在した為、関連レコードを削除しました：%s=%d"), ::rel_buildingname::kBuildingNameOID, bldNamePtID);
				LogSys::GetInstance().WriteFreeStyleLog(false, err_level::notice, procMsg, ::rel_buildingname::kTableName);
			}
		}
	}

	return true;
}

bool BldNameImporter::InsertBldNamePt(const BldNamePtInfo& bldNamePtInfo, const BldNameAddInfo& bldNameAddInfo, IFeatureCursorPtr& ipBldNamePtCursor,  long* bldNamePtID)
{
	IFeatureBufferPtr ipFeatureBuffer;
	if (FAILED(m_ipBldNamePt->CreateFeatureBuffer(&ipFeatureBuffer))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("インポート用フィーチャバッファ取得に失敗しました"), ::buildingname_point::kTableName);
		return false;
	}
	// 各種属性セット
	// OPERATOR
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kOperator,		m_bldNamePtIndeces, CComVariant(TOOLNAME)))
		return false;
	// PURPOSE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kPurpose,		m_bldNamePtIndeces, CComVariant(0)))
		return false;
	// MODIFYDATEはNULL
	// UPDATETYPE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kUpdateType,		m_bldNamePtIndeces, CComVariant(::buildingname_point::update_type::kImported)))
		return false;
	// PROGMODIFYDATE
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kProgModifyDate,	m_bldNamePtIndeces, CComVariant(GetTimeText(false).c_str())))
		return false;
	// MODIFYPROGNAME
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kModifyProgName,	m_bldNamePtIndeces, CComVariant(TOOLNAME)))
		return false;
	// USERCLAIM_Fは初期値, SOURCEはNULL
	// NAME
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kName,			m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_name)))
		return false;
	// BUILDING_OID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBuildingOID,	m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_bldOID)))
		return false;
	// SOUCETYPE_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSourceType,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_srcType)))
		return false;
	// SRC_OID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSrcOID,			m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_srcOID)))
		return false;
	// SURVEYDATEはNULL
	// ANYPLACE_ID
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kAnyPlaceID,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_anyPlaceID)))
		return false;
	// PRIORITY_C
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kPriority,		m_bldNamePtIndeces, CComVariant(bldNamePtInfo.e_priority)))
		return false;
	// MATCH_PER
	// 最大マッチング率を10毎の単位にする。(10,20,30,・・・,90,100) -1の場合はそのまま
	long matchPer = ( -1 == bldNamePtInfo.e_matchPer ) ? -1 : ( long( bldNamePtInfo.e_matchPer / 10 ) * 10 );
	if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kMatchPer,		m_bldNamePtIndeces, CComVariant(matchPer)))
		return false;
	// CONTENTS_CODE
	long contentsCode = bldNamePtInfo.GetContentsCode();
	if (contentsCode >= 0) {
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kContentsCode,	m_bldNamePtIndeces, CComVariant(contentsCode)))
			return false;
	}
	// CONTENTS_SEQ
	long contentsSeq = bldNamePtInfo.GetContentsSeq();
	if (contentsSeq >= 0) {
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kContentsSeq,	m_bldNamePtIndeces, CComVariant(contentsSeq)))
			return false;
	}

	// 付加情報（名称情報が格納されている場合は有効と判断）
	if (! bldNameAddInfo.e_name.IsEmpty())
	{
		// NAME_SEPARATED
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kNameSeparated,	m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_nameSep)))
			return false;
		// YOMI_SEPARATED
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kYomiSeparated,	m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_yomiSep)))
			return false;
		// BLDGCLASS1_C
		if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass1,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass1)))
			return false;
		// BLDGCLASS2_C
		if (0 < bldNameAddInfo.e_bldgClass2)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass2,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass2)))
				return false;
		}
		// BLDGCLASS3_C
		if (0 < bldNameAddInfo.e_bldgClass3)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kBldgClass3,		m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_bldgClass3)))
				return false;
		}
		// FLOORS
		if (0 < bldNameAddInfo.e_floors)
		{
			if (! ::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kFloors,			m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_floors)))
				return false;
		}
	}
	// ビル名称妥当フラグ、種別確定コードはNULL不許可のため明示的に設定する（設定値がなければ初期値（0）が格納される）
	if (!::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kSuitable, m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_suitable)))
		return false;

	if (!::common_proc::SetValue(ipFeatureBuffer, ::buildingname_point::kTableName, ::buildingname_point::kFixClass, m_bldNamePtIndeces, CComVariant(bldNameAddInfo.e_fixclass)))
		return false;


	// 形状付与
	if (FAILED(ipFeatureBuffer->putref_Shape(bldNamePtInfo.e_ipGeometry.m_T))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状付与に失敗しました"), ::buildingname_point::kTableName);
		return false;
	}
	// オブジェクト登録
	CComVariant id;
	if (FAILED(ipBldNamePtCursor->InsertFeature(ipFeatureBuffer, &id))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("オブジェクト登録に失敗しました"), ::buildingname_point::kTableName);
		return false;
	}
	*bldNamePtID = id.lVal;
	return true;
}

bool BldNameImporter::InsertRelBldName(long bldNamePtID, long bldSrcPtID, _ICursorPtr& ipRelBldNameCursor)
{
	std::map<long, std::set<long>>::const_iterator itr = m_relBldNameInfos.find(bldSrcPtID);
	if (itr == m_relBldNameInfos.end()) {
		// 関連素材が存在しないので終了
		return true;
	}

	for (const auto& srcOID : itr->second) {
		IRowBufferPtr ipRowBuffer;
		if (FAILED(m_ipRelBldName->CreateRowBuffer(&ipRowBuffer))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("インポート用rowバッファ取得に失敗しました"), ::rel_buildingname::kTableName);
			return false;
		}
		// BUILDINGNAME_OID
		if (! ::common_proc::SetValue(ipRowBuffer, ::rel_buildingname::kTableName, ::rel_buildingname::kBuildingNameOID,	m_relBldNameIndeces, CComVariant(bldNamePtID)))
			return false;
		// SRC_OID
		if (! ::common_proc::SetValue(ipRowBuffer, ::rel_buildingname::kTableName, ::rel_buildingname::kSrcOID,				m_relBldNameIndeces, CComVariant(srcOID)))
			return false;
		// オブジェクト登録
		CComVariant id;
		if (FAILED(ipRelBldNameCursor->InsertRow(ipRowBuffer, &id))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("オブジェクト登録に失敗しました"), ::rel_buildingname::kTableName);
			return false;
		}
	}
	return true;
}

bool BldNameImporter::GetAddInfo(const BldNamePtInfo& bldNamePtInfo, BldNameAddInfo& bldNameAddInfo)
{
	// Where句の生成
	CString strTmp1, strTmp2, strTmp3;
	strTmp1.Format(_T("%s = %d"), ::buildingname_src_addinfo::kSourceType, bldNamePtInfo.e_srcType);
	// 注記POI（特定カテゴリ・補完カテゴリ）以外の場合のみ「SOURCENAME」を条件に加える
	switch(bldNamePtInfo.e_srcType)
	{
	case ::buildingname_src_point::source_type::kAnnoPOI:		// 注記POI（特定カテゴリ）
	case ::buildingname_src_point::source_type::kCompAnnoPOI:	// 注記POI（補完カテゴリ）
		break;
	default:	// その他
		strTmp2.Format(_T(" AND %s = '%s'"), ::buildingname_src_addinfo::kSourceName, bldNamePtInfo.e_srcName);
		break;
	}
	strTmp3.Format(_T(" AND %s = '%s' AND %s = '%s'"), ::buildingname_src_addinfo::kSourceID, bldNamePtInfo.e_srcID, ::buildingname_src_addinfo::kName, bldNamePtInfo.e_name);
	CString strWhere = strTmp1 + strTmp2 + strTmp3;

	// 検索
	_ICursorPtr ipCurAddInfo;
	if (! ::common_proc::SearchByWhereClause(m_ipBldAddInfo, ipCurAddInfo, strWhere))
		return false;
	_IRowPtr ipAddInfoRow;
	if (SUCCEEDED(ipCurAddInfo->NextRow(&ipAddInfoRow)) && ipAddInfoRow)	// 1件だけしか見つからないはず
	{
		// NAME取得
		CComVariant varName;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kName, m_bldAddInfoIndeces, &varName))
			return false;
		bldNameAddInfo.e_name = varName.bstrVal;
		// NAME_SEPARATED取得
		CComVariant varNameSep;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kNameSeparated, m_bldAddInfoIndeces, &varNameSep))
			return false;
		bldNameAddInfo.e_nameSep = varNameSep.bstrVal;
		// YOMI_SEPARATED取得
		CComVariant varYomiSep;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kYomiSeparated, m_bldAddInfoIndeces, &varYomiSep))
			return false;
		bldNameAddInfo.e_yomiSep = varYomiSep.bstrVal;
		// BLDGCLASS1_C取得
		CComVariant varBldgClass1;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass1, m_bldAddInfoIndeces, &varBldgClass1))
			return false;
		bldNameAddInfo.e_bldgClass1 = varBldgClass1.lVal;
		// BLDGCLASS2_C取得
		CComVariant varBldgClass2;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass2, m_bldAddInfoIndeces, &varBldgClass2))
			return false;
		if (VT_I4 == varBldgClass2.vt)	bldNameAddInfo.e_bldgClass2 = varBldgClass2.lVal;
		// BLDGCLASS3_C取得
		CComVariant varBldgClass3;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kBldgClass3, m_bldAddInfoIndeces, &varBldgClass3))
			return false;
		if (VT_I4 == varBldgClass3.vt)	bldNameAddInfo.e_bldgClass3 = varBldgClass3.lVal;
		// FLOORS取得
		CComVariant varFloors;
		if (! ::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kFloors, m_bldAddInfoIndeces, &varFloors))
			return false;
		if (VT_I4 == varFloors.vt)		bldNameAddInfo.e_floors = varFloors.lVal;

		// SUITABLE_F取得
		CComVariant varSuitable;
		if (!::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kSuitable, m_bldAddInfoIndeces, &varSuitable))
			return false;
		if (VT_I4 == varSuitable.vt)		bldNameAddInfo.e_suitable = varSuitable.lVal;

		// FIXCLASS_C取得
		CComVariant varFixClass;
		if (!::common_proc::GetValue(ipAddInfoRow, ::buildingname_src_addinfo::kTableName, ::buildingname_src_addinfo::kFixClass, m_bldAddInfoIndeces, &varFixClass))
			return false;
		if (VT_I4 == varFixClass.vt)		bldNameAddInfo.e_fixclass = varFixClass.lVal;
	}

	return true;
}
