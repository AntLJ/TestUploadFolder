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

#include "StdAfx.h"
#include "TableFinder.h"
#include "TableContainer.h"
#include "AheLayerFunctions.h"
#include "ClassFactory.h"

using namespace sindy;

IWorkspacePtr AheGetWorkspace( IUnknown* ipUnk );

CTableFinder::CTableFinder(void)
{
}

CTableFinder::CTableFinder( const CModel& cModel ) : m_cModel(cModel)
{
}

CTableFinder::~CTableFinder(void)
{
	ClearCollection();
}

CSPTableContainer CTableFinder::FindTable( LPCTSTR lpcszTableName ) const
{
	std::map<CString, CSPTableContainer>::const_iterator it = m_mapCollection.find( lpcszTableName );
	if( m_mapCollection.end() == it /*|| ( ! (ITable*)*it->second ) テーブルインスタンスがない場合も当然ありうるでしょ*/ )
	{
		// テーブルオープン
		ITablePtr ipTable( GetExistTable() );
		if( ! ipTable )
			return CSPTableContainer();

		CSPTableContainer cTable( TableClassFactory( *this, AheOpenSameOwnerTable( m_ipWorkspace, ipTable, lpcszTableName ) ) );
		m_mapCollection[lpcszTableName] = cTable;
		it = m_mapCollection.find( lpcszTableName );
	}

	return it->second;
}

ITablePtr CTableFinder::GetExistTable() const
{
	for( std::map<CString,CSPTableContainer>::const_iterator it = m_mapCollection.begin(); it != m_mapCollection.end(); ++it )
	{
		ITablePtr ipTable( (ITable*)*it->second );
		if( ipTable )
			return ipTable;
	}
	return ITablePtr();
}

void CTableFinder::InitCollection( ITable* ipTable, const CModel& model )
{
	// ワークスペースをセット
	m_ipWorkspace = AheGetWorkspace( ipTable );
	CSPTableContainer cTable( TableClassFactory( *this, ITablePtr( ipTable ) ) );
	m_mapCollection[cTable->GetTableName()] = cTable;
	m_cModel = model;
}

void CTableFinder::ClearCollection()
{
	m_mapCollection.clear();
}
