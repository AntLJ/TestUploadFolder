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

#if !defined(AFX_GENTYDB_H__E642518A_D4AC_489D_B316_0D7038A5EA8D__INCLUDED_)
#define AFX_GENTYDB_H__E642518A_D4AC_489D_B316_0D7038A5EA8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GentyDB  
{
public:
	bool execCheckGentyDB(LONG cMeshCode);
	bool init(LPCTSTR cPgdbDir, LPCTSTR cImageDir, bool cNewRecordOnly, bool cPhotoF);
	GentyDB();
	virtual ~GentyDB();

protected:
	bool fnNoChange(ADODB::_RecordsetPtr ipRecord);
	_variant_t m_LimitDate;
	_variant_t m_NowDate;
	bool fnCheckRecord(ADODB::_RecordsetPtr ipRecord);
	LONG m_MeshCode;
	bool m_NewRecOnly;
	CString m_ImageDir;
	CString m_PgdbDir;
	LONG m_NewCount;
	bool m_PhotoF;
};

#endif // !defined(AFX_GENTYDB_H__E642518A_D4AC_489D_B316_0D7038A5EA8D__INCLUDED_)
