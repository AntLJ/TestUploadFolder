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

/*! \file FeatureTree.cpp
\brief CFeatureTree クラスのインプリメンテーション
\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
\date 2002-02-01
*/
#include "stdafx.h"
#include "FeatureTree.h"
//#include "resource.h"       // メイン シンボル

// コンストラクタ
CFeatureTree::CFeatureTree()
{
	m_pFeatureDefList = NULL;
	m_pLQDefList = NULL;
	m_arrayHilitFeatures.CreateInstance( CLSID_Set );
	m_bIsLineHilight = FALSE;
	m_bIsVertexHilight = FALSE;	
	m_bIsLQAttribute = false;
	m_bIsRButtonClick = false;
}

// デストラクタ
CFeatureTree::~CFeatureTree()
{
	Clear();
}

// ArcHelperをセットして初期化
void CFeatureTree::Init( IApplication* ipApp )
{
	ATLASSERT( ipApp != NULL );
	m_cArcHelper.Init( (IApplicationPtr)ipApp );
}

// ハイライト削除用
void CFeatureTree::Clear()
{
	m_arrayHilitFeatures->RemoveAll();
	m_ipHilitPoint = NULL;
	m_pFeatureDefList = NULL;
	m_pLQDefList = NULL;
	m_ipHilightGeometry = NULL;
}

// フィーチャ定義クラスのリストをツリーにセットする
void CFeatureTree::SetFeatures( std::list<CFeatureDef>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	m_pFeatureDefList = pFeatureDefList;
	m_bIsLQAttribute = false;

	Add( m_pFeatureDefList );
}

// リンク列定義クラスのリストをツリーにセットする
void CFeatureTree::SetLQs( std::list<CLQRowDef>* pFeatureDefList )
{
	ATLASSERT( pFeatureDefList != NULL );
	m_pLQDefList = pFeatureDefList;
	m_bIsLQAttribute = true;

	AddLQ( m_pLQDefList );
}

// ツリーで表示されているものを_ISetにいれて返す
void CFeatureTree::GetSelectedFeatures( _ISet** ipSet )
{
	long lFeatureIndex = -1, lTableIndex = -1, lRowIndex = -1;
	GetSelectedFeatures( lFeatureIndex, lTableIndex, lRowIndex );
	
	if( lFeatureIndex < 0 ) 
		return;

	_ISetPtr ipRetSet( CLSID_Set );
	if( !m_bIsLQAttribute )
	{
		std::list<CFeatureDef>::iterator itFeatDefList = m_pFeatureDefList->begin();
		if( m_pFeatureDefList->size() - 1 < (UINT)lFeatureIndex ) 
		{
			(*ipSet) = ipRetSet;
			(*ipSet)->AddRef();
			return;
		}
		advance( itFeatDefList, lFeatureIndex );
		
		if( lFeatureIndex >= 0 && ( lTableIndex == -1 && lRowIndex == -1 ) )
			ipRetSet->Add( itFeatDefList->GetFeature() );
		else if( lTableIndex >= 0 && lRowIndex >= 0 ) 
		{
			std::list<CRelationalTableDef>* pRelTableDefList = itFeatDefList->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, lTableIndex );
			
			std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
			advance( itRowDefList, lRowIndex );
			
			ipRetSet->Add( itRowDefList->GetRow() );
		}
		(*ipSet) = ipRetSet;
		(*ipSet)->AddRef();
	}
	else
	{
		std::list<CLQRowDef>::iterator itFeatDefList = m_pLQDefList->begin();
		if( m_pLQDefList->size() - 1 < (UINT)lFeatureIndex ) 
		{
			(*ipSet) = ipRetSet;
			(*ipSet)->AddRef();
			return;
		}
		advance( itFeatDefList, lFeatureIndex );
		
		if( lFeatureIndex >= 0 && ( lTableIndex == -1 && lRowIndex == -1 ) )
			ipRetSet->Add( itFeatDefList->GetFeature() );
		else if( lTableIndex >= 0 && lRowIndex >= 0 ) 
		{
			std::list<CRelationalTableDef>* pRelTableDefList = itFeatDefList->GetRelationalTableDefList();
			std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin();
			advance( itRelTableDefList, lTableIndex );
			
			std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
			advance( itRowDefList, lRowIndex );
			
			ipRetSet->Add( itRowDefList->GetRow() );
		}
		(*ipSet) = ipRetSet;
		(*ipSet)->AddRef();
	}
}

// 現在選択されているアイテムからインデックス番号を取得する
void CFeatureTree::GetSelectedFeatures( long& lFeatureIndex, long& lTableIndex, long& lRowIndex )
{
	if( !::IsWindow( m_hWnd ) )
		return;
	
	HTREEITEM hSelectedItem = GetSelectedItem();
	if( !hSelectedItem )
		return;
	DWORD dwSelected = GetItemData( hSelectedItem );
	
	// フィーチャ
	if( dwSelected & 0x4000000 )
	{
		// フィーチャのインデックス番号取得
		lFeatureIndex = (LONG)(dwSelected ^ 0x4000000);
		lTableIndex = lRowIndex = -1;
	}
	// レコード
	else if( dwSelected & 0x1000000 )
	{
		// テーブルのインデックス番号取得
		HTREEITEM hTableItem = GetParentItem( hSelectedItem );
		ATLASSERT( ( GetItemData( hTableItem ) & 0x2000000 ) != 0x0 );
		lTableIndex = (LONG)( GetItemData( hTableItem ) ^ 0x2000000 );
		// フィーチャのインデックス番号取得
		HTREEITEM hFeatItem = GetParentItem( hTableItem );
		ATLASSERT( ( GetItemData( hFeatItem ) & 0x4000000 ) != 0x0 );
		lFeatureIndex = (LONG)( GetItemData( hFeatItem ) ^ 0x4000000 );
		// レコードのインデックス番号取得
		lRowIndex = (LONG)( dwSelected ^ 0x1000000 );
	}
}

// アイテムをツリーに登録する
HTREEITEM CFeatureTree::AddItem( bool bIsRoot, HTREEITEM lpItem, LPCTSTR lptstrNodeName )
{
	TV_INSERTSTRUCT curTreeItem;
	
	if( bIsRoot )
		curTreeItem.hParent = NULL;
	else
		curTreeItem.hParent = lpItem;

	curTreeItem.hInsertAfter = TVI_LAST;
	curTreeItem.item.pszText = (LPTSTR)lptstrNodeName;
	curTreeItem.item.mask = TVIF_TEXT;
	
	HTREEITEM htItem = InsertItem(&curTreeItem);
	
	return htItem;
}

// 外部テーブルをツリーに登録する
void CFeatureTree::AddOuterTable( HTREEITEM& item, CRelationalTableDef* pRelTableDef, LONG lTableIndex )
{
	if( !pRelTableDef )
	{
		item = NULL;
		return;
	}

	// 外部テーブル名を登録
	HTREEITEM itemTable = AddItem( NULL, item, pRelTableDef->GetTableName() );
	SetItemData( itemTable, (DWORD)0x2000000 | (DWORD)lTableIndex );
	
	LONG lRowCount = 0;
	for( std::list<CRowDef>::iterator it = pRelTableDef->begin(); it != pRelTableDef->end(); it++, lRowCount++ )
	{
		// 外部レコードの表示には OBJECTID を使用
		CString strOID;
		strOID.Format( _T("%d"), it->GetRowOID() );
		HTREEITEM itemRow = AddItem( FALSE, itemTable, strOID );
		SetItemData( itemRow, (DWORD)0x1000000 | (DWORD)lRowCount );
	}
}

// フィーチャをツリーに登録する
void CFeatureTree::Add( std::list<CFeatureDef>* pFeatureDef )
{
	USES_CONVERSION;
	
	LONG lFeatCount = 0;
	BOOL bIsOver = FALSE;
	LONG lAddItemCount = 0;
	for( std::list<CFeatureDef>::iterator it = pFeatureDef->begin(); it != pFeatureDef->end(); it++, lFeatCount++ )
	{
		HTREEITEM pLayerNode = NULL, pFeatureNode = NULL, pRowNode = NULL, pVertexNode = NULL;
		IFeaturePtr ipFeature = it->GetFeature();
		
		// フィーチャのTOCに表示されているレイヤ名を取得
		CComBSTR bstrLayerName;
		GetLayerName( ipFeature, bstrLayerName );
		
		// OIDを取得
		long lObjectID = -1;
		ipFeature->get_OID( &lObjectID );
		CString strFieldValue;
		strFieldValue.Format(_T("%d"), lObjectID );
		
		// 既に登録されているレイヤ名かどうかチェック
		pLayerNode = SearchLayerName( bstrLayerName );
		// まだ登録されていないレイヤ名の場合は新規にレイヤを追加する
		if( !pLayerNode ) 
		{
			pLayerNode = AddItem( TRUE, NULL, OLE2T(bstrLayerName) );                // 親を登録
			lAddItemCount++;
			SetItemData( pLayerNode, (DWORD)0x8000000 );                                        // レイヤはHSB1ビット目がON
		}
		pFeatureNode = AddItem( FALSE, pLayerNode, strFieldValue );                // 子供として登録
		lAddItemCount++;

		// アイテムのデータとしてフィーチャリストのインデックス番号を保持
		SetItemData( pFeatureNode, (DWORD)0x4000000 | (DWORD)lFeatCount );        // フィーチャはHSB2ビット目がON
		CComBSTR bstrTmp;
		GetItemText( pLayerNode, (BSTR&)bstrTmp );
		
		// 選択状態にするかどうか
		if(  lFeatCount == 0 )
			SelectItem( pFeatureNode );
		
		// 頂点をツリーに追加
		HTREEITEM pVertex;
		LONG lCount = 0;
		IGeometryPtr ipGeom;
		ipFeature->get_Shape( &ipGeom );
		IPointCollectionPtr ipCol( ipGeom );
		if( !ipCol ) {
			IPointPtr ipGeomPoint;
			ipGeomPoint = ipGeom; 
			if( ipGeomPoint == NULL )
				return;

			CString strVertex;
			strVertex.Format( _T("%d：%s"), 0, getXYZAsString(ipGeomPoint));
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			lAddItemCount++;
			SetItemData( pVertexNode, (DWORD)0x800000 | (DWORD)lFeatCount );
			pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
			lAddItemCount++;
			// アイテムのデータとして構成点のインデックス番号を保持（フィーチャの場合と区別するため）
			SetItemData( pVertex, (DWORD)0x400000 );
		} else {	
			// 外部テーブルを登録する
			std::list<CRelationalTableDef>* pRelTableDefList = it->GetRelationalTableDefList();
			LONG lTableIndex = 0;
			for( std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin(); itRelTableDefList != pRelTableDefList->end(); itRelTableDefList++, lTableIndex++ )
			{
				AddOuterTable( pFeatureNode, &(*itRelTableDefList), lTableIndex );
			}
			ipCol->get_PointCount( &lCount );
						
			if( !bIsOver )
			{
				if( lAddItemCount + lCount > 10000 )
				{
					AheMessageBox( NULL, AheLoadString( IDS_TOOMENYITEM ), AheLoadString( IDS_CAPTION ), MB_OK|MB_TASKMODAL );
					bIsOver = TRUE;
				}
			}
			if( !bIsOver )
			{
				LONG        lVertexMax = 2000;
				if(lCount < lVertexMax)
				{
					pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
					lAddItemCount++;
					SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
					for( LONG i = 0; i < lCount; i++ ) {
						IPointPtr ipPoint;
						ipCol->get_Point( i, &ipPoint );

						CString strVertex;
						strVertex.Format(_T("%d：%s"), i, getXYZAsString(ipPoint));
						pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
						lAddItemCount++;
						// アイテムのデータとして構成点のインデックス番号を保持（フィーチャの場合と区別するため）
						SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
					}
				}
				else{
					CString strMessage;
					strMessage.Format( AheLoadString( IDS_CHECKDISPMENYVERTEX ) ,OLE2T(bstrLayerName),lObjectID,lCount,lVertexMax);
					int iRet = AheMessageBox( NULL, strMessage, AheLoadString( IDS_CAPTION ), MB_YESNO|MB_TASKMODAL|MB_DEFBUTTON2 );
					
					if( iRet == IDYES )
					{
						pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
						lAddItemCount++;
						SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
						for( LONG i = 0; i < lCount; i++ ) {
							IPointPtr ipPoint = NULL;
							ipCol->get_Point( i, &ipPoint );

							CString strVertex;
							strVertex.Format(_T("%d：%s"), i, getXYZAsString(ipPoint));
							pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
							lAddItemCount++;
							// アイテムのデータとして構成点のインデックス番号を保持（フィーチャの場合と区別するため）
							SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
						}
					}
				}
			}
		}
	}
    // 描画（一番最初の時だけの為に…）
    HilitSelectedFeatures();
}

// リンク列をツリーに登録する
void CFeatureTree::AddLQ( std::list<CLQRowDef>* pFeatureDef )
{
	if( !pFeatureDef )
		return;

	CString strFieldValue;
	HTREEITEM hSelected = NULL;

	USES_CONVERSION;

	LONG lFeatCount = 0;
	for( std::list<CLQRowDef>::iterator it = pFeatureDef->begin(); it != pFeatureDef->end(); it++, lFeatCount++ )
	{
		HTREEITEM pLayerNode = NULL, pFeatureNode = NULL, pRowNode = NULL, pVertexNode = NULL;
		_IRowPtr ipRow = it->GetFeature();

		if( !ipRow )
		{
			ATLASSERT( ipRow != NULL );
			return;
		}

		long lOID = -1;
		ipRow->get_OID( &lOID );
		strFieldValue.Format(_T("%d"), lOID );

		// リンク列のオーナ名を表示しなくてはいけないため
		CString strLayerName = AheGetFeatureClassOwnerName( ipRow );
		if( strLayerName.IsEmpty() == FALSE )
			strLayerName += _T(".");
		strLayerName += it->GetDispName();

		// 既に登録されているレイヤ名かどうかチェック
		pLayerNode = SearchLayerName( (LPCTSTR)strLayerName );
		// まだ登録されていないレイヤ名の場合は新規にレイヤを追加する
		if( !pLayerNode ) 
		{
			pLayerNode = AddItem( TRUE, NULL, strLayerName );		// 親を登録
			SetItemData( pLayerNode, (DWORD)0x8000000 );					// レイヤはHSB1ビット目がON
		}
		pFeatureNode = AddItem( FALSE, pLayerNode, strFieldValue );		// 子供として登録
		// アイテムのデータとしてフィーチャリストのインデックス番号を保持
		SetItemData( pFeatureNode, (DWORD)0x4000000 | (DWORD)lFeatCount );	// フィーチャはHSB2ビット目がON

		// 選択状態にするかどうか
		if(  lFeatCount == 0 || it->GetDefault() )
		{
			SelectItem( pFeatureNode );
			hSelected = pFeatureNode;
		}

		// 頂点をツリーに追加
		HTREEITEM pVertex;
		LONG lCount = 0;
		IGeometryPtr ipGeom = it->GetShape();
		IPointCollectionPtr ipCol( ipGeom );
		if( ipCol == NULL ) {
			IPointPtr ipGeomPoint;
			ipGeomPoint = ipGeom; 
			if( !ipGeomPoint )
				return;
			CString strVertex;
			DOUBLE x = 0.0, y = 0.0;
			ipGeomPoint->QueryCoords( &x, &y );
			char szXPos[ 128];
			_gcvt( x, 100, szXPos);
			char szYPos[ 128];
			_gcvt( y, 100, szYPos);
			USES_CONVERSION;
			strVertex.Format( _T("%d：X=%s Y=%s"), 0, A2T( szXPos), A2T( szYPos));
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			SetItemData( pVertexNode, (DWORD)0x800000 | (DWORD)lFeatCount );
			pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
			// アイテムのデータとして構成点のインデックス番号を保持（フィーチャの場合と区別するため）
			SetItemData( pVertex, (DWORD)0x400000 );
		} else {
			// 外部テーブルを登録する
			std::list<CRelationalTableDef>* pRelTableDefList = it->GetRelationalTableDefList();
			LONG lTableIndex = 0;
			for( std::list<CRelationalTableDef>::iterator itRelTableDefList = pRelTableDefList->begin(); itRelTableDefList != pRelTableDefList->end(); itRelTableDefList++, lTableIndex++ )
			{
				AddOuterTable( pFeatureNode, &(*itRelTableDefList), lTableIndex );
			}
			ipCol->get_PointCount( &lCount );
			pVertexNode = AddItem( FALSE, pFeatureNode, AheLoadString( IDS_VERTEX ) );
			SetItemData( pVertexNode, (DWORD)0x0800000 | (DWORD)lFeatCount );
			for( LONG i = 0; i < lCount; i++ ) {
				IPointPtr ipPoint = NULL;
				ipCol->get_Point( i, &ipPoint );
				CString strVertex;
				DOUBLE x = 0.0, y = 0.0;
				ipPoint->QueryCoords( &x, &y );
				char szXPos[ 128];
				_gcvt( x, 100, szXPos);
				char szYPos[ 128];
				_gcvt( y, 100, szYPos);
				USES_CONVERSION;
				strVertex.Format( _T("%d：X=%s Y=%s"), i, A2T( szXPos), A2T( szYPos));
				pVertex = AddItem( FALSE, pVertexNode, (LPCTSTR)strVertex );
				// アイテムのデータとして構成点のインデックス番号を保持（フィーチャの場合と区別するため）
				SetItemData( pVertex, (DWORD)0x400000 | (DWORD)i );
			}
		}
	}
	// 選択されたフィーチャをハイライトする	
	if( hSelected )
		HilitSelectedLQ( GetLQRowDef( hSelected )->GetShape() );
}

// IFeatureからTOCに表示されているレイヤ名を取得
void CFeatureTree::GetLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName )
{
	// [Bug 5358] [要望] 同一ソースのレイヤが複数TOCにいるときは選択レイヤ名を出して欲しいに対応
	// レイヤを選択している場合には、GetSelectedLayerNameから名前を取る。
	GetSelectedLayerName(ipFeature, bstrLayerName);

	// もし取れなかったら（グループレイヤとか選択中だったり、データセットを選択していた場合）がんばってフィーチャからレイヤ名を取得
	if( !bstrLayerName )
	{
		// すべてのフィーチャレイヤでループをまわして対象のレイヤを見つけてTOCのレイヤ名を取得する
		std::list<CAdapt<ILayerPtr> > listLayers = AheFindLayers( m_cArcHelper.GetMap(), ipFeature );
		// 基本的に1個のはず(複数個ある場合もあるが、その場合は最初に見つかったほうでかまわない)
		for( std::list<CAdapt<ILayerPtr> >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
		{
			(it->m_T)->get_Name( &bstrLayerName );
			break;
		}
	}
}

// 選択レイヤから名前を取得
void CFeatureTree::GetSelectedLayerName( IFeaturePtr ipFeature, CComBSTR& bstrLayerName)
{
	// ここにNULLでくる可能性はあるだろうか？一応チェック
	if( !ipFeature )
		return;
	// IFeatureからフィーチャクラス取得しておく
	IObjectClassPtr ipClass;
	ipFeature->get_Class( &ipClass );

	// レイヤ選択してフィーチャを選択してから、ここにくるまでに、レイヤの選択が変わらないという前提
	IMxDocumentPtr ipMxDoc = m_cArcHelper.GetDocument();
	IContentsViewPtr ipContentsView;
	ipMxDoc->get_ContentsView( 0, &ipContentsView );
	// 選択アイテムを取得
	CComVariant vaSelectedItem;
	ipContentsView->get_SelectedItem( &vaSelectedItem );
	if( VT_UNKNOWN == vaSelectedItem.vt)
	{
		_ISetPtr ipSet( vaSelectedItem );
		// 複数レイヤ選択しているとき
		if( ipSet )
		{
			IUnknownPtr ipUnk;
			ipSet->Reset();
			while( ipSet->Next( &ipUnk ) == S_OK )
			{
				IGeoFeatureLayerPtr ipGeoFeatureLayer( ipUnk );
				// ジオフィーチャレイヤのみ名前を取る
				if( ipGeoFeatureLayer )
				{
					IFeatureClassPtr ipFeatureClass;
					ipGeoFeatureLayer->get_FeatureClass( &ipFeatureClass );
					// フィーチャクラスとワークスペースのポインタで比較する
					if( (IUnknown*)(IUnknownPtr)ipClass == (IUnknown*)(IUnknownPtr)ipFeatureClass )
					{
						// TOCに表示されているレイヤ名を取得する
						ipGeoFeatureLayer->get_Name( &bstrLayerName );
						break;
					}
				}
			}
		}
		// ひとつだけレイヤ選択している時
		else 
		{
			IGeoFeatureLayerPtr ipGeoFeatureLayer( vaSelectedItem );
			// ジオフィーチャレイヤのみ
			if( ipGeoFeatureLayer )
			{
				// [Bug 5471]TOC選択してSplitToolで道路リンクを分割するとレイヤ表示がTOCで選択したものしかでないへの修正
				// 外部ツール（SplitTool）から呼ばれる場合には、TOCで選択しているレイヤ以外も表示することがあるのを忘れてたorz
				// ツリーに表示するフィーチャがTOCで選択しているフィーチャと同じフィーチャクラスかどうか調べる必要がある
				// 同じでなかったら、呼び出し元のGetLayerNameでがんばって対象のレイヤを見つけてやる
				IFeatureClassPtr ipFeatureClass;
				ipGeoFeatureLayer->get_FeatureClass( &ipFeatureClass );
				// フィーチャクラスとワークスペースのポインタで比較する
				if( (IUnknown*)(IUnknownPtr)ipClass == (IUnknown*)(IUnknownPtr)ipFeatureClass )
					ipGeoFeatureLayer->get_Name( &bstrLayerName );
			}
		}
	}
}


// 同じレイヤ名を持つアイテムを探す
HTREEITEM CFeatureTree::SearchLayerName( CComBSTR lpcszLayerName )
{
	if( lpcszLayerName.Length() == 0) 
		return NULL;

	HTREEITEM itemFirst;
	if( !m_bIsLQAttribute )
		itemFirst = GetFirstVisibleItem();
	else
		itemFirst = GetRootItem();

	if( !itemFirst )
		return NULL;
	if( IsSameText( lpcszLayerName, itemFirst ) )
		return itemFirst;
	
	HTREEITEM itemNext = GetNextItem( itemFirst, TVGN_NEXT );
	while( itemNext != NULL ) {
		if( IsSameText( lpcszLayerName, itemNext ) )
			return itemNext;
		itemNext = GetNextItem( itemNext, TVGN_NEXT );
	}
	return NULL;
}

// レイヤ名とOBJECTIDからアイテムを探す
HTREEITEM CFeatureTree::SearchOID( LPCTSTR lpcszLayerName, LONG lOID )
{
	HTREEITEM hLayerItem = SearchLayerName( lpcszLayerName );
	HTREEITEM hFeatureItem = GetChildItem( hLayerItem );
	LONG lObjectID = -1;

	if( !hFeatureItem ) 
		return NULL;

	CLQRowDef* pLQRowDef = GetLQRowDef( hFeatureItem );
	if( !pLQRowDef )
	{
		ATLASSERT( pLQRowDef != NULL );
		hFeatureItem = NULL;
	}
	else {
		_IRowPtr ipLQRow = pLQRowDef->GetFeature();
		ipLQRow->get_OID( &lObjectID );
		while( lOID != lObjectID )
		{
			hFeatureItem = GetNextSiblingItem( hFeatureItem );

			if( !hFeatureItem )
				break;
			ipLQRow = GetLQRowDef( hFeatureItem )->GetFeature();
			ipLQRow->get_OID( &lObjectID );
		}
	}

	return hFeatureItem;
}

// 渡されたレイヤ名とツリーのテキストが同じかどうかの判定
BOOL CFeatureTree::IsSameText( CComBSTR bstrLayerName, HTREEITEM item )
{
	if( bstrLayerName.Length() == 0 || !item )
		return FALSE;

	USES_CONVERSION;
	
	if( !m_bIsLQAttribute )
	{
		BSTR bstrCurrentLayerName = NULL;
		
		GetItemText( item, (BSTR&)bstrCurrentLayerName );
		if( lstrcmp( OLE2T( bstrLayerName ), OLE2T( bstrCurrentLayerName ) ) == 0 )
		{
			if( bstrCurrentLayerName != NULL )
				::SysFreeString( bstrCurrentLayerName );
			return TRUE;
		}
		else
		{
			if( bstrCurrentLayerName != NULL )
				::SysFreeString( bstrCurrentLayerName );
			return FALSE;
		}
	}
	else
	{
		BSTR bstrCurrentLayerName = NULL;
		GetItemText( item, (BSTR&)bstrCurrentLayerName );
		BOOL bRet = ( lstrcmp( OLE2T( bstrLayerName ), OLE2T( bstrCurrentLayerName ) ) == 0 );
		return bRet;
	}
}

// ツリーコントロール上のアイテムの種類を判定する
INT CFeatureTree::ItemKind( HTREEITEM item )
{
	if( item == NULL )
		return NOKINDS;

	DWORD dwData = GetItemData( item );
	
	// レイヤ
	if( dwData & 0x8000000 )
		return LAYERITEM;
	// フィーチャ
	else if( dwData & 0x4000000 )
		return FEATUREITEM;
	// テーブル
	else if( dwData & 0x2000000 )
		return OUTERTABLEITEM;
	// レコード
	else if( dwData & 0x1000000 )
		return OUTERROWITEM;
	// 構成点
	else if( dwData & 0x400000 )
		return FEATUREPOINTITEM;
	
	return NOKINDS;
}

// 選択されているリンク列をハイライトする
void CFeatureTree::HilitSelectedLQ( IGeometryPtr ipGeom,  BOOL bRefresh/* = FALSE */ )
{
	IScreenDisplayPtr ipScreen = m_cArcHelper.GetScreenDisplay();

	// ハイライト色の設定取得
	int nRed = 0, nGreen = 0, nBlue = 0;
	_bstr_t strXPath = _T("settings/setting[@name='LQAttributeTool' and @type='polyline_main']/color");
	bool bUseMainSetting = false;
	bUseMainSetting = GetHighlightColor( strXPath, nRed, nGreen, nBlue );

	if( !bRefresh ) {
		// 古いフィーチャ消去
		if( m_ipHilightGeometry )
			AheDrawArrowline( ipScreen, m_ipHilightGeometry, bUseMainSetting? RGB( nRed, nGreen, nBlue ) : _COLOR_LQFLASH, FALSE, TRUE );
	}

	// ハイライト形状の終点を取得
	IPolylinePtr ipLQLine( ipGeom );
	IPointPtr ipLQToPoint;
	ipLQLine->get_ToPoint( &ipLQToPoint );
	ISpatialReferencePtr ipSpRef = AheGetSpatialReference( ipGeom );

	// 画面内に終点が含まれているか？
	IEnvelopePtr ipEnv;
	m_cArcHelper.GetActiveView()->get_Extent( &ipEnv );
	IRelationalOperatorPtr ipRel( ipLQToPoint );
	VARIANT_BOOL vaIsWithin = VARIANT_FALSE;
	ipRel->Within( ipEnv, &vaIsWithin );
	
	// 含まれてないなら別の設定を取る必要があるのでハイライト色再取得
	bool bUseSubSetting = false;
	int nRedSub = 0, nGreenSub = 0, nBlueSub = 0;
	if( !vaIsWithin )
	{
		strXPath = _T("settings/setting[@name='LQAttributeTool' and @type='polyline_sub']/color");
		bUseSubSetting = GetHighlightColor( strXPath, nRedSub, nGreenSub, nBlueSub );
	}

	// 画面矩形でハイライト形状をカット(bug9405)
	ITopologicalOperatorPtr ipTopo( AheGetClone( ipLQLine ) );
	// Intersectだと自己交差がある場合に交差部分でラインが分割されるのでClipを使う
	ipTopo->Clip( ipEnv );
	
	// ハイライト形状がマルチパートになってしまった時はどれも矢印が出るようにする(bug9405)
	IGeometryCollectionPtr ipGeomCol( AheSetKnownSimple( ipTopo ) );
	if ( ipGeomCol )
	{
		long lPartCount = -1;
		ipGeomCol->get_GeometryCount( &lPartCount );

		for( int i = 0; i < lPartCount; ++i )
		{
			IGeometryCollectionPtr ipNewGeomCol( CLSID_Polyline );
			IPolylinePtr ipPolyline( ipNewGeomCol );
			ipPolyline->putref_SpatialReference( ipSpRef );

			IGeometryPtr ipPartGeom;
			ipGeomCol->get_Geometry( i, &ipPartGeom );
			
			IPathPtr ipPath( ipPartGeom );
			ipNewGeomCol->AddGeometry( ipPath );
			// 終点が画面内になければハイライトは黄色、画面内にあれば水色(bug9405)
			AheDrawArrowline( ipScreen, ipPolyline, vaIsWithin? (bUseMainSetting? RGB( nRed, nGreen, nBlue ) : _COLOR_LQFLASH) : (bUseSubSetting? RGB( nRedSub, nGreenSub, nBlueSub ) : _COLOR_LQFLASH_YELLOW), FALSE, TRUE);
		}
	}

	if( !bRefresh ) {
		m_ipHilightGeometry = ipGeom; // 元々の形状を取っておく
		/// ゴミを消すのには esriViewForeground
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
	}
	
	InvalidateRect( NULL, FALSE );
}

// 選択されているフィーチャをハイライトする
void CFeatureTree::HilitSelectedFeatures( BOOL bRefresh/* = FALSE */ )
{
	IFeaturePtr ipFeature;
	IScreenDisplayPtr ipScreen = m_cArcHelper.GetScreenDisplay();
	if( !bRefresh ) {
		// 古いフィーチャ消去
		m_arrayHilitFeatures->Reset();
		IUnknownPtr ipUnk;
		while( m_arrayHilitFeatures->Next( &ipUnk ) == S_OK ) 
		{
			ipFeature = ipUnk;
			ATLASSERT( ipFeature != NULL );
			// 道路リンクの方向に矢印をつける
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
			{
				BOOL bFixFlow = FALSE;
				IGeometryPtr ipRoadGeom;
				ipFeature->get_ShapeCopy( &ipRoadGeom );
				AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
				// ラインの回りをハイライトするかどうか
				if( m_bIsLineHilight == FALSE )
					AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
			}
			// ポイントフィーチャの場合
			else if( kinds == sindyePointFeature && kinds == sindyeNodeFeature)
				AheDrawFeatureXOR( ipScreen, ipFeature, 0, FALSE );
			// つまりポリゴンの場合
			else
			{
				IGeometryPtr ipGeom;
				ipFeature->get_Shape( &ipGeom );
				IPolygonPtr ipPolygon = ipGeom;
				AheDrawPolygonXOR( ipScreen, ipPolygon );
			}
		}
		
		// array 消去
		m_arrayHilitFeatures->RemoveAll();
		
		// 新しいフィーチャ描画
		_ISetPtr ipSet;
		GetSelectedFeatures( &ipSet );
		if( !ipSet )
			return;
		
		ipSet->Reset();
		while( ipSet->Next( &ipUnk ) == S_OK ) {
			ipFeature = ipUnk;
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( ipFeature != NULL ) {
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
				{
					BOOL bFixFlow = FALSE;
					IGeometryPtr ipRoadGeom;
					ipFeature->get_ShapeCopy( &ipRoadGeom );
					AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
				}
				
				// フィーチャの周りをハイライト表示
				if( kinds != sindyePointFeature && kinds != sindyeNodeFeature)
				{
					if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
					{
						if( m_bIsLineHilight == FALSE )
							AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
					}
					// つまりポリゴンの場合
					else
					{
						IGeometryPtr ipGeom;
						ipFeature->get_Shape( &ipGeom );
						IPolygonPtr ipPolygon = ipGeom;
						AheDrawPolygonXOR( ipScreen, ipPolygon );
					}
				}

				// 構成点をハイライト
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
				{
					if( !m_bIsVertexHilight )
						AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
				}
				else
					AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
				
				m_arrayHilitFeatures->Add( ipFeature );        //!< Array に保存
			}
			ipUnk = NULL;
		}
		/// ゴミを消すのには esriViewForeground
		m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		
	} else {
		IUnknownPtr ipUnk;
		m_arrayHilitFeatures->Reset();
		while( m_arrayHilitFeatures->Next( &ipUnk ) == S_OK ) 
		{
			ipFeature = ipUnk;
			ATLASSERT( ipFeature != NULL );
			// 道路リンクの方向に矢印をつける
			sindyeFeatureKinds        kinds = AheGetFeatureKinds(ipFeature);
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature)
			{
				BOOL bFixFlow = FALSE;
				IGeometryPtr ipRoadGeom;
				ipFeature->get_ShapeCopy( &ipRoadGeom );
				AheDrawArrowline( ipScreen, ipRoadGeom, _COLOR_LQFLASH, FALSE, TRUE );
			}
			// フィーチャの周りをハイライト表示
			if( kinds != sindyePointFeature && kinds != sindyeNodeFeature)
			{
				if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
				{
					if( !m_bIsLineHilight )
						AheDrawFeatureXOR( ipScreen, ipFeature, AheConvertPixelsToMapUnits( ipScreen, 10 ), FALSE );
				}
				// つまりポリゴンの場合
				else
				{
					IGeometryPtr ipGeom;
					ipFeature->get_Shape( &ipGeom );
					IPolygonPtr ipPolygon = ipGeom;
					AheDrawPolygonXOR( ipScreen, ipPolygon );
				}
			}

			// 構成点をハイライト
			if( kinds == sindyeRoadFeature || kinds == sindyeLineFeature )
			{
				if( !m_bIsVertexHilight )
					AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );
			}
			else
				AheDrawVertex( ipScreen, ipFeature, FALSE, 4, 10, 255, 10, esriSMSSquare, TRUE );

			ipUnk = NULL;
		}
	}
	if( m_ipHilitPoint != NULL )
		AheDrawPoint( ipScreen, m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );
	
	InvalidateRect( NULL, FALSE );
}

// マウスボタンのクリックを処理する関数
LONG CFeatureTree::OnMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick )
{
	HTREEITEM hItem;
	CPoint point;
	UINT nHF;
	
	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	INT nFlags = (INT)wParam;
	hItem = HitTest(point, &nHF);
	
	if( nHF & TVHT_ONITEMBUTTON ) {
		Expand( hItem, TVE_TOGGLE );
		
		return 1;
	} else if( nHF & TVHT_ONITEMLABEL ) {
		INT iKind = ItemKind( hItem );
		// レイヤのアイテムなら何もしない
		if( iKind == FEATUREITEM ) {
			// 選択状態を変更する
			SelectItem( hItem );
			SetFocus();
			
			if( m_bIsRButtonClick )
				ShowMenu( uMsg, wParam, lParam, bHandled );

			// 選択されたフィーチャをハイライトする
			HilitSelectedFeatures();
			
			// クリックされたアイテムの種類を通知
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		} else if( iKind == FEATUREPOINTITEM ) {
			std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
			advance( it, (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( hItem ) ) ) ) );
			IFeaturePtr ipFeature = it->GetFeature();
			
			m_ipHilitPoint = AheGetPoint( ipFeature, (LONG)( 0x400000 ^ GetItemData( hItem ) ) );
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );
			
			// クリックされた構成点を持つフィーチャが現在選択されているフィーチャと違う場合には
			// 選択フィーチャを変更する
			_ISetPtr ipSet;
			IUnknownPtr ipUnk;
			GetSelectedFeatures( &ipSet );
			if( ipSet )
			{
				ipSet->Reset();
				ipSet->Next( &ipUnk );
			}
			IFeaturePtr ipSelectedFeature = ipUnk;
			
			if( ipFeature != ipSelectedFeature )
			{
				SelectItem( GetParentItem( GetParentItem( hItem ) ) );
				SetFocus();
				
				// 選択されたフィーチャのハイライト
				HilitSelectedFeatures();
				// フィーチャの選択が変更されたことを通知
				return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
			}
			
			if( bDblClick )
				m_cArcHelper.SetCenter( m_ipHilitPoint );
			// ゴミを消すのには esriViewForeground
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		} else if( iKind == OUTERROWITEM ) {
			// 選択状態を変更する
			SelectItem( hItem );
			SetFocus();
			
			// クリックされたアイテムの種類を通知
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		}
	}
	return 0;
}

// リンク列専用マウスボタンのクリックを処理する
LONG CFeatureTree::OnLQMouseDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled, BOOL bDblClick )
{
	HTREEITEM hItem;
	CPoint point;
	UINT nHF;

	point.x = LOWORD(lParam);
	point.y = HIWORD(lParam);
	INT nFlags = (INT)wParam;
	hItem = HitTest(point, &nHF);

	if( nHF & TVHT_ONITEMBUTTON ) {
		Expand( hItem, TVE_TOGGLE );

		return 1;
	} else if( nHF & TVHT_ONITEMLABEL ) {
		INT iKind = ItemKind( hItem );
		// レイヤのアイテムなら何もしない
		if( iKind == FEATUREITEM ) {
			// 選択状態を変更する
			SelectItem( hItem );
			SetFocus();

			if( m_bIsRButtonClick )
				ShowMenu( uMsg, wParam, lParam, bHandled );
			
			m_ipHilitPoint = NULL;
			// 選択されたフィーチャをハイライトする
			HilitSelectedLQ(GetLQRowDef( hItem )->GetShape());
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
			
			// クリックされたアイテムの種類を通知
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		} else if( iKind == FEATUREPOINTITEM ) {
			std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
			advance( it, (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( hItem ) ) ) ) );
			
			((IPointCollectionPtr)it->GetShape())->get_Point( (LONG)( 0x400000 ^ GetItemData( hItem ) ), &m_ipHilitPoint );
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );

			// クリックされた構成点を持つフィーチャが現在選択されているフィーチャと違う場合には
			// 選択フィーチャを変更する
			_ISetPtr ipSet;
			IUnknownPtr ipUnk;
			GetSelectedFeatures( &ipSet );

			// Bug 1375暫定対応 by 米澤
			// ipSetがNULLのままの場合があるため
			if( ipSet )
			{
				ipSet->Reset();
				ipSet->Next( &ipUnk );
			}
			_IRowPtr ipSelectedFeature = ipUnk;
			if( it->GetFeature() != ipSelectedFeature )
			{
				SelectItem( GetParentItem( GetParentItem( hItem ) ) );
				SetFocus();

				// 選択されたフィーチャのハイライト
				HilitSelectedLQ(it->GetShape());
				// フィーチャの選択が変更されたことを通知
				return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
			}

			if( bDblClick )
				m_cArcHelper.SetCenter( m_ipHilitPoint );
			// ゴミを消すのには esriViewForeground
			m_cArcHelper.GetActiveView()->PartialRefresh( esriViewForeground, NULL, NULL );
		} else if( iKind == OUTERROWITEM ) {
			// 選択状態を変更する
			SelectItem( hItem );
			SetFocus();

			// クリックされたアイテムの種類を通知
			return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, iKind );
		}
	}
	return 0;
}

// ツリー上で右クリックを押したときにコピーのメニューを出します
BOOL CFeatureTree::ShowMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// メニュー読み込み
	HMENU hMenu = ::LoadMenu( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(IDR_MENU));
	if ( !hMenu )
		return FALSE;
	// ポップアップメニュー取得
	HMENU hPopup = ::GetSubMenu( hMenu, 0);
	if ( !hPopup )
		return FALSE;
	
	// コンテキストメニューの親となるマップのWindowハンドル取得
	HWND hWnd;
	IApplicationPtr piApp = m_cArcHelper.GetApp();
	piApp->get_hWnd( reinterpret_cast<UINT*>( &hWnd));
	
	// カーソル位置取得
	POINT stPos;
	::GetCursorPos( &stPos);
	
	// Popupメニュー表示
	int nResult = ::TrackPopupMenu( hPopup, TPM_RETURNCMD | TPM_NONOTIFY | TPM_LEFTALIGN | TPM_TOPALIGN, stPos.x, stPos.y, 0, hWnd, NULL);
	
	switch( nResult )
	{
        // 属性コピー
	case ID_OBJECTID_COPY:
		HTREEITEM hItem = GetSelectedItem();
		BSTR strTree = NULL;
		GetItemText( hItem, strTree );
		CComBSTR bstrTree;
		bstrTree.Attach(strTree);
		
		WriteClipBoard(bstrTree);
		::SysFreeString( strTree );
		break;
	}
	
	return TRUE;
}

// HTREEITEM 構造体から CFeatureDef を特定して返す
CFeatureDef* CFeatureTree::GetFeatureDef( HTREEITEM item )
{
	LONG l = (LONG)( 0x4000000 ^ GetItemData( item ) );
	LONG lListCount = m_pFeatureDefList->size();
	if( lListCount < (l - 1) ) return NULL;        // リストのサイズチェック
	std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
	advance( it, l );
	
	return &(*it);
}

// HTREEITEM 構造体から CRowDef を特定して返す
CRowDef* CFeatureTree::GetRowDef( HTREEITEM item )
{
	if( m_bIsLQAttribute == false )
	{
		LONG l = (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( item ) ) ) );
		LONG lListCount = m_pFeatureDefList->size();
		if( lListCount < (l - 1) ) return NULL;        // リストのサイズチェック
		std::list<CFeatureDef>::iterator it = m_pFeatureDefList->begin();
		advance( it, l );
		
		l = (LONG)( 0x2000000 ^ GetItemData( GetParentItem( item ) ) );
		LONG lRelationCount = it->GetRelationalTableDefList()->size();
		if( lRelationCount < (l - 1) ) return NULL; // リストのサイズチェック
		std::list<CRelationalTableDef>::iterator itRelTableDefList = it->GetRelationalTableDefList()->begin();
		advance( itRelTableDefList, l );
		
		l = (LONG)( 0x1000000 ^ GetItemData( item ) );
		LONG lRelDefCount = itRelTableDefList->size();
		if( lRelDefCount < (l - 1) ) return NULL; // リストのサイズチェック
		std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
		advance( itRowDefList, l );
		
		return &(*itRowDefList);
	}
	else
	{
		LONG l = (LONG)( 0x4000000 ^ GetItemData( GetParentItem( GetParentItem( item ) ) ) );
		LONG lListCount = m_pLQDefList->size();
		if( lListCount < (l - 1) ) return NULL;	// リストのサイズチェック
		std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
		advance( it, l );

		l = (LONG)( 0x2000000 ^ GetItemData( GetParentItem( item ) ) );
		LONG lRelationCount = it->GetRelationalTableDefList()->size();
		if( lRelationCount < (l - 1) ) return NULL; // リストのサイズチェック
		std::list<CRelationalTableDef>::iterator itRelTableDefList = it->GetRelationalTableDefList()->begin();
		advance( itRelTableDefList, l );

		l = (LONG)( 0x1000000 ^ GetItemData( item ) );
		LONG lRelDefCount = itRelTableDefList->size();
		if( lRelDefCount < (l - 1) ) return NULL; // リストのサイズチェック
		std::list<CRowDef>::iterator itRowDefList = itRelTableDefList->begin();
		advance( itRowDefList, l );

		return &(*itRowDefList);
	}
}

// HTREEITEM 構造体から CLQRowDef を特定して返す
CLQRowDef* CFeatureTree::GetLQRowDef( HTREEITEM item )
{
	if( item == NULL ) return NULL;

	LONG l = (LONG)( 0x4000000 ^ GetItemData( item ) );
	LONG lListCount = m_pLQDefList->size();
	if( lListCount < (l - 1) || l < 0 ) return NULL;	// リストのサイズチェック
	std::list<CLQRowDef>::iterator it = m_pLQDefList->begin();
	advance( it, l );

	return &(*it);
}
// クリップボードに選択したOIDをコピーします
void CFeatureTree::WriteClipBoard(CComBSTR bstrTree)
{
    USES_CONVERSION;
    LPTSTR str;
    HGLOBAL hGlobal;
    int iLength = 0;
    CString lpStr = OLE2T( bstrTree );
    iLength = bstrTree.Length();
    if (iLength > 1024)
        iLength = 1024;
    hGlobal = ::GlobalAlloc(GHND, 2*( iLength + 1 ));
    if (!hGlobal)
        return;

	/*
	グローバルメモリ領域のロック
 
	グローバルメモリオブジェクトをロックし、
	メモリブロックの最初の1バイトへのポインタを返す。
	*/
    if( !( str = (LPTSTR)::GlobalLock(hGlobal)) )
	{
		GlobalFree(hGlobal);
		return;
	}

	::lstrcpy(str,lpStr);

	/*
	グローバルメモリ領域のロック解除
	　
	指定されたグローバルメモリオブジェクトを解放し、
	そのハンドルを無効にする。
	*/
    ::GlobalUnlock(hGlobal);

	/*
	クリップボードを開く

	クリップボードを開き、
	他のアプリケーションがクリップボードの内容を変更できないようにする。
	他のウィンドウが既にクリップボードを開いている場合、
	OpenClipboard関数は失敗する。
	OpenClipboardの呼び出しに成功するたびに、
	アプリケーションはCloseClipboard関数を呼び出すべき。
	*/
	if( !( ::OpenClipboard(NULL) ) )
	{
		GlobalFree(hGlobal);
		return;
	}

	/*
	クリップボードを空にする
	
	クリップボードを空にし、
	クリップボード内のデータのハンドルを解放する。
	同時に、クリップボードを開いたウィンドウに、
	クリップボードの所有権を与える。
	*/
    if( !( ::EmptyClipboard() ) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}

	/*
	クリップボードに、指定されたデータ形式でデータを格納する。
	データを格納するには、
	呼び出し側のウィンドウがクリップボードの現在のオーナーであること、
	およびOpenClipboard関数を使ってクリップボードを開いておくことが必要。
	
	SetClipboardDataでクリップボードに渡したメモリオブジェクトは、
	それ以降使用できなくなる。
	(よって、必要なら別にバッファを確保してそこに保存しておく)。
	
	SetClipboardDataでクリップボードへの設定が成功した場合、
	グローバルメモリハンドルはWindowsの管理になるので、
	アプリケーションからGlobalFree()してはならない(絶対に)。
	 
	SetClipboardDataを実行した後は、
	メモリブロックはWindowsのものとなるので、
	解放(GlobalFree)したり、
	ロック(GlobalLockしっぱなし)したままであってはいけない。
　　 */
    if( !::SetClipboardData(CF_UNICODETEXT, hGlobal) )
	{
		CloseClipboard();
		GlobalFree(hGlobal);
		return;
	}

	/*
	クリップボードのクローズ

	CloseClipboard関数を呼び出してクリップボードを閉じると、
	他のアプリケーションがクリップボードにアクセスできるようになる。
	CloseClipboardを呼び出した後は、
	クリップボードにオブジェクトを配置しないこと。
　　 */
    ::CloseClipboard();

	return;
}

#include <TDC/useful_headers/str_util.h>

CString CFeatureTree::getXYZAsString(IPointPtr point) const
{
	double x = 0.0, y = 0.0, z = 0.0;
	point->QueryCoords(&x, &y);

	VARIANT_BOOL ZAware = VARIANT_FALSE;
	IZAwarePtr(point)->get_ZAware(&ZAware);
	if (ZAware)
		point->get_Z(&z);

	auto d2cstr = [](double d)->CString {
		char c[128];
		_gcvt(d, 100, c);
		return CString(c);
	};
	CString strVertex;
	strVertex.Format(_T("X=%s Y=%s Z=%s"), d2cstr(x), d2cstr(y), d2cstr(z));

	return strVertex;
}

/////////////////////////////////////////////////////////////////////////////
//
// ツリービューイベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////

// 左クリックイベント
LRESULT CFeatureTree::OnLButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	// [Bug 5490]ツリー表示部にゴミが残るバグ　処理を終えるまで描画をとめる
	SetRedraw( FALSE );
    m_bIsRButtonClick = FALSE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, FALSE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, FALSE );

	SetRedraw( TRUE );
    return lResult;
}

// 右クリックイベント
LRESULT CFeatureTree::OnRButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    m_bIsRButtonClick = TRUE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, FALSE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, FALSE );

    return lResult;
}

// ダブルクリックイベント
LRESULT CFeatureTree::OnLButtonDoubleClick( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
    m_bIsRButtonClick = FALSE;
	LONG lResult = 0;
	if( !m_bIsLQAttribute )
		lResult = OnMouseDown( uMsg, wParam, lParam, bHandled, TRUE );
	else
		lResult = OnLQMouseDown( uMsg, wParam, lParam, bHandled, TRUE );

    return lResult;
}

// 再描画イベント
LRESULT CFeatureTree::OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( !m_bIsLQAttribute )
		HilitSelectedFeatures( TRUE );
	else
	{
		if( m_ipHilitPoint )
			AheDrawPoint( m_cArcHelper.GetScreenDisplay(), m_ipHilitPoint, FALSE, 6, 250, 12, 250, esriSMSSquare, TRUE );

		HilitSelectedLQ( m_ipHilightGeometry, TRUE );
	}
    return 0;
}
// リンク列ハイライトのイベント
// リンク列選択ダイアログからしか呼ばれない前提
LRESULT CFeatureTree::OnSetCurrentFeature( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if( !wParam || lParam <= 0 )
		return 0;

	HTREEITEM hItem = SearchOID( (LPCTSTR)wParam, (LONG)lParam );

	if( !hItem )
		return 0;

	// 選択状態を変更する
	SelectItem( hItem );
	SetFocus();
		
	if( m_bIsLQAttribute )
	{
		HilitSelectedLQ( GetLQRowDef( hItem )->GetShape() );
	}

	// クリックされたアイテムの種類を通知
	return SendMessage( GetParent(), WM_TREESELECTCHANGE, NULL, FEATUREITEM );
}
