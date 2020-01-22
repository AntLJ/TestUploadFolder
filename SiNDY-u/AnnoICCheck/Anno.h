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

// Anno.h: Anno クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANNO_H__EEFC5A3A_0C2B_4AE8_A00C_C9D687E733CC__INCLUDED_)
#define AFX_ANNO_H__EEFC5A3A_0C2B_4AE8_A00C_C9D687E733CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "AnnoConv.h"

typedef std::list<long> ID_LIST;
typedef std::map<long, ID_LIST> ID_MAP;
typedef std::set<long> ID_SET;

struct KIND_TABLE {
	LONG m_Road;
	LONG m_Anno;
	LONG m_Rule[9];
	bool m_IsFree;
	bool m_RoadReady;
	KIND_TABLE(){
	}
	KIND_TABLE(const LONG cRoad, const LONG cAnno, const LONG *cRule, const bool cIsFree = false, const bool cReady = true){
		m_Road = cRoad;
		m_Anno = cAnno;
		m_IsFree = cIsFree;
		m_RoadReady = cReady;
		if(cRule != NULL){
			for(int i = 0; i < 9; i++){
				m_Rule[i] = cRule[i];
			}
		}
	}
};

class Anno  
{
public:
	bool setCityArea(LPCTSTR cMeshClass, LPCTSTR cSiteClass);
	bool execCheck();
	bool init(FILE *pFile, LPCTSTR cAnnoClass, LPCTSTR cRoadClass, double cPower, int cMode);
	Anno();
	Anno(IWorkspacePtr ipMapWorkspace, IWorkspacePtr ipAnnoWorkspace, IWorkspacePtr ipRodWorkSpace);
	virtual ~Anno();

protected:
	LONG m_SiteIndex;
	LONG m_CompIndex;
	bool fnIsTarget(IFeaturePtr ipFeature);
	IFeatureClassPtr m_ipSite;
	IFeatureClassPtr m_ipMesh;
	FILE *m_pFile;
	bool fnPointComp2(IFeaturePtr ipFeature, const LONG cRoadKind, FILE *cpFile);
	bool fnMakeSQL(const LONG cRoadKind, CString &cWhere);
	bool fnPointCheck2(const LONG cRoadKind, FILE *cpFile);
	bool fnIsTargetKind(LONG cKind, const LONG cRoadKind);
	static KIND_TABLE m_TargetKind[];
	static LPCSTR m_RoadKind[];
	bool fnPointComp(IFeaturePtr ipFeature, const LONG cRoadKind, FILE *cpFile);
	bool fnPointCheck(const LONG cRoadKind, FILE *cpFile);
	LONG m_AnnoAttrIndex;
	LONG m_KanjiAttrIndex;
	LONG m_RoadAttrIndex;
	LONG m_DummyIndex;
	LONG m_RoadCodeIndex;
	LONG m_AnnoName1Index;
	LONG m_AnnoDispTypeIndex[4];
	LONG m_RuleOffset;
	LONG m_NeedRank[8];
	CString m_AnnoLayerName;
	CString m_RoadLayerName;
	double m_BufDist;
	int m_Mode;
	IFeatureClassPtr m_ipRoad;
	IFeatureClassPtr m_ipAnno;
	IWorkspacePtr m_ipMapWorkspace;
	IWorkspacePtr m_ipAnnoWorkspace;
	IWorkspacePtr m_ipRodWorkspace;
	AnnoConv m_AnnoConv;
	int fnGetTableIndex(long cAnnoAttr, long cRoadAttr);
	bool fnDispScaleCheck(IFeaturePtr ipFeature, int cTableIndex, long cMesh, FILE *cpFile);
	bool fnSetNeedRank(void);
	ID_MAP m_ID_Map;
	ID_SET m_OK_ID;
	bool fnCompSamePoint(IFeaturePtr ipFeature, LPCTSTR cRoadName, FILE *cpFile);

};

#endif // !defined(AFX_ANNO_H__EEFC5A3A_0C2B_4AE8_A00C_C9D687E733CC__INCLUDED_)
