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

// CreateExistFile.h: CreateExistFile クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_)
#define AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <SDEAccess.h>
#include "MeshList.h"

// とりあえずVC11とそれ以外の2パターンにしておく 2012.11.02
#ifdef _DEBUG
	#if _MSC_VER == 1700
		#pragma comment(lib,"SDEAccess_vc11-gd.lib")
	#else
		#pragma comment(lib,"SDEAccessD.lib")
	#endif
#else
	#if _MSC_VER == 1700
		#pragma comment(lib,"SDEAccess_vc11.lib")
	#else
		#pragma comment(lib,"SDEAccess.lib")
	#endif
#endif


#define ARG_MIN		1

#define DIV_MAX		64

#define FC_CITYMESH		"reference.CityMesh"
#define FC_BASEMESH		"reference.BaseMesh"
#define FC_CITYSITE		"City_Site"

#define FD_CITYMESHCODE	"MeshCode"
#define FD_BASEMESHCODE	"MeshCode"
#define FD_COMPLETE_F	"Complete_f"
#define FD_BGCLASS_C	"BgClass_c"

/// 生成モード
enum	ECreateMode {
	kCreateNone			= 0,	// 無指定
	kCreateAllMesh		= 1,	// オプション -a による全メッシュ指定
	kCreateMeshesInList	= 2,	// オプション -l によるメッシュリストでのメッシュ指定
	kCreateSingleMesh	= 3		// オプション -m による単一メッシュ指定
};

/// 存在判定ファイル生成クラス
class CreateExistFile : public SDEAccess
{

private:

	int			mArgc;
	char		**mArgv;

	string		mOutDir;

	ECreateMode	mCreateMode;

	int			mDivNum;

	string		mFNMeshList;
	string		mMeshCode;

	MeshList	mMeshList;

	string		mDBMap;

	string		mFCCityMesh;
	string		mFCBaseMesh;
	string		mFCCitySite;

	IFeatureClassPtr	ipFC_CityMesh;
	IFeatureClassPtr	ipFC_BaseMesh;
	IFeatureClassPtr	ipFC_CitySite;

	long		mFID_CityMeshCode;
	long		mFID_BaseMeshCode;
	long		mFID_CompleteFlag;
	long		mFID_BGClass_C;

	bool	fnAnalyzeArg();

	void	fnPrintUsage();

	bool	fnMakeMeshList();

	bool	fnCreateCache(const string &cMeshCode);

	IPolygonPtr	fnCreateIncompletePoly(IEnvelopePtr ipE_CityMesh, long *cCount);

public:

	CreateExistFile();
	virtual ~CreateExistFile();

	/// 存在判定ファイルの生成
	bool	Create(int cArgc, char **cArgv);

private:
	bool ExistError;

};

#endif // !defined(AFX_CREATEEXISTFILE_H__012E22F1_38D2_426D_9F95_A7A3785DB30E__INCLUDED_)
