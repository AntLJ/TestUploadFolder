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

#define BUF_MAX (256)

#define ADDR_CODE_NAME "AddrCode"
#define MODIFY_PROG_NAME "MODIFYPROGNAME"
#define MODIFY_DATE_NAME "PROGMODIFYDATE"
#define PROG_NAME "ChgCRepAddrCode"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

ChgCode::ChgCode()
{

}

ChgCode::~ChgCode()
{

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
					fprintf(stderr, "#対応コードが一緒。:%s\n", aSrcCode);
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
	m_ipCityRep = ipFeatureClass;
	m_LayerName = cLayerName;
	return true;
}

bool ChgCode::execChgCRepAddrCode(bool cChangeMode)
{
	ADRSMAP::iterator aAddr;

	for(aAddr = m_AdrsMap.begin(); aAddr != m_AdrsMap.end(); aAddr++){
		fnChangeCode(aAddr->first, aAddr->second, cChangeMode);
		if(_kbhit()){
			int aKey = getch();
			if(aKey == 'Q'){
				printf("処理は中止されました。\n");
				fprintf(stderr, "処理は中止されました。\n");
				break;
			}
		}
	}
	return true;
}

bool ChgCode::fnChangeCode(const CString &cSrc, const CString &cTgt, bool cChangeMode)
{
	IQueryFilterPtr ipFilter(CLSID_QueryFilter);
	CString aWhere;
	aWhere.Format("%s ='%s'", ADDR_CODE_NAME, cSrc);
	ipFilter->put_WhereClause(CComBSTR(aWhere));
//	CString aSubFeld;
//	aSubFeld.Format("OBJECTID,%s,%s,%s", ADDR_CODE_NAME, MODIFY_PROG_NAME, MODIFY_DATE_NAME);
//	ipFilter->put_SubFields(CComBSTR(aSubFeld));
	IFeatureCursorPtr ipCursor;

	if(cChangeMode){
		if(FAILED(m_ipCityRep->Update(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error 検索失敗。,%s\n", cSrc);
			return false;
		}
	}else{
		if(FAILED(m_ipCityRep->Search(ipFilter, VARIANT_FALSE, &ipCursor))){
			fprintf(stderr, "#Error 検索失敗。,%s\n", cSrc);
			return false;
		}
	}
	//嶋野さんのパクリ
	CString aStr = _T("");
	SYSTEMTIME stSystemTime;
	//嶋野さんのパクリ　ここまで

	IFeaturePtr ipFeature;
	while(ipCursor->NextFeature(&ipFeature) == S_OK){
		long aObjID;
		ipFeature->get_OID(&aObjID);
		printf("%s->%s\n", cSrc, cTgt);
		
		fprintf(stderr, "0\t%s\t%d\t\t\tInfo\t0\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
		if(!cChangeMode){
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_AddrCodeIndex, CComVariant(cTgt)))){
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\tコード変更失敗\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
			continue;
		}
		if(FAILED(ipFeature->put_Value(m_ProgNameIndex, CComVariant(PROG_NAME)))){
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\tプログラム名セット失敗\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
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
			fprintf(stderr, "0\t%s\t%d\t\t\tERROR\t0\t更新失敗\t%s->%s\n", m_LayerName, aObjID, cSrc, cTgt);
		}
	}
	ipCursor->Flush();
	return true;
}
