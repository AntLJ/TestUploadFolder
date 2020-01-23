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

// SindyApplication.cpp: SindyApplication クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyApplication.h"


SindyApplication::SindyApplication() :
mpDBConnection(0),
mConnectionCount(0),
//mValueList(),
mFillCache(false)
{

}

SindyApplication::~SindyApplication()
{
//	for (long i = 0; i < mConnectionCount; i++) (mpDBConnection+i)->DeleteVersion();
	delete [] mpDBConnection;
}

HRESULT SindyApplication::init(int argc, char **argv)
{
	const char *func = "SindyApplication::init";
	HRESULT hr;

	if (argc <= 1) return usage();

	if (FAILED(hr = Read(argv[1]))) return hr;

	// コネクションをはる
	if (FAILED(hr = CreateConnection())) return hr;

	return S_OK;
}

HRESULT SindyApplication::search_connection(const std::string &cName, SindyDBConnection **cDBConnection)
{
	*cDBConnection = NULL;

	for (long i = 0; i < mConnectionCount; i++)
	{
		IDatasetPtr ipDataset = (mpDBConnection+i)->SearchDataset(cName);
		if (ipDataset != NULL)
		{
			*cDBConnection = mpDBConnection+i;
			return S_OK;
		}
	}

	*cDBConnection = mpDBConnection+0;

	return S_OK;
}

HRESULT SindyApplication::get_connection(long cID, SindyDBConnection **cDBConnection)
{
	*cDBConnection = mpDBConnection+cID;
	return S_OK;
}

HRESULT SindyApplication::CreateConnection()
{
	const char *func = "SindyApplication::CreateConnection";
	HRESULT hr;

	std::set<long> aIDList;
	GetConnectionID(&aIDList);
	if (! (mConnectionCount = (long)aIDList.size())) GDBERROR2(E_ACCESSDENIED, func);

	delete [] mpDBConnection;

	if (! (mpDBConnection = new SindyDBConnection [aIDList.size()]))
		return GDBERROR2(E_OUTOFMEMORY, func);

	long i = 0;
	for (std::set<long>::iterator aID = aIDList.begin(); aID != aIDList.end(); aID++, i++)
	{
		if (FAILED(hr = Connect(*aID, mpDBConnection+i))) return hr;
	}

	return S_OK;
}

HRESULT SindyApplication::CreateConnection_FullDBName()
{
	const char *func = "SindyRoadConvertApp::CreateConnection_FullDBName";
	HRESULT hr;

	std::set<long> aIDList;
	GetConnectionID_FullDBName(&aIDList);
	if(! (mConnectionCount = (long)aIDList.size()) ) GDBERROR2(E_ACCESSDENIED, func);

	delete []mpDBConnection;

	if(! (mpDBConnection = new SindyDBConnection [aIDList.size()]))
		return GDBERROR2(E_OUTOFMEMORY, func);

	long i = 0;
	for  (std::set<long>::iterator aID = aIDList.begin(); aID != aIDList.end(); aID++, i++)
	{
		if( FAILED(hr = Connect_FullDBName(*aID, mpDBConnection+i))) return hr;
	}

	return S_OK;
}


void SindyApplication::GetConnectionID(std::set<long> *cList)
{
	cList->clear();
	std::set<AppConfig,lessAppConfig>::iterator aIter = mConfiguration.begin();
	for (; aIter != mConfiguration.end(); aIter++)
	{
		if (! strcmp(aIter->eKeyword.c_str(), "SERVER"))
		{
			cList->insert(0);
		}
		else if (strstr(aIter->eKeyword.c_str(), "SERVER") != 0)
		{
			std::string aTemp = aIter->eKeyword.substr(strlen("SERVER"));
			cList->insert(boost::lexical_cast<long>(aTemp));
		}
	}
	return;
}

void SindyApplication::GetConnectionID_FullDBName(std::set<long> *cList)
{
	cList->clear();
	std::set<AppConfig,lessAppConfig>::iterator aIter = mConfiguration.begin();
	for(; aIter != mConfiguration.end(); aIter++)
	{
		if(! strcmp(aIter->eKeyword.c_str(), "DATABASE"))
		{
			cList->insert(0);
		}
		else if (strstr(aIter->eKeyword.c_str(), "DATABASE") != 0)
		{
			std::string aTemp = aIter->eKeyword.substr(strlen("DATABASE"));
			cList->insert(boost::lexical_cast<long>(aTemp));
		}
	}
	return;
}

HRESULT SindyApplication::Connect(long id, SindyDBConnection *cDBConnection)
{
	const char *func = "SindyApplication::Connect";
	HRESULT hr;

	std::string aKey = "";
	std::string aServer(""), aInstance(""), aDatabase(""), aUser(""), aPassword(""), aVersion(""), aNewVersion("");
	std::vector<std::string> aList;

	aKey = "SERVER";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aServer);

	aKey = "INSTANCE";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aInstance);

	aKey = "DATABASE";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aDatabase);

	aKey = "USER";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aUser);

	aKey = "PASSWORD";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aPassword);

	aKey = "VERSION";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aVersion);

	aKey = "NEWVERSION";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aNewVersion);

	if (FAILED(hr = cDBConnection->Connect(aServer, aInstance, aDatabase, aUser, aPassword, aVersion)))
		return hr;
	if (aNewVersion.size())
	{
		if (FAILED(hr = cDBConnection->CreateVersion(aNewVersion)))
			return hr;
	}

	aKey = "LAYER";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aList);
	if (aList.size())
	{
		for (unsigned long i = 0; i < aList.size(); i++)
		{
			if (FAILED(hr = cDBConnection->OpenDataset(aList[i]))) return hr;
		}
	}
	else
	{
		if (FAILED(hr = cDBConnection->OpenDataset())) return hr;
	}

	return S_OK;
}

HRESULT SindyApplication::Connect_FullDBName(long id, SindyDBConnection *cDBConnection)
{
	const char *func = "SindyRoadConvertApp::Coonect_FullDBName";
	HRESULT hr;

	std::string aKey("");
	std::string aDataBase(""),aNewVersion("");
	std::vector<std::string> aList;

	aKey = "DATABASE";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aDataBase);

	aKey = "NEWVERSION";
	if (id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aNewVersion);

	if (FAILED(hr = cDBConnection->Connect(aDataBase)))
		return hr;
	if (aNewVersion.size())
	{
		if (FAILED(hr = cDBConnection->CreateVersion(aNewVersion)))
			return hr;
	}
	
	aKey = "LAYER";
	if(id) aKey += boost::lexical_cast<std::string>(id);
	GetConfig(aKey, &aList);
	if (aList.size())
	{
		for (unsigned long i = 0; i < aList.size(); i++)
		{
			if (FAILED(hr = cDBConnection->OpenDataset(aList[i]))) return hr;
		}
	}
	else
	{
		if (FAILED(hr = cDBConnection->OpenDataset())) return hr;
	}

	return S_OK;
}
