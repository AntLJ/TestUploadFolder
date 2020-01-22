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

#include "stdafx.h"
#include "CheckAddress.h"

// 初期化
bool CheckAddress::init( int argc, TCHAR** argv )
{
	// コマンドライン引数処理
	if( !m_argument.setParameter(argc, argv))
	{
		cerr << CT2A(message::getMsg( eFailAnalyzeArgument)) << endl;
		return false;
	}

	// 実行時エラーログ初期化
	CString runLogFile = m_argument.getValue(argumentType::kRunLog);
	if( !RUNLOGGER.initialize(runLogFile) )
	{
		cerr << CT2A(message::getMsg( eFailRunLogInitialize)) << CT2A(_T(">>Path:")) << CT2A( runLogFile ) << endl;
		return false;
	}
	
	// 制御クラス初期化
	if( !m_checkControl.init( m_argument ))
	{
		printProgress( getMsg( eFailInitCheckControl ));
		return false;
	}
	return true;
}

// 実行
bool CheckAddress::run() 
{
	if(!m_checkControl.doCheck())
	{
		printProgress( getMsg( eFailDoCheck ));
		return false;
	}

	return true;
}
