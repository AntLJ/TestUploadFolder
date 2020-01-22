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
class LayerCheckBase;

/**
 * @class	RelationCheck
 * @brief	関係チェッククラス
 */
class RelationCheck
{
public:
	/**
	 * @brief  コンストラクタ
	 */
	RelationCheck(void){}

	 /**
	 * @brief  デストラクタ
	 */
	virtual ~RelationCheck(void){}

	/**
	* @brief	抜けチェック
	* @param	[in]ipMeshGeometry	　　メッシュジオメトリ
	* @param	[in]ipGeometryCollection citypolygonジオメトリコレクション
	* @param	[in]ipSpatialReference　空間参照
	* @param	[in]errorInfo           現れたエラー情報
	* @param	[in]layerCheck          レイヤチエック
	* @param	[in]errorInfoNoPolygonInMesh エラー情報(メッシュ内に行政界ポリゴンがない)
	* @param	[in]meshName            メッシュ名
	* @param	[in]meshId              メッシュオブジェクトID
	* @retval	true:成功
	* @retval	false:失敗
	*/
	bool checkOutArea( const IGeometryPtr& ipMeshGeometry, IGeometryCollectionPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference, const ErrorIdInfo&errorInfo, LayerCheckBase* layerCheck, const ErrorIdInfo& errorInfoNoPolygonInMesh, const CString& meshName, long meshId )const;
};

