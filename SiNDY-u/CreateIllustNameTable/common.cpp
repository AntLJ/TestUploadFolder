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

	bool getTable( const CString& strName, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable )
	{
		if( FAILED( ( (IFeatureWorkspacePtr)ipWorkspace )->OpenTable( CComBSTR( strName ), &ipTable ) ) ) return false;
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

	bool getCodedValues( const IFieldPtr& ipField, std::set< long, std::less<long> >& setValues )
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
			if( FAILED( ( ICodedValueDomainPtr( ipDomain ) )->get_Value( i, &value ) ) ) return false;
			if( VT_I4 != value.vt ) return false;
			setValues.insert( value.lVal );
		}

		return true;
	}

	bool getCodedValues( const ITablePtr& ipTable, const CString& strFieldName, std::set< long, std::less<long> >& setValues )
	{
		IFieldPtr ipField;
		if( !getField( ipTable, strFieldName, ipField ) ) return false;
		if( !getCodedValues( ipField, setValues ) ) return false;

		return true;
	}
}
