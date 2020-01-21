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
#include "CityAdmin.h"
#include "CodeConverter.h"
#include "YomiConverter.h"
#include "AreaCodeHelper.h"
#include "AddrGlobals.h"
namespace addr
{

// 住所コード取得
CString CCityAdmin::GetAddrCode( bool bChar /* = false */, bool bCodeDBSaved /* = false */ ) const
{
	// 取得は、一度でOK
	if (!m_strAdminCode.IsEmpty())
		return m_strAdminCode;

	m_strAdminCode = GetCityCode() + GetOazaAzaCode();
	AppendPntCode(m_strAdminCode, bChar, bCodeDBSaved);
	return m_strAdminCode;
}

CString CCityAdmin::GetAddrCode2( bool bChar /* = false */, bool bCodeDBSaved /* = false */ ) const
{
	if (!m_strAdminCode2.IsEmpty())
		return m_strAdminCode2;

	m_strAdminCode2 = GetCityCode() + GetOazaAzaCode2();
	AppendPntCode(m_strAdminCode2, bChar, bCodeDBSaved);
	return m_strAdminCode2;
}

void CCityAdmin::AppendPntCode( CString& strAdminCode, bool bChar /* = false */, bool bCodeDBSaved /* = false */ ) const
{
	// 文字付き考慮＋([拡張街区符号 != null] or [街区符号 > 99999])なら文字付き住所コード割り当て
	long lGaiku = GetGaikuFugo();
	if( bChar && (!GetExGaikuFugo().IsEmpty() || lGaiku > 99999) )
	{
		// 文字付き住所対応地番コード取得
		CString strPntCode = CCodeConverter::GetCodeConverter().GetPntCode(strAdminCode.Left(11), GetExGaikuFugo(), bCodeDBSaved);
		if( !strPntCode.IsEmpty() )
			strAdminCode += strPntCode;
	}
	else
	{
		// 通常のコード生成
		if( lGaiku > 0 )
			strAdminCode.AppendFormat( _T("%05ld"), GetGaikuFugo() );
	}
}

// 拡張街区符号ヨミ取得
CString CCityAdmin::GetExGaikuFugoYomi() const
{
	CString strExGaiku( GetExGaikuFugo() );

	// 最初にオブジェクトヨミテーブル→漢字ヨミテーブルの順に取得を試みる
	bool bRet = false;
	CString strYomi = CYomiConverter::GetYomiConverter().GetYomiFromCharAddr_Object( CityAdmin_Layer, GetOID(), strExGaiku, bRet ); 
	if( !bRet )
		strYomi = CYomiConverter::GetYomiConverter().GetYomiFromCharAddr_Kanji( strExGaiku, bRet );

	return bRet? strYomi : _T("");
}

// 住所名取得
CString CCityAdmin::GetAddrName() const
{
	// キャッシュなければＤＢ検索
	if( m_strAddrName.IsEmpty() )
	{
		m_strAddrName = g_cAreaCodeHelper.IsConnected()? g_cAreaCodeHelper.GetAddrKanji(GetAddrCode()) : _T("");
		if( !m_strAddrName.IsEmpty() )
		{
			if( GetGaikuFugo() > 0 )
				m_strAddrName.AppendFormat( _T("%d"), GetGaikuFugo() );
			else if( !GetExGaikuFugo().IsEmpty() )
				m_strAddrName += GetExGaikuFugo();

			m_strAddrName = GetZenkakuData( m_strAddrName );
		}		
	}
	return m_strAddrName;
}

// 住所名(通称住所)取得
CString CCityAdmin::GetAddrName2() const
{
	// キャッシュなければＤＢ検索
	if (m_strAddrName2.IsEmpty())
	{
		m_strAddrName2 = g_cAreaCodeHelper.IsConnected() ? g_cAreaCodeHelper.GetAddrKanji(GetAddrCode2()) : _T("");
		if (!m_strAddrName2.IsEmpty())
		{
			if (GetGaikuFugo() > 0)
				m_strAddrName2.AppendFormat(_T("%d"), GetGaikuFugo());
			else if (!GetExGaikuFugo().IsEmpty())
				m_strAddrName2 += GetExGaikuFugo();

			m_strAddrName2 = GetZenkakuData(m_strAddrName2);
		}
	}
	return m_strAddrName2;
}

} // namespace addr
