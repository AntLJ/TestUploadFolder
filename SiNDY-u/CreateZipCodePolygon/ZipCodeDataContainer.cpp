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

#include "stdafx.h"
#include "ZipCodeDataContainer.h"


ZipCodeDataContainer::ZipCodeDataContainer(void) : m_geometryIndex(-1)
{
}


ZipCodeDataContainer::~ZipCodeDataContainer(void)
{
}

bool ZipCodeDataContainer::CompareZipCode(const ZipCodeDataContainer& target)
{
	return (0 == m_zipcode.Compare(target.m_zipcode));
}

bool ZipCodeDataContainer::CompareAddrCode(const ZipCodeDataContainer& target)
{
	return (
		0 == m_prefCode.Compare(target.m_prefCode) &&
		0 == m_cityCode.Compare(target.m_cityCode) &&
		0 == m_oazaCode.Compare(target.m_oazaCode) &&
		0 == m_azaCode.Compare(target.m_azaCode));
}

void ZipCodeDataContainer::SetData(
	const CString& adminCityCode,
	const CString& adminAddrCode,
	const CString& adminZipCode,
	const IGeometryPtr& adminGeometry)
{
	m_prefCode = adminCityCode.Left(2);
	m_cityCode = adminCityCode.Right(3);
	m_oazaCode = adminAddrCode.Left(3);
	m_azaCode = adminAddrCode.Right(3);
	m_zipcode = adminZipCode;
	m_geometry = adminGeometry;
}

bool ZipCodeDataContainer::IsMultiPolygon()
{
	IGeometryCollectionPtr ipGeometryCollection(m_geometry);
	if( !ipGeometryCollection )
		return false;

	long geometryCount = 0;
	ipGeometryCollection->get_GeometryCount(&geometryCount);

	return (1 < geometryCount);
}

void ZipCodeDataContainer::SetPolygonId(long id)
{
	m_polygonId = id;
}

void ZipCodeDataContainer::SetGeometry(const IGeometryPtr& geometry)
{
	m_geometry = geometry;
}

double ZipCodeDataContainer::CalcArea(const IGeometryPtr& geometry)
{
	IAreaPtr polygon(geometry);
	if( !polygon )
		return 0;

	double area = 0;
	polygon->get_Area(&area);
	return area;
}
