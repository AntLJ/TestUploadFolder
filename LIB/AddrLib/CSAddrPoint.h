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
 * @class	CCSAddrPoint
 * @brief	CSポイント管理クラス
 */
class CCSAddrPoint : public CAddrBasePoint
{
public:
	CCSAddrPoint()
	{
		Init();
		m_emTableType = sindyTableType::cs_addr_point;
	}
	virtual ~CCSAddrPoint(){ DeleteCache(); }
	CCSAddrPoint( const CCSAddrPoint& obj )
	{
		Init();
		*this = obj;
	}
	CCSAddrPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::cs_addr_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CCSAddrPoint& operator=( const CCSAddrPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CCSAddrPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::cs_addr_point );
		return *this;
	}

	/**
	 * @brief	<b>住所コード取得</b>
  	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @note	CSポイントでは、上記引数は全て使用しません
	 * @return	住所コード
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(cs_addr_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief <b>キャッシュを削除する</b>\n
	 * CRow::DeleteCacheをオーバロード
	 */
	void DeleteCache( bool bDeleteContainerCache = false )
	{
		m_strAddrName = _T("");

		// コンテナのキャッシュを削除
		if( bDeleteContainerCache )
			CContainer::DeleteCache( bDeleteContainerCache );
	}

private:

	mutable CString	m_strAddrName;	//!< キャッシュ用

};

}	// namespace addr
