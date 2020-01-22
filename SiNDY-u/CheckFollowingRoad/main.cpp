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
#include "main.h"
#include "Arguments.h"
#include "RunLogger.h"
#include "ErrLogger.h"
#include "CheckFollowingRoad.h"

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				wcerr << L"�R�}���h���C�������ɖ�肪����܂�" << endl;
				wcerr << L"�ُ�I��" << endl;
				return 1;
			}

			// ���s���O���I�[�v��
			CRunLogger runLog(args.m_Runlog);
			if( !runLog.open() )
			{
				wcerr << L"�ُ�I��" << endl;
				return 1;
			}

			// �G���[���O���I�[�v��
			CErrLogger errLog(args.m_Errlog);
			if( !errLog.open() )
			{
				wcerr << L"�ُ�I��" << endl;
				return 1;
			}

			// ���s���O�E�G���[���O�̃w�b�_�[���o��
			runLog.header(args);
			errLog.header();

			// ���b�V�����X�g�����
			vector<long> meshes;
			map<int, CString> mapInvalidRow;
			bool isValid = args.parseMeshes( meshes, mapInvalidRow );
			runLog.writeParseMeshResult( args.m_MeshType, mapInvalidRow );
			// ���b�V�����X�g�ɕs���ȕ������܂܂�Ă�����A���b�V�������݂��Ă���ꍇ�ُ͈�I��
			if( !isValid )
			{
				wcerr << L"���b�V�����X�g���s���ł�" << endl;
				runLog.writeLogAndConsole( L"�ُ�I��" );
				return 1;
			}

			// �������s
			CCheckFollowingRoad checker(args, runLog, errLog, meshes);
			runLog.writeExeTime(true);
			bool isSucceed = checker.execute();
			runLog.writeExeTime(false);
			const CString exeResult( isSucceed ? L"����I��" : L"����I��(�G���[����)" );
			runLog.writeLogAndConsole( exeResult );

			return 0;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		wcerr << L"�ُ�I��" << endl;
		return 2;
	}
}
