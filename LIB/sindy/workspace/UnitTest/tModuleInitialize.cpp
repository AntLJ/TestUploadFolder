#include "stdafx.h"
#include "CppUnitTest.h"
#include <arctl/coinitializer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// �S�e�X�g���s�J�n�O�ɂP�x�����Ă΂�鏈��
namespace UnitTest
{	
	TEST_MODULE_INITIALIZE(ModuleStartup)
	{
		static const arctl::coinitializer aCoInitializer;
	}
}