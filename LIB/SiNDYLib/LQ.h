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

// LQ.h: CLQ クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_)
#define AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Row.h"

namespace sindy {
namespace link_queue {

class SINDYLIB_API CLQ : public CRow
{
	ROWCLASS_CONSTRUCTOR( CLQ, unknown )

	bool operator<( const CLQ& obj )
	{
		return (GetSequence() < obj.GetSequence());
	}
	bool operator>( const CLQ& obj )
	{
		return (GetSequence() > obj.GetSequence());
	}

	//@{ @name 属性取得関数
	/**
	 * @brief Road_LinkのリンクのIDを返す
	 * @retval -1 エラー
	 * @retval -1以外 Road_LinkのリンクのID
	 */
	inline long GetLinkId() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLinkID, -1 );
	}
	
	/**
	 * @brief 方向コードを返す
	 * @retval -1 エラー
	 * @retval -1以外 方向コード
	 */
	inline long GetLinkDir() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLinkDir, -1 );
	}

	/**
	 * @brief 順番を返す
	 * @retval -1 エラー
	 * @retval -1以外 順番
	 */
	inline long GetSequence() const
	{
		return GetLongValueByFieldName( schema::link_queue::kSequence, -1 );
	}

	/**
	 * @brief 終端フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 終端フラグ
	 */
	inline long GetLastLink() const
	{
		return GetLongValueByFieldName( schema::link_queue::kLastLink, -1 );
	}

	/**
	 * @brief InfIDを返す
	 * @retval -1 エラー
	 * @retval -1以外 InfID
	 */
	inline long GetInfId() const
	{
		return GetLongValueByFieldName( schema::link_queue::kInfID, -1 );
	}
	//@}

	//@{ @name 属性設定関数
	/**
	 * @brief Road_LinkのリンクのIDを設定する
	 * @retval -1 エラー
	 * @retval -1以外 Road_LinkのリンクのID
	 */
	errorcode::sindyErrCode SetLinkId(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLinkID, lValue );
	}
	
	/**
	 * @brief 方向コードを設定する
	 * @retval -1 エラー
	 * @retval -1以外 方向コード
	 */
	errorcode::sindyErrCode SetLinkDir(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLinkDir, lValue );
	}

	/**
	 * @brief 順番を設定する
	 * @retval -1 エラー
	 * @retval -1以外 順番
	 */
	errorcode::sindyErrCode SetSequence(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kSequence, lValue );
	}

	/**
	 * @brief 終端フラグを設定する
	 * @retval -1 エラー
	 * @retval -1以外 終端フラグ
	 */
	errorcode::sindyErrCode SetLastLink(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kLastLink, lValue );
	}

	/**
	 * @brief InfIDを設定する
	 * @retval -1 エラー
	 * @retval -1以外 InfID
	 */
	errorcode::sindyErrCode SetInfId(long lValue)
	{
		return SetLongValueByFieldName( schema::link_queue::kInfID, lValue );
	}
	//@}
};

} // link_queue

} // sindy

#endif // !defined(AFX_LQ_H__45408FF8_9E86_422A_A325_D808888844F2__INCLUDED_)
