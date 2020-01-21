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

// SiNDYConnectPropertyListDlg.cpp : CSiNDYConnectPropertyListDlg のインプリメンテーション
#include "stdafx.h"
#include "CStringTokenizer.h"
#include "SiNDYConnectPropertySetList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static TCHAR REGOPENKEY[] = (_T("Software\\INCREMENT P CORPORATION\\SiNDY-u\\ConnectPropertySetList"));

bool SiNDYConnectPropertySetList::setFromRegistry()
{
	CRegKey cRegKey;
	if(cRegKey.Create(HKEY_CURRENT_USER, REGOPENKEY) != ERROR_SUCCESS)
		return false;

	TCHAR buffer[4096];
	DWORD dwSize = sizeof(buffer);
	if(cRegKey.QueryValue(buffer, _T("ConnectPropertySetList"), &dwSize) != ERROR_SUCCESS)
		return false;

	CString strList = buffer;
	CStringTokenizer tkz(strList, _T("\t"), TOKEN_RET_EMPTY_ALL);
	while(tkz.HasMoreTokens()) {
		push_back(SiNDYConnectPropertySet(tkz.GetNextToken()));
	}

	return true;
}

bool SiNDYConnectPropertySetList::writeToRegistry() const
{
	CRegKey cRegKey;
	if(cRegKey.Create(HKEY_CURRENT_USER, REGOPENKEY) != ERROR_SUCCESS)
		return false;

	CString strList;
	for(const_iterator it = begin(); it != end(); ++it) {
		const SiNDYConnectPropertySet& rPropertySet = *it;
		if(it != begin()) {
			strList += '\t';
		}

		strList += rPropertySet.getPropertiesString();
	}

	cRegKey.SetValue(strList, _T("ConnectPropertySetList"));

	return true;
}
