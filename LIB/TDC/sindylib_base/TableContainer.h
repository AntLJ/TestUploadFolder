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

#include "TableBase.h"
#include "IndexedContainer.h"
#include "TableFinderSupport.h"
#include "sindylib_base_classconstructorimpl.h"

class CModel;

namespace sindy {

class CRowContainer;
class CTableContainer;
typedef boost::shared_ptr<CRowContainer> CSPRowContainer;
typedef boost::shared_ptr<CTableContainer> CSPTableContainer;

/**
 * @class CTableContainer
 * 
 * このクラスは編集系メソッドを直接呼ぶことをブロックするための
 * クラスです。CInfoMaster経由で行ってください。
 */
class CTableContainer : 
	public CTableBase, 
	public CIndexedContainer, 
	public CTableFinderSupport
{
	SINDYBASE_TABLECLASS_CONSTRUCTOR_BASE( CTableContainer )
public:
	/// sort用プレディケート
	class LessSPRowContainerOID
	{
	public:
		bool operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const;
	};

	//@{ CIndexedContainer オーバーライド
	virtual void push_back( const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void push_front( const CSPContainerBase& cSPContainer, bool bSecureUnique = true );
	virtual iterator insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique = true );
	virtual void Reset();
	//@}

	virtual CSPRowContainer Create();
	virtual CSPRowContainer CreateDummyRow();
	/// CreateDummyRowのpush_backしない版
	virtual CSPRowContainer CreateDummyRowNoPush();
	/// rowのキャッシュを使用してダミーRowを作成する
	virtual CSPRowContainer CreateDummyRow( const CRowContainer& row );
	virtual errorcode::sindyErrCode Materialization( LPCTSTR lpcszParentName, long lParentOID, CErrorObjectsBase& cErrs );
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const;

	/// 上り検索（単独）
	std::list<long>& SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszWhere, std::list<long>& listKey = std::list<long>() ) const;
	std::list<long>& SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszAddWhereKeyName, const std::list<long>& listAddWhereKey, std::list<long>& listKey = std::list<long>() ) const;
	/// 上り検索（階層）
	std::list<long>& SelectKey( LPCTSTR lpcszTopLevelTableName, const std::map<CString,IQueryFilterPtr>& mapQuery, std::list<long>& listKey = std::list<long>() ) const;

	/**
	 * @brief 下り検索（単独）
	 * 
	 * 検索結果は自分自身のコンテナに格納されます。
	 * @param listID       [in] キーID
	 * @param lpcszKeyName [in] キー名
	 */
	void _Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, bool bDetach, bool bUnique = false );
	void _Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, LPCTSTR lpcszSubFields, bool bDetach, bool bUnique = false );
	void _Select( IQueryFilterPtr& ipFilter, bool bDetach, bool bUnique = false );
	void _Select( _ICursorPtr& ipCursor, bool bDetach, bool bUnique = false );

	/// 子供検索
	void _SelectChild( const std::list<long>& listID, LPCTSTR lpcszKey = _T("OBJECTID"), bool bDetach = false, bool bUnique = true );

	/**
	 * @brief 下り検索（階層）
	 * 
	 * 検索結果は自分自身のコンテナに格納されます。\n
	 * モデル上子供が存在する場合、FindTableしたクラスに検索が
	 * 引き継がれます。\n
	 * 子供の検索が終了した段階で親子関係を構築します。
	 * そのため、検索結果は各コンテナ内及びTableFinderが保持する
	 * 各テーブルクラス内に保持されます（shared_ptrベースなので
	 * 検索結果の実体はそれぞれ一つです）。
	 * @param listID       [in] キーID
	 * @param lpcszKeyName [in,optional] キー名（デフォルト：OBJECTID）
	 * @param bDetach     [in,optional] Rowオブジェクトのインスタンスを切り離す場合はtrue（デフォルト：false）
	 * @param bUnique      [in,optional] 追加検索の場合はtrue（デフォルト：true）
	 */
	void Select( const std::list<long>& listID, LPCTSTR lpcszKey = _T("OBJECTID"), bool bDetach = false, bool bUnique = true );

	/// 上り・下り総合検索
	void Select( const std::map<CString,IQueryFilterPtr>& mapQuery, bool bDetach = false, bool bUnique = true );

	bool exist( const CRowContainer& cRow ) const;
	bool exist( const CTableBase& cTable ) const;

	/**
	 * @brief 中身が空のCSPTableContainerを作る
	 * 
	 * コンテナで一時的に管理したいが、既存のCSPTableContainerには影響を及ぼしたくない場合に
	 * 使用してください。
	 */
	CSPTableContainer CreateEmptyClone() const;

protected:
};

typedef boost::shared_ptr<CTableContainer> CSPTableContainer;

} // sindy
