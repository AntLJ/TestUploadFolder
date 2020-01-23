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
#include "LogSys.h"
#include "FileSJIS.h"


CFileSJIS::CFileSJIS() : m_ofsFile(NULL)
{
}

CFileSJIS::~CFileSJIS()
{
	close();
}

bool CFileSJIS::open(const CString& strDir, const CString& strFile, std::ofstream& ofs)
{
	CString filename = strDir + _T("\\") + strFile;
	ofs.open(filename, std::ios::trunc);
	if (!ofs.is_open()) {
		CLogSys::GetInstance().PrintFreeStyleLog(err_level::err, _T(""), D_LONG_NULL, err_code::errOpenFile, _T("ファイルオープン失敗 [%s]"), filename);
		return false;
	}
	m_ofsFile = &ofs;
	return true;
}

void CFileSJIS::close(void)
{
	if (m_ofsFile && m_ofsFile->is_open())
		m_ofsFile->close();
}
