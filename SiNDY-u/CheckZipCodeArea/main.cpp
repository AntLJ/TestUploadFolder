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

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if (!args.parse(argc, argv))
			{
				cerr << "コマンドライン引数に問題があります" << endl;
				return 1;
			}

			// 実処理
			CCheckZipCodeArea app(args);
			if (app.execute())
			{
				cerr << "正常終了" << endl;
				return 0;
			}
			else
			{
				cerr << "正常終了(エラーあり)" << endl;
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
		cerr << "異常終了" << endl;
		return 2;
	}
}
