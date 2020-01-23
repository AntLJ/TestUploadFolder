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
#include "CSV2DB.h"
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

		try
		{
			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}

			// ���O�t�@�C������
			if( !RUNLOGGER.Initialize( args.getValue( arg::kRunLog ).GetString() ) ||
				!ERRLOGGER.Initialize( args.getValue( arg::kErrLog ).GetString() ) )
			{
				throw std::runtime_error( "Log file open failure." );
			}

			// �I�v�V�����\��
			RUNLOGGER.Log( args.description(), true );

			// ������
			try{
				CCSV2DB app( args );
				app.execute();
			}
			catch( const std::runtime_error& err ){
				RUNLOGGER.Fatal( uh::toTStr(err.what()), true );
				RUNLOGGER.Log( uh::toTStr("\n�ُ�I��"), true );
				return 1;
			}
			
			RUNLOGGER.Log( uh::toTStr("\n����I��"), true );

			RUNLOGGER.Finalize();
			ERRLOGGER.Finalize();

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
		cerr << "�ُ�I��" << endl;
		return 2;
	}
}
