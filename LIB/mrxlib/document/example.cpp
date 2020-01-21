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
#include <exception>
#include "MrxLoad.h"

int main(int argc, char* argv[])
{
 	// TODO: ���̈ʒu�ɃR�[�h���L�q���Ă��������B
	try {
		//MRX�p�����^�̎w��
		string	aDir = "E:\\Mapple�a�ؔ������f�[�^sample\\MRX\\5339363.mrx";

		//MRX�̓ǂݍ���
		CMrxLoad	aMRXFile;
		char	aMrxFile[256]; strcpy(aMrxFile,aDir.c_str());
		if( aMRXFile.load(aMrxFile)){

			//MRX�f�[�^�N���X�̎擾
			MRX_Alldata	aData = aMRXFile.getMRXdata();

			//MRX���̃f�B�o�O
			aMRXFile.print();

			//MRX�t�@�C���̃N���[�Y	::close()�Ȃ��ł��J���\
//			aMRXFile.close();
		}
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}
