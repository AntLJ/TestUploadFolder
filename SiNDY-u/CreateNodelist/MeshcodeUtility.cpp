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

#include "stdafx.h"

#include "MeshcodeUtility.h"


static crd_cnv gCrdCnv;


// 実数緯度経度から2次メッシュコードを取得する(境界線上)
std::vector<int> CMeshcodeUtility::GetBelong2ndMeshcode(const WKSPoint &lonlat)
{
	std::vector<int> meshcodes;

	int nCurrentMesh = -1, nX = -1, nY = -1;
	gCrdCnv.LLtoMesh( lonlat.X, lonlat.Y, 2, &nCurrentMesh, &nX, &nY, 1 );
	meshcodes.push_back( nCurrentMesh );

	if( nX == 0 ){
		int nMesh = gCrdCnv.GetMeshCode( nCurrentMesh, -1, 0 );
		meshcodes.push_back( nMesh );
	}
	if( nY == 0 ){
		int nMesh = gCrdCnv.GetMeshCode( nCurrentMesh, 0, -1 );
		meshcodes.push_back( nMesh );
	}

	return meshcodes;
}


// 2次メッシュコードから実数緯度経度を取得する
WKSPoint CMeshcodeUtility::MeshToLL(const int &meshcode)
{
	double x = -1, y = -1;
	gCrdCnv.MeshtoLL( meshcode, 0, 0, &x, &y);
	WKSPoint lonlat = { x, y };
	return lonlat;
}


// 数緯度経度から2次メッシュコードを取得する
int CMeshcodeUtility::Get2ndMeshcode(const WKSPoint &lonlat)
{
	int mesh = -1, x = -1, y = -1;

	gCrdCnv.LLtoMesh( lonlat.X, lonlat.Y, 2, &mesh, &x, &y);
	return mesh;
}
