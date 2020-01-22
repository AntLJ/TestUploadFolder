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

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "コマンドライン引数に問題があります" << endl;
				return 1;
			}

			// 実処理
			CAddrPointDiff app( args );
			if( app.execute() ){
				// 正常終了
				RUNLOGGER.Info(_T("正常終了\n"), true);
				RUNLOGGER.Finalize();
				return 0;
			}
			else{
				// 異常終了
				RUNLOGGER.Error(_T("異常終了\n"), true);
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
		RUNLOGGER.Fatal(_T("異常終了\n"), true);
		RUNLOGGER.Fatal((uh::tstring)uh::str_util::format(_T("%s\n"), (CString)e.what()), true);
		RUNLOGGER.Finalize();
		return 2;
	}
}
