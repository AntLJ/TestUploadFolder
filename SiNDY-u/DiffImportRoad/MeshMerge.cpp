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
#include "MeshMerge.h"

using namespace sindy::schema;
using namespace Reflect;

namespace{
	double ACCEPTABLE_X  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（経度）
	double ACCEPTABLE_Y  = 0.000001;	///< メッシュ境界上の座標を比較する時の誤差許容に使用される値（緯度）
	long BG_EXCEPT_LIST[] = {101001, 101002, 101003, 102003, -1};	///< 背景のマージ除外種別（海、河川・沢・谷、湖・池・沼、堤外地）
}

// コンストラクタ
CMeshMerge::CMeshMerge(CLayerManage* cLayerManage)
{
	m_LayerManage = cLayerManage;
	m_ExecInfo = _T("不要構成点削除・メッシュ境界マージ");
}

// デストラクタ
CMeshMerge::~CMeshMerge(void)
{
}

// 初期化関数
bool CMeshMerge::init(void)
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
	if(aResult){
		//1つでもポリゴンがあれば実行する
		if(_tgetenv(_T("ACCEPTABLE")) == NULL){
			_tprintf(_T("マージ時誤差を許容しません\n"));
			_ftprintf(stderr, _T("#マージ時誤差を許容しません\n"));
			ACCEPTABLE_X = ACCEPTABLE_Y = 0.0;
		}else{
			_tprintf(_T("マージ時若干の誤差は許容します\n"));
			_ftprintf(stderr, _T("#マージ時若干の誤差は許容します\n"));
		}
		//背景用のマージ除外リストをsetに格納
		for(int i = 0; BG_EXCEPT_LIST[i] > 0; ++i){
			m_BGTarget.insert(BG_EXCEPT_LIST[i]);
		}
	}
	return aResult;
}

// メッシュ単位での境界上の不要構成点削除と境界マージ関数
bool CMeshMerge::fnExecMesh(void)
{
	//メッシュ境界（枠）を取得
	if(!fnGetMesh()){
		return false;
	}
	foreach(const CString& aLayer, m_TgtList){
		EnvTable *aEnv = m_LayerManage->findEnvTable(aLayer);
		//処理対象フィーチャクラス設定（検索等に使用）
		m_ipTable[0] = m_LayerManage->findTable(aLayer);

		//メッシュ境界の不要構成点削除
		fnDeleteDisusedPoint(*aEnv);

		//メッシュ境界マージ
		bool aLoop = true; 
		while(aLoop){
			// メッシュ境界上のshapeをマージする
			aLoop = fnShapeMerge(*aEnv);
		}
		m_IDList.clear();
	}
	return true;
}

// レイヤ単位のメッシュ境界上の不要構成点削除関数
bool CMeshMerge::fnDeleteDisusedPoint(EnvTable& cEnv)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshBoundary);
	if(ipCursor== NULL){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aResult &= fnCheckAndDelete(ipFeature, cEnv);
	}
	return aResult;
}

// フィーチャ単位のメッシュ境界上の不要構成点削除関数
bool CMeshMerge::fnCheckAndDelete(IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	long aObjID;	
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_ShapeCopy( &ipSrcShape );
	bool aResult = false;
	int aCount = 0;
	IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aCount);
	if(aCount > 0 && ipResult != NULL){
		if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
			fnSetCommonAttr((_IRowPtr)ipFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
			if(SUCCEEDED(ipFeature->Store())){
				fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("不要構成点削除"), NULL, TGT_LAYER);
				aResult = true;
			}else{
				fnPrintComError();
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("不要構成点削除更新失敗"), NULL, TGT_LAYER);
			}
		}else{
			fnPrintComError();
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("不要構成点削除 Shape設定失敗"), NULL, TGT_LAYER);
		}
	}else{
		aResult = true;
	}
	return aResult;
}

// 不要構成点を削除した後の形状と削除したポイント数を求める関数(必要に応じてポリゴンをリングに分解して再構成する)。
IGeometryPtr CMeshMerge::fnCroppingShape(const IGeometryPtr& ipSrcGeom, int& cCount)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	cCount = 0;
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cCount);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_LayerManage->get_SpRef());
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = fnCroppingRing(ipSubGeom, cCount);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// 不要構成点を削除した後の形状と削除したポイント数を求める関数（リング単位）。
IGeometryPtr CMeshMerge::fnCroppingRing(const IGeometryPtr& ipSrcGeom, int& cCount)
{
	//メッシュのコーナーの点は、一見不要だが、マージ前には必要になる場合が
	//あるため、残すように変更。
	//縦又は横に3メッシュ以上にわたるポリゴンの場合、メッシュコーナーを
	//除く処理が困難なため、各メッシュの範囲内のみを対象とするよう変更
	IGeometryPtr ipResultGeom = ipSrcGeom;
	bool aDel = false;
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	IPointPtr ipPoint;
	ipPointCollection->get_PointCount( &aPointCount );
	if(aPointCount > 4){
		for( int j = 0; j < aPointCount - 2; ++j) {
			double aBaseX, aBaseY;
			double aNextX1, aNextX2, aNextY1, aNextY2;
			ipPointCollection->get_Point( j, &ipPoint);
			ipPoint->QueryCoords(&aBaseX, &aBaseY);
			if(aBaseX == m_MinX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MinX && aNextX2 == m_MinX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--aPointCount;
						j--;
						continue;
					}
				}
			}else if(aBaseX == m_MaxX){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
					if(aNextY1 > m_MinY && aNextY1 < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--j;
						continue;
					}
				}
			}
			if(aBaseY == m_MinY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MinY && aNextY2 == m_MinY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						++cCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						--j;
					}
				}
			}else if(aBaseY == m_MaxY){
				ipPointCollection->get_Point( j+1, &ipPoint);
				ipPoint->QueryCoords(&aNextX1, &aNextY1);
				ipPointCollection->get_Point( j+2, &ipPoint);
				ipPoint->QueryCoords(&aNextX2, &aNextY2);
				if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
					if(aNextX1 > m_MinX && aNextX1 < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
						ipPointCollection->RemovePoints(j+1, 1);
						--aPointCount;
						_tprintf(_T("削除！(%d)\n"), j+1);
						++cCount;
						--j;
					}
				}
			}
		}
	}
	//始点,終点が削除対象となる時の処理
	if(aPointCount > 4){
		double aBaseX, aBaseY;
		double aNextX1, aNextX2, aNextY1, aNextY2;
		ipPointCollection->get_Point( 0, &ipPoint);
		ipPoint->QueryCoords(&aBaseX, &aBaseY);
		if(aBaseX == m_MinX){
			ipPointCollection->get_Point( 1, &ipPoint); //始点の次
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);	//終点の1つ前
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MinX && aNextX2 == m_MinX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseX == m_MaxX){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextX1 == m_MaxX && aNextX2 == m_MaxX){
				if(aBaseY > m_MinY && aBaseY < m_MaxY){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aBaseY == m_MinY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MinY && aNextY2 == m_MinY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}else if(aBaseY == m_MaxY){
			ipPointCollection->get_Point( 1, &ipPoint);
			ipPoint->QueryCoords(&aNextX1, &aNextY1);
			ipPointCollection->get_Point(aPointCount-2, &ipPoint);
			ipPoint->QueryCoords(&aNextX2, &aNextY2);
			if(aNextY1 == m_MaxY && aNextY2 == m_MaxY){
				if(aBaseX > m_MinX && aBaseX < m_MaxX){	//検査メッシュのコーナーを除くメッシュ範囲内なら
					ipPointCollection->RemovePoints(aPointCount-1, 1);
					_tprintf(_T("削除！(終点)\n"));
					++cCount;
					ipPointCollection->RemovePoints(0, 1);
					_tprintf(_T("削除！(始点)\n"));
					++cCount;
					aDel = true;
				}
			}
		}
		if(aDel){
			IRingPtr ipRing = ipResultGeom;
			if(ipRing != NULL){
				ipRing->Close();
			}else{
				IPolygonPtr ipPolygon = ipResultGeom;
				if(ipPolygon != NULL){
					ipPolygon->Close();
					ipPolygon->SimplifyPreserveFromTo();
				}else{
					ipResultGeom = NULL;
				}
			}
		}
	}
	return ipResultGeom;
}

// レイヤ単位でメッシュ境界のマージをする関数
bool CMeshMerge::fnShapeMerge(EnvTable& cEnv)
{
	bool aMerge = false;
	IFeatureCursorPtr	ipCursor;
	if((ipCursor = fnSelectByShape(m_ipMeshBoundary)) == NULL){
		return false;
	}

	IFeaturePtr ipFeature;
	while(!aMerge){
		if(ipCursor->NextFeature(&ipFeature) != S_OK){
			break;
		}
		long aObjID;
		ipFeature->get_OID(&aObjID);
		if(m_IDList.find(aObjID) == m_IDList.end()){
			//リストになければ実行
			if(fnIsTargetKind(ipFeature, cEnv) && fnSerchTargetsAndMerge(ipFeature, cEnv)){
				aMerge = true;
				break;
			}else{
				//マージ対象外の属性か、これ以上マージするものがなければリストに追加
				m_IDList.insert(aObjID);
			}
		}
	}
	return aMerge;
}

// フィーチャ毎にマージ可能なものをマージする関数
bool CMeshMerge::fnSerchTargetsAndMerge(IFeaturePtr& ipSrcFeature, EnvTable& cEnv)
{
	IGeometryPtr ipGeom;
	ipSrcFeature->get_Shape( &ipGeom );

	long aSrcObjID;
	ipSrcFeature->get_OID(&aSrcObjID);
	IFeatureCursorPtr ipFeatureCursor = fnSelectByShape(ipGeom);
	if(ipFeatureCursor == NULL){
		return false;
	}
	long aObjID;
	IFeaturePtr ipTgtFeature;
	int aResult;
	bool aRet = false;
	while(ipFeatureCursor->NextFeature(&ipTgtFeature) == S_OK){
		ipTgtFeature->get_OID(&aObjID);
		if(aSrcObjID == aObjID){
			continue;
		}
		if((aResult = fnIsNeedMerge(ipSrcFeature, ipTgtFeature, cEnv)) == 1){
			if(fnCompAttr(ipSrcFeature, ipTgtFeature, cEnv)){
				bool aMergeResult;
				//IDの小さい方に吸収する。
				if(aSrcObjID < aObjID){
					aMergeResult= fnMerge(ipSrcFeature, ipTgtFeature, cEnv);
				}else{
					aMergeResult= fnMerge(ipTgtFeature, ipSrcFeature, cEnv);
				}
				if(aMergeResult){
					CString aMsg;
					aMsg.Format(_T("%d"), aSrcObjID);
					fnPrintLogStyre(0, cEnv.m_OrgTable, aObjID, _T("マージしました"), aMsg, TGT_LAYER);
					aRet = true;
					break;
				}
			}else{
				CString aMsg;
				aMsg.Format(_T("%d"), aSrcObjID);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("隣接する属性が違う"), aMsg, TGT_LAYER);
			}
		}else if(aResult == 0){
//			printf("Not Merge:%d,%d\n", cObjID, aObjID);
		}else{
			CString aMsg;
			aMsg.Format(_T("%d"), aSrcObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("マージできませんでした"), aMsg, TGT_LAYER);
			aRet = false;
			break;
		}
	}
	return aRet;
}

// 形状的にマージ対象になるかを判定する関数（メッシュ境界上で線分を共有していること）
int CMeshMerge::fnIsNeedMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
	//ポリゴンの時しか呼ばれない
	IGeometryPtr ipSrcGeom, ipTgtGeom;

	if(FAILED(ipSrcFeature->get_Shape( &ipSrcGeom ))){
		long aObjID;
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("SrcShapeの取得失敗"), NULL, TGT_LAYER);
		return -1;
	}
	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom ))){
		long aObjID;
		ipTgtFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("TgtShapeの取得失敗"), NULL, TGT_LAYER);
		return -1;
	}

	IPointCollectionPtr ipPointCollectionS(ipSrcGeom);
	IPointCollectionPtr ipPointCollectionT(ipTgtGeom);
	long aPointCountS = 0;
	long aPointCountT = 0;
	ipPointCollectionS->get_PointCount( &aPointCountS );
	ipPointCollectionT->get_PointCount( &aPointCountT );

	if(aPointCountS > 3 && aPointCountT > 3){
		std::vector<WKSPoint> aSrcPoints( aPointCountS );
		ipPointCollectionS->QueryWKSPoints( 0, aPointCountS, &aSrcPoints.at(0) );
		std::vector<WKSPoint> aTgtPoints( aPointCountT );
		ipPointCollectionT->QueryWKSPoints( 0, aPointCountT, &aTgtPoints.at(0) );

		for(int i = 0; i < aPointCountS -1; i++){
			if(aSrcPoints[i].X == m_MinX && aSrcPoints[i+1].X == m_MinX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MinX  && aTgtPoints[j+1].X == m_MinX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].X == m_MaxX && aSrcPoints[i+1].X == m_MaxX){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].X == m_MaxX  && aTgtPoints[j+1].X == m_MaxX){
						if(fnCompXY(aSrcPoints[i].Y, aTgtPoints[j+1].Y, ACCEPTABLE_Y) && fnCompXY(aSrcPoints[i+1].Y, aTgtPoints[j].Y, ACCEPTABLE_Y)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MinY && aSrcPoints[i+1].Y == m_MinY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MinY  && aTgtPoints[j+1].Y == m_MinY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}else if(aSrcPoints[i].Y == m_MaxY && aSrcPoints[i+1].Y == m_MaxY){
				for(int j = 0; j < aPointCountT - 1; j++){
					if(aTgtPoints[j].Y == m_MaxY  && aTgtPoints[j+1].Y == m_MaxY){
						if(fnCompXY(aSrcPoints[i].X, aTgtPoints[j+1].X, ACCEPTABLE_X) && fnCompXY(aSrcPoints[i+1].X, aTgtPoints[j].X, ACCEPTABLE_X)){
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

//　属性的にマージ対象になるかを判定する関数（決められた属性が一致すること）
// (中縮背景固定になってしまったので、対象が増えたら調整が必要)
bool CMeshMerge::fnCompAttr(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
#if 0
	//汎用的にするには、こんな感じの方がいいが・・（都市地図用なので、このままではエラー）
	for(int i = 0; gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Name != NULL; i++){
		if(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_Compare){
			CComVariant aSrcVal, aTgtVal;
			ipSrcFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SinIndex, &aSrcVal);
			ipTgtFeature->get_Value(gEnvTable[g_DataIndex].m_Fields[cIndex][i].m_SinIndex, &aTgtVal);
			if(aSrcVal != aTgtVal){
				return false;
			}
		}
	}
#else
	CComVariant aSrcVal, aTgtVal;
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}
	ipSrcFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index, &aSrcVal);
	ipTgtFeature->get_Value(cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index, &aTgtVal);
	if(aSrcVal != aTgtVal){
		return false;
	}

#endif
	return true;
}

// 片方をマージした形状に更新し、片方を削除して、マージ後の状態を作る関数。
bool CMeshMerge::fnMerge(IFeaturePtr& ipSrcFeature, IFeaturePtr& ipTgtFeature, EnvTable& cEnv)
{
	long aObjID;
	IGeometryPtr ipSrcGeom, ipTgtGeom;
	bool aResult = false;
	if(FAILED(ipSrcFeature->get_ShapeCopy(&ipSrcGeom))){
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("SrcShapeの取得失敗"), NULL, TGT_LAYER);
		return false;
	}

	if(FAILED(ipTgtFeature->get_Shape( &ipTgtGeom ))){
		ipTgtFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("TgtShapeの取得失敗"), NULL, TGT_LAYER);
		return false;
	}
	ITopologicalOperatorPtr  ipSrcTopo = ipSrcGeom;
	IGeometryPtr ipTgtGeom2;
	if(ipSrcTopo->Union(ipTgtGeom, &ipTgtGeom2) ==  S_OK){
		if(ipTgtGeom2 != NULL){
			ipTgtGeom2->putref_SpatialReference(m_LayerManage->get_SpRef());
			((ITopologicalOperator2Ptr)ipTgtGeom2)->put_IsKnownSimple(VARIANT_FALSE);
			((ITopologicalOperatorPtr)ipTgtGeom2)->Simplify();
			if(SUCCEEDED(ipSrcFeature->putref_Shape(ipTgtGeom2))){
				fnSetCommonAttr((_IRowPtr)ipSrcFeature, cEnv.m_SDEIndexMap, cEnv.m_PGDBIndexMap, NULL);
				if(SUCCEEDED(ipSrcFeature->Store())){
					if(SUCCEEDED(ipTgtFeature->Delete())){
						aResult = true;
					}else{
						ipTgtFeature->get_OID(&aObjID);
						fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("フィーチャー削除失敗"), NULL, TGT_LAYER);
					}
				}else{
					ipSrcFeature->get_OID(&aObjID);
					fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("フィーチャー更新失敗"), NULL, TGT_LAYER);
					fnPrintComError();
				}
			}else{
				ipSrcFeature->get_OID(&aObjID);
				fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("マージ後Shape設定失敗"), NULL, TGT_LAYER);
			}
		}else{
			ipSrcFeature->get_OID(&aObjID);
			fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("マージ失敗"), NULL, TGT_LAYER);
		}
	}else{
		ipSrcFeature->get_OID(&aObjID);
		fnPrintLogStyre(2, cEnv.m_OrgTable, aObjID, _T("Union失敗"), NULL, TGT_LAYER);
	}
	return aResult;
}

// 許容範囲を持たせた座標（値）の一致確認関数
bool CMeshMerge::fnCompXY(double cPoint1, double cPoint2, double cAcceptable)
{
	if(fabs(cPoint1 - cPoint2) <= cAcceptable){
		return true;
	}else{
		return false;
	}
}

// マージ対象属性かを判定する関数(中縮背景固定になってしまったので、対象が増えたら調整が必要)
bool CMeshMerge::fnIsTargetKind(const IFeaturePtr& ipFeature, EnvTable& cEnv)
{
	if(!fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC1BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC2BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC3BgClass].m_Index)
		|| !fnIsTargetBaseKind(ipFeature, cEnv.m_SDEIndexMap[base_site::kSC4BgClass].m_Index)){
			return false;
	}
	return true;
}

// 指定フィーチャの指定属性が除外マージ属性かを判定する関数（中縮背景限定）
bool CMeshMerge::fnIsTargetBaseKind(const IFeaturePtr& ipFeature, long cIndex)
{
	CComVariant aVal;
	ipFeature->get_Value(cIndex, &aVal);
	if(m_BGTarget.find(aVal.lVal) != m_BGTarget.end()){
		//マージ除外リストにあれば
		return false;
	}
	return true;
}
