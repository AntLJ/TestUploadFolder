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

#pragma once
#include"Definition.h"
#include<map>

class CGetInfo
{
public:
	CGetInfo(void);
	~CGetInfo(void);
	bool GetInformation(std::multimap<Key,Info> & rInfoMap,std::wstring & rDirName, std::wstring & rLayerName,long aMeshCode,long & rFeatureType);	//フィーチャー情報をrInfoMapに格納する関数
	//virtual bool GetPoint( IGeometryPtr  ipGeometry,std::multimap<Key, Info>& rInfoMap,Key * pKey,Info * pInfo) =0;		//フィーチャーの点情報を取得
	//virtual bool GetPolyLine( IGeometryPtr  ipGeometry,std::multimap<Key, Info>& rInfoMap,Key * pKey,Info * pInfo) =0;		//フィーチャーの線情報を取得
	//virtual bool GetPolygon( IGeometryPtr  ipGeometry,std::multimap<Key, Info>& rInfoMap,Key * pKey,Info * pInfo) =0;		//フィーチャーの面情報を取得

};
