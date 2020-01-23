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

// CreateSmartGatewayNodelist.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Doc.h"
#include <arctl/coinitializer.h>


int _tmain(int argc, _TCHAR* argv[])
{
	try{
		const arctl::coinitializer aCoInitializer;
		CDoc	cDoc;
		CoInitialize(NULL);

		if (! cDoc.getenv(argv[1])){
			cout << "異常終了" << endl;
			return 0;
		}
		if (! cDoc.Proc()){
			cout << "異常終了" << endl;
			return 0;
		}
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		cout << "異常終了" << endl;
		return 0;
	}
	cerr << "正常終了" << endl;
	return 0;
}

