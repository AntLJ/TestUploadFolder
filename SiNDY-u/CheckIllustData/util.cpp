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
#include "util.h"
#include <TDC/useful_headers/str_util.h>

namespace util
{
	// �t�@�C�����ɓ��t��ǉ�����
	CString AddDateToFileName(const SYSTEMTIME& time, const CString& orgFilePath)
	{
		int folderIndex = orgFilePath.ReverseFind(_T('\\'));
		CString strFolderPath = orgFilePath.Left(folderIndex+1);

		CString strOrgFileName = orgFilePath.Mid(folderIndex+1);

		int extIndex = strOrgFileName.ReverseFind(_T('.'));
		CString strFileTitle = strOrgFileName.Left(extIndex); // �g���q�Ȃ��t�@�C����
		CString strSuffix = strOrgFileName.Mid(extIndex);  // �u.�g���q�v

		CString strDate = uh::str_util::format(_T("_%02d%02d%02d%02d%02d%02d"), time.wYear % 100, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

		return (strFolderPath + strFileTitle + strDate + strSuffix);
	}

	// �t�B�[�`���N���X���擾
	bool OpenFeatureClass(const CString& name, IWorkspacePtr work, IFeatureClassPtr& featureC)
	{
		IFeatureWorkspacePtr ipFeatureWork(work);

		if(FAILED(ipFeatureWork->OpenFeatureClass(CComBSTR(name), &featureC)))
			return false;

		return true;
	}

	// �e�[�u�����擾
	bool OpenTable(const CString& name, IWorkspacePtr work, ITablePtr& table)
	{
		IFeatureWorkspacePtr ipFeatureWork(work);
		if(FAILED(ipFeatureWork->OpenTable(CComBSTR(name), &table)))
			return false;

		return true;
	}

	int JAPANESE_LANGUAGE  = 0x0411; 
}