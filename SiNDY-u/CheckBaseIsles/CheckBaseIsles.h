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

// CheckBaseIsles.h: CheckBaseIsles クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKBASEISLES_H__0D58D27D_23CC_42CC_9AAE_E76FB6ED4459__INCLUDED_)
#define AFX_CHECKBASEISLES_H__0D58D27D_23CC_42CC_9AAE_E76FB6ED4459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <OutputErrLog.h>
#include "MeshList.h"
#include "arcobjects.h"

#define PROGRAM_ID		"H04xx"

#define ECT_MAX_RECORD			3

#define ERR_ILLEGAL_ADMIN_POLY	1
#define ERR_NO_ADMIN_POLY		2
#define ERR_FAILED_AO_METHOD	901

static ErrCodeRec gECTable[] = {
	{ ERR_ILLEGAL_ADMIN_POLY,	kError,		"適切な行政界ポリゴンに含まれていない" },
	{ ERR_NO_ADMIN_POLY,		kError,		"メッシュ内に行政界ポリゴンがない" },
	{ ERR_FAILED_AO_METHOD,		kError,		"メソッドの失敗(ArcObjects)" },
};

/// チェックモード
enum	ECheckMode {
	kCheckNone			= 0,	// 無指定
	kCheckAllMesh		= 1,	// オプション -a 指定
	kCheckMeshesInList	= 2,	// オプション -l 指定
	kCheckSingleMesh	= 3		// オプション -m 指定
};

class CheckBaseIsles
{

private:

	int		mArgc;
	char	**mArgv;

	ECheckMode	mCheckMode;

	char	*mDBCon_Base;
	char	*mDBCon_Address;

	IWorkspacePtr m_ipBaseWS;
	IWorkspacePtr m_ipAddrWS;

	char	*mMeshCode;
	char	*mMeshListFN;
	char	*mErrorLogFN;

	CString	mFCN_CityAdmin;
	CString	mFCN_BaseSite;
	CString	mFCN_BaseMesh;

	IFeatureClassPtr	ipFC_CityAdmin;
	IFeatureClassPtr	ipFC_BaseSite;
	IFeatureClassPtr	ipFC_BaseMesh;

	long	mFID_Sc1BgClass_c;
	long	mFID_Sc2BgClass_c;
	long	mFID_Sc3BgClass_c;
	long	mFID_Sc4BgClass_c;

	long	mFID_AreaClass_c;

	long	mFID_MeshCode;

	MeshList	mMeshList;

	OutputErrLog	mErrLog;

	bool	fnGetEnvVar(void);

	bool	fnAnalyzeArg(void);

	void	fnPrintUsage(void);

	bool	fnMakeMeshList(void);

public:

	CheckBaseIsles();

	virtual ~CheckBaseIsles();

	bool	Execute(int cArgc, char **cArgv);
};

#endif // !defined(AFX_CHECKBASEISLES_H__0D58D27D_23CC_42CC_9AAE_E76FB6ED4459__INCLUDED_)
