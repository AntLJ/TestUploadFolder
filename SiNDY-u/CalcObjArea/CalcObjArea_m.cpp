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
#include "StdAfx.h"
#include "main_rootin.h"
#include <arctl/coinitializer.h>


#ifdef	_DEBUG
#include <mmsystem.h>
#endif

void	main (	int		argc,
				char**	argv	)
{
	const arctl::coinitializer coinit;

//#ifdef	_DEBUG
	// 開始時間の取得
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	main_rootin		a_mPre_Proc;		// 前もってやっておくこと

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}

	IWorkspacePtr			ipWorkspace;

	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要

	// DBへの接続
	if( !a_mPre_Proc.DB_Open ( ipWorkspace ) ) {
		fprintf	(stderr, "接続出来なかった\n");
	}else {
		printf(_T("接続完了"));
	}
	printf(_T("\n"));

	// DB接続にかかった時間の表示
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

	// チェック実行
	if( ! a_mPre_Proc.check_exe ( ipWorkspace ) ) {
		exit ( 1 );
	}

	//　実行部終了
	// 終了時間の取得
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	DWORD			a_dDiff_Type	= 0;
	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "データチェック総時間 = %0.2f(s)でした\n", a_dDTime );

//	ATLASSERT( ipWorkspace != NULL );
}

