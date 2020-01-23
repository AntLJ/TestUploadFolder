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
#include "CheckRoadWalkNW.h"

int main(int argc, char* argv[])
{
	int	nRet = 0;
	try{
		arctl::coinitializer coinitializer;
		
		CCheckRoadWalkNW checkRoadWalkNW;

		if( !checkRoadWalkNW.init(argc, argv) )
		{
			nRet = 1;
		}
		else
		{
			if( !checkRoadWalkNW.execute() )
				nRet = 1;
			else if( checkRoadWalkNW.isError() )
				nRet = 3;
		}
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		nRet = 2;
	}

	switch(nRet)
	{
		case 0:
			std::cout << "正常終了" << std::endl;
			break;
		case 1:
			std::cerr << "異常終了!!" << std::endl;
			break;
		case 2:
			std::cerr << "例外終了!!" << std::endl;
			break;
		case 3:
			std::cout << "正常終了（チェックエラーあり）" << std::endl;
			break;
	}

	return nRet;
}
