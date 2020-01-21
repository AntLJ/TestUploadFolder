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
#include <boost/foreach.hpp>
#include "GlobalFunctions.h"
#include "TableContainer.h"
#include "RowContainer.h"
#include "ErrorObject2.h"
#include "Model.h"
#include "FieldMap.h"
#include "ClassFactory.h"
#include "TimeTrace.h"

#define foreach BOOST_FOREACH

using namespace sindy;
using namespace sindy::errorcode;

#ifndef SINDY_FOR_ENGLISH
const static _TCHAR FAIL_ITABLE_CREATEROW[]     = _T("ITable::CreateRowに失敗：HRESULT[%X] MSG[%s]");
const static _TCHAR FAIL_CREATE_RECORD[]        = _T("[TABLE:%s] レコード作成に失敗しました:%s");
const static _TCHAR NONEXIST_ASSOCIATE_PARENT[] = _T("CTableContainer::Select() : 関連付ける親オブジェクトが存在しない CHILD TABLE[%s] CHILD OID[%ld] PARENT TABLE[%s] PARENT OID[%ld]\n");
#else
const static _TCHAR FAIL_ITABLE_CREATEROW[]     = _T("ITable::Failed to CreateRow:HRESULT[%X] MSG[%s]");
const static _TCHAR FAIL_CREATE_RECORD[]        = _T("[TABLE:%s] Fail to create a record:%s");
const static _TCHAR NONEXIST_ASSOCIATE_PARENT[] = _T("CTableContainer::Select() : The parent object to associate does not exist. CHILD TABLE[%s] CHILD OID[%ld] PARENT TABLE[%s] PARENT OID[%ld]\n");
#endif	// SINDY_FOR_ENGLISH


bool CTableContainer::LessSPRowContainerOID::operator()( CSPContainerBase& c1, CSPContainerBase& c2 ) const
{
	if( ! ( c1 && c2 ) )
		return false;

	CRowContainer& row1( *boost::static_pointer_cast<CRowContainer>(c1) );
	CRowContainer& row2( *boost::static_pointer_cast<CRowContainer>(c2) );

	return row1.GetOID() < row2.GetOID();
}

void CTableContainer::Reset()
{
	iterator it = begin();
	while( it != end() )
	{
		(*it)->Reset();
		// 削除フラグが残っているならコンテナから除外
		if( boost::static_pointer_cast<CRowContainer>(*it)->IsDeleted() )
			it = erase(it);
		else
			++it;
	}
}

void CTableContainer::push_back( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::push_back()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand検索タイプのものは一時的にfind_by_keyでの検索をやめさせないと無限ループになる
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator it = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == it )
	{
/*#ifdef _DEBUG // 同一のオブジェクトが入っていないかどうかのチェック用
		foreach( CSPContainerBase& con, GetContainer() )
		{
			_ASSERTE( &*cContainer != &*con );
		}
#endif // ifdef _DEBUG*/
		GetContainer().push_back( cContainer );
		it = --rbegin().base();
	}

	add_key( it );
}

void CTableContainer::push_front( const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::push_front()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand検索タイプのものは一時的にfind_by_keyでの検索をやめさせないと無限ループになる
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator it = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == it )
	{
		GetContainer().push_front( cContainer );
		it = begin();
	}

	add_key( it );
}

CTableContainer::iterator CTableContainer::insert( iterator it, const CSPContainerBase& cContainer, bool bSecureUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::insert()"));
	CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(cContainer) );
	// OnDemand検索タイプのものは一時的にfind_by_keyでの検索をやめさせないと無限ループになる
	sindyContainerAutoCacheType emCache = m_emCacheType;
	m_emCacheType = sindyContainerAutoCacheNone;

	iterator itend = end();
	iterator ret = bSecureUnique ? find_by_key( _T("OBJECTID"), row.GetOID() ) : itend;
	m_emCacheType = emCache;
	if( itend == ret )
		ret = GetContainer().insert( it, cContainer );

	add_key( ret );

	return ret;
}

CSPRowContainer CTableContainer::Create()
{
	CSPRowContainer cRow;
	if( ! p )
		return cRow;

	cRow = RowClassFactory( *this, _IRowPtr() ); // 第二引数をNULLにしないとIsCreatedにならない！！
	_IRowPtr ipRow;
	LOGASSERTEM_IF( SUCCEEDED( p->CreateRow( &ipRow ) ), sindyErr_AOCreateRowFailed, FAIL_CREATE_RECORD, GetTableName(), GETCOMERRORSTRING() )
	{
		cRow->SetObject( ipRow, GetTableType(), true ); // ここで第三引数を明示的にfalseにしないとやっぱりIsCreatedにならない！！
		push_back( boost::static_pointer_cast<CContainerBase>(cRow), false );
	}
	return cRow;
}

CSPRowContainer CTableContainer::CreateDummyRow()
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	cRow->CreateCache( false, GetFieldMap(), GetNameString() ); // 実体がないのでキャッシュだけ作っておく
	cRow->SetDefaultValue(); // デフォルト値付与
	push_back( boost::static_pointer_cast<CContainerBase>(cRow), false ); // コンテナに追加

	return cRow;
}

// CreateDummyRowのpush_backしない版
CSPRowContainer CTableContainer::CreateDummyRowNoPush()
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	cRow->CreateCache( false, GetFieldMap(), GetNameString() ); // 実体がないのでキャッシュだけ作っておく
	cRow->SetDefaultValue(); // デフォルト値付与

	return cRow;
}

CSPRowContainer CTableContainer::CreateDummyRow( const CRowContainer& row )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::CreateDummyRow()"));
	CSPRowContainer cRow( RowClassFactory( *this, _IRowPtr() ) );
	row.CopyTo( *cRow, true, false, false );
	push_back( boost::static_pointer_cast<CContainerBase>(cRow), false ); // コンテナに追加

	return cRow;
}

sindyErrCode CTableContainer::Materialization( LPCTSTR lpcszParentName, long lParentOID, CErrorObjectsBase& cErrs )
{
	if( ! p )
		return sindyErr_NoErr;

	// モデルから関連を取得し、どのフィールドに
	// 親IDを入れたらよいのか判断する
	long lFieldIndex = -1;
	if( 0 < lParentOID )
	{
		// 親のIDを付与するフィールドインデックスを取得
		// 多対多の場合でも対応できる（それぞれの親からMaterializationを呼ぶ）
		const CModel& model = GetFinder().GetModel();
		std::list<stModel> listModel = model.GetKeys( GetTableName() );
		foreach( const stModel& model, listModel )
		{
			if( model.ParentName == lpcszParentName )
				lFieldIndex = GetFieldMap()->FindField( model.OuterKey );
		}
	}

	foreach( CSPContainerBase cCon, GetContainer() )
	{
		CSPRowContainer spRow = boost::static_pointer_cast<CRowContainer>(cCon);
		CRowContainer& cRow = *spRow;

		if( ( ! (_IRow*)cRow ) && cRow.IsCreated() && ( ! cRow.IsDeleted() ) )
		{
			_IRowPtr ipRow;
			HRESULT hr = S_OK;
			if( FAILED( hr = p->CreateRow( &ipRow ) ) )
			{
				cErrs.push_back( CSPErrorObjectBase( new CErrorObjectBase( cRow, CSPErrorInfoBase( new CErrorInfoBase( sindyErr_AOCreateRowFailed, sindyErrLevel_ERR, NULL, 0, -1, FAIL_ITABLE_CREATEROW, hr, GETCOMERRORSTRING() ) ) ) ) );
				return sindyErr_AOCreateRowFailed;
			}
#ifdef _DEBUG
			else
				ATLTRACE(_T("CreateRow : [TABLE]%s [OID]%d\n"), GetTableName(), AheGetOID(ipRow) );
#endif // ifdef _DEBUG

			// すげ替え
			cRow.Materialization( ipRow );
		}
		// 親ID付与
		if( 0 < lFieldIndex )
			cRow.SetValue( lFieldIndex, lParentOID );

		// CRowContainerが抱えるコンテナについてもここで適用する
		long lOID = cRow.GetOID();
		LPCTSTR lpcszTableName = cRow.GetTableName();
		foreach( CSPContainerBase cConTable, std::make_pair( cRow.begin(), cRow.end() ) )
		{
			CSPTableContainer spTable = boost::static_pointer_cast<CTableContainer>(cConTable);
			if( sindyErr_NoErr != spTable->Materialization( lpcszTableName, lOID, cErrs ) )
				return cErrs.GetDefaultErrCode();
			// 親子関係取得
			stModel m = GetFinder().GetModel().GetKey( spTable->GetTableName(), GetTableName() );
			// もし子側のキー名がOBJECTIDの場合は子側からIDを取得して親側にセットする必要がある（FEATURE_RELとINFO_POINTの場合など）
			if( m.OuterKey == _T("OBJECTID") )
			{
				CTableContainer::iterator it = spTable->begin();
				if( it != spTable->end() )
					cRow.SetValue( m.ParentKey, boost::static_pointer_cast<CRowContainer>(*it)->GetOID() );
			}
		}
	}

	return cErrs.GetDefaultErrCode();
}

sindyErrCode CTableContainer::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	return CContainerBase::CreateCache( bDetach, spFields, spNameString );
}

sindyErrCode CTableContainer::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrObjects ) const
{
	foreach( const CSPContainerBase& con, GetContainer() )
	{
		CRowContainer& row( *boost::static_pointer_cast<CRowContainer>(con) );
		row.CheckLogic( cRule, cErrObjects );
	}
	return cErrObjects.GetDefaultErrCode();
}

std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszWhere, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey(3)"));
	if( !( ipFilter || lpcszWhere ) )
		return listKey;

	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp = AheInitQueryFilter( NULL, lpcszSubFieldName );
	else
		ipFilterTmp = ipFilter;

	if( lpcszWhere && 0 < _tcslen( lpcszWhere ) )
	{
		CComBSTR bstrWhere;
		if( SUCCEEDED( ipFilterTmp->get_WhereClause( &bstrWhere ) ) )
		{
			CString strWhere;
			if( 0 < bstrWhere.Length() )
				strWhere.Format(_T("%s AND %s"), CString(bstrWhere), lpcszWhere );
			else
				strWhere = lpcszWhere;

			listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( AheInitQueryFilter( ipFilterTmp, lpcszSubFieldName, _T("%s"), strWhere ), true ), lpcszSubFieldName ) );
		}
		else
			_ASSERTE(false);
	}
	else
		listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( ipFilterTmp, true ), lpcszSubFieldName ) );

	listKey.sort();
	listKey.unique();

	return listKey;
}
std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTableName, IQueryFilter* ipFilter, LPCTSTR lpcszSubFieldName, LPCTSTR lpcszAddWhereKeyName, const std::list<long>& listAddWhereKey, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey(2)"));
	std::list<long> listAddWhereKey2;
	if( ( ! ipFilter ) && listAddWhereKey.empty() )
		return listKey;

	IQueryFilterPtr ipFilterTmp;

	if( ! ipFilter )
		ipFilterTmp = AheInitQueryFilter( NULL, lpcszSubFieldName );
	else
		ipFilterTmp = ipFilter;

	if( listAddWhereKey.empty() )
		listKey.merge( Cursor2IDs( FindTable(lpcszTableName)->Search( ipFilterTmp, true ), lpcszSubFieldName ) );
	else {
		// lpcszAddWhereKeyNameがOBJECTIDの場合は、QueryFilterでの検索のみ行い
		// listAddWhereKeyとの積をとる
		if( 0 == _tcsicmp( lpcszAddWhereKeyName, _T("OBJECTID") ) )
		{
			// その際、QueryFilterでの検索が全件になる場合はlistAddWhereKeyのみを採用する
			CComBSTR bstrWhere;
			ipFilterTmp->get_WhereClause( &bstrWhere );
			if( 0 < bstrWhere.Length() )
				listAddWhereKey2 = set_intersection( SelectKey( lpcszTableName, ipFilterTmp, lpcszSubFieldName, NULL ), listAddWhereKey );
			else
				listAddWhereKey2 = listAddWhereKey;
		}
		else
			listAddWhereKey2 = listAddWhereKey;

		std::list<CString> listIDs;
		CTableBase::IDs2Str( listAddWhereKey2, listIDs );
		for( std::list<CString>::const_iterator it = listIDs.begin(); it != listIDs.end(); ++it )
		{
			CString strWhere;
			strWhere.Format(_T("%s IN (%s)"), lpcszAddWhereKeyName, *it );
			listKey.merge( SelectKey( lpcszTableName, ipFilterTmp, lpcszSubFieldName, strWhere ) );
		}
	}

	listKey.sort();
	listKey.unique();
	return listKey;
}

std::list<long>& CTableContainer::SelectKey( LPCTSTR lpcszTopLevelTableName, const std::map<CString,IQueryFilterPtr>& mapQuery, std::list<long>& listKey/* = std::list<long>()*/ ) const
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::SelectKey()"));
	// 末端から順番に並べる
	const CModel& model = GetFinder().GetModel();
	std::list<CString> listTableName( model.GetOrderdNameList(lpcszTopLevelTableName) );
	listTableName.reverse();
	// 親検索用にsetにも入れておく
	std::set<CString> setTableName( listTableName.begin(), listTableName.end() );

	std::map<CString,std::list<long>> mapIDs; // 検索結果取得できたIDが全て入る
	foreach( const CString& name, listTableName )
	{
		// クエリフィルタ取得
		IQueryFilterPtr ipFilter;
		std::map<CString,IQueryFilterPtr>::const_iterator itQuery = mapQuery.find( name );
		if( itQuery != mapQuery.end() )
			ipFilter = itQuery->second;

		// 下で検索されたID取得
		const std::list<long>* plistID = NULL;
		std::map<CString,std::list<long>>::const_iterator itIDs = mapIDs.find( name );
		if( itIDs != mapIDs.end() )
			plistID = &itIDs->second;

		if( lpcszTopLevelTableName == name )
		{
			// トップレベルの場合
			if( ipFilter )
				SelectKey( name, ipFilter, _T("OBJECTID"), NULL, std::list<long>(), mapIDs[name] );
		}
		else {
			// 親のIDを検索
			std::list<stModel> listParent( model.GetKeys( name ) );
			foreach( const stModel& m, listParent )
			{
				if( setTableName.end() == setTableName.find(m.ParentName) )
					continue;

				if( ipFilter || ( plistID && ( ! (*plistID).empty() ) ) )
					SelectKey( name, ipFilter, m.OuterKey, m.ParentKey, plistID ? *plistID : std::list<long>(), mapIDs[m.ParentName] );
			}
		}
	}
	listKey = mapIDs[lpcszTopLevelTableName];

	// ユニーク処理
	listKey.sort();
	listKey.unique();

	return listKey;
}

// 下り検索
void CTableContainer::_Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, bool bDetach, bool bUnique/* = false*/ )
{
	_Select( listID, lpcszKeyName, _T("*"), bDetach, bUnique );
}

void CTableContainer::_Select( const std::list<long>& listID, LPCTSTR lpcszKeyName, LPCTSTR lpcszSubFields, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(listID)"));
	if( listID.empty() )
		return;

	std::list<CString> listIDs;
//	if( 0 == lstrcmpi( lpcszKeyName, _T("OBJECTID") ) && 1000 < listID.size() )
//		_Select( GetRows( listID, bDetach ), bDetach, bUnique );
//	else {
		IDs2Str( listID, listIDs );
		foreach( const CString& ids, listIDs )
			_Select( Search( AheInitQueryFilter( NULL, lpcszSubFields, _T("%s in ( %s )"), lpcszKeyName, ids ), bDetach ), bDetach, bUnique );
//	} <- GetRowsは遅すぎるので封印（POI_POINT 1000件 を GetRows … 2分 IN … 8秒 まあ、状況によるのかもしれないけど。）
}

void CTableContainer::_Select( IQueryFilterPtr& ipFilter, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(ipFilter)"));
	_Select( Search( ipFilter, bDetach ), bDetach, bUnique );
}

void CTableContainer::_Select( _ICursorPtr& ipCursor, bool bDetach, bool bUnique/* = false*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::_Select(ipCursor)"));
	if( ! ipCursor )
	{
		_ASSERT_EXPR( ! ipCursor, _T("検索に失敗") );
		return;
	}
	_IRowPtr ipRow;
	while( S_OK == ipCursor->NextRow( &ipRow ) )
	{
		if( ! ipRow )
			break;

		CSPRowContainer spRow( RowClassFactory( *this, ipRow ) );
		if( bDetach )
			spRow->CreateCache( true ); // キャッシュ作成
		push_back( spRow, bUnique );
	}
}

// 子供検索
void CTableContainer::_SelectChild( const std::list<long>& listID, LPCTSTR lpcszKey/* = _T("OBJECTID")*/, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
	const CModel& model = GetFinder().GetModel();

	// 子供を検索
	std::list<stModel> listChildModel = model.GetKeysByParentName( GetTableName() );
	foreach( const stModel& m, listChildModel )
	{
		// IDと実体の対応表作成（関連付けの為）
		// トップレベルのコンテナは使用せず、一時コンテナを作成する
		CSPTableContainer spChildTable( FindTable( m.Name )->CreateEmptyClone() );
		if( ! (ITable*)*spChildTable )
			continue; // テーブルが存在しない場合は無視（bug 9402 #15）

		// 親側のキーを取得する
		std::list<long> listKey;
		if( lpcszKey == m.ParentKey )
			listKey = listID;
		else {
			foreach( CSPContainerBase& con, GetContainer() )
				listKey.push_back( boost::static_pointer_cast<CRowContainer>(con)->GetValue( m.ParentKey, -1L ) );
		}
		// 子供の検索
		spChildTable->Select( listKey, m.OuterKey, bDetach, bUnique );
		// 親子の関連付け
		CTableContainer::iterator itEnd = end();
		foreach( CSPContainerBase spCon, std::make_pair( spChildTable->begin(), spChildTable->end() ) )
		{
			CSPRowContainer spRow = boost::static_pointer_cast<CRowContainer>(spCon);
			if( spRow->IsDeleted() )
				continue;
			// CIndexedContainerのインデックスを使用して紐付け
			CTableContainer::IteratorContainer its = finds_by_key( m.ParentKey, spRow->GetValue( m.OuterKey, -1L ) );
			if (! its.empty())
				foreach( CTableContainer::iterator it, its )
				{
					boost::static_pointer_cast<CRowContainer>(*it)->GetItemTable( m.Name )->push_back( spRow, bUnique );
				}
			else {
				_ASSERT_EXPR( ! its.empty(), _T("ERROR: 関連付ける親オブジェクトが存在しない") );
				TRACEMESSAGE( NONEXIST_ASSOCIATE_PARENT,
					spRow->GetTableName(), spRow->GetOID(), m.ParentKey, spRow->GetValue( m.OuterKey, -1L ) );
			}
		}
	}
}

// 下り検索
void CTableContainer::Select( const std::list<long>& listID, LPCTSTR lpcszKey/* = _T("OBJECTID")*/, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
//	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::Select(listID)"));
	CTimeTrace trace;
	trace.Start(_T("CTableContainer::Select()") + CString(GetTableName()) );
	// 全ての検索結果はTableFinderの中にもセットされる
	// この辺の仕様はSiNDY-iとは異なる
	// ただし、関連付けが終わった後ならFinder内にある
	// レコードはクリアしても問題ない。
	// （トップレベルテーブルがFinder内にないなら、の話）
	const CModel& model = GetFinder().GetModel();

	trace.Start(GetTableName());
	_Select( listID, lpcszKey, bDetach, bUnique );
	trace.End(GetTableName());

	_SelectChild( listID, lpcszKey, bDetach, bUnique );

	trace.End(_T("CTableContainer::Select()") + CString(GetTableName()) );
	trace.Trace();
}

// 上り・下り総合検索
void CTableContainer::Select( const std::map<CString,IQueryFilterPtr>& mapQuery, bool bDetach/* = false*/, bool bUnique/* = true*/ )
{
	CFuncTraceWrapper ft(g_trace,_T("CTableContainer::Select(mapQuery)"));
	if( mapQuery.size() == 1 && mapQuery.begin()->first == GetTableName() )
	{
		_Select( Search( mapQuery.begin()->second, bDetach ), bDetach, bUnique );
		std::list<long> listKey;
		foreach( CSPContainerBase& con, GetContainer() )
			listKey.push_back( boost::static_pointer_cast<CRowContainer>(con)->GetOID() );
		_SelectChild( listKey, _T("OBJECTID"), bDetach, bUnique );
	}
	else
		Select( SelectKey( GetTableName(), mapQuery ), _T("OBJECTID"), bDetach, bUnique );
}

bool CTableContainer::exist( const CRowContainer& cRow ) const
{
	foreach( const CSPContainerBase& con, GetContainer() )
	{
		if( static_cast<const CRowContainer&>(*con).exist( cRow ) )
			return true;
	}
	return false;
}
bool CTableContainer::exist( const CTableBase& cTable ) const
{
	if( this == &cTable )
		return true;

	foreach( const CSPContainerBase& con, GetContainer() )
	{
		if( static_cast<const CRowContainer&>(*con).exist( cTable ) )
			return true;
	}
	return false;
}

// 中身が空のコンテナを作る
CSPTableContainer CTableContainer::CreateEmptyClone() const
{
	return boost::static_pointer_cast<CTableContainer>(TableClassFactory( GetFinder(), ITablePtr(p) ) );
}
