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

// GentyDB.cpp: GentyDB クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GentyDB.h"

namespace {
LPCSTR PGDB_TABLE_NAME = "MainTable";
LPCSTR gConnectStr = "Provider=Microsoft.JET.OLEDB.4.0;Data source";


LPCSTR LONG_FIELD = "LONGITUDE";
LPCSTR LAT_FIELD = "LATITUDE";
LPCSTR ID_FIELD = "ID_IN_MESH";
LPCSTR PRE_NAME_FIELD = "PRE_NAME";
//LPCSTR AFT_NAME_FIELD = "AFT_NAME";
LPCSTR POS_F_FIELD = "POS_F";
LPCSTR NAME_F_FIELD = "NAME_F";
LPCSTR CLOSE_F_FIELD = "CLOSE_F";
//LPCSTR CLEAR_F_FIELD = "CLEAR_F";
//LPCSTR CONST_F_FIELD = "CONST_F";
LPCSTR UNCRE_F_FIELD = "UNCERTAIN_F";	//2009/05/25 撮影必須ポイント用として使用
LPCSTR SHOOT_F_FIELD = "SHOOT_F";		//2007/4/25追加
LPCSTR ORG_LONG_FIELD = "ORG_LONGITUDE";
LPCSTR ORG_LAT_FIELD = "ORG_LATITUDE";
LPCSTR DATE_FIELD = "RESERCH_DATE";
LPCSTR MEMO_FIELD = "MEMO_";		//2007/4/25追加
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

GentyDB::GentyDB()
{

}

GentyDB::~GentyDB()
{

}

bool GentyDB::init(LPCTSTR cPgdbDir, LPCTSTR cImageDir, bool cNewRecordOnly, bool cPhotoF)
{
	m_PgdbDir = cPgdbDir;
	m_ImageDir = cImageDir;
	m_NewRecOnly = cNewRecordOnly;
	m_PhotoF = cPhotoF;
	if(_access(m_PgdbDir, 4) != 0){
		fprintf(stderr, "指定したAccessDBのパスにアクセスできません。,%s\n", static_cast<LPCTSTR>(m_PgdbDir));
		return false;
	}
	if(_access(m_ImageDir, 4) != 0){
		fprintf(stderr, "指定したラスタデータのパスにアクセスできません。,%s\n", static_cast<LPCTSTR>(m_ImageDir));
		return false;
	}
	SYSTEMTIME aNowTime;
	::GetSystemTime(&aNowTime);
	double aValTime;
	::SystemTimeToVariantTime(&aNowTime, &aValTime);
	m_NowDate = aValTime;
	m_NowDate.ChangeType(VT_DATE);
	m_LimitDate = aValTime - 90;
	m_LimitDate.ChangeType(VT_DATE);
	return true;
}

bool GentyDB::execCheckGentyDB(LONG cMeshCode)
{
	m_NewCount = 0;
	m_MeshCode = cMeshCode;
	printf("Now %d\n", m_MeshCode);
	CString aDB;
	aDB.Format("%s = %s\\%d\\%d.mdb", gConnectStr, m_PgdbDir, m_MeshCode/10000, m_MeshCode);
	try
	{
		ADODB::_ConnectionPtr ipConnection(__uuidof(ADODB::Connection));
		ipConnection->Open(_bstr_t(aDB), "", "", ADODB::adConnectUnspecified);
		ADODB::_RecordsetPtr ipRecord(__uuidof(ADODB::Recordset)); 
		ipRecord->Open(PGDB_TABLE_NAME, _variant_t((IDispatch *) ipConnection, true), ADODB::adOpenKeyset, ADODB::adLockReadOnly, ADODB::adCmdTable);
		if(!ipRecord->EndOfFile){
			for(ipRecord->MoveFirst(); !ipRecord->EndOfFile; ipRecord->MoveNext()){
				fnCheckRecord(ipRecord);
			}
		}
		ipRecord->Close();
		ipConnection->Close();
    }
	catch(_com_error &e)
	{
		// Notify the user of errors if any.
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());

		fprintf(stderr, "#Error, %d, Source :  %s\tdescription : %s\n",
			m_MeshCode, (LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		return false;
	}
	fprintf(stderr, "#%d,新規追加分,%d\n", m_MeshCode, m_NewCount);
	return true;
}

bool GentyDB::fnCheckRecord(ADODB::_RecordsetPtr ipRecord)
{
	LONG aID = ipRecord->Fields->GetItem(ID_FIELD)->Value.lVal;

	//後で似たような処理するけど、最初に変更の有無をチェック
	if(fnNoChange(ipRecord)){
		if(!m_NewRecOnly && aID <= 900){
			fprintf(stderr, "%d,%03d,調査結果が入力されていない\n", m_MeshCode, aID);
		}
		return true;
	}
	if(aID > 900){
		m_NewCount++;
	}
	CComVariant aDate = ipRecord->Fields->GetItem(DATE_FIELD)->Value;
	if(aDate.vt == VT_NULL){
		fprintf(stderr, "%d,%03d,調査日付が入力されていない\n", m_MeshCode, aID);
	}else{
		if(aDate > m_NowDate || aDate < m_LimitDate){
			fprintf(stderr, "%d,%03d,調査日付が範囲外\n", m_MeshCode, aID);
		}
	}
	if(ipRecord->Fields->GetItem(POS_F_FIELD)->Value.lVal == 0){
		if(ipRecord->Fields->GetItem(LONG_FIELD)->Value.dblVal == ipRecord->Fields->GetItem(ORG_LONG_FIELD)->Value.dblVal
			&& ipRecord->Fields->GetItem(LAT_FIELD)->Value.dblVal == ipRecord->Fields->GetItem(ORG_LAT_FIELD)->Value.dblVal){
			fprintf(stderr, "%d,%03d,座標が移動されてないのに位置OKフラグが設定されていない\n", m_MeshCode, aID);
		}
	}else{
		double aLong = ipRecord->Fields->GetItem(LONG_FIELD)->Value.dblVal;
		double aOrgLong = ipRecord->Fields->GetItem(ORG_LONG_FIELD)->Value.dblVal;
		double aLat = ipRecord->Fields->GetItem(LAT_FIELD)->Value.dblVal;
		double aOrgLat = ipRecord->Fields->GetItem(ORG_LAT_FIELD)->Value.dblVal;
		if(aLong != aOrgLong || aLat != aOrgLat){
			if(float(aLong) != float(aOrgLong) || float(aLat) != float(aOrgLat)){
				 fprintf(stderr, "%d,%03d,位置OKフラグが設定されているのに座標が移動されている\n", m_MeshCode, aID);
			}else{
				 fprintf(stderr, "%d,%03d,(Warning)位置OKフラグが設定されているのに座標が微妙に移動されている\n", m_MeshCode, aID);
			}
		}
	}
	//写真の有無チェック
	bool aHavePhoto = true;
	CString aImagePath;
	aImagePath.Format("%s\\%d\\%d\\%03d.jpg", m_ImageDir, m_MeshCode/10000, m_MeshCode, aID);
	if(_access(aImagePath, 4) != 0){
		aImagePath.Format("%s\\%d\\%d\\%03d-1.jpg", m_ImageDir, m_MeshCode/10000, m_MeshCode, aID);
		if(_access(aImagePath, 4) != 0){
			aHavePhoto = false;
		}
	}
	//写真の有無チェック ここまで


	if(ipRecord->Fields->GetItem(SHOOT_F_FIELD)->Value.lVal){
		CString aMemo = ipRecord->Fields->GetItem(MEMO_FIELD)->Value.bstrVal;
		if(aMemo == ""){
			fprintf(stderr, "%d,%03d,撮影不可フラグがONなのに備考が入力されていない\n", m_MeshCode, aID);
		}
		if(aHavePhoto){
			fprintf(stderr, "%d,%03d,撮影不可フラグがONなのに写真が存在する\n", m_MeshCode, aID);
		}
	}else{
		if(!aHavePhoto){
			if(!m_PhotoF || ipRecord->Fields->GetItem(NAME_F_FIELD)->Value.lVal == 0 || ipRecord->Fields->GetItem(POS_F_FIELD)->Value.lVal == 0 || ipRecord->Fields->GetItem(UNCRE_F_FIELD)->Value.lVal != 0){
				//「全件撮影必須モード」か「位置OKじゃない」か「名称OKじゃない」か「撮影必須ポイント」
				fprintf(stderr, "%d,%03d,写真が１つもない\n", m_MeshCode, aID);
			}
		}
	}
	if(ipRecord->Fields->GetItem(NAME_F_FIELD)->Value.lVal 
		&& ipRecord->Fields->GetItem(CLOSE_F_FIELD)->Value.lVal){
		fprintf(stderr, "%d,%03d,フラグのつけ方が不正\n", m_MeshCode, aID);
	}
	return true;
}
bool GentyDB::fnNoChange(ADODB::_RecordsetPtr ipRecord)
{
	if(ipRecord->Fields->GetItem(NAME_F_FIELD)->Value.lVal != 0 
		|| ipRecord->Fields->GetItem(CLOSE_F_FIELD)->Value.lVal != 0
		|| ipRecord->Fields->GetItem(POS_F_FIELD)->Value.lVal != 0
		|| ipRecord->Fields->GetItem(SHOOT_F_FIELD)->Value.lVal != 0){
		return false;
	}
	CComVariant aDate = ipRecord->Fields->GetItem(DATE_FIELD)->Value;
	if(aDate.vt != VT_NULL){
		return false;
	}
	CString aMemo = ipRecord->Fields->GetItem(MEMO_FIELD)->Value.bstrVal;
	if(aMemo != ""){
		return false;
	}
	if(ipRecord->Fields->GetItem(LONG_FIELD)->Value.dblVal != ipRecord->Fields->GetItem(ORG_LONG_FIELD)->Value.dblVal
		   || ipRecord->Fields->GetItem(LAT_FIELD)->Value.dblVal != ipRecord->Fields->GetItem(ORG_LAT_FIELD)->Value.dblVal){
		return false;
	}
	LONG aID = ipRecord->Fields->GetItem(ID_FIELD)->Value.lVal;
	CString aImagePath;
	aImagePath.Format("%s\\%d\\%d\\%03d.jpg", m_ImageDir, m_MeshCode/10000, m_MeshCode, aID);
	if(_access(aImagePath, 4) == 0){
		return false;
	}
	aImagePath.Format("%s\\%d\\%d\\%03d-1.jpg", m_ImageDir, m_MeshCode/10000, m_MeshCode, aID);
	if(_access(aImagePath, 4) == 0){
		return false;
	}
	return true;
}
