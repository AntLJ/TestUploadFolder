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

// CommandLine.cpp : Defines the entry point for the console application.
//

#include "main_rootin.h"

int main(int argc, char* argv[])
{

	DWORD			a_dBegin_Time	= 0;
	a_dBegin_Time	= timeGetTime();

	const arctl::coinitializer coinit;

	main_rootin	a_mr;	// �v���O�����̐ݒ�

	if( !a_mr.arg_check ( argc, argv ) ) {
		a_mr.print_usage( argv );
		fflush(stdout);
		fflush(stderr);
		return	( 1 );
	}

	HRESULT		hr;
	
	try
	{
		// Execute the command
		string		a_Del_DB_Name;


		IWorkspaceFactoryPtr	ipWorkspaceFactory	( CLSID_SdeWorkspaceFactory );	// 
		IPropertySetPtr			ipPropertySet		( CLSID_PropertySet );			// �ڑ��v���p�e�B�[�i�[�p
		IWorkspacePtr			ipWorkspace;

		USES_CONVERSION;	// OLE2T ���̈ꕔ�̃}�N�����g�p����ꍇ�ɂ͕K���K�v

		// DB�ւ̐ڑ�
		if( !a_mr.DB_Open	(	ipWorkspaceFactory, ipPropertySet, ipWorkspace	) ) {
			fprintf	(stderr, "�ڑ��o���Ȃ�����\n");
			fflush(stdout);
			fflush(stderr);
			return	( 1 );
		//	End	();
		}else {
			printf("�ڑ�����");
		}
		printf(_T("\n"));

		// DB�ڑ��ɂ����������Ԃ̕\��
		DWORD				a_dConnect_Time		= 0;
		a_dConnect_Time		= timeGetTime();

		fprintf	( stdout,	"SiNDY-DB�ڑ��������� = %0.2f(s)�ł���\n",
							(double)((a_dConnect_Time - a_dBegin_Time) / 1000.0) );

		bool bRet = a_mr.exec_command( ipWorkspace );
		if( !bRet )
		{
			fprintf	( stderr, "�ُ�I��\n" );
			fflush(stdout);
			fflush(stderr);
			return 1;
		}
	}
	catch ( ... ){
		fprintf	( stderr, "��O���������Ă��܂�\n" );
		if( a_mr.m_fpOutLogFp != stderr && a_mr.m_fpOutLogFp != NULL ) {
			fprintf ( a_mr.m_fpOutLogFp, "��O���������Ă��܂�\n" );
		}
		fflush(stdout);
		fflush(stderr);
		return 2;
	}

	fprintf	( stderr, "����I��\n" );
	fflush(stdout);
	fflush(stderr);
	return ( 0 );
}
