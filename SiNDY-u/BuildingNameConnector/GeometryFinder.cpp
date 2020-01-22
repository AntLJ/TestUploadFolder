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

// GeometryFinder.cpp: CGeometryFinder クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeometryFinder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CGeometryFinder::CGeometryFinder() : m_unitpos(), m_unitdata()
{
	m_env.XMin = m_env.YMin = m_env.XMax = m_env.YMax = 0.0;
	m_xcount = m_ycount = 0;
	m_xwidth = m_ywidth = 0.0;
}

CGeometryFinder::CGeometryFinder(const WKSEnvelope& env, long xcnt, long ycnt) : m_unitpos(), m_unitdata()
{
	m_env.XMin = m_env.YMin = m_env.XMax = m_env.YMax = 0.0;
	m_xcount = m_ycount = 0;
	m_xwidth = m_ywidth = 0.0;
	Initialize(env, xcnt, ycnt);
}

CGeometryFinder::CGeometryFinder(IEnvelopePtr ipEnv, long xcnt, long ycnt) : m_unitpos(), m_unitdata()
{
	m_env.XMin = m_env.YMin = m_env.XMax = m_env.YMax = 0.0;
	m_xcount = m_ycount = 0;
	m_xwidth = m_ywidth = 0.0;
	WKSEnvelope env;
	ipEnv->QueryWKSCoords(&env);
	Initialize(env, xcnt, ycnt);
}

CGeometryFinder::~CGeometryFinder()
{

}

void CGeometryFinder::Clear()
{
	m_unitdata.clear();
}

void CGeometryFinder::Initialize(const WKSEnvelope& wksEnvelope, long xgrid, long ygrid)
{
	m_xcount = xgrid; m_ycount = ygrid;
	m_env = wksEnvelope;

	m_xwidth = (wksEnvelope.XMax - wksEnvelope.XMin) / (double)xgrid;
	m_ywidth = (wksEnvelope.YMax - wksEnvelope.YMin) / (double)ygrid;

	for (long y = 0; y < ygrid; y++)
	{
		for (long x = 0; x < xgrid; x++)
		{
			WKSEnvelope env;
			env.XMin = wksEnvelope.XMin + (m_xwidth * x);
			env.XMax = (x == xgrid - 1) ? wksEnvelope.XMax : env.XMin + m_xwidth;
			env.YMin = wksEnvelope.YMin + (m_ywidth * y);
			env.YMax = (y == ygrid - 1) ? wksEnvelope.YMax : env.YMin + m_ywidth;
			m_unitdata.insert(UNITDATA::value_type(env, std::vector<FeatureInfo>()));
			m_unitpos.insert(UNITPOS::value_type(UnitPos(x, y), env));
		}
	}

	m_ipFeatureClass = 0;
}

bool CGeometryFinder::SetData(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter)
{
	__FUNC__(CGeometryFinder::SetData);

	// データの検索(ここに到達するフィルタにはWHERE句は設定されていないため、フィルタ条件分離の必要なし)
	// [Bug 8428]の処理高速化対応 2010/12/6 ta_suga
	IFeatureCursorPtr ipFeatureCursor = 0;
	if (FAILED(ipFeatureClass->Search(ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor)))
		ERRHANDLE(0, false);

	IFeaturePtr ipFeature = 0;
	while (ipFeatureCursor->NextFeature(&ipFeature) == S_OK)
	{
		if (ipFeature == 0)
			continue;

		IGeometryPtr ipShape = 0;
		if (FAILED(ipFeature->get_ShapeCopy(&ipShape)))
			ERRHANDLE(0, false);

		std::vector<WKSEnvelope> unit;
		if (! GetUnit(ipShape, unit))
			return false;

		for (ULONG i = 0; i < unit.size(); i++)
		{
			UNITDATA::iterator it = m_unitdata.find(unit[i]);
			if (it == m_unitdata.end())
				continue;
			it->second.push_back(FeatureInfo(ipFeature));
		}
	}

	m_ipFeatureClass = ipFeatureClass;

	return true;
}

bool CGeometryFinder::GetUnit(IGeometryPtr ipGeometry, std::vector<WKSEnvelope>& unit)
{
	__FUNC__(CGeometryFinder::GetUnit);

	IEnvelopePtr ipEnvelope = 0;
	if (FAILED(ipGeometry->get_Envelope(&ipEnvelope)))
		ERRHANDLE(0, false);

	WKSEnvelope env;
	if (FAILED(ipEnvelope->QueryWKSCoords(&env)))
		ERRHANDLE(0, false);

	return GetUnit(env, unit);
}

bool CGeometryFinder::GetUnit(const WKSEnvelope& env, std::vector<WKSEnvelope>& unit)
{
	long xmin = (long)((env.XMin - m_env.XMin) / m_xwidth);
	long xmax = (long)((env.XMax - m_env.XMin) / m_xwidth);
	long ymin = (long)((env.YMin - m_env.YMin) / m_ywidth);
	long ymax = (long)((env.YMax - m_env.YMin) / m_ywidth);
	for (long y = ymin; y <= ymax; y++)
	{
		for (long x = xmin; x <= xmax; x++)
		{
			UNITPOS::iterator it = m_unitpos.find(UnitPos(x, y));
			if (it == m_unitpos.end())
				continue;
			unit.push_back(it->second);
		}
	}
	return true;
}

bool CGeometryFinder::FindNearestFeature(double longitude, double latitude, double xrange, double yrange, IFeature** ipFeature, double* crslon, double* crslat)
{
	*ipFeature = 0;
	*crslon = *crslat = 0.0;

	WKSEnvelope env;
	g_cCrdCnv.GetLLPt(longitude, latitude, xrange, yrange, &env.XMax, &env.YMax);
	g_cCrdCnv.GetLLPt(longitude, latitude, -xrange, -yrange, &env.XMin, &env.YMin);

	std::vector<WKSEnvelope> unit;
	if (! GetUnit(env, unit))
		return false;

	IPointPtr ipPoint(CLSID_Point);
	if (FAILED(ipPoint->PutCoords(longitude, latitude)))
		return false;
	IProximityOperatorPtr ipProximityOperator(ipPoint);

	double min_dist = 0.0;
	IFeaturePtr ipNearestFeature = 0;
	for (ULONG i = 0; i < unit.size(); i++)
	{
		UNITDATA::iterator ud = m_unitdata.find(unit[i]);
		if (ud == m_unitdata.end())
			continue;
		for (ULONG j = 0; j < ud->second.size(); j++)
		{
			if (! IsOverlapped(ud->second[j].m_wksEnvelope, env))
				continue;
			IGeometryPtr ipShape = 0;
			if (FAILED(ud->second[j].m_ipFeature->get_ShapeCopy(&ipShape)))
				return false;
			double d = 0.0;
			if (FAILED(ipProximityOperator->ReturnDistance(ipShape, &d)))
				return false;
			if (ipNearestFeature == 0 || min_dist > d)
			{
				min_dist = d;
				ipNearestFeature = ud->second[j].m_ipFeature;
			}
		}
	}

	if (ipNearestFeature != 0)
	{
		IGeometryPtr ipGeometry = 0;
		if (FAILED(ipNearestFeature->get_ShapeCopy(&ipGeometry)))
			return false;
		IProximityOperatorPtr ipProx(ipGeometry);
		IPointPtr ipNearestPoint = 0;
		if (FAILED(ipProx->ReturnNearestPoint(ipPoint, esriNoExtension, &ipNearestPoint)))
			return false;
		if (FAILED(ipNearestPoint->get_X(crslon)))
			return false;
		if (FAILED(ipNearestPoint->get_Y(crslat)))
			return false;
		*ipFeature = ipNearestFeature;
		(*ipFeature)->AddRef();
	}

	return true;
}

bool CGeometryFinder::FindCandidate(IGeometryPtr ipGeometry, std::vector< CAdapt<IFeaturePtr> >& foundFeatures)
{
	__FUNC__(CGeometryFinder::FindCandidate);

	foundFeatures.clear();

	WKSEnvelope findEnv;
	{
		IEnvelopePtr ipEnvelope = 0;
		if (FAILED(ipGeometry->get_Envelope(&ipEnvelope)))
			ERRHANDLE(0, false);
		if (FAILED(ipEnvelope->QueryWKSCoords(&findEnv)))
			ERRHANDLE(0, false);
	}

	std::vector<WKSEnvelope> unit;
	if (! GetUnit(findEnv, unit))
		return false;

	std::set<long> check;

	for (ULONG i = 0; i < unit.size(); i++)
	{
		UNITDATA::iterator ud = m_unitdata.find(unit[i]);
		if (ud == m_unitdata.end())
			continue;
		for (ULONG j = 0; j < ud->second.size(); j++)
		{
			if (! IsOverlapped(ud->second[j].m_wksEnvelope, findEnv))
				continue;
			long lOID;
			if (FAILED(ud->second[j].m_ipFeature->get_OID(&lOID)))
				ERRHANDLE(0, false);
			if (check.find(lOID) != check.end())
				continue;
			foundFeatures.push_back(ud->second[j].m_ipFeature);
			check.insert(lOID);
		}
	}

	return true;
}
