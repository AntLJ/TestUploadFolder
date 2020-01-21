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
#include "CodeConvTable.h"

// 変換テーブルスキーマ
namespace cv_schema
{
	namespace cv_common
	{
		const static TCHAR kAddrCode[]		= _T("AddrCode");
		const static TCHAR kRegistDate[]	= _T("RegistDate");
	}

	namespace pnt_conv
	{
		const static TCHAR kTableName[]	= _T("Pnt_CodeConv");
		const static TCHAR kPntCode[]	= _T("PntCode");
		const static TCHAR kPntName[]	= _T("PntName");
		using namespace ::cv_schema::cv_common;		
	}

	namespace gou_conv
	{
		const static TCHAR kTableName[]	= _T("Gou_CodeConv");
		const static TCHAR kGouCode[]	= _T("GouCode");
		const static TCHAR kGouName[]	= _T("GouName");
		using namespace ::cv_schema::cv_common;
	}
}

using namespace ::cv_schema;

// データベースオープン
bool CCodeConvTable::OpenDB( LPCTSTR lpcszDBProp )
{
	// ADO使用
	try
	{
		_ConnectionPtr ipConn( _uuidof(Connection) );

		CString strCon;
		strCon.Format( _T("Driver={Microsoft Access Driver (*.mdb)}; DBQ=%s;"), lpcszDBProp );
		if( SUCCEEDED(ipConn->Open(_bstr_t(strCon), _T(""), _T(""), adConnectUnspecified)) )
		{
			// 接続成功したら、変換前にコピーを作成する
			CString strPath( lpcszDBProp );
			int nPos = strPath.ReverseFind( '.' );
			if( nPos != -1 )
			{
				SYSTEMTIME st;
				::GetLocalTime( &st );
				CString strBkupFile;
				strBkupFile.Format( _T("%s_%04d%02d%02d.mdb"), strPath.Left(nPos), st.wYear, st.wMonth, st.wDay );
				if( CopyFile( lpcszDBProp, strBkupFile, FALSE ) )
				{
					m_cAdoHelper.attach( ipConn );
					return true;
				}
			}

			return progress_out::ProgressError<bool>( false, kError, "変換テーブルのバックアップに失敗(拡張子なし？) : %s", lpcszDBProp );
		}
		else
			return false;
	}
	catch( _com_error& e )
	{
		return progress_out::ProgressError<bool>( false, kError, "変換テーブルオープン時にCOMエラー発生[%s]", COLE2CT(e.Description()) );
	}
}

// 地番コード取得
CString CCodeConvTable::GetPntCode( const CString& strAddrCode, const CString& strPntName, bool bSave /* = true */ )
{
	CADOCursor cCur( m_cAdoHelper.search( "select count(*) from %s where %s='%s' and %s='%s'", 
										  pnt_conv::kTableName, pnt_conv::kAddrCode, strAddrCode, pnt_conv::kPntName, strPntName ) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strPntCode;
		switch( lCount )
		{
		case 0:	// 件数なし(新規コード発行)
			{
				// 該当11桁コードの最後に割り当てられた地番コードを取得
				CADOCursor cCur2( m_cAdoHelper.search( "select max(%s) from %s where %s='%s'", 
													   pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, strAddrCode ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					if( VT_BSTR == vaValue.vt )
					{
						// 取得したコード+1を新コードとして割り当て
						CString strMaxPntCode = CString(vaValue.bstrVal);						
						TCHAR szAlpha = strMaxPntCode.GetAt(0);
						long lSeq = _tstol(strMaxPntCode.Mid(1)) + 1;
						if( lSeq > 9999 )
						{
							++szAlpha;
							lSeq = 0;
							if( 'G' == szAlpha )
								return progress_out::ProgressError<CString>( _T(""), kFatal, "これ以上地番コードの割り当てができません : %s", strAddrCode );
						}
						strPntCode.Format( _T("%c%04d"), szAlpha, lSeq );
					}
					else
						strPntCode = _T("A0000");
					
					return (!bSave || InsertCode('p', strAddrCode, strPntName, strPntCode))? strPntCode : _T("");	// 保存するならテーブルに追加
				}
				else
					return progress_out::ProgressError<CString>( _T(""), kError, "地番コードの取得に失敗[%s]", cCur2.GetErrorMessage() );
			}
			break;
		case 1:	// 既存コード
			{
				CADOCursor cCur2( m_cAdoHelper.search( "select %s from %s where %s='%s' and %s='%s'", 
													   pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, strAddrCode, 
													   pnt_conv::kPntName, strPntName ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strPntCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
					return progress_out::ProgressError<CString>( _T(""), kError, "地番コードの取得に失敗[%s]", cCur2.GetErrorMessage() );
			}
			break;
		default:
			return progress_out::ProgressError<CString>( _T(""), kFatal, "同一キーに２つのコードが割り当てられている : %s, %s", strAddrCode, strPntName );
		}
		return strPntCode;
	}
	else
		return progress_out::ProgressError<CString>( _T(""), kError, "地番コードの取得に失敗[%s]", cCur.GetErrorMessage() );
}

// 住居番号コード取得
CString CCodeConvTable::GetGouCode( const CString& strAddrCode, const CString& strGouName )
{
	CADOCursor cCur( m_cAdoHelper.search("select count(*) from %s where %s='%s' and %s='%s'", 
										 gou_conv::kTableName, gou_conv::kAddrCode, strAddrCode, gou_conv::kGouName, strGouName) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strGouCode;
		switch( lCount )
		{
		case 0:	// 件数なし(新規コード発行)
			{
				// 該当11桁コードの最後に割り当てられた地番コードを取得
				CADOCursor cCur2( m_cAdoHelper.search( "select max(%s) from %s where %s='%s'", 
													   gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, strAddrCode ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					if( VT_BSTR == vaValue.vt )
					{
						// 取得したコード+1を新コードとして割り当て
						CString strMaxGouCode = CString( (*cCur2.begin())[0].bstrVal );
						TCHAR szAlpha = strMaxGouCode.GetAt(0);
						long lSeq = _tstol(strMaxGouCode.Mid(1)) + 1;
						if( lSeq > 999 )
						{
							++szAlpha;
							lSeq = 0;
							if( 'G' == szAlpha )
								return progress_out::ProgressError<CString>( _T(""), kFatal, "これ以上住居番号コードの割り当てができません : %s", strAddrCode );
						}
						strGouCode.Format( _T("%c%03d"), szAlpha, lSeq );
					}
					else
						strGouCode = _T("A000");

					return InsertCode('g', strAddrCode, strGouName, strGouCode)? strGouCode : _T("");
				}
				else
					return progress_out::ProgressError<CString>( _T(""), kError, "住居番号コードの取得に失敗[%s]", cCur2.GetErrorMessage() );
			}
			break;
		case 1:	// 既存コード
			{
				CADOCursor cCur2( m_cAdoHelper.search( "select %s from %s where %s='%s' and %s='%s'", 
													   gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, strAddrCode, 
													   gou_conv::kGouName, strGouName ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strGouCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
					return progress_out::ProgressError<CString>( _T(""), kError, "住居番号コードの取得に失敗[%s]", cCur2.GetErrorMessage() );
			}
			break;
		default:
			return progress_out::ProgressError<CString>( _T(""), kFatal, "同一キーに２つのコードが割り当てられている : %s, %s", strAddrCode, strGouName );
		}
		return strGouCode;
	}
	else
		return progress_out::ProgressError<CString>( _T(""), kError, "住居番号コードの取得に失敗[%s]", cCur.GetErrorMessage() );
}

//////////////////////////////////////////
//	内部関数
//////////////////////////////////////////

// 新規コード登録
bool CCodeConvTable::InsertCode( TCHAR szClass, const CString& strAddrCode, const CString& strName, const CString& strNewCode )
{
	// 登録日取得
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strRegistDate;
	strRegistDate.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	CADOCursor cCur = m_cAdoHelper.insert( "insert into %s values( '%s', '%s', '%s', '%s' )",
										   ('p' == szClass)? pnt_conv::kTableName : gou_conv::kTableName,	// テーブル名
										   strAddrCode, strName, strNewCode, strRegistDate );

	if( cCur.err() == S_OK )
		return true;
	else
		return progress_out::ProgressError<bool>( false, kError, "テーブルに新コードを追加失敗[%s] : %s %s %s", cCur.GetErrorMessage(), strAddrCode, strName, strNewCode );
}
