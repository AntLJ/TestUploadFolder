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
#include "AddrServerBase.h"
#include <iostream>
#include <tuple>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
#include "crd_cnv.h"

#define foreach BOOST_FOREACH
void PrintComError(_com_error &e);
extern crd_cnv g_cnv;

/// 先頭から連続するchを削除する
CString ltrim( const CString& str, TCHAR ch )
{
	long c = str.GetLength();
	long i = 0;
	for( ; i < c; ++i )
	{
		if( str.GetAt(i) != ch )
			break;
	}
	if( 0 < i )
		return str.Right( c - i );

	return str;
}

/// 文字列strに対して、fromからtoに変換する（一文字ずつ）
CString translate( const CString& str, const CString& from, const CString& to )
{
	_ASSERTE( from.GetLength() == to.GetLength() );

	CString ret( str );

	long c = from.GetLength();
	for( int i = 0; i < c; ++i )
		ret.Replace( from.GetAt(i), to.GetAt(i) );

	return ret;
}

bool is123( TCHAR ch ) {
	switch( ch )
	{
	case '1':
	case '2':
	case '3':
		return true;
		break;
	default: break;
	}
	return false;
}

CAddrServerBase::CAddrServerBase(void) : m_bUseProcedure(false), m_bIsOracle(false)
{
}

CAddrServerBase::~CAddrServerBase(void)
{
}

// 接続する
bool CAddrServerBase::Connect( LPCTSTR lpcszConn )
{
	if( m_cConn.handle() )
		return true; // 接続済み

	_ConnectionPtr conn( __uuidof(Connection) );
	m_cConn = conn;

	// 接続
	conn->ConnectionString = lpcszConn;
	try {
		conn->Open(_T(""),"","",adConnectUnspecified);
	}
	catch( _com_error& e )
	{
		conn = NULL;
		std::cerr << "ERROR: 住所サーバへの接続失敗 : [" << (LPCSTR)CStringA(lpcszConn) << "]" << std::endl;
		PrintComError(e);

		return false;
	}
	// OracleかPostgreSQLか
	if( ! _tcsstr( lpcszConn, _T("PostgreSQL") ) )
		m_bIsOracle = true;

	return true;

}

// 住所文字列から座標を取得するファンクション(SS-PJで使用)
CString CAddrServerBase::addrtocode( LPCTSTR lpcszAddr ) const
{
//	if( GetUseProcedure() )
//		return _addrtocode( lpcszAddr );

	// 住所文字列 → 住所コード化
	CString result( addr2code( lpcszAddr ) );

	// 解析住所コード取得
	CString strAddrCode( translate( result.Left( result.Find( _T(',') ) ), _T("x."), _T("00") ) );

	// 解析精度取得
	long ret_seido = _ttol( result.Mid( result.Find(_T(','))+1, 1 ) );

	std::vector<CString> vecAddrName; // 住所名各階層格納用
	int mesh = 0, x = 0, y = 0;
	long ap_seido = 0; // Address_Point精度
	if( 0 < ret_seido )
	{		
		if( ! ( GetAddrName( strAddrCode, vecAddrName ) && // 住所コードから住所名称取得
			GetAddrPos( strAddrCode, ret_seido, mesh, x, y, ap_seido ) ) ) // 住所コードから座標取得
			return CString();

		// proteus版は解析精度6段階なので、4以上は-1
		if( 3 < ret_seido )
			ret_seido -= 1;
	}

	// IPCODE, CHIBAN_STATUSは必要ないので、NULL
	CString ret;
	ret.Format(_T("%s,%ld,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%ld,%ld,%ld,,%ld,"), 
		lpcszAddr, ret_seido, 
		strAddrCode.Mid( 0, 2 ), strAddrCode.Mid( 2, 3 ), strAddrCode.Mid( 5, 3 ), 
		strAddrCode.Mid( 8, 3 ), strAddrCode.Mid( 11, 5 ), strAddrCode.Mid( 16, 4 ),
		vecAddrName[0], vecAddrName[1], vecAddrName[2], vecAddrName[3], vecAddrName[4], vecAddrName[5],
		mesh, x, y, ap_seido );

	return ret;
}

CString CAddrServerBase::_addr2code( const CString& strAddr ) const
{
	CStringA strSQL;
	if( m_bIsOracle )
		strSQL.Format( "SELECT /*+ FIRST_ROWS */ CAST( %s.ADRS2CODE.CONV('%s') AS CHAR(512)) FROM DUAL", CStringA(m_strOwner), CStringA(strAddr) );
	else
		strSQL.Format( "SELECT CAST( ADDRTOCODE.CONV('%s') AS CHAR(512)) FROM DUAL", CStringA(strAddr) );

	CADOCursor cur( m_cConn.search( "%s", strSQL ) );
	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;
	}

	return (*cur.begin())[0].bstrVal;
}

CString CAddrServerBase::addr2code( const CString& strAddr ) const
{
	if( GetUseProcedure() )
		return _addr2code( strAddr );

	// 正規化
	CString strAddr2( translate( strAddr, _T("0123456789ABCDEF"), _T("０１２３４５６７８９ＡＢＣＤＥＦ") ) ); // 全角化
	strAddr2.Replace(_T("　"),_T("")); // スペースの除去
	strAddr2 = CommonExpr(strAddr2); // 正規化

	int nRetFlag = 0;

	// 都道府県コードの取得
	CString strKenCode(_T("xx"));
	GetKenCode( strAddr2, strKenCode );
	if( strKenCode != _T("xx") )
		nRetFlag = 1;

	// 市区町村コードの取得
	CString strShiCode(_T("xxx"));
	if( 1 == nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetShiCode( strAddr2, strKenCode, strShiCode );
		if( strShiCode != _T("xxx") )
			nRetFlag = 2;
	}

	// 直地番？
	CString strOazaCode(_T("xxx")), strAzaCode(_T("xxx"));
	if( 2 == nRetFlag && 0 == strAddr2.FindOneOf( _T("０１２３４５６７８９") ) )
	{
		// [Bug 8817] 20桁対応住所に不正な数字が入力される
		// 移植ミス（intoは使えない）
		CADOCursor cur( m_cConn.search( 
			"select Oaza_Code, Aza_Code from %s.AreaCode_Master "
				"where Ken_Code  = '%s' and "
				"      Shi_Code  = '%s' and "
				"     Oaza_Code != '000' and "
				"      Aza_Code  = '000' and "
				"    Oaza_Kanji is null and "
				"    Haishi_Date = '000000'", CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode) ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr2;
		}

		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			strOazaCode = vals[0].bstrVal;
			strAzaCode  = vals[1].bstrVal;
			nRetFlag = 4;
			break;
		}
	}

	// 大字・字コードの取得
	if( 2 == nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetOazaAzaCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode );
		if( _T("xxx") != strOazaCode )
			nRetFlag = ( _T("xxx") == strAzaCode ) ? 3 : 4;
		else {
			// 大字・字・小字が先頭にあればそれを除いて再チェック
			if( 0 == strAddr2.Find( _T("大字") ) ) strAddr2.Delete( 0, 2 );
			if( 0 == strAddr2.Find( _T("小字") ) ) strAddr2.Delete( 0, 2 );
			if( 0 == strAddr2.Find(   _T("字") ) ) strAddr2.Delete( 0, 1 );
			GetOazaAzaCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode );
			if( _T("xxx") != strOazaCode )
				nRetFlag = ( _T("xxx") == strAzaCode ) ? 3 : 4;
		}
	}

	// 地番コードの取得
	CString strChibanCode(_T("xxxxx"));
	if( 3 <= nRetFlag && ( ! strAddr2.IsEmpty() ) )
	{
		GetChibanCode( strAddr2, nRetFlag, strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode );
		if( _T("xxxxx") != strChibanCode )
			nRetFlag = 5;
		else if( _T("xxx") == strAzaCode )
			nRetFlag = 3;
	}

	// 番号コードの取得
	CString strBangoCode(_T("xxxx"));
	if( 5 == nRetFlag )
	{
		GetBangoCode( strAddr2, strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode );
		if( _T("xxxx") != strBangoCode )
			nRetFlag = 6;
		else
			strBangoCode = _T("0000"); // 解析文字列が地番までしかないとき
	}

	if( strAddr2.IsEmpty() )
	{
		if( 5 > nRetFlag )
		{
			strBangoCode  = _T("....");
			strChibanCode = _T(".....");
		}
		if( 4 > nRetFlag )
			strAzaCode = _T("...");
		if( 3 > nRetFlag )
			strOazaCode = _T("...");
		if( 2 > nRetFlag )
			strShiCode = _T("...");
	}

	CString strRet;
	strRet.Format(_T("%s%s%s%s%s%s,%d,%s"), strKenCode, strShiCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode, nRetFlag, strAddr2 );

	return strRet;
}

// 住所コード取得関数２(市区町村コード指定版)
CString CAddrServerBase::addr2code( const CString& strCityCode, const CString& strAddr ) const
{
	if( strCityCode.GetLength() != 5 )
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;

	// 市区町村コードから名称取得
	CString strCityKanji;
	CADOCursor cur( m_cConn.search(
		"select distinct replace(ken_kanji || shi_kanji, '　', '') from %s.areacode_master "
		"where ken_code = substr('%s', 1, 2) "
		"and shi_code = substr('%s', 3, 3) "
		"and oaza_code = '000' "
		"and aza_code = '000'", CStringA(m_strOwner), CStringA(strCityCode), CStringA(strCityCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return _T("xxxxxxxxxxxxxxxxxxxx,0,") + strAddr;
	}


	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		strCityKanji = vals[0].bstrVal;

	return addr2code( strCityKanji + strAddr );
}

// 住所コードから住所名取得
bool CAddrServerBase::GetAddrName( LPCTSTR lpcszAddrCode, std::vector<CString>& listName ) const
{
	CString ken_name, shi_name, oaza_name, aza_name, tmp_name;
	CStringA strAddrCode( lpcszAddrCode );
	CStringA strPrefCode(   strAddrCode.Mid(  0, 2 ) );
	CStringA strCityCode(   strAddrCode.Mid(  2, 3 ) );
	CStringA strOazaCode(   strAddrCode.Mid(  5, 3 ) );
	CStringA strAzaCode(    strAddrCode.Mid(  8, 3 ) );
	CStringA strChibanCode( strAddrCode.Mid( 11, 5 ) );
	CStringA strBangoCode(  strAddrCode.Mid( 16, 4 ) );

	listName.assign( 6, CString() );

	// 都道府県～字・丁目取得
	{
		CADOCursor cur( m_cConn.search( 
			"select ken_kanji, shi_kanji, oaza_kanji, aza_kanji "
			"from %s.areacode_master "
			"where ken_code = '%s' and "
			"  shi_code = '%s' and "
			" oaza_code = '%s' and "
			"  aza_code = '%s'",
			CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return false;
		}

		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			for( int i = 0; i < 4; ++i )
				listName[i] = ( VT_BSTR == vals[i].vt ) ? CString(vals[i].bstrVal) : _T("");
		}
	}

	// 地番取得
	if( strChibanCode != "00000" )
	{
		// 文字付き？
		if( 0 == strChibanCode.Left(1).FindOneOf("ABCDEF") )
		{
			CADOCursor cur( m_cConn.search( 
				"select chiban_name from %s.address_point "
				"where ken_code = '%s' and "
				"	   shi_code = '%s' and "
				"     oaza_code = '%s' and "
				"      aza_code = '%s' and "
				"   chiban_code = '%s' and "
				"    bango_code = '0000'",
				CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode ) );

			if( S_OK != cur.err() )
			{
				ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
				return false;
			}

			foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
				listName[4] = vals[0].bstrVal;
		}
		else {
			listName[4] = translate( ltrim( CString(strChibanCode), _T('0') ), _T("0123456789ABCDEF"), _T("０１２３４５６７８９ＡＢＣＤＥＦ") );
		}
	}

	// 住所番号取得
	if( strBangoCode != "0000" )
	{
		// 文字付き？
		if( 0 == strBangoCode.Left(1).FindOneOf("ABCDEF") )
		{
			CADOCursor cur( m_cConn.search( 
				"select bango_name from %s.address_point "
				"where ken_code = '%s' and "
				"	   shi_code = '%s' and "
				"     oaza_code = '%s' and "
				"      aza_code = '%s' and "
				"   chiban_code = '%s' and "
				"    bango_code = '%s'",
				CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode ) );

			if( S_OK != cur.err() )
			{
				ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
				return false;
			}

			foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
			{
				listName[5] = vals[0].bstrVal;
				break;
			}
		}
		else {
			listName[5] = translate( ltrim( CString(strBangoCode), _T('0') ), _T("0123456789ABCDEF"), _T("０１２３４５６７８９ＡＢＣＤＥＦ") );
		}
		listName[5] = _T("－") + listName[5];
	}

	return true;
}

// 住所コードから座標取得
bool CAddrServerBase::GetAddrPos( LPCTSTR lpcszAddrCode, long seido, int& mesh, int& x, int& y, long& seido_ap ) const
{
	double lon = -1.0f, lat = -1.0f;
	seido_ap = -1;
	long tmp_seido = seido;

	CStringA strAddrCode(lpcszAddrCode);
	CStringA strPrefCode( strAddrCode.Mid( 0, 2 ) );
	CStringA strCityCode( strAddrCode.Mid( 2, 3 ) );
	CStringA strOazaCode;
	CStringA strAzaCode;
	CStringA strChibanCode;
	CStringA strBangoCode;

	// 住所コードを元に全号住所から座標取得(大字代表点の精度が2なので3にする)
	while( tmp_seido > 0 )
	{
		strOazaCode = (   tmp_seido > 2 ? strAddrCode.Mid(  5, 3 ) : "000" );
		strAzaCode  = (   tmp_seido > 3 ? strAddrCode.Mid(  8, 3 ) : "000" );
		strChibanCode = ( tmp_seido > 4 ? strAddrCode.Mid( 11, 5 ) : "00000" );
		strBangoCode = (  tmp_seido > 5 ? strAddrCode.Mid( 16, 4 ) : "0000" );
		CADOCursor cur( m_cConn.search(
			"select longitude,latitude, "
				"case "
					"when oaza_code != '000' and aza_code = '000' and chiban_code = '00000' then '3' "
					"else seido_status "
				"end "
			"from %s.address_point "
			"where  ken_code = '%s' "
			"and    shi_code = '%s' "
			"and   oaza_code = '%s' "
			"and    aza_code = '%s' "
			"and chiban_code = '%s' "
			"and  bango_code = '%s'",
			CStringA(m_strOwner), strPrefCode, strCityCode, strOazaCode, strAzaCode, strChibanCode, strBangoCode ) );

		if( S_OK != cur.err() )
		{
			ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
			return false;
		}
		
		foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		{
			CComVariant vaLon(vals[0]), vaLat(vals[1]), vaSeido(vals[2]);
			vaLon.ChangeType(VT_R8);
			vaLat.ChangeType(VT_R8);
			vaSeido.ChangeType(VT_I4);
			lon = vaLon.dblVal;
			lat = vaLat.dblVal;
			seido_ap = vaSeido.lVal;
		}
		if( 0 < lon )
			break; // 座標値取得できたらループ抜ける
		else
			--tmp_seido; // なければ精度下げて再検索
	}
	// 10進緯度経度から、2次メッシュXYへ
	g_cnv.LLtoMesh( lon, lat, 2, &mesh, &x, &y, 1 ); // XY座標が1000000になることを許容しない

	return true; // 検索結果が0でもtrueで返ることに注意
}

// 住所文字列を正規化する（揺らぎの吸収）
CString CAddrServerBase::CommonExpr( CString& strAdrs )
{
	CString strNewAdrs( translate( strAdrs, _T("‐―ーノ之ケヶツッ舘渕佛條澤檜藪槇ヱ萬籠溪藏鍜冶龍瀧冨曾峯邊"), _T("－－－ののががつつ館淵仏条沢桧薮槙エ万篭渓蔵鍛治竜滝富曽峰辺") ) );
	strNewAdrs.Replace( _T("通り"),   _T("通") );
	strNewAdrs.Replace( _T("通リ"),   _T("通") );
	strNewAdrs.Replace( _T("上がる"), _T("上") );
	strNewAdrs.Replace( _T("上る"),   _T("上") );
	strNewAdrs.Replace( _T("上ル"),   _T("上") );
	strNewAdrs.Replace( _T("下がる"), _T("下") );
	strNewAdrs.Replace( _T("下る"),   _T("下") );
	strNewAdrs.Replace( _T("下ル"),   _T("下") );
	strNewAdrs.Replace( _T("入る"),   _T("入") );
	strNewAdrs.Replace( _T("入ル"),   _T("入") );

	return strNewAdrs;
}

// 検索文字列が先頭から存在した場合にその文字列をカットする
CString CAddrServerBase::ReplaceHead( const CString& src, const CString& rep )
{
	if( 0 == src.Find( rep ) )
		return src.Right( src.GetLength() - rep.GetLength() );
	else
		return src;
}

// 「大字」「字」「小字」の除去
CString CAddrServerBase::RemoveAzaHead( const CString& src )
{
	CString strNew( ReplaceHead( src, _T("大字") ) );
	strNew = ReplaceHead( strNew, _T("小字") );
	return ReplaceHead( strNew, _T("字") );
}

// 漢数字→アラビア数字変換関数（「一」～「九十九」→ 「１」～「９９」）
CString CAddrServerBase::Kanji2Num( const CString& src )
{
	if( ! src )
		return CString();

	CString ret; // 戻り値

	long count = src.GetLength();
	TCHAR ch = _T('\0');
	int nTenFlag = 0;
	for( long i = 0; i < count; ++i )
	{
		ch = src.GetAt(i);
		switch( ch )
		{
		// 漢数字が現れたとき
		case _T('一'):
		case _T('二'):
		case _T('三'):
		case _T('四'):
		case _T('五'):
		case _T('六'):
		case _T('七'):
		case _T('八'):
		case _T('九'):
		case _T('〇'):
			nTenFlag = 1;
			ret += translate( CString(ch), _T("一二三四五六七八九〇"), _T("１２３４５６７８９０") );
			break;
		// 「十」が現れたとき
		case _T('十'):
			switch( nTenFlag )
			{
				case 0:	ret += _T("１"); break;    // 直前が漢数字でないとき
				case 2: ret += _T("０１"); break;  // 直前が「十」のとき
				default: break;
			}
			nTenFlag = 2;
			break;
		// 漢数字でないとき
		default:
			if( 2 == nTenFlag ) // 直前が「十」のとき
				ret += _T("０");
			nTenFlag = 0;
			ret += ch;
			break;
		}
	}
	return ret;
}

// 漢数字→アラビア数字変換関数（「一」～「九十九」→ 「１」～「９９」）
CString CAddrServerBase::Kanji2NumEx( const CString& src )
{
	static boost::wregex g_reg_exp(L"([一二三四五六七八九十]+)(条|地割|丁|線)");
	static std::list<std::tr1::tuple<CString,int,int>> g_container; // 置換対象位置と長さ
	typedef std::tr1::tuple<CString,int,int> TYPE_PAIR;

	// regex_grepのコールバック関数
	// g_containerに検索結果を蓄積する
	struct callback {
		// 後ろから置き換えするので、push_frontで突っ込む
		bool operator()( const boost::match_results<std::wstring::iterator>& mr ) {
			g_container.push_front( std::tr1::make_tuple( CString((mr.str(1)+mr.str(2)).c_str()), (int)mr.position(1), (int)mr.length(1) + (int)mr.length(2) ) ); // 数値部分の位置と長さを記憶（bug 8588）
			return true;
		}
	};

	// 処理はここから
	std::wstring wsrc( (LPCWSTR)CStringW(src) );
	g_container.clear();
	boost::regex_grep( callback(), wsrc.begin(), wsrc.end(), g_reg_exp );
	foreach( const TYPE_PAIR& _pair, g_container )
		wsrc.replace( std::tr1::get<1>(_pair), std::tr1::get<2>(_pair), std::wstring( (LPCWSTR)CStringW( Kanji2Num( std::tr1::get<0>(_pair) ) ) ) );

	return wsrc.c_str();
}

// 都道府県コード取得関数
bool CAddrServerBase::GetKenCode( CString& strAddr, CString& strRetKenCode, bool doCache/* = true*/ ) const
{
	strRetKenCode = _T("xx");

	// キャッシュ内で前方一致検索
	static std::map<CString,CString> g_mapPrefNameCode; //!< key:県名 val:県コード
	typedef std::pair<CString,CString> TYPE_PAIR;
	foreach( const TYPE_PAIR& _pair, g_mapPrefNameCode ) // TODO: 毎回ループまわすよりは、mapのindexを使ったほうがはやそう
	{
		if( strAddr.Left(_pair.first.GetLength()) == _pair.first )
		{
			strAddr.Delete( 0, _pair.first.GetLength() );
			strRetKenCode = _pair.second;
			return true;
		}
	}

	if( ! doCache )
		return false;

	// 無い場合はキャッシュする
	CADOCursor cur( m_cConn.search(
		"select distinct ken_code, ken_kanji from %s.areacode_master "
		"where shi_code = '000' and "
		"     oaza_code = '000' and "
		"      aza_code = '000'" , CStringA(m_strOwner) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
		g_mapPrefNameCode[vals[1].bstrVal] = vals[0].bstrVal;

	return GetKenCode( strAddr, strRetKenCode, false ); // 無限ループにならないよう、キャッシュはさせない
}

// 市区町村コード取得関数
bool CAddrServerBase::GetShiCode( CString& strAddr, const CString& strKenCode, CString& strRetShiCode, bool doCache/* = true*/ ) const
{
	strRetShiCode = _T("xxx");

	static std::map<CString,std::map<CString,CString>> g_mapCityNameCode; //!< key:県コード val{ key:市区町村名 val:市区町村コード }
	static std::map<CString,std::map<CString,bool>> g_mapCityCodeHaishi; //!< key:県コード val{ key:市区町村コード val:廃止フラグ }
	std::map<CString,std::map<CString,CString>>::const_iterator it = g_mapCityNameCode.find(strKenCode);
	if( g_mapCityNameCode.end() != it )
	{
		// キャッシュ内で前方一致検索
		typedef std::pair<CString,CString> TYPE_PAIR;
		const TYPE_PAIR* _pret = NULL;
		foreach( const TYPE_PAIR& _pair, it->second ) // TODO: 毎回ループまわすよりは、mapのindexを使ったほうがはやそう
		{
			if( strAddr.Left(_pair.first.GetLength()) == _pair.first )
			{
				// 廃止コードでも一応返すが、生きているほうを優先するので保留
				if( g_mapCityCodeHaishi[strKenCode][_pair.second] )
					_pret = &_pair;
				else {
					strAddr.Delete( 0, _pair.first.GetLength() );
					strRetShiCode = _pair.second;
					return true;
				}
			}
		}
		// 廃止コードしかない場合はそれで
		if( _pret )
		{
			strAddr.Delete( 0, _pret->first.GetLength() );
			strRetShiCode = _pret->second;
			return true;
		}

		// itがあるということは、一度はキャッシュを作成したということなので、
		// 再度キャッシュを作ることはしない
		doCache = false;
	}

	if( ! doCache )
		return false;

	// 無い場合はキャッシュする
	CADOCursor cur( m_cConn.search( 
		"select Shi_Code,Shi_Kanji, "
			"case Haishi_Date "
				"when '000000' then 0 "
				"else 1 "
			"end as Haishi_F "
		"from %s.AreaCode_Master "
		"where Ken_Code = '%s' "
		"and   Shi_Code != '000' "
		"and   Oaza_Code = '000' "
		"and    Aza_Code = '000' "
		"and Shi_Kanji not like '%s' "
		"and Shi_Kanji not like '%s' "
		"and not (Shi_Code like '1__' and Shi_Kanji like '%s')", CStringA(m_strOwner), CStringA(strKenCode), "%郡", "%支庁", "%市" ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	std::map<CString,CString>& mapCityNameCode = g_mapCityNameCode[strKenCode];
	std::map<CString,bool>& mapCityCodeHaishi = g_mapCityCodeHaishi[strKenCode];
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strCityName(vals[1].bstrVal);
		strCityName.Replace(_T("　"), _T("")); // 全角空白は除去しておく（bug 8570）
		mapCityNameCode[CommonExpr(strCityName)] = vals[0].bstrVal;
		CComVariant vaHaishiF( vals[2] );
		vaHaishiF.ChangeType( VT_I4 );
		mapCityCodeHaishi[vals[0].bstrVal] = ( 0 == vaHaishiF.lVal ) ? false : true;
	}

	return GetShiCode( strAddr, strKenCode, strRetShiCode, false ); // 無限ループにならないよう、キャッシュはさせない
}

// 大字・字コード取得関数
bool CAddrServerBase::GetOazaAzaCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, CString& strRetOazaCode, CString& strRetAzaCode ) const
{
	strAddr = Kanji2NumEx(strAddr); // 漢数字をアラビア数字に変換

	CADOCursor cur( m_cConn.search( 
		"select oaza_code, oaza_kanji, oaza_flag, "
			"aza_code, aza_kanji, aza_flag, haishi_date "
		"from %s.areacode_master "
		"where ken_code = '%s' and "
			"shi_code = '%s' and "
			"oaza_code != '000' and "
			"aza_code not between '851' and '899'", CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	int nMatchLen = 0; // マッチングした文字列長(長いものほど優先する)
	CString strMatchOazaCode(_T("xxx")); // マッチングした大字コード一時保持用
	CString strMatchAzaCode(_T("xxx")); // マッチングした字コード一時保持用
	CString strMatchHDate(_T("000000")); // マッチングしたレコードの廃止日

	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		bool bMatch = false; // 各レコード内でマッチングしたかを指し示すフラグ

		// 大字、字の正規化及び空白除去してくっつける
		CString strOazaKanji;
		if( VT_BSTR == vals[1].vt )
			strOazaKanji = Kanji2NumEx(CommonExpr(CString(vals[1].bstrVal))); // TODO: oaza_kanjiがNULLのものははじいてもいいかな？？ってか、はじかないと落ちる
		CString strAzaKanji;
		if( VT_BSTR == vals[4].vt )
			strAzaKanji = Kanji2NumEx(CommonExpr(CString(vals[4].bstrVal)));
		CString strOazaKanjiT(strOazaKanji); // 空白トリムあり
		CString strAzaKanjiT(strAzaKanji); // 空白トリムあり
		strOazaKanjiT.Trim();
		strAzaKanjiT.Trim();
		CString strTargetAdrs( strOazaKanjiT + strAzaKanjiT ); // 大字＋字

		// 特に表記揺れ考慮せずに検索
		CString strHaishiDate( vals[6].bstrVal );
		CString strOazaCode( vals[0].bstrVal );
		CString strAzaCode( vals[3].bstrVal );
		if( 0 == strAddr.Find( strTargetAdrs ) ) // 前方一致するなら
		{
			if( nMatchLen < strTargetAdrs.GetLength() ||
				( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
			{
				nMatchLen = strTargetAdrs.GetLength();
				strMatchOazaCode = strOazaCode;
				strMatchAzaCode = strAzaCode;
				strMatchHDate = strHaishiDate;
				bMatch = true;
			}
		}

		// 「丁目」除去して検索
		long lAzaCode = _ttol(strAzaCode);
		if( ( ! bMatch ) && 1 <= lAzaCode && lAzaCode <= 99 )
		{
			strTargetAdrs = strAzaKanjiT;
			// 丁目の場合，「丁目」をハイフンに置き換え
			strTargetAdrs.Replace( _T("丁目南"), _T("南") );	// 01 104 085 0xx など
			strTargetAdrs.Replace( _T("丁目北"), _T("北") );	// 01 104 086 0xx など
			strTargetAdrs.Replace( _T("丁目"), _T("") );
			strTargetAdrs.Replace( _T("丁"), _T("") );			// 20 201 xxx 0xx など
			strTargetAdrs = strOazaKanjiT + strTargetAdrs;

			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				// マッチングしてもその次の文字が数字ならＮＧ
				int len = strTargetAdrs.GetLength();
				if( strAddr.GetLength() == len ||
					0 != strAddr.Mid( len, 1 ).FindOneOf( _T("１２３４５６７８９０") ) )
				{
					if( nMatchLen < strTargetAdrs.GetLength() ||
						( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
					{
						nMatchLen = strTargetAdrs.GetLength();
						strMatchOazaCode = strOazaCode;
						strMatchAzaCode = strAzaCode;
						strMatchHDate = strHaishiDate;
						bMatch = true;
					}
				}
			}
		}

		// 「大字」「字」「小字」フラグによる表記揺れ対応検索
		TCHAR oazaFlag = CString(vals[2].bstrVal).GetAt(0);
		TCHAR azaFlag  = CString(vals[5].bstrVal).GetAt(0);
		bool oaza_is123 = is123(oazaFlag);
		bool  aza_is123 = is123(azaFlag);
		if( ( ! bMatch ) && ( oaza_is123 || aza_is123 ) )
		{
			strTargetAdrs = ( oaza_is123 ? RemoveAzaHead(strOazaKanji) : strOazaKanji ) + ( aza_is123 ? RemoveAzaHead(strAzaKanji) : strAzaKanji );
			strTargetAdrs.Replace(_T("　"), _T(""));
			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				if( nMatchLen < strTargetAdrs.GetLength() ||
					( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
				{
					nMatchLen = strTargetAdrs.GetLength();
					strMatchOazaCode = strOazaCode;
					strMatchAzaCode = strAzaCode;
					strMatchHDate = strHaishiDate;
					bMatch = true;
				};
			}
		}

		// 単純に「大字」「字」「小字」を除去(とりあえず多いものだけ)
		if( ( ! bMatch ) && ( 0 < strOazaKanji.Find(_T("　大字")) || 0 < strOazaKanji.Find(_T("　字")) ) )
		{
			strTargetAdrs = strOazaKanji;
			strTargetAdrs.Replace(_T("　大字"), _T(""));
			strTargetAdrs.Replace(_T("　字"), _T(""));
			strTargetAdrs.Replace(_T("　"), _T(""));
			if( 0 == strAddr.Find( strTargetAdrs ) )
			{
				if( nMatchLen < strTargetAdrs.GetLength() ||
					( strMatchHDate != _T("000000") && strHaishiDate == _T("000000") ) )
				{
					nMatchLen = strTargetAdrs.GetLength();
					strMatchOazaCode = strOazaCode;
					strMatchAzaCode = strAzaCode;
					strMatchHDate = strHaishiDate;
					bMatch = true;
				};
			}
		}
	}
	strRetOazaCode = strMatchOazaCode;
	strRetAzaCode  = strMatchAzaCode;

	if( strRetOazaCode != _T("xxx") )
		strAddr.Delete( 0, nMatchLen );

	return true;
}

// 地番コード取得関数
bool CAddrServerBase::GetChibanCode( CString& strAddr, int nRetF, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, CString& strAzaCode, CString& strRetChibanCode ) const
{
	strRetChibanCode = _T("xxxxx");

	// strAddrの一文字目でlike検索するが、これが字のケースあり
	// 鹿児島県薩摩川内市樋脇町市比野字宮元８８
	// 本来のコードにはないのだが、ここで字を取り除くのが一番よさそう
//	if( strAddr.Left(1) == _T("字") )
//		strAddr = strAddr.Right(strAddr.GetLength()-1);

	// 先頭がハイフンなら除去する
	if( strAddr.GetAt(0) == _T('－') )
		strAddr.Delete( 0, 1 );
	if( strAddr.IsEmpty() )
		return false;

	CADOCursor cur( m_cConn.search(
		"select chiban_code, chiban_name from %s.address_point "
		"where ken_code = '%s' and "
			"  shi_code = '%s' and "
			" oaza_code = '%s' and "
			"  aza_code = '%s' and "
			"addr_type in ('1', '3') and "
			"chiban_name like '%s' "
			"order by length(chiban_name) desc", 
			CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode), 
			CStringA(strOazaCode), CStringA(strAzaCode), CStringA(strAddr.Left(1)) + "%" ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	// 大字までしか解析できていない場合は、暫定的に字コード[000]にする
	if( 3 == nRetF )
		strAzaCode = _T("000");

	// 文字付き地番に一致するものあるか確認
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strChibanCode( vals[0].bstrVal );
		CString strChibanKanji( vals[1].bstrVal );
		// 最初に完全一致したものを採用
		if( 0 == strAddr.Find( strChibanKanji ) )
		{
			strRetChibanCode = strChibanCode;
			strAddr.Delete( 0, strChibanKanji.GetLength() );
			break;
		}
	}

	// 文字付き地番と一致しなければ、通常地番として解析
	static CString g_strWideNmber(_T("０１２３４５６７８９"));
	if( strRetChibanCode == _T("xxxxx") )
	{
		// 地番文字列の取得
		int finded = strAddr.FindOneOf(g_strWideNmber);
		if( -1 == finded )
			return true;
		// 一番初めに現れる「数値セット」を取得する
		// e.x. 字四ツ塚２０６－１ -> ２０６
		CString strChibanStr;
		{
			int nPos = finded;
			int nLen = strAddr.GetLength();
			for(; nLen > nPos && 0 <= g_strWideNmber.Find(strAddr.GetAt(nPos)); ++nPos );
			strChibanStr = strAddr.Mid( finded, nPos - finded );
		}
		if( strChibanStr.GetLength() > 5 )
			return true;
		strRetChibanCode.Format(_T("%05ld"), _ttol(translate( strChibanStr, _T("０１２３４５６７８９"), _T("0123456789"))) ); // TODO: プロシージャではA～Fも対象となっているが、ここでは不要なはずなので除く
		strAddr.Delete( 0, finded + strChibanStr.GetLength() );
	}

	// 解析後、先頭が番地・番・の・ノ・ハイフンなら除去する
	if( 0 == strAddr.Find( _T("番地") ) )
		strAddr.Delete( 0, 2 );
	if( 0 == strAddr.FindOneOf( _T("番のノ－") ) )
		strAddr.Delete( 0, 1 );

	return true;
}

// 住居番号コード取得関数
bool CAddrServerBase::GetBangoCode( CString& strAddr, const CString& strKenCode, const CString& strShiCode, const CString& strOazaCode, const CString& strAzaCode, const CString& strChibanCode, CString& strRetBangoCode ) const
{
	CADOCursor cur( m_cConn.search( 
		"select bango_code, bango_name from %s.address_point "
		"where  ken_code = '%s' and "
			"   shi_code = '%s' and "
			"  oaza_code = '%s' and "
			"   aza_code = '%s' and "
			"chiban_code = '%s' and "
			"addr_type in ('2', '3') "
		"order by length(bango_name) desc",
			CStringA(m_strOwner), CStringA(strKenCode), CStringA(strShiCode), 
			CStringA(strOazaCode), CStringA(strAzaCode), CStringA(strChibanCode) ) );

	if( S_OK != cur.err() )
	{
		ATLTRACE(_T("failed: err:%s SQL:%s\n"), cur.GetErrorMessage(), m_cConn.getLastQuery() );
		return false;
	}

	strRetBangoCode = _T("xxxx");

	// 文字付き号に一致するものあるか確認
	foreach( CADOCursor::val_type& vals, std::make_pair( cur.begin(), cur.end() ) )
	{
		CString strBangoCode( vals[0].bstrVal );
		CString strBangoKanji( vals[1].bstrVal );
		// 最初に完全一致したものを採用
		// ここに余計な一行あり、削除（bug 8570）
		if( 0 == strAddr.Find(strBangoKanji) )
		{
			strRetBangoCode = strBangoCode;
			strAddr.Delete( 0, strBangoKanji.GetLength() );
			break;
		}
	}

	// 文字付き号と一致しなければ、通常号として解析
	if( strRetBangoCode == _T("xxxx") )
	{
		if( 0 != strAddr.FindOneOf( _T("０１２３４５６７８９") ) )
			return true;

		CString strBangoStr( strAddr.SpanIncluding(_T("０１２３４５６７８９")) );
		if( strBangoStr.GetLength() > 4 )
			return true;
		strRetBangoCode.Format(_T("%04ld"), _ttol(translate( strBangoStr, _T("０１２３４５６７８９"), _T("0123456789"))) ); // TODO: プロシージャではA～Fも対象となっているが、ここでは不要なはずなので除く
		strAddr.Delete( 0, strRetBangoCode.GetLength() );
	}

	return true;
}
