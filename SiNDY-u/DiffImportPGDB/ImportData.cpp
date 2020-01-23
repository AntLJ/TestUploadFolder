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
#include "ImportData.h"

using namespace sindy;

CImportData::CImportData(void):
	m_updateStatus(0)
{
}


CImportData::~CImportData(void)
{
}


bool CImportData::UnionGeometry( const IFeaturePtr& ipFeatuer )
{
	IGeometryPtr ipCurrentGeometry, ipTargetGeometry;
	m_feature->get_ShapeCopy( &ipCurrentGeometry );
	ipFeatuer->get_ShapeCopy( &ipTargetGeometry );

	if( !ipCurrentGeometry )
		return false;

	ISpatialReferencePtr ipSpref;
	ipCurrentGeometry->get_SpatialReference( &ipSpref );

	IGeometryPtr tmpGeometry = ipCurrentGeometry;
	if( FAILED(((ITopologicalOperatorPtr)tmpGeometry)->Union( ipTargetGeometry, &ipCurrentGeometry )) )
	{
		return false;
	}

	ipCurrentGeometry->putref_SpatialReference( ipSpref );
	m_feature->putref_Shape( ipCurrentGeometry );
	return true;
}
