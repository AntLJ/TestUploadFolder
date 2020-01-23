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

#include "globals.h"

class CSmartSegment
{
public:
	CSmartSegment(void);
	~CSmartSegment(void);

	void SetOffset(double dOffset) { m_dAllowOffset = dOffset; };
	IPolylinePtr& GetBaseSegment() { return m_ipPolyline; };
	void SetBaseSegment(const IPolylinePtr& ipPolyline);
	void SetBaseSegment(const IRingPtr& ipRing);
	void AddSharePath(CSmartPath cSmart) { m_listSharePath.push_back( cSmart ); };
	IPolygonPtr GetPolygon();
	bool CheckCrossMyself(std::list<CSmartPath>& listSmart);
	bool CheckCross(const IGeometryPtr& ipGeom, const IGeometryPtr& ipOrgGeom);
	bool CheckIntersectGeom(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeom);
	bool SetGeneralize(bool bDo, long lOID, const CSmartPath& cSmart);

	bool ChangeAllSegment(IFeatureClassPtr ipFeatureClass = NULL);
	bool CheckTurnDir(const IPolylinePtr& ipTarget, const _ISegmentPtr& ipBaseSeg, const _ISegmentPtr& ipOrgSeg);

	bool CheckRing(const IGeometryPtr& ipGeom);

private:
	void GetAllPath(std::list<CAdapt<IPathPtr>>& listPath);

private:
	double m_dAllowOffset;
	IPolylinePtr	m_ipPolyline;
	std::list<CSmartPath>		m_listSharePath;
};
