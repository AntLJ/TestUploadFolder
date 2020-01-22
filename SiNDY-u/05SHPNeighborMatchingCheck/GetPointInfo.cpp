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
#include "GetPointInfo.h"


CGetPointInfo::CGetPointInfo(void) 
{
}

CGetPointInfo::~CGetPointInfo(void)
{
}

bool CGetPointInfo::GetPoint( IGeometryPtr ipGeometry,std::multimap<Key, Info>& rInfoMap,Key * pKey,Info * pInfo)
{
	//形状を点に
	IPointPtr ipPoint(ipGeometry);

	//座標取得
	if(FAILED(ipPoint->get_X(&(pKey->e_X))) || FAILED(ipPoint->get_Y(&(pKey->e_Y)))){
		
		return false;
	}

	//ポイントの場合補間番号が存在しないので、0を入れとく
	pInfo->e_PointNumber = 0;

	//ポイント情報をInfoMapに挿入
	rInfoMap.insert(std::pair<Key,Info>(*pKey,*pInfo));

	return true;

}
