#include "stdafx.h"
#include "Arguments.h"
#include "ConvertHeightNode.h"
#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "LogSys.h"


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
				std::cerr << "�R�}���h���C�������ɖ�肪����܂�" << std::endl;
				return 1;
			}

			LogSys::GetInstance().WriteProcLog(true, true, _T("�����J�n\n"));
	
			// ������
			CConvertHeightNode app( args );
			if( app.execute() ){
				// ����I��
				LogSys::GetInstance().WriteProcLog(true, true, _T("����I��\n"));
				return 0;
			}
			else{
				// �ُ�I��
				LogSys::GetInstance().WriteProcLog(true, true, _T("�ُ�I��\n"));
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
		std::cerr << e.what() << std::endl;
		LogSys::GetInstance().WriteProcLog(true, true, _T("�ُ�I��\n"));
		return 2;
	}
}
