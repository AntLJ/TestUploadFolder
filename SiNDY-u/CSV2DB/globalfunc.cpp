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
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_base/TableContainer.h>

using namespace sindy;
using namespace sindy::schema;

const CString NULL_VALUE = _T("<null>");         // NULL値指定
const CString WORK_VERSION = _T("ETC_CSV2DB_");  // 実際に編集する子バージョン名


ITablePtr TryOpenTable( const IWorkspacePtr& work, const CString& table )
{
	RUNLOGGER.Info( uh::str_util::format( _T("Open Table... : %s"), table ), true );

	CWorkspace cWork(work);
	cWork.m_bTrace = false;
	ITablePtr ipTable = cWork.OpenSameOwnersTable( table );
	if( !ipTable )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("Open failure: %s"), table) );
	}
	
	RUNLOGGER.Info( _T("OK.\n"), true );
	return ipTable;
}

IWorkspacePtr TryConnect( const CString& conn )
{
	RUNLOGGER.Info( uh::str_util::format( _T("Connect Workspace... : %s"), conn ), true );

	CWorkspace work;
	work.m_bTrace = false;
	if( !work.Connect( conn ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("Connect failure: %s"), conn) );
	}
	
	RUNLOGGER.Info( _T("OK.\n"), true );
	return (IWorkspace*)work;
}

IWorkspacePtr TryCreateVersion( const CTableContainer& table )
{
	// バージョン非対応だったらそのまま返す
	IVersionedObjectPtr versioned((ITable*)table);
	if(	!(versioned && versioned->GetIsRegisteredAsVersioned()) )
	{
		RUNLOGGER.Info( uh::str_util::format( _T("Non Versioned : %s"), table.GetTableName()), true );
		return table.GetWorkspace();
	}

	CTime cTime = CTime::GetCurrentTime();
	CString verName = WORK_VERSION + cTime.Format( _T("%Y%m%d%H%M%S") );

	RUNLOGGER.Info( uh::str_util::format( _T("Create Version : %s"), verName), true );

	IVersionPtr child;
	if( FAILED( ((IVersionPtr)table.GetWorkspace())->CreateVersion( CComBSTR( verName ), &child ) ) )
	{
		throw std::runtime_error( uh::str_util::formatA( _T(" Create failure: %s"), verName) );
	}

	RUNLOGGER.Info( _T("OK.\n"), true );
	return child;
}
