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
#include "..\GlobalFunc.h"
#include "RelationCheck.h"



// 抜けチェック
bool RelationCheck::checkOutArea( const IGeometryPtr& ipMeshGeometry, IGeometryCollectionPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference, const ErrorIdInfo&errorInfo, LayerCheckBase* layerCheck, const ErrorIdInfo& errorInfoNoPolygonInMesh, const CString& meshName, long meshId )const
{
	// 重複なエラーメッセージを出力しないための判断
	if( layerCheck->m_errorIdSet.find( errorInfo ) != layerCheck->m_errorIdSet.end() )	return true;

	// メッシュにADDR_POLYGON/CITY_POLYGONをマージする
	IPolygonPtr ipMergeGeometry = gf::constructUnion( ipGeometryCollection, ipSpatialReference );
	if( !ipMergeGeometry)
	{
		printProgress(getMsg( eFailConstructUnion ));
		return false;
	}
	if( gf::isTouches(ipMergeGeometry, ipMeshGeometry) && ( layerCheck->m_errorIdSet.find(errorInfoNoPolygonInMesh) == layerCheck->m_errorIdSet.end() ) )
	{
		layerCheck->m_errorLog->write( emCheck_ErrCode(errorInfoNoPolygonInMesh.errorCode), meshName, meshId, NULL, _T("%d"), errorInfoNoPolygonInMesh.oId );
		return true;
	}
	
	// Diffしたポリゴを取得する 
	IGeometryPtr ipDifferentGeometry = gf::difference( ipMeshGeometry, ipMergeGeometry );
	if( !ipDifferentGeometry )
	{
		printProgress( getMsg( eFailDifference ));
		return false;
	}

	// 抜けエリアが存在する場合、出力
	long geometryCount(0L);
	IGeometryCollectionPtr(ipDifferentGeometry)->get_GeometryCount(&geometryCount);
	for( long index = 0L; index < geometryCount;  ++index )
	{
		IGeometryPtr path;
		IGeometryCollectionPtr(ipDifferentGeometry)->get_Geometry( index, &path );
		if( !path ) continue;

		// チェック対象の都市地図メッシュにかかる住所ポリゴンを全てマージし、都市地図メッシュ図郭でClipした形状が都市地図メッシュ図郭と異なる場合エラー
		layerCheck->m_errorLog->write( emCheck_ErrCode(errorInfo.errorCode), layerCheck->getName(), INVALID_VALUE, path, _T("%d"), errorInfo.oId );
	}

	return true;
}