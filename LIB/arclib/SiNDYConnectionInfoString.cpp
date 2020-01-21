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

// �\�z/����
//////////////////////////////////////////////////////////////////////

ConnectionInfoStringI::ConnectionInfoStringI()
{
}

ConnectionInfoStringI::~ConnectionInfoStringI()
{
}

// �ݒ�
//////////////////////////////////////////////////////////////////////

/**
 * @brief ���[�N�X�y�[�X����ڑ���񕶎���𐶐�����
 *
 * @param ipWorkspace [in] ���[�N�X�y�[�X�B
 */
void ConnectionInfoStringI::setConnectionInfoString(IWorkspace* ipWorkspace)
{
	if(! ipWorkspace)
		return;

	// ���uDB�ɐڑ����Ă���ꍇ�B
	IPropertySetPtr ipPropertySet;
	ipWorkspace->get_ConnectionProperties(&ipPropertySet);
	if(setConnectionInfoString(ipPropertySet))
		return;

	// ���[�J��DB�ɐڑ����Ă���ꍇ�B
	CComBSTR bstrPathName;
	ipWorkspace->get_PathName(&bstrPathName);
	if(bstrPathName.Length() != 0) {
		USES_CONVERSION;
		ConnectionFile::Folder aFolder;
		setString(aFolder.convertPathFullPathToAlias(OLE2CT(bstrPathName)));
	}
}

/**
 * @brief �ڑ��v���p�e�B����ڑ���񕶎���𐶐�����
 *
 * ipPropertySet��NULL�ł��邩�A�ڑ��v���p�e�B�Ɉȉ��̂����ꂩ�̒l���ݒ肳��Ă��Ȃ��ꍇ�A������̐����͎��s����B
 * - SERVER
 * - USER
 * - VERSION
 *
 * @param ipPropertySet [in] �ڑ��v���p�e�B
 * @retval true �ڑ���񕶎��񐶐������B
 * @retval false �ڑ���񕶎��񐶐����s�B
 */
bool ConnectionInfoStringI::setConnectionInfoString(IPropertySet* ipPropertySet)
{
	if(! ipPropertySet)
		return false;

	// "SERVER"�l�̑��݊m�F�B
	_variant_t vServer, vUser, vVersion;
	if(ipPropertySet->GetProperty(CComBSTR(_T("SERVER")), &vServer) != S_OK)
		return false;

	// "USER"�l�̑��݊m�F�B
	if(ipPropertySet->GetProperty(CComBSTR(_T("USER")), &vUser) != S_OK)
		return false;

	// "VERSION"�l�̑��݊m�F�B
	if(ipPropertySet->GetProperty(CComBSTR(_T("VERSION")), &vVersion) != S_OK)
		return false;

	USES_CONVERSION;
	CString strConnectionInfoString;
	strConnectionInfoString.Format(_T("%s@%s(%s)"), OLE2CT(vUser.bstrVal), OLE2CT(vServer.bstrVal), OLE2CT(vVersion.bstrVal));

	setString(strConnectionInfoString);

	return true;
}

// ����
//////////////////////////////////////////////////////////////////////

/**
 * @brief �f�[�^�x�[�X�ɐڑ�����
 *
 * @return �ڑ���f�[�^�x�[�X�B�ڑ��Ɏ��s�����ꍇ��null�B
 */
IWorkspacePtr ConnectionInfoStringI::connect()
{
	return connectWorkspace(getString());
}

#pragma warning (disable : 4701)
#pragma warning (disable : 4706)
/**
 * @brief �f�[�^�x�[�X�ɐڑ�����
 *
 * @param lpszConnectionInfo [in] �ڑ���񕶎���B
 * @return �ڑ���f�[�^�x�[�X�B�ڑ��Ɏ��s�����ꍇ��null�B
 */
IWorkspacePtr ConnectionInfoStringI::connectWorkspace(LPCTSTR lpszConnectionInfo)
{
	if(! lpszConnectionInfo)
		return 0;

	LPCTSTR p = ::_tcsrchr(lpszConnectionInfo, _T('.'));
	if(p && (::_tcsicmp(p, _T(".sde")) == 0 || ::_tcsicmp(p, _T(".mdb")) == 0)) {
		// �t�@�C���w��ŊJ���ꍇ�B
		ConnectionFile::Folder aFolder;
		return ConnectionFile::open(aFolder.convertPathAliasToFullPath(lpszConnectionInfo));
	}
	else {
		struct _stat buf;
		if(::_tstat(lpszConnectionInfo, &buf) == 0 && (buf.st_mode & S_IFDIR)) {
			// �t�H���_�w��ŊJ���ishape�j�ꍇ�B
			IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);

			IWorkspacePtr ipWorkspace;
			ipWorkspaceFactory->OpenFromFile(CComBSTR(lpszConnectionInfo), 0, &ipWorkspace);
			return ipWorkspace;
		}
		else {
			// �v���p�e�B�w��ŊJ���ꍇ�B
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
