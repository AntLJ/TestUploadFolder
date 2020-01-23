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

// CreatePyramid.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"

#include "Pyramid.h"
#include <arctl/coinitializer.h>
#include <TDC/useful_headers/flush_scope_guard.h>

int _tmain(int argc, _TCHAR* argv[])
{
	// �X�R�[�v�𔲂��鎞�ɋ����I��flush����
	uh::flush_scope_guard fsg;

	try{
		const arctl::coinitializer coInitializer;

		bool bFirst = false;
		while(1)
		{
			CPyramid cPyramid;

			// �R�}���h���C�������擾
			bool bOption = false;
			if( argc > 1 && !bFirst )
			{
				std::list<CString> listOption;
				for( int i = 0; i < argc; i++ )
				{
					listOption.push_back( argv[i] );
					std::string ga;
					ga = argv[i];
				}

				bOption = cPyramid.InitOption( listOption );
			}

			int iMode = 0;
			// �J�n����
			while(1)
			{
				iMode = cPyramid.StartFunction( bOption );
				if( iMode == 1 || iMode == 2 || bOption )
					break;
				else
					continue;
			}

			// �s���~�b�h�t�@�C���쐬����
			switch( iMode )
			{
			case 1:// ���b�V�����X�g
				cPyramid.TargetMeshList( bOption );
				break;
			case 2:// �f�B���N�g��
				cPyramid.TargetPathList( bOption );
				break;
			default:
				break;
			}

			int iLoop = 0;
			// �I������
			if( argc <= 1 )
			{
				while(1)
				{
					int iRet = cPyramid.EndFunction();
					switch( iRet )
					{
					case 1:
						iLoop = 1;
						break;
					case 2:
						iLoop = 2;
						break;
					default:
						iLoop = 0;
						break;
					}
					if( iLoop == 1 || iLoop == 2 )
						break;
					else
						continue;
				}
			}
			else
				break;

			if( iLoop == 2 )
				break;

			bFirst = true;

			std::cout<<"\n\n";
		}
	}
	catch(const std::exception& exc){
		std::cerr << exc.what() << std::endl;
	}

	return 0;
}

