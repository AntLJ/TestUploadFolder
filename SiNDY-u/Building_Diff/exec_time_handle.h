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
// ���s���ԊǗ��p�N���X
//--------------------------------------------------
#include <mmsystem.h>

#ifndef	_EXEC_TIME_HANDLE_H_
#define	_EXEC_TIME_HANDLE_H_

/**
	���s���ԊǗ��i�~���Z�J���h�j
	���O�����	�N�����50���ȓ��ɏI��鏈���łȂ��Ƃ��߁i�����ӂꂷ�邽�߁j
*/
class	exec_time_handle
{
	DWORD	e_dStart_Time;		///< �X�^�[�g����
	DWORD	e_dLap_Start_Time;	///< ���b�v�X�^�[�g����

public:
	// �R���X�g���N�^
	exec_time_handle	( ) {
		e_dStart_Time		= 0;
		e_dLap_Start_Time	= 0;
	}
	// �f�X�g���N�^
	~exec_time_handle	( ) {
		e_dStart_Time		= 0;
		e_dLap_Start_Time	= 0;
	}
	// �J�n���ԃZ�b�g
	void	Set_Start	( void ) {
		e_dStart_Time	= timeGetTime ();
	}
	// ���b�v�J�n���ԃZ�b�g
	void	Set_Lap_Start	( void ) {
		e_dLap_Start_Time	= timeGetTime ();
	}
	// ���b�v�^�C���l��
	DWORD	Get_Lap	( void ) {
		DWORD	a_dCur_Time	= timeGetTime ();
		return ( a_dCur_Time - e_dLap_Start_Time );
	}
	// �J�n���猻�݂܂ł̌o�ߎ��Ԃ��l��
	DWORD	Get_From_Start ( void ) {
		DWORD	a_dCur_Time	= timeGetTime ();
		return ( a_dCur_Time - e_dStart_Time );
	}
};

#endif	// _EXEC_TIME_HANDLE_H_
