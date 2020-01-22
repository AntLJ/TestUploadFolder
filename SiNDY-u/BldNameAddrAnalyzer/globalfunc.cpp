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
#include "globalfunc.h"
#include "Logger.h"
#include <TDC/sindylib_base/TableContainer.h>
#include <TDC/sindylib_core/Workspace.h>
#include <WinLib/ADOBase.h>
#include <atlcomtime.h>

using namespace sindy;
using namespace sindy::schema;

CWorkspace TryConnectWorkspace( const CString& conn )
{
	RUNLOGGER.Info( uh::str_util::format( _T("Connect Workspace... : %s"), conn ), true );

	CWorkspace work;
	work.m_bTrace = false;
	if( !work.Connect( conn ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("Connect failure: %s"), conn) );
	}
	
	RUNLOGGER.Info( _T("OK.\n"), true );
	return work;
}

ITablePtr TryOpenTable( const CString& work, const CString& table )
{
	CWorkspace cWork(TryConnectWorkspace(work));

	RUNLOGGER.Info( uh::str_util::format( _T("Open Table... : %s"), table ), true );

	ITablePtr ipTable = cWork.OpenSameOwnersTable( table );
	if( !ipTable )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("Open failure: %s"), table) );
	}
	
	RUNLOGGER.Info( _T("OK.\n"), true );
	return ipTable;
}

void connectAdo( const CString& svr, const CString& usr, CADOBase& ado )
{
	RUNLOGGER.Info( uh::str_util::format( _T( "住所DB接続...： %s@%s" ), usr, svr ), true );

	CString con = uh::str_util::format( 
		_T(" Driver={Microsoft ODBC for Oracle}; Server=%s; UID=%s; PWD=%s; "), 
			svr, usr, usr );

	if( !ado.connect_by_string( con ) )
	{
		std::string msg = " サーバ接続エラー： " + CStringA(con);
		throw std::runtime_error( msg );
	}

	RUNLOGGER.Info( _T( "OK.\n" ), true );
}

IPointPtr createPoint( const WKSPoint point, const ISpatialReferencePtr spRef )
{
	IPointPtr ipPoint( CLSID_Point );
	ipPoint->putref_SpatialReference( spRef );
	ipPoint->PutCoords( point.X, point.Y );
	return ipPoint;
}

CString getDateWhere( IWorkspacePtr work, const CString& key, const CComVariant& val )
{
	// 時間は切り捨てる
	COleDateTime date( val );
	COleDateTime tomorrow( date + COleDateTimeSpan( 1, 0, 0, 0) );

	// 日付けの前後は後で置換するように適当に（+-）くくっておく
	CString dateWhere = uh::str_util::format( 
		_T(" %s >= +%s- AND %s < +%s-"), 
			key, date.Format( _T("%Y/%m/%d") ),
			key, tomorrow.Format( _T("%Y/%m/%d") ) );

	CWorkspace cWork(work);
	switch( cWork.GetType() )
	{
	case esriRemoteDatabaseWorkspace:
		{
			// TODO: ポスグレは知らない
			dateWhere.Replace( _T("+"), _T("to_date('") );
			dateWhere.Replace( _T("-"), _T("')") );
		} break;

	case esriLocalDatabaseWorkspace:
		{
			if( cWork.IsDirectory() ) // FGDB
			{
				dateWhere.Replace( _T("+"), _T("date '") );
				dateWhere.Replace( _T("-"), _T("'") );
			}
			else // PGDB
			{
				dateWhere.Replace( _T("+"), _T("#") );
				dateWhere.Replace( _T("-"), _T("#") );
			}
		} break;

	case esriFileSystemWorkspace:
	default:
		break;
	}

	return dateWhere;
}
