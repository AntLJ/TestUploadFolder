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
#include <arctl/ui/workspace_dialog.h>

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

using namespace arctl::ui;

namespace UITest
{
	[TestClass]
	public ref class WorkspaceDialogTest
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
		
		//各テストを実行する前にコードを実行するには、TestInitialize を使用します
		[TestInitialize()]
		void MyTestInitialize()
		{
			m_lpszInitalDir = new wchar_t[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, m_lpszInitalDir);

			m_pDlg = new CWorkspaceDialog;
			m_pDlg->m_OFN.lpstrInitialDir = m_lpszInitalDir;
		};
		
		//各テストを実行した後にコードを実行するには、TestCleanup を使用します
		[TestCleanup()]
		void MyTestCleanup()
		{
			delete m_pDlg;
			delete [] m_lpszInitalDir;
		};
		
		#pragma endregion 

		[TestMethod, Description(L"Open \"dummy.sde\" file.")]
		void TestOpenLocalSde()
		{
			m_pDlg->m_OFN.lpstrTitle = L"\"dummy.sde\" ファイルを開いてください";

			Assert::AreNotEqual(0, m_pDlg->DoModal(0));
			Assert::AreEqual(
				IO::Path::Combine(gcnew String(m_lpszInitalDir), L"dummy.sde"),
				gcnew String(m_pDlg->m_OFN.lpstrFile)
			);
		};

		[TestMethod, Description(L"Open \"dummy.mdb\" file.")]
		void TestOpenMdb()
		{
			m_pDlg->m_OFN.lpstrTitle = L"\"dummy.mdb\" ファイルを開いてください";

			Assert::AreNotEqual(0, m_pDlg->DoModal(0));
			Assert::AreEqual(
				IO::Path::Combine(gcnew String(m_lpszInitalDir), L"dummy.mdb"),
				gcnew String(m_pDlg->m_OFN.lpstrFile)
			);
		};

		[TestMethod, Description(L"Open \"dummy.pgs\" file.")]
		void TestOpenLocalPgs()
		{
			m_pDlg->m_OFN.lpstrTitle = L"\"dummy.pgs\" ファイルを開いてください";

			Assert::AreNotEqual(0, m_pDlg->DoModal(0));
			Assert::AreEqual(
				IO::Path::Combine(gcnew String(m_lpszInitalDir), L"dummy.pgs"),
				gcnew String(m_pDlg->m_OFN.lpstrFile)
			);
		};

		[TestMethod, Description(L"Open \"dummy.shp\" file.")]
		void TestOpenShp()
		{
			m_pDlg->m_OFN.lpstrTitle = L"\"dummy.shp\" ファイルを開いてください";

			Assert::AreNotEqual(0, m_pDlg->DoModal(0));
			Assert::AreEqual(
				gcnew String(m_lpszInitalDir),
				gcnew String(m_pDlg->m_OFN.lpstrFile)
			);
		};

		[TestMethod, Description(L"Free test.")]
		void TestOther()
		{
			m_pDlg->m_OFN.lpstrTitle = L"フリーテストを終えるにはキャンセルを押してください";
			m_pDlg->m_OFN.nFilterIndex = 1;

			while(m_pDlg->DoModal(0) != 0) {
				m_pDlg->m_OFN.lpstrFile[0] = 0;
				m_pDlg->m_OFN.lpstrFileTitle[0] = 0;
			}
		};

	private:
		wchar_t* m_lpszInitalDir;
		arctl::ui::CWorkspaceDialog* m_pDlg;
	};
}
