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
	// �h���C���}�b�v�擾�֐�
	bool GetDomainMap( ITablePtr ipTable, LPCTSTR lpcszFieldName, DOMAINMAP* map )
	{
		// �t�B�[���h������t�B�[���hID�擾
		long lFieldID = -1;
		if( FAILED(ipTable->FindField( CComBSTR(lpcszFieldName), &lFieldID )) )
			return false; 

		// IField�擾����
		IFieldsPtr ipFields;
		if( FAILED(ipTable->get_Fields( &ipFields )) )
			return false;

		IFieldPtr ipField;
		if( FAILED(ipFields->get_Field( lFieldID, &ipField )) )
			return false;

		// IDomain�擾���āAmap�𐶐�
		IDomainPtr ipDomain;
		if( FAILED(ipField->get_Domain( &ipDomain )) )
			return false;		// �h���C�������������ɊY�����Ȃ����H�v�m�F�B

		ICodedValueDomainPtr ipCVDomain( ipDomain );
		if( NULL == ipCVDomain )
			return true;		// �h���C�������H

		LONG lDomainCount;
		if( FAILED( ipCVDomain->get_CodeCount( &lDomainCount )) )
			return false;

		BSTR bstr;
		for( long i = 0; i < lDomainCount; i++ )
		{
			CComVariant vaValue;
			if( FAILED( ipCVDomain->get_Value( i, &vaValue )) )
				return false;
			if( FAILED( ipCVDomain->get_Name( i, &bstr )) )
				return false;

			map->insert( std::pair<long, CString>( vaValue.lVal, CString(bstr) ) );
			::SysFreeString( bstr );
		}

		return true;
	}

	// ��ԁE��������(�������擾��)
	bool SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long* pCount,
								 esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		if( FAILED(ipFeatureClass->get_ShapeFieldName( &bstrFieldName )) )
			return false;
		if( FAILED(ipFilter->put_GeometryField( bstrFieldName )) )
			return false;
		if( FAILED(ipFilter->putref_Geometry( ipGeo )) )
			return false;
		if( FAILED(ipFilter->put_SpatialRel( spatialRel )) )
			return false;
		
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
		{
			// Where��Z�b�g
			if( FAILED(ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) )) )
				return false;
			// �����D�揇�ʂ��w�肷��
			if( FAILED(ipFilter->put_SearchOrder( esriSearchOrderSpatial )) )
				return false;
		}

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields )
			if( FAILED(ipFilter->put_SubFields( CComBSTR(lpszSubFields) )) )
				return false;

		// �������ʂ� 0 �Ȃ琳�탊�^�[��
		if( FAILED(ipFeatureClass->FeatureCount( ipFilter, pCount )) )
			return false;
		if( *pCount < 1 )
			return true;

		// ����
		if( FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, &ipFeatureCursor )) )
			return false;

		return true;
	}

	// ��Ԍ��� (+ ��������) ... �����̂�
	long SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		ISpatialFilterPtr ipFilter( CLSID_SpatialFilter );
		CComBSTR bstrFieldName;	
		if( FAILED(ipFeatureClass->get_ShapeFieldName( &bstrFieldName )) )
			return -1;
		if( FAILED(ipFilter->put_GeometryField( bstrFieldName )) )
			return -1;
		if( FAILED(ipFilter->putref_Geometry( ipGeo )) )
			return -1;
		if( FAILED(ipFilter->put_SpatialRel( spatialRel )) )
			return -1;
		
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
		{
			// Where��Z�b�g
			if( FAILED(ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) )) )
				return -1;
			// �����D�揇�ʂ��w�肷��
			if( FAILED(ipFilter->put_SearchOrder( esriSearchOrderSpatial )) )
				return -1;
		}

		// �������ʂ̌��������^�[���A�ُ�Ȃ�-1�����^�[��
		long lCount = 0;
		return SUCCEEDED(ipFeatureClass->FeatureCount(ipFilter, &lCount)) ? lCount : -1;
	}
}
