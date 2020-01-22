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

// AdmColor.h: AdmColor クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMCOLOR_H__BB8E24CA_B122_4050_A736_B31AF28A7F04__INCLUDED_)
#define AFX_ADMCOLOR_H__BB8E24CA_B122_4050_A736_B31AF28A7F04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct COLOR_INFO {
	CString m_CityCode;
	CString m_AddrCode;
	LONG m_Color;
	bool operator<(const COLOR_INFO &cCmp) const{
		if(m_CityCode != cCmp.m_CityCode){
			return m_CityCode < cCmp.m_CityCode;
		}else{
			return m_AddrCode < cCmp.m_AddrCode;
		}
	}
};

typedef std::set<COLOR_INFO> COLOR_TABLE;

class AdmColor  
{
public:
	bool addColor(LONG cMeshCode);
	bool init(LPCTSTR cBaseDir, LPCTSTR cAdmLayer, LPCTSTR cTableFile,bool cChgMode);
	AdmColor();
	virtual ~AdmColor();

protected:
	LONG m_MeshCode;
	bool fnAddColor(IFeaturePtr ipFeature);
	IFeatureClassPtr fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace);
	IWorkspacePtr fnOpenLocalDatabase(LONG cMeshCode);
	bool fnLoadColorTable(LPCTSTR cTableFile);
	LONG m_UpdateIndex;
	LONG m_AddrIndex;
	LONG m_CityIndex;
	LONG m_ColorIndex;
	COLOR_TABLE m_ColorTable;
	bool m_ChgMode;
	CString m_AdmLayer;
	CString m_BaseDir;
};

#endif // !defined(AFX_ADMCOLOR_H__BB8E24CA_B122_4050_A736_B31AF28A7F04__INCLUDED_)
