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

// CheckPolygon.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ParamCtrl.h"
#include <arctl/coinitializer.h>
#include "CheckMain.h"
#include <WinLib/VersionInfo.h>

void Init(CParamCtrl& cParam);
void End();
void OutputStr(const CString& str);

bool usage()
{
	std::cout << "用途：ポリゴンデータの整合性をチェックする" << std::endl;
	std::cout << "使用法：CheckPolygon.exe [Option]" << std::endl;
	std::cout << " Option : " << std::endl;
	std::cout << "      -d [接続DB名]			--- 接続DBパス" << std::endl;
	std::cout << "      -f [フィーチャクラス名]	--- フィーチャクラス名" << std::endl;
	std::cout << "      -c 	--- 隙間チェックを行わない場合" << std::endl;
	std::cout << "      -m 	--- マルチポリゴンチェックを行わない場合" << std::endl;

	return false;
}

bool arg_check(int argc, _TCHAR **argv, CParamCtrl &cParam)
{
	if (argc <= 1)
		return usage();

	while (--argc)
	{
		_TCHAR *s = *++argv;

		switch (*s)
		{
			case _T('-'):
				switch (*++s)
				{
					case _T('D'):	// DBパス
					case _T('d'): cParam.SetDB( *++argv ); --argc; break;
					case _T('F'):	// フィーチャクラス名
					case _T('f'): cParam.SetFeatureClass( *++argv ); --argc; break;
					case _T('C'):	// 隙間チェックを行うか
					case _T('c'): cParam.SetClearance( false ); break;
					case _T('M'):	// マルチポリゴンのチェックを行うか
					case _T('m'): cParam.SetMultiP( false ); break;
					default:
						return usage();
						break;
				}

				break;
			default:
				return usage();
				break;
		}
	}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int iRET = 0;
	try {
		arctl::coinitializer coinit;

		CParamCtrl cParam;
		if( !( arg_check( argc, argv, cParam ) ) )
			return 1;

		Init( cParam );

		time_t st_time;
		time(&st_time);

		CCheckMain cChkMain;
		iRET = cChkMain.Run( cParam );
			

		time_t tm;
		time(&tm);

		End();

		std::cerr << "# Total " << difftime( tm, st_time ) << "[sec]" << std::endl;
	}
	catch( const std::exception& e )
	{
		std::cerr << "異常終了しました" << std::endl;
		return 2;
	}

	return iRET;
}

void Init(CParamCtrl& cParam)
{
	std::cout << "---------------------" << std::endl;
	std::cout << "処理を開始します。" << std::endl;
	CVersion cVer;
	CString strMsg;
	strMsg.Format( _T("#%s FILEVERSION %s PRODUCTVERSION %s\n#"), static_cast<LPCTSTR>(cVer.GetOriginalFilename()), static_cast<LPCTSTR>(cVer.GetFileVersion()), static_cast<LPCTSTR>(cVer.GetProductVersion() ) );
	std::cerr << CT2CA( strMsg ) << std::endl;
	std::cerr << "# DB_PATH = " << CT2CA( cParam.GetDB() ) << std::endl;
	std::cerr << "# FEATURECLASS = " << CT2CA( cParam.GetFeatureClass() ) << std::endl;
	std::cerr << "# Clearance = " << cParam.GetClearance() << std::endl;
	std::cerr << "# Multi = " << cParam.GetMultiP() << std::endl;
	std::cerr << "#" << std::endl;
	OutputStr( _T("Start Check PGM") );
}

void End()
{
	OutputStr( _T("End PGM") );
	std::cout << "処理を終了します。" << std::endl;
	std::cout << "---------------------" << std::endl;
}

void OutputStr(const CString& str)
{
	CTime cTime = CTime::GetCurrentTime();
	long lYear = cTime.GetYear();
	long lMonth = cTime.GetMonth();
	long lDay = cTime.GetDay();
	long lHour = cTime.GetHour();
	long lMinute = cTime.GetMinute();
	long lSecond = cTime.GetSecond();

	CString cStrTime;
	cStrTime.Format( _T("%02d:%02d:%02d\t%s"), lHour, lMinute, lSecond, str );

	std::cerr << CT2CA( cStrTime ) << std::endl;
}

