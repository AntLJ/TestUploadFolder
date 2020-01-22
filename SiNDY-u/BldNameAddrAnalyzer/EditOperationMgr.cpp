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
#include "logger.h"
#include "EditOperationMgr.h"
#include <TDC/sindylib_core/Workspace.h>
#include <TDC/sindylib_core/ErrorCode.h>

using namespace sindy;
using namespace sindy::errorcode;
using namespace uh::str_util;


// デストラクタ
EditOperationMgr::~EditOperationMgr(void)
{
	// ここで編集開始されているのはおかしい
	if( !IsBeingEdited() )
		return;

	if( sindyErr_NoErr == m_work.StopEditing( false ) )
	{
		CString msg = format( _T("Discard Edit: %s"), m_work.GetNameString()->GetConnectString() );
		RUNLOGGER.Error( uh::tstring(msg) );
	}
}

// 編集開始
void EditOperationMgr::Start()
{
	if( sindyErr_NoErr != m_work.StartEditing( false ) )
	{
		throw std::runtime_error( 
			formatA( _T("Start Editing failure: %s"), m_work.GetNameString()->GetConnectString() ));
	}

	if( sindyErr_NoErr != m_work.StartEditOperation() )
	{
		throw std::runtime_error( 
			formatA( _T("Start Operation failure: %s"), m_work.GetNameString()->GetConnectString() ) );
	}
}

// 編集終了
void EditOperationMgr::Stop()
{
	if( !IsBeingEdited() )
		return;

	if( sindyErr_NoErr != m_work.StopEditOperation() )
	{
		m_work.StopEditing( false );
		throw std::runtime_error( 
			formatA( _T("Stop Operation failure: %s"), m_work.GetNameString()->GetConnectString() ) );
	}

	if( sindyErr_NoErr != m_work.StopEditing( true ) )
	{
		throw std::runtime_error( 
			formatA( _T("Stop Editing failure: %s"), m_work.GetNameString()->GetConnectString() ) );
	}
}

// 編集破棄
void EditOperationMgr::Abort()
{
	IWorkspaceEdit2Ptr we(m_work);
	if( ! we->IsInEditOperation )
		return;

	if( sindyErr_NoErr == m_work.AbortEditOperation() )
	{
		CString msg = format( _T("Abort Operation: %s"), m_work.GetNameString()->GetConnectString() );
		RUNLOGGER.Error( uh::tstring(msg) );
	}

	if( sindyErr_NoErr == m_work.StopEditing( false ) )
	{
		CString msg = format( _T("Discard Edit: %s"), m_work.GetNameString()->GetConnectString() );
		RUNLOGGER.Error( uh::tstring(msg) );
	}
}

bool EditOperationMgr::IsBeingEdited()
{
	return (m_work && m_work.IsBeingEdited());
}
