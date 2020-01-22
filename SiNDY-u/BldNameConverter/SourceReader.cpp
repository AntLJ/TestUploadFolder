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
#include "SourceReader.h"

SourceReader::SourceReader(
	const IFeatureClassPtr& ipBldNamePt,
	const IFeatureClassPtr& ipBld,
	const FieldIndexMap* bldNamePtIndeces,
	const FieldIndexMap* bldIndeces,
	const std::set<long>* bldExcList
	)
	: m_ipBldNamePt(ipBldNamePt), m_ipBld(ipBld), m_bldNamePtIndeces(bldNamePtIndeces), m_bldIndeces(bldIndeces), m_bldExcList(bldExcList)
{
}

SourceReader::~SourceReader(void)
{
}

bool SourceReader::readSource(const IGeometryPtr& ipMeshGeometry, IDFeatureMap* bldFeatures, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData)
{
	// メッシュ矩形にインターセクトする建物とメッシュからはみ出た建物のエクステント取得
	IGeometryPtr ipExtentGeometry;
	if (! getTgtBldAndExtentGeometry(ipMeshGeometry, bldFeatures, ipExtentGeometry))
		return false;
	// エクステントにインターセクト建物名称取得
	if (! getTgtBldNamePt(ipExtentGeometry, bldNamePtFeatures, bldNamePtOIDs, existWrongData))
		return false;
	return true;
}

bool SourceReader::getTgtBldAndExtentGeometry(const IGeometryPtr ipMeshGeometry, IDFeatureMap* bldFeatures, IGeometryPtr& ipExtentGemetry)
{
	CString errMsg;
	// エクステント矩形用にメッシュ矩形のmin, max取得
	LONLAT minLonLat, maxLonLat;
	if (! common_proc::GetMinMax(ipMeshGeometry, &minLonLat, &maxLonLat))
		return false;
	// overlap調査用にIRelationalOperatorを準備
	IRelationalOperatorPtr ipRel(ipMeshGeometry);
	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetRelFeatureCursor(sindy::schema::building::kTableName, m_ipBld, ipMeshGeometry, esriSpatialRelIntersects, ipFeatureCursor))
		return false;
	// フィーチャ取得
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid取得
		long oid = -1;
		if (FAILED(ipFeature->get_OID(&oid))) {
			errMsg.Format(_T("%sの取得に失敗しました"), sindy::schema::building::kObjectID);
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, errMsg, sindy::schema::building::kTableName);
			return false;
		}
		// 処理対象かチェック
		CComVariant bldClass;
		if (! common_proc::GetValue((_IRowPtr)ipFeature, sindy::schema::building::kTableName, sindy::schema::building::kBldClass, *m_bldIndeces, &bldClass))
			return false;
		if (m_bldExcList->find(bldClass.lVal) != m_bldExcList->end()) 
			continue;	// 変換対象外
		// コンテナにインサート
		bldFeatures->insert(IDFeatureMap::value_type(oid, ipFeature));

		// メッシュ矩形にcontainしている場合、エクステント用の座標取得
		VARIANT_BOOL containChk = VARIANT_FALSE;
		IGeometryPtr ipBldGeometry;
		if (FAILED(ipFeature->get_ShapeCopy(&ipBldGeometry))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("形状取得に失敗しました"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (FAILED(ipRel->Contains(ipBldGeometry, &containChk))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("メッシュ矩形とのcontain判定に失敗しました"), sindy::schema::building::kTableName, oid);
			return false;
		}
		if (containChk == VARIANT_FALSE) {
			// エクステントした建物の最大・最小座標取得
			IGeometryPtr ipBldGeometry;
			if (FAILED(ipFeature->get_ShapeCopy(&ipBldGeometry))) {
				LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("建物形状の取得に失敗しました"), sindy::schema::building::kTableName, oid);
				return false;
			}
			LONLAT bldMinLonLat, bldMaxLonLat;
			if (! common_proc::GetMinMax(ipBldGeometry, &bldMinLonLat, &bldMaxLonLat))
				return false;
			// 最大(最小)座標のx,yが既存のものより大きい(小さい)場合は更新
			if (bldMinLonLat.e_lon < minLonLat.e_lon)
				minLonLat.e_lon = bldMinLonLat.e_lon;
			if (bldMinLonLat.e_lat < minLonLat.e_lat)
				minLonLat.e_lat = bldMinLonLat.e_lat;
			if (bldMaxLonLat.e_lon > maxLonLat.e_lon)
				maxLonLat.e_lon = bldMaxLonLat.e_lon;
			if (bldMaxLonLat.e_lat > maxLonLat.e_lat)
				maxLonLat.e_lat = bldMaxLonLat.e_lat;
		}
	}
	// 最終的な最大・最小座標からエンベロープGeometry作成
	// 空間参照取得
	ISpatialReferencePtr ipSpRef;
	if (FAILED(ipMeshGeometry->get_SpatialReference(&ipSpRef))) {
		LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("空間参照の取得に失敗しました"), sindy::schema::citymesh::kTableName);
		return false;
	}
	// エンベロープの作成を行う
	IEnvelopePtr ipEnvelope(CLSID_Envelope);
	if (FAILED(ipEnvelope->putref_SpatialReference(ipSpRef)) ||
		FAILED(ipEnvelope->PutCoords(minLonLat.e_lon, minLonLat.e_lat, maxLonLat.e_lon, maxLonLat.e_lat))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("エクステント矩形作成に失敗しました"), sindy::schema::citymesh::kTableName);
			return false;
	}
	ipExtentGemetry = (IGeometryPtr)ipEnvelope;
	return true;
}

bool SourceReader::getTgtBldNamePt(const IGeometryPtr ipExtentGeometry, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData)
{
	*existWrongData = true;
	// カーソル取得
	IFeatureCursorPtr ipFeatureCursor;
	if (! common_proc::GetRelFeatureCursor(sindy::schema::sj::buildingname_point::kTableName, m_ipBldNamePt, ipExtentGeometry, esriSpatialRelIntersects, ipFeatureCursor, _T("")))
		return false;
	// フィーチャ取得
	IFeaturePtr ipFeature;
	while (SUCCEEDED(ipFeatureCursor->NextFeature(&ipFeature)) && ipFeature)
	{
		// objectid取得
		long oid = -1;
		if (FAILED(ipFeature->get_OID(&oid))) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("オブジェクトIDの取得に失敗しました"), sindy::schema::sj::buildingname_point::kTableName);
			return false;
		}
		// 建物ID取得
		CComVariant bldID;
		if (! common_proc::GetValue((_IRowPtr)ipFeature, sindy::schema::sj::buildingname_point::kTableName, sindy::schema::sj::buildingname_point::kBuildingOID, *m_bldNamePtIndeces, &bldID))
			return false;
		// コンテナにインサート
		std::pair<IDFeatureMap::iterator, bool> result;
		result = bldNamePtFeatures->insert(IDFeatureMap::value_type(bldID.lVal, ipFeature));
		if (! result.second) {
			LogSys::GetInstance().WriteFreeStyleLog(true, err_level::err, _T("建物名称のコンテナ挿入に失敗しました。本オブジェクトを変換対象外とします。"), sindy::schema::sj::buildingname_point::kTableName, oid);
			*existWrongData = false;
			continue;
		}
		bldNamePtOIDs->insert(IDFeatureMap::value_type(oid, ipFeature));
	}
	return true;
}
