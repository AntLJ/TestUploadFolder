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

#if !defined(AFX_CHGCODE_H__045A3B03_1B5F_4AF5_B131_9F369499B03E__INCLUDED_)
#define AFX_CHGCODE_H__045A3B03_1B5F_4AF5_B131_9F369499B03E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef CString ADRS;
typedef std::map<CString, CString, std::less<CString> > ADRSMAP;
typedef std::set<CString, std::less<CString> > CITYLIST;

class ChgCode  
{
public:
	bool execChgAddrCode(bool cChangeMode);
	bool init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerNname);
	ChgCode();
	virtual ~ChgCode();
protected:
	bool fnChangeCode(const CString &cCityCode, bool cChangeMode);
	ADRSMAP m_AdrsMap;
	CITYLIST m_CityList;
	LONG m_CityCodeIndex;
	LONG m_AddrCodeIndex;
	LONG m_AddrCodeIndex2;	///< [bug 8793]対応
	LONG m_ProgNameIndex;
	LONG m_ModifyDateIndex;
#ifdef KAJO_CODE
	LONG m_KajoCodeIndex;
#endif
	IFeatureClassPtr m_ipAdmin;
	CString m_LayerName;
};

#endif // !defined(AFX_CHGCODE_H__045A3B03_1B5F_4AF5_B131_9F369499B03E__INCLUDED_)
