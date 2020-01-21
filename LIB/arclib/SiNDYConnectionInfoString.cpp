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
#include <sys/types.h>
#include <sys/stat.h>
#include "SiNDYConnectionFile.h"
#include "SiNDYConnectionInfoStringI.h"
#include <sindy/workspace.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace sindy;

// 構築/消滅
//////////////////////////////////////////////////////////////////////

ConnectionInfoStringI::ConnectionInfoStringI()
{
}

ConnectionInfoStringI::~ConnectionInfoStringI()
{
}

// 設定
//////////////////////////////////////////////////////////////////////

/**
 * @brief ワークスペースから接続情報文字列を生成する
 *
 * @param ipWorkspace [in] ワークスペース。
 */
void ConnectionInfoStringI::setConnectionInfoString(IWorkspace* ipWorkspace)
{
	if(! ipWorkspace)
		return;

	// 遠隔DBに接続している場合。
	IPropertySetPtr ipPropertySet;
	ipWorkspace->get_ConnectionProperties(&ipPropertySet);
	if(setConnectionInfoString(ipPropertySet))
		return;

	// ローカルDBに接続している場合。
	CComBSTR bstrPathName;
	ipWorkspace->get_PathName(&bstrPathName);
	if(bstrPathName.Length() != 0) {
		USES_CONVERSION;
		ConnectionFile::Folder aFolder;
		setString(aFolder.convertPathFullPathToAlias(OLE2CT(bstrPathName)));
	}
}

/**
 * @brief 接続プロパティから接続情報文字列を生成する
 *
 * ipPropertySetがNULLであるか、接続プロパティに以下のいずれかの値が設定されていない場合、文字列の生成は失敗する。
 * - SERVER
 * - USER
 * - VERSION
 *
 * @param ipPropertySet [in] 接続プロパティ
 * @retval true 接続情報文字列生成成功。
 * @retval false 接続情報文字列生成失敗。
 */
bool ConnectionInfoStringI::setConnectionInfoString(IPropertySet* ipPropertySet)
{
	if(! ipPropertySet)
		return false;

	// "SERVER"値の存在確認。
	_variant_t vServer, vUser, vVersion;
	if(ipPropertySet->GetProperty(CComBSTR(_T("SERVER")), &vServer) != S_OK)
		return false;

	// "USER"値の存在確認。
	if(ipPropertySet->GetProperty(CComBSTR(_T("USER")), &vUser) != S_OK)
		return false;

	// "VERSION"値の存在確認。
	if(ipPropertySet->GetProperty(CComBSTR(_T("VERSION")), &vVersion) != S_OK)
		return false;

	USES_CONVERSION;
	CString strConnectionInfoString;
	strConnectionInfoString.Format(_T("%s@%s(%s)"), OLE2CT(vUser.bstrVal), OLE2CT(vServer.bstrVal), OLE2CT(vVersion.bstrVal));

	setString(strConnectionInfoString);

	return true;
}

// 処理
//////////////////////////////////////////////////////////////////////

/**
 * @brief データベースに接続する
 *
 * @return 接続先データベース。接続に失敗した場合はnull。
 */
IWorkspacePtr ConnectionInfoStringI::connect()
{
	return connectWorkspace(getString());
}

#pragma warning (disable : 4701)
#pragma warning (disable : 4706)
/**
 * @brief データベースに接続する
 *
 * @param lpszConnectionInfo [in] 接続情報文字列。
 * @return 接続先データベース。接続に失敗した場合はnull。
 */
IWorkspacePtr ConnectionInfoStringI::connectWorkspace(LPCTSTR lpszConnectionInfo)
{
	if(! lpszConnectionInfo)
		return 0;

	LPCTSTR p = ::_tcsrchr(lpszConnectionInfo, _T('.'));
	if(p && (::_tcsicmp(p, _T(".sde")) == 0 || ::_tcsicmp(p, _T(".mdb")) == 0)) {
		// ファイル指定で開く場合。
		ConnectionFile::Folder aFolder;
		return ConnectionFile::open(aFolder.convertPathAliasToFullPath(lpszConnectionInfo));
	}
	else {
		struct _stat buf;
		if(::_tstat(lpszConnectionInfo, &buf) == 0 && (buf.st_mode & S_IFDIR)) {
			// フォルダ指定で開く（shape）場合。
			IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);

			IWorkspacePtr ipWorkspace;
			ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszConnectionInfo), 0, &ipWorkspace);
			return ipWorkspace;
		}
		else {
			// プロパティ指定で開く場合。
			IWorkspaceName2Ptr ipWorkspaceName = create_at_style_workspace_string_name(lpszConnectionInfo);
			
			IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
			IPropertySetPtr ipPropertySet(CLSID_PropertySet);
			ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);
			IWorkspacePtr ipWorkspace;

			ipWorkspaceFactory->Open(ipPropertySet, 0, &ipWorkspace);
			return ipWorkspace;
		}
	}

	return 0;
}
