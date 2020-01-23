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

// ChgSmartPolygon.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "globals.h"
#include <arctl/coinitializer.h>
#include "ParamCtrl.h"
#include "ChangeSmart.h"

bool usage()
{
	std::cout << "�p�r�F�|���S���f�[�^���Ԉ����v���O����" << std::endl;
	std::cout << "�g�p�@�FChgSmartPolygon.exe [Option]" << std::endl;
	std::cout << " Option : " << std::endl;
	std::cout << "      -d [DB�p�X]	--- �ڑ�DB��" << std::endl;
	std::cout << "      -f [�t�B�[�`���N���X]	--- �t�B�[�`���N���X" << std::endl;
	std::cout << "      -o [�ߎ��l]	--- �ߎ��Ƃ݂Ȃ��I�t�Z�b�g��" << std::endl;

	return false;
}

bool arg_check(int argc, _TCHAR **argv, CParamCtrl &cParam)
{
	if (argc <= 1)
		return usage();

	while (--argc)
	{
		_TCHAR *s = *++argv;

		switch (*s)
		{
			case _T('-'):
				switch (*++s)
				{
					case _T('O'):	// �ߎ��I�t�Z�b�g��
					case _T('o'): cParam.SetAllowOffset( _tstof( *++argv ) ); --argc; break;
					case _T('D'):	// �ڑ�DB��
					case _T('d'): cParam.SetDB( *++argv ); --argc; break;
					case _T('F'):	// FeatureClass��
					case _T('f'): cParam.SetFeatureClassName( *++argv ); --argc; break;
					default:
						return usage();
						break;
				}

				break;
			default:
				return usage();
				break;
		}
	}
	return true;
}

void Init(const CParamCtrl& cParam)
{
	std::cerr << "###### START ChgSmartPolygon ######" << std::endl;
	std::cerr << "# DB = " << CT2CA( cParam.GetDB() ) << std::endl;
	std::cerr << "# FeatureClass = " << CT2CA( cParam.GetFeatureClassName() ) << std::endl;
	std::cerr << "# AllowOffset = " << cParam.GetAllowOffset() << std::endl << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	const arctl::coinitializer coinit;

	try
	{
		std::cout << "--- START ChgSmartPolygon ---" << std::endl;
		std::cout << "Read Parameter" << std::endl;

		CParamCtrl cParam;
		if( !( arg_check( argc, argv, cParam ) ) )
			return 1;

		Init( cParam );

		CTime cTime = CTime::GetCurrentTime();
		long lYear = cTime.GetYear();
		long lMonth = cTime.GetMonth();
		long lDay = cTime.GetDay();
		long lHour = cTime.GetHour();
		long lMinute = cTime.GetMinute();
		long lSecond = cTime.GetSecond();
		
		time_t st_time;
		time(&st_time);
		CString cStrTime;
		cStrTime.Format( _T("# �J�n���� %04d/%02d/%02d %02d:%02d:%02d"), lYear, lMonth, lDay, lHour, lMinute, lSecond );
		std::cerr << CT2CA( cStrTime ) << std::endl;

		std::cout << "Get FeatureClass" << std::endl;

		IWorkspacePtr ipWorkspace;
		IFeatureClassPtr ipFeatureClass = GetFeatureClass( cParam.GetDB(), cParam.GetFeatureClassName(), ipWorkspace );
		if( !ipFeatureClass )
		{
			std::cerr << "[ERROR] Get FeatureClass" << std::endl;
			return 1;
		}
		CChangeSmart cSmart;
		cSmart.SetOffset( cParam.GetAllowOffset() );

		std::cout << "Start ExecSmart" << std::endl;

		IWorkspaceEditPtr ipWorkspaceEdit = ipWorkspace;
		if( ipWorkspaceEdit )
		{
			if (FAILED(ipWorkspaceEdit->StartEditing( VARIANT_FALSE )) || // Undo Redo ������Ȃ��Ȃ� VARIANT_FALSE
				FAILED(ipWorkspaceEdit->StartEditOperation())) {
					std::cerr << "[ERROR] failed to start edit." << std::endl;
					return 1;
			}
		} else {
			std::cerr << "[ERROR] get IWorkspaceEdit." << std::endl;
			return 1;
		}

		bool bRet = cSmart.ExecSmart( ipFeatureClass );
		if( !bRet )
			return 1;

		if( ipWorkspaceEdit )
		{
			ipWorkspaceEdit->StopEditOperation();
			HRESULT hr = ipWorkspaceEdit->StopEditing( VARIANT_TRUE );
			if( S_OK != hr )
			{
				std::cerr << "[ERROR] StopEditing." << std::endl;
				return 1;
			}
		} else {
			std::cerr << "[ERROR] get IWorkspaceEdit." << std::endl;
			return 1;
		}

		time_t tm;
		time(&tm);
		cTime = CTime::GetCurrentTime();
		lYear = cTime.GetYear();
		lMonth = cTime.GetMonth();
		lDay = cTime.GetDay();
		lHour = cTime.GetHour();
		lMinute = cTime.GetMinute();
		lSecond = cTime.GetSecond();
		cStrTime.Format( _T("# �I������ %04d/%02d/%02d %02d:%02d:%02d"), lYear, lMonth, lDay, lHour, lMinute, lSecond );
		std::cerr << CT2CA( cStrTime ) << std::endl;
		std::cerr << "# Total " << difftime( tm, st_time ) << "[sec]" << std::endl;
	}
	catch(const std::exception& e) 
	{
		printf("%s\n", e.what());
		CString str;
		str.Format( _T("[Error] %s\n"), e.what() );
		std::cerr << CT2CA( str ) << std::endl;
		return 2;
	}

	return 0;
}

