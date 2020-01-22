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
#include "GetPolygonInfo.h"


CGetPolygonInfo::CGetPolygonInfo(void) 
{
}

CGetPolygonInfo::~CGetPolygonInfo(void)
{
}

bool CGetPolygonInfo::GetPolygon( IGeometryPtr ipGeometry, std::multimap<Key,Info>& rInfoMap,Key * pKey, Info * pInfo)
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
		std::cerr << "構成点が2未満" << std::endl;
		return false;
	}

	//構成点の情報を取得し、それぞれrInfoMapに挿入
	IPointPtr ipPoint;
	for (int i=0;i < lPointCount;i++)
	{
		//i番目の座標を取得
		if(FAILED(ipPointCollection->get_Point(i,&ipPoint))
			||FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y) ) ) ){
				std::cerr << i <<"点目の取得に失敗しました" <<std::endl;
				return false;
		}
		//補間点番号の取得
		pInfo->e_PointNumber= i;
		//i番目の点情報をrInfoMapに挿入
		rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));
	}

	return true;
}



	
