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

// CreateMeshList.h: CreateMeshList クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_)
#define AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SDEAccess.h>
#include "MeshList.h"
#include <libsdeaccess.h>

#define FC_CITYMESH		"reference.CityMesh"
#define FC_BASEMESH		"reference.BaseMesh"
#define FC_CITYADMIN	"City_Admin"

#define FD_CITYMESHCODE	"MeshCode"		//2008/05/28 メッシュコードフィールドの変更に対応 （梶浦）
#define FD_COMPLETE_F	"CREATE_YEAR"	//2008/05/28 エリア内外判定の変更に対応 （梶浦）
#define FD_BASEMESHCODE	"MeshCode"

class CreateMeshList : public SDEAccess 
{

private:

	int		mArgc;
	char	**mArgv;

	char	*mOutFN;

	FILE	*mOutFP;

	bool	mAllAdmin;

	char	*mDBCon_Address;

	char	mFCCityMesh[50];
	char	mFCBaseMesh[50];
	char	mFCCityAdmin[50];

	char	mFDCityMeshCode[50];
	char	mFDCompleteF[50];

	IFeatureClassPtr	ipFC_CityMesh;
	IFeatureClassPtr	ipFC_BaseMesh;
	IFeatureClassPtr	ipFC_CityAdmin;

	MeshList	mMeshList;

	bool	fnAnalyzeArg(void);

	void	fnPrintUsage(void);

	/**
	 * @brief SpatialRelDescriptionを利用したshapeによる検索
	 * @param ipGeometry [in] 検索用shapeのジオメトリ・インターフェース
	 * @param cSpatialRelDesc [in] 空間的関係を示す記述
	 * @param ipFeatureClass [in] 検索対象のフィーチャクラス・インタフェース
	 * @param cCount [out] 検索されたフィーチャの件数
	 *
	 * @return 検索の結果が格納されるフィーチャカーソル
	 */
	IFeatureCursorPtr	SearchByShapeAndDesc(
		IGeometryPtr		ipGeometry,
		LPCTSTR				cSpatialRelDesc,
		IFeatureClassPtr	ipFeatureClass,
		LONG				*cCount
		);

public:

	CreateMeshList();
	virtual ~CreateMeshList();

	bool Create(int cArgc, char **cArgv);

};

#endif // !defined(AFX_CREATEMESHLIST_H__35D55BED_C2DE_4486_B7A4_1462AFDB1F56__INCLUDED_)
