#include "stdafx.h"
#include "CppUnitTest.h"
#include "../name.h"
#include <filesystem>
#include <vector>
#include <arctl/coinitializer.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// ★テスト用設定値 -----------------------------------------
// 正常接続用
const CString TEST_NORMAL_SERVER = _T("coral2");
const CString TEST_NORMAL_USER = _T("ronly");
const CString TEST_NORMAL_PASS = _T("ronly");
const CString TEST_NORMAL_VERSION = _T("SDE.DEFAULT");
const CString TEST_NORMAL_PORT = _T("5151");

// 異常接続用
const CString TEST_ERROR_SERVER = _T("yakumo10");
const CString TEST_ERROR_USER = _T("csetest10");
const CString TEST_ERROR_PASS = _T("csetest20");
const CString TEST_ERROR_VERSION = _T("SDE.DEFAULT10");
const CString TEST_ERROR_PORT = _T("5050");
// ★--------------------------------------------------------

// プロパティ名確認用固定文字列
const CString ITEM_SERVER				= _T("SERVER");
const CString ITEM_INSTANCE				= _T("INSTANCE");
const CString ITEM_DBCLIENT				= _T("DBCLIENT");
const CString ITEM_DB_CONN_PROPERTIES	= _T("DB_CONNECTION_PROPERTIES");

// 設定値確認用固定文字列
const CString AS_INSTANCE = _T("5151");
const CString DC_INSTANCE = _T("sde:oracle11g:");
const CString DC_DBCLIENT = _T("oracle");

// 接続用文字列
const CString TOKEN_AT				= _T("@");
const CString TOKEN_SLASH			= _T("/");
const CString TOKEN_FRONT_BRACKET	= _T("(");
const CString TOKEN_REAR_BRACKET	= _T(")");
const CString TOKEN_NONE			= _T("");
const CString TOKEN_PREFIX_AS		= _T("[AS]");

// 接続文字列パターン
const int CONN_PTN_1 = 1;	// user@server(version)
const int CONN_PTN_2 = 2;	// user/password/version/port/server
const int CONN_PTN_3 = 3;	// user/password/version/server
const int CONN_PTN_4 = 4;	// [AS]user@server(version)
const int CONN_PTN_5 = 5;	// [AS]user/password/version/port/server

namespace test_name
{
	TEST_CLASS(create_workspace)
	{
	private:
		// 接続プロパティ情報
		struct PROPERTY_INFO
		{
			CString csPropertieName;		// プロパティ名
			CString csValue;				// 設定値
		};

		/**
		* @brief excelファイル読み込み。
		*        開いて、最初のシートをITabeleとして読み込めれば成功
		*/
		void openExcel( const CString& filePath )
		{
			// Excelファイルオープン
			IWorkspacePtr ipExcelWorkspace = sindy::create_workspace(filePath);
			Assert::IsNotNull<IWorkspace>(ipExcelWorkspace, _T("ファイルオープン失敗： ") + filePath);

			// シート名取得準備
			IEnumDatasetNamePtr ipEnumDatasetName;
			ipExcelWorkspace->get_DatasetNames(esriDTTable, &ipEnumDatasetName);

			// 最初のシートを取得
			IDatasetNamePtr ipDatasetName;
			//auto hr = ipEnumDatasetName->Reset(); ← これをやると何故かNext()で失敗する。。（え
			auto hr = ipEnumDatasetName->Next(&ipDatasetName);
			Assert::IsNotNull<IDatasetName>(ipDatasetName, _T("シート取得失敗") );
			
			// シート名取得
			CComBSTR bstr;
			ipDatasetName->get_Name(&bstr);

			// シートを開く
			ITablePtr ipTable;
			((IFeatureWorkspacePtr)ipExcelWorkspace)->OpenTable(bstr, &ipTable);
			Assert::IsNotNull<ITable>(ipTable, _T("シートオープン失敗： ") + CString(bstr) );
		}

		/**
		* @brief サーバ接続がダイレクトコネクションか、プロパティ情報よりチェックする
		*/
		void checkConnection(std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			bool bHasInstance = false;
			bool bHasDBClient = false;
			bool bHasDBConnProperties = false;

			auto itr = vecPropertyInfo.begin();
			for (; itr != vecPropertyInfo.end(); ++itr) {
				CString csItem = itr->csPropertieName;
				CString csVal = itr->csValue;
				if (ITEM_SERVER == csItem) {
					// SERVER の場合、AS接続
					Assert::Fail("コネクション種別がDCではありません [" + ITEM_SERVER + "：" + csVal + "]");
				}
				else if (ITEM_INSTANCE == csItem) {
					// INSTANCE の場合
					bHasInstance = true;

					CString cstmp = DC_INSTANCE + TEST_NORMAL_SERVER;
					Assert::AreEqual(cstmp.GetString(), csVal.GetString(), "コネクション種別がDCではありません [" + ITEM_INSTANCE + "]： " + csVal);
				}
				else if (ITEM_DBCLIENT == csItem) {
					// DBCLIENT の場合
					bHasDBClient = true;

					Assert::AreEqual(DC_DBCLIENT.GetString(), csVal.GetString(), "ダイレクトコネクションの" + ITEM_DBCLIENT + "プロパティが異なっています [" + ITEM_DBCLIENT + "]： " + csVal);
				}
				else if (ITEM_DB_CONN_PROPERTIES == csItem) {
					// DB_CONNECTION_PROPERTIES の場合
					bHasDBConnProperties = true;

					Assert::AreEqual(TEST_NORMAL_SERVER.GetString(), csVal.GetString(), "ダイレクトコネクションの" + ITEM_DB_CONN_PROPERTIES + "プロパティが異なっています [" + ITEM_DB_CONN_PROPERTIES + "]： " + csVal);
				}
				else {
					// 他の項目はチェックしない
				}
			}

			if (!bHasInstance) {
				Assert::Fail(ITEM_INSTANCE + "プロパティが存在しません");
			}
			if (!bHasDBClient) {
				Assert::Fail(ITEM_DBCLIENT + "プロパティが存在しません");
			}
			if (!bHasDBConnProperties) {
				Assert::Fail(ITEM_DB_CONN_PROPERTIES + "プロパティが存在しません");
			}
		}

		/**
		* @brief サーバ接続がアプリケーション接続、プロパティ情報よりチェックする
		*/
		void checkConnection_as(std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			bool bHasServer   = false;
			bool bHasInstance = false;

			auto itr = vecPropertyInfo.begin();
			for (; itr != vecPropertyInfo.end(); ++itr) {
				CString csItem = itr->csPropertieName;
				CString csVal = itr->csValue;
				if (ITEM_SERVER == csItem) {
					// SERVER の場合
					bHasServer = true;

					Assert::AreEqual(TEST_NORMAL_SERVER.GetString(), csVal.GetString(), "アプリケーション接続の" + ITEM_SERVER + "プロパティが異なっています [" + ITEM_SERVER + "]： " + csVal);
				}
				else if (ITEM_INSTANCE == csItem) {
					// INSTANCE の場合
					bHasInstance = true;

					Assert::AreEqual(AS_INSTANCE.GetString(), csVal.GetString(), "コネクション種別がASではありません [" + ITEM_INSTANCE + "]： " + csVal);
				}
				else if (ITEM_DB_CONN_PROPERTIES == csItem) {
					// DB_CONNECTION_PROPERTIES の場合、ダイレクトコネクション

					Assert::Fail("コネクション種別がASではありません [" + ITEM_DB_CONN_PROPERTIES + "：" + csVal + "]");
				}
				else {
					// 他の項目はチェックしない
				}
			}

			if (!bHasServer) {
				Assert::Fail(ITEM_SERVER + "プロパティが存在しません");
			}
			if (!bHasInstance) {
				Assert::Fail(ITEM_INSTANCE + "プロパティが存在しません");
			}
		}

		/**
		* @brief サーバ接続した際のプロパティ情報を取得し、コンテナに詰める
		*/
		void getPropertyInfo(IWorkspacePtr& ipSDEWorkspace, std::vector<PROPERTY_INFO>& vecPropertyInfo)
		{
			// 接続プロパティ情報取得
			IPropertySetPtr ipPropertySetAfterConn;
			ipSDEWorkspace->get_ConnectionProperties(&ipPropertySetAfterConn);
			CComVariant vNames, vValues;
			ipPropertySetAfterConn->GetAllProperties(&vNames, &vValues);
			long lPropertieCnt;
			ipPropertySetAfterConn->get_Count(&lPropertieCnt);
			SAFEARRAY *pSarName = vNames.parray;
			SAFEARRAY *pSarVal = vValues.parray;

			// プロパティ情報をコンテナに詰める
			for (long i = 0; i < lPropertieCnt; i++) {
				PROPERTY_INFO tmpProperty;
				CString csName;
				VARIANT vVal;
				SafeArrayGetElement(pSarName, &i, (void**)&csName);
				SafeArrayGetElement(pSarVal, &i, (void**)&vVal);

				tmpProperty.csPropertieName = csName.GetString();
				tmpProperty.csValue = vVal.bstrVal;
				vecPropertyInfo.push_back(tmpProperty);
			}
		}

		/**
		* @brief 接続文字列でサーバ接続を行う
		*        ダイレクトコネクションで接続されていればテスト成功
		*/
		void connectServerTestNormal(const CString& csDBConnect, bool checkDC)
		{
			// 指定サーバへ接続
			IWorkspacePtr ipSDEWorkspace = sindy::create_workspace(csDBConnect);
			Assert::IsNotNull<IWorkspace>(ipSDEWorkspace, "サーバ接続に失敗しました [接続文字列：" + csDBConnect + "]");

			// 接続情報取得
			std::vector<PROPERTY_INFO> vecPropertyInfo;
			getPropertyInfo(ipSDEWorkspace, vecPropertyInfo);

			// 接続が意図通りの方式であるか確認
			checkDC ? checkConnection(vecPropertyInfo) : checkConnection_as(vecPropertyInfo);
		}

		/**
		* @brief 不正の接続文字列でサーバ接続を行う
		*        接続できなければテスト成功
		*/
		void connectServerTestError(const CString& csDBConnect)
		{
			// 指定サーバへ接続
			IWorkspacePtr ipSDEWorkspace = sindy::create_workspace(csDBConnect);
			Assert::IsNull<IWorkspace>(ipSDEWorkspace, "異常な接続文字列でサーバ接続されました [接続文字列：" + csDBConnect + "]");
		}

		/**
		* @brief 指定パターンのサーバ接続用文字列を返す
		*/
		CString makeConnectString(const int iPtn,
								const CString& csServer,
								const CString& csUser,
								const CString& csPass,
								const CString& csVersion,
								const CString& csPort)
		{
			CString csReturn;
			switch (iPtn) {
				case CONN_PTN_1:
					// user@server(version)
					csReturn = csUser + TOKEN_AT + csServer + TOKEN_FRONT_BRACKET + csVersion + TOKEN_REAR_BRACKET;
					break;

				case CONN_PTN_2:
					// user/password/version/port/server
					csReturn = csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csPort + TOKEN_SLASH + csServer;
					break;

				case CONN_PTN_3:
					// user/password/version/server
					csReturn = csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csServer;
					break;
				case CONN_PTN_4:
					// [AS]user@server(version)
					csReturn = TOKEN_PREFIX_AS + csUser + TOKEN_AT + csServer + TOKEN_FRONT_BRACKET + csVersion + TOKEN_REAR_BRACKET;
					break;
				case CONN_PTN_5:
					// [AS]user/password/version/port/server
					csReturn = TOKEN_PREFIX_AS + csUser + TOKEN_SLASH + csPass + TOKEN_SLASH + csVersion + TOKEN_SLASH + csPort + TOKEN_SLASH + csServer;
					break;
				default:
					break;
			}
			return csReturn;
		}

	public:
		/**
		* @brief .xlsファイル読み込みのテスト
		*/
		TEST_METHOD(create_workspace_excel_xls)
		{
			openExcel(_T("../data/_2003.xls"));
		}
		
		/**
		* @brief .xlsxファイル読み込みのテスト
		* @note このテストでファイルオープンに失敗した場合、下記を参考にして再実施すること。
		*       http://confluence.ipc.pioneer.co.jp/confluence/pages/viewpage.action?pageId=35727413
		*/
		TEST_METHOD(create_workspace_excel_xlsx)
		{
			openExcel(_T("../data/2007_.xlsx"));
		}

		/**
		* @brief ダイレクトコネクション 正常接続　接続文字列パターン1 [user@server(version)]
		* @note 
		*/
		TEST_METHOD(normal_ptn1)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief ダイレクトコネクション 正常接続　接続文字列パターン2 [user/password/version/port/server]
		* @note
		*/
		TEST_METHOD(normal_ptn2)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief ダイレクトコネクション 正常接続　接続文字列パターン2 [user/password/version/port/server] （誤ったポートを指定）
		* @note DC接続ではポートの値は無視されるので正常接続できる
		*/
		TEST_METHOD(normal_ptn2_port_miss)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_ERROR_PORT);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief ダイレクトコネクション 正常接続　接続文字列パターン3 [user/password/version/server]
		* @note
		*/
		TEST_METHOD(normal_ptn3)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, true);
		}

		/**
		* @brief アプリケーション接続 正常接続　接続文字列パターン4 [[AS]user@server(version)]/
		* @note
		*/
		TEST_METHOD(normal_ptn4)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestNormal(csConnStr, false);
		}

		/**
		* @brief アプリケーション接続  正常接続　接続文字列パターン5 [[AS]user/password/version/port/server]
		* @note
		*/
		TEST_METHOD(normal_ptn5)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestNormal(csConnStr, false);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(存在しないサーバ指定)
		* @note
		*/
		TEST_METHOD(error_ptn1_server_notexist)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_ERROR_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(サーバ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn1_server_notspecified)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TOKEN_NONE, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(存在しないユーザ指定)
		* @note
		*/
		TEST_METHOD(error_ptn1_user_notexist)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_ERROR_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(ユーザ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn1_user_notspecified)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TOKEN_NONE, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(存在しないバージョン)
		* @note
		*/
		TEST_METHOD(error_ptn1_version_notexist)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン1　(バージョン指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn1_version_notspecified)
		{
			// SDE接続文字列パターン1
			CString csConnStr = makeConnectString(CONN_PTN_1, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(存在しないサーバ指定)
		* @note
		*/
		TEST_METHOD(error_ptn2_server_notexist)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(サーバ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn2_server_notspecified)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(存在しないユーザ指定)
		* @note
		*/
		TEST_METHOD(error_ptn2_user_notexist)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(ユーザ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn2_user_notspecified)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(パスワード指定誤り)
		* @note
		*/
		TEST_METHOD(error_ptn2_pass_miss)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(パスワード指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn2_pass_notspecified)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(存在しないバージョン指定)
		* @note
		*/
		TEST_METHOD(error_ptn2_version_notexist)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(バージョン指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn2_version_notspecified)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン2　(ポート指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn2_port_notspecified)
		{
			// SDE接続文字列パターン2
			CString csConnStr = makeConnectString(CONN_PTN_2, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(存在しないサーバ指定)
		* @note
		*/
		TEST_METHOD(error_ptn3_server_notexist)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(サーバ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn3_server_notspecified)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(存在しないユーザ指定)
		* @note
		*/
		TEST_METHOD(error_ptn3_user_notexist)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(ユーザ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn3_user_notspecified)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(パスワード指定誤り)
		* @note
		*/
		TEST_METHOD(error_ptn3_pass_notexist)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(パスワード指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn3_pass_notspecified)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(存在しないバージョン指定)
		* @note
		*/
		TEST_METHOD(error_ptn3_version_notexist)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief ダイレクトコネクション 異常接続　接続文字列パターン3　(バージョン指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn3_version_notspecified)
		{
			// SDE接続文字列パターン3
			CString csConnStr = makeConnectString(CONN_PTN_3, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(存在しないサーバ指定)
		* @note
		*/
		TEST_METHOD(error_ptn4_server_notexist)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_ERROR_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(サーバ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn4_server_notspecified)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TOKEN_NONE, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(存在しないユーザ指定)
		* @note
		*/
		TEST_METHOD(error_ptn4_user_notexist)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_ERROR_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(ユーザ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn4_user_notspecified)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TOKEN_NONE, TOKEN_NONE, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(存在しないバージョン)
		* @note
		*/
		TEST_METHOD(error_ptn4_version_notexist)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_ERROR_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン4　(バージョン指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn4_version_notspecified)
		{
			// SDE接続文字列パターン4
			CString csConnStr = makeConnectString(CONN_PTN_4, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TOKEN_NONE, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(存在しないサーバ指定)
		* @note
		*/
		TEST_METHOD(error_ptn5_server_notexist)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_ERROR_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(サーバ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn5_server_notspecified)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TOKEN_NONE, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(存在しないユーザ指定)
		* @note
		*/
		TEST_METHOD(error_ptn5_user_notexist)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_ERROR_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(ユーザ指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn5_user_notspecified)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TOKEN_NONE, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(パスワード指定誤り)
		* @note
		*/
		TEST_METHOD(error_ptn5_pass_miss)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_ERROR_PASS, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(パスワード指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn5_pass_notspecified)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TOKEN_NONE, TEST_NORMAL_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(存在しないバージョン指定)
		* @note
		*/
		TEST_METHOD(error_ptn5_version_notexist)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_ERROR_VERSION, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(バージョン指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn5_version_notspecified)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TOKEN_NONE, TEST_NORMAL_PORT);
			connectServerTestError(csConnStr);
		}

		/**
		* @brief アプリケーション接続 異常接続　接続文字列パターン5　(ポート指定なし)
		* @note
		*/
		TEST_METHOD(error_ptn5_port_notspecified)
		{
			// SDE接続文字列パターン5
			CString csConnStr = makeConnectString(CONN_PTN_5, TEST_NORMAL_SERVER, TEST_NORMAL_USER, TEST_NORMAL_PASS, TEST_NORMAL_VERSION, TOKEN_NONE);
			connectServerTestError(csConnStr);
		}
	};
}