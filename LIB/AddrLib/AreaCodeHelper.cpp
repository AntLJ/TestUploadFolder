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
#include "AreaCodeHelper.h"

namespace addr
{
	CAreaCodeHelper	g_cAreaCodeHelper;

	// AreaCode_{Master|Kana}テーブルスキーマ(実際には、必要なものだけ定義)
	namespace am_schema
	{
		namespace common
		{
			const static TCHAR kKenCode[]		= _T("Ken_Code");
			const static TCHAR kShiCode[]		= _T("Shi_Code");
			const static TCHAR kOazaCode[]		= _T("Oaza_Code");
			const static TCHAR kAzaCode[]		= _T("Aza_Code");
		}

		namespace master
		{			
			const static TCHAR kTableName[]		= _T("Areacode_Master");
			const static TCHAR kKenKanji[]		= _T("Ken_Kanji");
			const static TCHAR kShiKanji[]		= _T("Shi_Kanji");
			const static TCHAR kOazaKanji[]		= _T("Oaza_Kanji");
			const static TCHAR kAzaKanji[]		= _T("Aza_Kanji");
			const static TCHAR kZipCode[]		= _T("Zip_Code");
			const static TCHAR kTorinaF[]		= _T("Torina_Flag");
			const static TCHAR kHaishiDate[]	= _T("Haishi_Date");
			const static TCHAR kShuseiCode[]	= _T("Shusei_Code");
			using namespace am_schema::common;
		}

		namespace kana
		{
			const static TCHAR kTableName[]		= _T("Areacode_Kana");
			const static TCHAR kKenKana[]		= _T("Ken_Kana_D");
			const static TCHAR kShiKana[]		= _T("Shi_Kana_D");
			const static TCHAR kOazaKana[]		= _T("Oaza_Kana_D");
			const static TCHAR kAzaKana[]		= _T("Aza_Kana_D");
			using namespace am_schema::common;
		}
	}

using namespace am_schema;

// 最新住所コード取得
CString CAreaCodeHelper::GetNewestCode( const _TCHAR* lpcszAddrCode )
{
	CString strSQL;
	strSQL.Format( _T("select getnewestcode('%s') from dual"), lpcszAddrCode );

	CJusyoRecordSet cRecords;
	return Select(strSQL, cRecords)? cRecords.GetStringValue((long)0) : _T("");
}

// 旧住所コード取得(合併前等の)
CString CAreaCodeHelper::GetPrevCode( const _TCHAR* lpcszAddrCode )
{
	CString strSQL;
	strSQL.Format( _T("select getprevcode('%s') from dual"), lpcszAddrCode );

	CJusyoRecordSet cRecords;
	return Select(strSQL, cRecords)? cRecords.GetStringValue((long)0) : _T("");
}

// 住所コードステータス返す(施行、廃止、無効)
addrCodeStatus CAreaCodeHelper::GetCodeStatus( const _TCHAR* lpcszAddrCode )
{
	CString strCode(lpcszAddrCode);
	if( strCode.GetLength() == 11 )
	{
		CString strSQL;
		strSQL.Format( _T("select %s, %s from %s where %s = '%s' and %s = '%s' and %s = '%s' and %s = '%s'"),
					   master::kHaishiDate, master::kShuseiCode, master::kTableName, 
					   master::kKenCode, strCode.Left(2), master::kShiCode, strCode.Mid(2, 3),
					   master::kOazaCode, strCode.Mid(5, 3), master::kAzaCode, strCode.Mid(8, 3) );

		CJusyoRecordSet cRecords;
		if( Select(strSQL, cRecords) && cRecords.GetRecordCount() == 1 )
		{
			// 施行・廃止チェック
			if( _T("000000") == cRecords.GetStringValue((long)0) )
				return _T("+") != cRecords.GetStringValue(1)? kShiko : kTmpShiko;	// 施行・仮施行チェック
			else
				return _T("-") != cRecords.GetStringValue(1)? kHaishi : kTmpHaishi;	// 廃止・仮廃止チェック
		}
		else
			return kInValid;
	}
	else
		return kInValid;
}

// 住所名取得
CString CAreaCodeHelper::GetAddrKanji( const _TCHAR* lpcszAddrCode )
{
	CString strCode( lpcszAddrCode );
	CString strSQL;
	int nLen = strCode.GetLength();
	if( 2 <= nLen && nLen <= 4 )
	{
		strSQL.Format( _T("select distinct %s from %s where %s = '%s'"), master::kKenKanji, master::kTableName, master::kKenCode, strCode.Left(2) );
	}
	else if( 5 <= nLen && nLen <= 7 )
	{
		strSQL.Format( _T("select distinct %s || %s from %s where %s = '%s' and %s = '%s'"), 
					   master::kKenKanji, master::kShiKanji, master::kTableName, master::kKenCode, strCode.Left(2), master::kShiCode, strCode.Mid(2, 3) );
	}
	else if( 8 <= nLen && nLen <= 10 )
	{
		strSQL.Format( _T("select distinct %s || %s || %s from %s where %s = '%s' and %s = '%s' and %s = '%s'"), 
					   master::kKenKanji, master::kShiKanji, master::kOazaKanji, master::kTableName, 
					   master::kKenCode, strCode.Left(2), master::kShiCode, strCode.Mid(2, 3), master::kOazaCode, strCode.Mid(5, 3) );
	}
	else if( 11 <= nLen )
	{
		strSQL.Format( _T("select distinct %s || %s || %s || %s from %s where %s = '%s' and %s = '%s' and %s = '%s' and %s = '%s'"), 
					   master::kKenKanji, master::kShiKanji, master::kOazaKanji, master::kAzaKanji, master::kTableName, 
					   master::kKenCode, strCode.Left(2), master::kShiCode, strCode.Mid(2, 3), master::kOazaCode, strCode.Mid(5, 3), master::kAzaCode, strCode.Mid(8, 3) );
	}
	else
		return _T("");

	CJusyoRecordSet cRecords;
	CString strAddrName = (Select(strSQL, cRecords) && cRecords.GetRecordCount() == 1)? cRecords.GetStringValue( (long)0 ) : _T("");
	strAddrName.Replace( _T("　"), _T("") );	// 全角空白除去
	return strAddrName;
}

// 住所カナ取得
CString CAreaCodeHelper::GetAddrKana( const _TCHAR* lpcszAddrCode )
{
	CString strCode( lpcszAddrCode );
	CString strSQL;
	int nLen = strCode.GetLength();
	if( 2 <= nLen && nLen <= 4 )
	{
		strSQL.Format( _T("select distinct %s from %s where %s = '%s'"), kana::kKenKana, kana::kTableName, kana::kKenCode, strCode.Left(2) );
	}
	else if( 5 <= nLen && nLen <= 7 )
	{
		strSQL.Format( _T("select distinct %s || %s from %s where %s = '%s' and %s = '%s'"), 
					   kana::kKenKana, kana::kShiKana, kana::kTableName, kana::kKenCode, strCode.Left(2), kana::kShiCode, strCode.Mid(2, 3) );
	}
	else if( 8 <= nLen && nLen <= 10 )
	{
		strSQL.Format( _T("select distinct %s || %s || %s from %s where %s = '%s' and %s = '%s' and %s = '%s'"), 
					   kana::kKenKana, kana::kShiKana, kana::kOazaKana, kana::kTableName, 
					   kana::kKenCode, strCode.Left(2), kana::kShiCode, strCode.Mid(2, 3), kana::kOazaCode, strCode.Mid(5, 3) );
	}
	else if( 11 <= nLen )
	{
		strSQL.Format( _T("select distinct %s || %s || %s || %s from %s where %s = '%s' and %s = '%s' and %s = '%s' and %s = '%s'"), 
					   kana::kKenKana, kana::kShiKana, kana::kOazaKana, kana::kAzaKana, kana::kTableName, 
					   kana::kKenCode, strCode.Left(2), kana::kShiCode, strCode.Mid(2, 3), kana::kOazaCode, strCode.Mid(5, 3), kana::kAzaCode, strCode.Mid(8, 3) );
	}
	else
		return _T("");

	CJusyoRecordSet cRecords;
	CString strAddrKana = (Select(strSQL, cRecords) && cRecords.GetRecordCount() == 1)? cRecords.GetStringValue( (long)0 ) : _T("");
	strAddrKana.Replace( _T("　"), _T("") );	// 全角空白除去
	return strAddrKana;
}

}	// namespace addr
