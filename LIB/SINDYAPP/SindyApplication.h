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

// SindyApplication.h: SindyApplication クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINDYAPPLICATION_H__E160A640_31C1_436D_9F08_1DD54E2E8002__INCLUDED_)
#define AFX_SINDYAPPLICATION_H__E160A640_31C1_436D_9F08_1DD54E2E8002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SindyAppConfig.h"
#include "WKHNumList.h"
#include "SindyDBConnection.h"

class SindyApplication : public SindyAppConfig  
{
public:
	SindyApplication();
	virtual ~SindyApplication();

	virtual HRESULT init(int argc, char **argv);
	virtual HRESULT run() = 0;
	virtual HRESULT usage() = 0;
	virtual HRESULT search_connection(const std::string &cName, SindyDBConnection **cDBConnection);
	virtual HRESULT get_connection(long cID, SindyDBConnection **cDBConnection);

protected:
	HRESULT CreateConnection();
	HRESULT CreateConnection_FullDBName();
	void	GetConnectionID_FullDBName(std::set<long> *cList);
	void	GetConnectionID(std::set<long> *cList);
	HRESULT Connect(long cID, SindyDBConnection *cDBConnection);
	HRESULT	Connect_FullDBName(long id, SindyDBConnection *cDBConnection);

protected:
	SindyDBConnection *mpDBConnection;
	long mConnectionCount;

//	WKHNumList mValueList;
	bool mFillCache;
};

#endif // !defined(AFX_SINDYAPPLICATION_H__E160A640_31C1_436D_9F08_1DD54E2E8002__INCLUDED_)
