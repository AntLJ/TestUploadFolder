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

// Check.h: CCheck クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECK_H__BB21CF49_D53F_4154_AC3F_0010D560ADCE__INCLUDED_)
#define AFX_CHECK_H__BB21CF49_D53F_4154_AC3F_0010D560ADCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace sindy;
using namespace addr;

typedef std::set<CString> CODE_SET;
struct GOU_INFO {
	CString m_20Code;
	long m_ObjID;
	bool m_PriFlag;
	bool m_Oaza;
	GOU_INFO(){};
	GOU_INFO(LPCTSTR cSrc){
		m_20Code = cSrc;
		m_ObjID = -1;
		m_PriFlag = false;
		m_Oaza = false;
	}
	bool operator<(const GOU_INFO & cCmp) const {
		if(m_20Code != cCmp.m_20Code){
			return m_20Code < cCmp.m_20Code;
		}else if(cCmp.m_ObjID > 0 && m_ObjID > 0){
			return m_ObjID < cCmp.m_ObjID;
		}else{
			return false;
		}
	}
	bool operator==(const CString & cCmp) const {
		return m_20Code == cCmp;
	}
};

struct ERR_INFO{
	CString m_AddrCode;
	CString m_Source;
	CString m_Purpose;
};

typedef std::set<GOU_INFO, std::less<GOU_INFO> > GOU_INFO_SET;
typedef std::map<long, CString> PURPOSE_MAP;
typedef std::multimap<CString, CCSAddrPoint> CS_MAP;
typedef std::multimap<CString, long> CS_CODE_MAP;
typedef std::set<long> ATTR_SET;
class CCheck  
{
public:
	bool execCheck(LPCTSTR cListFile);
	bool init(IFeatureClassPtr ipAdmClass, IFeatureClassPtr ipGouClass, IFeatureClassPtr ipCSClass, IFeatureClassPtr ipCityClass, IFeatureClassPtr ipCityDividerClass, IFeatureClassPtr ipBaseClass, ITablePtr ipRule, LPCTSTR cJdbConnect);
	CCheck();
	virtual ~CCheck();

private:
	bool fnIsInvalidBaseAttr(LONG cAttr);
	bool fnIsInvalidCityAttr(LONG cAttr);
	bool fnBaseSiteCheck(IGeometryPtr ipGeom, LONG cObjID);
	bool fnCitySiteCheck(IGeometryPtr ipGeom, LONG cObjID);
	bool fnCityDividerCheck(IGeometryPtr ipGeom, LONG cObjID);
	bool fnSameCodeCheck(LONG cObjID, const ERR_INFO* cpErrInfo);
	CString fnGetPurpose(CCSAddrPoint &cCsp);
	LONG m_PurposeIndex;
	LONG m_InfoIndex;
	CString m_LayerName;
	CString m_GouLayerName;
	bool fnPointCheck(CCSAddrPoint &cCsp);
	LONG m_BaseSiteIndex;
	LONG m_CitySiteIndex;
	LONG m_ModProgIndex;
	LONG m_TwentyIndex;
	LONG m_FugoIndex;
	LONG m_AddrIndex;
	LONG m_CityIndex;
	LONG m_AreaIndex;
	IFeatureClassPtr m_ipBase;
	IFeatureClassPtr m_ipCity;
	IFeatureClassPtr m_ipCityDivider;
	CFeatureClass m_FcCsAddr;
	CFeatureClass m_FcGou;
	CFeatureClass m_FcAdmin;
	CGouPoints *m_pGous;
	CSPFieldMap m_AdmFld;
	CSPFieldMap m_GouFld;
	CSPFieldMap m_CspFld;
	CSPTableNameString m_AdmName;
	CSPTableNameString m_GouName;
	CSPTableNameString m_CspName;
	bool fnLoadList(LPCTSTR cListFile);
	CODE_SET m_CodeSet;
	GOU_INFO_SET m_GouSet;
	bool fnGetGouInfo(LPCTSTR cCityCode, bool cIsList);
	bool fnCheckAt8Code(LPCTSTR cAddrCode, bool cNeedLoad);
	CAreaCodeHelper	m_Helper;
	bool fnSearch8Code(void);
	bool fnCheckPriorGP(GOU_INFO_SET &cGouSet);
	PURPOSE_MAP m_PurposeMap;
	CS_MAP m_CsMap;
	CS_CODE_MAP m_CsMap20;
	ATTR_SET m_CityAttr;
	ATTR_SET m_BaseAttr;
	bool fnInitRule(ITablePtr ipRule);
	bool fnCheckPriorGPAll(void);
	void fnErrOut(LPCTSTR cLayer1, long cObjID1, LPCTSTR cLayer2, long cObjID2, int cLevel, LPCTSTR cErrMsg, const ERR_INFO* cpErrInfo);
};

#endif // !defined(AFX_CHECK_H__BB21CF49_D53F_4154_AC3F_0010D560ADCE__INCLUDED_)
