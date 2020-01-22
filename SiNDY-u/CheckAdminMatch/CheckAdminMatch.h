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

// CheckAdminMatch.h: CheckAdminMatch �N���X�̃C���^�[�t�F�C�X
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

#define	ON_FiveCode		0x01	// �s�撬���P��(5��)���s���[�h
#define	ON_ElevenCode	0x04	// 11���P�ʎ��s���[�h

/// �s�撬�����X�g�쐬�^�C�v
enum	ECityListType {
	kCLNone			= 0,	// ���w��
	kCLAll			= 1,	// �S�s�撬���w��
	kCLList			= 2,	// ���X�g�t�@�C���ł̎s�撬���w��
	kCLSingle		= 3,	// �P��s�撬���w��
	kCLRange		= 4		// �R�[�h�͈͎w��
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
		if(aAC8Itr != end()) {	// ����W������������
			aAC8Rec = *aAC8Itr;	// �R�s�[
			erase(aAC8Itr);		// ��U�폜
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			AC11Set::iterator aAC11Itr = aAC8Rec.eAC11Set.find(aAC11Rec);
			if(aAC11Itr != aAC8Rec.eAC11Set.end()) {		// ����11������������
				aAC11Rec = *aAC11Itr;				// �R�s�[
				aAC8Rec.eAC11Set.erase(aAC11Itr);	// ��U�폜
				aAC11Rec.eOIDSet.insert(cOID);		// �o�^�iOID�j
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// �ēo�^�i11���j
				insert(aAC8Rec);					// �ēo�^�i�W���j
			} else {										// ����11����������Ȃ�
				aAC11Rec.eOIDSet.insert(cOID);		// �o�^�iOID�j
				aAC8Rec.eAC11Set.insert(aAC11Rec);	// �o�^�i11���j
				insert(aAC8Rec);					// �ēo�^�i�W���j
			}
		} else {				// ����W����������Ȃ�
			AC11Rec	aAC11Rec;
			aAC11Rec.eAC11	= cAC11;
			aAC11Rec.eOIDSet.insert(cOID);			// �o�^�iOID�j
			aAC8Rec.eAC11Set.insert(aAC11Rec);		// �o�^�i11���j
			insert(aAC8Rec);						// �o�^�i�W���j
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
	// [bug 9651]�Ή��ŏC��
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

	int		m_iExeMode;			// 11�����sor5�����s

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
