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
 * @file EditMesh.h
 * @brief <b>CEditMeshクラス定義ファイル</b>
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _EDITMESH_H_
#define _EDITMESH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"
#include "RuleCommunity.h"

namespace sindy {
namespace editmesh {
/**
 * @class CEditMesh
 * @brief <b>編集可能メッシュ管理レコードクラス（ルール用）</b>\n
 * このクラスは、REFERNCE.EDITMESHテーブルのレコード用ラップクラスです。
 */
class SINDYLIB_API CEditMesh : public CRow  
{
	ROWCLASS_CONSTRUCTOR( CEditMesh, editmesh )
	//@{ @name 属性取得関数
	/**
	 * @brief <b>メッシュコードを取得する</b>\n
	 * @retval 0以上 メッシュコード
	 * @retval -1 エラー又はNULL
	 */
	long GetMeshCode() const {
		return GetLongValueByFieldName( schema::editmesh::kMeshCode, -1 );
	}

	/**
	 * @brief <b>レイヤ名IDを取得する</b>\n
	 * @retval 0以上 レイヤ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetLayerNameID() const {
		return GetLongValueByFieldName( schema::editmesh::kLayerNameID, -1 );
	}

	/**
	 * @brief <b>作業目的IDを取得する</b>\n
	 * @retval 0以上 作業目的ID
	 * @retval -1 エラー又はNULL
	 */
	long GetPurposeID() const {
		return GetLongValueByFieldName( schema::editmesh::kPurposeID, -1 );
	}

	/**
	 * @brief <b>オペレータ名IDを取得する</b>\n
	 * @retval 0以上 オペレータ名ID
	 * @retval -1 エラー又はNULL
	 */
	long GetOperatorID() const {
		return GetLongValueByFieldName( schema::editmesh::kOperatorID, -1 );
	}
	//@}
	//@{ @name 属性設定関数
	/**
	 * @brief <b>メッシュコードを設定する</b>\n
	 * @param lCode		[in]	メッシュコード
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetMeshCode( long lCode ) {
		return SetLongValueByFieldName( schema::editmesh::kMeshCode, lCode );
	}

	/**
	 * @brief <b>レイヤ名IDを設定する</b>\n
	 * @param lID		[in]	レイヤ名ID
	 * @retval sindyErrCode
	 */
	errorcode::sindyErrCode SetLayerNameID( long lID ) {
		return SetLongValueByFieldName( schema::editmesh::kLayerNameID, lID );
	}

	/**
	 * @brief <b>作業目的IDを設定する</b>\n
	 * @param lPurposeID	[in]	作業目的ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetPurposeID( long lPurposeID ) {
		return SetLongValueByFieldName( schema::editmesh::kPurposeID, lPurposeID );
	}

	/**
	 * @brief <b>オペレータ名を設定する</b>\n
	 * @param lID			[in]	オペレータ名ID
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode SetOperatorID( long lID ) {
		return SetLongValueByFieldName( schema::editmesh::kOperatorID, lID );
	}
	//@}
};

} // editmesh

} // sindy

#endif // _EDITMESH_H_
