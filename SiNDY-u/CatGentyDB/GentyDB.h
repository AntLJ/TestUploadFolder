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

// GentyDB.h: GentyDB クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENTYDB_H__D2E128F1_C1D4_4DCE_A601_2850C089E4D6__INCLUDED_)
#define AFX_GENTYDB_H__D2E128F1_C1D4_4DCE_A601_2850C089E4D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace ADOX;

struct PGDB_FIELD {
	LPCTSTR m_Name;
	DataTypeEnum m_Type;
	LONG m_Length;
	PGDB_FIELD::PGDB_FIELD(LPCTSTR cName, DataTypeEnum cType, LONG cLength){
		m_Name = cName;
		m_Type = cType;
		m_Length = cLength;
	}
};

class GentyDB  
{
public:
	bool execCatGentyDB(LONG cMeshCode);
	bool init(LPCTSTR cPgdbDir, LPCTSTR cOutFile, LPCTSTR cImageDir, bool cTextModeFlag, bool cNewRecordOnly);
	GentyDB();
	virtual ~GentyDB();

protected:
	bool GentyDB::fnNoChange(ADODB::_RecordsetPtr ipRecord);
	bool fnAppendRecord(ADODB::_RecordsetPtr ipRecord);
	LONG m_MeshCode;
	static PGDB_FIELD m_Fields[];
	FILE *m_pFile;
	ADODB::_ConnectionPtr m_ipConnection;
	ADODB::_RecordsetPtr m_ipRecord; 
	_TablePtr fnCreateLocalTable(_CatalogPtr ipCatalog);
	_CatalogPtr fnCreateLocalDatabase(LPCTSTR cOutFile);
	bool m_NewRecOnly;
	bool m_TextModeFlag;
	CString m_ImageDir;
	CString m_PgdbDir;
};

#endif // !defined(AFX_GENTYDB_H__D2E128F1_C1D4_4DCE_A601_2850C089E4D6__INCLUDED_)
