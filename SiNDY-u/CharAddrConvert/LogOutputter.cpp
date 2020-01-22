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
#include "LogOutputter.h"
#include <WinLib/VersionInfo.h>

namespace progress_out
{
	// メッセージ整形
	CString FormatMsg( ERROR_LEVEL kErrorLevel, LPCTSTR lpcszFormat, va_list vaList )
	{
		CString strMsg( _T("#") );
		switch( kErrorLevel )
		{
		case kError:	strMsg += _T("Error ");			break;
		case kFatal:	strMsg += _T("Fatal Error ");	break;
		case kWarning:	strMsg += _T("Warning ");		break;
		default:
			break;
		}
		if( lpcszFormat )
			strMsg.AppendFormatV( lpcszFormat, vaList );
		else
			strMsg.Append( _T("メッセージなし") );
		return strMsg;
	}

	// 進捗メッセージ出力
	void Progress( LPCTSTR lpcszFormat, ... )
	{
		va_list vaList;
		if( lpcszFormat )
			va_start( vaList, lpcszFormat );

		std::cout << FormatMsg(kInfo, lpcszFormat, vaList) << std::endl;
		std::cerr << FormatMsg(kInfo, lpcszFormat, vaList) << std::endl;
	}

	// 時間表示
	void ProgressTime( LPCTSTR lpcszMsg /* = NULL */ )
	{
		SYSTEMTIME st;
		::GetLocalTime( &st );

		CString strTime;
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

		CString strMsg;
		if( lpcszMsg )
			strMsg.AppendFormat( _T("%s %s"),lpcszMsg, strTime );
		else
			strMsg.Append( strTime );
		Progress( strMsg );
	}
}

namespace object_out
{
	CString g_strCurrentCityCode;

	// ヘッダー出力
	void LogHeader()
	{
		CVersion cVer;
		CString strHeader;
		strHeader.Format( _T("#FREESTYLELOG\n")
						  _T("#%s FILEVERSION %s PRODUCTVERSION %s\n")
						  _T("LAYER\tOBJECTID\tCITYCODE\tMSG"),
						  cVer.GetOriginalFilename(), cVer.GetFileVersion(), cVer.GetProductVersion() );
		std::cout << strHeader << std::endl;
	}

	// エラー出力
	void ObjectError( LPCTSTR lpcszLayer, long lOID, LPCTSTR lpcszFormat, ... )
	{
		va_list vaList;
		if( lpcszFormat )
			va_start( vaList, lpcszFormat );

		std::cout << lpcszLayer << "\t" << lOID << "\t" << g_strCurrentCityCode << "\t" << FormatMsg(lpcszFormat, vaList) << std::endl;
	}

	// エラー出力(IRow指定版)
	void ObjectError( _IRowPtr ipRow, LPCTSTR lpcszFormat, ... )
	{
		va_list vaList;
		if( lpcszFormat )
			va_start( vaList, lpcszFormat );

		if( ipRow )
		{
			CString strLayer;
			ITablePtr ipTable( ipRow->Table );
			if( ipTable )
			{
				IDatasetPtr ipDataset( ipTable );
				if( ipDataset ){
					BSTR bstrName = 0;
					ipDataset->get_Name(&bstrName);
					strLayer = CString(bstrName);
				}
			}
			std::cout << strLayer << "\t" << ipRow->OID << "\t" << g_strCurrentCityCode << "\t" << FormatMsg(lpcszFormat, vaList) << std::endl;
		}
	}
}
