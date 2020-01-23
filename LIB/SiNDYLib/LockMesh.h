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
 * @file LockMesh.h
 * @brief <b>CLockMeshクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _LOCKMESH_H_
#define _LOCKMESH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace lockmesh {
/**
 * @class CLockMesh
 * @brief <b>編集ロックメッシュ管理レコードクラス（ルール用）</b>\n
 * このクラスはREFERENCE.LOCKMESHテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CLockMesh : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CLockMesh, lockmesh )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>メッシュコードを取得する</b>\n
	 * @retval 0以上 メッシュコード
	 * @retval -1 エラー又はNULL
	 */
	long GetMeshCode() const {
		return GetLongValueByFieldName( schema::lockmesh::kMeshCode, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::lockmesh::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>接続ユーザ名IDを取得する</b>\n
	 * @retval 0以上 接続ユーザ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetConnectUserID() const {
		return GetLongValueByFieldName( schema::lockmesh::kConnectUserID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>メッシュコードを設定する</b>\n
	 * @param lCode		[in]	メッシュコード
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetMeshCode( long lCode ) {
		return SetLongValueByFieldName( schema::lockmesh::kMeshCode, lCode );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lID		[in]	レイヤ名ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::lockmesh::kLayerNameID, lID );
	}

	/**
	 * @brief <b>接続ユーザ名IDを設定する</b>\n
	 * @param lID		[in]	接続ユーザ名ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetConnectUserID( long lID ) {
		return SetLongValueByFieldName( schema::lockmesh::kConnectUserID, lID );
	}
	//@}
};

} // lockmesh

} // sindy

#endif // _LOCKMESH_H_
