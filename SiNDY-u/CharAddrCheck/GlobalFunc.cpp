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


#include "stdafx.h"
#include "GlobalFunc.h"

std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, long lType )
{
	std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapRet;

	long lFieldNum = ipFeatureClass->_FindField( _bstr_t(lpcszField) );
	CString strWhere;
	strWhere.Format( _T("%s %s"), lpcszField, lpcszWhere );
	IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
	ipQFilter->put_WhereClause(CComBSTR(strWhere));
	IFeatureCursorPtr ipFeatureCursor;
	if( S_OK == ipFeatureClass->Search( ipQFilter, VARIANT_FALSE, &ipFeatureCursor ) && ipFeatureCursor )
	{
		IFeaturePtr ipFeature;
		while( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) && ipFeature )
		{
			CComVariant vaValue = ipFeature->GetValue( lFieldNum );
			CString strValue = vaValue.bstrVal;
			if( lType == 0 )	// 号ポイント
			{
				if( _tcscmp( strValue.Left(1), _T("#") ) == 0 )
					strValue.Delete( 0,1 );
			}
			strValue.Replace(_T("　"), _T(" "));
			strValue.Remove(_T(' '));
			if( !strValue.IsEmpty() )
				mapRet.insert( std::pair<long,std::pair<CString,CAdapt<IFeaturePtr>>>( ipFeature->GetOID(), std::pair<CString,CAdapt<IFeaturePtr>>( strValue, ipFeature ) ) );
		}
	}
	return mapRet;
}

std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> GetItems2( LPCTSTR lpcszWhere, LPCTSTR lpcszField, IFeatureClass* ipFeatureClass, LPCTSTR lpcszListPath, long lType )
{
	std::map<long,std::pair<CString,CAdapt<IFeaturePtr>>> mapRet;

	std::ifstream ifs(static_cast<CT2W>(lpcszListPath));
	if( !ifs.fail() )
	{
		CString strWhere;
		while( !ifs.eof() )
		{
			char szBuf[512] = {0};
			ifs.getline( szBuf, 512 );
			CString strBuf;
			strBuf.Format( _T("%s"), szBuf );
			if( strBuf.IsEmpty() )
				continue;
			int nPos = 0;
			CString strOKNG = strBuf.Tokenize( _T("\t"), nPos );
			CString strOID = strBuf.Tokenize( _T("\t"), nPos );
			CString strKanji = strBuf.Tokenize( _T("\t"), nPos );

			if( _tcscmp( strOKNG, _T("NG") ) == 0 && !strOID.IsEmpty() )
			{
				IQueryFilterPtr ipQFilter(CLSID_QueryFilter);
				strWhere = _T("OBJECTID IN (") + strOID + _T(")");
				ipQFilter->put_WhereClause( CComBSTR(strWhere) );
				IFeatureCursorPtr ipFeatureCursor;
				if( S_OK == ipFeatureClass->Search( ipQFilter, VARIANT_FALSE, &ipFeatureCursor ) && ipFeatureCursor )
				{
					IFeaturePtr ipFeature;
					while( S_OK == ipFeatureCursor->NextFeature( &ipFeature ) && ipFeature )
					{
						CComVariant vaValue = ipFeature->GetValue( ipFeatureClass->_FindField( _bstr_t(lpcszField) ) );
						CString strValue = vaValue.bstrVal;
						if( lType == 0 )	// 号ポイント
						{
							if( _tcscmp( strValue.Left(1), _T("#") ) == 0 )
								strValue.Delete( 0,1 );
						}
						strValue.Replace(_T("　"), _T(" "));
						strValue.Remove(_T(' '));
						mapRet.insert( std::pair<long,std::pair<CString,CAdapt<IFeaturePtr>>>( ipFeature->GetOID(), std::pair<CString,CAdapt<IFeaturePtr>>( strValue, ipFeature ) ) );
					}
				}
			}
		}
	} else {
		std::cout << "#ERROR\t" << CT2A(lpcszListPath) << "\tログファイル作成失敗\n";
	}
	return mapRet;
}


bool CodeToKanji( LPCTSTR lpcszAddrCode, char *lpKanji, const ODatabase& eOraDB )
{
	char aSQL[1024];
	TCHAR aKenCode[3], aShiCode[4], aOazaCode[4], aAzaCode[4];

	if( !PartitionCode( lpcszAddrCode, aKenCode, aShiCode, aOazaCode, aAzaCode ) )
		return false;

	sprintf( aSQL,
		"select replace(ken_kanji || shi_kanji || oaza_kanji || aza_kanji, '　') from areacode_master "
		"where ken_code = '%s' and shi_code = '%s' and oaza_code = '%s' and aza_code = '%s'",
		CT2CA(aKenCode), CT2CA(aShiCode), CT2CA(aOazaCode), CT2CA(aAzaCode) );

	ODynaset aDSQuery;
	if( aDSQuery.Open( eOraDB, aSQL ) == OFAILURE )
	{
		std::cerr << "[JDBAccess] クエリに失敗しました" << std::endl;
		return false;
	}

	if( aDSQuery.IsEOF() )
		strcpy( lpKanji, "" );	// char型なので_tcscpyは使わない（UNICODEの場合もあるので）
	else
		aDSQuery.GetFieldValue( 0, lpKanji, ALL_KANJI_MAX_LEN );

	return true;
}

bool PartitionCode( LPCTSTR lpcszCode, LPTSTR lpszKen, LPTSTR lpszShi, LPTSTR lpszOaza, LPTSTR lpszAza )
{
	if( lpszKen != NULL) _tcscpy( lpszKen, _T("00"));
	if( lpszShi != NULL) _tcscpy( lpszShi, _T("000"));
	if( lpszOaza != NULL) _tcscpy( lpszOaza, _T("000"));
	if( lpszAza != NULL) _tcscpy( lpszAza, _T("000"));

	unsigned int aLen = _tcslen(lpcszCode);

	switch(aLen)
	{
	case 11:
		if( lpszAza != NULL) _tcsncpy( lpszAza, &lpcszCode[8], 3 );
	case 8:
		if( lpszOaza != NULL) _tcsncpy( lpszOaza, &lpcszCode[5], 3 );
	case 5:
		if( lpszShi != NULL) _tcsncpy( lpszShi, &lpcszCode[2], 3 );
	case 2:
		if( lpszKen != NULL) _tcsncpy( lpszKen, &lpcszCode[0], 2 );
		break;
	default:
		std::cerr << "[JDBAccess] 無効な住所コードです:" << CT2CA(lpcszCode) << std::endl;
		return false;
	}
	return true;
}

CString CreateLogFile( LPCTSTR lpcszOutput, LPCTSTR lpcszFile, long lCount )
{
	CString strFilePath;
	strFilePath.Format( _T("%s\\%s_%03d.txt"), lpcszOutput, lpcszFile, lCount );
	lCount++;
	if( PathFileExists( strFilePath ) )
		strFilePath = CreateLogFile( lpcszOutput, lpcszFile, lCount );
	else
		strFilePath;
	return strFilePath;
}
