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
 *	@file	DBAccess.h
 *	@brief	DB接続管理クラス 定義部
 *	@author	F.Adachi
 *	@date	2005/08/05		新規作成
 *	$ID$
 */

#ifndef ___DBACCESS___
#define ___DBACCESS___

#include "DBInfo.h"
#include <boost/utility.hpp>

/// <b>DB接続管理クラス</b>
class DBAccess : boost::noncopyable
{
public:
	DBAccess(void);
	~DBAccess();

	IWorkspacePtr	open( const DBInfo* cpDBInfo ) const;	///< 対象DBに接続

private:
	const DBInfo*	m_pDBInfo;			///< DB情報クラスへの関連

};

#endif
