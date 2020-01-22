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

// CalcArea.cpp: CalcArea クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcArea.h"

//#define __SPACIAL__ //ダイナのエリア面積計算用
#define CITY_CODE_NAME "CITYCODE"
#define AREA_FLAG_NAME "AREACLASS_C"

#define M_PI (3.1415926535897932)
#define EARTH_R (6370)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CalcArea::CalcArea()
{
	m_AllFlag = false;
	m_SpecialFlag = false;
}

CalcArea::~CalcArea()
{

}

#ifdef __FOR_BT__
bool CalcArea::init(IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipExtClass, LPCTSTR cOutDir, bool cAllFlag, bool cSpecialFlag)
#else
bool CalcArea::init(IFeatureClassPtr ipFeatureClass, LPCTSTR cOutDir, bool cAllFlag, bool cSpecialFlag)
#endif
{
#ifdef __FOR_BT__
	m_ipExtClass = ipExtClass;
#endif

	m_ipFeatureClass = ipFeatureClass;
#ifndef __SPACIAL__
	if(FAILED(m_ipFeatureClass->FindField(CComBSTR(CITY_CODE_NAME), &m_CityCodeIndex))){
		fprintf(stderr, "都道府県コードフィールド検索失敗[%s]。\n",CITY_CODE_NAME);
		return false;
	}
	if(FAILED(m_ipFeatureClass->FindField(CComBSTR(AREA_FLAG_NAME), &m_AreaFlagIndex))){
		fprintf(stderr, "エリアフラグフィールド検索失敗[%s]。\n",AREA_FLAG_NAME);
		return false;
	}
#endif
	m_AllFlag = cAllFlag;
	m_SpecialFlag = cSpecialFlag;
	m_OutDir = cOutDir;
	return true;
}

bool CalcArea::execCalcArea(long cAddrCode)
{
	printf("%02d,計算中・・・\n", cAddrCode);
	if(!fnPrecheck(cAddrCode)){
		return false;
	}
	bool aResult = false;
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
#ifndef __SPACIAL__
	CString aWhere;
	aWhere.Format("%s LIKE '%02d%%'", CITY_CODE_NAME, cAddrCode);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
#endif
	IFeatureCursorPtr ipCursor;
	if(SUCCEEDED(m_ipFeatureClass->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
		IFeaturePtr ipFeature;
		CityArea aCityArea;
		m_AreaSet.clear();
		while(ipCursor->NextFeature(&ipFeature) == S_OK){
#ifndef __SPACIAL__
			if(!m_AllFlag){
				CComVariant aFlag;
				ipFeature->get_Value(m_AreaFlagIndex, &aFlag);
				if(aFlag.lVal != 1){ //都市地図エリア内でなければ
					continue;
				}
			}
#endif
#ifdef _DEBUG
			LONG aObjID;
			ipFeature->get_OID(&aObjID);
//			if(aObjID == 2639925 || aObjID == 2614825 || aObjID == 2621128){
			if(aObjID == 2575324){
				int a = 0;
				IGeometryPtr ipGeom;
				ipFeature->get_Shape(&ipGeom);
				aCityArea.m_Area = fnCalcArea(ipGeom);
			}
#endif
			IGeometryPtr ipGeom = fnGetTgtGeom(ipFeature);
			aCityArea.m_Area = fnCalcArea(ipGeom);
			if(aCityArea.m_Area == 0.0){
				continue;
#ifdef _DEBUG
				printf("面積０,%d\n", aObjID);
#endif
			}
#ifndef __SPACIAL__
			CComVariant aVal;
			ipFeature->get_Value(m_CityCodeIndex, &aVal);
			_bstr_t aTmpStr(aVal.bstrVal);
			aCityArea.m_CityCode = atoi((char*)aTmpStr);
			if(m_SpecialFlag){
				int Shi = aCityArea.m_CityCode % 1000;
				int Ken = aCityArea.m_CityCode / 1000;
				if(Ken != 13 && (Shi / 100) == 1){ //東京以外の政令市
//					if((Shi / 10) == 13 || (Shi / 10) == 14){	//2007/11/22 「2714* 堺市」暫定対応
//					if((Shi / 10) == 13 || (Shi / 10) == 14 || (Shi / 10) == 19){	//2009/11/17 「1419* 相模原市」暫定対応
//					if((Shi / 10) == 13 || (Shi / 10) == 14 || (Shi / 10) == 15){	//2010/05/18 「1415* 相模原市」対応
					if((Shi / 10) == 13 || (Shi / 10) == 14 || (Shi / 10) == 15 || (Shi / 10) == 19){	//2011/11/16 「熊本市」対応→仮コード全般に有効だと思うので「19」は入れておく
						aCityArea.m_CityCode = (aCityArea.m_CityCode / 10) * 10;
					}else{
						aCityArea.m_CityCode = (aCityArea.m_CityCode /100) * 100;
					}
				}
			}
#else
			aCityArea.m_CityCode = cAddrCode;
#endif
			m_AreaSet.insert(aCityArea);
//			printf("%s,%f\n", (char*)aTmpStr, aCityArea.m_Area);
		}
		aResult = fnResultOut();
	}else{
		fprintf(stderr, "Error, 検索失敗:%02d\n", cAddrCode);
	}
	printf("終了\n");
	return aResult;
}

double CalcArea::fnCalcArea(IGeometryPtr ipGeom)
{
	IAreaPtr ipArea(ipGeom);
	double aArea;
	ipArea->get_Area(&aArea);
	if(aArea > 0.0){
		IPointPtr ipPoint(CLSID_Point);
		ipArea->QueryCentroid(ipPoint);
		double aY;
		ipPoint->get_Y(&aY);
		double aRatio = EARTH_R * sin(M_PI / 2.0 - aY * M_PI / 180.0) * 2.0 * M_PI/360.0;
	//	aArea *= (aRatio * aRatio);
		aArea *= (aRatio * EARTH_R * 2.0 * M_PI / 360.0);
	//	aArea *= 1000000.0;
	}else{
		aArea = 0.0;
	}
	return aArea;
}

bool CalcArea::fnResultOut()
{
	AREA_MSET::const_iterator it;
	FILE *aOutFP;
	if((aOutFP = fopen(m_OutFile, "wt")) == NULL){
		fprintf(stderr, "ファイルが開けない。,%s\n", static_cast<LPCTSTR>(m_OutFile));
		return false;
	}
	it = m_AreaSet.begin();
	if(it != m_AreaSet.end()){
		long aCurrent = it->m_CityCode;
		double Total = it->m_Area;
		for(it++; it != m_AreaSet.end(); it++){
			if(aCurrent == it->m_CityCode){
				Total += it->m_Area;
			}else{
				fprintf(aOutFP, "%05d,%f\n", aCurrent, Total);
				aCurrent = it->m_CityCode;
				Total = it->m_Area;
			}
		}
		fprintf(aOutFP, "%05d,%f\n", aCurrent, Total);
	}
	fclose(aOutFP);
	return true;
}

bool CalcArea::fnPrecheck(long cAddrCode)
{
	m_OutFile.Format("%s\\%02d.log", m_OutDir, cAddrCode);
	FILE *aOutFP;
	if((aOutFP = fopen(m_OutFile, "wt")) == NULL){
		fprintf(stderr, "ファイルが開けない。,%s\n", static_cast<LPCTSTR>(m_OutFile));
		return false;
	}
	fclose(aOutFP);
	return true;
}

IGeometryPtr CalcArea::fnGetTgtGeom(IFeaturePtr ipFeature)
{
	IGeometryPtr ipGeom;
	ipFeature->get_Shape(&ipGeom);
#ifdef __FOR_BT__
	ISpatialFilterPtr ipFilter(CLSID_SpatialFilter);
	CComBSTR          bstrFieldName;
	m_ipExtClass->get_ShapeFieldName(&bstrFieldName);
	ipFilter->put_GeometryField(bstrFieldName);
	ipFilter->putref_Geometry(ipGeom);
	ipFilter->put_SpatialRel( esriSpatialRelIntersects );
//	aSubField.Format("OBJECTID,SHAPE,%s,%s", BLDCLASS_FIELD, WATARI_FIELD);
	IFeatureCursorPtr ipCursor;
	if(m_ipExtClass->Search(ipFilter, VARIANT_FALSE, &ipCursor) != S_OK){
		fprintf(stderr, "#Error,行政界にかかる除外エリアの検索失敗,\n");
		return NULL;
	}
	IFeaturePtr ipExtFeature;
	while(ipCursor->NextFeature(&ipExtFeature) == S_OK){
		ITopologicalOperatorPtr ipTopo = ipGeom;
		IGeometryPtr ipExtGeom;
		ipExtFeature->get_Shape(&ipExtGeom);
		((ITopologicalOperator2Ptr)ipExtGeom)->put_IsKnownSimple(VARIANT_FALSE);
		((ITopologicalOperator2Ptr)ipExtGeom)->Simplify();
		IGeometryPtr ipResult;
		ipTopo->Difference(ipExtGeom, &ipResult);
		((ITopologicalOperator2Ptr)ipResult)->put_IsKnownSimple(VARIANT_FALSE);
		((ITopologicalOperator2Ptr)ipResult)->Simplify();
		ipGeom = ipResult;
	}
#endif
	return ipGeom;
}
