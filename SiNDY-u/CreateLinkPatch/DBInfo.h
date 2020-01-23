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
 *	@file DBInfo.h
 *	@brief DB情報クラス 定義部
 *	@author	F.Adachi
 *	@date	2005/08/05		新規作成
 *	$ID$
 */

#ifndef ___DBINFO___
#define ___DBINFO___

#include <boost/utility.hpp>

/// <B>DB情報クラス</B>
struct DBInfo : boost::noncopyable
{
	CString m_DataBase;					///< データベース名 (例) sindy2010@onyx(SDE.DEFAULT)など
	static const CString m_Instance;	///< インスタンス名

public:
	DBInfo(void);
	~DBInfo();

	void	init( char* cpCStr );		///< 初期化
	void	disp( std::ostream& crStream ) const;		///< 各フィールドの値を表示

};

#endif
