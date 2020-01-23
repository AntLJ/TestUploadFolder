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

namespace gf
{
	/**
	 * @brief	ドメイン名取得
	 * @param	ipRow		[in]	IRow
	 * @param	lFID		[in]	フィールドＩＤ
	 * @param	lCode		[in]	コード値
	 * @param	strDomain	[out]	ドメイン
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool getDomain( const _IRowPtr& ipRow, const LONG& lFID, const LONG& lCode, CString& strDomain );

	/**
	 * @brief	緯度経度取得
	 * @param	ipGeo		[in]	ジオメトリ
	 * @param	point		[out]	緯度経度格納用
	 * @param	bLabelPoint	[in]	ポリゴンに対する緯度経度を、重心ではなくラベルポイントで取得するかどうか
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool getLonLat( const IGeometryPtr& ipGeo, WKSPoint& point, bool bLabelPoint = false );

	/**
	 * @brief	構成点数取得
	 * @param	ipGeo	[in]	ジオメトリ
	 * @param	lCount	[out]	構成点数
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool getPointCount( const IGeometryPtr& ipGeo, LONG& lCount );

	/**
	 * @brief	線形オブジェクト？
	 * @param	ipGeo	[in]	ジオメトリ
	 ` @param	bResult	[out]	bool
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool isLinearObject( const IGeometryPtr& ipGeo, bool& bResult );

	/**
	 * @brief	長さ取得
	 * @param	ipGeo	[in]	ジオメトリ
	 * @param	dLen	[out]	長さ(10進度単位)
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool getLength( const IGeometryPtr& ipGeo, double& dLen );

	/**
	 * @brief	面積取得
	 * @param	ipGeo	[in]	ジオメトリ
	 * @param	dArea	[out]	面積(10進度単位)
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool getArea( const IGeometryPtr& ipGeo, double& dArea );
}
