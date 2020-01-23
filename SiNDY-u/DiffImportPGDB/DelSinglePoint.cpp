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
#include "DelSinglePoint.h"

using namespace Reflect;

// コンストラクタ
CDelSinglePoint::CDelSinglePoint(void)
{
	m_ExecInfo = _T("不要構成点その２削除");
}

// デストラクタ
CDelSinglePoint::~CDelSinglePoint(void)
{
}

// 初期化関数
bool CDelSinglePoint::init(const CArguments& args, const IWorkspacePtr& ipWorkspace,
						   const ISpatialReferencePtr& ipSpRef, const FCArray& ipFeatureClasses, bool isSindyCImport )
{
	m_testMode = args.m_testMode;
	m_logFile = args.m_logFile;
	m_ipWorkspaceEdit = ipWorkspace;
	m_ipSpRef = ipSpRef;
	m_ipFeatureClasses = ipFeatureClasses;
	m_sindyCImport = isSindyCImport;
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		if(g_ShapeType[i] == esriGeometryPolygon){
			//1つでもポリゴンがあれば実行する
			return true;
		}
	}
	return false;
}

// メッシュ単位での不要構成点削除（その２）関数
bool CDelSinglePoint::fnExecMesh(COMMAND cCommand)
{
	//メッシュ境界（枠）を取得
	if(!fnGetMesh(true)){
		return false;
	}
	bool aResult = true; 
	for(int i = 0; gEnvTable[g_DataIndex].m_OrgTableNames[i] != NULL; ++i){
		if(g_ShapeType[i] == esriGeometryPolygon){
			//処理対象フィーチャクラス設定（検索等に使用）
			m_ipFeatureClass = m_ipFeatureClasses[i];
			aResult &= fnDeleteDisusedPoint(i);
		}
	}
	return aResult;
}

// レイヤ単位のメッシュ境界上の不要構成点削除関数
bool CDelSinglePoint::fnDeleteDisusedPoint(long cIndex)
{
	IFeatureCursorPtr ipCursor = fnSelectByShape(m_ipMeshGeom, esriSpatialRelOverlaps);
	if(ipCursor== NULL){
		return false;
	}
	bool aResult = true;
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK && ipFeature){
		aResult &= fnCheckAndDelete(ipFeature, cIndex);
	}
	return aResult;
}

// フィーチャ単位のメッシュ境界上の不要構成点削除関数
bool CDelSinglePoint::fnCheckAndDelete(IFeaturePtr ipFeature, long cIndex)
{
	long aObjID;
	ipFeature->get_OID(&aObjID);
	IGeometryPtr ipSrcShape;
	ipFeature->get_Shape( &ipSrcShape );
	POINT_SET aCheckPoints;

	//削除候補点とその前後の点の情報を取得
	bool aResult = fnGetCheckPoints(ipSrcShape, aCheckPoints);
	if(aCheckPoints.size() > 0){
		//削除候補があったら、接するポリゴンを検索
		IFeatureCursorPtr ipCursor = fnSelectByShape(ipSrcShape);
		if(ipCursor== NULL){
			return false;
		}
		IFeaturePtr ipTgtFeature;
		POINT_SET aDelPoints;
		while(ipCursor->NextFeature(&ipTgtFeature) == S_OK && ipTgtFeature){
			long aTgtObjID;	
			ipTgtFeature->get_OID(&aTgtObjID);
			if(aObjID == aTgtObjID){
				//自分も見つかるはず
				continue;
			}
			//候補の前後からなるラインを持っているかチェック
			fnFindPairLine(ipTgtFeature, aCheckPoints, aDelPoints);
		}
		if(aDelPoints.size() > 0){
			if(!m_testMode){
				//ポイント削除
				IGeometryPtr ipResult = fnCroppingShape(ipSrcShape, aDelPoints, aObjID, cIndex);
				if(ipResult != NULL){
					if(SUCCEEDED(ipFeature->putref_Shape(ipSrcShape))){
						fnSetCommonAttr(ipFeature, cIndex, NULL);
						if(SUCCEEDED(ipFeature->Store())){
							POINT_SET::const_iterator aCmp;
							for(aCmp = aDelPoints.begin(); aCmp != aDelPoints.end(); aCmp++){
								fnPrintLogStyle(info, cIndex, aObjID, _T("不要構成点削除。"));
							}
							aResult = true;
						}else{
							fnPrintLogStyle(error, cIndex, aObjID, _T("更新失敗。"));
						}
					}else{
						fnPrintLogStyle(error, cIndex, aObjID, _T("Shape設定失敗！。"));
					}
				}else{
					aResult = true;
				}
			}else{
				POINT_SET::const_iterator aCmp;
				for(aCmp = aDelPoints.begin(); aCmp != aDelPoints.end(); aCmp++){
					fnPrintLogStyle(info, cIndex, aObjID, _T("不要構成点削除可能。"));
				}
				aResult = true;
			}
		}
	}
	return aResult;
}

// ポリゴンの削除候補点とその前後の点の情報を取得する関数
bool CDelSinglePoint::fnGetCheckPoints(IGeometryPtr ipGeom, POINT_SET& cCheckPoints)
{
	IGeometryCollectionPtr ipGeoCol = ipGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	if(aPart == 1){
		fnCheckRing(ipGeom, cCheckPoints);
	}else{
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			fnCheckRing(ipSubGeom, cCheckPoints);
		}
	}
	return true;
}

// 削除候補ポイントの前後からなるラインを持っているかチェックして、削除対象ポイントを求める関数
bool CDelSinglePoint::fnFindPairLine(IFeaturePtr ipFeature, const POINT_SET& cCheckPoints, POINT_SET& cDeletePoints)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
	IPointCollectionPtr ipPointCollection = ipGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount(&aPointCount);
	if(aPointCount > 4){
		std::vector<WKSPoint> aPoint( aPointCount );
		ipPointCollection->QueryWKSPoints( 0, aPointCount, &aPoint.at(0) );
		for(int i = 0; i < aPointCount - 1; i++){
			POINT_SET::const_iterator aCmp;
			for(aCmp = cCheckPoints.begin(); aCmp != cCheckPoints.end(); aCmp++){
				if((fnIsEqual(aCmp->m_Left , aPoint[i]) && fnIsEqual(aCmp->m_Right , aPoint[i+1]))
				|| (fnIsEqual(aCmp->m_Left , aPoint[i+1]) && fnIsEqual(aCmp->m_Right , aPoint[i]))){
					cDeletePoints.insert(*aCmp);
					break;
				}
#if 0 //必ずしも良い結果にならないので一時保留
				if(i < aPointCount - 2 && !fnIsEqual(aCmp->m_Center , aPoint[i+1]) && (aPoint[i+1].X == m_MinX || aPoint[i+1].X == m_MaxX || aPoint[i+1].Y == m_MinY || aPoint[i+1].Y == m_MaxY)){
					if((fnIsEqual(aCmp->m_Left , aPoint[i]) && fnIsEqual(aCmp->m_Right , aPoint[i+2]))
					|| (fnIsEqual(aCmp->m_Left , aPoint[i+2]) && fnIsEqual(aCmp->m_Right , aPoint[i]))){
						cDeletePoints.insert(*aCmp);
						break;
					}					
				}
#endif
			}
		}
	}
	return false;
}

// 不要構成点を削除した後の形状を求める関数(必要に応じてポリゴンをリングに分解して再構成する)
IGeometryPtr CDelSinglePoint::fnCroppingShape(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex)
{
	IGeometryPtr ipResultGeom;
	IGeometryCollectionPtr ipGeoCol = ipSrcGeom;
	long aPart;
	ipGeoCol->get_GeometryCount(&aPart);
	
	if(aPart == 1){
		ipResultGeom = fnCroppingRing(ipSrcGeom, cDeletePoints, cObjID, cIndex);
	}else{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference(m_ipSpRef);
		IGeometryCollectionPtr ipResultGeoCol = ipPolygon;
		for(int i = 0; i < aPart; i++){
			IGeometryPtr ipSubGeom, ipResultSub;
			ipGeoCol->get_Geometry(i, &ipSubGeom);
			ipResultSub = fnCroppingRing(ipSubGeom, cDeletePoints, cObjID, cIndex);
			ipResultGeoCol->AddGeometry(ipResultSub);
		}
		ipPolygon->SimplifyPreserveFromTo();
		ipResultGeom = ipPolygon;
	}
	return ipResultGeom;
}

// （ポリゴンを分解した）リングの削除候補点とその前後の点の情報を取得する関数
bool CDelSinglePoint::fnCheckRing(IGeometryPtr ipGeom,  POINT_SET& cCheckPoints)
{
	IPointCollectionPtr ipPointCollection = ipGeom;
	long aPointCount = 0;
	ipPointCollection->get_PointCount(&aPointCount);
	if(aPointCount > 4){
		//4点以下（実質既に4点の時しかない）はそれ以上削除できないから見ない
		std::vector<WKSPoint> aPoint( aPointCount );
		ipPointCollection->QueryWKSPoints( 0, aPointCount, &aPoint.at(0) );
		for(int i = 0; i < aPointCount - 1; i++){
			if(aPoint[i].X == m_MinX || aPoint[i].X == m_MaxX
					|| aPoint[i].Y == m_MinY || aPoint[i].Y == m_MaxY){
				int aSt;
				if(i == 0){
					aSt = aPointCount - 2;
				}else{
					aSt = i-1;
				}
				//メッシュ境界上の点に対し、その前後の点が同じ境界上ではなく、かつ別のメッシュに含まれているものを候補として選択
				//元々一直線だったものが、分割・マージによって構成点ができてしまった部分を拾いたい
				//隣り合うオブジェクトも同じように分割・マージされて新たに点ができたなら、整合はとれているので、消さないので、
				//この時点では、「削除候補」である
				//東西にまたがる場合の3点のY座標が同じ場合、南北にまたがる場合の3点のX座標が同じ場合に漏れるバグがある
				if(aPoint[i].X != aPoint[aSt].X && aPoint[i].X != aPoint[i+1].X
					&& aPoint[i].Y != aPoint[aSt].Y && aPoint[i].Y != aPoint[i+1].Y
					&& fnCalcMeshCode(aPoint[aSt]) != fnCalcMeshCode(aPoint[i+1])){
					CheckPoint aChk;
					aChk.m_Center = aPoint[i];
					aChk.m_Left = aPoint[aSt];
					aChk.m_Right = aPoint[i+1];
					cCheckPoints.insert(aChk);
				}
			}
		}
	}
	return false;
}

// 2点のWKSPoint一致確認関数
bool CDelSinglePoint::fnIsEqual(const WKSPoint& aPoint1, const WKSPoint& aPoint2)
{
	if(aPoint1.X == aPoint2.X && aPoint1.Y == aPoint2.Y){
		return true;
	}else{
		return false;
	}
}

// 不要構成点を削除した後の形状を求める関数(リング単位)
IGeometryPtr CDelSinglePoint::fnCroppingRing(IGeometryPtr ipSrcGeom, const POINT_SET& cDeletePoints, long cObjID, long cIndex)
{
	IGeometryPtr ipResultGeom = ipSrcGeom;
	bool aDel = false;
	IPointCollectionPtr ipPointCollection = ipResultGeom;
	long aPointCount = 0;
	IPointPtr ipPoint;
	ipPointCollection->get_PointCount( &aPointCount );
	double aBaseX, aBaseY;

	if(aPointCount > 4){
		//4点以下（実質既に4点の時しかない）はそれ以上削除できないから見ない
		for( long j = 0; j < aPointCount; j++) {
			ipPointCollection->get_Point( j, &ipPoint);
			ipPoint->QueryCoords(&aBaseX, &aBaseY);
			POINT_SET::const_iterator aCmp;
			for(aCmp = cDeletePoints.begin(); aCmp != cDeletePoints.end(); aCmp++){
				if(aBaseX == aCmp->m_Center.X && aBaseY == aCmp->m_Center.Y){
					ipPointCollection->RemovePoints(j, 1);
					if(j == 0){
						aDel = true;
						CString aMsg;
						aMsg.Format(_T("始点削除！(%d)"), j);
						fnPrintLogStyle(info, cIndex, cObjID, aMsg);
					}else{
						CString aMsg;
						aMsg.Format(_T("削除！(%d)"), j);
						fnPrintLogStyle(info, cIndex, cObjID, aMsg);
					}
					aPointCount--;
					j--;
					break;
				}
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
	return ipResultGeom;
}

// ポイント（座標）から都市地図のメッシュコードを計算する関数
long CDelSinglePoint::fnCalcMeshCode(const WKSPoint& cPoint)
{
	//座標変換ライブラリから抜粋＋α
	double	wid2		= 0.0,	hei2		= 0.0;
	double	wid64		= 0.0,	hei64		= 0.0;
	double	lat_amari1	= 0.0,	lon_amari1	= 0.0;
	double	lat_amari2	= 0.0,	lon_amari2	= 0.0;
	int		up_1st		= 0,	low_1st		= 0;
	int		up_2nd		= 0,	low_2nd		= 0;
	int		up_64		= 0,	low_64		= 0;

	wid2		= 7.0 * 1.0 / 60.0 + 30.0 * 1.0 / 3600.0; 
	hei2		= 5.0 * 1.0 / 60.0;

	wid64		= wid2 / 8.0;
	hei64		= hei2 / 8.0;

	
	up_1st		= int(cPoint.Y * 1.5);
	low_1st		= int(cPoint.X - 100);

	lat_amari1	= cPoint.Y - (up_1st / 1.5) ;
	lon_amari1	= cPoint.X - (low_1st + 100);

	up_2nd		= int(lat_amari1 / hei2);
	low_2nd		= int(lon_amari1 / wid2);
	lat_amari2	= lat_amari1 - up_2nd * hei2;
	lon_amari2	= lon_amari1 - low_2nd * wid2;
	up_64		= int(lat_amari2 / hei64);
	low_64		= int(lon_amari2 / wid64);

	long meshcode	= up_1st * 1000000 + low_1st * 10000 +
					up_2nd * 1000 + low_2nd * 100 +
					up_64 * 10 + low_64;
	return meshcode;
}
