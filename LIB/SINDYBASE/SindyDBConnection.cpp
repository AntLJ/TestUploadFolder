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

// SindyDBConnection.cpp: SindyDBConnection クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyDBConnection.h"


SindyDBConnection::SindyDBConnection() :
ipWorkspace(NULL),
mDatasetList(),
mStateOfEditing(false),
mNewVersion(false)
{

}

SindyDBConnection::~SindyDBConnection()
{
	if (mStateOfEditing) StopEdit();
}


HRESULT SindyDBConnection::Connect(const std::string &cServer, const std::string &cInstance, const std::string &cDatabase, const std::string &cUser, const std::string &cPassword, const std::string &cVersion)
{
	const char *func = "SindyDBConnection::Connect";

	mUserName = cUser;
	if (ipWorkspace != NULL) return GDBERROR2(E_ACCESSDENIED, func);
	return GDBOpenWorkspace(cServer, cInstance, cDatabase, cUser, cPassword, cVersion, &ipWorkspace);
}

HRESULT SindyDBConnection::Connect(const std::string &cDataBase)
{
	const char *func = "SindyDBConnection::Connect";
	HRESULT hr;

	if (ipWorkspace != NULL) return GDBERROR2 (E_ACCESSDENIED, func);
	//DB接続
	IUnknown* ipUnknown = NULL;
	if(FAILED(hr = INamePtr(sindy::create_workspace_name(cDataBase.c_str()))->Open(&ipUnknown)))
		return GDBERROR(hr,func);
	ipWorkspace = IWorkspacePtr(ipUnknown);
	//ユーザ名取得
	IPropertySetPtr ipPropertySet;
	if(FAILED(hr = ipWorkspace->get_ConnectionProperties(&ipPropertySet)))
		return GDBERROR (hr, func);
	_variant_t vUser;
	ipPropertySet->GetProperty( CComBSTR( _T("user") ), &vUser );
	if( vUser.vt != VT_NULL || vUser.vt != VT_EMPTY )
		mUserName = (char*)(_bstr_t)vUser;
	return S_OK;
}

HRESULT SindyDBConnection::GetVersionName(std::string *cVersionName)
{
	return GDBGetVersionName(ipWorkspace, cVersionName);
}

HRESULT SindyDBConnection::OpenFeatureClass(const std::string &cFeatureClassName, IFeatureClass **ipFeatureClass)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::OpenFeatureClass";

	std::string key = "";
	toupper(cFeatureClassName, &key);
	std::map<std::string, IDatasetPtr>::iterator aDataset = mDatasetList.find(key);
	if (aDataset != mDatasetList.end())
	{
		*ipFeatureClass = (IFeatureClassPtr)aDataset->second;
		(*ipFeatureClass)->AddRef();
	}
	else
	{
		if (FAILED(hr = GDBOpenFeatureClass(ipWorkspace, cFeatureClassName, ipFeatureClass)))
			return hr;
		mDatasetList.insert(std::pair<std::string, IDatasetPtr>(key, (IDatasetPtr)(*ipFeatureClass)));
	}

	return S_OK;
}

HRESULT SindyDBConnection::OpenTable(const std::string &cTableName, ITable **ipTable)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::OpenTable";

	std::string key = "";
	toupper(cTableName, &key);
	std::map<std::string, IDatasetPtr>::iterator aDataset = mDatasetList.find(key);
	if (aDataset != mDatasetList.end())
	{
		*ipTable = (ITablePtr)aDataset->second;
		(*ipTable)->AddRef();
	}
	else
	{
		if (FAILED(hr = GDBOpenTable(ipWorkspace, cTableName, ipTable)))
			return hr;
		mDatasetList.insert(std::pair<std::string, IDatasetPtr>(key, (IDatasetPtr)(*ipTable)));
	}

	return S_OK;
}

HRESULT SindyDBConnection::OpenDataset(IEnumDatasetPtr ipEnumDataset)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::OpenDataset";

	IDatasetPtr ipDataset = NULL;
	while (ipEnumDataset->Next(&ipDataset) == S_OK && ipDataset != NULL)
	{
		IEnumDatasetPtr ipEnumSubDataset = NULL;
		if (FAILED(hr = ipDataset->get_Subsets(&ipEnumSubDataset)))
			return GDBERROR(hr, func);
		if (ipEnumSubDataset == NULL)
		{
			CComBSTR bstrName;
			if (FAILED(hr = ipDataset->get_Name(&bstrName)))
				return GDBERROR(hr, func);
			USES_CONVERSION;
			std::string aName = "";
			toupper(OLE2T(bstrName), &aName);
			mDatasetList.insert(std::pair<std::string, IDatasetPtr>(aName, ipDataset));
		}
		else
		{
			if (FAILED(hr = OpenDataset(ipEnumSubDataset)))
				return hr;
		}
	}

	return S_OK;
}

HRESULT SindyDBConnection::OpenDataset()
{
	HRESULT hr;
	const char *func = "SindyDBConnection::OpenDataset";

	const long dataset_type[] = {
//		esriDTAny,
//		esriDTContainer,
//		esriDTGeo,
		esriDTFeatureDataset,
		esriDTFeatureClass,
//		esriDTPlanarGraph,
//		esriDTGeometricNetwork,
//		esriDTText,
		esriDTTable,
//		esriDTRelationshipClass,
//		esriDTRasterDataset,
//		esriDTRasterBand,
//		esriDTTin,
//		esriDTCadDrawing,
//		esriDTRasterCatalog,
		-1
	};

	for (long i = 0; dataset_type[i] != -1; i++)
	{
		IEnumDatasetPtr ipEnumDataset = NULL;
		if (FAILED(hr = ipWorkspace->get_Datasets((esriDatasetType)dataset_type[i], &ipEnumDataset)))
			return GDBERROR(hr, func);
		if (FAILED(hr = OpenDataset(ipEnumDataset)))
			return hr;
	}

	return S_OK;
}

HRESULT SindyDBConnection::OpenDataset(const std::string &cName)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::OpenDataset";

	std::string key = "";
	toupper(cName, &key);
	std::map<std::string, IDatasetPtr>::iterator aDataset = mDatasetList.find(key);
	if (aDataset != mDatasetList.end()) return S_OK;	// 既にオープン済みならそのままreturn

	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	ATLASSERT(ipFeatureWorkspace != NULL);

	IFeatureClassPtr ipFeatureClass = NULL;
	if (FAILED(hr = ipFeatureWorkspace->OpenFeatureClass(CComBSTR(_T(key.c_str())), &ipFeatureClass)) || ipFeatureClass == NULL)
	{
		ITablePtr ipTable = NULL;
		if (FAILED(hr = ipFeatureWorkspace->OpenTable(CComBSTR(_T(key.c_str())), &ipTable)) || ipTable == NULL)
			return GDBERROR(hr, func);
		else
			mDatasetList.insert(std::pair<std::string, IDatasetPtr>(key, (IDatasetPtr)ipTable));
	}
	else
	{
		mDatasetList.insert(std::pair<std::string, IDatasetPtr>(key, (IDatasetPtr)ipFeatureClass));
	}

	return S_OK;
}

HRESULT SindyDBConnection::CreateVersion(const std::string &cVersionName)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::CreateVersion";

	IVersionPtr ipVersion = NULL;
	if (FAILED(hr = GDBCreateNewVersion(ipWorkspace, cVersionName, &ipVersion)))
		return hr;
	ipWorkspace = (IWorkspacePtr)ipVersion;

	mNewVersion = true;

	return S_OK;
}

HRESULT SindyDBConnection::DeleteVersion()
{
	HRESULT hr;
	const char *func = "SindyDBConnection::DeleteVersion";
	if (! mNewVersion) return S_OK;
	IVersionPtr ipVersion(ipWorkspace);
	ATLASSERT(ipVersion != NULL);
	if (FAILED(hr = ipVersion->Delete())) return GDBERROR(hr, func);
	mNewVersion = false;
	return S_OK;
}

HRESULT SindyDBConnection::StartEdit()
{
	mStateOfEditing = true;
	return GDBStartEditing(ipWorkspace);
}

HRESULT SindyDBConnection::StopEdit()
{
	HRESULT hr;

	if (! mStateOfEditing)
		return S_OK;
	if (FAILED(hr = GDBStopEditing(ipWorkspace)))
		return hr;
	mStateOfEditing = false;

	return S_OK;
}

HRESULT SindyDBConnection::Reconcile(const std::string &cVersionName, bool *cConflict)
{
	return GDBReconcile(ipWorkspace, cVersionName, cConflict);
}

HRESULT SindyDBConnection::Post(const std::string &cVersionName)
{
	return GDBPost(ipWorkspace, cVersionName);
}

HRESULT SindyDBConnection::is_connected(bool *cConnected)
{
	*cConnected = (ipWorkspace == NULL) ? false : true;
	return S_OK;
}

HRESULT SindyDBConnection::FillCache(IEnvelopePtr ipEnvelope)
{
	return GDBFillCache(ipWorkspace, ipEnvelope);
}

HRESULT SindyDBConnection::EmptyCache(void)
{
	return GDBEmptyCache(ipWorkspace);
}

HRESULT SindyDBConnection::QueryDef(const std::string &cTablesName, const std::string &cWhereClause, const std::string &cSubFields, _ICursor **ipCursor)
{
	return GDBQueryDef(ipWorkspace, cTablesName, cWhereClause, cSubFields, ipCursor);
}

HRESULT SindyDBConnection::GetSpatialReference(const std::string &cName, ISpatialReference **ipSpatialReference)
{
	HRESULT hr;
	const char *func = "SindyDBConnection::GetSpatialReference";

	USES_CONVERSION;

	*ipSpatialReference = 0;

	IWorkspaceSpatialReferenceInfoPtr ipWorkspaceSpatialReferenceInfo(ipWorkspace);
	ATLASSERT(ipWorkspaceSpatialReferenceInfo != 0);

	IEnumSpatialReferenceInfoPtr ipEnumSpatialReferenceInfo = 0;
	if ((hr = ipWorkspaceSpatialReferenceInfo->get_SpatialReferenceInfo(&ipEnumSpatialReferenceInfo)) != S_OK)
		return GDBERROR(hr, func);
	long aID;
	while (ipEnumSpatialReferenceInfo->Next(&aID, ipSpatialReference) == S_OK && *ipSpatialReference != 0)
	{
		CComBSTR bstrName;
		if ((hr = (*ipSpatialReference)->get_Name(&bstrName)) != S_OK)
			return GDBERROR(hr, func);
		std::string aName  = OLE2T(bstrName);
		if (cName == aName)
			return S_OK;
	}

	*ipSpatialReference = 0;

	return S_OK;
}

