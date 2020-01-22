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
#include "main_rootin.h"
#include <arctl/coinitializer.h>

int main(int argc, char* argv[])
{
	try {
		const arctl::coinitializer coinit;

		main_rootin	a_mr;	// プログラムの設定

		if( !a_mr.arg_check ( argc, argv ) ) {
			a_mr.print_usage( argv );
			fprintf	( stderr, "異常終了\n" );
			return 1;
		}

		if( !a_mr.check_exe ( ) ) {
			fprintf	( stderr, "異常終了\n" );
			return 1;
		}
	}catch(...) {
		fprintf	( stderr, "例外発生\n" );
		return 2;	
	}
	fprintf	( stderr, "正常終了\n" );
	return 0;
}
