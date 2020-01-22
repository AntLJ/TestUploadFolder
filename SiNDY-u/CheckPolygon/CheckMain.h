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

#include "ParamCtrl.h"

class CCheckMain
{
public:
	CCheckMain(void);
	~CCheckMain(void);

	int Run(const CParamCtrl& cParam);

private:
	IFeatureClassPtr GetFeatureClass(const CString& strDB, const CString& strFeatureClass);
	
	bool CheckCrossMyself(long lOID, const IGeometryPtr& ipGeom);
	bool CheckMultiPolygon(long lOID, const IGeometryPtr& ipGeom);
	bool CheckClearance(IFeatureClassPtr ipFeatureClass, long lOID, const IGeometryPtr& ipGeom);
};
