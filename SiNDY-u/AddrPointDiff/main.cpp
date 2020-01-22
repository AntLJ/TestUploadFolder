#include "stdafx.h"
#include "Arguments.h"
#include "AddrPointDiff.h"
#include "Logger.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>

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
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}

			// ������
			CAddrPointDiff app( args );
			if( app.execute() ){
				// ����I��
				RUNLOGGER.Info(_T("����I��\n"), true);
				RUNLOGGER.Finalize();
				return 0;
			}
			else{
				// �ُ�I��
				RUNLOGGER.Error(_T("�ُ�I��\n"), true);
				RUNLOGGER.Finalize();
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
		RUNLOGGER.Fatal(_T("�ُ�I��\n"), true);
		RUNLOGGER.Fatal((uh::tstring)uh::str_util::format(_T("%s\n"), (CString)e.what()), true);
		RUNLOGGER.Finalize();
		return 2;
	}
}
