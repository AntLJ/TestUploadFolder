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
#include "globals.h"
#include <sindy/workspace.h>

IFeatureClassPtr GetFeatureClass(const CString& strDir, const CString& strFileName, IWorkspacePtr& ipWorkspace)
{
	ipWorkspace = sindy::create_workspace( strDir );
	if( !ipWorkspace )
		return NULL;

	IFeatureClassPtr ipFeatureC_Area;
	IFeatureWorkspacePtr( ipWorkspace )->OpenFeatureClass( CComBSTR( strFileName ), &ipFeatureC_Area );
	if( !ipFeatureC_Area )
		return NULL;

	return ipFeatureC_Area;
}

void OutPutTime(std::string str)
{
	CTime cTime = CTime::GetCurrentTime();
	long lHour = cTime.GetHour();
	long lMinute = cTime.GetMinute();
	long lSecond = cTime.GetSecond();
	
	CString cStrTime;
	cStrTime.Format( _T("%02d:%02d:%02d"), lHour, lMinute, lSecond );
	std::string strTime = CT2CA( cStrTime );

	fprintf( stderr, "%s\t%s\n", strTime.c_str(), str.c_str() );
	fflush( stderr );
}

bool CheckCrossMyself(const IGeometryPtr& ipGeom)
{
	ISegmentCollectionPtr ipSegCol( ipGeom );
	if( !ipSegCol )
		return false;

	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );
	long lSegCnt = 0;
	ipSegCol->get_SegmentCount( &lSegCnt );
	for( long l=0; l<lSegCnt; ++l )
	{
		_ISegmentPtr ipBaseSegment;
		ipSegCol->get_Segment( l, &ipBaseSegment );

		ISegmentCollectionPtr ipBasePoly( CLSID_Polyline );
		((IGeometryPtr)ipBasePoly)->putref_SpatialReference( ipSpRef );
		ipBasePoly->AddSegment( ipBaseSegment );

		for( long j=l+1; j<lSegCnt; ++j )
		{
			_ISegmentPtr ipTargetSegment;
			ipSegCol->get_Segment( j, &ipTargetSegment );

			ISegmentCollectionPtr ipTargetPoly( CLSID_Polyline );
			((IGeometryPtr)ipTargetPoly)->putref_SpatialReference( ipSpRef );
			ipTargetPoly->AddSegment( ipTargetSegment );

			if( AheIsCross( (IGeometryPtr)ipBasePoly, (IGeometryPtr)ipTargetPoly ) )
				return false;
		}
	}

	return true;
}
