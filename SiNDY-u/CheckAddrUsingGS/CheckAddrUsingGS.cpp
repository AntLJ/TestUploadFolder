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

// CheckAddrUsingGS.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <arctl/coinitializer.h>
#include "CheckAddrUsingGSObj.h"

namespace return_code {
	enum ECode {
		unknown			= -1,	//!< 初期状態
		ok				= 0,	//!< 正常終了
		tool_error		= 1,	//!< ツールが出すエラー
		exception_error	= 2,	//!< 例外エラー
		licence_error	= 3,	//!< ライセンス認証失敗
	};
}

int _tmain(int argc, _TCHAR* argv[])
{
	int retcode = return_code::unknown;
	try{
		arctl::coinitializer coinitializer;
//		::setlocale(LC_ALL, "Japanese"); // これだと boost::program_options で日本語交じりの引数が正しく解釈されなかった…
		std::locale::global( std::locale( "Japanese", std::locale::ctype ) );

		try{
			CCheckAddrUsingGSObj cCheckAddrUsingGSObj;

			if( cCheckAddrUsingGSObj.Init(argc, argv) )
			{
				if( cCheckAddrUsingGSObj.Execute() )
				{
					std::cerr << "正常終了" << std::endl;
					retcode = return_code::ok;
				}
				else
				{
					std::cerr << "異常終了" << std::endl;
					retcode = return_code::tool_error;
				}
			}
		}
		catch (const _com_error& com_err)
		{
			// ツール例外エラー
			std::cerr << CT2A( com_err.ErrorMessage() ) << std::endl;
			retcode = return_code::exception_error;
		}
	}
	catch(const std::exception &e)
	{
		// ライセンス認証失敗
		std::cerr << e.what() << std::endl;
		retcode = return_code::licence_error;
	}
	return retcode;
}
