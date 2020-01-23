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

/**
 * @file FeatureClass.cpp
 * @brief CFeatureClassクラス実装
 * $Id$
 */
#include "stdafx.h"
#include "FeatureClass.h"
#include "ErrorCode.h"
#include "sindycomutil.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace winutil;
namespace sindy {
using namespace errorcode;

// IObjectClassインターフェース関数定義
LPCTSTR CFeatureClass::GetAliasName( LPTSTR lpszAliasName, int nLen ) const
{
	BSTR bstrName = 0;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_AliasName( &bstrName ) );
	LOGASSERTE_IF( BSTR2TCpy( lpszAliasName, bstrName, nLen ), sindyErr_BufferIsNotEnough );
	SysFreeString(bstrName);

	return lpszAliasName;
}

long CFeatureClass::GetObjectClassID() const
{
	long lID = -1;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_ObjectClassID( &lID ) );

	return lID;
}

// IFeatureClassインターフェース関数定義
IFeaturePtr CFeatureClass::CreateFeature()
{
	IFeaturePtr ipFeature;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, CreateFeature( &ipFeature ) );

	return ipFeature;
}

long CFeatureClass::FeatureCount( IQueryFilter* Filter ) const
{
	long lCount = -1;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, FeatureCount( Filter, &lCount ) );

	return lCount;
}

IFieldPtr CFeatureClass::GetAreaField() const
{
	IFieldPtr ipField;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_AreaField( &ipField ) );

	return ipField;
}

long CFeatureClass::GetFeatureClassID() const
{
	long lClassID = -1;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_FeatureClassID( &lClassID ) );

	return lClassID;
}

IFeatureDatasetPtr CFeatureClass::GetFeatureDataset() const
{
	IFeatureDatasetPtr ipDataset;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_FeatureDataset( &ipDataset ) );

	return ipDataset;
}

esriFeatureType CFeatureClass::GetFeatureType() const
{
	esriFeatureType type = (esriFeatureType)-1;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_FeatureType( &type ) );

	return type;
}

IFieldPtr CFeatureClass::GetLengthField() const
{
	IFieldPtr ipField;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_LengthField( &ipField ) );
	
	return ipField;
}
LPCTSTR CFeatureClass::GetShapeFieldName( LPTSTR lpszShapeFieldName, int nLen ) const
{
	BSTR bstrName = 0;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_ShapeFieldName( &bstrName ) );
	LOGASSERTE_IF( BSTR2TCpy( lpszShapeFieldName, bstrName, nLen ), sindyErr_BufferIsNotEnough );
	SysFreeString(bstrName);

	return lpszShapeFieldName;
}

esriGeometryType CFeatureClass::GetShapeType() const
{
	esriGeometryType type = (esriGeometryType)-1;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, get_ShapeType( &type ) );

	return type;
}

IFeaturePtr CFeatureClass::GetFeature( long OID ) const
{
	IFeaturePtr ipFeature;
	LOGASSERTE_IF( 0 <= OID, sindyErr_ArgLimitOver )
	{
		IFeatureClassPtr ipClass;
		LOGASSERTE_IF( SUCCEEDED( p->QueryInterface( IID_IFeatureClass, (void**)&ipClass ) ), sindyErr_COMQueryInterfaceFailed )
		{
			HRESULT hr = ipClass->GetFeature( OID, &ipFeature );
			switch( hr )
			{
				case FDO_E_FEATURE_NOT_FOUND: // フィーチャが見つからなかった場合は何もエラーを出さない（NULLで返すから分かるでしょ）
					break;
				default:
					LOGASSERTE_IF( SUCCEEDED(hr), sindyErr_COMFunctionFailed );
					break;
			}
		}
	}

	return ipFeature;
}

IFeatureCursorPtr CFeatureClass::GetFeatures( const VARIANT& fids, bool Recycling ) const
{
	IFeatureCursorPtr ipCursor;
	QUERYINVOKEMETHOD( IFeatureClass, IID_IFeatureClass, GetFeatures( fids, bool2VB(Recycling), &ipCursor ) );

	return ipCursor;
}

// ヘルパー関数
/// 空間参照を取得する
ISpatialReferencePtr CFeatureClass::GetSpatialReference() const
{
	ISpatialReferencePtr ipSpRef;
	IGeoDatasetPtr ipGeoDataset( p );
	if( ipGeoDataset != NULL )
		ipGeoDataset->get_SpatialReference( &ipSpRef );

	return ipSpRef;
}

// OIDリストで検索する
IFeatureCursorPtr CFeatureClass::GetFeatures( const std::list<long>& fids, bool Recycling ) const
{
	SAFEARRAYBOUND bound = { fids.size(), 0 };
	SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
	LONG* pArray;
	SafeArrayAccessData( pSa, (void**)&pArray );

	long i = 0;
	for( std::list<long>::const_iterator it = fids.begin(); it != fids.end(); ++it, ++i )
		pArray[i] = *it;

	SafeArrayUnaccessData( pSa );
	VARIANT vaArray;
	VariantInit( &vaArray );
	vaArray.vt = VT_ARRAY | VT_I4;
	vaArray.parray = pSa;

	IFeatureCursorPtr ipCursor = GetFeatures( vaArray, Recycling );
	VariantClear( &vaArray );

	return ipCursor;
}

} // sindy
