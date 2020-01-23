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
 *	@file ConfirmedLink.cpp
 *	@brief 確認済みリンク集合クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/21		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "ConfirmedLink.h"


ConfirmedLink::ConfirmedLink(void)
{
}
ConfirmedLink::~ConfirmedLink()
{
}


/**
 *	@brief	存在判定
 *	@param	cObjectID	[in]	対象オブジェクトID
 *	@return	存在するか否か
 **/
bool ConfirmedLink::exist( long cObjectID ) const
{
	return m_Data.find( cObjectID ) != m_Data.end();
}


/**
 *	@brief	追加
 *	@param	cObjectID	[in]	追加するオブジェクトID
 **/
void ConfirmedLink::insert( long cObjectID )
{
	m_Data.insert( cObjectID ).second;	// 既存の値を追加しようとするとfalseが返るが、ここではチェックしない
}


/**
 *	@brief	全削除
 **/
void ConfirmedLink::clear(void)
{
	m_Data.clear();
}

