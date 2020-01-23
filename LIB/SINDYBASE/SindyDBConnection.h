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

// SindyDBConnection.h: SindyDBConnection クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYDBCONNECTION_H__F90404A8_8B0E_4595_BFF7_5B396D630D07__INCLUDED_)
#define AFX_SINDYDBCONNECTION_H__F90404A8_8B0E_4595_BFF7_5B396D630D07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GDB.h"
#include "SindyAttrdef.h"
#include "WKHString.h"
#include "sindy/workspace.h"

class SindyDBConnection  
{
public:
	SindyDBConnection();
	virtual ~SindyDBConnection();

	HRESULT Connect(const std::string &cServer, const std::string &cInstance, const std::string &cDatabase, const std::string &cUser, const std::string &cPassword, const std::string &cVersion);
	//sindy::create_workspace_name()を使用したDB接続
	HRESULT Connect(const std::string &cDataBase);
	HRESULT GetVersionName(std::string *cVersionName);
	HRESULT OpenFeatureClass(const std::string &cFeatureClassName, IFeatureClass **ipFeatureClass);
	HRESULT OpenTable(const std::string &cTableName, ITable **ipTable);
	HRESULT OpenDataset();
	HRESULT OpenDataset(const std::string &cName);
	HRESULT QueryDef(const std::string &cTablesName, const std::string &aWhereClause, const std::string &cSubFields, _ICursor **ipCursor);
	HRESULT CreateVersion(const std::string &cVersionName);
	HRESULT DeleteVersion();
	HRESULT StartEdit();
	HRESULT StopEdit();
	HRESULT GetState(bool *state) { *state = mStateOfEditing; return S_OK; }
	HRESULT Reconcile(const std::string &cVersionName, bool *cConflict);
	HRESULT Post(const std::string &cVersionName);
	HRESULT is_connected(bool *cConnected);
	HRESULT FillCache(IEnvelopePtr ipEnvelope);
	HRESULT EmptyCache(void);

	IDatasetPtr SearchDataset(const std::string &cName)
	{
		std::string key = "";
		toupper(cName, &key);
		std::map<std::string, IDatasetPtr>::iterator aIter = mDatasetList.find(key);
		if (aIter == mDatasetList.end()) return NULL;
		return aIter->second;
	}

	std::string GetUserName() { return mUserName; }

	// cNameは欲しい空間参照の名前
	HRESULT GetSpatialReference(const std::string &cName, ISpatialReference **ipSpatialReference);

protected:
	HRESULT OpenDataset(IEnumDatasetPtr ipEnumDataset);

private:
	IWorkspacePtr ipWorkspace;
	std::map<std::string, IDatasetPtr> mDatasetList;
	bool mStateOfEditing;
	bool mNewVersion;
	std::string mUserName;
};

#endif // !defined(AFX_SINDYDBCONNECTION_H__F90404A8_8B0E_4595_BFF7_5B396D630D07__INCLUDED_)
