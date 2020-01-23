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

//--------------------------------------------------
// 実行時間管理用クラス
//--------------------------------------------------
#include <mmsystem.h>

#ifndef	_EXEC_TIME_HANDLE_H_
#define	_EXEC_TIME_HANDLE_H_

/**
	実行時間管理（ミリセカンド）
	※前提条件	起動後約50日以内に終わる処理でないとだめ（桁あふれするため）
*/
class	exec_time_handle
{
	DWORD	e_dStart_Time;		///< スタート時間
	DWORD	e_dLap_Start_Time;	///< ラップスタート時間

public:
	// コンストラクタ
	exec_time_handle	( ) {
		e_dStart_Time		= 0;
		e_dLap_Start_Time	= 0;
	}
	// デストラクタ
	~exec_time_handle	( ) {
		e_dStart_Time		= 0;
		e_dLap_Start_Time	= 0;
	}
	// 開始時間セット
	void	Set_Start	( void ) {
		e_dStart_Time	= timeGetTime ();
	}
	// ラップ開始時間セット
	void	Set_Lap_Start	( void ) {
		e_dLap_Start_Time	= timeGetTime ();
	}
	// ラップタイム獲得
	DWORD	Get_Lap	( void ) {
		DWORD	a_dCur_Time	= timeGetTime ();
		return ( a_dCur_Time - e_dLap_Start_Time );
	}
	// 開始から現在までの経過時間を獲得
	DWORD	Get_From_Start ( void ) {
		DWORD	a_dCur_Time	= timeGetTime ();
		return ( a_dCur_Time - e_dStart_Time );
	}
};

#endif	// _EXEC_TIME_HANDLE_H_
