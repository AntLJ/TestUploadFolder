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
 *	@file DBHandleBase.h
 *	@brief DB処理アプリケーション基底クラス 定義部
 *	@author	F.Adachi
 *	@date	2005/08/02		新規作成
 *	@note	Template Method
 *	$ID$
 */

#ifndef ___DBHANDLEBASE___
#define ___DBHANDLEBASE___

#include <ostream>
#include <memory>
#include <boost/utility.hpp>
#include "DBInfo.h"
#include "DBAccess.h"

/// <B>アプリケーション基底クラス</B>
class DBHandleBase : boost::noncopyable
{
public:
	DBHandleBase(void);
	virtual ~DBHandleBase();

	virtual void	run(void);										///< メイン処理実行
	virtual void	process(void) = 0;								///< 処理（実装はサブクラスに依存）

	const DBInfo*		getDBInfo(void) const { return m_pDBInfo.get(); }
	void				setDBInfo( std::auto_ptr<const DBInfo> cpDBInfo ) { m_pDBInfo = cpDBInfo; }
	const IWorkspacePtr	getWorkspace(void) const { return m_ipWorkspace; }
	CString				getProgramName(void) { return m_ProgramName; }

private:
	std::auto_ptr<const DBInfo>		m_pDBInfo;				///< DB情報クラスへの関連
	IWorkspacePtr	m_ipWorkspace;							///< 対象ワークスペースへのポインタ
	static const CString m_ProgramName;						///< プログラム名

	DBAccess*	m_pDBAccess;								///< DB接続管理クラスへの関連
};

#endif
