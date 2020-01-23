#include "stdafx.h"
#include "COutput.h"
#include <WinLib/VersionInfo.h>

// ���ݎ����擾
CString COutput::GetNow(LPSYSTEMTIME lpSystemTime/* = nullptr*/) const
{
	SYSTEMTIME stSystemTime = {};
	if (nullptr != lpSystemTime)
		CopyMemory(&stSystemTime, lpSystemTime, sizeof(SYSTEMTIME));
	else
		::GetLocalTime(&stSystemTime);

	CString cStrModifyDate;
	cStrModifyDate.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay, stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	return cStrModifyDate;
}

// �o�[�W�������擾
std::pair<CString, CString> COutput::GetVersion()
{
	CVersion version;
	std::pair<CString, CString> pairRet(version.GetProductVersion(), version.GetFileVersion());
	return pairRet;
}

//  ���s�t�@�C�����擾
CString COutput::GetModuleName() const
{
	TCHAR szBuf[_MAX_PATH + 1] = {};

	GetModuleFileName(nullptr, szBuf, _MAX_PATH);

	LPCTSTR pos = ::_tcsrchr(szBuf, _T('\\'));
	CString strName;
	strName.Format(_T("%s"), pos ? pos + 1 : szBuf);

	return strName;
}

// ���O�w�b�_�쐬
CString COutput::CreateLogHeader(LPCTSTR lpcszOpParam)
{
	CString strHeader, strTmp;
	std::pair<CString, CString> pairVersion = GetVersion();

	strTmp.Format(_T("# FREESTYLELOG\n#%s FILEVERSION %s PRODUCTVERSION %s\n%s\n"), GetModuleName(), pairVersion.second, pairVersion.first, lpcszOpParam);
	strHeader = strTmp;

	return strHeader;
}