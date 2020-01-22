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

// �����擾
void CAdminInfo::SetAttr()
{
	// �s���E�Z���R�[�h�擾
	CComVariant vaValue;
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kCityCode), &vaValue );
	m_strAddrCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("00000");
	vaValue.Clear();

	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrCode), &vaValue );
	m_strAddrCode += (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("000000");
	vaValue.Clear();

	// �X�敄��
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kGaikuFugo), &vaValue );
	m_lGaikuFugo = (vaValue.vt == VT_I4)? vaValue.lVal : 0;

	// �g���X�敄���擾
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kExtGaikuFugo), &vaValue );
	m_strExtGaiku = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();
	if( !m_strExtGaiku.IsEmpty() )
	{
		// ���~�擾
		bool bRet = false;
		std::string strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Object( CityAdmin_Layer, GetOID(), m_strExtGaiku, bRet );
		if( !bRet )
			strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Kanji( m_strExtGaiku, bRet );

		if( bRet )
			m_strExtGaikuYomi = addr_func::GetZenkakuData( strTmpYomi.c_str() );	// �O�ׁ̈A�Ō�ɑS�p��(�I�u�W�F�N�g->���~�ł͐������͂��̂܂܊i�[�����\������)
		else
			object_out::ObjectError( m_ipFeature, "���~�擾�Ɏ��s\t%s", m_strExtGaiku );
	}

	// �Z��������ʎ擾
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrClass), &vaValue );
	m_lAddrClass_C = (VT_I4 == vaValue.vt)? vaValue.lVal : -1;

	// �G���A���ʃR�[�h
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAreaClass), &vaValue );
	m_lAreaClass_C = (VT_I4 == vaValue.vt)? vaValue.lVal : -1;

	// ADDRCODE2(Bug8177)
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(city_admin::kAddrCode2), &vaValue );
	m_strOazaAzaCode2 = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("000000");
	vaValue.Clear();
}

// �����擾
void CGouInfo::SetAttr()
{
	// ���ԍ��擾
	CComVariant vaValue;
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kGouNo), &vaValue );
	m_strGouNo = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
	vaValue.Clear();

	// �����t��(#)�Ȃ�A���O��[#]�����������~���擾
	if( m_strGouNo.Find('#') == 0 )
	{
		m_bMoji = true;
		m_strGouNo = m_strGouNo.Mid(1);
	}

	// ���~�擾
	if( !m_strGouNo.IsEmpty() && m_strGouNo != _T("*") )
	{
		bool bRet = false;
		std::string strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Object( GouPoint_Layer, GetOID(), m_strGouNo, bRet );
		if( !bRet )
			strTmpYomi = g_cYomiHelper.GetYomiFromCharAddr_Kanji( m_strGouNo, bRet );

		// �ǂݎ擾���s���͋�̂܂܂ɂ���
		if( bRet )
			m_strGouNoYomi = addr_func::GetZenkakuData( strTmpYomi.c_str() );	// �O�ׁ̈A�Ō�ɑS�p��(�I�u�W�F�N�g->���~�ł͐������͂��̂܂܊i�[�����\������)
		else
			object_out::ObjectError( m_ipFeature, "���~�擾�Ɏ��s\t%s", m_strGouNo );
	}
	
	// ���ԍ�����/���~���g�[�N���ɕ���
	Tokenize( m_strGouNo, L"-", m_listTokenName );
	Tokenize( m_strGouNoYomi, L"�|", m_listTokenYomi );	// ���~�擾����[-]��[�|]�ɂȂ�̂ŁA��؂蕶��������ɍ��킹��

	// ���^�C�v�擾
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kGouType), &vaValue );
	m_lGouType = (VT_I4 == vaValue.vt)? vaValue.lVal : 0;

	// �g�����ԍ��t���O
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kExpGouNo), &vaValue );
	m_bExGouF = (VT_I4 == vaValue.vt && vaValue.lVal == 1);

	// �D��t���O
	m_ipFeature->get_Value( m_cFIDMap.GetFieldIndex(gou_point::kPriority), &vaValue );
	m_bPriorF = (VT_I4 == vaValue.vt && vaValue.lVal == 1);
}

// �g�[�N���擾�֐�
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
