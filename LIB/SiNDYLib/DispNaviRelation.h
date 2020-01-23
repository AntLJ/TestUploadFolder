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
 * @file DispNaviRelation.h
 * @brief <b>CDispNaviRelationクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DISPNAVIRELATION_H_
#define _DISPNAVIRELATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace dispnavirelation {

/**
 * @class CDispNaviRelation
 * @brief <b>道路リンクの表示種別と経路種別の対応管理表用レコードクラス</b>\n
 * このクラスは、REFERENCE.DISPNAVIREALTIONテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CDispNaviRelation : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CDispNaviRelation, dispnavirelation )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>道路リンクの道路種別IDを取得する</b>\n
	 * @retval 0以上 道路リンクの道路種別ID
	 * @retval -1 エラー又はNULL
	 */
	long GetDispClassID() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kDispClassID, -1 );
	}

	/**
	 * @brief <b>道路リンクの経路種別IDを取得する</b>\n
	 * @retval 0以上 路リンクの経路種別ID
	 * @retval -1 エラー又はNULL
	 */
	long GetNaviClassID() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kNaviClassID, -1 );
	}

	/**
	 * @brief <b>関連値を取得する</b>\n
	 * @retval 0以上 関連値
	 * @retval -1 エラー又はNULL
	 */
	long GetRelValue() const {
		return GetLongValueByFieldName( schema::dispnavirelation::kRelValue, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>道路リンクの道路種別IDを設定する</b>\n
	 * @param lID	[in]	道路リンクの道路種別ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDispClassID( long lID ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kDispClassID, lID );
	}

	/**
	 * @brief <b>道路リンクの経路種別IDを設定する</b>\n
	 * @param lID	[in] 路リンクの経路種別ID
	 * @retval -1 エラー又はNULL
	 */
	errorcode::sindyErrCode SetNaviClassID( long lID ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kNaviClassID, lID );
	}

	/**
	 * @brief <b>関連値を設定する</b>\n
	 * @param lVal	[in]	関連値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRelValue( long lVal ) {
		return SetLongValueByFieldName( schema::dispnavirelation::kRelValue, lVal );
	}
	//@}
};

} // dispnavirelation

} // sindy

#endif // _DISPNAVIRELATION_H_
