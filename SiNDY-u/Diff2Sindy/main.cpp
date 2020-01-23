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
#include "Diff2Sindy.h"
#include "Notifier.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <TDC/useful_headers/scope_guard.h>

using namespace std;

class PrintEndTime
{
public:
	~PrintEndTime(){
		wcout << L"#�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\" << endl;
		wcout << L"# �I������ : " << boost::lexical_cast< std::wstring >( boost::posix_time::second_clock::local_time() ) << endl;
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef _DEBUG
	uh::scope_guard sg( []{ system("pause"); } );
#endif

	int ret = 0;
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		std::wofstream runLogStream;		//!< ���s���O�t�@�C���̏o�̓X�g���[��
		std::wofstream errLogStream;		//!< �G���[���O�t�@�C���̏o�̓X�g���[��

		try
		{
			PrintEndTime ptime;

			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}

			// ���O�t�@�C���p�X�̃`�F�b�N�ƃX�g���[���쐬
			// wcout, wcerr�̏o�͐�����O�X�g���[���ɕύX����
			// �Q�l�FC++: �X�g���[���̏o�͐���t�@�C����W���o�͂ɐ؂�ւ��� http://handasse.blogspot.com/2009/05/c.html
			if( !args.m_runLog.empty() )
			{
				runLogStream.open( args.m_runLog );
				if (! runLogStream )
					notifier::critical::asCannotOpenLogFile( args.m_runLog );
				wcout.rdbuf( runLogStream.rdbuf() );
			}
			if( !args.m_errLog.empty() )
			{
				errLogStream.open( args.m_errLog );
				if (! errLogStream )
					notifier::critical::asCannotOpenLogFile( args.m_errLog );
				wcerr.rdbuf( errLogStream.rdbuf() );
			}

			// ������
			CDiff2Sindy app( args );
			if( app.execute() ){
				// ����I��
				cerr << "����I��" << endl;
				ret = 0;
			}
			else{
				// �ُ�I��
				cerr << "�ُ�I��" << endl;
				ret = 1;
			}
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
		ret = 2;
	}
	return ret;
}
