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

#include "AddrBasePoint.h"

namespace addr
{
using namespace sindy;
using namespace sindy::errorcode;
using namespace sindy::schema;

/**
 * @class	CBaseRepPoint
 * @brief	中縮ADDRポイント管理クラス
 */
class CBaseRepPoint : public CAddrBasePoint
{
public:
	CBaseRepPoint()
	{
		Init();
		m_emTableType = sindyTableType::base_rep_point;
	}
	virtual ~CBaseRepPoint(){ DeleteCache(); }
	CBaseRepPoint( const CBaseRepPoint& obj )
	{
		Init();
		*this = obj;
	}
	CBaseRepPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::base_rep_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CBaseRepPoint& operator=( const CBaseRepPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CBaseRepPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::base_rep_point );
		return *this;
	}

	/**
	 * @brief	<b>住所コード取得</b>
  	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @note	中縮ADDRポイントでは、上記引数は全て使用しません
	 * @return	住所コード
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>住所名称取得</b>
 	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
 	 * @note	中縮ADDRポイントでは、上記引数は全て使用しません
	 * @return	住所名称
	 */
	CString GetAddrName( bool bChar = false );

	/**
	 * @brief	<b>代表点名称取得</b>
	 * @return	代表点名称
	 */
	CString GetAdminName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kAdminName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>郵便番号取得</b>
	 * @note	Bug8043
	 * @return	代表点名称
	 */
	CString GetZipCode() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(base_rep_point::kZipCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}
};

}	// namespace addr
