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

CWorkspace tryConnectWorkspace( const CString& conn )
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

ITablePtr tryOpenTable( const CString& work, const CString& table )
{
	CWorkspace cWork(tryConnectWorkspace(work));

	RUNLOGGER.Info( uh::str_util::format( _T("Open Table... : %s"), table ), true );

	ITablePtr ipTable = cWork.OpenSameOwnersTable( table );
	if( !ipTable )
	{
		throw std::runtime_error( uh::str_util::formatA( _T("Open failure: %s"), table) );
	}
	
	RUNLOGGER.Info( _T("OK.\n"), true );
	return ipTable;
}
