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

#include "stdafx.h"
#include "CppUnitTest.h"
#include "CheckInit.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace sindy;
using namespace sindy::schema;
using namespace addr;

namespace Test
{
	TEST_CLASS(JusyoHelperTest)
	{
	public:
		// テストクラス初期化処理
		TEST_CLASS_INITIALIZE(ClassInitialize)
		{
			CheckInit::GetInstance();
		}

		// テストクラス終了処理
		TEST_CLASS_CLEANUP(ClassFinalize)
		{
		}

		// 接続テスト1(接続記述子)
		TEST_METHOD(JusyoHelper_Connect1)
		{
			const _TCHAR* lpcszConnectString = _T("release/release@proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszConnectString));

			// コンストラクタからの接続
			CJusyoHelper cHelper_tmp( lpcszConnectString );
			Assert::IsTrue(cHelper_tmp.IsConnected());
		}
	
		// 接続テスト2(ユーザ・パス・サービス分割バージョン)
		TEST_METHOD(JusyoHelper_Connect2)
		{
			const _TCHAR* lpcszUser = _T("release");
			const _TCHAR* lpcszPass = _T("release");
			const _TCHAR* lpcszService = _T("proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszUser, lpcszPass, lpcszService));

			// コンストラクタからの接続
			CJusyoHelper cHelper_tmp(lpcszUser, lpcszPass, lpcszService);
			Assert::IsTrue(cHelper_tmp.IsConnected());
		}

		// 接続テスト3(NGケース)
		TEST_METHOD(JusyoHelper_Connect3)
		{
			CJusyoHelper jusyoHelper;

			// 解析NG
			Assert::IsFalse(jusyoHelper.Connect(_T("")));
			Assert::IsFalse(jusyoHelper.IsConnected());
			Assert::IsFalse(jusyoHelper.Connect(_T("/@")));
			Assert::IsFalse(jusyoHelper.IsConnected());

			// 解析OKで接続NG
			Assert::IsFalse(jusyoHelper.Connect(_T("a/b@c")));
			Assert::IsFalse(jusyoHelper.IsConnected());
		}

		// 検索テスト
		TEST_METHOD(JusyoHelper_Select)
		{
			// 昔はテスト用のユーザを作ってあったようだ(map@delltcp)
			// 今はないのでreleaseユーザからデータを取ってこれるかどうかの検査を行う
			const _TCHAR* lpcszConnectString = _T("release/release@proteus");
			CJusyoHelper jusyoHelper;
			Assert::IsTrue(jusyoHelper.Connect(lpcszConnectString));

			// Areacode_Kanaテーブルから文字列値を取ってみる
			CJusyoRecordSet cRecords;
			CString whereClause;
			whereClause += _T("select * ");
			whereClause += _T("from areacode_kana ");
			whereClause += _T("where ken_code = '03' and shi_code = '201' and oaza_code = '000'");
			Assert::IsTrue(jusyoHelper.Select(whereClause, cRecords));

			// レコード件数は上記条件なら1件
			Assert::AreEqual((long)1, cRecords.GetRecordCount());

			// 各データ型のフィールドからデータを取ってみる
			Assert::AreEqual(_T("03"), cRecords.GetStringValue((long)0)); // ken_code(char)
			Assert::AreEqual(_T("イワテケン"), cRecords.GetStringValue(4)); // ken_kana_d(varchar2)

			// フィールド名で取ってみる
			Assert::AreEqual(_T("03"), cRecords.GetStringValue(_T("ken_code")));
			Assert::AreEqual(_T("イワテケン"), cRecords.GetStringValue(_T("ken_kana_d")));

			// レコード終端の判定
			cRecords.next();
			Assert::IsTrue(cRecords.end());

			// Address_Pointから数値を取ってみる
			whereClause.Empty();
			whereClause += _T("select * ");
			whereClause += _T("from address_point ");
			whereClause += _T("where ken_code = '03' and shi_code = '201' and oaza_code = '000'");
			Assert::IsTrue(jusyoHelper.Select(whereClause, cRecords));
			
			// レコード件数は上記条件なら1件
			Assert::AreEqual((long)1, cRecords.GetRecordCount());

			// もし代表点位置が変わったら個々の条件も適宜変更
			Assert::AreEqual((long)141, cRecords.GetLongValue(12)); // longitude(number)
			Assert::AreEqual((double)39.699249166667, cRecords.GetDoubleValue(13)); // latitude(number)

			Assert::AreEqual((long)141, cRecords.GetLongValue(_T("longitude")));
			Assert::AreEqual((double)39.699249166667, cRecords.GetDoubleValue(_T("latitude")));

			// レコード終端の判定
			cRecords.next();
			Assert::IsTrue(cRecords.end());
		}
	};
}
