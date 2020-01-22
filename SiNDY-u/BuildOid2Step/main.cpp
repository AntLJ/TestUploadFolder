#include "stdafx.h"
#define _USE_SINDYLIB_STATIC	// sindylib_base���X�^�e�B�b�N�����N���邽�߂ɕK�v
#include <TDC/sindylib_base/libsindylibbase.h>
#include <TDC/sindylib_core/libsindylibcore.h>
#include "Arguments.h"
#include "BuildOid2Step.h"
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
			CBuildOid2Step app( args );
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
		RUNLOGGER.writeInfo( _T( "�I�������F�ُ�I��" ) );
		return 2;
	}
}
