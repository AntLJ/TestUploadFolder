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
#include "Windows.h"
#include "globalfunc.h"
#include <list>

namespace gf {

std::vector<CString> Tokenize2Vector( const CString & strSrc, LPCTSTR lpcszTorkens, LPCTSTR lpcszTrim )
{
	std::vector<CString> retVec;

	CString resToken;
	int nPos = 0;

	resToken= strSrc.Tokenize( lpcszTorkens, nPos );
	while ( !resToken.IsEmpty() )
	{
		if( lpcszTrim != NULL )
			resToken.Trim( lpcszTrim );

		retVec.push_back( resToken );
		resToken= strSrc.Tokenize( lpcszTorkens, nPos );
	};
	return retVec;
}

CString GetIniFileValue( LPCTSTR lpcszFilepath, LPCTSTR lpcszSection, LPCTSTR lpcszKey )
{
	CString strRet;
	if( lpcszFilepath != NULL )
	{
		//CString strPath;
		//GetCurrentDirectory(MAX_PATH , strPath.GetBuffer(MAX_PATH));
		//strPath.ReleaseBuffer();
		//strPath = strPath + _T("\\") + lpcszFilepath;

		if( PathFileExists(lpcszFilepath) )
		{
			CString strErr;
			strErr.Format( _T("[%s]%sキーがありません。"), lpcszSection, lpcszKey );
			TCHAR strText[1024];
			DWORD dw = GetPrivateProfileString( lpcszSection, lpcszKey, strErr, strText, 1024, lpcszFilepath );
			strRet = strText;
		}
	}
	return strRet;
}

int GetArraySize( tFieldSchema* tFields )
{ 
	short        id[] = {123, 156, 234, 310, 344, 350};

	int n = sizeof (tFields);
	n= sizeof (id);
	//n = sizeof g_CSDBFieldDef;
	//n = sizeof (g_NAVIFieldDef);
	n = sizeof (tFields[0]); 
	return sizeof (tFields) / sizeof (tFields[0]); 
}

CString DummyStr( int n )
{
	CString strDmy(_T("あ"));
	for( int i=0; i<n-2; i++)
		strDmy += _T("あ");
	return strDmy;
}

bool CheckFieldValue( IField* ipField, CComVariant& vaVal, CString& strErr )
{
	CComBSTR bstrName;
	ipField->get_Name( &bstrName );

	//VARIANT_BOOL vbIsOK = VARIANT_FALSE;
	//ipField->CheckValue( vaVal, &vbIsOK );

	//if(!vbIsOK )
	//{
	//	strErr = strErr + bstrName + _T("（CheckValue()） ");
	//	return false;
	//}

	esriFieldType emType;
	ipField->get_Type( &emType );

	switch( emType )
	{
	// 文字列フィールドは幅を超えてたら切り詰める
	case esriFieldTypeString:
		{
			if( vaVal.vt != VT_NULL )
			{
				CStringA strTmpA( vaVal.bstrVal );

				if( strTmpA.GetLength() > 4000  )
				{
					vaVal = CA2CT(strTmpA.Left( 3999 ));
					strErr = strErr + bstrName + _T("（切り詰め） ");
				}
			}
			break;
		}
	case esriFieldTypeDouble:
		{
			vaVal.ChangeType( VT_R8 );
			if( vaVal.vt == VT_BSTR )
				vaVal.ChangeType( VT_NULL );
			break;
		}
	default:
		break;
	}
	return true;
}


CString Var2Str( const CComVariant& vaVal, CString strDefault /*= _T("")*/ )
{
	return ( vaVal.vt == VT_EMPTY || vaVal.vt == VT_NULL ) ? strDefault : vaVal;
}

long Var2Long( const CComVariant& vaVal, long lDefault /*= -1*/ )
{
	return ( vaVal.vt == VT_EMPTY || vaVal.vt == VT_NULL ) ? lDefault : vaVal.lVal;
}

/**
* @brief <b>ID列からINで使用できる文字列を作成する</b>\n
* INで使用できるIDの数の上限は1000なので、1000を超える場合は
* 複数の文字列が返ります。
* @param listIDs	[in]	ID列
* @param strIDs	[out]	1000個区切りのID列（e.x. "'100','110','115','200'"）
*/
void IDs2Str( const std::list<CString>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep/* = NULL*/ )
{
	if( ! listIDs.empty() )
	{
		strIDs.push_back( CString(_T("")) );

		long lCount = 1;
		std::list<CString>::reverse_iterator itStr = strIDs.rbegin();

		for( std::list<CString>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it, ++lCount )
		{
			CString item;
			if( str_sep )
				item.Format(_T("%s%s%s"), str_sep, *it, str_sep );
			else
				item = *it;

			if( itStr->IsEmpty() ) 
				*itStr = item;
			else {
				*itStr += _T(",");
				*itStr += item;
			}

			if( 1000 == lCount )
			{
				strIDs.push_back( CString(_T("")) );
				itStr = strIDs.rbegin();
				lCount = 0;
			}
		}
	}
	// 1000で割り切れる場合だと空が余計に入るので除去
	if( ( ! strIDs.empty() ) && strIDs.rbegin()->IsEmpty() )
		strIDs.pop_back();
}
}// namespace gf::
