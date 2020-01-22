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

// AChiban.cpp: AChiban クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AChiban.h"

namespace{
	LPCTSTR ANNOF_FIELD = _T("ANNO_F");	///< 注記フラグフィールド名 
}
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//　コンストラクタ 
AChiban::AChiban()
:m_MeshCode(-1),
 m_AnnofIndex(-1)
{

}

//　デストラクタ
AChiban::~AChiban()
{

}

//　初期化
bool AChiban::init(IFeatureClassPtr ipChibanClass, bool cChgMode)
{
	m_ChgMode = cChgMode;
	m_ipRep = ipChibanClass;

	m_ipRep->FindField(CComBSTR(ANNOF_FIELD), &m_AnnofIndex);
	if(m_AnnofIndex < 0){
		_ftprintf(stderr, _T("#フィールドが見つかりません。,%s\n"), ANNOF_FIELD);
		return false;
	}

	//ログ用にフィーチャクラス名を取得
	CComBSTR aLayerName;
	((IDatasetPtr)m_ipRep)->get_Name(&aLayerName);
	m_LayerName = aLayerName;

	return true;
}

//　メッシュ境界上の地番代表点の注記フラグをオフにする
bool AChiban::execAdjust(IFeaturePtr ipMesh, LONG cMeshCode)
{
	m_MeshCode = cMeshCode;
	_tprintf(_T("Now %d\r"), m_MeshCode);

	//メッシュポリゴンからメッシュ枠を作成
	IGeometryPtr ipGeom;
	ipMesh->get_Shape(&ipGeom);
	ITopologicalOperatorPtr ipTopo(ipGeom);
	IGeometryPtr ipBoundary;
	ipTopo->get_Boundary(&ipBoundary);

	//チェック及び変更
	return fnCheck(ipBoundary);
}

//　指定ジオメトリ上の地番代表点で注記フラグが立っているものを検出する。変更モードの時はオフにする
bool AChiban::fnCheck(IGeometryPtr ipBoundary)
{
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR bstrName;
	m_ipRep->get_ShapeFieldName( &bstrName );
	ipFilter->put_GeometryField(bstrName);
	ipFilter->putref_Geometry(ipBoundary);
	ipFilter->put_SpatialRel(esriSpatialRelIntersects);

	bool aResult = false;
	IFeatureCursorPtr ipCursor;
	if(m_ChgMode){
		if(m_ipRep->Update(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
			_ftprintf(stderr, _T("#境界上のポイント検索失敗,%d\n"), m_MeshCode);
			return false;
		}
	}else{
		if(m_ipRep->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
			_ftprintf(stderr, _T("#境界上のポイント検索失敗,%d\n"), m_MeshCode);
			return false;
		}
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		CComVariant aAnnoF;
		ipFeature->get_Value(m_AnnofIndex, &aAnnoF);
		if(aAnnoF.lVal != 0){
			LONG aObjID;
			ipFeature->get_OID(&aObjID);
			if(m_ChgMode){
				//変更モードの時だけ更新
				ipFeature->put_Value(m_AnnofIndex, CComVariant(0));
				ipCursor->UpdateFeature(ipFeature);
				_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\tフラグ更新。\t\n"), m_LayerName, aObjID);
			}else{
				//チェックモード（変更モードではない）の時は、ログ出力のみ
				_ftprintf(stderr, _T("0\t%s\t%d\t\t\tInfo\t0\tフラグ更新可能。\t\n"), m_LayerName, aObjID);
			}
		}
	}
	return aResult;
}
