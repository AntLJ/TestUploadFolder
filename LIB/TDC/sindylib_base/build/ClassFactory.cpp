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

#include "ClassFactory.h"
#include "RowContainer.h"
#include "TableContainer.h"

CString AheGetFeatureClassName( IUnknownPtr ipUnk ); // AheLayerFunctions.h

using namespace sindy;

CSPRowContainer _RowClassFactory( const CTableContainer& cTable, _IRowPtr& ipRow )
{
	return CSPRowContainer( new CRowContainer( ipRow, cTable.GetFinder(), cTable.GetTableType(), ( NULL != ipRow ) ? false : true, cTable.GetFieldMap(), cTable.GetNameString() ) );
}

CSPTableContainer _TableClassFactory( const CTableFinder& cFinder, ITablePtr& ipTable )
{
	CSPTableContainer cTable; // –ß‚è’l
	cTable.reset( new CTableContainer( ipTable, cFinder ) );

	return cTable;
}

CSPTableContainer _FastTableClassFactory( const CTableFinder& cFinder, LPCTSTR lpcszTableName )
{
	CSPTableContainer cTable; // –ß‚è’l
	cTable.reset( new CTableContainer( (ITable*)NULL, cFinder ) );
	
	CSPTableContainer cFindTable( cFinder.FindTable( lpcszTableName ) );
	CSPFieldMap cFields;
	CSPTableNameString cNames;
	if( (ITable*)*cFindTable )
	{
		cFields = cFindTable->GetFieldMap();
		cNames = cFindTable->GetNameString();
		cTable->SetObject( (ITable*)*cFindTable, cFields, cNames );
	}

	return cTable;
}

RowClassFactoryFunc RowClassFactory = _RowClassFactory;
TableClassFactoryFunc TableClassFactory = _TableClassFactory;
FastTableClassFactoryFunc FastTableClassFactory = _FastTableClassFactory;
