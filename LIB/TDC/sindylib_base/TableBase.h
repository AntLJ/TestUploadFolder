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

/**
 * @file TableBase.h
 * @brief CTableBaseクラス定義
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _TABLEBASE_H_
#define _TABLEBASE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WinLib/ComWrapper.h"
#include "ErrorCode.h"
#include "SchemaSupport.h"

namespace sindy {
class CRowBase;

/**
 * @class CTableBase
 * @brief ITablePtrのラッパークラス
 *
 * 全てのテーブルクラスはこのクラスが基底となります
 */
#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)
class CTableBase : public CComWrapper<ITable>, public CSchemaSupport
{
public:
	CTableBase()
	{
	}
	virtual ~CTableBase(){}

	CTableBase( ITable* lp ){ SetObject( sindyTableType::unknown, lp ); }
	CTableBase( IFeatureClass* lp ){ SetObject( sindyTableType::unknown, lp ); }
	CTableBase( sindyTableType::ECode emTableType, ITable *lp ){ SetObject( emTableType, lp ); }
	CTableBase( sindyTableType::ECode emTableType, IFeatureClass *lp ){	SetObject( emTableType, lp ); }

	CTableBase& operator =( ITable* lp ){ SetObject( sindyTableType::unknown, lp ); return *this; }
	CTableBase& operator =( IFeatureClass* lp ){ SetObject( sindyTableType::unknown, lp ); return *this; }
	CTableBase& operator =( const CTableBase& obj );

	/**
	 * @brief <b>テーブルをセットする</b>\n
	 * テーブルをセットします。通常はこちらを使用してください。
	 * @param lp	[in]	テーブル
	 */
	void SetObject( ITable* lp, const CSPFieldMap& cFields = CSPFieldMap(), const CSPTableNameString& cNames = CSPTableNameString() );
	void SetObject( IFeatureClass* lp, const CSPFieldMap& cFields = CSPFieldMap(), const CSPTableNameString& cNames = CSPTableNameString() ){ SetObject( (ITablePtr)lp, cFields, cNames ); }

	/**
	 * @brief <b>テーブルをセットする</b>\n
	 * テーブルタイプを直接指定する版です。
	 * テーブル名が違うがフィールドは一緒という場合に動作を既存のタイプに合わせるために
	 * 使用します。\n
	 * テーブルタイプがsindyTableType::unknownの場合には、GetTableTypeが呼ばれたときに
	 * 一度だけテーブルタイプの調査のための処理が走ります。直接指定することで、そのコス
	 * トを削減することができるというメリットもあります。
	 * @warning 以前のバージョンまではテーブルタイプを必ず指定する必要がありましたが、
	 * 現在はテーブルタイプはGetTableTypeが呼ばれたときに必要に応じてセットされるので
	 * 必須ではなくなりました。
	 */
	void SetObject( sindyTableType::ECode emTableType, ITable *lp);
	void SetObject( sindyTableType::ECode emTableType, IFeatureClass *lp){ SetObject( emTableType, (ITablePtr)lp ); }

	operator ITable*() const
	{
		return (ITable*)p;
	}

	//@{ @name IClassインターフェース
	IUIDPtr GetCLSID() const;
	long FindField( LPCTSTR Name ) const;
	IFieldsPtr GetFields() const;
	bool HasOID() const;
	LPCTSTR GetOIDFieldName( int nLen, LPTSTR lpszName ) const;
	//@}
	//@{ @name ITableインターフェース
	/**
	 * @brief レコードを作成する
	 * @note 新規作成時の自動属性付与に対応しています。
	 */
	virtual CRowBase& CreateRow( CRowBase& cRow );
	/**
	 * @brief ダミーレコードを作成する
	 * 実体化させる場合は、CRowBase::Materializationを使用してください。
	 * @note 新規作成時の自動属性付与に対応しています。
	 */
	virtual CRowBase& CreateDummyRow( CRowBase& cRow );
	errorcode::sindyErrCode DeleteSearchedRows( IQueryFilter* QueryFilter );
	_IRowPtr GetRow( long OID ) const;
	_IRowPtr GetRow( IQueryFilter* QueryFilter ) const;
	_ICursorPtr GetRows( const VARIANT& oids, bool Recycling ) const;
	long RowCount( IQueryFilter* QueryFilter ) const;
	_ICursorPtr Search( IQueryFilter* QueryFilter, bool Recycling ) const;
	//@}
	/**
	 * @brief <b>テーブル名などを管理するメンバの共有コピーを返す</b>\n
	 * m_spNameStringの実体のshared_ptrを返します。その際、作成されていなければ
	 * 自動的に作成されます。
	 * @return boost::shared_ptr<CTableNameString>
	 */
	CSPTableNameString GetNameString() const;

	/**
	 * @brief <b>フィールド対応表の共有コピーを返す</b>\n
	 * m_spFieldMapの実体のshared_ptrを返します。
	 * その際、作成されてなければ自動的に作成します。
	 * @return boost::shared_ptr<CFieldMap>
	 */
	CSPFieldMap GetFieldMap() const;

	/**
	 * @brief <b>同じワークスペースに存在するテーブルをオープンする</b>\n
	 * IFeatureWorkspace::OpenTableと同じ動作をします。\n
	 * オーナー名を考慮する必要がある場合は、オーナー名付きのテーブル名を渡す必要があります。\n
	 * このオブジェクトと同じオーナー名のテーブルをオープンする場合はOpenSameOwnerTable関数を
	 * 使用してください。
	 * @param lpcszTableName	[in]	テーブル名
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>同じワークスペースに存在するテーブルをオープンする</b>\n
	 * @param lpcszOwnerName	[in]	オーナー名
	 * @param emTableType		[in]	テーブルタイプ
	 * @return ITablePtr
	 */
	ITablePtr OpenTable( LPCTSTR lpcszOwnerName, sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>同じワークスペースに存在する同じオーナー名のテーブルをオープンする</b>\n
	 * lpcszTableNameにオーナー名が含まれている場合はそのオーナー名をこのオブジェクトのオーナー名
	 * にしてテーブルをオープンします。
	 * @param lpcszTableName	[in]	テーブル名
	 * @return ITablePtr
	 */
	ITablePtr OpenSameOwnerTable( LPCTSTR lpcszTableName ) const;

	/**
	 * @brief <b>同じワークスペースに存在する同じオーナー名のテーブルをオープンする</b>\n
	 * @param emTableType		[in]	テーブルタイプ
	 * @return ITablePtr
	 */
	ITablePtr OpenSameOwnerTable( sindyTableType::ECode emTableType ) const;

	/**
	 * @brief <b>ワークスペースを取得する</b>\n
	 * @return IWorkspacePtr
	 */
	IWorkspacePtr GetWorkspace() const;

	/**
	 * @brief <b>ID列からINで使用できる文字列を作成する</b>\n
	 * INで使用できるIDの数の上限は1000なので、1000を超える場合は
	 * 複数の文字列が返ります。
	 * @param listIDs	[in]	ID列
	 * @param strIDs	[out]	1000個区切りのID列（e.x. "100,110,115,200"）
	 */
	static void IDs2Str( const std::list<long>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep = NULL )
	{
		if( ! listIDs.empty() )
		{
			strIDs.push_back( CString(_T("")) );
	
			long lCount = 1;
			std::list<CString>::reverse_iterator itStr = strIDs.rbegin();

			for( std::list<long>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it, ++lCount )
			{
				CString item;
				if( str_sep )
					item.Format(_T("%s%ld%s"), str_sep, *it, str_sep );
				else
					item.Format(_T("%ld"), *it );

				if( itStr->IsEmpty() ) 
					*itStr = item;
				else {
					*itStr += _T(",");
					*itStr += item;
				}

				if( 1000 == lCount )
				{
					strIDs.push_back( CString(_T("")) );
					itStr = strIDs.rbegin();
					lCount = 0;
				}
			}
		}
		// 1000で割り切れる場合だと空が余計に入るので除去
		if( ( ! strIDs.empty() ) && strIDs.rbegin()->IsEmpty() )
			strIDs.pop_back();
	}

	/**
	 * @brief <b>ID列からINで使用できる文字列を作成する</b>\n
	 * INで使用できるIDの数の上限は1000なので、1000を超える場合は
	 * 複数の文字列が返ります。
	 * @param listIDs	[in]	ID列
	 * @param strIDs	[out]	1000個区切りのID列（e.x. "'100','110','115','200'"）
	 */
	static void IDs2Str( const std::list<CString>& listIDs, std::list<CString>& strIDs, LPCTSTR str_sep = NULL )
	{
		if( ! listIDs.empty() )
		{
			strIDs.push_back( CString(_T("")) );
	
			long lCount = 1;
			std::list<CString>::reverse_iterator itStr = strIDs.rbegin();

			for( std::list<CString>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it, ++lCount )
			{
				CString item;
				if( str_sep )
					item.Format(_T("%s%s%s"), str_sep, *it, str_sep );
				else
					item = *it;

				if( itStr->IsEmpty() ) 
					*itStr = item;
				else {
					*itStr += _T(",");
					*itStr += item;
				}

				if( 1000 == lCount )
				{
					strIDs.push_back( CString(_T("")) );
					itStr = strIDs.rbegin();
					lCount = 0;
				}
			}
		}
		// 1000で割り切れる場合だと空が余計に入るので除去
		if( ( ! strIDs.empty() ) && strIDs.rbegin()->IsEmpty() )
			strIDs.pop_back();
	}

	/**
	 * @brief <b>OIDリストで検索する</b>\n
	 * @param fids		[in]	OIDリスト
	 * @param Recycling	[in]	NextRowでのIRowをリサイクルする場合はtrue
	 * @return ICursorPtr
	 */
	
	template <typename _Container> _ICursorPtr GetRows( const _Container& fids, bool Recycling ) const
	{
		SAFEARRAYBOUND bound = { (ULONG)fids.size(), 0 };
		SAFEARRAY* pSa = SafeArrayCreate( VT_I4, 1, &bound );
		LONG* pArray;
		SafeArrayAccessData( pSa, (void**)&pArray );

		long i = 0;
		for( _Container::const_iterator it = fids.begin(); it != fids.end(); ++it, ++i )
			pArray[i] = *it;

		SafeArrayUnaccessData( pSa );
		VARIANT vaArray;
		VariantInit( &vaArray );
		vaArray.vt = VT_ARRAY | VT_I4;
		vaArray.parray = pSa;

		_ICursorPtr ipCursor = GetRows( vaArray, Recycling );
		VariantClear( &vaArray );

		return ipCursor;
	}

	/**
	 * @brief <b>OIDリストで検索する</b>\n
	 * @note 引数二つのものと異なり、WHERE句にlpcszRelIDFieldName in ( fids[1], fids[2], ... )という指定が入ります
	 * @warning fidsが1000個を超える場合は一度に検索できないためNULLを返します（@see IDs2Str）
	 * @param fids					[in]	OIDリスト
	 * @param Recycling				[in]	NextRowでのIRowをリサイクルする場合はtrue
	 * @param lpcszRelIDFieldName	[in]	fidsが適用されるフィールド名
	 * @return ICursorPtr
	 */
	_ICursorPtr GetRows( const std::list<long>& fids, bool Recycling, LPCTSTR lpcszRelIDFieldName ) const;
};
#pragma warning(pop)

typedef boost::shared_ptr<CTableBase> CSPTableBase;

} // sindy

#endif // _TABLE_H_
