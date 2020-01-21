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

#include <tuple>
#include "sindylib_base_global.h"
#include "RowBase.h"
#include "ContainerBase.h"
#include "TableFinderSupport.h"
#include "sindylib_base_classconstructorimpl.h"

namespace sindy {

class CContainerBase;
class CRowContainer;
class CTableBase;
typedef boost::shared_ptr<CContainerBase> CSPContainerBase;
typedef boost::shared_ptr<CRowContainer> CSPRowContainer;

/**
 * @class CRowContainer
 * 
 * このクラスはモデルサポート用レコードクラスのベースとなる
 * クラスです。
 * モデルサポートに特化した特長は以下の通り：
 * @li 子供となるレコード群をテーブル単位でCContainerBaseで一括管理\
 *     今までは各レコードクラスのメンバとして独自に保持していましたが、
 *     それだとオーバーライドミス等によりうまく伝達されないことが多々
 *     ありました。そこで、このクラスでは子供レコードへ階層的に処理される
 *     メソッドは全てオーバーライドして伝達ミスがなくなるようにしています。
 *     各子テーブルコンテナの取得はGetItemTable()関数で行います。
 */
class CRowContainer : 
	public CRowBase,
	public CContainerBase,
	public CTableFinderSupport
{
public:
	SINDYBASE_ROWCLASS_CONSTRUCTOR_BASE( CRowContainer, unknown )
protected:
	void InitRelationalTables();
public:
	//@{ @name CRowBaseメソッドオーバーライド
	virtual bool Changed( schema::ipc_table::update_type::ECode* emUpdateType = NULL ) const;
	virtual errorcode::sindyErrCode CreateCache( bool bDetach = false, const CSPFieldMap& spFields = CSPFieldMap(), const CSPTableNameString& spNameString = CSPTableNameString() );
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const;
	virtual errorcode::sindyErrCode Store( CErrorObjectsBase& cErrs );
	virtual void Delete();
	virtual void UnDelete();
	//@}
	//@{ @name CContainerBaseメソッドオーバーライド
	virtual CContainerBase& Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique = true ) const;
	virtual void Reset();
	//@}
	/// 与えられたものが配下に存在するか確認する
	bool exist( const CRowContainer& cRow ) const;
	bool exist( const CTableBase& cTable ) const;

	/// コンテナリストからコンテナを取得する
	virtual CSPTableContainer GetItemTable( LPCTSTR lpcszTableName );
	virtual CSPTableContainer GetItemTable( LPCTSTR lpcszTableName ) const;
};

#define CAST_ROWC boost::static_pointer_cast<CRowContainer>

} // sindy
