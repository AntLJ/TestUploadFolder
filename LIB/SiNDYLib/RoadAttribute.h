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
 * @file RoadAttribute.h
 * @brief <b>CRoadAttributeクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROADATTRIBUTE_H_
#define _ROADATTRIBUTE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace roadattribute {
/**
 * @class CRoadAttribute
 * @brief <b>道路リンク属性名管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.ROADATTRIBUTEテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CRoadAttribute : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CRoadAttribute, roadattribute )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>道路リンクの属性名を取得する</b>\n
	 * @param nLen		[in]	バッファ収納可能文字数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR 道路リンクの属性名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roadattribute::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>道路リンクの属性名のエイリアス名を取得する</b>\n
	 * @param nLen		[in]	バッファ収納可能文字数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR 道路リンクの属性名のエイリアス名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetAliasName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roadattribute::kAliasName, nLen, lpszName );
	}

	/**
	 * @brief <b>道路リンクの属性名IDを取得する</b>\n
	 * @retval 0以上 道路リンクの属性名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetNameID() const {
		return GetLongValueByFieldName( schema::roadattribute::kNameID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>道路リンクの属性名を設定する</b>\n
	 * @param lpcszName		[in]	道路リンクの属性名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roadattribute::kName, lpcszName );
	}

	/**
	 * @brief <b>道路リンクの属性名のエイリアス名を設定する</b>\n
	 * @param lpcszName		[in]	道路リンクの属性名のエイリアス名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetAliasName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roadattribute::kAliasName, lpcszName );
	}

	/**
	 * @brief <b>道路リンクの属性名IDを設定する</b>\n
	 * @param lID			[in]	道路リンクの属性名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetNameID( long lID ) {
		return SetLongValueByFieldName( schema::roadattribute::kNameID, lID );
	}
	//@}
};

} // roadattribute

} // sindy

#endif // _ROADATTRIBUTE_H_
