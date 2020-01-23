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
#include "sdebase.h"
#include "../ArcMapUIBase/esrihidinginterface.h"
#include "time_convert.h"
#include "../TDC/useful_headers/variant_util.h"

IWorkspacePtr AheGetWorkspace( IUnknown* ipUnk );

CString CSDEException::ToString() const
{
	CString strMsg;
	strMsg.Format(_T("詳細：\n%s\n%s\nエラーコード：SDE(%d) 拡張(%d)"), CString(err_msg1), CString(err_msg2), sde_error, ext_error );
	return strMsg;
}

CString CSDEStream::GetErrorMessage() const
{
	CSDEException e;
	SSE_stream_get_ext_error( stream_, &e );
	return e.ToString();
}

CSDECursor::CSDECursor() : stream_(new CSDEStream() ) {}
CSDECursor::CSDECursor(SE_CONNECTION conn) : stream_( new CSDEStream(conn) ) {}

void CSDECursor::iterator::increment()
{
	SE_STREAM stream = (SE_STREAM)*base()->stream_;
	val_type& values( base()->val_ );

	static CComVariant vaNull;
	vaNull.vt = VT_NULL;

	// fetch
	long& errcode( base()->stream_->errcode_ );
	try {
		if( SE_FINISHED != ( errcode = SSE_stream_fetch( stream ) ) )
		{
			if ( SE_SUCCESS == errcode )
			{
				short lNum = (short)base()->fields_.size();
				if( values.empty() )
					values.assign(lNum, CComVariant()); // メモリ確保
				for( short i = 0; i < lNum; ++i )
				{
					switch( base()->fields_[i].sde_type )
					{
						case SE_SMALLINT_TYPE:
						{
							short v = 0;
							errcode = SSE_stream_get_smallint( stream, i + 1, &v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							values[i] = ( SE_NULL_VALUE == errcode ) ? vaNull : v;
							break;
						}
						case SE_INTEGER_TYPE:
						{
							long v = 0;
							errcode = SSE_stream_get_integer( stream, i + 1, &v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							values[i] = ( SE_NULL_VALUE == errcode ) ? vaNull : v;
							break;
						}
						case SE_FLOAT_TYPE:
						{
							float v = 0;
							errcode = SSE_stream_get_float( stream, i + 1, &v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							values[i] = ( SE_NULL_VALUE == errcode ) ? vaNull : v;
							break;
						}
						case SE_DOUBLE_TYPE:
						{
							double v = 0;
							errcode = SSE_stream_get_double( stream, i + 1, &v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							values[i] = ( SE_NULL_VALUE == errcode ) ? vaNull : v;
							break;
						}
						case SE_STRING_TYPE:
						{
							LPSTR v = new CHAR[base()->fields_[i].size+1];
							errcode = SSE_stream_get_string( stream, i + 1, v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								 delete [] v;
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							values[i] = ( SE_NULL_VALUE == errcode ) ? vaNull : v;
							delete [] v;
							break;
						}
						case SE_DATE_TYPE:
						{
							DATE dDate = 0;
							tm v = {0};
							errcode = SSE_stream_get_date( stream, i + 1, &v );
							 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
							 {
								CSDEException e;
								SSE_stream_get_ext_error( stream, &e );
								throw e;
							 }
							if( SE_NULL_VALUE != errcode )
							{
								values[i] = tmToVariantTime( v );
								values[i].vt = VT_DATE; // CComVariantはDATEで渡してもdoubleになるので
							}
							else
								values[i] = vaNull;
							break;
						}
						case SE_BLOB_TYPE:
						{
							SE_BLOB_INFO blob = { 0, NULL };
							errcode = SSE_stream_get_blob( stream, i + 1, &blob );
							if( SE_SUCCESS == errcode )
							{
								blob.blob_buffer = ( new char[blob.blob_length] );
								errcode = SSE_stream_get_blob( stream, i + 1, &blob );
								// VT_BLOBはBSTRとして格納する
								// なぜなら、CComVariantのデストラクタでちゃんと開放
								// してくれそうなのはこれしかないから。。。
								values[i].vt = VT_BSTR;
								values[i].bstrVal = SysAllocStringByteLen( blob.blob_buffer, blob.blob_length );
							}
							break;
						}
						default:
							_ASSERTE( false );
							break;
					}
				}
			}
			else {
				CSDEException e;
				SSE_stream_get_ext_error( stream, &e );
				throw e;
			}
		}
		else {
			SSE_stream_free( stream );
			stream = NULL;
			base_reference() = NULL;
		}
	}
	catch( CSDEException& e )
	{
		SSE_stream_free( stream );
		stream = NULL;
		base_reference() = NULL;

		throw e;
	}
}

#ifdef _USE_SDE_UPDATE
long CSDECursor::iterator::store()
{
	SE_STREAM stream = (SE_STREAM)*base()->stream_;
	const val_type& values( base()->val_ );
	const std::vector<CSDECursor::Field>& fields( base()->fields_ );
	long& errcode( base()->stream_->errcode_ );

	unsigned long lNum = values.size();
	try {
		for( unsigned long i = 0; i < lNum; ++i )
		{
			const CSDECursor::Field& field( fields[i] );
			const CComVariant& vVal( values[i] );
			switch( field.sde_type )
			{
				case SE_SMALLINT_TYPE:
				{
					errcode = SSE_stream_set_smallint( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : &vVal.iVal );
					 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					 {
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					 }
					break;
				}
				case SE_INTEGER_TYPE:
				{
					errcode = SSE_stream_set_integer( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : &vVal.lVal );
					 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					 {
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					 }
					break;
				}
				case SE_FLOAT_TYPE:
				{
					errcode = SSE_stream_set_float( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : &vVal.fltVal );
					 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					 {
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					 }
					break;
				}
				case SE_DOUBLE_TYPE:
				{
					errcode = SSE_stream_set_double( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : &vVal.dblVal );
					if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					{
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					}
					break;
				}
				case SE_STRING_TYPE:
				{
					CStringA str;
					if( VT_BSTR == vVal.vt )
						str = vVal.bstrVal;
					else
						str = "";
					errcode = SSE_stream_set_string( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : (LPCSTR)str );
					 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					 {
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					 }
					break;
				}
				case SE_DATE_TYPE:
				{
					tm v = {0};
					if( VT_DATE == vVal.vt )
						VariantTimeTotm( vVal.date, v );
					errcode = SSE_stream_set_date( stream, i + 1, ( VT_NULL == vVal.vt ) ? NULL : &v );
					 if( SE_NULL_VALUE != errcode && SE_SUCCESS != errcode )
					 {
						CSDEException e;
						SSE_stream_get_ext_error( stream, &e );
						throw e;
					 }
					break;
				}
				default:
					_ASSERTE( false );
					break;
			}
		}
	}
	catch( CSDEException& e )
	{
		throw e;
	}
}
#endif // ifdef _USE_SDE_UPDATE

CSDECursor CSDEBase::search( LPCSTR format, ... ) const
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

#ifdef _USE_SDE_UPDATE
CSDECursor CSDEBase::update( LPCSTR format, ... ) const
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
#endif // ifdef _USE_SDE_UPDATE

CSDECursor CSDEBase::query( bool/* bUpdate*/, LPCSTR query ) const
{
	CSDECursor cur; // 戻り値

	lastquery_ = query; // クエリ保存

	// 検索実行
	long& errcode = cur.stream_->errcode_;

	// stream作成
	if( SE_SUCCESS == ( errcode = cur.stream_->StreamCreate( conn_ ) ) )
	{
		SE_STREAM stream = (SE_STREAM)*cur.stream_;

#ifdef _USE_SDE_UPDATE
		// updateから呼ばれたときは書き込み可にする
		if( bUpdate )
			errcode = SSE_stream_set_write_mode( stream, FALSE );
#endif // ifdef _USE_SDE_UPDATE

		if( SE_SUCCESS == errcode )
		{
			// SQL用意
			if( SE_SUCCESS == ( errcode = SSE_stream_prepare_sql( stream, query ) ) )
			{
				// SQL実行
				if( SE_SUCCESS == ( errcode = SSE_stream_execute( stream ) ) )
				{
					// カラム数
					short lNum = 0;
					SSE_stream_num_result_columns( stream, &lNum );
					SE_COLUMN_DEF* column = new SE_COLUMN_DEF[lNum];

					// フィールド名
					for( short i = 0; i < lNum; ++i )
					{
						if( SE_SUCCESS == SSE_stream_describe_column( stream, i + 1, &column[i] ) )
						{
							cur.fields_.push_back( CSDECursor::Field() );
							CSDECursor::Field& field( *cur.fields_.rbegin() );
							field.name     = column[i].column_name;
							field.sde_type = column[i].sde_type;
							field.size     = column[i].size;
						}
					}
					delete [] column;
				}
			}
		}
	}

	return cur;
}

void CSDEBase::attach( IUnknown* ipUnk )
{
	ISdeWorkspaceImplPtr ipSDE( AheGetWorkspace( ipUnk ) );
	if( ! ipSDE )
		return;

	if( SUCCEEDED( ipSDE->get_ConnectionHandle( (long*)&conn_ ) ) && conn_ )
		attached_ = true;
}

void CSDEBase::connect( IUnknown* ipUnk )
{
	IWorkspacePtr ipWorkspace( AheGetWorkspace( ipUnk ) );

	CString strServer, strUser, strInstance;
	if ( ipWorkspace ) {
		IPropertySetPtr ipPropertySet;

		ipWorkspace->get_ConnectionProperties( &ipPropertySet );

		if ( ipPropertySet ) {
			CComVariant vaUser, vaInstance, vaServer;

			ipPropertySet->GetProperty( CComBSTR(_T("USER")), &vaUser );
			ipPropertySet->GetProperty( CComBSTR(_T("INSTANCE")), &vaInstance );
			ipPropertySet->GetProperty( CComBSTR(_T("DB_CONNECTION_PROPERTIES")), &vaServer );
			if ( uh::variant_util::isNullOrEmpty( vaServer ) )
				ipPropertySet->GetProperty( CComBSTR(_T("SERVER")), &vaServer );

			strServer = vaServer.bstrVal;
			strUser = vaUser.bstrVal;
			strInstance = vaInstance.bstrVal;

			strServer.MakeUpper();
			strUser.MakeUpper();
			strInstance.MakeUpper();
		}
	}

	if ( conn_ ) {
		if ( conninfo_.get<0>() != strServer || conninfo_.get<1>() != strUser || conninfo_.get<2>() != strInstance )
			disconnect();
	}

	if ( ! conn_ ) {
		SE_ERROR err = {0};
		long errcode = SE_SUCCESS;

		if( SE_SUCCESS == ( errcode = SSE_connection_create( CT2A(strServer), CT2A(strInstance), "", CT2A(strUser), CT2A(strUser), &err, &conn_ ) ) )
			conninfo_ = boost::tuples::make_tuple( strServer, strUser, strInstance );
	}
}

void CSDEBase::disconnect()
{
	if ( conn_ )
	{
		if( ! attached_ )
			SSE_connection_free( conn_ );
	}

	conn_ = NULL;
	attached_ = false;
}
