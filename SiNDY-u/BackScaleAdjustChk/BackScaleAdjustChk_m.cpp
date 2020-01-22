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
	�����ł́A�v���O�����̃��C�����L�q���Ă���B�@\
	COM�̏������ƌ㏈�����s���Ă���B
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
	// �J�n���Ԃ̎擾
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	char			a_cpPrnBuf[2048];	// �G���[�v�����g�p�o�b�t�@
	smp_prn_info	a_sPrnInfo;			// �G���[���o�͗p
	main_rootin		a_mPre_Proc;		// �O�����Ă���Ă�������

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}


	// COM ���g�p�ł���悤�ɂ��邽�߂̏�����
	::CoInitialize(0);

	if( ! a_mPre_Proc.check_exe () ) {
		End	();
		exit ( 1 );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
//#ifdef	_DEBUG
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	
	DWORD			a_dDiff_Type	= 0;

	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "�f�[�^�ϊ������� = %0.2f(s)�ł���\n", a_dDTime );
	End();
//#endif

//	ATLASSERT( ipWorkspace != NULL );
	End	();
}

