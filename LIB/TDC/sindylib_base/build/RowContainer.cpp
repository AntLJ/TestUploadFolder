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
#include "AheLayerFunctions.h"
#include "sindycomutil.h"
#include "GeometryRule.h"
#include "ErrorObject2.h"
#include "RowContainer.h"
#include "TableContainer.h"
#include "ClassFactory.h"

#define foreach BOOST_FOREACH

using namespace sindy;
using namespace sindy::errorcode;

void CRowContainer::InitRelationalTables()
{
	const CModel& model = GetFinder().GetModel();
	// 自分の子供に当たるコンテナを作成（TODO: 子側テーブル名のコピーが無駄だなぁ…）
	std::list<CString> listChild = model.GetChild( GetTableName() );
	foreach( const CString& str, listChild )
	{
		CSPTableContainer spTable( FastTableClassFactory( GetFinder(), str ) );
		if( (ITable*)*spTable )
			push_back( spTable ); // テーブルが見つからない場合は登録しない（bug 9402 #15）
	}
}

CContainerBase& CRowContainer::Query( sindyCompareOperator2 emOperator, LPCTSTR lpcszTableName, CContainerBase& cContainer, bool bSecureUnique/* = true*/ ) const
{
	for( const_iterator it = begin(); it != end(); ++it )
	{
		CSPTableContainer cTable( boost::static_pointer_cast<CTableContainer>(*it) ); // 子供は必ずCTableContainer
		if( cTable )
		{
			CString strFieldName( cTable->GetTableName() );
			switch( emOperator )
			{
			case sindyCompOpeEqual2:            if( strFieldName == lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmall2:        if( strFieldName <  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBig2:          if( strFieldName >  lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanSmallOrEqual2: if( strFieldName <= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeThanBigOrEqual2:   if( strFieldName >= lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			case sindyCompOpeNotEqual2:         if( strFieldName != lpcszTableName ) cContainer.push_back( *it, bSecureUnique ); break;
			default: _ASSERTE( false ); break;
			}
		}
		(*it)->Query( emOperator, lpcszTableName, cContainer, bSecureUnique );
	}
	return cContainer;
}

void CRowContainer::Reset()
{
	CRowBase::Reset();
	CContainerBase::Reset();
}

bool CRowContainer::exist( const CRowContainer& cRow ) const
{
	// 自分自身かどうか
	if( this == &cRow || *this == cRow )
		return true;

	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
	{
		if( static_cast<const CTableContainer&>(*con).exist( cRow ) )
			return true;
	}
	return false;
}
bool CRowContainer::exist( const CTableBase& cTable ) const
{
	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
	{
		if( static_cast<const CTableContainer&>(*con).exist( cTable ) )
			return true;
	}
	return false;
}
// コンテナリストからコンテナを取得する
CSPTableContainer CRowContainer::GetItemTable( LPCTSTR lpcszTableName )
{
	// 必ず存在することが前提。なので、protectメンバにしてある
	foreach( CSPContainerBase& con, std::make_pair( begin(), end() ) )
	{
		CSPTableContainer cTable( boost::static_pointer_cast<CTableContainer>(con) );
		if( 0 == _tcsicmp( cTable->GetTableName(), lpcszTableName ) )
			return cTable;
	}

	return CSPTableContainer();
}
CSPTableContainer CRowContainer::GetItemTable( LPCTSTR lpcszTableName ) const
{
	return const_cast<CRowContainer*>(this)->GetItemTable(lpcszTableName);
}

bool CRowContainer::Changed( schema::ipc_table::update_type::ECode* emUpdateType/* = NULL*/ ) const
{
	bool bChanged = CRowBase::Changed(emUpdateType);
	if( ! bChanged )
		bChanged = CContainerBase::Changed(emUpdateType);
	return bChanged;
}

sindyErrCode CRowContainer::CreateCache( bool bDetach/* = false*/, const CSPFieldMap& spFields/* = CSPFieldMap()*/, const CSPTableNameString& spNameString/* = CSPTableNameString()*/ )
{
	sindyErrCode emErr = CRowBase::CreateCache( bDetach, spFields, spNameString );
	sindyErrCode emErr2 = CContainerBase::CreateCache( bDetach );

	return ( sindyErr_NoErr != emErr ) ? emErr : emErr2;
}

sindyErrCode CRowContainer::CheckLogic( const CLogicRule& cRule, CErrorObjectsBase& cErrs ) const
{
	CRowBase::CheckLogic( cRule, cErrs );
	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
		con->CheckLogic( cRule, cErrs );

	return cErrs.GetDefaultErrCode();
}

// 編集内容を保存する
sindyErrCode CRowContainer::Store( CErrorObjectsBase& cErrs )
{
	CRowBase::Store( cErrs );
	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
		con->Store( cErrs );

	return cErrs.GetDefaultErrCode();
}

void CRowContainer::Delete()
{
	CRowBase::Delete();
	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
		con->Delete();
}

void CRowContainer::UnDelete()
{
	CRowBase::UnDelete();
	foreach( const CSPContainerBase& con, std::make_pair( begin(), end() ) )
		con->UnDelete();
}
