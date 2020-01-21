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
 * @class	CKLinkPoint
 * @brief	家形リンクポイント管理クラス
 */
class CKLinkPoint : public CAddrBasePoint
{
public:
	CKLinkPoint()
	{
		Init();
		m_emTableType = sindyTableType::klink_point;
	}
	virtual ~CKLinkPoint(){ DeleteCache(); }
	CKLinkPoint( const CKLinkPoint& obj )
	{
		Init();
		*this = obj;
	}
	CKLinkPoint( _IRow* lp, sindyTableType::ECode emTableType = sindyTableType::klink_point, bool bCreated = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() )
	{
		Init();
		SetObject( lp, emTableType, bCreated, spFields, spNameString );
	}
	CKLinkPoint& operator=( const CKLinkPoint& obj )
	{
		obj.CopyTo( *this );
		m_strAddrName = obj.m_strAddrName;
		return *this;
	}
	CKLinkPoint& operator=( _IRow* lp )
	{
		SetObject( lp, sindyTableType::klink_point );
		return *this;
	}

	/**
	 * @brief	<b>初期22桁住所コード取得</b>
	 * @return	初期22桁住所コード
	 */
	CString GetAddrCodeOrg() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kAddrCodeOrg, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>最新22桁住所コード取得</b>
   	 * @param	bChar			[in, optional]	文字付き住所考慮する？(trueにすると、文字付き住所考慮した結果が返ります)
	 * @param	bCodeDBSaved	[in, optional]	文字付き住所コード変換DBに存在しな名称のとき保存した上でコードを新規発行する？(trueにすると、コード変換DBに更新処理が入ります)
	 * @note	家形リンクポイントでは、上記引数は全て使用しません
	 * @return	最新22桁住所コード
	 */
	CString GetAddrCode( bool bChar = false, bool bCodeDBSaved = false )
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kAddrCode, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>タウンページ方書名称取得</b>
	 * @return	タウンページ方書名称
	 */
	CString GetKatagakiName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kKatagakiName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>タウンページ掲載名称取得</b>
	 * @return	タウンページ掲載名称
	 */
	CString GetKeisaiName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kKeisaiName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>情報提供元コード取得</b>
	 * @return	情報提供元コード
	 */
	klink_point::info_src::ECode GetInfoSrc() const{	return static_cast<klink_point::info_src::ECode>(GetLongValueByFieldName(klink_point::kInfoSrc, 0)); }

	/**
	 * @brief	<b>入力用座標精度コード取得</b>
	 * @return	入力用座標精度供元コード
	 */
	klink_point::input_seido::ECode GetInputSeido() const{	return static_cast<klink_point::input_seido::ECode>(GetLongValueByFieldName(klink_point::kInputSeido, 0)); }

	/**
	 * @brief	<b>リリース用座標精度コード取得</b>
	 * @return	リリース用座標精度供元コード
	 */
	klink_point::release_seido::ECode GetReleaseSeido() const{	return static_cast<klink_point::release_seido::ECode>(GetLongValueByFieldName(klink_point::kReleaseSeido, 0));	}

	/**
	 * @brief	<b>ビル名称取得</b>
	 * @return	ビル名称
	 */
	CString GetBuildName() const
	{
		CComVariant vaValue;
		return (sindyErr_NoErr == GetValueByFieldName(klink_point::kBuildName, vaValue) && VT_BSTR == vaValue.vt)?
				CString(vaValue.bstrVal) : _T("");
	}

	/**
	 * @brief	<b>ビル情報提供元コード取得</b>
	 * @return	ビル情報提供元コード
	 */
	klink_point::buildinfo_src::ECode GetBuildInfoSrc() const{	return static_cast<klink_point::buildinfo_src::ECode>(GetLongValueByFieldName(klink_point::kBuildInfoSrc, 0)); }

	/**
	 * @brief	<b>ビル最上階数取得</b>
	 * @return	ビル最上階数
	 */
	long GetMaxFloors() const{	return GetLongValueByFieldName(klink_point::kMaxFloors, 0); }

	/**
	 * @brief	<b>テナント階数取得</b>
	 * @return	テナント階数
	 */
	long GetFloor() const{	return GetLongValueByFieldName(klink_point::kFloor, 0); }

	// 11桁不一致フラグは、メンテされていないので使用不可(毎回独自で確認している)
			
};

}	// namespace addr
