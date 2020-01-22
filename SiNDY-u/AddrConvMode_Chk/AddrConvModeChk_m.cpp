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

void	End	() {
	::CoUninitialize();
	exit	(0);
}

void	main (	int		argc,
				char**	argv	)
{

	const arctl::coinitializer coinit;

	main_rootin			a_mPre_Proc;		// 前もってやっておくこと

	try {
		// ツール自体の初期設定を行う
		if( !a_mPre_Proc.arg_check ( argc, argv ) ){
			a_mPre_Proc.print_usage	( argv );
			exit ( 1 );
		}

		// チェック実行
		if( ! a_mPre_Proc.check_exe ( ) ) {
			exit ( 1 );
		}
	}
	catch ( ... ){
		fprintf	( stderr, "例外が発生しています\n" );
		if( a_mPre_Proc.e_fErr_Fp != stderr && a_mPre_Proc.e_fErr_Fp != NULL ) {
			fprintf ( a_mPre_Proc.e_fErr_Fp, "例外が発生しています\n" );
		}
	}
//	End();
}
