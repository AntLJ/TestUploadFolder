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
 * @file Application.cpp
 * @brief CApplicationクラス実装ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * @version $Id$
 */
#include "stdafx.h"
#include "Application.h"
#include "AheLayerFunctions.h"
#include "AheZooMoveFunctions.h"
#include "type_convert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IDocumentPtr CApplication::GetDocument() const
{
	IDocumentPtr ipDoc;
	if( FAILED( p->get_Document( &ipDoc ) ) )
		_ASSERTE(false);

	return ipDoc;
}

IExtensionPtr CApplication::FindExtensionByProgID( LPCTSTR progID ) const
{
	IUIDPtr ipUID( CLSID_UID );

	if( ! ( progID && ipUID ) )
		return NULL;

	// UIDオブジェクトがProgIDをちゃんとCLSID文字列に変換してくれているかどうか不明（というか、あやしい）ので（bug 4748)
	// ProgIDをCLSIDに変換
	CLSID clsid = {0};
	if( SUCCEEDED( CLSIDFromProgID( CComBSTR(progID), &clsid ) ) )
	{
		LPOLESTR lpszString = 0;
		if( SUCCEEDED( StringFromCLSID( clsid, &lpszString ) ) )
			ipUID->put_Value( CComVariant(lpszString) );

		::CoTaskMemFree(lpszString);
	}

	return FindExtensionByCLSID( ipUID );
}

IExtensionPtr CApplication::FindExtensionByCLSID( IUID* ExtensionCLSID ) const
{
	if( ! ExtensionCLSID )
		return NULL;

	IExtensionPtr ipExt;	// 返り値
	if( FAILED( p->FindExtensionByCLSID( ExtensionCLSID, &ipExt ) ) )
		_ASSERTE(false);

	return ipExt;
}

IExtensionPtr CApplication::FindExtensionByName( LPCTSTR extensionName ) const
{
	if( ! extensionName )
		return NULL;

	IExtensionPtr ipExt;	// 返り値
	CComBSTR bstrExtName(extensionName);
	if( FAILED( p->FindExtensionByName( bstrExtName, &ipExt ) ) )
		_ASSERTE(false);

	return ipExt;
}

HWND CApplication::GetHWND() const
{
	OLE_HANDLE hWnd = 0;	// 返り値
	if( FAILED( p->get_hWnd( &hWnd ) ) )
		_ASSERTE(false);

	return (HWND)hWnd;
}

IStatusBarPtr CApplication::GetStatusBar() const
{
	IStatusBarPtr ipRet;	// 返り値
	if( FAILED( p->get_StatusBar( &ipRet ) ) )
		_ASSERTE(false);

	return ipRet;
}

IDispatchPtr CApplication::GetVBE() const
{
	IDispatchPtr ipDisp;	// 返り値
	if( FAILED( p->get_VBE( &ipDisp ) ) )
	{
		// SMWの場合は失敗する
	}
	return ipDisp;
}
/*bool CApplication::GetVisible() const;
sindyErrCode CApplication::PutVisible( bool Visible );*/

IDisplayPtr CApplication::GetDisplay() const
{
	IAppDisplayPtr ipDisp;
	if( FAILED( ((IMxApplicationPtr)p)->get_Display( &ipDisp ) ) )
		_ASSERTE(false);

	return ipDisp;
}

// TOCを構成するデータフレームを取得する
IMapPtr CApplication::GetFocusMap() const
{
	IMapPtr ipMap;
	IMxDocumentPtr ipDoc( GetDocument() );

	if( ! ipDoc )
		return NULL;

	if( FAILED( ipDoc->get_FocusMap( &ipMap ) ) )
		_ASSERTE(false);

	return ipMap;
}

// フォーカスのある地図画面のスクリーンを取得する
IScreenDisplayPtr CApplication::GetFocusMapScreenDisplay() const
{
	IScreenDisplayPtr ipRet;	// 返り値

	IActiveViewPtr ipView( GetFocusMap() );

	if( ! ipView )
		return NULL;

	if( FAILED( ipView->get_ScreenDisplay( &ipRet ) ) )
		_ASSERTE(false);

	return ipRet;
}

// フォーカスのある地図画面のウィンドウハンドルを取得する
HWND CApplication::GetFocusMapHWND() const
{
	HWND hWnd = NULL;	// 返り値

	IScreenDisplayPtr ipDisp( GetFocusMapScreenDisplay() );

	if( ! ipDisp )
		return NULL;

	if( FAILED( ipDisp->get_hWnd( (OLE_HANDLE*)&hWnd ) ) )
		_ASSERTE(false);

	return hWnd;
}

// エディタコンポーネントを取得する
IEditorPtr CApplication::GetEditor() const
{
	static IEditorPtr ipEditor;
	if( ! ipEditor )
	{
		ipEditor = FindExtensionByProgID( _T("sindyEditor.Editor") );
		if( ! ipEditor )
			ipEditor = FindExtensionByProgID( _T("esriEditor.Editor") );
	}

	return ipEditor;
}

// 現在のビューオブジェクトを取得する
IActiveViewPtr CApplication::GetActiveView() const
{
	IActiveViewPtr ipView;
	IMxDocumentPtr ipDoc( GetDocument() );
	if( ipDoc )
		ipDoc->get_ActiveView( &ipView );

	return ipView;
}

// 現在のビューの矩形を取得する
IEnvelopePtr CApplication::GetViewBounds() const
{
	IEnvelopePtr ipEnv;
	IActiveViewPtr ipView( GetActiveView() );
	if( ipView )
		ipView->get_Extent( &ipEnv );

	return ipEnv;
}

// アクティブデータフレームに読み込まれているテーブル・フィーチャクラスを取得する
ITablePtr CApplication::GetTableByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly/* = true*/ ) const // TODO: いるか？？
{
	return AheGetTableByTableName2( GetFocusMap(), lpcszOwnerTableName, nFindCount, bEditableOnly );
}

// TOCに読み込まれているフィーチャクラスを取得する
IFeatureClassPtr CApplication::GetFeatureClassByTableName( LPCTSTR lpcszOwnerTableName, int& nFindCount, bool bEditableOnly/* = true*/ ) const // TODO: いるか？？
{
	IFeatureClassPtr ipClass;
	ITablePtr ipTable = GetTableByTableName( lpcszOwnerTableName, nFindCount, bEditableOnly );
	if( ipTable != NULL )
		ipTable->QueryInterface( IID_IFeatureClass, (void**)&ipClass );

	return ipClass;
}

// デバイス座標 -> マップ座標変換
IPointPtr CApplication::ToMapPoint( IActiveView* ipView, long X, long Y ) const
{
	if( ! ipView )
		return NULL;

	IPointPtr ipPoint;	// 返り値

	IScreenDisplayPtr ipDisp;
	ipView->get_ScreenDisplay( &ipDisp );

	if( ! ipDisp )
		return NULL;

	IDisplayTransformationPtr ipDispTrans;
	ipDisp->get_DisplayTransformation( &ipDispTrans );

	if( ! ipDispTrans )
		return NULL;

	if( FAILED( ipDispTrans->ToMapPoint( X, Y, &ipPoint ) ) )
		_ASSERTE(false);

	return ipPoint;
}

// Pixel -> マップ距離変換
double CApplication::ToMapUnits( IActiveView* ipView, long nPixel ) const
{
	if( ! ( ipView && 0 < nPixel ) )
		return NULL;

	double dRet = -1.0;	// 返り値

	IScreenDisplayPtr ipDisp;
	ipView->get_ScreenDisplay( &ipDisp );
	if( ipDisp )
	{
		IDisplayTransformationPtr ipTrans;
		ipDisp->get_DisplayTransformation( &ipTrans );
		if( ipTrans )
		{
			RECT pRect = {0,0,0,0};
			if( SUCCEEDED( ipTrans->get_DeviceFrame( &pRect ) ) )
			{
				IEnvelopePtr ipEnv;
				ipTrans->get_FittedBounds( &ipEnv );
				if( ipEnv )
				{
					double dRealWorldDisplayExtent = 0.0;
					if( SUCCEEDED( ipEnv->get_Width( &dRealWorldDisplayExtent ) ) )
						dRet = ( dRealWorldDisplayExtent * static_cast<double>(nPixel) ) / static_cast<double>(pRect.right - pRect.left);
				}
			}
		}
	}
	_ASSERTE( 0 <= dRet );

	return dRet;
}

// キーコードでの各種画面操作を行う
bool CApplication::KeyZooMove( IActiveView* ipView, long lKeyCode, long lShift ) const
{
	return AheKeyZooMove( ipView, lKeyCode, lShift );
}

// スナップする
bool CApplication::Snap( IPoint* ipPoint ) const
{
	VARIANT_BOOL vaSnapped = VARIANT_FALSE;
	ISnapEnvironmentPtr ipSnap( GetEditor() );
	if( ipSnap )
		ipSnap->SnapPoint( ipPoint, &vaSnapped );

	return VB2bool(vaSnapped);
}

// 編集開始状態を取得する
esriEditState CApplication::GetEditState() const
{
	esriEditState emState = esriStateNotEditing;	// 返り値
	IEditorPtr ipEditor = GetEditor();

	if( ipEditor )
	{
		if( FAILED( ipEditor->get_EditState( &emState ) ) )
			_ASSERTE(false);
	}

	return emState;
}

HRESULT CApplication::StartOperation()
{
	HRESULT hr = S_OK; // 返り値
	IEditorPtr ipEditor( GetEditor() );
	if( ipEditor )
	{
		if( FAILED( hr = ipEditor->StartOperation() ) )
			_ASSERTE( SUCCEEDED(hr) );
		else
			m_bIsStartEditOperation = true;
	}
	else
		hr = E_FAIL;

	return hr;
}

HRESULT CApplication::StopOperation( LPCTSTR lpcszMenuText )
{
	HRESULT hr = S_OK;	// 返り値
	IEditorPtr ipEditor( GetEditor() );

	if( ipEditor )
	{
		if( FAILED( hr = ipEditor->StopOperation( CComBSTR(lpcszMenuText) ) ) )
			_ASSERTE( SUCCEEDED(hr) );
		else
			m_bIsStartEditOperation = false;
	}
	else
		hr = E_FAIL;

	return hr;
}

HRESULT CApplication::AbortOperation()
{
	HRESULT hr = S_OK;	// 返り値

	IEditorPtr ipEditor( GetEditor() );

	if( ! ipEditor )
		return E_FAIL;

	// 一度 Stop してから Undo し、そのスタックを削除する
	if( SUCCEEDED( hr = ipEditor->StopOperation( CComBSTR(_T("TMP")) ) ) )
	{
		m_bIsStartEditOperation = false;

		IOperationStackPtr ipOpStack;
		if( SUCCEEDED( hr = ((IMxDocumentPtr)GetDocument())->get_OperationStack( &ipOpStack ) ) )
		{
			// [bug 2065] 編集キャンセル時の再描画をさせないようにIWorkspaceEditを使用する
			IWorkspacePtr ipEditWorkspace;
			IWorkspaceEditPtr ipWorkspaceEdit;
			if( SUCCEEDED( hr = ipEditor->get_EditWorkspace( &ipEditWorkspace ) ) )
			{
				if( SUCCEEDED( hr = ipEditWorkspace->QueryInterface( IID_IWorkspaceEdit, (void**)&ipWorkspaceEdit ) ) )
				{
					if( SUCCEEDED( hr = ipWorkspaceEdit->UndoEditOperation() ) )
					{
						long lOpCount = -1;
						if( SUCCEEDED( hr = ipOpStack->get_Count( &lOpCount ) ) )
						{
							if( SUCCEEDED( hr = ipOpStack->Remove( lOpCount - 1 ) ) )
							{
							}
						}
					}
				}
			}
		}
	}
	_ASSERTE( SUCCEEDED(hr) );

	return hr;
}

ISiNDYRulePtr CApplication::GetRule() const
{
	if( ! m_ipRule )
		m_ipRule.CreateInstance( CLSID_SiNDYRuleObj );

	return m_ipRule;
}
