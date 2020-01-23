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
#include <SiNDYLib/FieldMap.h>

namespace Util
{
	/**
	 * @brief   ジオメトリから指定距離だけバッファリンクした形状を作成する
	 * @param[in] geo    バッファリング元のジオメトリ
	 * @param[in] meter  バッファリング距離(メートル)
	 * @return  バッファリンクした形状
	 */
	IGeometryPtr getBuffer( const IGeometryPtr& geo, double meter );

	/**
	 * @brief   比較元と比較先の最短距離を取得する
	 * @param[in]	ipBase			比較元
	 * @param[in]	ipComp			比較先
	 * @param[out]	distance		最短距離
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool getNearestDistance( const IFeaturePtr& ipBase, const IFeaturePtr& ipComp, double& nearestDistance );

	/**
	 * @brief	リンクの中点のXYを取得する
	 * @param[in]	ipFeature	リンク
	 * @param[out]	x			経度
	 * @param[out]	y			緯度
	 * @param[in]	ipSpRef		空間参照
	 */
	void getXY( const IFeaturePtr& ipFeature, double& x, double& y, const ISpatialReferencePtr& ipSpRef = nullptr );

	/**
	 * @brief	GIDを持つフィーチャー群をGIDと紐付けて返す
	 * @param[in]	features	フィーチャー群
	 * @param[in]	fieldMap	フィールドマップ
	 * @param[out]	ret			GIDと紐付いたフィーチャー群
	 */
	void getGid2Feature( const std::vector<IFeaturePtr>& features, const sindy::CFieldMap& fieldMap, std::map<CString, IFeaturePtr>& ret );
}