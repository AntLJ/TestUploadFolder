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

//#include "admin_chk_err.h"
//#include "admin_chk_state.h"
#include "StdAfx.h"
#include "main_rootin.h"
#include <arctl/coinitializer.h>

#define	IPPAN_KOKUDOU		1	///< 一般国道
#define	SYUYOUDOU_TOFUKEN	2	///< 主要地方道（都府県）
#define	SYUYOUDOU_SITEISI	3	///< 主要地方道（指定市）
/*
#define	IPPAN_KOKUDOU_YURYOU	103
#define	IPPAN_KOKUDOU_YURYOU	104
#define	IPPAN_KOKUDOU_YURYOU	105
*/
#define	KOKUDOU_NUM			1202101	///< 国道番号
#define	KENDOU_NUM			1202102	///< 県道番号

#define	FERRY_TERM			203001	///< フェリーターミナル
#define	FERRY_ARVL			203105	///< フェリー航路名称又は到着地名称
#define	OTHER_FERRY_ARVL	203106	///< その他旅客路線航路名称又は到着地名称

//#ifdef	_DEBUG
#include <mmsystem.h>
//#endif

//void	End	() {
//	::CoUninitialize();
//	exit	(0);
//}


void	main (	int		argc,
				char**	argv	)
{
	const arctl::coinitializer coinit;
//	smp_prn_info	a_sPrnInfo;			// エラー等出力用

	// 開始時間の取得
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();

	main_rootin		a_mPre_Proc;		// 前もってやっておくこと

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}

	// COM を使用できるようにするための初期化
//	::CoInitialize(0);

	USES_CONVERSION;	// OLE2T 等の一部のマクロを使用する場合には必ず必要
	
	// DB接続にかかった時間の表示
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB接続処理時間 = %0.2f(s)でした\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

//	if( ! check_exe ( a_mPre_Proc, a_pTool_Init, ipWorkspace_Map, ipWorkspace_Base, ipWorkspace_Addr ) ) {
//	if( ! a_mPre_Proc.check_exe ( a_mPre_Proc, ipWorkspace_Map, ipWorkspace_Base ) ) {
	if( ! a_mPre_Proc.check_exe () ) {
		exit ( 1 );
	}

	//　実行部終了
	// 終了時間の取得
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	
	DWORD			a_dDiff_Type	= 0;

	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "データ変換総時間 = %0.2f(s)でした\n", a_dDTime );
//	End();

//	ATLASSERT( ipWorkspace != NULL );
}

