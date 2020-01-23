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
#include "Arguments.h"
#include "CorrectZTool.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <TDC/useful_headers/flush_scope_guard.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		// �X�R�[�v�𔲂��鎞�ɋ����I��flush����
		uh::flush_scope_guard fsg;
		
		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();
		
		try
		{
			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}

			// ������
			CCorrectZTool app( args );
			if( app.execute() ){
				// ����I��
				RUNLOGGER.writeInfo( _T( "�I�������F����I��" ) );
				return 0;
			}
			else{
				// �ُ�I��
				RUNLOGGER.writeInfo( _T( "�I�������F�ُ�I��" ) );
				return 1;
			}
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		RUNLOGGER.writeInfo( e.what() );
		RUNLOGGER.writeInfo( _T( "�I�������F�ُ�I��" ) );
		return 2;
	}
}