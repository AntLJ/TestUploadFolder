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

#pragma once

#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include "schema.h"
#include "crd_cnv/coord_converter.h"

extern crd_cnv g_cnv;

class CImportData
{
public:
	typedef std::map< CString, std::map<long, CComVariant> > RECORDSMAP;
	typedef std::map<CString, WKSPoint> POINTSMAP;

	CImportData(void);
	~CImportData(void);
	CImportData( LPCTSTR lpcszFilepath, LPCTSTR SeetName, IFields* ipFields ){ ReadExcelFile( lpcszFilepath, SeetName, ipFields); }

	void ReadExcelFile( LPCTSTR lpcszFilepath, LPCTSTR SeetName, IFields* ipFields );
	RECORDSMAP& GetReCords(){ return m_mapRecords; }
	POINTSMAP& GetPoints( const CString& strID ){ return m_mapGeoms[strID]; }
	std::list<CString>& GetIDList(){ return m_lstIDs; }

//	long ToTargetFieldIndex( long lIndex ){ return m_mapToTargetFieldIndex[ lIndex ]; }
//	CString GetFieldName( int nIndex ){ return m_vecField[nIndex]; }

private:
	double DMS2DEC( CString& strDMS );

private:
	std::ifstream m_ifs;

	crd_cnv m_cnv;
	RECORDSMAP m_mapRecords;	// レコード < key:管理番号 val:< key:SDEでのフィールドインデックス val:値 > >
	std::map< CString, std::map<CString, WKSPoint> > m_mapGeoms;			// 形状のマップ < key:管理番号 val:< key:フィールド名 val:座標 > >
	std::list<CString> m_lstIDs;										// ID（管理番号）のリスト
};
