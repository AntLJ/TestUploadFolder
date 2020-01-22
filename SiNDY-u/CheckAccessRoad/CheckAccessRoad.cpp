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

// CheckAccessRoad.cpp : �R���\�[�� �A�v���P�[�V�����p�̃G���g�� �|�C���g�̒�`
//

#include "stdafx.h"
#include "CheckAccessRoad.h"
#include <arctl/coinitializer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// �B��̃A�v���P�[�V���� �I�u�W�F�N�g

CWinApp theApp;

using namespace std;

int _tmain(int cArgc, TCHAR* cArgv[], TCHAR* envp[])
{
	int nRetCode = 0;

	try
	{
		//ARC931�Ή��B���C�Z���X�F�؁i�{COM�̏������j
		const arctl::coinitializer aCoInitializer;

		//���{��t�@�C�����ɑΉ������
	    std::locale loc = std::locale();
	    loc = std::locale(loc, "japanese", std::locale::ctype);
	    std::locale::global(loc);

		// MFC �̏���������я��������s���̃G���[�̏o��
		if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
			// TODO: �K�v�ɉ����ăG���[ �R�[�h��ύX���Ă��������B
			cerr << _T("Fatal Error: initialization failed") << endl;
			nRetCode = 1;
		}
		else{
			// �{�݃|�C���g�ɃA�N�Z�X���铹�H�ǐՏ����G���W���N��
			CApp	*apApp = new CApp();
			if( apApp ){
				if( apApp->init(cArgc, cArgv)){
					//�N�����[�h(m_JobNo)�����͂���Ă��邩�ۂ��ŋ�����ς���B
					//�N�����[�h�����͂���Ă��Ȃ��ꍇ�Θb���[�h�ŋN��
					if(apApp->m_JobNo == 0)
					{
						do {
							if(apApp->showMenu() == 1)
								break;
							nRetCode = apApp->run();
							if(nRetCode == 1)
								return nRetCode;
							char	aCh;
							cout << "Enter to 'e' end. " << endl;
							cout << "Enter    'c' to restart. : ";
							cin >> aCh;
							if(( aCh == 'e' )||( aCh == 'q' )){
								break;
							}
						} while( 1 );
					}
					//�N�����[�h�����͂���Ă���ꍇ�͂��̂܂܎��s
					else if(apApp->m_JobNo >= 1 && apApp->m_JobNo <= 4)
					{
						nRetCode = apApp->run();
						if (nRetCode == 1)
							return nRetCode;
					}
					//�N�����[�h�̓��͂����������ꍇ�̓G���[
					else
					{
						std::cerr << "Job_No�@�́@1�`4 �̊ԂŐݒ肵�Ă�������" << std::endl;
						std::cerr << "�ُ�I��" << std::endl;
						return 1;
					}
				}
				else
				{
					std::cerr << "config.txt �̎w��Ɍ�肪����܂�" << std::endl;
					std::cerr << "�ُ�I��" << std::endl;
					return 1;
				}
				delete apApp;
			}
			else
			{
				std::cerr << "���H�ǐՏ����G���W���̋N���Ɏ��s���܂���" << std::endl;
				std::cerr << "�ُ�I��" << std::endl;
				return 1;
			}
		}
	}
	catch(std::exception& e) {
		std::cerr << e.what() << std::endl;
		std::cerr << "�ُ�I��(��O)" << std::endl;
		return 2;
	}
	return nRetCode;
}


