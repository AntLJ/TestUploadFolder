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

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			// コマンドライン引数処理
			Arguments args;
			if(! args.parse(argc, argv))
			{
				std::cerr << "コマンドライン引数に問題があります" << std::endl;
				return 1;
			}

			LogSys::GetInstance().WriteProcLog(true, true, _T("処理開始\n"));
	
			// 実処理
			CConvertHeightNode app( args );
			if( app.execute() ){
				// 正常終了
				LogSys::GetInstance().WriteProcLog(true, true, _T("正常終了\n"));
				return 0;
			}
			else{
				// 異常終了
				LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
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
		LogSys::GetInstance().WriteProcLog(true, true, _T("異常終了\n"));
		return 2;
	}
}
