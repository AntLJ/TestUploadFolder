#include "stdafx.h"
#include "Arguments.h"
#include "resource.h"
#include "CopyOverlapFeatureAttr.h"

#include <arctl/coinitializer.h>
#include <atl2/exception.h>
#include <TDC/useful_headers/japanese_locale.h>
#include <ArcHelperEx/AheglobalsMisc.h>

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
			if( !args.parse(argc, argv) )
			{
				cerr << CStringA(AheLoadString(IDS_ARGUMENTERROR)).GetString() << endl;
				return 1;
			}

			// ������
			CCopyOverlapFeatureAttr app;
			if( app.execute( args ) )
			{
				// ����I��
				cerr << CStringA(AheLoadString(IDS_NORMALEND)).GetString() << endl;
				return 0;
			}
			else
			{
				// �ُ�I��
				cerr << CStringA(AheLoadString(IDS_ABNORMALEND)).GetString() << endl;
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
		cerr << e.what() << endl;
		cerr << CStringA(AheLoadString(IDS_ABNORMALEND)).GetString() << endl;
		return 2;
	}
}
