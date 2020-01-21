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
#include "CodeConverter.h"

namespace addr
{
	// 変換テーブルスキーマ
	namespace cv_schema
	{
		namespace cv_common
		{
			const TCHAR kAddrCode[]		= _T("AddrCode");
			const TCHAR kRegistDate[]	= _T("RegistDate");
		}

		namespace pnt_conv
		{
			const TCHAR kTableName[]	= _T("Pnt_CodeConv");
			const TCHAR kPntCode[]	= _T("PntCode");
			const TCHAR kPntName[]	= _T("PntName");
			using namespace cv_schema::cv_common;		
		}

		namespace gou_conv
		{
			const TCHAR kTableName[]	= _T("Gou_CodeConv");
			const TCHAR kGouCode[]	= _T("GouCode");
			const TCHAR kGouName[]	= _T("GouName");
			using namespace cv_schema::cv_common;
		}
	}

using namespace cv_schema;


bool CCodeConverter::Init( const _TCHAR* lpcszCodeConvDB )
{
	 // ADO使用
	try
	{
		m_cAdoHelper.disconnect();

		_ConnectionPtr ipConn( _uuidof(Connection) );

		CString strCon;
		strCon.Format( _T("Driver={Microsoft Access Driver (*.mdb)}; DBQ=%s;"), lpcszCodeConvDB );
		if( SUCCEEDED(ipConn->Open(_bstr_t(strCon), _T(""), _T(""), adConnectUnspecified)) )
		{
			m_cAdoHelper.attach( ipConn );
			return true;
		}
		else
			return false;
	}
	catch( _com_error& e )
	{
		m_strError.Format( _T("変換テーブルオープン時にCOMエラー発生, %s"), COLE2CT(e.Description()) );
		return false;
	}
}

// 地番コード取得
CString CCodeConverter::GetPntCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntName, bool bSave /* = false */ )
{
	CString strSQL;
	strSQL.Format( _T("select count(*) from %s where %s='%s' and %s='%s'"), pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntName, lpcszPntName );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strPntCode;
		switch( lCount )
		{
		case 0:	// 件数なし(新規コード発行)
			{
				// 該当11桁コードの最後に割り当てられた地番コードを取得
				strSQL.Format( _T("select max(%s) from %s where %s='%s'"), pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
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
							{
								m_strError.Format( _T("これ以上地番コードの割り当てができません, %s"), lpcszAddrCode );
								return _T("");
							}
						}
						strPntCode.Format( _T("%c%04d"), szAlpha, lSeq );
					}
					else
						strPntCode = _T("A0000");
					
					return (!bSave || InsertCode('p', lpcszAddrCode, lpcszPntName, strPntCode))? strPntCode : _T("");	// 保存するならテーブルに追加
				}
				else
				{
					m_strError.Format( _T("地番コードの取得に失敗, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		case 1:	// 既存コード
			{
				strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
							   pnt_conv::kPntCode, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntName, lpcszPntName );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strPntCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
				{
					m_strError.Format( _T("地番コードの取得に失敗, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		default:
			m_strError.Format( _T("同一キーに２つのコードが割り当てられている, %s, %s"), lpcszAddrCode, lpcszPntName );
			return _T("");
		}
		return strPntCode;
	}
	else
	{
		m_strError.Format( _T("地番コードの取得に失敗, %s"), cCur.GetErrorMessage() );
		return _T("");
	}		
}

// 住居番号コード取得
CString CCodeConverter::GetGouCode( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouName, bool bSave /* = false */ )
{
	CString strSQL;
	strSQL.Format( _T("select count(*) from %s where %s='%s' and %s='%s'"), gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouName, lpcszGouName );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
 	if( cCur.err() == S_OK )
	{
		long lCount = (*cCur.begin())[0].lVal;
		CString strGouCode;
		switch( lCount )
		{
		case 0:	// 件数なし(新規コード発行)
			{
				// 該当11桁コードの最後に割り当てられた地番コードを取得
				strSQL.Format( _T("select max(%s) from %s where %s='%s'"), gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode );
				CADOCursor cCur2( m_cAdoHelper.search(CT2CA(strSQL)) );
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
							{
								m_strError.Format( _T("これ以上住居番号コードの割り当てができません, %s"), lpcszAddrCode );
								return _T("");
							}
						}
						strGouCode.Format( _T("%c%03d"), szAlpha, lSeq );
					}
					else
						strGouCode = _T("A000");

					return (!bSave || InsertCode('g', lpcszAddrCode, lpcszGouName, strGouCode))? strGouCode : _T("");	// 保存するならテーブルに追加
				}
				else
				{
					m_strError.Format( _T("住居番号コードの取得に失敗, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		case 1:	// 既存コード
			{
				strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
							   gou_conv::kGouCode, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouName, lpcszGouName );
				CADOCursor cCur2( m_cAdoHelper.search( CT2CA(strSQL) ) );
				if( cCur2.err() == S_OK )
				{
					CComVariant vaValue = (*cCur2.begin())[0];
					strGouCode = (VT_BSTR == vaValue.vt)? CString(vaValue.bstrVal) : _T("");
				}
				else
				{
					m_strError.Format( _T("住居番号コードの取得に失敗, %s"), cCur2.GetErrorMessage() );
					return _T("");
				}
			}
			break;
		default:
			m_strError.Format( _T("同一キーに２つのコードが割り当てられている, %s, %s"), lpcszAddrCode, lpcszGouName );
			return _T("");
		}
		return strGouCode;
	}
	else
	{
		m_strError.Format( _T("住居番号コードの取得に失敗, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

// 地番名称取得
CString CCodeConverter::GetPntName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszPntCode ) const
{
	CString strSQL;
	strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
				   pnt_conv::kPntName, pnt_conv::kTableName, pnt_conv::kAddrCode, lpcszAddrCode, pnt_conv::kPntCode, lpcszPntCode );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
	if( cCur.err() == S_OK && cCur.begin() != cCur.end() )
	{
		CComVariant vaValue = (*cCur.begin())[0];
		return SUCCEEDED(vaValue.ChangeType(VT_BSTR))? CString(vaValue.bstrVal) : _T("");	// バイナリ型で格納しているので、VT_BSTRに変換
	}
	else
	{
		m_strError.Format( _T("地番名称の取得に失敗, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

// 住居番号名称取得
CString CCodeConverter::GetGouName( const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszGouCode ) const
{
	CString strSQL;
	strSQL.Format( _T("select %s from %s where %s='%s' and %s='%s'"), 
				   gou_conv::kGouName, gou_conv::kTableName, gou_conv::kAddrCode, lpcszAddrCode, gou_conv::kGouCode, lpcszGouCode );
	CADOCursor cCur( m_cAdoHelper.search(CT2CA(strSQL)) );
	if( cCur.err() == S_OK && cCur.begin() != cCur.end()  )
	{
		CComVariant vaValue = (*cCur.begin())[0];
		return SUCCEEDED(vaValue.ChangeType(VT_BSTR))? CString(vaValue.bstrVal) : _T("");	// バイナリ型で格納しているので、VT_BSTRに変換
	}
	else
	{
		m_strError.Format( _T("住居番号名称の取得に失敗, %s"), cCur.GetErrorMessage() );
		return _T("");
	}
}

//////////////////////////////////////////
//	内部関数
//////////////////////////////////////////

// 新規コード登録
bool CCodeConverter::InsertCode( TCHAR szClass, const _TCHAR* lpcszAddrCode, const _TCHAR* lpcszName, const _TCHAR* lpcszNewCode )
{
	// 登録日取得
	SYSTEMTIME st;
	::GetLocalTime( &st );
	CString strRegistDate;
	strRegistDate.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );

	CString strSQL;
	strSQL.Format( _T("insert into %s values( '%s', '%s', '%s', '%s' )"),
				   ('p' == szClass)? pnt_conv::kTableName : gou_conv::kTableName, lpcszAddrCode, lpcszName, lpcszNewCode, strRegistDate );
	CADOCursor cCur = m_cAdoHelper.insert( CT2CA(strSQL) );
	if( cCur.err() == S_OK )
		return true;
	else
	{
		m_strError.Format( _T("テーブルに新コードを追加失敗, %s, %s, %s, %s"), cCur.GetErrorMessage(), lpcszAddrCode, lpcszName, lpcszNewCode );
		return false;
	}
}

}	// namespace addr
