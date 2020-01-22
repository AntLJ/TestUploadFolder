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

// CalcArea.h: CalcArea クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCAREA_H__E8CC6AD2_4193_45B0_9F68_DD20A419A41A__INCLUDED_)
#define AFX_CALCAREA_H__E8CC6AD2_4193_45B0_9F68_DD20A419A41A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct CityArea {
	long m_CityCode;
	double m_Area;
	const bool operator<(const CityArea &Cmp) const {
		return m_CityCode < Cmp.m_CityCode;
	}
};

typedef std::multiset<CityArea, std::less<CityArea> > AREA_MSET;

class CalcArea  
{
public:
	bool execCalcArea(long cAddrCode);
#ifdef __FOR_BT__
	bool init(IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipExtClass, LPCTSTR cOutDir, bool cAllFlag, bool cSpecialFlag);
#else
	bool init(IFeatureClassPtr ipFeatureClass, LPCTSTR cOutDir, bool cAllFlag, bool cSpecialFlag);
#endif
	CalcArea();
	virtual ~CalcArea();

protected:
	IGeometryPtr fnGetTgtGeom(IFeaturePtr ipFeature);
	bool fnPrecheck(long cAddrCode);
	CString m_OutDir;
	CString m_OutFile;
	bool fnResultOut(void);
	bool m_AllFlag;
	bool m_SpecialFlag;
	AREA_MSET m_AreaSet;
	double fnCalcArea(IGeometryPtr ipGeom);
	long m_AreaFlagIndex;
	long m_CityCodeIndex;
	IFeatureClassPtr m_ipFeatureClass;
#ifdef __FOR_BT__
	IFeatureClassPtr m_ipExtClass;
#endif
};

#endif // !defined(AFX_CALCAREA_H__E8CC6AD2_4193_45B0_9F68_DD20A419A41A__INCLUDED_)
