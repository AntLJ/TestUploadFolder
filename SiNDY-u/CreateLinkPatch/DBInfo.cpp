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
 *	@file DBInfo.cpp
 *	@brief DB情報クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/08/05		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "DBInfo.h"
#include <string>

const CString DBInfo::m_Instance = "5151";

/// コンストラクタ
DBInfo::DBInfo(void)
{
}

/// デストラクタ
DBInfo::~DBInfo()
{
}

/**
 *	@brief	初期化
 *	@param	cpDBStr	[in]	DB指定文字列 (例："TECHROAD\@coral.mr.ipc.pioneer.co.jp(SNAP_TEST)" )
 **/
void DBInfo::init( char* cpDBStr )
{
	std::string aFunc = "DBInfo::init()";
	
	m_DataBase = cpDBStr;
}


/**
 *	@brief	各フィールドの値を表示
 *	@param	crStream	[in]	出力ストリーム
 **/
void DBInfo::disp( std::ostream& crStream ) const
{
	crStream << std::endl;
	crStream << "データベース名：["	<< static_cast<const char*>(m_DataBase) << "]" << std::endl;				
	crStream << std::endl;
}

