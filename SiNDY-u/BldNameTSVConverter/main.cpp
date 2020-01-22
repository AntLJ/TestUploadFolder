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

// main.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "BldNameTSVConverter.h"
#include "LogMgr.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/japanese_locale.h>

namespace return_code
{
	enum ECode
	{
		kUnknown	= -1,
		kOK			= 0,
		kError		= 1,
		kException	= 2,
	};
}

int _tmain( int argc, _TCHAR* argv[] )
{
	int retCode = return_code::kUnknown;
	try
	{
		const arctl::coinitializer coinitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		BldNameTSVConverter cConverter;
		if( !cConverter.init( argc, argv ) )
		{
			LogMgr::getInstance().writeRunLog( _T("異常終了") );
			retCode = return_code::kError;
		}
		else
		{
			if( !cConverter.exec() )
			{
				LogMgr::getInstance().writeRunLog( _T("異常終了") );
				retCode = return_code::kError;
			}
			else
			{
				LogMgr::getInstance().writeRunLog( _T("正常終了") );
				retCode = return_code::kOK;
			}
		}
	}
	catch( const _com_error &e )
	{
		LogMgr::getInstance().writeRunLog( _T("異常終了（例外発生） : ") + CString( e.ErrorMessage() ) );
		retCode = return_code::kException;
	}
	catch( const std::exception &e )
	{
		LogMgr::getInstance().writeRunLog( _T("異常終了（例外発生） : ") + CString( e.what() ) );
		retCode = return_code::kException;
	}
	catch( ... )
	{
		LogMgr::getInstance().writeRunLog( _T("異常終了（例外発生）") );
		retCode = return_code::kException;
	}

	return retCode;
}
