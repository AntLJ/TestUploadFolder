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

/**
	ここでは、プログラムのメインを記述している。　\
	COMの初期化と後処理も行っている。
*/

#include "main_rootin.h"
#include "StdAfx.h"
#include <mmsystem.h>

void	End	() {
	::CoUninitialize();
	exit	(0);
}

void	main (	int		argc,
				char**	argv	)
{
//#ifdef	_DEBUG
	// 開始時間の取得
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	char			a_cpPrnBuf[2048];	// エラープリント用バッファ
	smp_prn_info	a_sPrnInfo;			// エラー等出力用
	main_rootin		a_mPre_Proc;		// 前もってやっておくこと

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}


	// COM を使用できるようにするための初期化
	::CoInitialize(0);

	if( ! a_mPre_Proc.check_exe () ) {
		End	();
		exit ( 1 );
	}

	//　実行部終了
	// 終了時間の取得
//#ifdef	_DEBUG
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	
	DWORD			a_dDiff_Type	= 0;

	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "データ変換総時間 = %0.2f(s)でした\n", a_dDTime );
	End();
//#endif

//	ATLASSERT( ipWorkspace != NULL );
	End	();
}

