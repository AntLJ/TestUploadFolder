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
#include "Utility.h"
#include "Logger.h"

#include <TDC/useful_headers/str_util.h>

void writeMessage(const CString& msg, Utility::RunLogMsgStatus runMsgStatus, bool isWriteStdOut)
{
	using namespace Utility;

	switch ( runMsgStatus )
	{
	case RunLogMsgStatus::Normal :
		RUNLOGGER.Log((uh::tstring)msg);
		break;
	case RunLogMsgStatus::Info :
		RUNLOGGER.Info((uh::tstring)msg);
		break;
	case RunLogMsgStatus::Error :
		RUNLOGGER.Error((uh::tstring)msg);
		break;
	case RunLogMsgStatus::Fatal :
		RUNLOGGER.Fatal((uh::tstring)msg);
		break;
	default:
		break;
	}

	if( isWriteStdOut )
		std::cout << CT2A(msg);
}

void writeErrMsg(
				 const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const std::vector<CString>& valueList)
{
	// 既定値をリスト化
	std::vector<CString> msgList = boost::assign::list_of
		( _T("0") )
		( layerName )
		( uh::str_util::ToString(oid) )
		( _T("") )
		( _T("") )
		( errLevel )
		( _T("") )
		( msg );

	// 任意の値のリストをタブ区切りで文字列化し、既定値のリストの追加
	if( !valueList.empty() )
	{
		auto joinValueList = uh::str_util::join(valueList, _T("\t"));
		msgList.push_back(joinValueList);
	}

	// エラーログ出力
	ERRLOGGER.Log(_T("%s\n"), uh::str_util::join(msgList, _T("\t")));
}

void writeErrMsg(
				 const CString& layerName, 
				 long oid,
				 const CString& errLevel, 
				 const CString& msg,
				 const CString& value)
{
	std::vector<CString> valueList = boost::assign::list_of
			( value );

	writeErrMsg(layerName, oid, errLevel, msg, valueList);
}