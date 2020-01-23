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

// CountGouPoint.h: CountGouPoint クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COUNTGOUPOINT_H__A9AF2398_68F2_4061_AB39_03A6D3E6A44B__INCLUDED_)
#define AFX_COUNTGOUPOINT_H__A9AF2398_68F2_4061_AB39_03A6D3E6A44B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SDEAccessMS.h>
#include <libsdeaccessms.h>
#include "CityList.h"

#define FC_GOU_POINT	"Gou_Point"
#define FC_CITY_ADMIN	"City_Admin"
#define FC_BASE_ADMIN	"Base_Admin"

#define FD_GOUTYPE		"GouType"
#define FD_CITYCODE		"CityCode"
#define FD_ADDRCODE		"AddrCode"


/// 処理モード
enum	EMode {
	kNone			= 0,	// 無指定
	kAllCity		= 1,	// オプション -a による全市区町村に対する処理
	kCitiesInList	= 2,	// オプション -l による市区町村コードリストによる処理
	kSingleCity		= 3,	// オプション -c による指定単一市区町村の処理
};

#define	ON_FiveCode		0x01	// 市区町村単位(5桁)実行モード
#define	ON_ElevenCode	0x04	// 11桁単位実行モード

struct GouCount
{

	char	eAddrCode[12];
	long	eGouCount;
	long	eTpgCount;
	long	eAdmCount;

	GouCount() {
		memset(eAddrCode, 0, 12);
		eGouCount	= 0;
		eTpgCount	= 0;
		eAdmCount	= 0;
	}

	GouCount(char *cAddrCode, long cGouCount, long cTpgCount, long cAdmCount) {
		strcpy(eAddrCode, cAddrCode);
		eGouCount	= cGouCount;
		eTpgCount	= cTpgCount;
		eAdmCount	= cAdmCount;
	}

	bool operator < (const GouCount &cGC) const {
		if(strncmp(eAddrCode, cGC.eAddrCode, 11) < 0) return 1;
		else return 0;
	}

};

class GCSet : public set<GouCount, less<GouCount> >
{

public:

	iterator	FindCode(char *cAddrCode) {
		GouCount	aGC(cAddrCode, 0, 0, 0);
		return find(aGC);
	}

};

//typedef	set<GouCount, less<GouCount> >				GCSet;
//typedef	set<GouCount, less<GouCount> >::iterator	GCItr;

class CountGouPoint : public SDEAccessMS  
{

private:

	int		mArgc;
	char	**mArgv;

	char	*mDBCon_Address;

	char	mFCGouPoint[50];
	char	mFCCityAdmin[50];
	char	mFCBaseAdmin[50];

	char	*mCityCode;
	char	*mFNCityList;
	char	*mFNOutput;

	FILE	*mFPOutput;

	EMode	mMode;

	int		m_iExeMode;			// 11桁実行or5桁実行

	long	mDBID_Address;

	long	mFID_GouType;
	long	mFID_CityCode;
	long	mFID_AddrCode;

	IFeatureClassPtr	ipFC_GouPoint;
	IFeatureClassPtr	ipFC_CityAdmin;
	IFeatureClassPtr	ipFC_BaseAdmin;

	CityList	mCityList;

	ISpatialCacheManagerPtr	ipSpCacheMgr;

	bool	fnAnalyzeArg(void);

	void	fnPrintUsage(void);

	bool	fnMakeCityList(void);
#ifdef _USE_SPCACHE_
	bool	fnCreateCache(const char *cCityCode);

	bool	fnCreateCache(IGeometryPtr ipG_TgtArea);
#endif
public:

	CountGouPoint();

	virtual ~CountGouPoint();

	bool	Execute(int cArgc, char **cArgv);

};

#endif // !defined(AFX_COUNTGOUPOINT_H__A9AF2398_68F2_4061_AB39_03A6D3E6A44B__INCLUDED_)
