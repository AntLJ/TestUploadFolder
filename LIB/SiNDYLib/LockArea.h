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
 * @file LockArea.h
 * @brief <b>CLockAreaクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LOCKAREA_H_
#define _LOCKAREA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Feature.h"
#include "RuleCommunity.h"

namespace sindy {
namespace lockarea {
/**
 * @class CLockArea
 * @brief <b>編集ロックエリア管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LOCKAREAテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CLockArea : public CFeature  
{
	ROWCLASS_CONSTRUCTOR( CLockArea, lockarea )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::lockarea::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>接続ユーザ名IDを取得する</b>\n
	 * @retval 0以上 接続ユーザ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::lockarea::kConnectUserID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param nID		[in]	レイヤ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long nID ) {
		return SetLongValueByFieldName( schema::lockarea::kLayerNameID, nID );
	}

	/**
	 * @brief <b>接続ユーザ名IDを取得する</b>\n
	 * @param nID		[in]	接続ユーザ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long nID ) {
		return SetLongValueByFieldName( schema::lockarea::kConnectUserID, nID );
	}
	//@}
};

} // lockarea

} // sindy

#endif // _LOCKAREA_H_
