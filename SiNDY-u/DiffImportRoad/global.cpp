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
#include <WinLib/arcobjects_import_highmethod.h>
#include <sindy/workspace.h>
#include <schema.h>
#include "global.h"

CTimeTrace g_timetrace; //<! 時間計測用

using namespace std;
using namespace sindy;
using namespace sindy::schema;

bool IsSameGeom( const IGeometryPtr& ipGeom1, const IGeometryPtr& ipGeom2 )
{
	IPointPtr ipPoint1( ipGeom1 ), ipPoint2( ipGeom2 );
	if( ( !ipPoint1 && ipPoint2 ) || ( ipPoint1 && !ipPoint2 ) )
	{
		_ASSERT(0);
		return false;
	}
	else if( ipPoint1 && ipPoint2 )
	{
		WKSPoint wksPoint1, wksPoint2;
		ipPoint1->QueryCoords( &wksPoint1.X, &wksPoint1.Y );
		ipPoint2->QueryCoords( &wksPoint2.X, &wksPoint2.Y );
		if( wksPoint1.X != wksPoint2.X || wksPoint1.Y != wksPoint2.Y )
			return false;
	}
	else
	{
		long lCount1 = 0, lCount2 = 0;
		IPointCollectionPtr(ipGeom1)->get_PointCount( &lCount1 );
		IPointCollectionPtr(ipGeom2)->get_PointCount( &lCount2 );
	
		// ポイント数が異なれば同一形状ではない
		if( lCount1 != lCount2 )
			return false;

		vector<WKSPoint> vecWks1(lCount1);
		vector<WKSPoint> vecWks2(lCount2);

		// ポイント数が同じ場合は各構成点の緯度経度を比較
		IPointCollectionPtr(ipGeom1)->QueryWKSPoints( 0, lCount1, &vecWks1[0] );
		IPointCollectionPtr(ipGeom2)->QueryWKSPoints( 0, lCount2, &vecWks2[0] );
		for( long lVertex = 0; lVertex < lCount1; ++lVertex )
		{
			if( vecWks1[lVertex].X != vecWks2[lVertex].X || vecWks1[lVertex].Y != vecWks2[lVertex].Y )
				return false;
		}
	}

	return true;
}

CString GetTableName( const ITablePtr& ipTable )
{
	CComBSTR bstrName;
	IDatasetPtr(ipTable)->get_Name( &bstrName );
	return CString(bstrName);
}

bool IsAGOPSTable( const CString strTableName )
{
	// リンク内属性、傾斜、一方通行、通行規制、簡易傾斜、一通/通禁車種別規制の場合
	if( 0 == _tcsicmp( strTableName, segment_attr::kTableName ) ||
		0 == _tcsicmp( strTableName, grad::kTableName ) || 
		0 == _tcsicmp( strTableName, time_oneway::kTableName ) ||
		0 == _tcsicmp( strTableName, time_nopassage::kTableName ) || 
		0 == _tcsicmp( strTableName, simple_grad::kTableName ) || 
		0 == _tcsicmp( strTableName, vehicle_reg::kTableName ))
		return true;
	else
		return false;
}

bool IsVehicleTypeTable( const CString strTableName )
{
	if( 0 == _tcsicmp( strTableName, vehicle_type::kTableName ) )
		return true;
	else
		return false;
}

bool IsInfTable( const CString strTableName )
{
	// INFの場合
	if( 0 == _tcsicmp( strTableName, inf_byway::kTableName ) ||
		0 == _tcsicmp( strTableName, inf_dirguide::kTableName ) || 
		0 == _tcsicmp( strTableName, inf_guide::kTableName ) ||
		0 == _tcsicmp( strTableName, inf_internavi::kTableName ) || 
		0 == _tcsicmp( strTableName, inf_intersection::kTableName ) ||
		0 == _tcsicmp( strTableName, inf_lane::kTableName ) || 
		0 == _tcsicmp( strTableName, inf_route::kTableName ) || 
		0 == _tcsicmp( strTableName, inf_turnreg::kTableName ) || 
		0 == _tcsicmp( strTableName, inf_uturn::kTableName ) ||
		0 == _tcsicmp( strTableName, inf_vics::kTableName ))
		return true;
	else
		return false;
}
bool IsLqTable( const CString strTableName )
{
	// LQの場合
	if( 0 == _tcsicmp( strTableName, lq_byway::kTableName ) ||
		0 == _tcsicmp( strTableName, lq_dirguide::kTableName ) || 
		0 == _tcsicmp( strTableName, lq_guide::kTableName ) ||
		0 == _tcsicmp( strTableName, lq_internavi::kTableName ) || 
		0 == _tcsicmp( strTableName, lq_intersection::kTableName ) ||
		0 == _tcsicmp( strTableName, lq_lane::kTableName ) || 
		0 == _tcsicmp( strTableName, lq_route::kTableName ) || 
		0 == _tcsicmp( strTableName, lq_turnreg::kTableName ) || 
		0 == _tcsicmp( strTableName, lq_uturn::kTableName ) ||
		0 == _tcsicmp( strTableName, lq_vics::kTableName ))
		return true;
	else
		return false;
}

CString getNowTime()
{
	SYSTEMTIME stSystemTime;
	::GetLocalTime( &stSystemTime);
	CString strNowTime;
	strNowTime.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), 
	stSystemTime.wYear,	stSystemTime.wMonth, stSystemTime.wDay,
	stSystemTime.wHour,	stSystemTime.wMinute,stSystemTime.wSecond);

	return strNowTime;
}
