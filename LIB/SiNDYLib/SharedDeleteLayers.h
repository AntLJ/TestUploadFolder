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
 * @file SharedDeleteLayers.h
 * @brief <b>CSharedDeleteLayersクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _SHAREDDELELAYERS_H_
#define _SHAREDDELELAYERS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace shareddeletelayers {
/**
 * @class CSharedDeleteLayers
 * @brief <b>共有削除管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.SHAREDDELETELAYERSテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CSharedDeleteLayers : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CSharedDeleteLayers, shareddeletelayers )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>共有移動セットIDを取得する</b>\n
	 * @retval 0以上 共有移動セットID
	 * @retval -1 エラー又はNULL
	 */
	long GetSetID() const {
		return GetLongValueByFieldName( schema::shareddeletelayers::kSetID, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::shareddeletelayers::kLayerNameID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>共有移動セットIDを設定する</b>\n
	 * @param lID	[in]	共有移動セットID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetSetID( long lID ) {
		return SetLongValueByFieldName( schema::shareddeletelayers::kSetID, lID );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lID	[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::shareddeletelayers::kLayerNameID, lID );
	}
	//@}
};

} // shareddeletelayers

} // sindy

#endif // _SHAREDDELELAYERS_H_
