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

// CreateSJShape.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "CreateSJShapeObj.h"

int _tmain(int argc, _TCHAR* argv[])
{
	arctl::coinitializer initializer;
	{
		::setlocale(LC_ALL, "Japanese");
		CCreateSJShapeObj *pCreateSJShape = new CCreateSJShapeObj();
		if (pCreateSJShape != 0 && pCreateSJShape->Init(argc, argv) && pCreateSJShape->Run())
			std::cerr << "正常に終了しました" << std::endl;
		else
			std::cerr << "異常終了しました" << std::endl;
		delete pCreateSJShape;
	}

	return 0;
}

