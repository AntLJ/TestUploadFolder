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
#include "sindy/schema/map.h"

/**
 * @class	CFeature
 */


// 属性取得
inline CComVariant CFeature::getValue( LPCTSTR lpszField ) const
{
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	assert( ipFields != NULL );
	
	LONG lFID;
	ipFields->FindField( CComBSTR(_T(lpszField)), &lFID );

	return getValue(lFID);
}

// 属性値取得
inline CComVariant CFeature::getValue( LONG lFID ) const
{
	CComVariant vaValue;
	m_ipFeature->get_Value( lFID, &vaValue );

	return vaValue;
}

/**
 * @class	CTelTP
 */

// メンバ変数にセット
void CTelTP::setInfo()
{
	// 住所コード
	CComVariant vaValue = getValue( sindy::schema::teltp_point::kAddrCode );
	
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode = COLE2T( vaValue.bstrVal );
	else
		m_strAddrCode = "00000000000000000000";
	vaValue.Clear();

	// 調査結果コード
	vaValue = getValue( sindy::schema::teltp_point::kPointStat );
	if( vaValue.vt == VT_I4 )
		m_ePointStatC = static_cast<POINTSTAT_C>(vaValue.lVal);
	vaValue.Clear();

	// 住所名
	vaValue = getValue( sindy::schema::teltp_point::kAddrName );
	if( vaValue.vt == VT_BSTR )
		m_strAddrName = COLE2T( vaValue.bstrVal );
	vaValue.Clear();

	// 方書名称
	vaValue = getValue( sindy::schema::teltp_point::kKatagakiName );
	if( vaValue.vt == VT_BSTR )
		m_strKatagaki = COLE2T( vaValue.bstrVal );
	vaValue.Clear();

	// 掲載名称
	vaValue = getValue( sindy::schema::teltp_point::kKeisaiName );
	if( vaValue.vt == VT_BSTR )
		m_strKeisai = COLE2T( vaValue.bstrVal );
	vaValue.Clear();

	// 情報ソース
	vaValue = getValue( sindy::schema::teltp_point::kSource );
	if( vaValue.vt == VT_BSTR )
		m_strSource = COLE2T( vaValue.bstrVal );
	vaValue.Clear();

	// 電話番号
	vaValue = getValue( sindy::schema::teltp_point::kTelNum );
	if( vaValue.vt == VT_BSTR )
		m_strTelNum = COLE2T( vaValue.bstrVal );
	vaValue.Clear();
/*
	// TelTPのXY座標
	IPointPtr ipPoint( m_ipGeo );
	ipPoint->get_X( &dX_TelTP );
	ipPoint->get_Y( &dY_TelTP );
*/
	// 座標付与種別
	vaValue = getValue( sindy::schema::teltp_point::kAddXY );
	if( vaValue.vt == VT_I4 )
		m_eAddXYC = static_cast<ADDXY_C>(vaValue.lVal);
	vaValue.Clear();

	// シーケンス（Bug9295 追加）
	vaValue = getValue( sindy::schema::teltp_point::kSequence );
	if( VT_I4 == vaValue.vt )
		m_lSequence = vaValue.lVal;
	vaValue.Clear();

}

// 調査結果ドメイン取得
const std::string CTelTP::getPointStatDomain() const
{
	// 調査結果
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	ipFields->FindField( CComBSTR(sindy::schema::teltp_point::kPointStat), &lFID );
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

// 座標付与種別ドメイン取得
const std::string CTelTP::getAddXYDomain() const
{
	// 座標付与種別
	IFieldsPtr ipFields;
	m_ipFeature->get_Fields( &ipFields );
	if( ipFields == NULL )
		return "";
	LONG lFID;
	ipFields->FindField( CComBSTR(sindy::schema::teltp_point::kAddXY), &lFID );
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
		if( static_cast<LONG>(m_eAddXYC) == vaValue.lVal ){
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

// メンバ変数にセット
void CCityAdmin::setInfo()
{
	// 住所コード作成
	CComVariant vaValue = getValue( sindy::schema::city_admin::kCityCode );
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode = COLE2T( vaValue.bstrVal );
	else
		m_strAddrCode = "00000";
	vaValue.Clear();

	vaValue = getValue( sindy::schema::city_admin::kAddrCode );
	if( vaValue.vt == VT_BSTR )
		m_strAddrCode += COLE2T( vaValue.bstrVal );
	else
		m_strAddrCode += "000000";
	vaValue.Clear();

	// 住所整備種別
	vaValue = getValue( sindy::schema::city_admin::kAddrClass );
	if( vaValue.vt == VT_I4 )
		m_eAddrClassC = static_cast<ADDRCLASS_C>(vaValue.lVal);
}

/**
 * @class	CSite
 */

// 背景種別ドメイン取得
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
		ipFields->FindField( CComBSTR(sindy::schema::city_site::kBgClass), &lFID );
		break;
	case 'B':
		ipFields->FindField( CComBSTR(sindy::schema::base_site::kSC4BgClass), &lFID );
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

// メンバ変数にセット
void CSite::setInfo()
{
	// 背景種別・種別名取得
	CComVariant vaValue;
	switch( m_szScale )
	{
	case 'C':
		vaValue = getValue( sindy::schema::city_site::kBgClass );
		m_lBGClassC = vaValue.lVal;

		break;
	case 'B':
		vaValue = getValue( sindy::schema::base_site::kSC4BgClass );
		m_lBGClassC = vaValue.lVal;

		break;
	default:
		break;
	}
}
