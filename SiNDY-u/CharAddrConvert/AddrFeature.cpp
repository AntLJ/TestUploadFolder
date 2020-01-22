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
#include "AddrFeature.h"

// 属性取得
void CAdminInfo::SetAttr()
{
	// 行政界住所コード取得
	CComVariant vaValue;
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kCityCode), &vaValue );
	m_strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("00000");
	vaValue.Clear();

	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrCode), &vaValue );
	m_strAddrCode += (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("000000");
	vaValue.Clear();

	// 街区符号
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kGaikuFugo), &vaValue );
	m_lGaikuFugo = (vaValue.vt == VT_I4)? vaValue.lVal : 0;

	// 拡張街区符号取得
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kExtGaikuFugo), &vaValue );
	m_strExtGaiku = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();
	if( !m_strExtGaiku.IsEmpty() )
	{
		// ヨミ取得
		bool bRet = false;
		std::string strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Object( CityAdmin_Layer, GetOID(), m_strExtGaiku, bRet );
		if( !bRet )
			strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Kanji( m_strExtGaiku, bRet );

		if( bRet )
			m_strExtGaikuYomi = addr_func::GetZenkakuData( strTmpYomi.c_str() );	// 念の為、最後に全角化(オブジェクト->ヨミでは数字等はそのまま格納される可能性ある)
		else
			object_out::ObjectError( m_ipFeature, "ヨミ取得に失敗\t%s", m_strExtGaiku );
	}

	// 住所整備種別取得
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrClass), &vaValue );
	m_lAddrClass_C = (VT_I4 == vaValue.vt)? vaValue.lVal : -1;

	// エリア判別コード
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAreaClass), &vaValue );
	m_lAreaClass_C = (VT_I4 == vaValue.vt)? vaValue.lVal : -1;

	// ADDRCODE2(Bug8177)
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrCode2), &vaValue );
	m_strOazaAzaCode2 = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("000000");
	vaValue.Clear();
}

// 属性取得
void CGouInfo::SetAttr()
{
	// 号番号取得
	CComVariant vaValue;
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kGouNo), &vaValue );
	m_strGouNo = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();

	// 文字付き(#)なら、事前に[#]を除去かつヨミを取得
	if( m_strGouNo.Find('#') == 0 )
	{
		m_bMoji = true;
		m_strGouNo = m_strGouNo.Mid(1);
	}

	// ヨミ取得
	if( !m_strGouNo.IsEmpty() && m_strGouNo != _T("*") )
	{
		bool bRet = false;
		std::string strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Object( GouPoint_Layer, GetOID(), m_strGouNo, bRet );
		if( !bRet )
			strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Kanji( m_strGouNo, bRet );

		// 読み取得失敗時は空のままにする
		if( bRet )
			m_strGouNoYomi = addr_func::GetZenkakuData( strTmpYomi.c_str() );	// 念の為、最後に全角化(オブジェクト->ヨミでは数字等はそのまま格納される可能性ある)
		else
			object_out::ObjectError( m_ipFeature, "ヨミ取得に失敗\t%s", m_strGouNo );
	}
	
	// 号番号名称/ヨミをトークンに分解
	Tokenize( m_strGouNo, L"-", m_listTokenName );
	Tokenize( m_strGouNoYomi, L"－", m_listTokenYomi );	// ヨミ取得時に[-]が[－]になるので、区切り文字もそれに合わせる

	// 号タイプ取得
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kGouType), &vaValue );
	m_lGouType = (VT_I4 == vaValue.vt)? vaValue.lVal : 0;

	// 拡張号番号フラグ
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kExpGouNo), &vaValue );
	m_bExGouF = (VT_I4 == vaValue.vt && vaValue.lVal == 1);

	// 優先フラグ
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kPriority), &vaValue );
	m_bPriorF = (VT_I4 == vaValue.vt && vaValue.lVal == 1);
}

// トークン取得関数
CString CGouInfo::GetToken( const TOKENLIST& listToken, int iStart, int iEnd /* = -1 */ ) const
{
	if( iStart >= 0 && iStart < GetTokenCount() )
	{
		TOKENLIST::const_iterator itrStart = listToken.begin();
		std::advance( itrStart, iStart );

		TOKENLIST::const_iterator itrEnd = listToken.begin();
		if( iStart < iEnd && iEnd < GetTokenCount() )
			std::advance( itrEnd, iEnd );
		else
			itrEnd = listToken.end();

		CString strGouNo( *itrStart );
		++itrStart;
		while( itrStart != itrEnd )
		{
			strGouNo += _T("-") + *itrStart;
			++itrStart;
		}
		return strGouNo;
	}
	else
		return _T("");	
}
