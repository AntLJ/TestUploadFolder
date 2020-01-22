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
	// �J�n���Ԃ̎擾
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();
//#endif

	main_rootin		a_mPre_Proc;		// �O�����Ă���Ă�������

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}

	IWorkspacePtr			ipWorkspace;

	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

	// DB�ւ̐ڑ�
	if( !a_mPre_Proc.DB_Open ( ipWorkspace ) ) {
		fprintf	(stderr, "�ڑ��o���Ȃ�����\n");
	}else {
		printf(_T("�ڑ�����"));
	}
	printf(_T("\n"));

	// DB�ڑ��ɂ����������Ԃ̕\��
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

	// �`�F�b�N���s
	if( ! a_mPre_Proc.check_exe ( ipWorkspace ) ) {
		exit ( 1 );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	DWORD			a_dDiff_Type	= 0;
	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "�f�[�^�`�F�b�N������ = %0.2f(s)�ł���\n", a_dDTime );

//	ATLASSERT( ipWorkspace != NULL );
}

