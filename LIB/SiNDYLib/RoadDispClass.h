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
 * @file RoadDispClass.h
 * @brief <b>CRoadDispClassクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ROADDISPCLASS_H_
#define _ROADDISPCLASS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace roaddispclass {
/**
 * @class CRoadDispClass
 * @brief <b>道路リンクの表示種別管理レコードクラス（ルール用）</b>\n
 * このクラスは、REFERENCE.ROADDISPCLASSテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CRoadDispClass : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CRoadDispClass, roaddispclass )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>道路種別名を取得する</b>\n
	 * @param nLen		[in]	格納バッファ文字列数
	 * @param lpszName	[out]	格納バッファ
	 * @retval LPCTSTR 道路種別名
	 * @retval NULL エラー又はNULL
	 */
	LPCTSTR GetName( int nLen, LPTSTR lpszName ) const {
		return GetStringValueByFieldName( schema::roaddispclass::kName, nLen, lpszName );
	}

	/**
	 * @brief <b>道路種別名IDを取得する</b>\n
	 * @retval 0以上 道路種別名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetNameID() const {
		return GetLongValueByFieldName( schema::roaddispclass::kNameID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>道路種別名を設定する</b>\n
	 * @param lpcszName	[in]	道路種別名
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetName( LPCTSTR lpcszName ) {
		return SetStringValueByFieldName( schema::roaddispclass::kName, lpcszName );
	}

	/**
	 * @brief <b>道路種別名IDを設定する</b>\n
	 * @param lID	[in]	道路種別名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetNameID( long lID ) {
		return SetLongValueByFieldName( schema::roaddispclass::kNameID, lID );
	}
	//@}
};

} // roaddispclass

} // sindy

#endif // _ROADDISPCLASS_H_
