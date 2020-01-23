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

#include "CheckRelStationGatePoint.h"

int main(int argc, char* argv[])
{
	int retval = 0;
	try {
		arctl::coinitializer coinitializer;

		CCheckRelStationGatePoint checker;

		if (! checker.arg_check(argc, argv)) {
			retval = 1;
		} else {
			if (! checker.execute())
				retval = 1;
			else if(checker.isError())
				retval = 3;
		}
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		retval = 2;
	} catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;

	}

	switch(retval)
	{
		case 0:
			std::cout << "����I��" << std::endl;
			break;
		case 1:
			std::cerr << "�ُ�I��!!" << std::endl;
			break;
		case 2:
			std::cerr << "��O�I��!!" << std::endl;
			break;
		case 3:
			std::cout << "����I��(�`�F�b�N�G���[����)" << std::endl;
			break;
	}

	return retval;
}
