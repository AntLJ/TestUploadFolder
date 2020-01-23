#include "stdafx.h"
#include "Arguments.h"
#include "CheckZipCodeArea.h"
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
			if (!args.parse(argc, argv))
			{
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}

			// ������
			CCheckZipCodeArea app(args);
			if (app.execute())
			{
				cerr << "����I��" << endl;
				return 0;
			}
			else
			{
				cerr << "����I��(�G���[����)" << endl;
				return 1;
			}
		}
		catch (const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch (const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "�ُ�I��" << endl;
		return 2;
	}
}
