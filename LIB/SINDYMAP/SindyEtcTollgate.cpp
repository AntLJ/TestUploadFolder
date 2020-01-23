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

// SindyEtcTollgate.cpp: SindyEtcTollgate クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "SindyEtcTollgate.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

SindyEtcTollgate::SindyEtcTollgate( SindyDBConnection *cDBConnection ) :
	SindyTable( cDBConnection, sindy::layer::ETC_TOLLGATE )
{

}

SindyEtcTollgate::~SindyEtcTollgate()
{

}


HRESULT SindyEtcTollgate::Load()
{
	HRESULT hr;
	const char *func = "SindyEtcTollgate::Load";

	if( !is_open() ) return GDBERROR2( E_ACCESSDENIED, func );

	_ICursorPtr ipCursor;
	if( FAILED( hr = GDBSearchByWhereClause( (ITablePtr)ipDataset, std::string( "" ), &ipCursor ) ) )
		return hr;

	_IRowPtr ipRow;
	while( ipCursor->NextRow( &ipRow ) == S_OK && ipRow )
	{
		long aOID;
		if( FAILED( hr = ipRow->get_OID( &aOID ) ) )
			return GDBERROR( hr, func );
		insert( GDBOBJSET( aOID, ipRow ) );

		CComVariant vaTollgateID;
		if( FAILED( hr = GDBGetValue( ipRow, sindy::etc_tollgate::TOLLGATE_ID, &vaTollgateID ) ) )
			return GDBERROR( hr, func );
		CString sTollgateId = vaTollgateID.bstrVal;

		mTollgateIDList.insert( GDBOBJSET( _ttoi(sTollgateId), ipRow ) );
	}

	return S_OK;
}

HRESULT SindyEtcTollgate::SearchByTollgatetID( const CString& cTollgteID, _IRow **ipRow )
{
	GDBOBJITER aIter = mTollgateIDList.find( _ttoi(cTollgteID) );

	if( aIter == mTollgateIDList.end() )
	{
		( *ipRow ) = NULL;
		return S_FALSE;
	}

	( *ipRow ) = aIter->second;
	( *ipRow )->AddRef();

	return S_OK;
}

