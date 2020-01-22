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
#include "AddrMgr.h"
#include "Logger.h"


void AddrMgr::init( const CString& conStr )
{
	RUNLOGGER.Info( uh::str_util::format( _T( "住所DB接続...： %s" ), conStr ), true );

	CString con;
	try{
		auto param = uh::str_util::split( conStr, _T("@") );
		con = uh::str_util::format( 
			_T(" Driver={Microsoft ODBC for Oracle}; Server=%s; UID=%s; PWD=%s; "), 
			param[1], param[0], param[0] );
	}catch(...){
		throw std::runtime_error( "接続文字列が不正" );
	}


	if( !m_ado.connect_by_string( con ) )
	{
		std::string msg = " サーバ接続エラー： " + CStringA(con);
		throw std::runtime_error( msg );
	}

	RUNLOGGER.Info( _T( "OK.\n" ), true );

}

void AddrMgr::cacheAddrInCity( const CString& cityCode )
{
	using namespace uh::str_util;

	// 住所マスタを利用して5桁レベルで住所文字列をキャッシュ
	CString strSQL = format(
		_T("SELECT oaza_code, aza_code, ken_kanji, ken_kana, shi_kanji, shi_kana, oaza_kanji, oaza_kana, aza_kanji, aza_kana FROM areacode_master ") \
		_T("WHERE ken_code = '%s' AND shi_code = '%s' "),
		cityCode.Mid( 0, 2 ), cityCode.Mid( 2, 3 ));

	auto cur = m_ado.search( CT2A(strSQL) );

	if( FAILED(cur.err()) )
	{
		throw std::runtime_error( CT2A(cur.GetErrorMessage()) );
	}

	m_addrs.clear();
	for( const auto& row : cur )
	{
		AddrInfo addr;

		CString oazaCode = ToString(row[0]);   // 大字コード
		CString azaCode  = ToString(row[1]);   // 字コード

		addr.ken_kanji  = ToString(row[2]);
		addr.ken_kana   = ToString(row[3]);
		addr.shi_kanji  = ToString(row[4]);
		addr.shi_kana   = ToString(row[5]);
		addr.oaza_kanji = ToString(row[6]);
		addr.oaza_kana  = ToString(row[7]);
		addr.aza_kanji  = ToString(row[8]);
		addr.aza_kana   = ToString(row[9]);

		m_addrs[ oazaCode + azaCode ] = addr;
	}
}

AddrInfo& AddrMgr::getAddrInfo( const CString& addrCode )
{
	// 11桁コードも付けて返す
	auto& addr = m_addrs[addrCode.Right( 6 )];
	addr.addr_code = addrCode;
	return addr;
}
