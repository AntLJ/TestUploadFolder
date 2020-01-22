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
#include "common.h"
#include <sindy/workspace.h>
#include <WKF_LIB/libwkf.h>

namespace common_proc
{
	bool getWorkspace( const CString& strConnect, IWorkspacePtr& ipWorkspace )
	{
		IWorkspaceName2Ptr ipWorkspaceName = sindy::create_workspace_name( strConnect );
		if( !ipWorkspaceName ) return false;
		IUnknownPtr ipUnknown;
		if( FAILED( ( (INamePtr)ipWorkspaceName )->Open( &ipUnknown ) ) ) return false;
		ipWorkspace = IWorkspacePtr( ipUnknown );
		return true;
	}

	bool getFeatureClass( const CString& strName, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass )
	{
		if( FAILED( ( (IFeatureWorkspacePtr)ipWorkspace )->OpenFeatureClass( CComBSTR( strName ), &ipFeatureClass ) ) ) return false;
		return true;
	}

	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable )
	{
		if( FAILED( ( (IFeatureWorkspacePtr)ipWorkspace )->OpenTable( CComBSTR( strName ), &ipTable ) ) ) return false;
		return true;
	}

	bool getFieldIndexes( const ITablePtr& ipTable, FieldIndexes& mapIndexes )
	{
		IFieldsPtr ipFields;
		long lCount = 0;
		if( FAILED( ipTable->get_Fields( &ipFields ) ) ) return false;
		if( FAILED( ipFields->get_FieldCount( &lCount ) ) ) return false;

		for( long i = 0; i < lCount; ++i )
		{
			IFieldPtr ipField;
			if( FAILED( ipFields->get_Field( i, &ipField ) ) ) return false;
			CComBSTR bstrName;
			if( FAILED( ipField->get_Name( &bstrName ) ) ) return false;
			mapIndexes.insert( FieldIndexes::value_type( CString( bstrName ), i ) );
		}

		return true;
	}

	bool getField( const ITablePtr& ipTable, const CString& strFieldName, IFieldPtr& ipField )
	{
		IFieldsPtr ipFields;
		long lIndex = 0;
		if( FAILED( ipTable->get_Fields( &ipFields ) ) ) return false;
		if( FAILED( ipFields->FindField( CComBSTR( strFieldName ), &lIndex ) ) ) return false;
		if( FAILED( ipFields->get_Field( lIndex, &ipField ) ) ) return false;

		return true;
	}

	bool getValue( const _IRowPtr& ipRow, const CString& strFieldName, FieldIndexes& mapIndexes, CComVariant& varValue )
	{
		const auto itr = mapIndexes.find( strFieldName );
		if( mapIndexes.end() == itr ) return false;
		if( FAILED( ipRow->get_Value( (*itr).second, &varValue ) ) ) return false;

		return true;
	}

	bool getCodedValues( const IFieldPtr& ipField, std::map< long, CString >& mapValues )
	{
		IDomainPtr ipDomain;
		esriDomainType type;
		long lCount = 0;
		if( FAILED( ipField->get_Domain( &ipDomain ) ) ) return false;
		if( FAILED( ipDomain->get_Type( &type ) ) ) return false;
		if( esriDTCodedValue != type ) return false;
		if( FAILED( ( ICodedValueDomainPtr( ipDomain ) )->get_CodeCount( &lCount ) ) ) return false;
		for( long i = 0; i < lCount; ++i )
		{
			CComVariant value;
			CComBSTR name;
			if( FAILED( ( ICodedValueDomainPtr( ipDomain ) )->get_Value( i, &value ) ) ) return false;
			if( FAILED( ( ICodedValueDomainPtr( ipDomain ) )->get_Name( i, &name ) ) ) return false;
			if( VT_I4 != value.vt ) return false;
			mapValues.insert( std::pair< long, CString >( value.lVal, CString( name ) ) );
		}

		return true;
	}

	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::map< long, CString >& mapValues )
	{
		IFieldPtr ipField;
		if( !getField( ipTable, strFieldName, ipField ) ) return false;
		if( !getCodedValues( ipField, mapValues ) ) return false;

		return true;
	}

	bool searchByWhereClause( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const CString& strWhere, const CString& strPostfix )
	{
		// フィルタ作成
		IQueryFilterPtr ipQueryFilter( CLSID_QueryFilter );
		if( FAILED( ipQueryFilter->put_WhereClause( CComBSTR( strWhere ) ) ) ) return false;

		// カウント
		if( FAILED( ipFeatureClass->FeatureCount( ipQueryFilter, &lCount ) ) ) return false;

		// PostfixClause設定（FeatureCountに使用すると失敗するようなのでここで設定）
		if( FAILED( ( (IQueryFilterDefinitionPtr)ipQueryFilter )->put_PostfixClause( CComBSTR( strPostfix ) ) ) ) return false;

		// 検索
		if( FAILED( ipFeatureClass->Search( ipQueryFilter, VARIANT_FALSE, &ipFeatureCursor ) ) ) return false;

		return true;
	}

	bool searchByGeometry( const IFeatureClassPtr& ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long& lCount, const IGeometryPtr& ipGeometry, esriSpatialRelEnum spRel, const CString& strPostfix )
	{
		// フィルタ作成
		ISpatialFilterPtr ipSpatialFilter( CLSID_SpatialFilter );
		if( FAILED( ipSpatialFilter->putref_Geometry( ipGeometry ) ) ) return false;
		if( FAILED( ipSpatialFilter->put_SpatialRel( spRel ) ) ) return false;

		// カウント
		if( FAILED( ipFeatureClass->FeatureCount( ipSpatialFilter, &lCount ) ) ) return false;

		// PostfixClause設定（FeatureCountに使用すると失敗するようなのでここで設定）
		if( FAILED( ( (IQueryFilterDefinitionPtr)ipSpatialFilter )->put_PostfixClause( CComBSTR( strPostfix ) ) ) ) return false;

		// 検索
		if( FAILED( ipFeatureClass->Search( ipSpatialFilter, VARIANT_FALSE, &ipFeatureCursor ) ) ) return false;

		return true;
	}

	std::string convEUC2SJIS( const std::string& str )
	{
		char szRet[4096] = { 0 };
		wkfConvertKanjiCodeOfString( KC_EUC, const_cast< char * >( str.c_str() ), KC_SJIS, szRet, sizeof( szRet ) );
		return szRet;
	}

	std::string convSJIS2EUC( const std::string& str )
	{
		char szRet[4096] = { 0 };
		wkfConvertKanjiCodeOfString( KC_SJIS, const_cast< char * >( str.c_str() ), KC_EUC, szRet, sizeof( szRet ) );
		return szRet;
	}

} // namespace common_proc
