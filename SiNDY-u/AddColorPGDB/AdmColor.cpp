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

// AdmColor.cpp: AdmColor クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdmColor.h"

namespace {
LPCSTR CITY_CODE_NAME = "CITYCODE";
LPCSTR ADDR_CODE_NAME = "ADDRCODE";
LPCSTR COLOR_CODE_NAME = "COLORCODE";
LPCSTR AREACLASS_NAME = "AREACLASS_C";
LPCSTR UPDATE_NAME = "UPDATE_C";

}
//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

AdmColor::AdmColor()
{

}

AdmColor::~AdmColor()
{

}

bool AdmColor::init(LPCTSTR cBaseDir, LPCTSTR cAdmLayer, LPCTSTR cTableFile, bool cChgMode)
{
	m_BaseDir = cBaseDir;
	m_AdmLayer = cAdmLayer;
	m_ChgMode = cChgMode;
	return fnLoadColorTable(cTableFile);
}

bool AdmColor::addColor(LONG cMeshCode)
{
	m_MeshCode = cMeshCode;
	IWorkspacePtr ipWorkspace = fnOpenLocalDatabase(cMeshCode);
	if(ipWorkspace == NULL){
		return false;
	}
	IFeatureClassPtr ipAdm = fnOpenLocalFeatureClass(ipWorkspace);
	if(ipAdm == NULL){
		return false;
	}
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	ipFilter->put_WhereClause(CComBSTR("AREACLASS_C = 1"));
	IFeatureCursorPtr ipCursor;
	ipAdm->Search(ipFilter, VARIANT_FALSE, &ipCursor);
	if(ipCursor == NULL){
		return false;
	}
	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		fnAddColor(ipFeature);
	}
	return true;
}

bool AdmColor::fnLoadColorTable(LPCTSTR cTableFile)
{
	FILE *pFile;
	bool aResult = true;
	if((pFile = fopen(cTableFile, "rt")) != NULL){
		char aBuf[64];
		while(fgets(aBuf, 64, pFile) != NULL){
			COLOR_INFO aColor;
			char *p = strchr(aBuf, ',');
			if(p == NULL){
				printf("Error,テーブルファイルのフォーマットが違う\n");
				fprintf(stderr, "Error,テーブルファイルのフォーマットが違う\n");
				aResult = false;
				break;
			}
			*p = '\0';
			aColor.m_CityCode = aBuf;
			p++;
			char *p2 = strchr(p, ',');
			if(p2 == NULL){
				printf("Error,テーブルファイルのフォーマットが違う\n");
				fprintf(stderr, "Error,テーブルファイルのフォーマットが違う\n");
				aResult = false;
				break;
			}
			*p2 = '\0';
			aColor.m_AddrCode = p;
			p2++;
			aColor.m_Color = atol(p2);
			m_ColorTable.insert(aColor);
		}
		fclose(pFile);
	}else{
		printf("Error,テーブルファイルが開けません。,%s\n", cTableFile);
		fprintf(stderr, "Error,テーブルファイルが開けません。,%s\n", cTableFile);
		aResult = false;
	}
	return aResult;
}

IWorkspacePtr AdmColor::fnOpenLocalDatabase(LONG cMeshCode)
{
	CString strDatabase;
	strDatabase.Format("%s\\%d\\%d.mdb", m_BaseDir, cMeshCode/10000, cMeshCode);

	// パーソナルジオDB
	IWorkspaceFactoryPtr ipWorkspaceFactory( CLSID_AccessWorkspaceFactory );
	IWorkspacePtr ipWorkspace;

	// オープンしてみる
	ipWorkspaceFactory->OpenFromFile( CComBSTR(strDatabase), 0, &ipWorkspace );
	if( ipWorkspace == NULL )
	{
		fprintf( stderr, "#Error,%s が開けません。\n", static_cast<LPCSTR>(strDatabase));
	}
	return ipWorkspace;
}

IFeatureClassPtr AdmColor::fnOpenLocalFeatureClass(IWorkspacePtr ipWorkspace)
{
	IFeatureClassPtr ipAdm;
	((IFeatureWorkspacePtr)ipWorkspace)->OpenFeatureClass(CComBSTR(m_AdmLayer), &ipAdm);

	if(ipAdm->FindField(CComBSTR(_T(CITY_CODE_NAME)), &m_CityIndex) == S_OK){
		if(m_CityIndex < 0){
			fprintf(stderr, "都道府県コードフィールドが見つかりません。,[%s]\n", CITY_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "都道府県コードフィールドが見つかりません。,[%s]\n", CITY_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(ADDR_CODE_NAME)), &m_AddrIndex) == S_OK){
		if(m_AddrIndex < 0){
			fprintf(stderr, "住所コードフィールドが見つかりません。,[%s]\n", ADDR_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "住所コードフィールドが見つかりません。,[%s]\n", ADDR_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(COLOR_CODE_NAME)), &m_ColorIndex) == S_OK){
		if(m_ColorIndex < 0){
			fprintf(stderr, "カラーコードフィールドが見つかりません。,[%s]\n", COLOR_CODE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "カラーコードフィールドが見つかりません。,[%s]\n", COLOR_CODE_NAME);
		return NULL;
	}
	if(ipAdm->FindField(CComBSTR(_T(UPDATE_NAME)), &m_UpdateIndex) == S_OK){
		if(m_UpdateIndex < 0){
			fprintf(stderr, "更新フラグフィールドが見つかりません。,[%s]\n", UPDATE_NAME);
			return NULL;
		}
	}else{
		fprintf(stderr, "更新フラグフィールドが見つかりません。,[%s]\n", UPDATE_NAME);
		return NULL;
	}

	return ipAdm;
}

bool AdmColor::fnAddColor(IFeaturePtr ipFeature)
{
	CComVariant aCityCode, aAddrCode;

	ipFeature->get_Value(m_CityIndex, &aCityCode);
	ipFeature->get_Value(m_AddrIndex, &aAddrCode);
	COLOR_INFO aCmp;
	aCmp.m_CityCode = aCityCode.bstrVal;
	aCmp.m_AddrCode = aAddrCode.bstrVal;
	COLOR_TABLE::iterator aColor = m_ColorTable.find(aCmp);
	if(aColor != m_ColorTable.end()){
		LONG aObjID;
		ipFeature->get_OID(&aObjID);
		if(aColor->m_Color > 0 && aColor->m_Color < 7){
			CComVariant aColorCode;
			ipFeature->get_Value(m_ColorIndex, &aColorCode);
			if(aColorCode.lVal != aColor->m_Color){
				if(m_ChgMode){
					ipFeature->put_Value(m_ColorIndex, CComVariant(aColor->m_Color));
					CComVariant aUpdate;
					ipFeature->get_Value(m_UpdateIndex, &aUpdate);
					aUpdate.lVal |= 2;	//属性変更
					ipFeature->put_Value(m_UpdateIndex, aUpdate);
					ipFeature->Store();
					fprintf(stderr, "色変更。,%d,%d,%s,%s,%d->%d\n", m_MeshCode, aObjID, static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode), aColorCode.lVal, aColor->m_Color);
				}else{
					fprintf(stderr, "色変更が必要。,%d,%d,%s,%s,%d->%d\n", m_MeshCode, aObjID, static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode), aColorCode.lVal, aColor->m_Color);
				}
			}else{
				fprintf(stderr, "変更の必要がない。,%d,%d\n", m_MeshCode, aObjID);
			}
		}else if(aColor->m_Color == 0){
			fprintf(stderr, "変更対象外。,%d,%d\n", m_MeshCode, aObjID);
		}else{
			fprintf(stderr, "色が正しくありません。,%d,%d\n", m_MeshCode, aObjID);
		}
	}else{
		fprintf(stderr, "Error,対応が見つからない,%s,%s\n", static_cast<LPCSTR>(aCmp.m_CityCode), static_cast<LPCSTR>(aCmp.m_AddrCode));
		return false;
	}
	return true;
}
