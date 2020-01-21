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

/**
 *	GDBWorkspace.cpp
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		êVãKçÏê¨
 */

#include "GDB.h"
#include <sindy/workspace.h>

HRESULT GDBOpenWorkspace(const std::string &cServer, const std::string &cInstance, const std::string &cDatabase, const std::string &cUser, const std::string &cPassword, const std::string &cVersion, IWorkspace **ipWorkspace)
{
	HRESULT hr;
	const char *func = "GDBOpenWorkspace";

	IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name_oracle_dc(cServer.c_str(), cUser.c_str(), cPassword.c_str(), cVersion.c_str());

	IWorkspaceFactoryPtr ipWorkspaceFactory(CLSID_SdeWorkspaceFactory);
	ATLASSERT(!ipWorkspaceFactory);

	// ï∂éöâªÇØëŒçÙ
	IWorkspacePtr ipTempWorkspace;
	IWorkspaceFactoryPtr ipTempWorkspaceFactory(CLSID_ShapefileWorkspaceFactory);
	ATLASSERT(!ipTempWorkspaceFactory);
	if (FAILED(hr = ipTempWorkspaceFactory->OpenFromFile(CComBSTR(_T("C:\\")), 0, &ipTempWorkspace)))
		return GDBERROR(hr, func);
	// ï∂éöâªÇØëŒçÙ(èIóπ)

	IPropertySetPtr ipPropertySet(CLSID_PropertySet);
	ATLASSERT(!ipPropertySet);

	ipWorkspaceName->get_ConnectionProperties(&ipPropertySet);
	if (FAILED(hr = ipWorkspaceFactory->Open(ipPropertySet, NULL, ipWorkspace)))
		return GDBERROR(hr, func);

	return hr;
}

HRESULT GDBOpenFeatureClass(IWorkspacePtr ipWorkspace, const std::string &cAliasName, IFeatureClass **ipFeatureClass)
{
	HRESULT hr;
	const char *func = "GDBOpenFeatureClass";

	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ATLASSERT(!ipFeatureWorkspace);

	if (FAILED(hr = ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(cAliasName.c_str())), ipFeatureClass)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBOpenTable(IWorkspacePtr ipWorkspace, const std::string &cAliasName, ITable **ipTable)
{
	HRESULT hr;
	const char *func = "GDBOpenTable";

	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ATLASSERT(!ipFeatureWorkspace);

	if (FAILED(hr = ipFeatureWorkspace->OpenTable(CComBSTR(_T(cAliasName.c_str())), ipTable)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBStartEditing(IWorkspacePtr ipWorkspace)
{
	HRESULT hr;
	const char *func = "GDBStartEditing";

	IWorkspaceEditPtr ipWorkspaceEdit(ipWorkspace);
	ATLASSERT(!ipWorkspaceEdit);

	if (FAILED(hr = ipWorkspaceEdit->StartEditing(VARIANT_TRUE)))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipWorkspaceEdit->StartEditOperation()))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBStopEditing(IWorkspacePtr ipWorkspace)
{
	HRESULT hr;
	const char *func = "GDBStopEditing";

	IWorkspaceEditPtr ipWorkspaceEdit(ipWorkspace);
	ATLASSERT(!ipWorkspaceEdit);

	if (FAILED(hr = ipWorkspaceEdit->StopEditOperation()))
		return GDBERROR(hr, func);
	if (FAILED(hr = ipWorkspaceEdit->StopEditing(VARIANT_TRUE)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBCreateNewVersion(IWorkspacePtr ipWorkspace, const std::string &cNewVersionName, IVersion **ipNewVersion)
{
	HRESULT hr;
	const char *func = "GDBCreateNewVersion";

	IVersionPtr ipCurrentVersion(ipWorkspace);
	ATLASSERT(!ipCurrentVersion);

	if (FAILED(hr = ipCurrentVersion->CreateVersion(CComBSTR(_T(cNewVersionName.c_str())), ipNewVersion)))
		return GDBERROR(hr, func);

/*
	if (FAILED(hr = (*ipNewVersion)->put_Access(esriVersionAccessPrivate)))
		return GDBERROR(hr, func);
*/

	return S_OK;
}

HRESULT GDBCheckVersionExistence(IWorkspacePtr ipWorkspace, const std::string &cVersionName, bool *cExistence)
{
	HRESULT hr;
	const char *func = "GDBCheckVersionExistence";

	*cExistence = false;

	IVersionedWorkspacePtr ipVersionedWorkspace(ipWorkspace);
	ATLASSERT(!ipVersionedWorkspace);
	IEnumVersionInfoPtr ipEnumVersionInfo;
	IVersionPtr ipVersion;
	if (FAILED(hr = ipVersionedWorkspace->FindVersion(CComBSTR(_T(cVersionName.c_str())), &ipVersion)))
		return GDBERROR(hr, func);
	if (ipVersion == NULL) *cExistence = false;
	else *cExistence = true;

	return S_OK;
}

HRESULT GDBReconcile(IWorkspacePtr ipWorkspace, const std::string &cVersionName, bool *cConflict)
{
	HRESULT hr;
	const char *func = "GDBReconcile";

	IVersionEditPtr ipVersionEdit(ipWorkspace);
	ATLASSERT(!ipVersionEdit);

	VARIANT_BOOL conflict = VARIANT_FALSE;
	if (FAILED(hr = ipVersionEdit->Reconcile(CComBSTR(_T(cVersionName.c_str())), &conflict)))
		return GDBERROR(hr, func);

	*cConflict = conflict ? true : false;

	return S_OK;
}

HRESULT GDBPost(IWorkspacePtr ipWorkspace, const std::string &cVersionName)
{
	HRESULT hr;
	const char *func = "GDBPost";

	IVersionEditPtr ipVersionEdit(ipWorkspace);
	ATLASSERT(!ipVersionEdit);

	VARIANT_BOOL can_post = VARIANT_FALSE;
	if (FAILED(hr = ipVersionEdit->CanPost(&can_post)))
		return GDBERROR(hr, func);
	if (!can_post)
		return GDBERROR2(E_ACCESSDENIED, func);

	if (FAILED(hr = ipVersionEdit->Post(CComBSTR(_T(cVersionName.c_str())))))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBFillCache(IWorkspacePtr ipWorkspace, IEnvelopePtr ipEnvelope)
{
	HRESULT hr;
	const char *func = "GDBFillCache";

	ISpatialCacheManagerPtr ipSpatialCacheManager(ipWorkspace);
	ATLASSERT(!ipSpatialCacheManager);

	if (FAILED(hr = ipSpatialCacheManager->FillCache(ipEnvelope)))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBEmptyCache(IWorkspacePtr ipWorkspace)
{
	HRESULT hr;
	const char *func = "GDBEmptyCache";

	ISpatialCacheManagerPtr ipSpatialCacheManager(ipWorkspace);
	ATLASSERT(!ipSpatialCacheManager);

	if (FAILED(hr = ipSpatialCacheManager->EmptyCache()))
		return GDBERROR(hr, func);

	return S_OK;
}

HRESULT GDBGetVersionName(IWorkspacePtr ipWorkspace, std::string *cVersionName)
{
	HRESULT hr;
	const char *func = "GDBGetVersionName";

	IVersionPtr ipVersion(ipWorkspace);
	ATLASSERT(!ipVersion);

	CComBSTR bstrName;
	if (FAILED(hr = ipVersion->get_VersionName(&bstrName)))
		return GDBERROR(hr, func);

	*cVersionName = CString(bstrName);

	return S_OK;
}
