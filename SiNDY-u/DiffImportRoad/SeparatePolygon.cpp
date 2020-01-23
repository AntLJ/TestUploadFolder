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

#include "StdAfx.h"
#include "SeparatePolygon.h"

using namespace sindy::schema;
using namespace Reflect;

// コンストラクタ
CSeparatePolygon::CSeparatePolygon(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("マルチパートポリゴン解消");
}

// デストラクタ
CSeparatePolygon::~CSeparatePolygon(void)
{
}

// 初期化関数
bool CSeparatePolygon::init(void)
{
	bool aResult = false;
	for(EnvTable *aEnv = m_LayerManage->getFitstEnv(CLayerManage::FC_TABLE); ! aEnv->m_OrgTable.IsEmpty(); aEnv = m_LayerManage->getNextEnv(CLayerManage::FC_TABLE)){
		//1つでもポリゴンがあれば実行する
		IFeatureClassPtr ipClass = m_LayerManage->findTable(aEnv->m_OrgTable);
		if(ipClass){
			esriGeometryType aGeometryType;
			ipClass->get_ShapeType(&aGeometryType);
			if(aGeometryType == esriGeometryPolygon){
				//対象をリスト管理する（現状中縮背景のみだが）
				m_TgtList.push_back(aEnv->m_OrgTable);
				aResult = true;
			}
		}
	}
	return aResult;
}

// メッシュ単位でのマルチパートポリゴン解消関数
bool CSeparatePolygon::fnExecMesh(void)
{
	//メッシュポリゴン,枠を取得
	if(!fnGetMesh()){
		return false;
	}
	foreach(const CString& aLayer, m_TgtList){
		EnvTable *aEnv = m_LayerManage->findEnvTable(aLayer);
		//処理対象フィーチャクラス設定（検索等に使用）
		m_ipTable[0] = m_LayerManage->findTable(aLayer);
		fnSeparate(*aEnv);
	}
	return true;
}

// レイヤ毎のマルチパート解消関数
bool CSeparatePolygon::fnSeparate(EnvTable& cEnv)
{
	bool aResult = false;
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshPoly);
	if(!ipCursor){
		return aResult;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature){
		fnCheckAndSeparate(ipFeature, cEnv);
	}
	return true;;
}

// マルチパートであるかのチェックと分割関数
bool CSeparatePolygon::fnCheckAndSeparate(IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aCount;
	if(FAILED(ipGeoCol->get_GeometryCount(&aCount))){
		fnPrintComError();
		return false;
	}
	if(aCount > 1){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		msi_get_parts_info aPolUtil;
		if(aPolUtil.Init(ipGeom, aObjID, (string)CT2A(m_LayerManage->findTableName(cEnv.m_OrgTable)).m_psz, 0, stderr)){
			aCount = aPolUtil.Get_Part_Num();
			if(aCount > 1){
				IFeatureCursorPtr ipInsertCursor;
				if(FAILED(((IFeatureClassPtr)m_ipTable[0])->Insert(VARIANT_FALSE, &ipInsertCursor))){
					fnPrintComError();
					return false;
				}
				int iPart;
				int aReNew = 0;
				double aMaxArea = 0;
				for(iPart = 0; iPart < aCount; iPart++){
					IPolygonPtr ipSubPoly = aPolUtil.Get_Part_Polygon(iPart+1);
					IAreaPtr ipArea = ipSubPoly;
					if(ipArea != NULL){
						double aArea;
						ipArea->get_Area(&aArea);
						if(aArea > aMaxArea){
							aMaxArea = aArea;
							aReNew = iPart;
						}
					}
				}
				for(iPart = 0; iPart < aCount; iPart++){
					if(iPart == aReNew){
						continue;
					}
					IFeatureBufferPtr ipBuffer;
					if(FAILED(((IFeatureClassPtr)m_ipTable[0])->CreateFeatureBuffer(&ipBuffer))){
						fnPrintComError();
						return false;
					}
					IPolygonPtr ipSubPoly = aPolUtil.Get_Part_Polygon(iPart+1);
					if(FAILED(ipBuffer->putref_Shape(ipSubPoly))){
						fnPrintComError();
						return false;
					}
					fnCopyAttr(ipBuffer, ipFeature, cEnv.m_SDEIndexMap);
					fnSetCommonAttr((_IRowPtr)ipBuffer, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
					CComVariant vIDVariant;
					if(FAILED(ipInsertCursor->InsertFeature(ipBuffer, &vIDVariant))){
						fnPrintComError();
						return false;
					}
					fnPrintLogStyre(0, cEnv.m_OrgTable, vIDVariant.lVal, _T("マルチ解消追加"), NULL, TGT_LAYER);
				}
				if(FAILED(ipInsertCursor->Flush())){
					fnPrintComError();
					return false;
				}
				fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
				ipGeom = aPolUtil.Get_Part_Polygon(aReNew+1);
				ipFeature->putref_Shape(ipGeom);
				ipFeature->Store();
				fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("マルチ解消更新"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("ポリゴンの分析失敗"), NULL, TGT_LAYER);
			return false;
		}
	}
	return true;
}

// 継承する属性のコピー関数
bool CSeparatePolygon::fnCopyAttr(IFeatureBufferPtr& ipBuffer, const IFeaturePtr& ipFeature, INDEX_MAP& cSDEIndex)
{
	for(INDEX_MAP::const_iterator it = cSDEIndex.begin(); it != cSDEIndex.end(); ++it){
		//最終更新日付は反映対象外、最終更新者は後で指定値で統一するので除外、INF_IDは別途更新するので除外
		//作業目的、更新内容（？）も対象外（制作要望）
		//文字列比較は、コンテナに入れる際に大文字にしているのと、スキーマが大文字統一のはずなので「==」でいいはず
		if(it->first == ipc_table::kOperator || it->first == ipc_table::kModifyDate || it->first == link_queue::kInfID
			|| it->first == ipc_table::kPurpose || it->first == ipc_table::kUpdateType){
			continue;
		}
		CComVariant aVal;
		ipFeature->get_Value(cSDEIndex[it->first].m_Index, &aVal);
		ipBuffer->put_Value(it->second.m_Index, aVal);
	}
	return true;
}
