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

// ChgCode.h: ChgCode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHGCODE_H__C1C2FD94_76E9_49AE_8A78_DAA9822B995C__INCLUDED_)
#define AFX_CHGCODE_H__C1C2FD94_76E9_49AE_8A78_DAA9822B995C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef map<CString, CString, less<CString> > ADRSMAP;

class ChgCode  
{
public:
	bool execChgCRepAddrCode(bool cChangeMode);
	bool init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerName);
	ChgCode();
	virtual ~ChgCode();
protected:
	bool fnChangeCode(const CString &cSrc, const CString &cTgt, bool cChangeMode);
	ADRSMAP m_AdrsMap;
	LONG m_AddrCodeIndex;
	LONG m_ProgNameIndex;
	LONG m_ModifyDateIndex;
	IFeatureClassPtr m_ipCityRep;
	CString m_LayerName;

};

#endif // !defined(AFX_CHGCODE_H__C1C2FD94_76E9_49AE_8A78_DAA9822B995C__INCLUDED_)
