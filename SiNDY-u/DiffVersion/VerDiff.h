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

// VerDiff.h: VerDiff クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VERDIFF_H__804A5EA1_DF72_4D1B_90DE_B70A776884CA__INCLUDED_)
#define AFX_VERDIFF_H__804A5EA1_DF72_4D1B_90DE_B70A776884CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct TypeTbl {
	LPCTSTR m_Msg;
	esriDifferenceType m_Type;
};

typedef std::set<CString, std::less<CString> > CODE_SET;
 
class VerDiff  
{
public:
	bool execCSDiff(LPCTSTR cLayer);
	bool execAddrDiff();
	bool resultOut(LPCTSTR cOutFile);
	bool execDiff(LPCTSTR cLayer);
	bool init(IWorkspacePtr ipWorkspace, IWorkspacePtr ipAdrWorkspace, LPCTSTR cIndexLayer, LPCTSTR cOldVersion, LPCTSTR cOldAdmVersion, int cMode);
	VerDiff();
	virtual ~VerDiff();

protected:
	bool fnSearchAddr(LONG cObjID, IFeatureClassPtr ipFeatureClass, IFeatureClassPtr ipAddrFeatureClass);
	bool fnSearchMesh(LONG cObjID, IFeatureClassPtr ipFeatureClass);
	bool fnSearchMeshEx(LONG cObjID, IFeatureClassPtr ipFeatureClass);
	bool fnSearchMesh(IFeaturePtr ipFeature);
	bool fnCheckDiff(int cIndex, bool cKlinkTgt);
	CODE_SET m_CodeSet;
	LONG m_MeshIndex;
	LONG m_CityCodeIndex;
	LONG m_AddrCodeIndex;
	int m_Mode;
	static TypeTbl VerDiff::m_Table[];
	CString m_OldVersion;
	IFeatureClassPtr m_ipBuildFeatureClass;
	IFeatureClassPtr m_ipMeshFeatureClass;
	IFeatureClassPtr m_ipOldFeatureClass;
	IFeatureClassPtr m_ipNewFeatureClass;
	IFeatureClassPtr m_ipNewAdrFeatureClass;
	IFeatureClassPtr m_ipOldAdrFeatureClass;
	IWorkspacePtr m_ipWorkspace;
//	IWorkspacePtr m_ipAdrWorkspace;
};

#endif // !defined(AFX_VERDIFF_H__804A5EA1_DF72_4D1B_90DE_B70A776884CA__INCLUDED_)
