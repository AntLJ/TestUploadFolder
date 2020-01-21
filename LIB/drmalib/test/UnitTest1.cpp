/*
 * Copyright (C) INCREMENT P CORP. All Rights Reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY INCREMENT P CORP., WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT.
 *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDER BE LIABLE FOR ANY
 * CLAIM, DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING
 * OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
*/

﻿#include "stdafx.h"
#include <DRMALayerLoad.h>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace Test
{
	[TestClass]
	public ref class UnitTest1
	{
	public:
		#pragma region Additional test attributes
		//
		//テストを作成しているときに、次の追加属性を使用することができます:
		//
		//クラス内の最初のテストを実行する前にコードを実行するには、ClassInitialize を使用します
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//クラス内のすべてのテストを実行した後にコードを実行するには、ClassCleanup を使用します
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//各テストを実行する前にコードを実行するには、TestInitialize を使用します
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//各テストを実行した後にコードを実行するには、TestCleanup を使用します
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
		#pragma endregion 

		[TestMethod]
		void Bug5369()
		{
			DRMAAdmin2Load adm2load;
			Assert::IsTrue(adm2load.load("Bug5369_1.dat"));
			Assert::AreEqual(static_cast<DRMAAdmin2Load::size_type>(1), adm2load.size());

			// 使用済み基本道路ノード番号が16進化されている
			Assert::AreEqual(0xa035, adm2load.front().eMaxItemId.eBaseRoadNode); 

			// 使用済み全道路ノード番号1が16進化されている
			Assert::AreEqual(0xb1499, adm2load.front().eMaxItemId.eAllRoadNode1);

			// 使用済み全道路ノード番号2が16進化されている
			Assert::AreEqual(0xf5773, adm2load.front().eMaxItemId.eAllRoadNode2);
		};
	};
}
