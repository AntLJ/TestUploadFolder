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
#include "GetPolyLineInfo.h"

CGetPolyLineInfo::CGetPolyLineInfo(void)
{
}

CGetPolyLineInfo::~CGetPolyLineInfo(void)
{
}

bool CGetPolyLineInfo::GetPolyline(IGeometryPtr ipGeometry,std::multimap<Key, Info>& rInfoMap, Key * pKey, Info * pInfo)
{
	//形状を点列の集合に変換
	IPointCollectionPtr ipPointCollection(ipGeometry);
	long lPointCount;
	if(FAILED(ipPointCollection->get_PointCount(&lPointCount) ) ){
		std::cerr << "構成点の取得に失敗" << std::endl;
		return false;
	}
	//構成点が2未満であれば
	if(lPointCount < 2 ){
		std::cerr << "構成点数が2点未満" << std::endl;
		return false;
	}

	//始点を取得
	IPointPtr ipPoint;
	if(FAILED(ipPointCollection->get_Point(0,&ipPoint))
		||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
			std::cerr << "始点の取得に失敗しました" <<std::endl;
			return false;
	}
	//始点の補間点番号取得
	pInfo->e_PointNumber = 0;
	//始点の情報をInfoMapに挿入
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));


	//終点を取得
	if(FAILED(ipPointCollection->get_Point(lPointCount-1,&ipPoint))
		||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
			std::cerr << "終点の取得に失敗しました" <<std::endl;
			return false;
	}
	//終点の補間点番号を取得
	pInfo->e_PointNumber = lPointCount-1;
	//終点の情報をrInfoMapに挿入
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));

	return true;

}

	
	




