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
#include "CheckDlcBld.h"
#include <WinLib/VersionInfo.h>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		const arctl::coinitializer aCoInitializer;

		// ���P�[���֘A�̕s��h�~
		uh::setJapaneseCharacterTypeFacet();

		try
		{
			time_t st_time;
			time( &st_time );

			// �R�}���h���C����������
			Arguments args;
			if(! args.parse(argc, argv))
			{
				cerr << "�R�}���h���C�������ɖ�肪����܂�" << endl;
				return 1;
			}
			// ���O�t�@�C���쐬
			if(!InitLog(args)){
				CLog::GetInstance().PrintLog(true, false, false, false, _T("�ُ�I��"));
				return 1;
			}

			// �`�F�b�N�J�n
			bool bRun = false, bCheckErr = false;
			CCheckDlcBld cCheckDlcBld;
			if(cCheckDlcBld.init(args)){
				bool bErr = false;
				if(cCheckDlcBld.run(bErr)){
					bRun = true;
					if(bErr)
						CLog::GetInstance().PrintLog(true, false, false, false, _T("�`�F�b�N�G���[����"));
					else
						CLog::GetInstance().PrintLog(true, false, false, false, _T("�`�F�b�N�G���[�Ȃ�"));
				}
			}

			time_t end_time;
			time( &end_time );

			CString strTotal;
			strTotal.Format( _T("\n# �g�[�^������ %.2f[sec]\n"), difftime( end_time, st_time ) );
			CLog::GetInstance().PrintLog(true, false, true, false, strTotal);

			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			CString strMsg;
			strMsg.Format(_T("End : %04d/%02d/%02d %02d:%02d:%02d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
			CLog::GetInstance().PrintLog(true, false, true, false, strMsg);

			if(bRun)
				CLog::GetInstance().PrintLog(true, false, true, false, _T("����I��"));
			else
				CLog::GetInstance().PrintLog(true, false, true, false, _T("�ُ�I��"));
			
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
		CLog::GetInstance().PrintLog(true, false, true, false, _T("�ُ�I��"));
		return 2;
	}
	catch(...)
	{
		CLog::GetInstance().PrintLog(true, false, true, false, _T("�ُ�I��"));
		return 2;
	}
	return 1;
}

bool InitLog(Arguments& args)
{
	// ���O�t�@�C���쐬
	if(!CLog::GetInstance().SetRunLog(args.strRunLog.c_str()) ||
		!CLog::GetInstance().SetErrLog(args.strErrLog.c_str())){
		return false;
	}

	CLog::GetInstance().PrintLog(false, false, false, true,_T("#FREESTYLELOG"));
	CLog::GetInstance().PrintLog(false, false, false, true,_T("ERROR_TYPE\tTABLE_NAME\tOBJECTID\tERROR_CODE\tERROR_MSG\tOTHER_MSG1\tOTHER_MSG2\tOTHER_MSG3\t"));

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

