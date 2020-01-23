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
#include "main_routine.h"
#include <arctl/coinitializer.h>

int  main ( int     argc,
            char**  argv )
{
	const arctl::coinitializer coinit;	// 前もってやっておくこと

	main_routine		a_mPre_Proc;

	// ツールの初期設定を行う
	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		return 1;
	}

	// 変換実行
	//	if( ! a_mPre_Proc.update_exec ( ) ) {
	if( ! a_mPre_Proc.update_exec_nolist ( ) ) {
		return 1;
	}
	return 0;
}
