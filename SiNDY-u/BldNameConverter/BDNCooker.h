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

#pragma once
#include "LogSys.h"
#include "common.h"

/**
 * @brief 建物ビル名称データ変換クラス
 */
class BDNCooker
{
public:
	BDNCooker(const FieldIndexMap* bldNamePtIndeces);
	~BDNCooker(void);

	/**
	 * @brief  BDN変換データ作成
	 * @param  ipMeshGeometry [in] メッシュ矩形
	 * @param  bldFeatures [in] 建物情報
	 * @param  bldNamePtFeatures [in] 建物名称ポイント情報
	 * @param  bldNamePtIDs [in, out] 処理対象建物名称ポイントID群
	 * @param  cnvInfo [out] BDN変換情報
	 * @param  existWrongData [out] 不正なデータが存在するかどうか(true: 存在する, false: 存在しない)
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool cookBDN(
		const IGeometryPtr& ipMeshGeometry, 
		const IDFeatureMap& bldFeatures, 
		const IDFeatureMap& bldNamePtFeatures,
		IDFeatureMap* bldNamePtIDs, 
		IDRecordMap* cnvInfo,
		bool* existWrongData
		);

private:
	/**
	 * @brief  穴埋め形状作成関数
	 * @param  bldinfo [in] 建物情報
	 * @param  ipInfillBldGeometry [out] 穴埋め形状
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool makeInfillBldGeometry(const IDFeatureMap::value_type& bldInfo, IGeometryPtr& ipInfillBldGeometry);

	/**
	 * @brief  建物に関連する建物ビル名称取得
	 * @param  bldID [in] 建物のOBJECTID
	 * @param  ipInfillBldGeometry [in] 穴埋め建物形状
	 * @param  bldNamePtFeature [in] 建物ビル名称群	
	 * @param  meshMin [in] 処理対象メッシュ矩形最小座標
	 * @param  meshMax [in] 処理対象メッシュ矩形最大座標
	 * @param  bldNamePtIDs [in, out] 処理対象建物名称ポイントID群
	 * @param  cnvInfo [out] BDN変換情報
	 * @param  relExist [out] 関連する建物ビル名称が存在したかどうか(ture: 存在した false: 存在しなかった)
	 * @param  posChk [out] 紐づいた建物ビル名称が建物形状状に存在したかのチェック(ture: 存在した false: 存在しなかった)
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool getRelatedBldNamePt(
		long bldID,
		const IGeometryPtr& ipInfillBldGeometry,
		const IDFeatureMap& bldNamePtFeatures,
		const LONLAT& meshMin,
		const LONLAT& meshMax,
		IDFeatureMap* bldNamePtIDs, 
		IDRecordMap* cnvInfo,
		bool* relExist,
		bool* posChk
		);

	/**
	 * @brief  IRingPtr → IPolygonPtr変換関数(リングがExteriorRingであるこが条件)
	 * @param  bldID [in] 建物のobjectid
	 * @param  ipRing [in] 対象IRingPtr
	 * @param  ipSpRef [in] 空間参照
	 * @param  ipPolygon [out] ポリゴン形状
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool ring2Polygon(long bldID, const IRingPtr& ipRing, const ISpatialReferencePtr& ipSpRef, IPolygonPtr& ipPolygon);

	/**
	 * @biref  建物ビル名採用ポイントが変換対象かチェックする。
	 * @note   変換条件：ポイントがメッシュ矩形の中に含まれる。メッシュ境界線上に存在する場合は、小さいほうのメッシュで変換する。
	 * @param  tableName [in] テーブル名
	 * @param  id [in] 対象ポイントのOBJECTID
	 * @param  ipPtGeometry [in] ポイント形状
	 * @param  meshMin [in] 処理対象メッシュ矩形最小座標
	 * @param  meshMax [in] 処理対象メッシュ矩形最大座標
	 * @param  isCnv [out] 変換対象かどうか
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool chkPointCnv(LPCTSTR tableName, long id, const IGeometryPtr& ipPtGeometry, const LONLAT& meshMin, const LONLAT& meshMax, bool* isCnv);

private:
	const FieldIndexMap* m_bldNamePtIndeces;	//!< 建物ビル名称ポイント・フィールドインデックス対応表
};
