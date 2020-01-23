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
 *	@file CmdLineArg.h
 *	@brief コマンドライン引数クラス 定義部
 *	@note	コマンドライン引数に依存する情報を管理するクラス
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */

#ifndef ___CMDLINEARG___
#define ___CMDLINEARG___

#include "DBInfo.h"
#include <boost/utility.hpp>
#include <string>

/// <b>コマンドライン引数クラス</b>
class CmdLineArg : boost::noncopyable
{
public:
	CmdLineArg(void);
	~CmdLineArg();

	void			analyze( int cArgc, char** cArgv );			///< 解析

	void			setDBInfo( DBInfo* cpDBInfo ) { m_pDBInfo = cpDBInfo; }
	const DBInfo*	getDBInfo(void) const { return m_pDBInfo; }

	const char*		getApprAttrFilePath(void) const { return m_ApprAttrFilePath.c_str(); }
	const char*		getLinkPatchFilePath(void) const { return m_LinkPatchFilePath.c_str(); }
	const char*		getEtcLinkFilePath(void) const { return m_EtcLinkFilePath.c_str(); }
	bool			useEUC(void) const { return m_EUC; }

private:
	DBInfo*	m_pDBInfo;										///< DB情報クラスへの関連

	// オプションフラグ
	std::string	m_ApprAttrFilePath;							///< 路線コード指定ファイルのパス
	std::string m_LinkPatchFilePath;						///< リンクパッチファイルのパス
	std::string m_EtcLinkFilePath;							///< スマートIC用道路リンクリストのパス
	bool		m_EUC;										///< 文字コードをEUCにするか否か

};

#endif
