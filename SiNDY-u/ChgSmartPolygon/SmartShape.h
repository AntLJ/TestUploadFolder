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

#include "SmartSegment.h"

class CSmartShape
{
public:
	CSmartShape(void);
	~CSmartShape(void);

	bool SetGeometry(const IGeometryPtr& ipGeom);
	void SetOID(long lOID) { m_lOID = lOID; };
	long GetOID() { return m_lOID; };
	void SetAllowOffset(double dOffset) { m_dAllowOffset = dOffset; };
	void SetEnvelope(IEnvelopePtr ipEnv) { m_ipEnvelope = ipEnv; };
	const IEnvelopePtr& GetEnvelope() const { return m_ipEnvelope; };

	bool ExecShareLine(CSmartShape& cSmart);
	bool _ExecShareLine(CSmartSegment& cSmartSegment, CSmartShape& cSmartTarget, bool bIsOuter);
	bool __ExecShareLine(CSmartSegment& cBase, CSmartSegment& cSegmentTarget, CSmartShape& cSmartTarget, bool bIsOuter);
	CSmartSegment& GetOuterSegment() { return m_cOuter; };
	std::list<CSmartSegment>& GetInnerSegments() { return m_listInner; };

	IPolygonPtr GetPolygon();

	long CheckCompEnvelope(const CSmartShape& cSmart);

	bool CheckCrossMyself(std::list<CSmartPath>& listSmart);
	bool CheckCross(const IGeometryPtr& ipGeom, const IGeometryPtr& ipOrgGeom, bool bOuter);

	bool SetGeneralize(bool bDo, long lOID, const CSmartPath& cSmart);

	bool ChangeAllSegment(IFeatureClassPtr ipFeatureClass = NULL);

private:
	long m_lOID;
	double m_dAllowOffset;
	long m_lInclude;

	IEnvelopePtr m_ipEnvelope;							// 高速化のため
	CSmartSegment m_cOuter;								// 基準ポリゴンのアウターループ
	std::list<CSmartSegment> m_listInner;				// 基準ポリゴンのインナーループ
};
