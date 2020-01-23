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
#include "CompShape2Shape.h"

int main(int argc, char* argv[])
{
	int iRET=0;
	try
	{
		::CoInitialize(0);
		//日本語ファイル名に対応する為
		std::locale loc = std::locale();
		loc = std::locale(loc, "japanese", std::locale::ctype);
		std::locale::global(loc);
		{
			CCompShape2Shape a_cs2s;

			if( !a_cs2s.arg_check( argc, argv ) ){
				a_cs2s.printUsage();
				return 1;
			}
			iRET = a_cs2s.execute();
		}
		::CoUninitialize();
	}
	catch( std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		iRET = 2;
	}

	return iRET;
}

