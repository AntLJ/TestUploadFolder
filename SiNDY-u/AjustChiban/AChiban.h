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

// AChiban.h: AChiban クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACHIBAN_H__7A2C7379_70BF_4D5A_8508_18B577E60FB2__INCLUDED_)
#define AFX_ACHIBAN_H__7A2C7379_70BF_4D5A_8508_18B577E60FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
typedef std::set<LONG, std::less<LONG> > IDSET;

class AChiban  
{
public:
	bool unInit(void);
	bool execAjustChiban(LPCTSTR cAddrCode);
	bool init(IFeatureClassPtr ipGyoseiClass, IFeatureClassPtr  ipChibanClass, bool cChgMode);
	AChiban();
	AChiban(IWorkspacePtr ipWorkspace);
	virtual ~AChiban();

private:
	bool fnAjust(IFeaturePtr ipSrcFeature);
	bool fnCheckChiban(IFeaturePtr ipFeature);
	IDSET m_IDSet;
	CString m_TargetCode;
	LONG m_AreafIndex;
	LONG m_ModifyDateIndex;
	LONG m_ProgNameIndex;
	LONG m_ElevenIndex;
	LONG m_CClassIndex;
	LONG m_CTypeIndex;
	IFeatureClassPtr m_ipChiban;
	IFeatureClassPtr m_ipGyosei;
	IWorkspacePtr m_ipWorkspace;
	bool m_ChgMode;
	CString m_LayerName;
};

#endif // !defined(AFX_ACHIBAN_H__7A2C7379_70BF_4D5A_8508_18B577E60FB2__INCLUDED_)
