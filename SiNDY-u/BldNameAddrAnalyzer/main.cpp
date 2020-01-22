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
#include "Logger.h"
#include "BldNameAddrAnalyzer.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	// ���s����ɂ͉����L�[�������Ă�������
	uh::scope_guard sg( []{system( "pause" );} );
#endif

	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		// �R�}���h���C����������
		Arguments args;
		if(! args.parse(argc, argv))
		{
			cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
			return 1;
		}

		// �I�v�V�����\��
		RUNLOGGER.Initialize( args.getValue( option::kRunLog ).GetString() );
		RUNLOGGER.Log( args.description(), true );

		// ������
		BldNameAddrAnalyzer app( args );
		try{
			app.execute();
			RUNLOGGER.Log( _T("\n����I��"), true );
		}
		catch( const std::runtime_error& e )
		{
			RUNLOGGER.Fatal( uh::toTStr(e.what()), true );
			RUNLOGGER.Log( _T("\n�ُ�I��"), true );
			return 1;
		}
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "�ُ�I��" << endl;
		return 2;
	}

	return 0;
}
