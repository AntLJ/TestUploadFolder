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
 * @file DispOtherRelation.h
 * @brief <b>CDispOtherRelationクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _DISPOTHERRELATION_H_
#define _DISPOTHERRELATION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace dispotherrelation {
/**
 * @class CDispOtherRelation
 * @brief <b>道路リンクの表示種別とその他の属性の対応管理用レコードクラス</b>\n
 * このクラスは、REFERENCE.DISPOTHERRELATIONテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CDispOtherRelation : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CDispOtherRelation, dispotherrelation )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>道路リンクの道路種別IDを取得する</b>\n
	 * @retval 0以上 道路リンクの道路種別ID
	 * @retval -1 エラー又はNULL
	 */
	long GetDispClassID() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kDispClassID, -1 );
	}

	/**
	 * @brief <b>道路リンクのその他の属性のIDを取得する</b>\n
	 * @retval 0以上 道路リンクのその他の属性のID
	 * @retval -1 エラー又はNULL
	 */
	long GetOtherAttrID() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kOtherAttrID, -1 );
	}

	/**
	 * @brief <b>関連値を取得する</b>\n
	 * @retval 0以上 関連値
	 * @retval -1 エラー又はNULL
	 */
	long GetRelValue() const {
		return GetLongValueByFieldName( schema::dispotherrelation::kRelValue, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>道路リンクの道路種別IDを設定する</b>\n
	 * @param nID	[in]	道路リンクの道路種別ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetDispClassID( long nID ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kDispClassID, nID );
	}

	/**
	 * @brief <b>道路リンクのその他の属性のIDを設定する</b>\n
	 * @param nID	[in]	道路リンクのその他の属性のID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOtherAttrID( long nID ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kOtherAttrID, nID );
	}

	/**
	 * @brief <b>関連値を設定する</b>\n
	 * @param lVal	[in]	関連値
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetRelValue( long lVal ) {
		return SetLongValueByFieldName( schema::dispotherrelation::kRelValue, lVal );
	}
	//@}
};

} // dispotherrelation

} // sindy

#endif // _DISPOTHERRELATION_H_
