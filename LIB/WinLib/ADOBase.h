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

#include <vector>
#include <boost/iterator_adaptors.hpp>
/**
 * @class CADOCursor
 * @brief 検索カーソルとして動作するクラス
 */
class CADOCursor
{
	friend class CADOBase;
public:
	CADOCursor() : errcode_(0) {}
	CADOCursor(_Recordset* stream) : stream_(stream), errcode_(0){}

	/// フィールド定義クラス
	class Field
	{
	public:
		CString name;
		long sde_type;
		long size;
	};

	//@{ @name イテレータ定義
	typedef std::vector<CComVariant> val_type;
	struct iterator
		: boost::iterator_adaptor<iterator,CADOCursor*, val_type,boost::forward_traversal_tag>
	{
		iterator( CADOCursor* p )
			: iterator_adaptor<iterator,CADOCursor*, val_type, boost::forward_traversal_tag>( p ){}
		void iterator::getvalue();
		void increment();
		val_type& dereference() const { return base_reference()->val_; }

		/// 更新結果を適用する
		long store();
	};
	/// begin, end のいい加減な定義
	iterator begin(){ if( ! stream_->EndOfFile ){ iterator it(this); it.getvalue(); return it; } return end(); }
	iterator end()  { return iterator(NULL); }
	//@}

	/// 最後のエラーを文字列で返す
	CString GetErrorMessage() const;

	/// 最後のエラーコード
	long err() const { return errcode_; }

	/// フィールド情報
	const std::vector<Field>& fields() const { return fields_; }

private:
	_RecordsetPtr stream_; //!< カーソル用ストリーム
	HRESULT errcode_; //!< 最終エラーコード
	std::vector<Field> fields_; //!< フィールド定義リスト
	val_type val_;
};

/**
 * @class CADOBase
 * @biref _Connection*を管理するクラス
 */
class CADOBase
{
public:
	CADOBase() : conn_(NULL), attached_(false) {}
	CADOBase(_Connection* conn) : conn_(conn), attached_(true) {}
	CADOBase( const CADOBase& obj ) : conn_(obj.conn_), attached_(true) {}
	CADOBase( LPCTSTR lpcszPath ) : attached_(false) { connect(lpcszPath); }
	virtual ~CADOBase() { disconnect(); }

	CADOBase& operator=( _Connection* conn )
	{
		disconnect();
		attach( conn );
		return *this;
	}
	_Connection* handle(){	return conn_; }

	void attach( _Connection* conn );
/**
 * @brief ADO接続する
 * @note .xls、.csv、.mdbが対象
 * @param lpcszPath [in] フルパス
 * @param errMsg [out,optional] 失敗した場合、_com_error::ErrorMessage()の値
 * @retval true 成功
 * @retval false 失敗
 */
	bool connect( LPCTSTR lpcszPath, CString& errMsg = CString() );
	bool connect_by_string( LPCTSTR lpcszConnectString );
	void disconnect();

	/**
	 * @brief SQL検索を実行する
	 * 読み取り専用及びforward専用のカーソルを使用して
	 * select文を実行します。
	 */
	CADOCursor search( LPCSTR format, ... ) const;
	/**
	 * @brief 更新カーソル用SQLを実行する
	 * 更新可能なselect文を発行する場合に使用します。
	 * 変更を反映するには、iterator::store()を実行する
	 * 必要があります。
	 * @see iterator::store
	 */
	CADOCursor update( LPCSTR format, ... ) const;
	/**
	 * @brief インサート用SQLを実行する
	 * insert文等実行後カーソル操作が不要なものはこの関数で
	 * 実行する必要があります（update関数ではエラーが出る）
	 */
	CADOCursor insert( LPCSTR format, ... ) const;

	LPCTSTR getLastQuery() const { return lastquery_; }
protected:
	CADOCursor query( bool is_update, LPCSTR query, bool is_nocursor = false ) const;
protected:
	_ConnectionPtr conn_; //!< ADO接続オブジェクト
	bool attached_; //!< アタッチかどうか\n新規に接続した場合はfalse
	mutable CString lastquery_; //!< 最終クエリ
};
