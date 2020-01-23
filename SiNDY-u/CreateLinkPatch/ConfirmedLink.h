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
 *	@file ConfirmedLink.h
 *	@brief 確認済みリンク集合クラス 定義部
 *	@author	F.Adachi
 *	@date	2005/10/21		新規作成
 *	$ID$
 */

#ifndef ___CONFIRMEDLINK___
#define ___CONFIRMEDLINK___

#include <set>
#include <boost/utility.hpp>

/// <b>確認済みリンク集合クラス</b>
class ConfirmedLink : boost::noncopyable
{
public:
	ConfirmedLink(void);
	~ConfirmedLink();

	bool	exist( long cObjectID ) const;	//< 存在判定
	void	insert( long cObjectID );		//< 追加
	void	clear(void);					//< 全削除

private:
	std::set<long> m_Data;		//< 確認済みリンクのオブジェクトID集合

};

#endif
