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
#include "ADOBase.h"
#include <iostream>

//void PrintComError(_com_error &e);
void PrintComError(_com_error &e)
{

    _bstr_t bstrSource(e.Source());
    _bstr_t bstrDescription(e.Description());

    // Print Com errors.
	std::cerr << "\nError" << std::endl;
	CStringA strErrorCode;
	strErrorCode.Format("\tCode = %08lx", e.Error());
	std::cerr << strErrorCode << std::endl;
	std::cerr << "\tCode meaning = " << CStringA(e.ErrorMessage()) << std::endl;
	std::cerr << "\tSource = " << CStringA((BSTR)bstrSource) << std::endl;
	std::cerr << "\tDescription = " << CStringA((BSTR)bstrDescription) << std::endl;
}

void CADOCursor::iterator::getvalue()
{
	_RecordsetPtr stream( base()->stream_ );
	val_type& values( base()->val_ );

	short lNum = (short)base()->fields_.size();
	if( values.empty() )
		values.assign(lNum, CComVariant()); // メモリ確保
	// 値を取得
	for( short i = 0; i < lNum; ++i )
		values[i] = stream->ADOFields->Item[i]->Value;
}

void CADOCursor::iterator::increment()
{
	_RecordsetPtr stream( base()->stream_ );
	base_reference()->errcode_ = stream->MoveNext();

	if( ! stream->EndOfFile )
		getvalue();
	else {
		stream->Close();
		stream = NULL;
		base_reference() = NULL;
	}
}

// 更新結果を適用する
long CADOCursor::iterator::store()
{
	_RecordsetPtr stream( base()->stream_ );
	val_type& values( base()->val_ );

	if( ! stream )
	{
		base()->errcode_ = E_HANDLE;
		return E_HANDLE;
	}

	short lNum = (short)base()->fields_.size();
	for( short i = 0; i < lNum; ++i )
	{
		try {
			stream->ADOFields->Item[i]->Value = values[i];
		}
		catch( _com_error& e )
		{
#ifdef _DEBUG
			PrintComError( e );
#endif // ifdef _DEBUG
			base()->errcode_ = e.Error();
			throw e;
		}
	}
	// _RecordSet::Update
	try {
		stream->Update();
	}
	catch( _com_error& e )
	{
#ifdef _DEBUG
		PrintComError( e );
#endif // ifdef _DEBUG
		base()->errcode_ = e.Error();
		throw e;
	}

	return base()->errcode_;
}

CString CADOCursor::GetErrorMessage() const
{
	CString strRet; // 戻り値
	_ConnectionPtr conn( stream_->GetActiveConnection() );
	if( conn && conn->Errors->Count > 0 )
	{
		long lCount = conn->Errors->Count;
		for( long i = 0; i < lCount; ++i )
		{
			ErrorPtr ipErr( conn->Errors->Item[i] );
			CString strMsg;
			strMsg.Format(_T("ERR NUM:%x %s"), ipErr->Number, (LPCTSTR)ipErr->Description );
			if( ! strRet.IsEmpty() )
				strRet += _T("\n");
			strRet += strMsg;
		}
	}
	return strRet;
}

CADOCursor CADOBase::search( LPCSTR format, ... ) const
{
	// 可変長引数の展開
	CStringA strSQL;
	if( format )
	{
		va_list args;
		va_start( args, format );
		strSQL.FormatV( format, args );
		va_end(args);
	}

	return query( false, strSQL );
}

CADOCursor CADOBase::update( LPCSTR format, ... ) const
{
	// 可変長引数の展開
	CStringA strSQL;
	if( format )
	{
		va_list args;
		va_start( args, format );
		strSQL.FormatV( format, args );
		va_end(args);
	}

	return query( true, strSQL );
}

CADOCursor CADOBase::insert( LPCSTR format, ... ) const
{
	// 可変長引数の展開
	CStringA strSQL;
	if( format )
	{
		va_list args;
		va_start( args, format );
		strSQL.FormatV( format, args );
		va_end(args);
	}

	return query( true, strSQL, true );
}

CADOCursor CADOBase::query( bool is_update, LPCSTR query, bool is_nocursor/* = false*/ ) const
{
	CADOCursor cRet; // 戻り値

	lastquery_ = query; // クエリ保存

	// 検索実行
	_RecordsetPtr stream( __uuidof(Recordset) );
	try {
		cRet.errcode_ = stream->Open( query, _variant_t((IDispatch *)conn_,true), ( is_update ) ? adOpenKeyset : adOpenForwardOnly, ( is_update ) ? adLockOptimistic : adLockReadOnly, adCmdText );
	}
	catch( _com_error& e )
	{
		cRet.errcode_ = e.Error();
		throw e;
	}

	if( S_OK == cRet.errcode_ )
	{
		if( ! is_nocursor )
		{
			// フィールド情報取得
			long lCount = stream->ADOFields->Count;
			for( long i = 0; i < lCount; ++i )
			{
				cRet.fields_.push_back( CADOCursor::Field() );
				CADOCursor::Field& field( *cRet.fields_.rbegin() );
				field.name = (BSTR)stream->ADOFields->Item[i]->Name;
				//field.size = stream->ADOFields->Item[i]->ActualSize; レコードセットが0だと落ちる（bug 6386）
				field.size = 0; // これはSDEBaseでのみ必要。ADOはVariantで受けるのでいらない
			}

			try {
				if( ! stream->EndOfFile )
					cRet.errcode_ = stream->MoveFirst();
			}
			catch( _com_error& e )
			{
				cRet.errcode_ = e.Error();
				throw e;
			}
		}
	}

	cRet.stream_ = stream;

	return cRet;
}

void CADOBase::attach( _Connection* conn )
{
	conn_ = conn;
	attached_ = true;
}

bool CADOBase::connect( LPCTSTR lpcszPath, CString& errMsg /*= CString()*/ )
{
	if( ! lpcszPath )
		return false;

	conn_.CreateInstance( __uuidof(Connection) );
	CString strExt = ((CString)lpcszPath).Right(4).MakeLower();
	CString strConn;
	if( strExt == _T(".xls"))
		strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=1\" "), lpcszPath );
	if( strExt == _T(".csv") )
	{
		CString strPath(lpcszPath);
		// フォルダ指定は最後の\まで必要（bug 9354）
		strPath = strPath.Left( strPath.ReverseFind(_T('\\'))+1 );
		strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties=\"Text;HDR=Yes;FMT=Delimited\" "), strPath );
	}
	else if(strExt == _T(".mdb"))
		strConn.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s "), lpcszPath );
	conn_->ConnectionString = (LPCTSTR)strConn;
	try {
		conn_->Open(_T(""),"","",adConnectUnspecified);
	}
	catch( _com_error& e ) {
		PrintComError(e);
		errMsg = e.ErrorMessage();
		conn_ = NULL;
		return false;
	}
	return true;
}

bool CADOBase::connect_by_string( LPCTSTR lpcszConnectString )
{
	if( ! lpcszConnectString )
		return false;

	conn_.CreateInstance( __uuidof(Connection) );
	conn_->ConnectionString = lpcszConnectString;
	try {
		conn_->Open(_T(""),"","",adConnectUnspecified);
	}
	catch( _com_error& e ) {
		PrintComError(e);
		conn_ = NULL;
		return false;
	}
	return true;
}

void CADOBase::disconnect()
{
	if ( conn_ )
	{
		if( ! attached_ )
			conn_->Close();
	}

	conn_ = NULL;
	attached_ = false;
}

