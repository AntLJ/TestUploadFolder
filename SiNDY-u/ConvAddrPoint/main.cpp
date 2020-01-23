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
#include <comdef.h>
#include <arctl/coinitializer.h>
#include <WinLib/VersionInfo.h>
#include <TDC/useful_headers/japanese_locale.h>
#include "Param.h"
#include "LogSys.h"
#include "DBUtils.h"			// ConvAddrPoint.hで必要
#include "Addr2TsvBase.h"		// ConvAddrPoint.hで必要
#include <sindy/libschema.h>	// ConvAddrPoint.hで必要
#include "FileEUC.h"			// ConvAddrPoint.hで必要
#include "Adrp.h"				// ConvAddrPoint.hで必要
#include "Zipp.h"				// ConvAddrPoint.hで必要
#include "ConvAddrPoint.h"

bool initLog(const CParam& cParam)
{
	if (!cParam.getRunLog().empty())
	{
		if (!CLogSys::GetInstance().SetRunLog(CString(cParam.getRunLog().c_str())))
			return false;
	}
	if (!cParam.getErrLog().empty())
	{
		if (!CLogSys::GetInstance().SetErrLog(CString(cParam.getErrLog().c_str())))
			return false;
	}

	CVersion cVer;
	CString strMsg;
	strMsg.Format(_T("%s VERSION %s\n"), cVer.GetOriginalFilename(), cVer.GetFileVersion());
	CLogSys::GetInstance().WriteRunLog(true, false, strMsg);
	CLogSys::GetInstance().WriteRunLog(true, false, CString(cParam.getOption().c_str()));

	return true;
}

void putRunLog(const CString& msg)
{
	SYSTEMTIME st;
	CString strDateTime;
	CString strMsg;
	GetLocalTime(&st);
	strDateTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	strMsg.Format(_T("\n%s : %s\n\n"), msg, strDateTime);
	CLogSys::GetInstance().WriteRunLog(true, false, strMsg);
}

int _tmain(int argc, _TCHAR* argv[])
{
	int nRet = 0;
	try {
		arctl::coinitializer coinitializer;

		// ロケール関連の不具合防止
		uh::setJapaneseCharacterTypeFacet();

		CParam cParam;
		if (!cParam.parse(argc, argv) || !initLog(cParam))
			nRet = 1;
		else {
			putRunLog(_T("start"));

			CConvAddrPoint convAddrPoint(cParam);
			if (!convAddrPoint.init() || !convAddrPoint.execute())
				nRet = 1;

			putRunLog(_T("end"));
		}
	}
	catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		nRet = 2;
	}
	catch (const _com_error &com_err) {
		std::cerr << CT2CA(com_err.ErrorMessage()) << std::endl;
		nRet = 2;
	}
	catch (...) {
		std::cerr << "例外発生" << std::endl;
		nRet = 2;
	}

	if (nRet == 0)
		CLogSys::GetInstance().WriteRunLog(true, false, _T("正常終了\n"));
	else
		CLogSys::GetInstance().WriteRunLog(true, false, _T("異常終了\n"));

	return nRet;
}
