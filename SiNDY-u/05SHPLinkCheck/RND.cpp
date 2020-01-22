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
#include "RND.h"
#include<map>
#include <atl2/io.h>

CRND::CRND(IFeatureClassPtr ipFeatureClass)
{
	m_ipFeatureClass = ipFeatureClass;
}

CRND::~CRND(void)
{
}


bool CRND::Point2Map(std::map<std::pair<long, long>, WKSPoint>& rSortSet, std::wstring rNodeFileName, bool bWalkOpt)
{
	HRESULT hRetObjct, hRetDiv;
	long lOBJECTIDIndex;
	//OBJECTIDのフィールド上のｲﾝﾃﾞｯｸｽ取得
	hRetObjct = m_ipFeatureClass->FindField(_T("OBJECTID"),&lOBJECTIDIndex);
	if(FAILED(hRetObjct) || (-1 == lOBJECTIDIndex)){
		std::cerr << rNodeFileName << ":フィールドインデックスの取得に失敗しました"<<std::endl;
		return false;
	}

	long lDIVIDIndex;
	if(bWalkOpt){
		//DIVIDのフィールド上のｲﾝﾃﾞｯｸｽ取得
		hRetDiv = m_ipFeatureClass->FindField(_T("DIVID"), &lDIVIDIndex);
		if(FAILED(hRetDiv) || (-1 == lDIVIDIndex)){
			std::cerr << rNodeFileName << ":フィールドインデックスの取得に失敗しました"<<std::endl;
			return false;
		}
	}

	IFeatureCursorPtr ipFeatureCursor;
	//ﾌｨｰﾁｬｰｶｰｿﾙ取得
	if( FAILED( m_ipFeatureClass->Search(NULL, VARIANT_FALSE, & ipFeatureCursor) ) ){
		std::cerr << rNodeFileName << ":ｶｰｿﾙの検索に失敗しました。" << std::endl;
		return false;
		}
		
	IFeaturePtr ipFeature;						
	//ﾌｨｰﾁｬｰｶｰｿﾙが尽きるまでﾌｨｰﾁｬを巡回
	while (ipFeatureCursor->NextFeature(&ipFeature)==S_OK && (ipFeature != NULL)) {

		long lOID = ipFeature->GetOID();

		CComVariant ObjectID;						//ObjectID
		if(FAILED(ipFeature->get_Value(lOBJECTIDIndex, &ObjectID))){
			std::cerr << rNodeFileName << "\tFID:" << lOID << "\tOBJECTIDの取得に失敗" << std::endl;
			continue;
		}
				
		CComVariant DivID;							//DivID
		DivID.lVal = -1;							//初期値設定(RNDの場合は-1固定とする)
		if(bWalkOpt){
			if(FAILED(ipFeature->get_Value(lDIVIDIndex, &DivID))){
				std::cerr << rNodeFileName << "\tFID:" << lOID << "\tDIVIDの取得に失敗" << std::endl;
				continue;
			}
		}

		//形状を取得
		IGeometryPtr ipGeometry;
		if(FAILED(ipFeature->get_ShapeCopy(&ipGeometry)) ){
			std::cerr << rNodeFileName << "\tFID:" <<  lOID << "\t形状の取得に失敗" << std::endl;
			continue;
		}

		//形状→点				
		IPointPtr ipPoint(ipGeometry);
		
		//座標取得
		WKSPoint Point;	
		if( FAILED(ipPoint->get_X(&(Point.X))) || FAILED(ipPoint->get_Y(&(Point.Y))) ){
			std::cerr << rNodeFileName << "\tFID:" <<  lOID << "\t座標の取得に失敗"<<std::endl;
			continue;
		}

		std::pair<long, long> pairOidDivid(ObjectID.lVal, DivID.lVal);
		rSortSet.insert(std::pair<std::pair<long, long>, WKSPoint>(pairOidDivid, Point));
	}
	return true;
}
