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
#include "main.h"
#include "Arguments.h"
#include "CheckSplit.h"
#include <WinLib/VersionInfo.h>


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			time_t st_time;
			time( &st_time );

			// コマンドライン引数処理
			Arguments args;
			if(!args.parse(argc, argv))
			{
				std::cerr << "コマンドライン引数に問題があります" << std::endl;
				return 1;
			}
			// ログファイル作成
			if(!InitLog(args)){
				CLog::GetInstance().PrintLog(true, false, false, false, _T("異常終了"));
				return 1;
			}
			// crd_cnv設定
			if(!args.Tky2JgdPath.empty()){
				if(!g_cnv.JGD2000_RP_Init(CT2A(args.Tky2JgdPath.c_str())))
					return 1;
			}

			// チェック開始
			bool bRun = false, bCheckErr = false;
			CCheckSplit cCheckSplit;
			if(cCheckSplit.Init(args)){
				if(cCheckSplit.run(bCheckErr)){
					// 正常終了だがチェックエラーがあった場合
					if(bCheckErr){
						CLog::GetInstance().PrintLog(true, false, true, false, _T("チェックエラーあり"));
					} else {
						CLog::GetInstance().PrintLog(true, false, true, false, _T("チェックエラーなし"));
					}
					bRun = true;
				}
			}

			time_t end_time;
			time( &end_time );

			CString strTotal;
			strTotal.Format( _T("\n# トータル時間 %.2f[sec]\n"), difftime( end_time, st_time ) );
			CLog::GetInstance().PrintLog(true, false, true, false, strTotal);

			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			CString strMsg;
			strMsg.Format(_T("End : %04d/%02d/%02d %02d:%02d:%02d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

			if(bRun)
				CLog::GetInstance().PrintLog(true, false, true, false, _T("正常終了"));
			else
				CLog::GetInstance().PrintLog(true, false, true, false, _T("異常終了"));
			
			return bRun ? 0 : 1;
		}
		catch(const _com_error& e)
		{
			throw std::runtime_error(uh::toStr(atl2::stringize(e)));
		}
	}
	catch(const std::exception& e)
	{
		CLog::GetInstance().PrintLog(true, false, true, false, e.what());
		CLog::GetInstance().PrintLog(true, false, true, false, _T("異常終了"));
		return 2;
	}
	catch(...)
	{
		CLog::GetInstance().PrintLog(true, false, true, false, _T("異常終了"));
		return 2;
	}
	return 1;
}

bool InitLog(Arguments& args)
{
	// ログファイル作成
	if(!CLog::GetInstance().SetRunLog(args.RunLogPath.c_str()) ||
		!CLog::GetInstance().SetErrLog(args.ErrLogPath.c_str())){
		return false;
	}

	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tDIVID\tERROR_CODE\tERROR_MSG\tOTHER_MSG1\tOTHER_MSG2\tOTHER_MSG3\t"));

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s FILEVERSION:%s  PRODUCTVERSION:%s"), cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion());
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);
	CLog::GetInstance().PrintLog(true, false, true, false, args.GetOption());

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strMsg.Format(_T("Start : %04d/%02d/%02d %02d:%02d:%02d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

	return true;
}
