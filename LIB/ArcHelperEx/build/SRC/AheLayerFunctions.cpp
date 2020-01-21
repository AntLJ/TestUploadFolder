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
 * @file LayerFunctions.cpp
 * @brief <b>ArcObjectsグローバルヘルパー関数実装ファイル</b>\n
 * このファイルには、主にレイヤ系のグローバル関数が実装されています。
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#include "stdafx.h"
#include "AheLayerFunctions.h"
#include "AheGlobals.h"
#include "Map.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//namespace sindy {

// テーブル名が同じかどうかをチェックする
bool _AheIsSameTableName( LPCTSTR lpcszTableName1, LPCTSTR lpcszTableName2 )
{
	// どちらかがオーナー名抜きの場合はオーナー名を評価対象から外す
	LPCTSTR lpcszTableName1Tmp = _tcsstr( lpcszTableName1, _T(".") );
	LPCTSTR lpcszTableName2Tmp = _tcsstr( lpcszTableName2, _T(".") );
	if( NULL == lpcszTableName2Tmp || NULL == lpcszTableName1Tmp )
	{
		lpcszTableName1Tmp = ( NULL != lpcszTableName1Tmp ) ? lpcszTableName1Tmp + 1 : lpcszTableName1;
		lpcszTableName2Tmp = ( NULL != lpcszTableName2Tmp ) ? lpcszTableName2Tmp + 1 : lpcszTableName2;
	}

	return ( lstrcmpi( lpcszTableName1Tmp, lpcszTableName2Tmp ) == 0 ) ? true : false;
}

// レイヤからテーブルを取得する
ITablePtr AheLayer2Table( ILayer* ipLayer )
{
	if( ipLayer != NULL )
	{
		IFeatureLayerPtr ipFeatureLayer( ipLayer );
		if( ipFeatureLayer != NULL )
		{
			IFeatureClassPtr ipFeatureClass;
			ipFeatureLayer->get_FeatureClass( &ipFeatureClass );
			if( ipFeatureClass != NULL )
				return ipFeatureClass;
		}
	}
	return NULL;
}

// テーブルが編集可能かどうかをチェックする
BOOL AheIsEditableTable( ITable* ipTable )
{
	if( ! ipTable )
		return FALSE;

	// 編集可能かどうか
	VARIANT_BOOL vbCanEdit = VARIANT_FALSE;
	IDatasetEditInfoPtr ipDatasetEditInfo( ipTable );
	if( ipDatasetEditInfo )
		ipDatasetEditInfo->get_CanEdit( &vbCanEdit );
	if( ! vbCanEdit )
		return FALSE;

	// 編集開始されているかどうか
	VARIANT_BOOL vbBeingEdited = VARIANT_FALSE;
	IDatasetEditPtr ipDatasetEdit( ipTable );
	if( ipDatasetEdit )
		ipDatasetEdit->IsBeingEdited( &vbBeingEdited );
	if( ! vbBeingEdited )
		return FALSE;

	// 接続ユーザ名とテーブルオーナー名が異なる場合は
	// 編集可能とみなさない（SiNDY全般のルール）
	CString strConnUserName( AheGetConnectUserName( ipTable ) ); // 接続ユーザ名
	if( 0 != AheGetFeatureClassOwnerName( ipTable ).CompareNoCase( AheGetConnectUserName( ipTable ) ) )
		return FALSE;

	return TRUE;
}

// テーブルが同じ物かどうかをチェックする
bool /*SINDYLIB_API*/ AheIsEqualTable( ITable* ipTable1, ITable* ipTable2 )
{
	return ( AheGetOwnerTableName( ipTable1 ).Compare( AheGetOwnerTableName( ipTable2 ) ) == 0 ) ? true : false;
}

//////////////////////////////////////////////////////////////////////
//
// レイヤ名・テーブル名取得に関する関数
//
//////////////////////////////////////////////////////////////////////
CString AheGetConnectUserName( IUnknownPtr ipUnk )
{
	IDatabaseConnectionInfoPtr ipConn( AheGetWorkspace( ipUnk ) );
	if( ! ipConn )
		return CString(_T(""));

	CComBSTR bstrUser;
	ipConn->get_ConnectedUser( &bstrUser );

	return CString(bstrUser);
}

CString AheGetOwnerDotClassName( IUnknownPtr ipUnk )
{
	IDatasetPtr ipDataset( AheGetTable( ipUnk ) );
	BSTR bstrName = 0;

	if( ipDataset == NULL ) return CString(_T(""));

	ipDataset->get_Name( &bstrName );
	CString strLayerName( bstrName );
	::SysFreeString(bstrName);

	// 大文字限定
	strLayerName.MakeUpper();

	return strLayerName;
}

CString AheGetFeatureClassName( IUnknownPtr ipUnk )
{
	CString strLayerName = AheGetOwnerDotClassName( ipUnk );

	INT iPos = strLayerName.ReverseFind('.');
	if( iPos >= 0 ) strLayerName = strLayerName.Right( strLayerName.GetLength() - iPos - 1 );

	return strLayerName;
}

CString AheGetFeatureClassOwnerName( IUnknownPtr ipUnk )
{
	CString strOwnerName = AheGetOwnerDotClassName( ipUnk );

	INT iPos = strOwnerName.ReverseFind('.');
	if( iPos > 0 ) 
		strOwnerName = strOwnerName.Left( iPos );
	else
		strOwnerName = _T("");

	return strOwnerName;
}

BOOL AheGetLayerTableName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName )
{
	if( ipUnk == NULL || bstrLayerName == NULL ) return FALSE;

	CComBSTR bstrLayerName2 = AheGetFeatureClassName( ipUnk );
	(*bstrLayerName) = bstrLayerName2.Detach();

	return TRUE;
}

BOOL AheGetLayerAliasName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrLayerName )
{
	IFeaturePtr ipFeature( ipUnk );
	IFeatureClassPtr ipFeatureClass = NULL;

	// ipUnk == IFeature の時はショートカット
	if( ipFeature != NULL ) {
		IObjectClassPtr ipObjectClass = NULL;
		ipObjectClass->get_AliasName( bstrLayerName );

		return TRUE;
	} 
	else if( AheGetFeatureClass( piMap, ipUnk, &ipFeatureClass ) == TRUE ) {
		ipFeatureClass->get_AliasName( bstrLayerName );
		
		return TRUE;
	}

	return FALSE;
}

CString AheGetLayerAliasName( IUnknownPtr ipUnk )
{
	CComBSTR bstrLayerName = 0;

	IFeatureClassPtr ipFeatureClass = AheGetFeatureClass( ipUnk );
	if( ipFeatureClass != NULL )
		ipFeatureClass->get_AliasName( &bstrLayerName );
		
	return CString( bstrLayerName );
}

BOOL AheGetLayerMXDName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrMXDName )
{
	IFeaturePtr ipFeature( ipUnk );
	IFeatureClassPtr ipFeatureClass( ipUnk );
	ILayerPtr ipLayer( ipUnk );

	BOOL bIsGet = FALSE;
	if( ipFeature != NULL || ipFeatureClass != NULL ) {
		CComBSTR bstrName;
		USES_CONVERSION;

		// 取得したテーブル名取得
		IDatasetPtr ipDataset;
		if( ipFeature != NULL )
		{
			ITablePtr ipTable;
			ipFeature->get_Table( &ipTable );
			ipDataset = ipTable;
		}
		else if( ipFeatureClass != NULL )
		{
			IFeatureLayerPtr ipFeatureLayer;
			ipFeatureLayer = ipFeatureClass;
			if( ipFeatureLayer != NULL )
			{
				IFeatureClassPtr ipClass;
				ipFeatureLayer->get_FeatureClass( &ipClass );
				ipDataset = ipClass;
			}
		}

		if( ipDataset == NULL )
			return FALSE;

		ipDataset->get_Name( &bstrName );

		CMap cLayers( piMap );
		for( CMap::const_iterator it = cLayers.begin(); it != cLayers.end(); ++it )
		{
			// [bug 1765] 赤びっくりなレイヤでget_Workspaceすると落ちる（未報告）ので
			// 調査は見えるレイヤに限定
			VARIANT_BOOL vaValid = VARIANT_FALSE;
			it->get_Valid( &vaValid );
			if( vaValid )
			{
				IFeatureClassPtr ipSelectFeatureClass;
				IFeatureLayerPtr ipFeatureLayer;
				ipFeatureLayer = *it;
				if( ipFeatureLayer != NULL )
					ipFeatureLayer->get_FeatureClass( &ipSelectFeatureClass );

				IDatasetPtr	ipSelectDataset;
				ipSelectDataset = ipSelectFeatureClass;

				if( ipSelectDataset == NULL )
					continue;

				CComBSTR bstrLayerName;
				ipSelectDataset->get_Name( &bstrLayerName );

				if( lstrcmpiW( bstrName, bstrLayerName ) == 0 )
					return SUCCEEDED( it->get_Name( bstrMXDName ) );
			}
		}
	}
	else
		return SUCCEEDED( ipLayer->get_Name( bstrMXDName ));

	return FALSE;
}

CString AheGetLayerMXDName( IMapPtr ipMap, IUnknownPtr ipUnk )
{
	CComBSTR bstrLayerName = 0;

	ILayerPtr ipLayer( ipUnk );
	if( ipLayer == NULL )
		ipLayer	= AheGetLayerByTableName( ipMap, AheGetFeatureClassName( ipUnk ) );

	if( ipLayer != NULL )
		ipLayer->get_Name( &bstrLayerName );

	return CString( bstrLayerName );
}

BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, BSTR* bstrName, FINDTYPE type )
{
	switch( type ) {
		case _FIND_TABLE: // テーブル名を取得
			return AheGetLayerTableName( piMap, ipUnk, bstrName );
			break;
		case _FIND_ALIAS: // エイリアス名を取得
			return AheGetLayerAliasName( piMap, ipUnk, bstrName );
			break;
		case _FIND_MXDNAME: // MXDファイルに保存されているレイヤ名を取得
			return AheGetLayerMXDName( piMap, ipUnk, bstrName );
			break;
		default:
			return FALSE;
			break;
	}

	return FALSE;
}

BOOL AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, LPCTSTR& lpszName, FINDTYPE type )
{
	BSTR bstrName = NULL;

	USES_CONVERSION;

	if( AheGetLayerName( piMap, ipUnk, &bstrName, type ) == FALSE ) return FALSE;
	lpszName = OLE2T( bstrName );

	return TRUE;
}

CString AheGetLayerName( IMapPtr piMap, IUnknownPtr ipUnk, FINDTYPE type )
{
	BSTR bstrName = NULL;

	USES_CONVERSION;

	if( AheGetLayerName( piMap, ipUnk, &bstrName, type ) == FALSE ) return CString(_T(""));

	return CString(OLE2T( bstrName ));
}

//////////////////////////////////////////////////////////////////////
//
// レイヤ・テーブル・フィーチャクラス取得に関する関数
//
//////////////////////////////////////////////////////////////////////
ILayerPtr AheGetLayerByTableName( IMapPtr ipMap, LPCTSTR lpcszLayerName, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetLayerByName( ipMap, lpcszLayerName, _FIND_TABLE, bEditableOnly );
}
BOOL AheGetLayerByTableName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, BOOL bEditableOnly/* = FALSE*/ )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_TABLE, bEditableOnly );
}

BOOL AheGetLayerByAliasName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_ALIAS);
}

ILayerPtr AheGetLayerByAliasName( IMapPtr ipMap, LPCTSTR lpszLayerName )
{
	return AheGetLayerByName( ipMap, lpszLayerName, _FIND_ALIAS );
}

BOOL AheGetLayerByMXDName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer )
{
	return AheGetLayerByName( piMap, lpszLayerName, ipRetLayer, _FIND_MXDNAME);
}

ILayerPtr AheGetLayerByMXDName( IMapPtr ipMap, LPCTSTR lpszLayerName )
{
	return AheGetLayerByName( ipMap, lpszLayerName, _FIND_MXDNAME );
}

BOOL AheGetLayerByName( IMapPtr piMap, LPCTSTR lpszLayerName, ILayer** ipRetLayer, FINDTYPE type, BOOL bEditableOnly/* = FALSE*/, BOOL bFullName/* = FALSE*/ )
{
	ILayerPtr ipLayer = AheGetLayerByName( piMap, lpszLayerName, type, bEditableOnly, bFullName );
	if( ipLayer != NULL )
	{
		(*ipRetLayer) = ipLayer;
		(*ipRetLayer)->AddRef();

		return TRUE;
	}

	return FALSE;
}

ILayerPtr AheGetLayerByName( IMapPtr ipMap, LPCTSTR lpszLayerName, FINDTYPE type/* = _FIND_TABLE*/, BOOL bEditableOnly/* = FALSE*/, BOOL bFullName/* = FALSE*/ )
{
	if( ipMap == NULL || lpszLayerName == NULL )
		return NULL;

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( bEditableOnly ? LayerTypeFeatureLayer : LayerTypeAny, bEditableOnly ? true : false ); it != cLayers.end(); ++it )
	{
		// [bug 1765] 赤びっくりなレイヤでget_Workspaceすると落ちる（未報告）ので
		// 調査は見えるレイヤに限定
		VARIANT_BOOL vaValid = VARIANT_FALSE;
		it->get_Valid( &vaValid );
		if( vaValid )
		{
			CString strLayerName;
			switch( type ) 
			{
			case _FIND_TABLE:	//!< レイヤのテーブル名で検索したい場合
				strLayerName = ( bFullName ) ? AheGetOwnerDotClassName( *it ) : AheGetFeatureClassName( *it );
				break;
			case _FIND_ALIAS:	//!< レイヤのエイリアス名で検索したい場合
				strLayerName = AheGetLayerAliasName( *it );
				break;
			case _FIND_MXDNAME:	//!< MXDファイルに保存されているレイヤ名で検索したい場合
				strLayerName = AheGetLayerMXDName( ipMap, *it );
				break;
			default:
				return NULL;
				break;
			}
			if( strLayerName.CompareNoCase( lpszLayerName ) == 0 ) 
				return *it;
		}
	}

	return NULL;
}

bool _AheGetSelectedLayer( ICompositeLayerPtr ipCompositeLayer, ILayer** ipSelectLayer )
{
	// 引数チェック
	if( ( ! ipCompositeLayer ) || ( ! ipSelectLayer ) )
		return false;

	// 返り値初期化
	*ipSelectLayer = NULL;

	ILayerPtr ipRet; // 見つかったレイヤ
	long lEditLayerNum = 0; // 見つかったレイヤ数
	long lCompositeLayerNum = 0; // グループレイヤ内レイヤ数

	if( SUCCEEDED( ipCompositeLayer->get_Count( &lCompositeLayerNum ) ) )
	{
		for( long i = 0; i < lCompositeLayerNum; ++i )
		{
			ILayerPtr ipLayer;
			if( SUCCEEDED( ipCompositeLayer->get_Layer( i, &ipLayer ) ) && ipLayer )
			{
				// フィーチャレイヤでなかったら無視
				// もしグループレイヤがあったとしても無視してしまう。
				IFeatureLayerPtr ipFeatureLayer( ipLayer );
				if( ipFeatureLayer )
				{
					// フィーチャクラスとれなかったりワークスペースとれなかったら無視
					IFeatureClassPtr ipFeatureClass;
					if( SUCCEEDED( ipFeatureLayer->get_FeatureClass( &ipFeatureClass ) ) && ipFeatureClass )
					{
						IDatasetPtr ipDataset( ipFeatureClass );
						if( ipDataset )
						{
							IWorkspacePtr ipWorkspace( AheGetWorkspace( ipDataset ) );
							if( ipWorkspace )
							{
								// 編集開始されているワークスペースかどうか調べる
								VARIANT_BOOL vbIsBeginingEdit = VARIANT_FALSE;
								if( SUCCEEDED( ((IWorkspaceEditPtr)ipWorkspace)->IsBeingEdited( &vbIsBeginingEdit ) ) && vbIsBeginingEdit )
								{
									// 編集することができるデータセットか調べる
									VARIANT_BOOL vbIsCanEdit = VARIANT_FALSE;
									IDatasetEditInfoPtr ipDatasetEditInfo( ipDataset );
									if( ipDatasetEditInfo )
									{
										if( SUCCEEDED( ipDatasetEditInfo->get_CanEdit( &vbIsCanEdit ) ) && vbIsCanEdit )
										{
											// 編集開始されているワークスペースかつ、編集可能レイヤで
											// ある場合にのみ編集ができることにする
											if( vbIsBeginingEdit && vbIsCanEdit )
											{
												// 同じソースのレイヤだったら、複数見つけたことにしない（bug 5170）
												bool bFindSameSourceLayer = false;
												if( 0 < lEditLayerNum )
												{
													IFeatureClassPtr ipFeatureClassTmp;
													((IFeatureLayerPtr)ipRet)->get_FeatureClass( &ipFeatureClassTmp );
													if( (IUnknown*)ipFeatureClass == (IUnknown*)ipFeatureClassTmp )
														bFindSameSourceLayer = true;
												}
												if( ! bFindSameSourceLayer )
												{
													++lEditLayerNum;
													ipRet = ipLayer;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			// 複数ある場合には中断
			if( 1 < lEditLayerNum )
				break;
		}
	}
	// 該当するものが一つのみの場合のみレイヤを返す
	if( 1 == lEditLayerNum )
	{
		*ipSelectLayer = ipRet;
		if( *ipSelectLayer )
			(*ipSelectLayer)->AddRef();
	}
	
	return ( 1 == lEditLayerNum ) ? true : false;
}

BOOL AheGetSelectedLayer( IMxDocumentPtr piMxDoc, ILayer** ipRetLayer, bool bIsEditable /* = true */)
{
	IContentsViewPtr piContentsView;
	if( SUCCEEDED( piMxDoc->get_ContentsView(0, &piContentsView)))
	{
		CComVariant vTmp;
		if( SUCCEEDED( piContentsView->get_SelectedItem( &vTmp)))
		{
			if( VT_UNKNOWN == vTmp.vt)
			{
				ILayerPtr ipLayer;
				if( SUCCEEDED( vTmp.punkVal->QueryInterface(&ipLayer)))
				{
					if( bIsEditable )
					{
						// グループレイヤ選択されている場合には、そのなかの編集開始されているレイヤを取得してくる
						// そのグループレイヤの中に複数の編集可能レイヤがある場合には、選択しているグループレイヤipLayerを返すことにします。
						ICompositeLayerPtr ipCompositeLayer( ipLayer );
						if( ipCompositeLayer != NULL )
							_AheGetSelectedLayer( ipCompositeLayer, ipRetLayer );
					}

					if( *ipRetLayer == NULL )
					{
						( *ipRetLayer) = ipLayer;
						( *ipRetLayer)->AddRef();
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

// AheGetTableByTableName、_AhGetTableByTableNameFromCompositeLayer共通で使用するサブルーチン（非公開）
void _AheGetTableByTableName( ITablePtr& ipTable, ITable* ipTableTmp, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly )
{
	// 引数チェック
	if( ipTableTmp == NULL ) return;

	if( _AheIsSameTableName( AheGetOwnerTableName( ipTableTmp ), lpcszOwnerTableName ) )
	{
		// 編集可能かどうかをチェック
		if( ! bEditableOnly || AheIsEditableTable( ipTableTmp ) )
		{
			if( ipTable == NULL )
			{
				ipTable = ipTableTmp;
				++nFindCount;
			}
			else {
				// 既にある場合は同じかどうかチェック
				if( ! AheIsEqualTable( ipTable, ipTableTmp ) )
					++nFindCount;	// カウンタのみ増加、返すテーブルは最初に見つけたもののみ
			}
		}
	}
}

// グループレイヤの中からテーブルを取得する（非公開）
ITablePtr _AhGetTableByTableNameFromCompositeLayer( ICompositeLayer* ipCompLayer, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly = true )
{
	// 初期化
	nFindCount = 0;

	ITablePtr ipTable;
	if( ipCompLayer != NULL && lpcszOwnerTableName != NULL )
	{
		long lCount = 0;
		ipCompLayer->get_Count( &lCount );
		for( long i = 0; i < lCount; ++i )
		{
			ILayerPtr ipLayer;
			VARIANT_BOOL vbIsValid = VARIANT_FALSE;

			ipCompLayer->get_Layer( i, &ipLayer );
			if( ipLayer != NULL )
			{
				// [bug 1765] 赤びっくりなレイヤでget_Workspaceすると落ちる（未報告）ので
				// 調査は見えるレイヤに限定
				ipLayer->get_Valid( &vbIsValid );
				if( vbIsValid )
				{
					if( (ICompositeLayerPtr)ipLayer != NULL )
					{
						int nFindCountTmp = 0;
						ITablePtr ipTableTmp = _AhGetTableByTableNameFromCompositeLayer( (ICompositeLayerPtr)ipLayer, lpcszOwnerTableName, nFindCountTmp, bEditableOnly );
						if( ipTable == NULL )
						{
							ipTable = ipTableTmp;
							nFindCount = nFindCountTmp;
						}
						else if( ! AheIsEqualTable( ipTable, ipTableTmp ) )
							nFindCount += nFindCountTmp;
					}
					else {
						if( AheIsEqualTable( ipTable, AheLayer2Table( ipLayer ) ) )
							++nFindCount;
					}

					// 共通サブルーチン
					_AheGetTableByTableName( ipTable, AheLayer2Table( ipLayer ), lpcszOwnerTableName, nFindCount, bEditableOnly );
				}
			}
		}
	}
	else
		_ASSERTE( ipCompLayer != NULL && lpcszOwnerTableName != NULL );
		
	return ipTable;
}

// データフレームに読み込まれているテーブル・フィーチャクラスを取得する
ITablePtr AheGetTableByTableName2( IMap* ipMap, LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly/* = true*/ )
{
	// 初期化
	nFindCount = 0;
	ITablePtr ipTable;

	if( ipMap != NULL && lpcszOwnerTableName != NULL )
	{
		long lCount = 0;
		ipMap->get_LayerCount( &lCount );
		for( long i = 0; i < lCount; ++i )
		{
			ILayerPtr ipLayer;
			VARIANT_BOOL vbIsValid = VARIANT_FALSE;

			ipMap->get_Layer( i, &ipLayer );
			if( ipLayer != NULL )
			{
				// [bug 1765] 赤びっくりなレイヤでget_Workspaceすると落ちる（未報告）ので
				// 調査は見えるレイヤに限定
				ipLayer->get_Valid( &vbIsValid );
				if( vbIsValid )
				{
					if( (ICompositeLayerPtr)ipLayer != NULL )
					{
						int nFindCountTmp = 0;
						ITablePtr ipTableTmp = _AhGetTableByTableNameFromCompositeLayer( (ICompositeLayerPtr)ipLayer, lpcszOwnerTableName, nFindCountTmp, bEditableOnly );
					}
					else
						// 共通サブルーチン
						_AheGetTableByTableName( ipTable, AheLayer2Table(ipLayer), lpcszOwnerTableName, nFindCount, bEditableOnly );
				}
			}
		}
	}
	else
		_ASSERTE( ipMap != NULL && lpcszOwnerTableName != NULL );

	return ipTable;
}

BOOL AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, ITable** ipRetTable, BOOL bReference/* = FALSE*/, BOOL bEditableOnly /* = FALSE */ )
{
	if( ipMap == NULL || lpszTableName == NULL || ipRetTable == NULL ) return FALSE;

	ITablePtr ipTable = AheGetTableByTableName( ipMap, lpszTableName, bReference, bEditableOnly );
	if( ipTable != NULL )
	{
		*ipRetTable = ipTable;
		(*ipRetTable)->AddRef();

		return TRUE;
	}

	return FALSE;
}
BOOL _AheIsAlreadyCheckedWorkspace( IWorkspacePtr ipWorkspace, const std::list<CString>& listCheck, CString& strDBString )
{
	USES_CONVERSION;

	if( ipWorkspace == NULL ) return TRUE;
	if( listCheck.size() < 1 ) return FALSE;

	CComBSTR bstrWorkspacePathName;

	// 既に調べたワークスペースかどうかチェック
	ipWorkspace->get_PathName( &bstrWorkspacePathName );			// SDE じゃない場合
	if( bstrWorkspacePathName.Length() == 0 )
	{
		IPropertySetPtr ipConnProp;
		ipWorkspace->get_ConnectionProperties( &ipConnProp );
		if( ipConnProp != NULL )
		{
			// コネクションプロパティでサーバ名があるものを採用
			CComVariant vaServer, vaUser, vaVersion;
			ipConnProp->GetProperty( CComBSTR(_T("SERVER")), &vaServer );
			ipConnProp->GetProperty( CComBSTR(_T("USER")), &vaUser );
			ipConnProp->GetProperty( CComBSTR(_T("VERSION")), &vaVersion );

			if( vaServer.vt != VT_BSTR ) { vaServer.vt = VT_BSTR; vaServer.bstrVal = ::SysAllocString( L"NODEFINE" ); }
			if( vaUser.vt != VT_BSTR) {  vaUser.vt = VT_BSTR; vaUser.bstrVal = ::SysAllocString( L"NODEFINE" ); }
			if( vaVersion.vt != VT_BSTR) {  vaVersion.vt = VT_BSTR; vaVersion.bstrVal = ::SysAllocString( L"NODEFINE" ); }

			strDBString.Format(_T("%s:%s:%s"), OLE2T(vaServer.bstrVal), OLE2T(vaUser.bstrVal), OLE2T(vaVersion.bstrVal) );
		}
	}
	else
		strDBString = bstrWorkspacePathName;

	if( ! strDBString.IsEmpty() )
	{
		BOOL bAlreadyCheckedWorkspace = FALSE;
		for( std::list<CString>::const_iterator it = listCheck.begin(); it != listCheck.end(); it++ )
		{
			if( it->CompareNoCase( strDBString ) == 0 )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

ITablePtr AheGetTableByTableName( IMapPtr ipMap, LPCTSTR lpszTableName, BOOL bReference/* = FALSE*/, BOOL bEditableOnly/* = FALSE */ )
{
	if( ipMap == NULL || lpszTableName == NULL )
		return NULL;

	ITablePtr ipTable;
	std::list<CString> listWorkspacePathName;

	CMap cLayers( ipMap );
	for( CMap::const_iterator it = cLayers.begin( bEditableOnly ? LayerTypeFeatureLayer : LayerTypeAny, bEditableOnly ? true : false ); it != cLayers.end(); ++it )
	{
		if( AheGetFeatureClassName( *it ).CompareNoCase( lpszTableName ) != 0 )
		{
			IWorkspacePtr ipWorkspace( AheGetWorkspace( *it ) );

			if( ! ipWorkspace )
				continue;

			
			// 既に調べたワークスペースかどうかチェック
			CString strDBString;
			if( ! _AheIsAlreadyCheckedWorkspace( ipWorkspace, listWorkspacePathName, strDBString ) )
			{
				ipTable = AheOpenTableByTableName( ipWorkspace, lpszTableName, bReference );
				if( ipTable != NULL )
					return ipTable;
				else
					listWorkspacePathName.push_back( strDBString );
			}
		}
		else
			return AheGetFeatureClass( *it );
	}

	return NULL;
}

ITablePtr AheOpenTableByTableName( IWorkspacePtr ipWorkspace, LPCTSTR lpszTableName, BOOL bReference/* = FALSE*/ )
{
	// 引数チェック
	if( ipWorkspace == NULL || lpszTableName == NULL ) 
	{
		_ASSERTE( ipWorkspace != NULL && lpszTableName != NULL );
		return NULL;
	}
	IFeatureWorkspacePtr ipFeatWorkspace( ipWorkspace );
	if( ipFeatWorkspace == NULL )
	{
		_ASSERTE( ipFeatWorkspace != NULL );
		return NULL;
	}

	// テーブル名決定
	IDatabaseConnectionInfoPtr ipDBConInfo( ipWorkspace );
	CComBSTR bstrTableName;

	if( bReference == FALSE || IDatabaseConnectionInfoPtr( ipWorkspace ) == NULL )
		bstrTableName = lpszTableName;
	else {
		CComBSTR bstrRefTableName( _T("REFERENCE.") );
		if( FAILED( bstrRefTableName.Append( lpszTableName ) ) )
			_ASSERTE(false);
		bstrTableName = bstrRefTableName;
	}

	// テーブルオープン
	ITablePtr ipTable;
	if( FAILED( ipFeatWorkspace->OpenTable( bstrTableName, &ipTable ) ) )
		AheTraceErrorMessage( _T("AheOpenTableByTableName") );

	return ipTable;
}

// 同じオーナーのテーブルをオープンする
ITablePtr AheOpenSameOwnerTable( IWorkspace* ipWork, ITable* ipTable, LPCTSTR lpcszTableName )
{
	ITablePtr ipRet; // 返り値

	// 引数チェック
	if( ! ( ipTable && lpcszTableName ) )
		return ipRet;

	CString strTableName(lpcszTableName);

	// オーナー名付きのテーブル名の場合
	if( _tcschr( lpcszTableName, _T('.') ) )
	{
		// オーナー名を取る
		int nPeriodIndex = strTableName.Find( _T('.'), 0 );
		strTableName = strTableName.Right( strTableName.GetLength() - nPeriodIndex - 1 );
	}

	CString strOwnerName( AheGetFeatureClassOwnerName( ipTable ) );
	if( ! strOwnerName.IsEmpty() )
		return AheOpenTableByTableName( ipWork, strOwnerName + _T(".") + strTableName );
	else
		return AheOpenTableByTableName( ipWork, strTableName );
}

ITablePtr AheOpenSameOwnerTable( ITable* ipTable, LPCTSTR lpcszTableName )
{
	return AheOpenSameOwnerTable( AheGetWorkspace( ipTable ), ipTable, lpcszTableName );
}

ITablePtr AheGetReferenceTable( IMapPtr ipMap, LPCTSTR lpcszTableName )
{
	return AheGetTableByTableName( ipMap, lpcszTableName, TRUE );
}

BOOL AheGetFeatureClass( IMapPtr/* piMap*/, IUnknownPtr ipUnk, IFeatureClass** ipFeatureClass )
{
	IFeatureClassPtr ipRet( AheGetFeatureClass( ipUnk ) );
	if( ipRet != NULL )
	{
		(*ipFeatureClass) = ipRet;
		(*ipFeatureClass)->AddRef();
		return TRUE;
	}
	return FALSE;
}

ITablePtr AheGetTable( IUnknownPtr ipUnk )
{
	ITablePtr ipRet; // 戻り値

	// FeatureLayerはITableのインターフェースを持っているので
	// 先に判別する必要がある
	IFeatureLayerPtr ipLayer( ipUnk );
	if( ipLayer )
	{
		IFeatureClassPtr ipClass;
		ipLayer->get_FeatureClass( &ipClass );
		ipRet = ipClass;
	}
	else {
		// 都合上、ITable オブジェクトがそのまま来ることがある
		ipRet = ipUnk;
		if( ! ipRet )
		{
			_IRowPtr ipRow( ipUnk );

			// ipUnk == IRow の場合
			if( ipRow ) 
				ipRow->get_Table( &ipRet );
		}
	}

	return ipRet;
}

//} // sindy
