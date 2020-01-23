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
 *	@file	DBAccess.cpp
 *	@brief	DB接続管理クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/08/05		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "DBAccess.h"
#include "sindy/workspace.h"


/// コンストラクタ
DBAccess::DBAccess(void)
{
}

/// デストラクタ
DBAccess::~DBAccess()
{
}


/**
 *	@brief	対象DBに接続
 *	@return	ワークスペースへのポインタ
 **/
IWorkspacePtr DBAccess::open( const DBInfo* cpDBInfo ) const
{
	if (!cpDBInfo) { return NULL; }

	// 接続用プロパティ設定

	// SDE接続
	IWorkspacePtr ipWorkspace = sindy::create_workspace(cpDBInfo->m_DataBase);
	if(! ipWorkspace)
		return NULL;

	return ipWorkspace;
}

