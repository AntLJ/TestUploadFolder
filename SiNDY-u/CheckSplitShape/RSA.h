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

#pragma once

#include "LayerBase.h"

class CRSA : public CRecodeBase {
public:
	CRSA(){}
	~CRSA(){}
};

class CRSAAll: public CTableBase {
public:
	CRSAAll(){}
	~CRSAAll(){}

public:
	virtual bool LoadData();

private:

public:
	std::map<UNIQID,CRSA> m_mapData;
	std::map<long,std::set<long>> m_mapLink2Segattr;
	std::set<LONLAT> m_setFromToLonLat;
};
