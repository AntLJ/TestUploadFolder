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

#if !defined(AFX_CHGCODE_H__A3F94A53_C105_40D9_9F51_AF5B39B58F87__INCLUDED_)
#define AFX_CHGCODE_H__A3F94A53_C105_40D9_9F51_AF5B39B58F87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//typedef CString ADRS;
typedef std::map<CString, CString, std::less<CString> > ADRSMAP;

class ChgCode  
{
public:
	bool execChgRepAddrCode(bool cChangeMode);
	bool init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerName);
	ChgCode();
	virtual ~ChgCode();
protected:
	bool fnChangeCode(const CString &cSrc, const CString &cTgt, bool cChgMode);
	ADRSMAP m_AdrsMap;
	LONG m_AddrCodeIndex;
	LONG m_ProgNameIndex;
	LONG m_ModifyDateIndex;
	IFeatureClassPtr m_ipAdmin;
	CString m_LayerName;
};

#endif // !defined(AFX_CHGCODE_H__A3F94A53_C105_40D9_9F51_AF5B39B58F87__INCLUDED_)
