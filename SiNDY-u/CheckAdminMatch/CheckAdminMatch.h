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

// CheckAdminMatch.h: CheckAdminMatch クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKADMINMATCH_H__7B981D5E_E56A_4605_8981_A34876DDC8BC__INCLUDED_)
#define AFX_CHECKADMINMATCH_H__7B981D5E_E56A_4605_8981_A34876DDC8BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FD_CITY_CODE		"CityCode"
#define FD_ADDR_CODE		"AddrCode"
#define FD_GAIKU_FUGO		"GaikuFugo"
#define FD_EXT_GAIKU_FUGO	"ExtGaikuFugo"
#define FD_AREA_CLASS_C		"AreaClass_c"
#define FD_ADDR_CLASS_C		"AddrClass_c"

#define	ON_FiveCode		0x01	// 市区町村単位(5桁)実行モード
#define	ON_ElevenCode	0x04	// 11桁単位実行モード

/// 市区町村リスト作成タイプ
enum	ECityListType {
	kCLNone			= 0,	// 無指定
	kCLAll			= 1,	// 全市区町村指定
	kCLList			= 2,	// リストファイルでの市区町村指定
	kCLSingle		= 3,	// 単一市区町村指定
	kCLRange		= 4		// コード範囲指定
};

struct AC11Rec
{

	string	eAC11;
	set<long, less<long> >	eOIDSet;

	AC11Rec() {
	}

	AC11Rec(const AC11Rec &cAC11Rec) {
		eAC11		= cAC11Rec.eAC11;
		eOIDSet		= cAC11Rec.eOIDSet;
	}

	~AC11Rec() {
	}

	bool operator < (const AC11Rec &cAC11Rec) const {
		return eAC11 < cAC11Rec.eAC11;
	}

};

typedef set<AC11Rec, less<AC11Rec> >	AC11Set;

struct AC8Rec
{

	string	eAC8;
	AC11Set	eAC11Set;

	AC8Rec() {
	}

	AC8Rec(const AC8Rec &cAC8Rec) {
		eAC8		= cAC8Rec.eAC8;
		eAC11Set	= cAC8Rec.eAC11Set;
	}

	~AC8Rec() {
	}

	bool operator < (const AC8Rec &cAC8Rec) const {
		return eAC8 < cAC8Rec.eAC8;
	}

};

typedef set<AC8Rec, less<AC8Rec> >		AC8Set;

class ACSet : public AC8Set
{

public:

	void	Add(const string &cAC11, const long cOID) {
		AC8Rec	aAC8Rec;
		aAC8Rec.eAC8	= cAC11.substr(0, 8);
		AC8Set::iterator aAC8Itr = find(aAC8Rec);
		if(aAC8Itr != end()) {	// 同一８桁が見つかった
			aAC8Rec = *aAC8Itr;	// コピー
			erase(aAC8Itr);		// 一旦削除
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			AC11Set::iterator aAC11Itr = aAC8Rec.eAC11Set.find(aAC11Rec);
			if(aAC11Itr != aAC8Rec.eAC11Set.end()) {		// 同一11桁が見つかった
				aAC11Rec = *aAC11Itr;				// コピー
				aAC8Rec.eAC11Set.erase(aAC11Itr);	// 一旦削除
				aAC11Rec.eOIDSet.insert(cOID);		// 登録（OID）
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// 再登録（11桁）
				insert(aAC8Rec);					// 再登録（８桁）
			} else {										// 同一11桁が見つからない
				aAC11Rec.eOIDSet.insert(cOID);		// 登録（OID）
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// 登録（11桁）
				insert(aAC8Rec);					// 再登録（８桁）
			}
		} else {				// 同一８桁が見つからない
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			aAC11Rec.eOIDSet.insert(cOID);			// 登録（OID）
			aAC8Rec.eAC11Set.insert(aAC11Rec);		// 登録（11桁）
			insert(aAC8Rec);						// 登録（８桁）
		}
	}

};

struct AddrPolyRec
{

	long				eOID;
	IPolygonPtr			ipPolygon;

	AddrPolyRec& operator = (const AddrPolyRec &cAPRec) {
		eOID		= cAPRec.eOID;
		ipPolygon	= cAPRec.ipPolygon;
		return *this;
	}

};

typedef list<AddrPolyRec>	AddrPoly;
typedef list<AddrPoly>		AddrGroup;

struct AdminAttr
{

	string	eAddrCode;
	long	eGaikuFugo;
	char	eAreaClassC;
	char	eAddrClassC;

	AdminAttr() {
		eGaikuFugo	= 0;
		eAreaClassC	= '\0';
		eAddrClassC	= '\0';
	}

	AdminAttr& operator = (const AdminAttr &cAdmAttr) {
		eAddrCode	= cAdmAttr.eAddrCode;
		eGaikuFugo	= cAdmAttr.eGaikuFugo;
		eAreaClassC	= cAdmAttr.eAreaClassC;
		eAddrClassC	= cAdmAttr.eAddrClassC;
		return *this;
	}

};

class CheckAdminMatch : public SDEAccessMS  
{

private:

	int		mArgc;
	char	**mArgv;

	char	*mDBCon_Address;
	char	*mPGDB_OUT;
	char	*mFC_OUT;
	char	*m_JDB_CONNECT;

	int		mDBID_Address;
	int		mDBID_AddrChk;

	char	*mFC_CityAdmin;
	char	*mFC_BaseAdmin;
	// [bug 9651]対応で修正
	string	mFC_TmpPoly;

	IFeatureClassPtr	ipFC_CityAdmin;
	IFeatureClassPtr	ipFC_BaseAdmin;

	IFeatureClassPtr	ipFC_TmpPoly;

	long	mFID_CityCode;
	long	mFID_AddrCode;
	long	mFID_GaikuFugo;
	long	mFID_ExtGaikuFugo;
	long	mFID_AreaClassC;
	long	mFID_AddrClassC;

	long	mFID_AddrCode_TP;

	char	*mFN_ChkLog;
	OutputErrLog	mChkLog;

	char	*mFN_CityList;
	char	*mCityCode;

	long	mRangeMin;
	long	mRangeMax;

	double	mDist1;
	double	mDist2;
	double	mDist3;
	double	mDist4;

	IPolygonPtr	ipP_BaseAdmin;
	long		mOID_BaseAdmin;

	int		m_iExeMode;			// 11桁実行or5桁実行

	CityList	mCityList;

	ECityListType	mCLType;

	ACSet		mACSet;

	AddrPoly	mAddrPoly11;
	AddrPoly	mAddrPoly8;
	AddrPoly	mAddrPolyB;

	crd_cnv		mCConv;
	JDBAccess	mJDBAccess;
	
	ISpatialCacheManagerPtr	ipSpCacheMgr;

	bool	fnGetEnvVar();

	bool	fnAnalyzeArg();

	bool	fnCreateTmpFC();

	bool	fnMakeCityList();

	void	fnSetBaseAdminPoly(const char *cCityCode);

	bool	fnMakeACList(const char *cCityCode);

	bool	fnOpenChkLog();

	bool	fnAdminProc(IFeaturePtr ipF_CityAdmin, const char *cCityCode);

	bool	fnCheckAPDist(const string &cAddrCode, const AddrPoly &cAddrPoly, double cDist);

	bool	fnCreateCache8(const string &cAddrCode);

	bool	fnCreatePolyList8(const string &cAddrCode);

public:

	CheckAdminMatch();

	virtual ~CheckAdminMatch();

	bool Execute(int cArgc, char **cArgv);

	static	void	printUsage();

};

#endif // !defined(AFX_CHECKADMINMATCH_H__7B981D5E_E56A_4605_8981_A34876DDC8BC__INCLUDED_)
