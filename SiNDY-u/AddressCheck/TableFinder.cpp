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
#include "TableFinder.h"

TableFinder::TableFinder(void)
{
}

TableFinder::~TableFinder(void)
{
}

// ワークスペース取得
IWorkspacePtr TableFinder::getWorkspace( const CString& lpcszKey )
{
	auto itr = m_mapWorkspace.find( lpcszKey );
	if( itr != m_mapWorkspace.end() )
	{
		// PGDBのときにメッシュが変わったときにもワークスペース取得しなおし
		if( !itr->second.m_T )
		{
			itr->second.m_T = gf::getWorkspace( lpcszKey );
		}
		return itr->second.m_T;
	}
	else
		return nullptr;
}

// テーブル検索
ITablePtr TableFinder::findTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable )
{
	CString strKey;
	strKey.Format( _T("%s.%s"), lpcszWorkspaceKey, lpcszTable );	// キーは、[ワークスペースキー.テーブル名]
	strKey.MakeLower();

	auto itr = m_mapTable.find( strKey );
	if( itr != m_mapTable.end() )
	{
		if( !itr->second.m_T )
		{
			itr->second.m_T = openTable( lpcszWorkspaceKey, lpcszTable );	// テーブルまだ取得していない
		}
		return itr->second.m_T;
	}
	else
	{
		// キー自体がないときは、追加してテーブル取得
		ITablePtr ipTable = openTable( lpcszWorkspaceKey, lpcszTable );
		if( ipTable )
		{
			m_mapTable.insert( std::pair< CString, CAdapt<ITablePtr> >(strKey, ipTable) );
		}
		return ipTable;
	}
	return nullptr;
}

//////////////////////////////////////////
//	非公開メンバ関数
//////////////////////////////////////////

// テーブル取得
ITablePtr TableFinder::openTable( const CString& lpcszWorkspaceKey, const CString& lpcszTable )
{
	// キー自体がないときは、追加してテーブル取得
	const IWorkspacePtr& ipWorkspace = getWorkspace( lpcszWorkspaceKey );
	if( ipWorkspace )
	{	
		ITablePtr ipTable;
		IFeatureWorkspacePtr(ipWorkspace)->OpenTable( CComBSTR(lpcszTable), &ipTable );
		return ipTable;
	}
	return nullptr;
}

// ワークスペース追加
bool TableFinder::addWorkspace( const CString& lpcszKey, const CString& lpcszWorkspaceKey )
{
	CAdapt<IWorkspacePtr> obj(nullptr);
	IWorkspacePtr ipWorkspace = gf::getWorkspace( lpcszWorkspaceKey );
	if( !ipWorkspace ) return false;
	m_mapWorkspace.insert( std::pair< CString, CAdapt<IWorkspacePtr> >(lpcszKey, ipWorkspace) );

	return true;
}