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
#include "GlobalFunc.h"

namespace gf
{
	// ��ԁE��������(�������擾��)
	LONG SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursor** ppFeatureCursor, 
					esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		/// �N�G���t�B���^�쐬
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		ipFeatureClass->get_ShapeFieldName( &bstrFieldName );
		ipFilter->put_GeometryField( bstrFieldName );
		ipFilter->putref_Geometry( ipGeo );
		ipFilter->put_SpatialRel( spatialRel );

		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause != NULL )
		{
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );	// Where��Z�b�g
			ipFilter->put_SearchOrder( esriSearchOrderSpatial );	// �����D�揇�ʂ��w�肷��
		}

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields != NULL )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		/// �������ʂ� 0 �Ȃ烊�^�[��
		LONG lHitCount = 0;
		ipFeatureClass->FeatureCount( ipFilter, &lHitCount );
		if( lHitCount < 1 )
			return 0;

		/// ����
		ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor );
		if( NULL == *ppFeatureCursor )
			return 0;
				
		return lHitCount;
	}
}
