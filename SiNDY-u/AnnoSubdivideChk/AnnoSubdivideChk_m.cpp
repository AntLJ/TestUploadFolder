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

//#include "admin_chk_err.h"
//#include "admin_chk_state.h"
#include "StdAfx.h"
#include "main_rootin.h"
#include <arctl/coinitializer.h>

#define	IPPAN_KOKUDOU		1	///< ��ʍ���
#define	SYUYOUDOU_TOFUKEN	2	///< ��v�n�����i�s�{���j
#define	SYUYOUDOU_SITEISI	3	///< ��v�n�����i�w��s�j
/*
#define	IPPAN_KOKUDOU_YURYOU	103
#define	IPPAN_KOKUDOU_YURYOU	104
#define	IPPAN_KOKUDOU_YURYOU	105
*/
#define	KOKUDOU_NUM			1202101	///< �����ԍ�
#define	KENDOU_NUM			1202102	///< �����ԍ�

#define	FERRY_TERM			203001	///< �t�F���[�^�[�~�i��
#define	FERRY_ARVL			203105	///< �t�F���[�q�H���̖��͓����n����
#define	OTHER_FERRY_ARVL	203106	///< ���̑����q�H���q�H���̖��͓����n����

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
//	smp_prn_info	a_sPrnInfo;			// �G���[���o�͗p

	// �J�n���Ԃ̎擾
	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();

	main_rootin		a_mPre_Proc;		// �O�����Ă���Ă�������

	if( !a_mPre_Proc.arg_check ( argc, argv ) ){
		a_mPre_Proc.print_usage	( argv );
		exit ( 1 );
	}

	// COM ���g�p�ł���悤�ɂ��邽�߂̏�����
//	::CoInitialize(0);

	USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v
	
	// DB�ڑ��ɂ����������Ԃ̕\��
	DWORD				a_dConnect_Time		= 0;
	a_dConnect_Time		= timeGetTime();

	fprintf	( stdout,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
						(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

//	if( ! check_exe ( a_mPre_Proc, a_pTool_Init, ipWorkspace_Map, ipWorkspace_Base, ipWorkspace_Addr ) ) {
//	if( ! a_mPre_Proc.check_exe ( a_mPre_Proc, ipWorkspace_Map, ipWorkspace_Base ) ) {
	if( ! a_mPre_Proc.check_exe () ) {
		exit ( 1 );
	}

	//�@���s���I��
	// �I�����Ԃ̎擾
	DWORD			a_dEnd_Time		= 0;
	a_dEnd_Time		= timeGetTime();
	
	DWORD			a_dDiff_Type	= 0;

	a_dDiff_Type	= a_dEnd_Time - a_dBegin_Time;
	double			a_dDTime	= a_dDiff_Type / 1000.0;
	fprintf	( stdout, "�f�[�^�ϊ������� = %0.2f(s)�ł���\n", a_dDTime );
//	End();

//	ATLASSERT( ipWorkspace != NULL );
}

