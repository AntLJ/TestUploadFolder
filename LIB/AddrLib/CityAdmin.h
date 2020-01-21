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

#pragma once

#include <SiNDYLib/Feature.h>

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

class CCityAdmin;
typedef boost::shared_ptr<CCityAdmin> CSPCityAdmin;

/**
 * @class	CCityAdmin
 * @brief	都市地図行政界クラス
 */
class CCityAdmin : public CFeature
{
public:
	CCityAdmin()
	{
		Init();
		m_emTableType = sindyTableType::city_admin;
	}
	virtual ~CCityAdmin(){ DeleteCache(); }
	CCityAdmin( const CCityAdmin& obj )
	{
		Init();
		*this = obj;
	}
	CCityAdmin( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::city_admin, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CCityAdmin& operator=( const CCityAdmin& obj )
	{
		obj.CopyTo( *this );
		m_strAdminCode = obj.m_strAdminCode;
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CCityAdmin& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::city_admin );
		return *this;
	}	

	/**
	 * @brief	<b>市区町村コード取得</b>
	 * @return	市区町村コード
	 */
	CString GetCityCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kCityCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>大字・字コード</b>
	 * @return	大字・字コード
	 */
	CString GetOazaAzaCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>大字・字コード2</b>
	 * @note	Bug8041
	 * @return	大字・字コード
	 */
	CString GetOazaAzaCode2() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kAddrCode2, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>街区符号取得</b>
	 * @return	街区符号
	 */
	long GetGaikuFugo() const
	{
		return GetLongValueByFieldName(city_admin::kGaikuFugo, 0);
	}

	/**
	 * @brief	<b>拡張街区符号取得</b>
	 * @return	拡張街区符号
	 */
	CString GetExGaikuFugo() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(city_admin::kExtGaikuFugo, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>拡張街区符号ヨミ取得</b>
	 * @return	拡張街区符号ヨミ
	 */
	CString GetExGaikuFugoYomi() const;

	/**
	 * @brief	<b>住所整備種別取得</b>
	 * @return	住所整備種別
	 */
	city_admin::addr_class::ECode GetAddrClass() const
	{
		return static_cast<city_admin::addr_class::ECode>(GetLongValueByFieldName(city_admin::kAddrClass, 0));
	}

	/**
	 * @brief	<b>エリア判別コード取得</b>
	 * @return	エリア判別コード
	 */
	city_admin::area_class::ECode GetAreaClass() const
	{
		return static_cast<city_admin::area_class::ECode>(GetLongValueByFieldName(city_admin::kAreaClass, 0));
	}

	/**
	 * @brief	<b>住所変換モード取得</b>
	 * @return	住所変換モード
	 */
	city_admin::conv_type::ECode GetConvType() const
	{
		return static_cast<city_admin::conv_type::ECode>(GetLongValueByFieldName(city_admin::kConvType, 0));
	}

	/**
	 * @brief	<b>住所コード取得</b>
	 * @note	行政界で表現できる限りの住所コードを返す
	 * @param	bChar	[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @return	住所コード
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false ) const;

	/**
	 * @brief	<b>住所コード2取得</b>
	 * @note	行政界で表現できる限りの住所コード(通称住所)を返す
	 * @param	bChar	[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @return	住所コード
	 */
	CString GetAddrCode2( bool bChar = false, bool bCodeDBSaved = false ) const;

	/**
	 * @brief	<b>住所名取得</b>
	 * @note	行政界が表現できる桁レベルでの住所名を返す
	 * @return	住所名
	 */
	CString GetAddrName() const;

	/**
	 * @brief	<b>住所名取得</b>
	 * @note	行政界が表現できる桁レベルでの住所名(通称住所)を返す
	 * @return	住所名
	 */
	CString GetAddrName2() const;

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * CRow::DeleteCacheをオーバロード
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strAdminCode = _T("");
		m_strAddrName = _T("");
	}

private:
	/**
	 * @brief <b>11桁コードに地番コードを追加する</b>
	 * @note  行政界で表現できる限りの住所コード(通称住所)を返す
	 * @param strAdminCode [in, out] 処理対象の11桁コード
	 * @param bChar	[in, optional] 文字付き住所考慮する？
	          (trueにすると、文字付き住所考慮した結果が返ります)
	 * @param bCodeDBSaved	[in, optional]
	          文字付き住所コード変換DBに存在しない名称のとき保存した上で
	          コードを新規発行する？(trueにすると、コード変換DBに更新処理が入る)
	 */
	void AppendPntCode(
		CString& strAdminCode,
		bool bChar = false,
		bool bCodeDBSaved = false ) const;

protected:

	mutable CString	m_strAdminCode;		//!< 行政界コード(キャッシュ用)
	mutable CString	m_strAdminCode2;		//!< 行政界コード(通称住所)(キャッシュ用)
	mutable CString	m_strAddrName;		//!< 住所名称(キャッシュ用)
	mutable CString	m_strAddrName2;		//!< 住所名称(通称住所)(キャッシュ用)
};

} // namespace addr
