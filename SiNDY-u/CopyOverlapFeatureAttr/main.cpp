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

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if( !args.parse(argc, argv) )
			{
				cerr << CStringA(AheLoadString(IDS_ARGUMENTERROR)).GetString() << endl;
				return 1;
			}

			// 実処理
			CCopyOverlapFeatureAttr app;
			if( app.execute( args ) )
			{
				// 正常終了
				cerr << CStringA(AheLoadString(IDS_NORMALEND)).GetString() << endl;
				return 0;
			}
			else
			{
				// 異常終了
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
