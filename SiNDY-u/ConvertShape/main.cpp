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

// ConvertShape.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <arctl/coinitializer.h>
#include "TransParam.h"
#include "global.h"
#include <WinLib/VersionInfo.h>
#include "CnvShp.h"
#include "ConvertShape.h"

bool initLog(CTransParam& cParam)
{
	if(!cParam.GetRunlogFile().empty()){
		if(!CLog::GetInstance().SetRunLog(CString(cParam.GetRunlogFile().c_str()))){
			std::cerr << "ERRORです" << std::endl;
		}
	}
	if(!cParam.GetErrlogFile().empty()){
		if(!CLog::GetInstance().SetErrLog(CString(cParam.GetErrlogFile().c_str()))){
			std::cerr << "ERRORです" << std::endl;
		}
		CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
		CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\tOTHER_MSG\t"));
	}

	CVersion cVer;
	std::string strMsg = (boost::format( "%s FILEVERSION %s PRODUCTVERSION %s\n" ) % CT2CA( cVer.GetOriginalFilename() ).m_psz % CT2CA( cVer.GetFileVersion() ).m_psz % CT2CA( cVer.GetProductVersion() ).m_psz ).str();
	CLog::GetInstance().PrintLog(true, false, true, false, CString(strMsg.c_str()));
	CLog::GetInstance().PrintLog(true, false, true, false, CString(cParam.GetOption().c_str()));

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CLog::GetInstance().PrintLog(true, false, true, false, CString((boost::format("Start : %04d/%02d/%02d %02d:%02d:%02d\n\n") % sysTime.wYear % sysTime.wMonth % sysTime.wDay % sysTime.wHour % sysTime.wMinute % sysTime.wSecond ).str().c_str()));

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		time_t st_time;
		time( &st_time );

		// パラメタの解析
		CTransParam cParam;
		if( !cParam.parse(argc, argv) )
			return 1;

		if( !initLog( cParam ) )
			return 1;

		// 初期化
		CConvertShape cConvertShape;
		if(!cConvertShape.Init(cParam))
			return 1;

		// 処理実行
		bool bConvert = cConvertShape.run();
		
		time_t end_time;
		time( &end_time );

		CString strTotal;
		strTotal.Format( _T("\n# トータル時間 %.2f[sec]"), difftime( end_time, st_time ) );
		CLog::GetInstance().PrintLog(true, false, true, false, strTotal);

		if(bConvert)
			CLog::GetInstance().PrintLog(true, false, true, false, _T("正常終了\n"));
		else
			CLog::GetInstance().PrintLog(true, false, true, false, _T("異常終了\n"));

		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		CLog::GetInstance().PrintLog(true, false, true, false, CString((boost::format("End : %04d/%02d/%02d %02d:%02d:%02d") % sysTime.wYear % sysTime.wMonth % sysTime.wDay % sysTime.wHour % sysTime.wMinute % sysTime.wSecond ).str().c_str()));

		return bConvert ? 0 : 1;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
		cerr << "異常終了" << endl;
		return 2;
	}
	catch(...)
	{
		cerr << "異常終了" << endl;
		return 2;
	}

	return 0;
}

