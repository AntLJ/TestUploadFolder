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
 *	@file CmdLineArg.cpp
 *	@brief コマンドライン引数クラス 実装部
 *	@author	F.Adachi
 *	@date	2005/10/18		新規作成
 *	$ID$
 */

#include "StdAfx.h"
#include "CmdLineArg.h"
#include "ZCL/ZFileSwitch.h"
#include "ZCL/ZBoolSwitch.h"
#include "ZCL/ZCmdLine.h"
#include "ZCL/ZSepByTab.h"
#include "LogFile.h"


/// コンストラクタ
CmdLineArg::CmdLineArg(void) :
	m_pDBInfo(NULL),
	m_EUC(false)
{
}

/// デストラクタ
CmdLineArg::~CmdLineArg()
{
}


/**
 *	@brief	解析
 *	@note	与えられたコマンドライン引数を解析して、得られた値を格納
 *	@param	cArgc	[in]	コマンドライン引数の数
 *	@param	cArgv	[in]	コマンドライン引数の列
 **/
void CmdLineArg::analyze( int cArgc, char** cArgv )
{
	std::string aFunc = "CmdLineArg::analyze()";

	ZCmdLine aCmdLine;
	aCmdLine.append( new ZFileSwitch( 'd', true, "DBSpecificationString", "DB指定文字列" ) );
	aCmdLine.append( new ZFileSwitch( 'l', true, "OutLogFile", "出力ログファイル" ) );
	aCmdLine.append( new ZFileSwitch( 'r', true, "AppropriateAttributeFile", "路線コード指定ファイル" ) );
	aCmdLine.append( new ZFileSwitch( 'p', true, "LinkPatchFile", "リンクパッチファイル" ) );
	aCmdLine.append( new ZBoolSwitch( 'u', false, "UNIX", "改行コードをLFで出力" ) );

	if (! aCmdLine.analyze( cArgc, cArgv ) ) {
		aCmdLine.errorHandle();
		throw std::runtime_error( "R1F:" + aFunc + " : コマンドライン引数が不正" );
	}
	
	char* aDBStr = const_cast<char*>(static_cast<const char*>(aCmdLine.getSwitch("DBSpecificationString")->getObject()));
	const char* pLogFileName = static_cast<const char*>(aCmdLine.getSwitch("OutLogFile")->getObject());
	m_ApprAttrFilePath = static_cast<const char*>(aCmdLine.getSwitch("AppropriateAttributeFile")->getObject());
	m_LinkPatchFilePath = static_cast<const char*>(aCmdLine.getSwitch("LinkPatchFile")->getObject());
	
	// オプションフラグの設定
	m_EUC = (aCmdLine.getSwitch("UNIX")->getObject() == NULL) ? false : true;

	if (m_pDBInfo == NULL) { throw std::runtime_error( "R1F:" + aFunc + " : DBInfoが未セット" ); }
	m_pDBInfo->init(aDBStr);

	// 出力ログファイルの設定
	LogFile* pLog = LogFile::Instance();
	pLog->setStream( pLogFileName );
}
	
