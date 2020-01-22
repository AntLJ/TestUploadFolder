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
#include "RunLogger.h"
#include "WinLib/VersionInfo.h"
#include <WinLib/winutil.h>
#include <sindy/libschema.h>

using namespace std;
using namespace uh::str_util;
using namespace uh::variant_util;
using namespace meshtype;

void CRunLogger::header( const Arguments& args )
{
	CString toolInfo;
	CVersion ver;
	toolInfo += ver.GetOriginalFilename();
	toolInfo += _T(" FV:") + ver.GetFileVersion();
	toolInfo += _T(" PV:") + ver.GetProductVersion() + _T("\n");
	write( toolInfo );
	
	write( format( _T("FIXバージョンDB:%s"), args.m_BaseDB.c_str() ) );
	write( format( _T("編集バージョンDB:%s"), args.m_CompDB.c_str() ) );
	write( format( _T("実行ログ:%s"), args.m_Runlog.c_str() ) );
	write( format( _T("エラーログ:%s"), args.m_Errlog.c_str() ) );
	write(
		format(_T("指定更新日:%s"), (!isNullOrEmpty(args.m_ModDate) ? CString(args.m_ModDate).GetString() : _T("なし")))
		);
}

void CRunLogger::writeParseMeshResult( const MeshType& type, const map<int, CString>& mapInvalidRow )
{
	write( format( _T("処理メッシュ単位:%s\n"), getStrMeshType(type).c_str() ) );

	if( mapInvalidRow.empty() )
		return;

	// メッシュリストに不正な文字が含まれていたり、メッシュが混在している場合はログに残す
	for( const auto& row : mapInvalidRow )
	{
		CString msg = _T("ERROR : ");

		if( kInvalid == type )
			msg += _T("メッシュリストに不正があります");
		else
			msg += _T("メッシュコードが混在しています");

		msg += format(_T("(%d行目:\""), row.first) + row.second + _T("\")");
		write( msg );
	}
}

void CRunLogger::writeExeTime( bool isStart )
{
	write( ( isStart ? _T("開始時刻: ") : _T("\n終了時刻: ") ) + winutil::GetTime() + _T("\n") );
}

void CRunLogger::writeErrMsg( const CString& msg )
{
	writeLogAndConsole( _T("ERROR : ") + msg );
}

void CRunLogger::writeLogAndConsole( const CString& msg )
{
	write( msg );
	wcerr << msg.GetString() << endl;
}
