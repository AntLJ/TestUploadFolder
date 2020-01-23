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
#include "MasterDBCache.h"
#include "sindy/schema/sindyk.h"

using namespace std;

CMasterDBCache::CMasterDBCache(void)
{
}

CMasterDBCache::~CMasterDBCache(void)
{
}

// キャッシュ作成
bool CMasterDBCache::Create( IWorkspacePtr ipWorkspace )
{
	ITablePtr ipMasterTbl;

	// CONTENTS_MASTER テーブル取得
	((IFeatureWorkspacePtr)ipWorkspace)->OpenTable( _bstr_t(m_strContentsMaster), &ipMasterTbl );
	if( ! ipMasterTbl )
	{
		// テーブル名が上手く表示できないので、LPWSTRにキャスト
		wcerr << L"#Error テーブルの取得に失敗 : " << (LPWSTR)(LPCWSTR)m_strContentsMaster << endl;
		return false;
	}

	// フィールドインデックス取得
	long lMasterGrpCodeFID = -1, lMasterChainCodeFID = -1, lMasterContentsCodeFID = -1;

	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kGroupCode), &lMasterGrpCodeFID );
	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kChainCode), &lMasterChainCodeFID );
	ipMasterTbl->FindField( _bstr_t(sindy::schema::sindyk::contents_master::kContentsCode), &lMasterContentsCodeFID );
	if( lMasterGrpCodeFID== -1 || lMasterChainCodeFID == -1 || lMasterContentsCodeFID == -1)
	{
		wcerr << L"#Error フィールドインデックスの取得に失敗 : " << endl;
		return false;
	}

	CString aStr;
	aStr.Format( _T("%s in (0, 1)"), sindy::schema::sindyk::contents_master::kContentsType );

	IQueryFilterPtr ipQuery( CLSID_QueryFilter );
	ipQuery->put_WhereClause( CComBSTR(aStr) );

	long lAllCount = 0;
	ipMasterTbl->RowCount( ipQuery, &lAllCount );

	_ICursorPtr ipMasterCursor;
	ipMasterTbl->Search( ipQuery, VARIANT_FALSE, &ipMasterCursor );
	long lCount = 1;
	_IRowPtr ipMasterRow;
	while( ipMasterCursor && ipMasterCursor->NextRow(&ipMasterRow) == S_OK )
	{
		long lContentsCode = 0;
		POIMASTERDBINFO info;

		CComVariant vaValue;

		// コンテンツコード取得
		ipMasterRow->get_Value( lMasterContentsCodeFID, &vaValue );
		lContentsCode = vaValue.lVal;

		// データ分類コード取得
		ipMasterRow->get_Value( lMasterGrpCodeFID, &vaValue );
		info.first = vaValue.lVal;

		// 店舗コード取得
		ipMasterRow->get_Value( lMasterChainCodeFID, &vaValue );
		info.second = vaValue.lVal;

		// キー：コンテンツコード 値：GRPCODEとCHAINCODEのペア型
		m_mapInfo.insert( pair<long, POIMASTERDBINFO>(lContentsCode, info) );

		// POIポイントを取得する際のWhere句として、コンテンツコードを使用するため、ためておく（Bug9215対応）
		m_setContentsCode.insert( lContentsCode );

		wcerr << lCount++ << L" / " << lAllCount << L" 件取得完了\r";
	}
	wcerr << endl;

	return true;
}
