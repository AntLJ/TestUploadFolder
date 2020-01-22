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
#include "main_routine.h"
#include <arctl/coinitializer.h>

int main ( int     argc,
           char**  argv )
{
	try {
		const arctl::coinitializer coinit;	// �O�����Ă���Ă�������

		main_routine aPre_Proc;

		// �c�[���̏����ݒ���s��
		if( !aPre_Proc.arg_check ( argc, argv ) ){
			aPre_Proc.print_usage	( argv );
			PrnMsg ( stderr, NULL, "�ُ�I��\n");
			return 1;
		}

		// �ϊ����s
		if( ! aPre_Proc.exec ( ) ) {
			PrnMsg ( stderr, NULL, "�ُ�I��\n");
			return 1;
		}
	}catch(...) {
		PrnMsg ( stderr, NULL, "��O����\n");
		return 2;
	}
	return 0;
}
