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
#include "BDNCooker.h"
#include "common.h"

BDNCooker::BDNCooker(const FieldIndexMap* bldNamePtIndeces)
	: m_bldNamePtIndeces(bldNamePtIndeces)
{
}

BDNCooker::~BDNCooker(void)
{
}

bool BDNCooker::cookBDN(
	const IGeometryPtr& ipMeshGeometry,
	const IDFeatureMap& bldFeatures,
	const IDFeatureMap& bldNamePtFeatures,
	IDFeatureMap* bldNamePtIDs,
	IDRecordMap* cnvInfo,
	bool* existWrongData
	)
{
	*existWrongData = true;
	bool retval = true;
	// メッシュ内外判定のため、メッシュ矩形のmin, max取得
	LONLAT meshMin, meshMax;
	if (! common_proc::GetMinMax(ipMeshGeometry, &meshMin, &meshMax))
		return false;
	// 建物基準で処理
	for (const auto& bldInfo :bldFeatures)
	{
		// 形状チェック用に穴埋め形状作成
		IGeometryPtr ipInfillBldGeometry;
		if (! makeInfillBldGeometry(bldInfo, ipInfillBldGeometry))
			return false;
		// 建物IDから建物名称ポイント検索
		bool relExist = false;
		bool posChk = true;
		if (! getRelatedBldNamePt(bldInfo.first, ipInfillBldGeometry, bldNamePtFeatures, meshMin, meshMax, bldNamePtIDs, cnvInfo, &relExist, &posChk))
			return false;
		// 建物名称ポイントが建物上に存在しなかった場合、retval=falseとする。(処理は続けたいがエラーが存在したことを保持したいため)
		if (! posChk)
			*existWrongData = false;
	}
	// 建物名称ポイント情報で使用されていないものがあるようであったらメッセージを出力する。
	for (const auto bldNamePtID : *bldNamePtIDs)
	{
		// 変換対象外ポイントであれば出力しない(変換対象のメッシュで出力されるので)
		IGeometryPtr ipGeometry;
		if (FAILED(bldNamePtID.second->get_ShapeCopy(&ipGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first);
			return false;
		}
		bool isCnv = false;
		if (! chkPointCnv(sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first, ipGeometry, meshMin, meshMax, &isCnv))
			return false;
		if (isCnv) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::notice, _T("紐づく建物が存在しませんでした。本オブジェクトを変換対象外とします。"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID.first);
			*existWrongData = false;
		}
	}
	return retval;
}

bool BDNCooker::makeInfillBldGeometry(const IDFeatureMap::value_type& bldInfo, IGeometryPtr& ipInfillBldGeometry)
{
	// 建物形状取得
	IGeometryPtr ipBldGeometry;
	if (FAILED(bldInfo.second->get_ShapeCopy(&ipBldGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// 中抜き形状を考慮し、ExteriorRingを取得する。
	long exteriorCount = -1;
	if (FAILED(((IPolygonPtr)ipBldGeometry)->get_ExteriorRingCount(&exteriorCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRing数の取得に失敗しました"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// ExteriorRingが1でなかったらエラー
	if (exteriorCount != 1) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRingが複数存在します"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	IRingPtr ipRing;
	if (FAILED(((IPolygonPtr)ipBldGeometry)->QueryExteriorRings(&ipRing))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ExteriorRingの取得に失敗しました"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// 建物形状の空間参照取得
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipBldGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の取得に失敗しました"), sindy::schema::building::kTableName, bldInfo.first);
		return false;
	}
	// Ring2Polygon
	IPolygonPtr ipPolygon;
	if (! ring2Polygon(bldInfo.first, ipRing, ipSpRef, ipPolygon))
		return false;
	ipInfillBldGeometry = (IGeometryPtr)ipPolygon;

	return true;

}

bool BDNCooker::getRelatedBldNamePt(
	long bldID,
	const IGeometryPtr& ipInfillBldGeometry,
	const IDFeatureMap& bldNamePtFeatures,
	const LONLAT& meshMin,
	const LONLAT& meshMax,
	IDFeatureMap* bldNamePtIDs, 
	IDRecordMap* cnvInfo,
	bool* relExist,
	bool* posChk
	)
{
	*relExist = false;
	*posChk = true;
	// 建物のOBJECTIDで建物ビル名称検索
	IDFeatureMap::const_iterator itr = bldNamePtFeatures.find(bldID);
	if (itr == bldNamePtFeatures.end()) 
		return true;			// 関連付く建物ビル名称は存在しないので終了
	// 紐づいた建物ビル名称ポイントのobjectid取得
	long bldNamePtID = -1;
	if (FAILED(itr->second->get_OID(&bldNamePtID))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("オブジェクトID取得に失敗しました"), sindy::schema::sj::buildingname_point::kTableName);
		return false;
	}
	// 紐づいた建物ビル名称ポイントIDを引数で取得した処理対象ID群から削除する。
	bldNamePtIDs->erase(bldNamePtID);

	// ビル名称ポイントのジオメトリ取得
	IGeometryPtr ipBldNamePtGeometry;
	if (FAILED(itr->second->get_ShapeCopy(&ipBldNamePtGeometry))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状の取得に失敗しました"), sindy::schema::sj::buildingname_point::kTableName, bldNamePtID);
		return false;
	}
	// ポイントが基準となる建物内に存在するかチェック
	VARIANT_BOOL disjointChk = VARIANT_FALSE;
	IRelationalOperatorPtr ipRel(ipInfillBldGeometry);
	if (FAILED(ipRel->Disjoint(ipBldNamePtGeometry, &disjointChk))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("建物ビル名とのDISJOINTチェックに失敗しました"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	if (disjointChk == VARIANT_TRUE) {
		CString noticeMsg;
		noticeMsg.Format(_T("紐づいている家形上に形状が存在しません。本オブジェクトを変換対象外とします。:建物ID=%d"), itr->first); 
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::notice, noticeMsg, sindy::schema::sj::buildingname_point::kTableName, bldNamePtID);
		*posChk = false;
		return true;
	}
	// ここまで来たら建物とID・位置共に紐づく建物名称ポイントが存在することとなるのでrelExistをtrueとする。
	*relExist = true;
	
	// ポイントが変換対象かチェック(メッシュ内に存在するかチェック)
	bool isCnv = false;
	if (! chkPointCnv(sindy::schema::sj::buildingname_point::kTableName, bldNamePtID, ipBldNamePtGeometry, meshMin, meshMax, &isCnv))
		return false;

	// 変換対象の場合は、建物ID・建物名称を登録
	if (isCnv) {
		// 建物名称取得
		CComVariant name;
		if (! common_proc::GetValue(itr->second, sindy::schema::sj::buildingname_point::kTableName, sindy::schema::sj::buildingname_point::kName, *m_bldNamePtIndeces, &name))
			return false;
		// 名称が空の場合を除いて、コンテナ挿入
		CString bldName = name.bstrVal;
		if (! bldName.IsEmpty()) {
			RECORDINFO recordInfo(bldID, bldName, ipBldNamePtGeometry);
			cnvInfo->insert(IDRecordMap::value_type(bldID, recordInfo));
		}
	}
	return true;
}

bool BDNCooker::ring2Polygon(long bldID, const IRingPtr& ipRing, const ISpatialReferencePtr& ipSpRef, IPolygonPtr& ipPolygon)
{
	// リングが閉じているか,右回りかチェック
	VARIANT_BOOL isClosed, isExterior;
	if (FAILED(ipRing->get_IsClosed(&isClosed)) || isClosed == VARIANT_FALSE ||
		FAILED(ipRing->get_IsExterior(&isExterior) || isExterior == VARIANT_FALSE)){
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング形状が不正です"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	IPointCollectionPtr ipPointCol(ipRing);
	long pointCount;
	if (FAILED(ipPointCol->get_PointCount(&pointCount))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リングのポイント数取得に失敗しました"), sindy::schema::building::kTableName, bldID);
		return false;
	}
	WKSPoint* wksPoints = new WKSPoint[pointCount];
	if (FAILED(ipPointCol->QueryWKSPoints(0, pointCount, wksPoints))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング→ポイント群抽出に失敗しました"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	IPointCollectionPtr ipNewPointCol(CLSID_Polygon);
	if (FAILED(((IGeometryPtr)ipNewPointCol)->putref_SpatialReference(ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング→ポリゴンの際の空間参照付与に失敗しました"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	if (FAILED(ipNewPointCol->AddWKSPoints(pointCount, wksPoints))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("リング→ポリゴンへの変換に失敗しました"), sindy::schema::building::kTableName, bldID);
		delete [] wksPoints;
		return false;
	}
	delete [] wksPoints;
	IPolygonPtr ipTemp(ipNewPointCol);
	ipPolygon = ipTemp;
	return true;
}

bool BDNCooker::chkPointCnv(LPCTSTR tableName, long id, const IGeometryPtr& ipPtGeometry, const LONLAT& meshMin, const LONLAT& meshMax, bool* isCnv)
{
	// ポイントフィーチャの座標取得
	IPointPtr ipPoint(ipPtGeometry);
	LONLAT ptLonLat;
	if (FAILED(ipPoint->get_X(&ptLonLat.e_lon)) || FAILED(ipPoint->get_Y(&ptLonLat.e_lat))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("ポイント座標の取得に失敗しました"), tableName, id);
		return false;
	}
	// ポイント座標がメッシュ矩形に含まれるかチェック
	if ((ptLonLat.e_lon < meshMin.e_lon) || (ptLonLat.e_lat < meshMin.e_lat) ||
		(ptLonLat.e_lon > meshMax.e_lon) || (ptLonLat.e_lat > meshMax.e_lat)) {
		// メッシュの外側にいるケースは変換対象外
		*isCnv=false;
	} else if ((ptLonLat.e_lon > meshMin.e_lon) && (ptLonLat.e_lat > meshMin.e_lat) &&
		       (ptLonLat.e_lon < meshMax.e_lon) && (ptLonLat.e_lat < meshMax.e_lat)) {
		// メッシュの内側なので変換対象
		*isCnv=true;
	} else if ((ptLonLat.e_lon == meshMin.e_lon) || (ptLonLat.e_lat == meshMin.e_lat)) {
		// メッシュ境界上の場合、最小メッシュで変換する。よって、左辺と底辺の場合は変換しない
		*isCnv=false;
	} else if ((ptLonLat.e_lon == meshMax.e_lon) || (ptLonLat.e_lat == meshMax.e_lat)) {
		// メッシュ境界上の場合、最小メッシュで変換する。よって、右辺と上辺の場合は変換する。(左辺と底辺との接続部は除く)
		*isCnv=true;
	} else {
		// ここまで来る場合、プログラムがおかしい
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::fatal, _T("プログラムが不正です。開発者にお問い合わせください。"), tableName, id);
		return false;
	}
	return true;
}
