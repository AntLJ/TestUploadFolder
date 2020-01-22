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
 * @class	CExTP
 */

// �����o�ϐ��ɃZ�b�g
void CExTP::setInfo()
{
	USES_CONVERSION;
	
	// �Z���R�[�h
	CComVariant vaValue = getValue( _T("ADDRCODE") );
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode = OLE2A( vaValue.bstrVal );
	else
		m_strAddrCode = "00000000000000000000";

	// �������ʃR�[�h
	vaValue = getValue( _T("POINTSTAT_C") );
	if( vaValue.vt == VT_I4 )
		m_ePointStatC = static_cast<POINTSTAT_C>(vaValue.lVal);

	// �Z����
	vaValue = getValue( _T("ADDRNAME") );
	if( vaValue.vt == VT_BSTR )
		m_strAddrName = OLE2A( vaValue.bstrVal );

	// ��������
	vaValue = getValue( _T("KATAGAKINAME") );
	if( vaValue.vt == VT_BSTR )
		m_strKatagaki = OLE2A( vaValue.bstrVal );

	// �f�ږ���
	vaValue = getValue( _T("KEISAINAME") );
	if( vaValue.vt == VT_BSTR )
		m_strKeisai = OLE2A( vaValue.bstrVal );

	// ���\�[�X
	vaValue = getValue( _T("SOURCE") );
	if( vaValue.vt == VT_BSTR )
		m_strSource = OLE2A( vaValue.bstrVal );

	// �Z�������|�C���g���t���O
	vaValue = getValue( _T("ADDPOINT_F") );
	m_lAddPointF = vaValue.lVal;
}

// �������ʃh���C���擾
const std::string CExTP::getPointStatDomain() const
{
	// ��������
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	ipFields->FindField( CComBSTR(_T("POINTSTAT_C")), &lFID );
	IFieldPtr ipField;
	ipFields->get_Field( lFID, &ipField );
	if( ipField == NULL )
		return "";

	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	if( ipDomain == NULL )
		return "";

	ICodedValueDomainPtr ipCVDoamin( ipDomain );
	if( ipCVDoamin == NULL )
		return "";

	USES_CONVERSION;

	LONG lDomainCount = 0;
	ipCVDoamin->get_CodeCount( &lDomainCount );
	for( LONG i=0;i<lDomainCount;++i )
	{
		CComVariant vaValue;
		ipCVDoamin->get_Value( i, &vaValue );
		if( static_cast<LONG>(m_ePointStatC) == vaValue.lVal ){
			CComBSTR bstr;
			ipCVDoamin->get_Name( i, &bstr );
			return OLE2A(bstr);
		}
	}
	
	return "";
}

/**
 * @class	CCityAdmin
 */

// �����o�ϐ��ɃZ�b�g
void CCityAdmin::setInfo()
{
	USES_CONVERSION;
	// �Z���R�[�h�쐬
	CComVariant vaValue = getValue( _T("CITYCODE") );
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode = OLE2A( vaValue.bstrVal );
	else
		m_strAddrCode = "00000";

	vaValue = getValue( _T("ADDRCODE") );
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode += OLE2A( vaValue.bstrVal );
	else
		m_strAddrCode += "000000";

	vaValue = getValue( _T("GAIKUFUGO") );
	if( vaValue.vt != VT_NULL || vaValue.lVal != 0 ){
		char szGaiku[6] = {0};
		sprintf( szGaiku, "%05d", vaValue.lVal );
		m_strAddrCode += szGaiku;
		m_lGaikuFugo = vaValue.lVal;
	}
	else
		m_strAddrCode += "00000";

	// �g���X�敄��
	vaValue = getValue( _T("EXTGAIKUFUGO") );
	if( vaValue.vt == VT_BSTR )
		m_strExGaikuFugo = OLE2A( vaValue.bstrVal );

	// �Z���������
	vaValue = getValue( _T("ADDRCLASS_C") );
	if( vaValue.vt == VT_I4 )
		m_eAddrClassC = static_cast<ADDRCLASS_C>(vaValue.lVal);
}

/**
 * @class	CSite
 */

// �w�i��ʃh���C���擾
const std::string CSite::getBGClassDomain()
{
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	switch( m_szScale )
	{
	case 'C':
		ipFields->FindField( CComBSTR(_T("BGCLASS_C")), &lFID );
		break;
	case 'B':
		ipFields->FindField( CComBSTR(_T("SC4BGCLASS_C")), &lFID );
		break;
	default:
		return "";
	}

	IFieldPtr ipField;
	ipFields->get_Field( lFID, &ipField );
	if( ipField == NULL )
		return "";

	IDomainPtr ipDomain;
	ipField->get_Domain( &ipDomain );
	if( ipDomain == NULL )
		return "";

	ICodedValueDomainPtr ipCVDoamin( ipDomain );
	if( ipCVDoamin == NULL )
		return "";

	USES_CONVERSION;

	LONG lDomainCount = 0;
	ipCVDoamin->get_CodeCount( &lDomainCount );
	for( LONG i=0;i<lDomainCount;++i )
	{
		CComVariant vaValue;
		ipCVDoamin->get_Value( i, &vaValue );
		if( m_lBGClassC == vaValue.lVal ){
			CComBSTR bstr;
			ipCVDoamin->get_Name( i, &bstr );
			return OLE2A(bstr);
		}
	}
	
	return "";
}

// �����o�ϐ��ɃZ�b�g
void CSite::setInfo()
{
	USES_CONVERSION;

	// �w�i��ʁE��ʖ��擾
	CComVariant vaValue;
	switch( m_szScale )
	{
	case 'C':
		vaValue = getValue( _T("BGCLASS_C") );
		m_lBGClassC = vaValue.lVal;

		break;
	case 'B':
		vaValue = getValue( _T("SC4BGCLASS_C") );
		m_lBGClassC = vaValue.lVal;

		break;
	default:
		break;
	}
}
