#include "stdafx.h"
#define _USE_SINDYLIB_STATIC	// sindylib_baseをスタティックリンクするために必要
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
			CBuildOid2Step app( args );
			if( app.execute() ){
				// 正常終了
				RUNLOGGER.writeInfo( _T( "終了時刻：正常終了" ) );
				return 0;
			}
			else{
				// 異常終了
				RUNLOGGER.writeInfo( _T( "終了時刻：異常終了" ) );
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
		RUNLOGGER.writeInfo( _T( "終了時刻：異常終了" ) );
		return 2;
	}
}
