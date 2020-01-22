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
#include <sindy/workspace.h>

using namespace sindy::schema;

namespace gf
{
	// ���[�N�X�y�[�X�擾
	IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
	{
		IUnknownPtr ipUnknown;

		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( !ipName )
			return nullptr;

		HRESULT hr = ipName->Open( &ipUnknown );
		if( !ipUnknown )
			return nullptr;
		
		return ipUnknown;
	}

	// ��������
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount /* = NULL */, LPCTSTR lpszWhereClause /* = NULL */, LPCTSTR lpszSubFields /* = NULL */ )
	{
		// �N�G���t�B���^�쐬
		IQueryFilterPtr ipFilter( CLSID_QueryFilter );
			
		// Where��w�肳��Ă�����A�Z�b�g����
		if( lpszWhereClause )
			ipFilter->put_WhereClause( CComBSTR(lpszWhereClause) );

		// SubFields�w�肳��Ă�����A�Z�b�g����
		if( lpszSubFields )
			ipFilter->put_SubFields( CComBSTR(lpszSubFields) );

		// �������ʐ��擾
		if( pCount )
		{
			if( FAILED(ipTable->RowCount(ipFilter, pCount)) )
				*pCount = INVALID_VALUE;

			// �������R�[�h���킩���Ă���Ƃ��́A������Search���Ȃ�
			if( *pCount < 1 )
				return nullptr;
		}

		// ����
		_ICursorPtr ipCursor;
		if( FAILED(ipTable->Search(ipFilter, VARIANT_FALSE, &ipCursor)) && pCount )	
			*pCount = INVALID_VALUE;
		return ipCursor;
	}

	// �}�[�W
	IGeometryPtr constructUnion( const IGeometryPtr& ipGeometryCollection, const ISpatialReferencePtr& ipSpatialReference )
	{
		IPolygonPtr ipPolygon(CLSID_Polygon);
		ipPolygon->putref_SpatialReference( ipSpatialReference );		
		if( FAILED( ITopologicalOperatorPtr(ipPolygon)->ConstructUnion( IEnumGeometryPtr(ipGeometryCollection) ) ) )
		{
			return nullptr;
		}

		return ipPolygon;
	}	

	// �|���S���̍���
	IGeometryPtr difference( const IGeometryPtr& ipMeshGeometry, const IGeometryPtr& ipMergeGeometry )
	{
		IGeometryPtr ipDifferentGeometry;
		if( FAILED(((ITopologicalOperatorPtr)ipMeshGeometry)->Difference( ipMergeGeometry, &ipDifferentGeometry )) )
		{
			return nullptr;
		}

		return ipDifferentGeometry;
	}

	// Touches����
	bool isTouches( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		IRelationalOperatorPtr ipRel( ipBaseGeometry );
		if( S_OK != ipRel->Touches( ipOtherGeometry, &vb ) ) return false;
		return !( vb == VARIANT_FALSE );
	}

	// Contains����
	bool IsContains( const IGeometryPtr& ipBaseGeometry, const IGeometryPtr& ipOtherGeometry )
	{
		VARIANT_BOOL vb = VARIANT_FALSE;
		IRelationalOperatorPtr ipRel( ipBaseGeometry );
		if( S_OK != ipRel->Contains( ipOtherGeometry, &vb ) ) return false;
		return !( vb == VARIANT_FALSE );
	}

	// �t�B�[���h�}�b�v�쐬
	FIELDSMAP createFieldMap( const ITablePtr& ipTable )
	{
		FIELDSMAP fieldMap;

		IFieldsPtr ipFields;
		if( S_OK !=ipTable->get_Fields( &ipFields ) || !ipFields ) return fieldMap;

		long lFieldCount(0L);
		ipFields->get_FieldCount( &lFieldCount );

		for( long index = 0; index < lFieldCount; ++index ){
		
			IFieldPtr ipField;
			if( S_OK != ipFields->get_Field( index, &ipField ) ) continue;

			CComBSTR bstr;
			if( S_OK != ipField->get_Name( &bstr ) ) continue;

			fieldMap.insert( pair<CString, long>(CString(bstr), index) );
		}
		return fieldMap;
	}

	// �����l�擾
	CComVariant getValue( const IFeaturePtr &ipFeature, long fieldIdx) 
	{
		CComVariant vaValue;
		if( fieldIdx == -1 ) return vaValue;
		ipFeature->get_Value( fieldIdx, &vaValue );

		return vaValue;
	}
} // gf
