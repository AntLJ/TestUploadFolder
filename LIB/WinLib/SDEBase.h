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

#pragma once

#include "ArcHelperEx/sde_ondemand.h" // TODO: 何とかしたい
#include <boost/tuple/tuple.hpp>
//#include "ListString.h"
#include <algorithm>
#include <boost/iterator_adaptors.hpp>
#include <boost/shared_ptr.hpp>

// #define _USE_SDE_UPDATE updateカーソルが使えないらしいので封印

/**
 * @class CSDEException
 * @brief SDE API 例外クラス
 */
class CSDEException : public SE_ERROR
{
public:
	CSDEException() : errcode(SE_SUCCESS)
	{
		sde_error = ext_error = 0;
		err_msg1[0] = err_msg2[0] = '\0';
	}
	CString ToString() const;
	long errcode; //!< SDE API が返したエラーコード
};

/**
 * @brief ストリーム管理クラス
 * @note ストリームの作成・開放を管理します。
 */
class CSDEStream
{
public:
	friend class CSDECursor;
	friend class CSDEBase;
	CSDEStream() : stream_(NULL), errcode_(0) {}
	CSDEStream(SE_CONNECTION conn) : stream_(NULL), errcode_(0) { StreamCreate( conn ); }
	virtual ~CSDEStream() { StreamFree(); }
	operator SE_STREAM() { return stream_; }
	long StreamCreate(SE_CONNECTION conn) { if( conn ) errcode_ = SSE_stream_create( conn, &stream_ ); return errcode_; }
	long StreamFree(){ if( stream_ ) { errcode_ = SSE_stream_free( stream_ ); stream_ = NULL; } return errcode_; }
	/// 最後のエラーコード
	long err() const { return errcode_; }
	/// 最後のエラーを文字列で返す
	CString GetErrorMessage() const;
protected:
	SE_STREAM stream_;
	long errcode_; //!< 最終エラーコード
};
typedef boost::shared_ptr<CSDEStream> CSPSDEStream;

/**
 * @class CSDECursor
 * @brief 検索カーソルとして動作するクラス
 * 
 * @note イテレータを使用する場合、フェッチ時のエラーは
 * CSDEExceptionの例外として飛ばします。
 * NULL OKなカラムで集合関数等を使用した場合に、NULLを検出すると
 * エラーが発生しますので、必ず例外を補足してください。
 */
class CSDECursor
{
	friend class CSDEBase;
public:
	CSDECursor();
	CSDECursor(SE_CONNECTION conn);
	operator SE_STREAM() { return (SE_STREAM)*stream_; }
	/// フィールド定義クラス
	class Field
	{
	public:
		CString name;
		long sde_type;
		long size;
	};
	typedef std::vector<CComVariant> val_type;

	//@{ @name イテレータ定義
	struct iterator
		: boost::iterator_adaptor<iterator,CSDECursor*, val_type, boost::forward_traversal_tag>
	{
		iterator( CSDECursor* p )
			: iterator_adaptor<iterator,CSDECursor*, val_type, boost::forward_traversal_tag>( p ){}
		void increment();
		val_type& dereference() const { return base_reference()->val_; }

#ifdef _USE_SDE_UPDATE
		/// 更新モードの場合に更新結果を適用する
		long store();
#endif // ifdef _USE_SDE_UPDATE
	};
	/// begin, end のいい加減な定義
	iterator begin(){ return ++iterator(this); }
	iterator end()  { return iterator(NULL); }
	//@}

	/// フィールド情報
	const std::vector<Field>& fields() const { return fields_; }

	/// 最後のエラーコード
	long err() const { return stream_->err(); }
	/// 最後のエラーを文字列で返す
	CString GetErrorMessage() const { return stream_->GetErrorMessage(); }
private:
	CSPSDEStream stream_;
	std::vector<Field> fields_; //!< フィールド定義リスト
	val_type val_;
};

/**
 * @class CSDEBase
 * @biref SE_CONNECTIONを管理するクラス
 */
class CSDEBase
{
public:
	CSDEBase() : conn_(NULL), attached_(false) {}
	CSDEBase(SE_CONNECTION conn) : conn_(conn), attached_(true) {}
	CSDEBase( const CSDEBase& obj ) : conn_(obj.conn_), attached_(true) {}
	CSDEBase( IUnknown* ipUnk ) : conn_(NULL), attached_(false) { attach( ipUnk ); }
	virtual ~CSDEBase() { disconnect(); }

	SE_CONNECTION handle(){	return conn_; }

	void attach( IUnknown* ipUnk );
	void connect( IUnknown* ipUnk );
	void disconnect();

	/// 検索を実行する
	CSDECursor search( LPCSTR format, ... ) const;
#ifdef _USE_SDE_UPDATE
	CSDECursor update( LPCSTR format, ... ) const;
#endif // ifdef _USE_SDE_UPDATE

	LPCTSTR getLastQuery() const { return lastquery_; }
protected:
	CSDECursor query( bool bUpdate, LPCSTR query ) const;
protected:
	SE_CONNECTION conn_; //!< SDE接続ハンドル
	bool attached_; //!< アタッチかどうか\n新規に接続した場合はfalse
	boost::tuple<CString,CString,CString>	conninfo_; //!< 新規に接続した場合の接続情報
	mutable CString lastquery_; //!< 最終クエリ
};
