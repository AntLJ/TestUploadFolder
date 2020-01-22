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

namespace util
{
	/**
	* @brief  ポリラインをセグメント単位で分割したポリライン群で返す
	* @param  [in]  polyline 分割するポリライン
	* @return 分割後のポリライン群
	*/
	std::vector<IPolylinePtr> splitPolyline(IPolylinePtr polyline);

	/**
	* @brief  2つのポリライン間の最近傍点を取得する
	* @param[in]   polygon    最近傍を求める対象のポリゴン
	* @param[in]   polyline   最近傍を求める対象のポリライン
	* @param[out]  nearest_point1  ポリゴン側の最近傍点
	* @param[out]  nearest_point2  ポリライン側の最近傍点
	* @retval true   取得成功
	* @retval false  取得失敗
	*/
	bool getNearestPoint(IPolygonPtr polygon, IPolylinePtr polyline, IPointPtr& nearest_point1, IPointPtr& nearest_point2);

	/**
	 * @brief  2つのポリライン間の最近傍点を取得する
	 * @param[in]   polyline1   最近傍を求める対象のポリライン
	 * @param[in]   polyline2   最近傍を求める対象のポリライン
	 * @param[out]  nearest_point1	polyline1側の最近傍点
	 * @param[out]  nearest_point2  polyline2側の最近傍点
	 * @retval true   取得成功
	 * @retval false  取得失敗
	*/
	bool getNearestPointBetweenPolyline(IPolylinePtr polyline1, IPolylinePtr polyline2, IPointPtr& nearest_point1, IPointPtr& nearest_point2);

	/**
	 * @brief  任意のポイントを基準に、他のポリライン上の一番近いポイント、及びポイントまでの距離を取得する
	 * @param[in]  ipBasePoint       基準側のポイント
	 * @param[in]  ipTargetPolyline  一番近いポイントを検索される側のポリライン
	 * @param[out] ipNearestPoint    一番近いポイント
	 * @param[out] distance          一番近いポイントまでの距離
	 * @retval true   取得成功
	 * @retval false  取得失敗
	 */
	bool getNearestPointAndDistance(IPointPtr ipBasePoint, IPolylinePtr ipTargetPolyline, IPointPtr& ipNearestPoint, double& distance);
}

