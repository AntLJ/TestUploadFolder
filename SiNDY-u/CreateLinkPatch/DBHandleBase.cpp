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
 *	@file DBHandleBase.cpp
 *	@brief DB処理アプリケーション基底クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "DBHandleBase.h"
#include "DBAccess.h"
#include <fstream>
#include <set>
#include <vector>
#include <string>


/// コンストラクタ
DBHandleBase::DBHandleBase(void) : 
	m_ipWorkspace(NULL)
{
}

/// デストラクタ
DBHandleBase::~DBHandleBase()
{
}


/**
 *	@brief	メイン処理
 **/
void DBHandleBase::run(void)
{
	std::string aFunc = "DBHandleBase::run()";
#ifdef	_DEBUG
	m_pDBInfo->disp( std::cout );
#endif
	DBAccess aDBAccess;

	m_ipWorkspace = aDBAccess.open( m_pDBInfo.get() );
	if (m_ipWorkspace == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : ワークスペース取得失敗" ); }

	// 処理（実装はサブクラスに依存）
	process();
}

