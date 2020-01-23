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
#include <crd_cnv/crd_cnv.h>
#include "Properties.h"

#pragma warning(disable:4244)

class StationGateFinder
{
	bool initialized;
	static crd_cnv converter;

	typedef std::map<long, WKSPoint> Cache;
	Cache cache;

public:
	StationGateFinder(LPCTSTR filename);
	virtual ~StationGateFinder(void);

	bool is_open() { return initialized; }
	long find(long id, double x, double y, crd_cnv *crd_cnv);

private:
	bool init(LPCTSTR filename);
};

