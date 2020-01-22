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
 * @brief 変換元データ読み込みクラス
 */
class SourceReader
{
public:
	SourceReader(
		const IFeatureClassPtr& ipBldNamePt,
		const IFeatureClassPtr& ipBld,
		const FieldIndexMap* bldNamePtIndeces,
		const FieldIndexMap* bldIndeces,
		const std::set<long>* bldExcList
		);
	~SourceReader(void);

	/**
	 * @brief  変換元データ読み込み関数
	 * @param  ipMeshGeometry [in] 対象メッシュ形状
	 * @param  bldFeatures [out] 建物情報
	 * @param  bldNamePtFeatures [out] 建物名称情報
	 * @param  bldNamePtOIDs [out] 
	 * @param  existWrongData [out] 不正なデータが存在したかどうか
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool readSource(const IGeometryPtr& ipMeshGeometry, IDFeatureMap* bldFeatures, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData);

private:
	/**
	 * @brief  処理対象建物取得&取得した建物を全て含む矩形取得
	 * @param  ipMeshGeometry [in] メッシュ矩形
	 * @param  bldFeatures [out] 処理対象建物
	 * @param  ipExtentGemetry　[out] 建物を全て含む矩形
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool getTgtBldAndExtentGeometry(const IGeometryPtr ipMeshGeometry, IDFeatureMap* bldFeatures, IGeometryPtr& ipExtentGemetry);

	/**
     * @brief  処理対象建物名称取得
	 * @note   建物名称が存在するメッシュでしか変換はされないが、建物に紐づく建物名称が存在するかのチェックが必要であるため、エクステントした矩形で取得
	 * @param  ipExtentGeometr [in] メッシュをエクステントした矩形
	 * @param  bldNamePtFeatures [out] 処理対象建物名称
	 * @param  bldNamePtIDs [out] 処理対象建物名称OBJECTIDリスト
	 * @retval true 成功
	 * @retval false 失敗
	 */
	bool getTgtBldNamePt(const IGeometryPtr ipExtentGeometry, IDFeatureMap* bldNamePtFeatures, IDFeatureMap* bldNamePtOIDs, bool* existWrongData);

private:
	const IFeatureClassPtr	m_ipBldNamePt;		//!< 建物ビル名称ポイント・フィーチャクラス
	const IFeatureClassPtr	m_ipBld;			//!< 建物ポリゴン・フィーチャクラス

	const FieldIndexMap*	m_bldNamePtIndeces;	//!< 建物ビル名称ポイント・フィールドインデックス対応表
	const FieldIndexMap*	m_bldIndeces;		//!< 建物ポリゴン・フィールドインデックス対応表

	const std::set<long>*	m_bldExcList;		//!< 処理対象外建物種別リスト
};
