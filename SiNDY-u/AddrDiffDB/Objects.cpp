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
#include "Objects.h"
#include <assert.h>

/**
 * @class	CFeature
 */

// �����擾
inline CComVariant CFeature::getValue( LPCTSTR lpszField ) const
{
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	assert( ipFields != NULL );
	
	LONG lFID;
	ipFields->FindField( CComBSTR(_T(lpszField)), &lFID );

	return getValue(lFID);
}

// �����l�擾
inline CComVariant CFeature::getValue( LONG lFID ) const
{
	CComVariant vaValue;
	m_ipFeature->get_Value( lFID, &vaValue );

	return vaValue;
}

/**
 * @class	CCityAdmin
 */

// �����o�ϐ��ɃZ�b�g
void CCityAdmin::setInfo()
{
	// �Z���R�[�h�쐬
	CComVariant vaValue = getValue( m_rFID.m_lCityCode_FID );
	if( VT_BSTR == vaValue.vt )
		m_strAddrCode = COLE2T( vaValue.bstrVal );
	else
		m_strAddrCode = _T("00000");

	vaValue = getValue( m_rFID.m_lAddrCode_FID );
	if( VT_BSTR == vaValue.vt )
		m_strAddrCode += COLE2T( vaValue.bstrVal );
	else
		m_strAddrCode = _T("00000");

	vaValue = getValue( m_rFID.m_lGaikuFugo_FID );
	if( vaValue.vt != VT_NULL || vaValue.lVal != 0 ){
		TCHAR szGaiku[6] = {0};
		_stprintf( szGaiku, "%05d", vaValue.lVal );
		m_strAddrCode += szGaiku;
		m_lGaikuFugo = vaValue.lVal;
	}
	else{
		m_strAddrCode += _T("00000");
		m_lGaikuFugo = 0;
	}

	// �Z���������
	vaValue = getValue( m_rFID.m_lAddrClass_FID );
	m_lAddrClass = (VT_I4 == vaValue.vt)? vaValue.lVal : -1;
}

/**
 * @class	CGouPoint
 */

// �����o�ϐ��ɃZ�b�g
void CGouPoint::setInfo()
{
	// ���ԍ�
	CComVariant vaValue = getValue( m_rFID.m_lGouNo_FID );
	if( VT_BSTR == vaValue.vt )
		m_strGouNo = COLE2T( vaValue.bstrVal );

	// �g�����ԍ��t���O
	vaValue = getValue( m_rFID.m_lExpGouNo_FID );
	m_lExGouNoF = vaValue.lVal;
}
