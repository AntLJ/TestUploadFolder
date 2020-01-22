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

int main(int argc, char* argv[])
{
	try {
		const arctl::coinitializer coinit;

		main_rootin	a_mr;	// �v���O�����̐ݒ�

		if( !a_mr.arg_check ( argc, argv ) ) {
			a_mr.print_usage( argv );
			fprintf	( stderr, "�ُ�I��\n" );
			return 1;
		}

		if( !a_mr.check_exe ( ) ) {
			fprintf	( stderr, "�ُ�I��\n" );
			return 1;
		}
	}catch(...) {
		fprintf	( stderr, "��O����\n" );
		return 2;	
	}
	fprintf	( stderr, "����I��\n" );
	return 0;
}
