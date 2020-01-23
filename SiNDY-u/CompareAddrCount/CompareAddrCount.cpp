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

#include "StdAfx.h"
#include "CompareAddrCount.h"
#include <sindy/workspace.h>

void CompareAddrCount::PrintUsage()
{
	cerr << "【住所データ件数比較ツール】" << endl
		 << "《使用方法》" << endl
		 << "CompareAddrCount.exe [-charaddr] {-ca|-cr <範囲>|-cl <リストファイル>|-cs <5桁コード>} -o <ログファイル>" << endl
		 << endl
		 << "《オプション》" << endl
		 << "\t-charaddr" << endl 
		 << "\t\t文字付き住所データを対象にカウントを実施" << endl
		 << "\t-ca" << endl
		 << "\t\t【デフォルト】全国を対象にカウントを実施" << endl
		 << "\t-cr <開始5桁コード>-<終了5桁コード>" << endl
		 << "\t\t指定した5桁コードの範囲を対象にカウントを実施" << endl
		 << "\t-cl <リストファイル>" << endl
		 << "\t\tリストファイルに格納されている住所コードリスト(5/8/11桁)を対象にカウントを実施" << endl
		 << "\t-cs <住所コード(5/8/11桁)>" << endl
		 << "\t\t指定した住所コードを対象にカウントを実施" << endl
		 << "\t-o <ログファイル>" << endl
		 << "\t\t【必須】出力先ログファイルを指定" << endl
		 << endl
		 << "《環境変数》" << endl
		 << "\tDB_ADDRESS_OLD" << endl
		 << "\t\t住所系SDE接続先(旧)" << endl
		 << "\tDB_ADDRESS_NEW" << endl
		 << "\t\t住所系SDE接続先(新)" << endl
		 << "\tFC_GOU_POINT" << endl
		 << "\t\t号ポイントフィーチャクラス名" << endl
		 << "\tFC_CITY_ADMIN" << endl
		 << "\t\t都市地図行政界フィーチャクラス名" << endl
		 << "\tFC_CS_ADDR_POINT" << endl
		 << "\t\tCSポイントフィーチャクラス名" << endl
		 << "\tJDB_CONNECT_OLD" << endl
		 << "\t\t住所DB接続先(旧)" << endl
		 << "\tJDB_CONNECT_NEW" << endl
		 << "\t\t住所DB接続先(新)" << endl
		 << endl;
}

bool CompareAddrCount::Init(int cArgc, _TCHAR *cArgv[])
{
	// 引数がない場合は使用方法を表示して終了
	if(cArgc == 1) {
		PrintUsage();
		return false;
	}

	// 引数・環境変数の取得
	if(!fnAnalyzeArg(cArgc, cArgv))	return false;
	if(!fnGetEnvVar())	return false;

	// ログファイルのオープン
	locale aLoc(locale("Japanese"), "C", locale::numeric);
	mLog.open(static_cast<CT2CW>(mLogFilename));
	if(!mLog.is_open()) {
		cerr << "#Error ログファイルが開けません : " << CT2A(mLogFilename) << endl;
		return false;
	}
	mLog.imbue(aLoc);

	// 住所DB接続
	if(!mACHelperOld.Connect(mJDBOld)) {
		cerr << "#Error 住所DB(旧)に接続できません : " << CT2A(mJDBOld) << endl;
		return false;
	}
	if(!mACHelperNew.Connect(mJDBNew)) {
		cerr << "#Error 住所DB(新)に接続できません : " << CT2A(mJDBNew) << endl;
		return false;
	}

	// 各DBからフィーチャクラスを取得
	IFeatureClassPtr ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld;
	IFeatureClassPtr ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew;
	if(!fnGetFeatureClasses(mDBOld, ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld)) return false;
	if(!fnGetFeatureClasses(mDBNew, ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew)) return false;

	// カウントクラスの初期化
	if(!mAddrCountOld.Init(mCharF, mTarget, mTgtOpt, ipGouPointOld, ipCityAdminOld, ipCSAddrPointOld)) return false;
	if(!mAddrCountNew.Init(mCharF, mTarget, mTgtOpt, ipGouPointNew, ipCityAdminNew, ipCSAddrPointNew)) return false;

	return true;
}

bool CompareAddrCount::Execute()
{
	cerr << "旧DBでのカウントを開始" << endl;
	if(!mAddrCountOld.Count()) return false;

	cerr << "新DBでのカウントを開始" << endl;
	if(!mAddrCountNew.Count()) return false;

	// ログへの結果出力
	for(AddrCount::const_iterator itrOld = mAddrCountOld.begin(), itrNew = mAddrCountNew.begin();
		itrOld != mAddrCountOld.end() || itrNew != mAddrCountNew.end(); )
	{
		CString aShiko, aHaishi;

		if(itrOld != mAddrCountOld.end() && (itrNew == mAddrCountNew.end() || (*itrOld).first < (*itrNew).first))
		{
			// 「旧」レコードのみ出力
			fnGetShikoHaishiDate((*itrOld).first, aShiko, aHaishi);
			mLog << CT2A((*itrOld).first) << "\t" << CT2A(mACHelperOld.GetAddrKanji((*itrOld).first)) << "\t" << (*itrOld).second << "\t0\t" << -(*itrOld).second << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t0件に変化" << endl;
			++itrOld;
		}
		else if(itrNew != mAddrCountNew.end() && (itrOld == mAddrCountOld.end() || (*itrOld).first > (*itrNew).first))
		{
			// 「新」レコードのみ出力
			fnGetShikoHaishiDate((*itrNew).first, aShiko, aHaishi);
			mLog << CT2A((*itrNew).first) << "\t" << CT2A(mACHelperNew.GetAddrKanji((*itrNew).first))<< "\t0\t" << (*itrNew).second << "\t" << (*itrNew).second << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t" << endl;
			++itrNew;
		}
		else
		{
			// 両レコードを出力
			fnGetShikoHaishiDate((*itrNew).first, aShiko, aHaishi);
			int aDiff = (*itrNew).second - (*itrOld).second;
			mLog << CT2A((*itrOld).first) << "\t" << CT2A(mACHelperNew.GetAddrKanji((*itrNew).first)) << "\t" << (*itrOld).second << "\t" << (*itrNew).second << "\t" << aDiff << "\t"
				<< CT2A(aShiko) << "\t" << CT2A(aHaishi) << "\t";
			if((*itrOld).second != 0 && (*itrNew).second == 0) {
				mLog << "0件に変化" << endl;
			}
			else if((*itrOld).second >= 2 && (*itrOld).second / 2.0 >= (*itrNew).second) {
				mLog << "半分以下に変化" << endl;
			}
			else if(aDiff <= -20) {
				mLog << "20件以上減少" << endl;
			}
			else {
				mLog << endl;
			}
			++itrOld;
			++itrNew;
		}
	}

	mLog.close();

	return true;
}

bool CompareAddrCount::fnAnalyzeArg(int cArgc, _TCHAR *cArgv[])
{
	for(int i = 1; i < cArgc; ++i) {
		CString aTmp1(cArgv[i]);
		if(aTmp1.Left(1) != _T("-")) {
			cerr << "#Error 引数が不正です" << endl;
			return false;
		}

		if(aTmp1 == _T("-charaddr")) {
			mCharF = true;
			continue;
		}
		if(aTmp1 == "-ca") {
			mTarget = kTgtAll;
			continue;
		}

		if(i + 1 < cArgc) {
			CString aTmp2(cArgv[++i]);
			if(aTmp1 == "-cr") {
				mTarget = kTgtRange;
				mTgtOpt = aTmp2;
				if(mTgtOpt.GetLength() != 11 || mTgtOpt.Find(_T('-')) != 5) {
					cerr << "#Error 範囲指定が不正です : " << CT2A(mTgtOpt) << endl;
					return false;
				}
				continue;
			}
			if(aTmp1 == "-cl") {
				mTarget = kTgtList;
				mTgtOpt = aTmp2;
				// ここでファイルの存在チェックをする？
				continue;
			}
			if(aTmp1 == "-cs") {
				mTarget = kTgtSingle;
				mTgtOpt = aTmp2;
				if(mTgtOpt.GetLength() != 5 && mTgtOpt.GetLength() != 8 && mTgtOpt.GetLength() != 11) {
					cerr << "#Error 住所コード指定が不正です(5/8/11桁のみ可) : " << CT2A(mTgtOpt) << endl;
					return false;
				}
				continue;
			}
			if(aTmp1 == "-o") {
				mLogFilename = aTmp2;
				continue;
			}
		}

		cerr << "#Error 引数が不正です" << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetEnvVar()
{
	if(!mDBOld.GetEnvironmentVariable(_T("DB_ADDRESS_OLD"))) {
		cerr << "#Error 環境変数 DB_ADDRESS_OLD の値が取得できません" << endl;
		return false;
	}

	if(!mDBNew.GetEnvironmentVariable(_T("DB_ADDRESS_NEW"))) {
		cerr << "#Error 環境変数 DB_ADDRESS_NEW の値が取得できません" << endl;
		return false;
	}

	if(!mFCGouPoint.GetEnvironmentVariable(_T("FC_GOU_POINT"))) {
		cerr << "#Error 環境変数 FC_GOU_POINT の値が取得できません" << endl;
		return false;
	}

	if(!mFCCityAdmin.GetEnvironmentVariable(_T("FC_CITY_ADMIN"))) {
		cerr << "#Error 環境変数 FC_CITY_ADMIN の値が取得できません" << endl;
		return false;
	}

	if(!mFCCSAddrPoint.GetEnvironmentVariable(_T("FC_CS_ADDR_POINT"))) {
		cerr << "#Error 環境変数 FC_CS_ADDR_POINT の値が取得できません" << endl;
		return false;
	}

	if(!mJDBOld.GetEnvironmentVariable(_T("JDB_CONNECT_OLD"))) {
		cerr << "#Error 環境変数 JDB_CONNECT_OLD の値が取得できません" << endl;
		return false;
	}

	if(!mJDBNew.GetEnvironmentVariable(_T("JDB_CONNECT_NEW"))) {
		cerr << "#Error 環境変数 JDB_CONNECT_NEW の値が取得できません" << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetFeatureClasses(const CString& cDB, IFeatureClassPtr& cGouPoint, IFeatureClassPtr& cCityAdmin, IFeatureClassPtr& cCSAddrPoint)
{
	IWorkspacePtr ipWorkspace;
	INamePtr ipName = sindy::create_workspace_name(cDB);
	if(ipName) {
		IUnknownPtr ipUnknown;
		ipName->Open(&ipUnknown);
		ipWorkspace = ipUnknown;
	}

	if(!ipWorkspace) {
		cerr << "#Error SDE接続に失敗 : " << CT2A(cDB) << endl;
		return false;
	}
	cout << "SDE接続 : " << CT2A(cDB) << endl;

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCGouPoint), &cGouPoint);
	if(!cGouPoint) {
		cerr << "#Error 号ポイントフィーチャクラスの取得に失敗 : " << CT2A(mFCGouPoint) << endl;
		return false;
	}

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCCityAdmin), &cCityAdmin);
	if(!cCityAdmin) {
		cerr << "#Error 都市地図行政界フィーチャクラスの取得に失敗 : " << CT2A(mFCCityAdmin) << endl;
		return false;
	}

	IFeatureWorkspacePtr(ipWorkspace)->OpenFeatureClass(CComBSTR(mFCCSAddrPoint), &cCSAddrPoint);
	if(!cCSAddrPoint) {
		cerr << "#Error 都市地図行政界フィーチャクラスの取得に失敗 : " << CT2A(mFCCSAddrPoint) << endl;
		return false;
	}

	return true;
}

bool CompareAddrCount::fnGetShikoHaishiDate(const CString& cAddrCode, CString& cShikoDate, CString& cHaishiDate)
{
	CString aSQL;
	aSQL.Format(_T("select Shiko_Date, Haishi_Date from Areacode_Master where Ken_Code = '%s' and Shi_Code = '%s' and Oaza_Code = '%s' and Aza_Code = '%s'"),
		cAddrCode.Left(2), cAddrCode.Mid(2, 3), cAddrCode.Mid(5, 3), cAddrCode.Mid(8, 3));

	CJusyoRecordSet aRecSet;
	if(mACHelperNew.Select(aSQL, aRecSet) && aRecSet.GetRecordCount() == 1) {
		cShikoDate	= aRecSet.GetStringValue(0L);
		cHaishiDate	= aRecSet.GetStringValue(1L);
	} else {
//		cerr << "#Notice 施行年月・廃止年月を正しく取得できません : " << CT2A(cAddrCode) << endl;
		return false;
	}

	return true;
}
