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
#include "crd_cnv/coord_converter.h"
#include "AccessStoreCache.h"
#include <sindy/schema.h>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

using namespace access_store_cache;

CASCache::CASCache(void)
{
}

CASCache::~CASCache(void)
{
}

void CASCache::addRow( long cAccessPointID )
{
	CASRow aRow;
	aRow.m_IsAdded = true;
	long aTempOID = getTempObjectID();

	using namespace sindy::schema::access_store;
	aRow.m_Fields = boost::assign::map_list_of
		( tstring( _T("OBJECTID") ),	CASField( aTempOID ) )
		( tstring( kAccessPointID ),	CASField( cAccessPointID ) )
		( tstring( kMultipleStore ),	CASField() )
		( tstring( kStartHour ),		CASField() )
		( tstring( kStartMin ),			CASField() )
		( tstring( kEndHour ),			CASField() )
		( tstring( kEndMin ),			CASField() )
		( tstring( kSunday ),			CASField() )
		( tstring( kMonday ),			CASField() )
		( tstring( kTuesday ),			CASField() )
		( tstring( kWednesday ),		CASField() )
		( tstring( kThursday ),			CASField() )
		( tstring( kFriday ),			CASField() )
		( tstring( kSaturday ),			CASField() )
		( tstring( kBon ),				CASField() )
		( tstring( kYearEnd ),			CASField() )
		( tstring( kNewYear ),			CASField() )
		( tstring( kGoldenWeek ),		CASField() )
		( tstring( kHoliday ),			CASField() )
		( tstring( kStoreType ),		CASField() )
	;

	m_Rows.insert( std::make_pair( aTempOID, aRow ) );
}

void CASCache::addRow( _IRow* cpRow )
{
	ATLASSERT( cpRow );
	if (! cpRow) { return; }

	using namespace sindy::schema::access_store;
	long aOID = cpRow->OID;
	CASRow aRow;
	aRow.m_pRow = cpRow;
	aRow.m_Fields = boost::assign::map_list_of
		( tstring( _T("OBJECTID") ),	CASField( aOID ) )
		( tstring( kAccessPointID ),	CASField( cpRow->GetValue( cpRow->Fields->_FindField( kAccessPointID ) ) ) )
		( tstring( kMultipleStore ),	CASField( cpRow->GetValue( cpRow->Fields->_FindField( kMultipleStore ) ) ) )
		( tstring( kStartHour ),		CASField( cpRow->GetValue( cpRow->Fields->_FindField( kStartHour ) ) ) )
		( tstring( kStartMin ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kStartMin ) ) ) )
		( tstring( kEndHour ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kEndHour ) ) ) )
		( tstring( kEndMin ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kEndMin ) ) ) )
		( tstring( kSunday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kSunday ) ) ) )
		( tstring( kMonday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kMonday ) ) ) )
		( tstring( kTuesday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kTuesday ) ) ) )
		( tstring( kWednesday ),		CASField( cpRow->GetValue( cpRow->Fields->_FindField( kWednesday ) ) ) )
		( tstring( kThursday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kThursday ) ) ) )
		( tstring( kFriday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kFriday ) ) ) )
		( tstring( kSaturday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kSaturday ) ) ) )
		( tstring( kBon ),				CASField( cpRow->GetValue( cpRow->Fields->_FindField( kBon ) ) ) )
		( tstring( kYearEnd ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kYearEnd ) ) ) )
		( tstring( kNewYear ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kNewYear ) ) ) )
		( tstring( kGoldenWeek ),		CASField( cpRow->GetValue( cpRow->Fields->_FindField( kGoldenWeek ) ) ) )
		( tstring( kHoliday ),			CASField( cpRow->GetValue( cpRow->Fields->_FindField( kHoliday ) ) ) )
		( tstring( kStoreType ),		CASField( cpRow->GetValue( cpRow->Fields->_FindField( kStoreType ) ) ) )
	;

	m_Rows.insert( std::make_pair( cpRow->OID, aRow ) );
}

void CASCache::updateField( long cObjectID, tstring cFieldName, long cValue )
{
	// 対象フィールドを探す
	CASField* pField = getField( cObjectID, cFieldName );
	if (! pField) { return; }	// 見つからなければ何もせずに終了

	// フィールドの値を更新
	pField->m_NewValue = cValue;
}

void CASCache::changeDeleteFlag( long cObjectID )
{
	CASRow* pRow = getRow( cObjectID );
	if (! pRow) { return; }		// 見つからなければ何もせずに終了
	pRow->m_IsDeleted = ! pRow->m_IsDeleted;
}

CASRow* CASCache::getRow( long cObjectID )
{
	CASRows::iterator itRow = m_Rows.find( cObjectID );
	return itRow == m_Rows.end() ? NULL : &itRow->second;
}

CASField* CASCache::getField( long cObjectID, tstring cFieldName )
{
	CASRow* pRow = getRow( cObjectID );
	if (! pRow) { return NULL; }

	CASFields::iterator itField = pRow->m_Fields.find( cFieldName );
	return itField == pRow->m_Fields.end() ? NULL : &itField->second;
}

long CASCache::getTempObjectID() const
{
	if (m_Rows.size() == 0)
	{
		return -1;
	}
	long aMinOID = m_Rows.begin()->first;
	return 0 <= aMinOID ? -1 : aMinOID - 1;
}

const CSortedASRows* CASCache::sortRows()
{
	m_SortedRows.clear();
	try
	{
		BOOST_FOREACH (const CASRows::value_type& rElement, m_Rows)
		{
			long aOID = rElement.first;
			const CASRow& rRow = rElement.second;
			long aMasterCode, aSundayFlag;

			using namespace sindy::schema;
			aMasterCode = rRow.validField( access_store::kMultipleStore ).m_NewValue;
			aSundayFlag = rRow.validField( access_store::kSunday ).m_NewValue;

			m_SortedRows.insert( std::make_pair( 
				boost::make_tuple( aMasterCode, aSundayFlag, aOID ), rRow
			));
		}
	}
	// CASRow::validField()失敗時に送出される例外をcatch。
	// その場合はNULLポインタを返し、呼び出し元に異常を伝える。
	catch (...)
	{
		return NULL;
	}

	return &m_SortedRows;
}

const CSortedASRows& CASCache::sortedRows()
{
	return m_SortedRows;
}
