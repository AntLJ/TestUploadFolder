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

// ChgCode.cpp: ChgCode クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChgCode.h"

namespace {
const int BUF_MAX =256;

LPCSTR ADDR_CODE_NAME = "AddrCode";
LPCSTR MODIFY_PROG_NAME = "MODIFYPROGNAME";
LPCSTR MODIFY_DATE_NAME = "PROGMODIFYDATE";
LPCSTR PROG_NAME = "ChgRepAddrCode";
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ChgCode::ChgCode()
{

}

ChgCode::~ChgCode()
{
	m_AdrsMap.clear();
}

bool ChgCode::init(IFeatureClassPtr ipFeatureClass, LPCTSTR cFile, LPCTSTR cLayerName)
{
	FILE *pFile;
	char aBuf[BUF_MAX];
	CString aSrcCode, aTgtCode;
	char *pPointer;

	if((pFile = fopen(cFile, "rt")) != NULL){
		while(fgets(aBuf, BUF_MAX, pFile) != NULL){
			pPointer = strchr(aBuf, '\t');
			if(pPointer != NULL){
				*pPointer = '\0';
				aSrcCode = aBuf;
				pPointer++;
				aTgtCode = CString(pPointer).Left(11);
				if(aSrcCode != aTgtCode){
					m_AdrsMap.insert(ADRSMAP::value_type(aSrcCode, aTgtCode));
				}else{
					fprintf(stderr, "#対応コードが一緒。:%s\n", static_cast<LPCSTR>(aSrcCode));
				}
			}else{
				fprintf(stderr, "#対応リストのフォーマットが違う。:%s", aBuf);
			}
		}
		fclose(pFile);
	}else{
		fprintf(stderr, "#ファイルが開けない。:%s\n", cFile);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(ADDR_CODE_NAME)), &m_AddrCodeIndex) != S_OK){
		fprintf(stderr, "#住所コードフィールドが見つからない。[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	if(m_AddrCodeIndex < 0){
		fprintf(stderr, "#住所コードフィールドが見つからない。[%s]\n", ADDR_CODE_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_PROG_NAME)), &m_ProgNameIndex) != S_OK){
		fprintf(stderr, "#最終更新プログラム名フィールドが見つからない。[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(m_ProgNameIndex < 0){
		fprintf(stderr, "#最終更新プログラム名フィールドが見つからない。[%s]\n", MODIFY_PROG_NAME);
		return false;
	}
	if(ipFeatureClass->FindField(CComBSTR(_T(MODIFY_DATE_NAME)), &m_ModifyDateIndex) != S_OK){
		fprintf(stderr, "#最終更新日付フィールドが見つからない。[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	if(m_ModifyDateIndex < 0){
		fprintf(stderr, "#最終更新日付フィールドが見つからない。[%s]\n", MODIFY_DATE_NAME);
		return false;
	}
	m_ipAdmin = ipFeatureClass;
	m_LayerName = cLayerName;
	return true;
}

bool ChgCode::execChgRepAddrCode(bool cChangeMode)
{
	ADRSMAP::iterator aAddr;

	for(aAddr = m_AdrsMap.begin(); aAddr != m_AdrsMap.end(); aAddr++){
		fnChangeCode(aAddr->first, aAddr->second, cChangeMode);
	}
	return true;
}

bool ChgCode::fnChangeCode(const CString &cSrc, const CString &cTgt, bool cChangeMode)
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
//	aWhere.Format("%s = '%s'", ADDR_CODE_NAME, cSrc);
	aWhere.Format("%s LIKE '%s%%'", ADDR_CODE_NAME, cSrc);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
	CString aSubFeld;
	aSubFeld.Format("OBJECTID,%s,%s,%s", ADDR_CODE_NAME, MODIFY_PROG_NAME, MODIFY_DATE_NAME);
	ipFilter->put_SubFields(CComBSTR(aSubFeld));
	IFeatureCursorPtr ipCursor;

	if(cChangeMode){
		if(FAILED(m_ipAdmin->Update(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error 検索失敗。,%s\n", static_cast<LPCSTR>(cSrc));
			return false;
		}
	}else{
		if(FAILED(m_ipAdmin->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error 検索失敗。,%s\n", static_cast<LPCSTR>(cSrc));
			return false;
		}
	}
	//嶋野さんのパクリ
	CString aStr = _T("");
	SYSTEMTIME stSystemTime;
	//嶋野さんのパクリ　ここまで

	IFeaturePtr ipFeature;
	int aCount = 0;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		aCount++;
		long aObjID;
		ipFeature->get_OID(&aObjID);

		CComVariant aOrgCode;
		ipFeature->get_Value(m_AddrCodeIndex, &aOrgCode);
		CString aOrg = aOrgCode.bstrVal;
		CString aTgt = cTgt + aOrg.Mid(11);
//		fprintf(stderr, "0\t%s\t%d\t\t\tInfo\t0\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
		fprintf(stderr, "0\t%s\t%d\t\t\tInfo\t0\t%s->%s\n", static_cast<LPCSTR>(m_LayerName), aObjID, static_cast<LPCSTR>(aOrg), static_cast<LPCSTR>(aTgt));
		if(!cChangeMode){
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_AddrCodeIndex, CComVariant(aTgt)))){
			fprintf(stderr, "0\t%s\t%d\t\t\tError\t0\t住所コード変更失敗。\t%s\n", static_cast<LPCSTR>(m_LayerName), aObjID, static_cast<LPCSTR>(aOrg));
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_ProgNameIndex, CComVariant(PROG_NAME)))){
			fprintf(stderr, "プログラム名セット失敗。\n");
			continue;
		}
		//嶋野さんのパクリ（ちょっとでも正確な時間をとるためにここに移動）
		::GetLocalTime( &stSystemTime);
		aStr.Format( TEXT( "%04d/%02d/%02d %02d:%02d:%02d"),
				stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
				stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);
		//嶋野さんのパクリ　ここまで
		ipFeature->put_Value(m_ModifyDateIndex, CComVariant(aStr));
		if(FAILED(ipCursor->UpdateFeature(ipFeature))){
			fprintf(stderr, "#更新失敗\n");
		}
	}
	ipCursor->Flush();
	if(aCount > 1){
		fprintf(stderr, "#変更対象が複数存在する。:%s\n", static_cast<LPCSTR>(cSrc));
	}
	return true;
}
