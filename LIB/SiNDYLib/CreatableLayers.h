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

/**
 * @file CreatableLayers.h
 * @brief <b>CCreatableLayersクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _CREATABLELAYERS_H_
#define _CREATABLELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"
#include "AutoValue.h"

namespace sindy {
namespace creatablelayers {
/**
 * @class CCreatableLayers
 * @brief <b>作成可能レイヤ管理テーブルクラス（ルール用）</b>\n
 * このクラスはREFERENCE.MOVABLELAYERSテーブルのラップクラスです。
 */
class SINDYLIB_API CCreatableLayers : public CRow
{
public:
	ROWCLASS_CONSTRUCTOR( CCreatableLayers, creatablelayers )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>作業目的IDを取得する</b>\n
	 * @retval 0以上 作業目的ID
	 * @retval -1 エラー又はNULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::creatablelayers::kPurposeID, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::creatablelayers::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>自動作成フラグを取得する</b>\n
	 * @retval false NULL、エラー、0
	 * @retval true 0以外
	 */
	bool GetAutoFlag() const {
		return GetBoolValueByFieldName( schema::creatablelayers::kAutoFlag, false );
	}

	/**
	 * @brief <b>自動作成属性値を取得する</b>\n
	 * @note なるべくCAutoValueを返すほうを使用してください。
	 * @param nLen		[in]	バッファ格納可能文字列数（_AUTOVALUE_MAX_LENを使用してください）
	 * @param lpszValue	[out]	格納バッファ
	 * @retval LPCTSTR 自動作成属性値
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetAutoValue( int nLen, LPTSTR lpszValue ) const {
		return GetStringValueByFieldName( schema::creatablelayers::kAutoValue, nLen, lpszValue );
	}
	/**
	 * @brief <b>自動作成属性値を取得する</b>\n
	 * @return CAutoValue
	 */
	const CAutoValue& GetAutoValue() const {
		return m_cAutoValue;
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>作業目的IDを設定する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::creatablelayers::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lLayerNameID	[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lLayerNameID ) {
		return SetLongValueByFieldName( schema::creatablelayers::kLayerNameID, lLayerNameID );
	}

	/**
	 * @brief <b>自動作成フラグを設定する</b>\n
	 * @param bAutoFlag		[in]	自動作成フラグ
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoFlag( bool bAutoFlag ) {
		return SetBoolValueByFieldName( schema::creatablelayers::kAutoFlag, bAutoFlag );
	}

	/**
	 * @brief <b>自動作成属性値を設定する</b>\n
	 * @param lpcszValue	[in]	自動作成属性値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAutoValue( LPCTSTR lpcszValue ) {
		m_cAutoValue.Init( lpcszValue );
		return SetStringValueByFieldName( schema::creatablelayers::kAutoValue, lpcszValue );
	}
	//@}
private:
	CAutoValue m_cAutoValue; //!< 自動付与属性
};

} // creatablelayers

} // sindy

#endif // _CREATABLELAYERS_H_
