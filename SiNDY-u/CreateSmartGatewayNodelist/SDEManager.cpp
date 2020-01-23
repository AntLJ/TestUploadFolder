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
#include "SDEManager.h"
#include <sindy/workspace.h>

CSDEManager::CSDEManager(void)
{
}

CSDEManager::~CSDEManager(void)
{
}

// 指定バージョンを取得する
bool CSDEManager::ConnectSDE(const LPCTSTR lpszUserName,
							 const LPCTSTR lpszPassword,
							 const LPCTSTR lpszInstance,
							 const LPCTSTR lpszServer,
							 const LPCTSTR lpszVersion,
							 IWorkspacePtr& ppWorkspace)
{
	// OLE2T 等の一部のマクロを使用
	USES_CONVERSION;

	CString connectionInfo;
	connectionInfo.Format(_T("%s/%s/%s/%s"), lpszUserName, lpszPassword, lpszVersion, lpszServer );
	// SDE接続
	ppWorkspace = sindy::create_workspace(connectionInfo);
	if( !ppWorkspace ) {
		return false;
	}
	return true;
}

//ターゲットレイヤのオープン
bool CSDEManager::OpenTargetLayer(const LPCTSTR lpcszLayerName, IWorkspacePtr ipWorkspace, IFeatureClass **ppFeatureClass)
{
	// フィーチャワークスペースの作成
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	// テーブルのオープンに失敗したらfalseを返す
	if( FAILED( ipFeatureWorkspace->OpenFeatureClass(CComBSTR(lpcszLayerName), ppFeatureClass))){
		cerr << "対象レイヤ" << lpcszLayerName << "のオープンに失敗" << endl;
		return false;
	}
	return true;
}

//ターゲットテーブルのオープン
bool CSDEManager::OpenTargetTable(const LPCTSTR lpcszTableName, IWorkspacePtr ipWorkspace, ITable **ppTable)
{
	// フィーチャワークスペースの作成
	IFeatureWorkspacePtr ipFeatureWorkspace(ipWorkspace);
	if( FAILED( ipFeatureWorkspace->OpenTable(CComBSTR(lpcszTableName), ppTable))){
		cerr << "対象テーブル" << lpcszTableName << "のオープンに失敗" << endl;
		return false;
	}
	return true;
}	

//カーソルの取得
bool CSDEManager::GetCursor(ITablePtr ipTable, const LPCTSTR lpszWhereClause, _ICursor **ppCursor)
{
	IQueryFilterPtr		ipFilter(CLSID_QueryFilter);
						ipFilter->put_WhereClause(CComBSTR(lpszWhereClause));

	// テーブルを検索してカーソルに格納
	if( FAILED(ipTable->Search( ipFilter, VARIANT_FALSE, ppCursor ))){
		cerr << "行の検索に失敗" << endl;
		return false;
	}	
	return true;
}

//フィーチャカーソルの取得
bool CSDEManager::GetFeatureCursor(IFeatureClassPtr ipFeatureClass, 
							   const LPCTSTR lpszWhereClause,
							   IFeatureCursor **ppFeatureCursor)
{
	IQueryFilterPtr		ipFilter(CLSID_QueryFilter);
						ipFilter->put_WhereClause(CComBSTR(lpszWhereClause));
	// テーブルを検索してカーソルに格納
	if( FAILED(ipFeatureClass->Search( ipFilter, VARIANT_FALSE, ppFeatureCursor ))){
		cerr << "行の検索に失敗" << endl;
		return false;
	}
	return true;
}

// 与えられたRowの指定されたフィールドの値を取得
CComVariant CSDEManager::GetValue( _IRowPtr ipRow, LPCTSTR lpcszFieldName )
{
	CComVariant vaValue;
	vaValue.vt = VT_EMPTY;

	long lFieldIndex;
	IFieldsPtr ipFields;
	ipRow->get_Fields( &ipFields );

	ipFields->FindField( CComBSTR( lpcszFieldName ), &lFieldIndex );
	if( lFieldIndex >= 0 ){
		ipRow->get_Value( lFieldIndex, &vaValue );
	}
	return vaValue;
}
