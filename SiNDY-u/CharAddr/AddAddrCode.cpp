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

// AddAddrCode.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"

#include "stdafx.h"
#include <sindy/schema/road.h>
#include <sindy/schema/map.h>
#include <sindy/workspace.h>
#include <libschema.h>

#include "AddAddrCode.h"

CString g_strNo1 = _T("0123456789-#");
CString g_strNo2 = _T("０１２３４５６７８９");
CString g_strNo3 = _T("ⅠⅡⅢⅣⅤⅥⅦⅧⅨⅩ");
CString g_strHiragana1 = _T("あいうえおかきくけこさしすせそたちつてとなにぬねのはひふへほまみむめもやゆよらりるれろわをんぁぃぅぇぉゃゅょー");
CString g_strHiragana2 = _T("がぎぐげござじずぜぞだぢづでどばびぶべぼぱぴぷぺぽ");
CString g_strKatakana1 = _T("アイウエオカキクケコサシスセソタチツテトナニヌネノハヒフヘホマミムメモヤユヨラリルレロワヲンァィゥェォャュョ");
CString g_strKatakana2 = _T("ガギグゲゴザジズゼゾダヂヅデドバビブベボパピプペポ");
CString g_strMark = _T("―‐ 　");
CString g_strABC1 = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
CString g_strABC2 = _T("abcdefghijklmnopqrstuvwxyz");
CString g_strABC3 = _T("ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ");
CString g_strABC4 = _T("ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ");
CString g_strABC = g_strABC1 + g_strABC2 + g_strABC3 + g_strABC4;
CString g_strABCandNo = g_strABC + g_strNo1 + g_strNo2 + g_strNo3 + g_strMark;
CString g_strKana = g_strHiragana1 + g_strHiragana2 + g_strKatakana1 + g_strKatakana2;

ODatabase eOraDB;			///< 接続DB

void CheckLog( const CString& strLogPath )
{
	IFeatureWorkspacePtr ipAdminWorkspace(INamePtr(sindy::create_workspace_name(_T("sindy2008b@topaz2(SDE.Current08B)")))->_Open());
	IFeatureClassPtr ipGouPointClass(ipAdminWorkspace->_OpenFeatureClass(_T("sindy2008b.GOU_POINT")));

	IFeatureClassPtr ipCityAdminClass(ipAdminWorkspace->_OpenFeatureClass(_T("sindy2008b.CITY_ADMIN")));

	int nAddrCodeIndex = ipGouPointClass->_FindField(sindy::schema::gou_point::kAddrCode);
	int nCityCodeIndex = ipCityAdminClass->_FindField(sindy::schema::city_admin::kCityCode);
	int nCityAddrCodeIndex = ipCityAdminClass->_FindField(sindy::schema::city_admin::kAddrCode);

	ISpatialFilterPtr ipSFilter( CLSID_SpatialFilter );
	CComBSTR bstrShapeFieldName;
	ipCityAdminClass->get_ShapeFieldName( &bstrShapeFieldName );
	ipSFilter->put_GeometryField( bstrShapeFieldName );
	ipSFilter->put_SpatialRel( esriSpatialRelIntersects );

	IQueryFilterPtr ipQFilter( CLSID_QueryFilter );
	ipQFilter->put_SubFields(CComBSTR(_T("")));

	std::set<CString> setChar;
	std::map<CString,std::set<long>> mapCharAndIDs, mapCharAndIDss;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	long lRow = 0;
	std::ifstream iFile(strLogPath);
	while( !iFile.eof() )
	{
		lRow++;
		char aLine[256] = {};
		iFile.getline(aLine,256);
		CString strLine = aLine;
		int iTab = strLine.Find('\t');
		if( iTab >= 0 )
		{
			long OID = _ttol( strLine.Left( iTab ) );
			if( OID > 0 )
			{
				strLine = strLine.Right( strLine.GetLength() - iTab - 1 );
				while(1)
				{
					int iPos = StrCSpn( strLine, g_strABCandNo );
					int iPos2 = 0;
					if( iPos == 0 )
					{
						int iPos2 = StrSpn( strLine, g_strABCandNo );
						strLine = strLine.Right( strLine.GetLength() - iPos2 );
						iPos = StrCSpn( strLine, g_strABCandNo );
					}
					iPos2 = StrSpn( strLine, g_strABCandNo );
					CString strSet = strLine.Left( iPos );
					int nCheck1 = StrCSpn( strSet, g_strKana );
					int nCheck2 = StrSpn( strSet, g_strKana );
					int nLen = strSet.GetLength();
					if( nCheck2 != 0 && nLen > nCheck2 )
						mapCharAndIDs[strSet].insert( OID );
					else if( nCheck1 > 0 && nLen > 0 )
						mapCharAndIDs[strSet].insert( OID );

					strLine = strLine.Right( strLine.GetLength() - iPos );
					iPos = StrSpn( strLine, g_strABCandNo );
					if( iPos == 0 || iPos == strLine.GetLength() )
						break;
					else
						strLine = strLine.Right( strLine.GetLength() - iPos );
				}
			}
			else
			{
				std::cerr << lRow << _T("行目\tOIDエラー") << std::endl;
				continue;
			}
		}
		else
		{
			std::cerr << lRow << _T("行目\tタブなし") << std::endl;
			continue;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::ifstream iFiles(_T("C:\\admin.log"));
	lRow = 0;
	while( !iFiles.eof() )
	{
		lRow++;
		char aLine[256] = {};
		iFiles.getline(aLine,256);
		CString strLine = aLine;
		int iTab = strLine.Find('\t');
		if( iTab >= 0 )
		{
			long OID = _ttol( strLine.Left( iTab ) );
			if( OID > 0 )
			{
				strLine = strLine.Right( strLine.GetLength() - iTab - 1 );
				while(1)
				{
					int iPos = StrCSpn( strLine, g_strABCandNo );
					int iPos2 = 0;
					if( iPos == 0 )
					{
						int iPos2 = StrSpn( strLine, g_strABCandNo );
						strLine = strLine.Right( strLine.GetLength() - iPos2 );
						iPos = StrCSpn( strLine, g_strABCandNo );
					}
					iPos2 = StrSpn( strLine, g_strABCandNo );
					CString strSet = strLine.Left( iPos );
					int nCheck1 = StrCSpn( strSet, g_strKana );
					int nCheck2 = StrSpn( strSet, g_strKana );
					int nLen = strSet.GetLength();
					if( nCheck2 != 0 && nLen > nCheck2 )
						mapCharAndIDss[strSet].insert( OID );
					else if( nCheck1 > 0 && nLen > 0 )
						mapCharAndIDss[strSet].insert( OID );

					strLine = strLine.Right( strLine.GetLength() - iPos );
					iPos = StrSpn( strLine, g_strABCandNo );
					if( iPos == 0 || iPos == strLine.GetLength() )
						break;
					else
						strLine = strLine.Right( strLine.GetLength() - iPos );
				}
			}
			else
			{
				std::cerr << lRow << _T("行目\tOIDエラー") << std::endl;
				continue;
			}
		}
		else
		{
			std::cerr << lRow << _T("行目\tタブなし") << std::endl;
			continue;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::map<CString,std::map<CString,std::set<long>>> mapCharAddrOIDs;
	std::map<CString,std::map<CString,std::set<long>>> mapCharAddrOIDss;
	OStartup();
	CString strConnect;
	if( Connect( _T("k0703/k0703@delltcp") ) )
	{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if( nAddrCodeIndex < 0 )
		{
			std::cerr << _T("sindy2008b@topaz2(SDE.Current08B)->sindy2008b.GOU_POINT：") << sindy::schema::gou_point::kAddrCode << _T(" がありません") << std::endl;
			return;
		}
		if( nCityCodeIndex < 0 )
		{
			std::cerr << _T("ronly@topaz2(SDE.DEFAULT)->ronly.CITYMESH：") << sindy::schema::citymesh::kCityMeshCode << _T(" がありません") << std::endl;
			return;
		}
		for( std::map<CString,std::set<long>>::const_iterator it = mapCharAndIDs.begin(); it != mapCharAndIDs.end(); ++it )
		{
			CString strOID, strWhere;
			for( std::set<long>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				strOID.Format( _T("%d"), *it2 );
				strWhere += strOID + _T(",");
			}
			strWhere = _T("OBJECTID IN (") + strWhere.Left( strWhere.GetLength() - 1 ) + _T(")");
			ipQFilter->put_WhereClause(CComBSTR(strWhere));
			IFeatureCursorPtr ipFCursor;
			ipGouPointClass->Search( ipQFilter, VARIANT_TRUE, &ipFCursor );
			std::map<CString,std::set<long>> mapAddr;
			if( ipFCursor )
			{
				IFeaturePtr ipFeature;
				ipFCursor->Flush();
				while( ipFCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
				{
					_IRowPtr ipRow( ipFeature );
					if( ipRow )
					{
						long lOID = 0;
						ipRow->get_OID( &lOID );

						IGeometryPtr ipGeom;
						ipFeature->get_ShapeCopy( &ipGeom );
						ipSFilter->putref_Geometry( ipGeom );

						IFeatureCursorPtr ipCityAdminCursor;
						ipCityAdminClass->Search( ipSFilter, VARIANT_TRUE, &ipCityAdminCursor );
						if( ipCityAdminCursor )
						{
							IFeaturePtr ipCityAdmin;
							ipCityAdminCursor->Flush();
							while( ipCityAdminCursor->NextFeature( &ipCityAdmin ) == S_OK && ipCityAdmin )
							{
								CComVariant vaCityCode, vaCityAddrCode;
								IRowBufferPtr(ipCityAdmin)->get_Value( nCityCodeIndex, &vaCityCode );
								IRowBufferPtr(ipCityAdmin)->get_Value( nCityAddrCodeIndex, &vaCityAddrCode );
								CString strCode11 = CString(vaCityCode.bstrVal) + CString(vaCityAddrCode.bstrVal);

								char cTemp[100] = {};
								CodeToKanji( strCode11, cTemp );
								CString strAddrName;
								strAddrName.Format(_T("%s"), cTemp);

								if( strAddrName.IsEmpty() )
								{
									CodeToKanji( CString(vaCityCode.bstrVal), cTemp );
									strAddrName.Format(_T("%s"), cTemp);
									if( strAddrName.IsEmpty())
										strAddrName = strCode11;
								}

								mapAddr[strAddrName].insert( lOID );
							}
						}
					}
				}
				mapCharAddrOIDs[it->first] = mapAddr;
			}
		}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for( std::map<CString,std::set<long>>::const_iterator it = mapCharAndIDss.begin(); it != mapCharAndIDss.end(); ++it )
		{
			CString strOID, strWhere;
			for( std::set<long>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			{
				strOID.Format( _T("%d"), *it2 );
				strWhere += strOID + _T(",");
			}
			strWhere = _T("OBJECTID IN (") + strWhere.Left( strWhere.GetLength() - 1 ) + _T(")");
			ipQFilter->put_WhereClause(CComBSTR(strWhere));
			IFeatureCursorPtr ipCityAdminCursor;
			ipCityAdminClass->Search( ipQFilter, VARIANT_TRUE, &ipCityAdminCursor );
			std::map<CString,std::set<long>> mapAddrs;
			if( ipCityAdminCursor )
			{
				IFeaturePtr ipCityAdmin;
				ipCityAdminCursor->Flush();
				while( ipCityAdminCursor->NextFeature( &ipCityAdmin ) == S_OK && ipCityAdmin )
				{
					_IRowPtr ipRow( ipCityAdmin );
					if( ipRow )
					{
						long lOID = 0;
						ipRow->get_OID( &lOID );

						CComVariant vaCityCode, vaCityAddrCode;
						IRowBufferPtr(ipCityAdmin)->get_Value( nCityCodeIndex, &vaCityCode );
						IRowBufferPtr(ipCityAdmin)->get_Value( nCityAddrCodeIndex, &vaCityAddrCode );
						CString strCode11 = CString(vaCityCode.bstrVal) + CString(vaCityAddrCode.bstrVal);

						char cTemp[100] = {};
						CodeToKanji( strCode11, cTemp );
						CString strAddrName;
						strAddrName.Format(_T("%s"), cTemp);

						if( strAddrName.IsEmpty() )
						{
							CodeToKanji( CString(vaCityCode.bstrVal), cTemp );
							strAddrName.Format(_T("%s"), cTemp);
							if( strAddrName.IsEmpty())
								strAddrName = strCode11;
						}
						mapAddrs[strAddrName].insert( lOID );
					}
				}
				for( std::map<CString,std::set<long>>::iterator it3 = mapAddrs.begin(); it3 != mapAddrs.end(); ++it3 )
				{
					if( mapCharAddrOIDs[it->first].find( it3->first) != mapCharAddrOIDs[it->first].end() )
						mapCharAddrOIDs[it->first].insert( *it3 );
				}
				if( mapCharAddrOIDs.find( it->first ) != mapCharAddrOIDs.end() )
				{
					for( std::map<CString,std::set<long>>::iterator it3 = mapAddrs.begin(); it3 != mapAddrs.end(); ++it3 )
					{
						if( mapCharAddrOIDs[it->first].find(it3->first) != mapCharAddrOIDs[it->first].end() )
						{
							for( std::set<long>::iterator it4 = it3->second.begin(); it4 != it3->second.end(); ++it4 )
								mapCharAddrOIDs[it->first][it3->first].insert( *it4 );
						}
						else
							mapCharAddrOIDs[it->first][it3->first] = it3->second;
					}
				}
				else
					mapCharAddrOIDs[it->first] = mapAddrs;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::ofstream oFile(_T("C:\\asca.txt"));
	for( std::map<CString,std::map<CString,std::set<long>>>::const_iterator it = mapCharAddrOIDs.begin(); it != mapCharAddrOIDs.end(); ++it )
	{
		oFile << it->first << _T("\t");
		for( std::map<CString,std::set<long>>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2 )
			oFile << it2->first << _T(",") << it2->second.size() << _T("個|");
		oFile << std::endl;
	}
}

int main(int argc, char **argv)
{
	::CoInitialize(0);
	{
		if( argc > 1 )
		{
			CString strLogPath = argv[1];
			CheckLog( strLogPath );
		}
	}
	::CoUninitialize();

	return 0;
}

// =================================================================================================================
// =================================================================================================================
bool Connect( const CString& strConnect )
{
	char aUser[100], aPass[100], aService[100];

	if( !strConnect.IsEmpty())
	{
		if( _tcslen(strConnect) >= CON_STR_SIZE )
		{
			std::cerr << _T("[JDBAccess] 接続文字列が許容サイズを超えています") << std::endl;
			return false;
		}
	}

	if( !fnAnalyzeConStr( strConnect, aUser, aPass, aService ) )
	{
		std::cerr << _T("[JDBAccess] 接続文字列が不正です:") << strConnect << std::endl;
		return false;
	}

	if( eOraDB.Open(aService, aUser, aPass) == OFAILURE )
	{
		std::cerr << _T("[JDBAccess] 住所DBに接続できません: ") << strConnect << std::endl;
		return false;
	}
	return true;
}

bool fnAnalyzeConStr( const CString& strConnect, char *cUser, char *cPass, char *cService)
{

	char *aToken, aConTmp[CON_STR_SIZE];
	if( strConnect.IsEmpty() ) return false;
	_tcscpy(aConTmp, CT2A(strConnect));

	aToken = strtok(aConTmp, "/@");
	if(aToken == NULL) return false;
	_tcscpy(cUser, aToken);
	aToken = strtok(NULL, "/@");
	if(aToken == NULL) return false;
	_tcscpy(cPass, aToken);
	aToken = strtok(NULL, "/@");
	if(aToken == NULL) return false;
	_tcscpy(cService, aToken);

	return true;
}

bool CodeToKanji( const char *cCode, char *cKanji )
{

	char aSQL[1024];
	char aKenCode[3], aShiCode[4], aOazaCode[4], aAzaCode[4];

	if( !PartitionCode(cCode, aKenCode, aShiCode, aOazaCode, aAzaCode ) ) return false;

	sprintf(aSQL,
		"select replace(ken_kanji || shi_kanji || oaza_kanji || aza_kanji, '　') from areacode_master "
		"where ken_code = '%s' and shi_code = '%s' and oaza_code = '%s' and aza_code = '%s'",
		aKenCode, aShiCode, aOazaCode, aAzaCode);

	ODynaset aDSQuery;
	if(aDSQuery.Open(eOraDB, aSQL) == OFAILURE)
	{
		std::cerr << _T("[JDBAccess] クエリに失敗しました") << std::endl;
		return false;
	}

	if( aDSQuery.IsEOF() )
		_tcscpy(cKanji, "");
	else
		aDSQuery.GetFieldValue(0, cKanji, ALL_KANJI_MAX_LEN);

	return true;
}

bool PartitionCode(const char *cCode, char *cKen, char *cShi, char *cOaza, char *cAza, char *cChiban, char *cBango, char *cKatagaki)
{
	if(cKen			!= NULL) _tcscpy(cKen,		"00");
	if(cShi			!= NULL) _tcscpy(cShi,		"000");
	if(cOaza		!= NULL) _tcscpy(cOaza,		"000");
	if(cAza			!= NULL) _tcscpy(cAza,		"000");
	if(cChiban		!= NULL) _tcscpy(cChiban,	"00000");
	if(cBango		!= NULL) _tcscpy(cBango,	"0000");
	if(cKatagaki	!= NULL) _tcscpy(cKatagaki,	"00");

	unsigned int aLen = _tcslen(cCode);

	switch(aLen) {
	case 22:
		if(cKatagaki	!= NULL) strncpy(cKatagaki,	&cCode[20], 2);
	case 20:
		if(cBango		!= NULL) strncpy(cBango,	&cCode[16], 4);
	case 16:
		if(cChiban		!= NULL) strncpy(cChiban,	&cCode[11], 5);
	case 11:
		if(cAza			!= NULL) strncpy(cAza,		&cCode[8], 3);
	case 8:
		if(cOaza		!= NULL) strncpy(cOaza,		&cCode[5], 3);
	case 5:
		if(cShi			!= NULL) strncpy(cShi,		&cCode[2], 3);
	case 2:
		if(cKen			!= NULL) strncpy(cKen,		&cCode[0], 2);
		break;
	default:
		std::cerr << _T("[JDBAccess] 無効な住所コードです:") << cCode << std::endl;
		return false;
	}
	return true;
}
