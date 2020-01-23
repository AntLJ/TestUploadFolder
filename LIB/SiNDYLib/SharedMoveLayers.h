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
 * @file SharedMoveLayers.h
 * @brief <b>CSharedMoveLayersクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _SHAREDMOVELAYERS_H_
#define _SHAREDMOVELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace sharedmovelayers {
/**
 * @class CSharedMoveLayers
 * @brief <b>共有移動管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.SHAREDMOVELAYERSテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CSharedMoveLayers : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CSharedMoveLayers, sharedmovelayers )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>共有移動セットIDを取得する</b>\n
	 * @retval 0以上 共有移動セットID
	 * @retval -1 エラー又はNULL
	 */
	long GetSetID() const {
		return GetLongValueByFieldName( schema::sharedmovelayers::kSetID, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::sharedmovelayers::kLayerNameID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>共有移動セットIDを設定する</b>\n
	 * @param lID	[in]	共有移動セットID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetSetID( long lID ) {
		return SetLongValueByFieldName( schema::sharedmovelayers::kSetID, lID );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lID	[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::sharedmovelayers::kLayerNameID, lID );
	}
	//@}
};

} // shareddeletelayers

} // sindy

#endif // _SHAREDMOVELAYERS_H_
