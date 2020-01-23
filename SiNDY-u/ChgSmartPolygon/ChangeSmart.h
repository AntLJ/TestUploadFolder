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

#include "SmartShape.h"

typedef std::list<CAdapt<IPointPtr>> LIST_POINTS;
typedef std::list<LIST_POINTS> LIST_LOOP;

class CChangeSmart
{
public:
	CChangeSmart(void);
	~CChangeSmart(void);

	bool ExecSmart(IFeatureClassPtr ipFeatureClass);
	bool ToSmart(IFeatureClassPtr ipFeatureClass, std::map<long, bool>& mapFeatureChk);

	void CreateShareSegment(long lBaseOID, long lTargetOID);
	void StorePolygon(IFeatureClassPtr ipFeatureClass);

	void SetOffset(double dOffset) { m_dAllowOffset = dOffset; };

private:
	double m_dAllowOffset;
	std::map<long, CSmartShape> m_mapSmartShape;
};
