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

#define PGDB_TABLE_NAME "MainTable"
const char *gConnectStr = "Provider=Microsoft.JET.OLEDB.4.0;Data source";

PGDB_FIELD GentyDB::m_Fields[] = {
//	PGDB_FIELD("OBJECTID", esriFieldTypeOID, 8),
	PGDB_FIELD("MESH", adInteger, 0), //8
	PGDB_FIELD("LONGITUDE", adDouble, 0), //10
	PGDB_FIELD("LATITUDE", adDouble, 0), //10
	PGDB_FIELD("ID_IN_MESH", adInteger, 0), //4
	PGDB_FIELD("ATTR_CODE", adInteger, 0),//8
	PGDB_FIELD("ATTR_NAME", adVarWChar, 32),
	PGDB_FIELD("ORG_NAME", adVarWChar, 128),
	PGDB_FIELD("PRE_NAME", adVarWChar, 128),
	PGDB_FIELD("AFT_NAME", adVarWChar, 128),
	PGDB_FIELD("POS_F", adBoolean, 0), //1
	PGDB_FIELD("NAME_F", adBoolean, 0), //1
	PGDB_FIELD("CLOSE_F", adBoolean, 0), //1
	PGDB_FIELD("CLEAR_F", adBoolean, 0), //1
	PGDB_FIELD("CONST_F", adBoolean, 0), //1
	PGDB_FIELD("UNCERTAIN_F", adBoolean, 0),//1
	PGDB_FIELD("SHOOT_F", adBoolean, 0),//1		2007/4/23追加
	PGDB_FIELD("LAYER", adVarWChar, 32),
	PGDB_FIELD("OID", adInteger, 0),//8
	PGDB_FIELD("ADDRCODE", adVarWChar, 12),
	PGDB_FIELD("X", adInteger, 0),//8
	PGDB_FIELD("Y", adInteger, 0),//8
	PGDB_FIELD("ORG_LONGITUDE", adDouble, 0),//10
	PGDB_FIELD("ORG_LATITUDE", adDouble, 0),//10
	PGDB_FIELD("RESERCH_DATE", adDate, 0),//10
	PGDB_FIELD("MEMO_", adLongVarWChar, 512),//1		2007/4/23追加
	PGDB_FIELD(NULL, adInteger, 0)
};

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

GentyDB::GentyDB() /*:
m_ipRecord(__uuidof(ADODB::Recordset))*/
{
	m_pFile = NULL;
}

GentyDB::~GentyDB()
{
	if(m_ipRecord != NULL){
		m_ipRecord->Close();
	}
	if(m_ipConnection != NULL){
		m_ipConnection->Close();
	}
	if(m_pFile != NULL){
		fclose(m_pFile);
	}
}

bool GentyDB::init(LPCTSTR cPgdbDir, LPCTSTR cOutFile, LPCTSTR cImageDir, bool cTextModeFlag, bool cNewRecordOnly)
{
	m_PgdbDir = cPgdbDir;
	m_ImageDir = cImageDir;
	m_TextModeFlag = cTextModeFlag;
	m_NewRecOnly = cNewRecordOnly;
	if(m_TextModeFlag){
		if((m_pFile = fopen(cOutFile, "wt")) == NULL){
			fprintf(stderr, "出力リスト作成に失敗しました。%s\n", cOutFile);
			return false;
		}
	}else{
		_CatalogPtr ipCatalog = fnCreateLocalDatabase(cOutFile);
		if(ipCatalog == NULL){
			return false;
		}
		_TablePtr ipLocalTbl = fnCreateLocalTable(ipCatalog);
		if(ipLocalTbl == NULL){
			fprintf(stderr, "PGDB作成に失敗しました。%s\n", cOutFile);
			return false;
		}
		try
		{
			m_ipConnection = ipCatalog->GetActiveConnection();
			m_ipRecord.CreateInstance(__uuidof(ADODB::Recordset));
			m_ipRecord->Open(PGDB_TABLE_NAME, _variant_t((IDispatch *) m_ipConnection, true), ADODB::adOpenKeyset, ADODB::adLockOptimistic, ADODB::adCmdTable);
	    }
		catch(_com_error &e)
		{
			// Notify the user of errors if any.
			_bstr_t bstrSource(e.Source());
			_bstr_t bstrDescription(e.Description());

			fprintf(stderr, "#Error, Source :  %s \n\tdescription : %s \n ",
				(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		}
	}
	return true;
}

bool GentyDB::execCatGentyDB(LONG cMeshCode)
{
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
		for(ipRecord->MoveFirst(); !ipRecord->EndOfFile; ipRecord->MoveNext()){
			fnAppendRecord(ipRecord);
		}
		ipRecord->Close();
		ipConnection->Close();
    }
	catch(_com_error &e)
	{
		// Notify the user of errors if any.
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());

		fprintf(stderr, "#Error, Source :  %s \n\tdescription : %s \n ",
			(LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		return false;
	}
	return true;
}

_CatalogPtr GentyDB::fnCreateLocalDatabase(LPCTSTR cOutFile)
{
//	HRESULT hr = S_OK;

	// Define ADOX object pointers.
	// Initialize pointers on define.
	// These are in the ADOX::  namespace.

	_CatalogPtr ipCatalog(__uuidof (Catalog));
	CString aDatabase;
	aDatabase.Format("%s = %s", gConnectStr, cOutFile); 
	//Set ActiveConnection of Catalog to this string
	try {
		ipCatalog->Create(_bstr_t(aDatabase));
	}
    catch(_com_error &e)
    {
        // Notify the user of errors if any.
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
        printf("\n\tSource :  %s \n\tdescription : %s \n ",
            (LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		fprintf(stderr, "#Error AccessDBが作成できない。:%s\n", cOutFile);
		ipCatalog= NULL;
    }

//		fprintf(stderr, "#%d,Error AccessDBが作成できない。:%s%d.mdb\n", m_MeshCode, strDir, m_MeshCode);
//		return NULL;
//	}
	return ipCatalog;

}

_TablePtr GentyDB::fnCreateLocalTable(_CatalogPtr ipCatalog)
{
	// Define ADOX object pointers.
	// Initialize pointers on define.
	// These are in the ADOX::  namespace.
	_TablePtr ipTable(__uuidof(ADOX::Table));
    try
    {
		ipTable->PutName(PGDB_TABLE_NAME);
		for(int i = 0; m_Fields[i].m_Name != NULL; i++){
			ipTable->Columns->Append(m_Fields[i].m_Name ,m_Fields[i].m_Type, m_Fields[i].m_Length);
		}
		_ColumnPtr ipColumn;
		ipColumn = ipTable->Columns->GetItem(CComVariant(m_Fields[23].m_Name));
		if(ipColumn != NULL){
			ipColumn->Attributes = adColNullable;
		}
		ipCatalog->Tables->Append(_variant_t((IDispatch *)ipTable));
	}
    catch(_com_error &e)
    {
        // Notify the user of errors if any.
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
        printf("\n\tSource :  %s \n\tdescription : %s \n ",
            (LPCSTR)bstrSource,(LPCSTR)bstrDescription);
		ipTable= NULL;
    }
	return ipTable;
}

bool GentyDB::fnAppendRecord(ADODB::_RecordsetPtr ipRecord)
{
	if(m_NewRecOnly && fnNoChange(ipRecord)){
		//最初に変更の有無をチェック
		return false;
	}
	if(m_TextModeFlag){
		for(int i = 0; m_Fields[i].m_Name != NULL; i++){
			switch(m_Fields[i].m_Type){
				case adInteger:
					fprintf(m_pFile, "%d,", ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value.lVal);
					break;
				case adDouble:
					fprintf(m_pFile, "%.16f,", ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value.dblVal);
					break;
				case adVarWChar:
				case adLongVarWChar:
					fprintf(m_pFile, "%s,", static_cast<LPCTSTR>(CString(ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value.bstrVal)));
					break;
				case adBoolean:
					if(ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value.boolVal){
						fprintf(m_pFile, "1,");
					}else{
						fprintf(m_pFile, "0,");
					}
					break;
				case adDate:
					{
						CComVariant aTmp = ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value;
						if(aTmp.vt == VT_NULL){
							fprintf(m_pFile, ",");
						}else{
							aTmp.ChangeType(VT_BSTR);
							fprintf(m_pFile, "%s,", static_cast<LPCTSTR>(CString(aTmp.bstrVal)));
						}
					}
					break;
				default:
					break;
			}
		}
		fseek(m_pFile, -1, SEEK_CUR);	//カンマが余分に付くので最後のを消す
		fprintf(m_pFile, "\n");
	}else{
		m_ipRecord->AddNew();
		for(int i = 0; m_Fields[i].m_Name != NULL; i++){
			m_ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value = ipRecord->Fields->GetItem(m_Fields[i].m_Name)->Value;
		}
		m_ipRecord->Update();
	}
	return true;
}
bool GentyDB::fnNoChange(ADODB::_RecordsetPtr ipRecord)
{
	CString aAftName = ipRecord->Fields->GetItem(_T("AFT_NAME"))->Value.bstrVal;
	if(aAftName != ""){
		return false;
	}
	if(ipRecord->Fields->GetItem(_T("UNCERTAIN_F"))->Value.lVal != 0
		|| ipRecord->Fields->GetItem(_T("NAME_F"))->Value.lVal != 0 
		|| ipRecord->Fields->GetItem(_T("CLOSE_F"))->Value.lVal != 0
		|| ipRecord->Fields->GetItem(_T("CLEAR_F"))->Value.lVal != 0
		|| ipRecord->Fields->GetItem(_T("CONST_F"))->Value.lVal != 0
		|| ipRecord->Fields->GetItem(_T("POS_F"))->Value.lVal != 0
		|| ipRecord->Fields->GetItem(_T("SHOOT_F"))->Value.lVal != 0){
		return false;
	}
	CComVariant aDate = ipRecord->Fields->GetItem(_T("RESERCH_DATE"))->Value;
	if(aDate.vt != VT_NULL){
		return false;
	}
	CString aMemo = ipRecord->Fields->GetItem(_T("MEMO_"))->Value.bstrVal;
	if(aMemo != ""){
		return false;
	}
	if(ipRecord->Fields->GetItem(_T("LONGITUDE"))->Value.dblVal != ipRecord->Fields->GetItem(_T("ORG_LONGITUDE"))->Value.dblVal
		   || ipRecord->Fields->GetItem(_T("LATITUDE"))->Value.dblVal != ipRecord->Fields->GetItem(_T("ORG_LATITUDE"))->Value.dblVal){
		return false;
	}
	LONG aID = ipRecord->Fields->GetItem(_T("ID_IN_MESH"))->Value.lVal;
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
