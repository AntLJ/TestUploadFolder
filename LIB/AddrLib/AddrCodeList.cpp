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
#include "AddrCodeList.h"
#include <SiNDYLib/Workspace.h>
#include <fstream>
#include <string>

namespace addr
{

using namespace sindy;
using namespace sindy::schema;

// 全国リスト作成
bool CAddrCodeList::MakeCodeListAll( long lLevel )
{
	CString strWhere;
	strWhere.Format( _T("%s <> '00000'"), city_admin::kCityCode );
	return AddFromQuery( strWhere, lLevel );
}

// コード範囲指定
bool CAddrCodeList::MakeCodeListFromRange( const _TCHAR* lpcszStartRange, const _TCHAR* lpcszEndRange )
{
	// 開始・終了でコード桁が合ってなかったらエラー
	if( lpcszStartRange && lpcszEndRange && _tcslen(lpcszStartRange) != _tcslen(lpcszEndRange) )
		return false;

	// コード桁取得
	long lLevel = _tcslen(lpcszStartRange);

	// Where句作成
	CString strWhere;
	switch( lLevel )
	{
	case 2:
		strWhere.Format( _T("%s between '%s000' and '%s999'"), city_admin::kCityCode, lpcszStartRange, lpcszEndRange );
		break;
	case 5:
		strWhere.Format( _T("%s between '%s' and '%s'"), city_admin::kCityCode, lpcszStartRange, lpcszEndRange );
		break;
	case 8:
		strWhere.Format( _T("%s %s %s between '%s000' and '%sF99'"),
						 city_admin::kCityCode, esriLocalDatabaseWorkspace == CWorkspace(GetWorkspace()).GetType()? _T("&") : _T("||"), city_admin::kAddrCode,
						 lpcszStartRange, lpcszEndRange );
		break;
	case 11:
		strWhere.Format( _T("%s %s %s between '%s' and '%s'"),
						 city_admin::kCityCode, esriLocalDatabaseWorkspace == CWorkspace(GetWorkspace()).GetType()? _T("&") : _T("||"), city_admin::kAddrCode,
						 lpcszStartRange, lpcszEndRange );
		break;
	default:
		return false;
	}

	return AddFromQuery( strWhere, lLevel );
}

// コードリストファイルから追加
bool CAddrCodeList::AddFromFile( const _TCHAR* lpcszFile, long lLevel /* = 0 */ )
{
	using namespace std;

	basic_ifstream<_TCHAR, char_traits<_TCHAR> > ifs( static_cast<CT2CW>(lpcszFile) );
	if( ifs )
	{
		typedef basic_string< _TCHAR, char_traits<_TCHAR>, allocator<_TCHAR> > tstring;
		tstring str;
		while( getline(ifs, str) )
		{
			if( !str.empty() )
			{
				if( lLevel < 1 || lLevel >= (signed)str.length() )
					m_listCode.insert( str.c_str() );
				else
					m_listCode.insert( str.substr(0, lLevel).c_str() );
			}
		}

		return ifs.eof();
	}
	else
		return false;	
}

/////////////////////////////////////////
// 非公開関数
/////////////////////////////////////////

// クエリから追加
bool CAddrCodeList::AddFromQuery( const _TCHAR* lpcszWhere, long lLevel )
{
	if( 1 <= lLevel && lLevel <= 11 )
	{
		// コード桁より、取得フィールド決定
		CString strSubFields;
		if( lLevel < 6 )
			strSubFields.Format( _T("distinct(%s)"), city_admin::kCityCode );
		else
			strSubFields.Format( _T("distinct(%s %s %s)"), 
								 city_admin::kCityCode, esriLocalDatabaseWorkspace == CWorkspace(GetWorkspace()).GetType()? _T("&") : _T("||"), city_admin::kAddrCode );

		// クエリ準備
		IQueryDefPtr ipQueryDef;
		((IFeatureWorkspacePtr)GetWorkspace())->CreateQueryDef( &ipQueryDef );
		ipQueryDef->put_Tables( _bstr_t(GetTableName()) );
		ipQueryDef->put_WhereClause( _bstr_t(lpcszWhere) );
		ipQueryDef->put_SubFields( _bstr_t(strSubFields) );

		_ICursorPtr ipCursor;
		if( SUCCEEDED(ipQueryDef->Evaluate(&ipCursor)) )
		{
			_IRowPtr ipRow;
			while( ipCursor->NextRow(&ipRow) == S_OK )
			{
				CComVariant vaValue;
				ipRow->get_Value( 0, &vaValue );
				if( VT_BSTR == vaValue.vt )
					m_listCode.insert( CString(vaValue.bstrVal).Left(lLevel) );
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

}	// namespace addr
