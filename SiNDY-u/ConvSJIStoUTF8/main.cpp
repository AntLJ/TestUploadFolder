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

// ExportMultiLanguageAnno.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ConvSJIStoUTF8.h"

int main(int argc, char* argv[])
{
	try{
		CConvSJIStoUTF8 lpApp;
		int	nRet = 0;

		if( !lpApp.init(argc, argv) )
		{
			lpApp.printUsage();
			nRet = 1;
		}
		else
		{
			if( !lpApp.execute() )
				nRet = 2;
		}

		if( !nRet )
			std::cerr << "正常終了" << std::endl;
		else
		{
			std::cerr << "異常終了" << std::endl;
			return 1;
		}
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "異常終了" << std::endl;
		return 2;
	}
	catch(...)
	{
		std::cerr << "異常終了" << std::endl;
		return 2;
	}

	return 0;
}


