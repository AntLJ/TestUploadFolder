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
#include "LotClipProcessor.h"
#include <TDC/sindylib_core/meshutil.h>
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_base/RowContainer.h>
#include <ArcHelperEx/GlobalFunctions.h>
#include "ClipUtility.h"

using namespace sindy;

template<>
LotClipProcessor<uh::tstring>::LotClipProcessor(const CWorkspace& workspace, const ClippingDefinitionManager& m_ClipDef, const std::vector<uh::tstring>& meshes)
	: MeshClipProcessor<uh::tstring>( workspace, m_ClipDef ), m_vecMesh(meshes)
{
}

// ロットポリゴンを初期化する
template<>
void LotClipProcessor<uh::tstring>::initLotPolygon( ISpatialReferencePtr spref )
{
	IGeometryCollectionPtr bag( CLSID_GeometryBag );
	((IGeometryPtr)bag)->putref_SpatialReference(spref);
	for( const auto& mesh : m_vecMesh )
		bag->AddGeometry( MESH2POLY( _ttol(mesh.c_str()), spref ) );

	ITopologicalOperatorPtr topo( CLSID_Polygon );
	((IGeometryPtr)topo)->putref_SpatialReference(spref);
	topo->ConstructUnion((IEnumGeometryPtr)bag);
	m_ipLotPolygon = topo;
}

/// クリップ処理を実施する
template<>
bool LotClipProcessor<uh::tstring>::doClip( CTableContainer& con, IEnvelopePtr mesh )
{
	if( !m_ipLotPolygon )
		initLotPolygon( AheGetSpatialReference((ITable*)con) );

	ISpatialReferencePtr spref;
	m_ipLotPolygon->get_SpatialReference(&spref);

	// ロット境界とメッシュの共有する辺を取得
	IGeometryPtr lotBoundary;
	ITopologicalOperatorPtr(m_ipLotPolygon)->get_Boundary( &lotBoundary );
	IGeometryPtr sharedLine;
	// TODO:接触判定がうまくいかないこともあり得るので、メッシュ矩形のバッファで判定するよう修正
	ITopologicalOperatorPtr(lotBoundary)->Intersect(mesh, esriGeometry1Dimension, &sharedLine);
	// ジオメトリが空の場合は飛ばす
	if( IsEmpty(sharedLine) )
		return true;

	// TODO:検索漏れもあり得るので、ポイントやラインで検索せず、
	//      バッファリングしたポリゴンで検索するよう修正
	ISpatialFilterPtr pMeshFilter = AheInitSpatialFilter( nullptr, sharedLine );
	// 共有辺に接するフィーチャを全て取得し、キャッシュする
	con._Select( (IQueryFilterPtr)pMeshFilter, false );
	con.CreateCache();

	// クリップ処理失敗を未然に防ぐため、メッシュ境界付近の構成点を
	// メッシュ境界にスナップさせる
	// TODO: これをやらないとメッシュ境界付近の構成点に引っ張られる
	// ことがあるが、問題が見えてから処理を追加することにする。
	// ちなみに、スナップする関数自体は実装済み

	// クリップ処理
	for( auto& row : con )
		Clip( con, *CAST_ROWC(row), m_ipLotPolygon );

	return true;
}
