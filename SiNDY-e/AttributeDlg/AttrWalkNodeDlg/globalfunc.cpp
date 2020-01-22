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
#include "globalfunc.h"


namespace gf {

// 周辺検索
IFeatureCursorPtr AroundSearch( IFeature* ipFeature, IFeatureClass* ipFeatClass, double dRange, LPCTSTR strWhereClause/*=NULL*/ )
{
	IFeatureCursorPtr ipRetCursor;

	if( ipFeature && ipFeatClass )
	{
		IGeometryPtr ipTargetGeo;
		ipFeature->get_ShapeCopy( &ipTargetGeo );
		if( ipTargetGeo )
		{
			IEnvelopePtr ipEnv( CLSID_Envelope );
			ipTargetGeo->get_Envelope( &ipEnv );
			double dConvRange = 0.0;
			IUnitConverterPtr ipUnitConv( CLSID_UnitConverter );
			ipUnitConv->ConvertUnits( dRange, esriMeters, esriDecimalDegrees, &dConvRange );
			// TODO: 正しいやり方が見つかったらそちらを使用する
			// この方法では、場所によっては、指定のm以内に引っかからないので、適当に広げる
			// タイを囲むメッシュの端で試したときに、下記の値程度に拡げないと、計測上は指定メートル以内でも
			// 検出できないフィーチャが存在したため
			dConvRange *= 1.3;
			if(FAILED(ipEnv->Expand( dConvRange, dConvRange, VARIANT_FALSE )))
				return nullptr;

			// 空間検索
			AheSelectByShapeAndGetCount( ipEnv, ipFeatClass, &ipRetCursor, esriSpatialRelIntersects, NULL, strWhereClause );
		}
	}

	return ipRetCursor;
}

// ポイントフィーチャを点滅させる
void BlinkFeature( IScreenDisplay* ipScDisp, LONG lOID, IFeatureClass* ipFeatClass, DWORD sleepTime/*=100*/, unsigned int blinkCount/*=4*/ )
{
	IFeaturePtr ipFeature;
	ipFeatClass->GetFeature( lOID, &ipFeature );
	if( ipFeature )
	{
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		IPointPtr ipPoint(ipGeom);
		if( ipPoint )
		{
			for(unsigned int i=0; i<blinkCount; ++i)
			{
				AheDrawPoint( ipScDisp, ipPoint, TRUE, 8, 200, 200, 200, esriSMSSquare, TRUE, TRUE ); ::Sleep( sleepTime );
				AheDrawPoint( ipScDisp, ipPoint, TRUE, 8, 200, 200, 200, esriSMSSquare, TRUE, TRUE ); ::Sleep( sleepTime );
			}
		}
	}
}

CString VariantToCString( const CComVariant& variant, LPCTSTR strNull )
{
	// VT_NULLでなければ文字列型を保持するという前提での処理
	CString str;
	if(VT_BSTR==variant.vt)
	{
		// 文字列型であるなら文字列を格納
		str = variant.bstrVal;
	}
	else
	{
		// 文字列型でないならNULL文字列を格納
		str = strNull;

	}
	return str;
}

} // namespace gf
