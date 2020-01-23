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
#include "main_rootin.h"
#include <arctl/coinitializer.h>
#include <atl2/io.h>

int	main (	int		argc,
				char**	argv	)
{
	int iResult = 0;

	try {
		const arctl::coinitializer coinit;

		main_rootin			a_mPre_Proc;		// �O�����Ă���Ă�������

		// �c�[�����̂̏����ݒ���s��
		if( !a_mPre_Proc.arg_check ( argc, argv ) ){
			a_mPre_Proc.print_usage	( argv );
			iResult = 1;
		}
		else if( ! a_mPre_Proc.conv_exe ( ) ) { 	// �ϊ����s
			iResult = 1;
		}
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		iResult = 2;
	}
	catch( const _com_error& e ) {
		std::cerr << atl2::stringize(e) << std::endl;
		iResult = 2;
	}
	catch( ... ) {
		std::cerr << "��O���������܂���" << std::endl;
		iResult = 2;
	}

	if( iResult == 0 )
		std::cout << "����I��" << std::endl;
	else
		std::cout << "�ُ�I��" << std::endl;

	fflush(stderr);

	return ( iResult );

}
