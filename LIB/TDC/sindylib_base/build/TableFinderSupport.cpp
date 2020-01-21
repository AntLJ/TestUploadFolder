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
#include "TableFinderSupport.h"
#include "GlobalFunctions.h"
#include "TableContainer.h"

using namespace sindy;

CTableFinderSupport::CTableFinderSupport(void) : m_cFinder(CTableFinder())
{
}

CTableFinderSupport::~CTableFinderSupport(void)
{
}

std::list<long> CTableFinderSupport::Cursor2IDs( _ICursor* ipCursor, LPCTSTR lpcszFieldName )
{
	std::list<long> listRet;
	if( ipCursor && lpcszFieldName )
	{
		IFieldsPtr ipFields;
		ipCursor->get_Fields( &ipFields );
		long lIndex = -1;
		ipFields->FindField( CComBSTR(lpcszFieldName), &lIndex );
		if( 0 <= lIndex )
		{
			_IRowPtr ipRow;
			while( S_OK == ipCursor->NextRow( &ipRow ) )
			{
				CComVariant vaValue;
				ipRow->get_Value( lIndex, &vaValue );
				_ASSERTE( VT_I4 == vaValue.vt || VT_NULL == vaValue.vt );
				if( VT_I4 == vaValue.vt )
					listRet.push_back( vaValue.lVal );
			}
		}
	}
	listRet.sort();
	ATLTRACE(_T("åèêîÅF%d\n"), listRet.size() );

	return listRet;
}
