#include "stdafx.h"
#include "CppUnitTest.h"
#include <arctl/coinitializer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// 全テスト実行開始前に１度だけ呼ばれる処理
namespace UnitTest
{	
	TEST_MODULE_INITIALIZE(ModuleStartup)
	{
		static const arctl::coinitializer aCoInitializer;
	}
}