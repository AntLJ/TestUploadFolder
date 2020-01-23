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

#include "StdAfx.h"
#include "ImportData.h"
#include "globalfunc.h"
#include "atlfile.h"
#include "WinLib/ADOBase.h"

#include <boost/regex.hpp>

CImportData::CImportData(void){};
CImportData::~CImportData(void){};

void CImportData::ReadExcelFile( LPCTSTR lpcszFilepath, LPCTSTR SeetName, IFields* ipFields )
{
	using namespace gf;

	CADOBase cDB( lpcszFilepath );
	CStringA strQuery;
	//strQuery.Format( "SELECT * FROM %s", CStringA(SeetName) ); 
	strQuery.Format( "SELECT * FROM [%s$]", CStringA(SeetName) ); 
	CADOCursor cur( cDB.search(strQuery) );
	const std::vector<CADOCursor::Field>& fields = cur.fields();

	// エイリアス->SDEのインデックス
	std::map< CString, long > mapFields;
	foreach( const CADOCursor::Field& field, fields)
	{
		long lIndex = -1;
		CComBSTR strName = ( field.name == L"症状詳細" ) ? L"詳細情報" : CComBSTR(field.name);
		ipFields->FindFieldByAliasName( strName, &lIndex );
		if( lIndex > -1 )
			mapFields[field.name] = lIndex;
	}

	// Excelにいないやつを補足
	long lTDC_F=-1, lWorkF=-1;
	ipFields->FindFieldByAliasName( L"TDC担当フラグ", &lTDC_F );
	mapFields[_T("TDC担当フラグ")] = lTDC_F;
	//ipFields->FindFieldByAliasName( L"制作工程フラグ", &lWorkF );
	//mapFields[_T("制作工程フラグ")] = lWorkF;

	for( CADOCursor::iterator it = cur.begin(); it != cur.end(); ++it )
	{
		CString strID;
		std::map< long, CComVariant > mapVal;
		std::map<CString, WKSPoint> mapGeom;
		for( int i=0; i < fields.size(); ++i )
		{
			if( _T("不具合発生工程") == fields[i].name )
			{
				CString strVal = Var2Str((*it)[i]);
				if( strVal == _T("データ整備（TDC）") )
					mapVal[ mapFields[_T( "TDC担当フラグ" )] ] = 1L;
				//else if( strVal == _T("データ整備") )
				//	mapVal[ mapFields[_T( "制作工程フラグ" ) ]] = 1L;
				continue;
			}

			double dDec = -1;
			if( fields[i].name ==_T("管理番号") ){
				strID = (*it)[i];
			}
			else if( fields[i].name == _T("不具合発生地点_緯度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("不具合発生地点")].Y = dDec;
				}else{
					mapVal[ mapFields[fields[i].name] ] = 0.0;
					continue;
				}
			}else if( fields[i].name == _T("不具合発生地点_経度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("不具合発生地点")].X = dDec;
				}else{
					mapVal[ mapFields[fields[i].name] ] = 0.0;
					continue;
				}
			}else if( fields[i].name == _T("出発地_緯度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("出発地")].Y = dDec;
				}
			}else if( fields[i].name == _T("出発地_経度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("出発地")].X = dDec;
				}
			}else if( fields[i].name == _T("目的地_緯度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("目的地")].Y = dDec;
				}
			}else if( fields[i].name == _T("目的地_経度") ){
				dDec = DMS2DEC( Var2Str((*it)[i]) );
				if( dDec > -1 ){
					mapGeom[_T("目的地")].X = dDec;
				}
			}

			// doubleものはdoubleで、その他のものはそのまま（テキスト）で突っ込む
			mapVal[ mapFields[fields[i].name] ] = (dDec > -1) ? dDec : (*it)[i];
#ifdef _DEBUG
if(strID == _T("10-396-1-00005") && fields[i].name == _T("No1:原因の説明"))
{
	printf(CStringA((*it)[i]));
}
#endif
		}
		m_lstIDs.push_back(strID);
		m_mapRecords[strID] = mapVal;
		m_mapGeoms[strID] = mapGeom;
	}
}

double CImportData::DMS2DEC( CString& strDMS )
{
	if( strDMS.IsEmpty() )
		return -1;

	int pos = 0;  

	dms _dms;
	_dms.deg = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.min = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.sec = _ttol( strDMS.Tokenize( _T("."), pos ) );
	_dms.sec += _ttol( strDMS.Tokenize( _T("."), pos ) ) / 10.0;

	return m_cnv.DMStoDEC( _dms );
}
