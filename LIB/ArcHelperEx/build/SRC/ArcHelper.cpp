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
* @file ArcHelper.cpp
*
* @brief IApplicationに関連する処理を行うArcHelperの実装ファイル
*/
#include "StdAfx.h"
#include "ArcHelper.h"
#include "AheGlobals.h"
#include "AheGlobalsX.h"
#include "InifileHelper.h"
#include "Map.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace {
#ifndef SINDY_FOR_ENGLISH
	LPCTSTR WARN_DO_INITIALIZATION      = _T("CArcHelper::Init() で初期化してください");
	LPCTSTR INIT_WARN                   = _T("警告");
	LPCTSTR MESSAGE_BFIRSTONLY_IS_FALSE = _T("bFirstOnly == FALSE のコードは実装していません");
	LPCTSTR NOT_IMPLEMENTED             = _T("未実装");
#else
	LPCTSTR WARN_DO_INITIALIZATION      = _T("Do initialization at CArcHelper::Init().");
	LPCTSTR INIT_WARN                   = _T("Warning");
	LPCTSTR MESSAGE_BFIRSTONLY_IS_FALSE = _T("It isn't implemented ( bFirstOnly == FALSE ).");
	LPCTSTR NOT_IMPLEMENTED             = _T("Not implemented.");

#endif // SINDY_FOR_ENGLISH
}

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CArcHelper::CArcHelper()
{
}

CArcHelper::~CArcHelper()
{
}

/////////////////////////////////////////////////////////////////////////////
//
// 初期化に関する関数
//
/////////////////////////////////////////////////////////////////////////////
void CArcHelper::Init( IApplicationPtr ipApp )
{
	ATLASSERT( ipApp != NULL );

	((CComPtr<IApplication>&)*this) = (IApplication*)ipApp;

	GetEditor()->EnableUndoRedo( VARIANT_TRUE );	// UndoRedo を有効に
}

void CArcHelper::Init( IEditorPtr ipEditor )
{
	ATLASSERT( ipEditor != NULL );

	IApplicationPtr ipApp;
	ipEditor->get_Parent( &ipApp );
	if( ipApp )
		Init( ipApp );
}

void CArcHelper::Init( IDispatchPtr hook )
{
	ATLASSERT( hook != NULL );

	IApplicationPtr ipApp( hook );
	if( ipApp )
		Init( ipApp );
	else {
		IEditorPtr ipEditor( ipApp );
		if( ipEditor )
			Init( ipEditor );
		else
			_ASSERTE(false); // サポートしていない
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// 初期化方法の判定用関数
//
/////////////////////////////////////////////////////////////////////////////
BOOL CArcHelper::CheckInit() const
{
	if( ! p )
	{
		MessageBox( GetHWND(),	WARN_DO_INITIALIZATION, INIT_WARN, MB_OK );

		return FALSE;
	}
	return TRUE;
}

/// IEditor が使用可能かどうかの判定用
BOOL CArcHelper::CheckEditor() const
{
	if( ! GetEditor() )
	{
		_ASSERTE(false);

		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
//
// オブジェクト取得関数
//
/////////////////////////////////////////////////////////////////////////////
IScreenDisplayPtr CArcHelper::GetScreenDisplay( BOOL bMapDisplay/* = TRUE*/ ) const
{
	IActiveViewPtr ipView( GetActiveView(bMapDisplay) );
	IScreenDisplayPtr ipDisp;
	// [Bug 6141]テンプレート選択でテンプレートを切替えると落ちるバグ対応
	if( ipView )
		ipView->get_ScreenDisplay( &ipDisp );

	return ipDisp;
}
IGraphicsContainerPtr CArcHelper::GetGraphicsContainer( BOOL bMapDisplay/* = TRUE*/ ) const
{
	IGraphicsContainerPtr ipContainer;
	IActiveViewPtr ipActiveView( GetActiveView( bMapDisplay ) );
	// [Bug 6141]テンプレート選択でテンプレートを切替えると落ちるバグ対応
	if( ipActiveView )
		ipActiveView->get_GraphicsContainer( &ipContainer );

	return ipContainer;
}

// TOCから現在選択されているレイヤを取得する
ILayerPtr CArcHelper::GetSelectedLayer( bool bIsEditable /* = true */) const
{
	ILayerPtr ipLayer;
	AheGetSelectedLayer( GetDocument(), &ipLayer, bIsEditable );
	return ipLayer;
}

/////////////////////////////////////////////////////////////////////////////
//
// その他の関数
//
/////////////////////////////////////////////////////////////////////////////

// センタリングと拡大率変更
void CArcHelper::SetCenter( IPointPtr piPoint, DOUBLE dScale/*  = -1.0 */, BOOL bMapDisplay/* = TRUE */ ) const
{
	IActiveViewPtr ipActiveView = GetActiveView( bMapDisplay );
	IEnvelopePtr ipEnv = NULL;

	if ( ipActiveView != NULL ) {
		ipActiveView->get_Extent( &ipEnv );
		ipEnv->CenterAt( piPoint );
		ipActiveView->put_Extent( ipEnv );

		if( dScale > 0.0 )
			GetMap()->put_MapScale( dScale );

		// 再描画
		ipActiveView->Refresh();
		GetScreenDisplay()->UpdateWindow();
	}
}

void CArcHelper::RefreshContentsView() const
{
	// TOC再描画
	IContentsViewPtr ipContentsView;
	IMxDocumentPtr ipMxDoc( GetDocument() );
	LONG lContentsCount;
	CComVariant va;
	va.vt = VT_NULL;

	if( ipMxDoc != NULL ) 
	{
		ipMxDoc->get_ContentsViewCount( &lContentsCount );
		for( LONG i = 0; i < lContentsCount; i++ )
		{
			ipMxDoc->get_ContentsView( i, &ipContentsView );
			if( ipContentsView != NULL ) ipContentsView->Refresh( va );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
//
// スナップに関する関数
//
/////////////////////////////////////////////////////////////////////////////
// スナップポイントを取得する
BOOL CArcHelper::Snap( IPointPtr piPoint ) const
{
	VARIANT_BOOL vaIsSnapped = VARIANT_FALSE;

	/// スナップ環境取得
	ISnapEnvironmentPtr ipSnapEnv( GetEditor() );

	if ( ipSnapEnv != NULL )
		ipSnapEnv->SnapPoint( piPoint, &vaIsSnapped );

	if( vaIsSnapped == VARIANT_TRUE )
		return TRUE;

	return FALSE;
}
// スナップポイントを取得する
BOOL CArcHelper::Snap( IPointPtr piPoint, IPoint** ipSnappedPoint, BOOL bFirstOnly ) const
{
	IPointCollectionPtr ipSnappedPoints = NULL;
	LONG lSnappedPointCount = 0;
	sindyeSnappingType type;

	///  bFirstOnly == FALSE のコードを実装したら外してください
	if( bFirstOnly == FALSE ) 
	{
		::MessageBox( ::GetActiveWindow(),
			MESSAGE_BFIRSTONLY_IS_FALSE,
			NOT_IMPLEMENTED, MB_OK );
		return FALSE;
	}

	if( Snap( piPoint, &ipSnappedPoints, &type ) == TRUE ) {
	
		ipSnappedPoints->get_PointCount( &lSnappedPointCount );

		for( LONG i = 0; i < lSnappedPointCount; i++ ) {
			IPointPtr ipSnappedPointTmp = NULL;
			ipSnappedPoints->get_Point( i, &ipSnappedPointTmp );
			if( i == 0 && bFirstOnly == TRUE ) {
				(*ipSnappedPoint) = ipSnappedPointTmp;
				(*ipSnappedPoint)->AddRef();
			}
		}

		return TRUE;
	} else 
		return FALSE;
}
// スナップポイントを取得する
BOOL CArcHelper::Snap(IPointPtr					piPoint, 
						  IPointCollection**		ipSnappedPoints,
						  sindyeSnappingType*		SnapType,
						  BOOL						bCheckClass,
						  BOOL						bCheckHitType,
						  esriGeometryHitPartType*	HitType) const
{
	DOUBLE		dSnapTolerance = 0.0;
	BOOL		bSnapped = FALSE;

	/// コレクション作成
	(*ipSnappedPoints) = NULL;
	::CoCreateInstance( CLSID_Multipoint, NULL, CLSCTX_ALL, IID_IPointCollection, (void **)&(*ipSnappedPoints) );
	// スナップポイントは一つ一つは個々のフィーチャクラスの空間参照を
	// 付与すべきだが、Multipointにまとめるとなるとデータフレームの空間参照
	// しかセットする余地がない…
	ISpatialReferencePtr ipMapSpRef;
	GetMap()->get_SpatialReference( &ipMapSpRef );
	((IGeometryPtr)(*ipSnappedPoints))->putref_SpatialReference( ipMapSpRef );

	/// ArcMap ベースの場合
	if( CheckApp() || CheckEditor() ) {

		LONG					lSnapCount = 0;
		esriSnapToleranceUnits	type;

		/// スナップ環境取得
		ISnapEnvironmentPtr ipSnapEnv( GetEditor() );

		if ( ipSnapEnv == NULL )
			return FALSE;

		/// スナップエージェントの数
		ipSnapEnv->get_SnapAgentCount( &lSnapCount );
		/// スナップ距離
		ipSnapEnv->get_SnapTolerance( &dSnapTolerance );
		ipSnapEnv->get_SnapToleranceUnits( &type );
		if( type == esriSnapTolerancePixels ) {
			DOUBLE d = AheConvertPixelsToMapUnits( GetScreenDisplay(), dSnapTolerance );
			dSnapTolerance = d;
		}
//		/// スナップ矩形作成
//		IGeometryPtr ipGeom( GetEnvelope( piPoint, dSnapTolerance ) );

		for( LONG i = 0; i < lSnapCount; i++ ) {
			ISnapAgentPtr ipSnapAgent;
			VARIANT_BOOL vaIsSnapped = VARIANT_FALSE;
			
			/// ポイントコピー
			IClonePtr ipClone( piPoint ), ipClone2 = NULL;
			ipClone->Clone( &ipClone2 );
			IPointPtr ipSnappedPoint( ipClone2 );

			ipSnapEnv->get_SnapAgent( i, &ipSnapAgent );
			ipSnapAgent->Snap( NULL, ipSnappedPoint, dSnapTolerance, &vaIsSnapped );
			/// スナップした
			if( vaIsSnapped == VARIANT_TRUE ) {

				bSnapped = TRUE;

				ATLASSERT( SnapType != NULL );

#ifdef USE_ROADINTERSECTIONSNAPAGENT
				/// スナップしたフィーチャ特定
				IRoadIntersectionSnapAgentPtr ipIntersectSnapAgent( ipSnapAgent );
				if( ipIntersectSnapAgent != NULL ) 
					(*SnapType) = sindyeSnapRoadIntersection;
				else
#endif // ifdef IRoadIntersectionSnapAgentPtr
					if( SnapType )
						(*SnapType) = sindyeSnapAny;
				
				/// フィーチャクラスを調べる場合
				if( bCheckClass == TRUE ) {					
					IFeatureSnapAgentPtr ipFeatureSnapAgent( ipSnapAgent );
					IFeatureClassPtr ipFeatureClass = NULL;

					/// 詳細なスナップフィーチャの特定
					if( ipFeatureSnapAgent != NULL )
					{
						ipFeatureSnapAgent->get_FeatureClass( &ipFeatureClass );
						if( SnapType )
						{
							if( AheIsRoadClass( ipFeatureClass ) )
								(*SnapType) = sindyeSnapRoad;
							else if( AheIsNodeClass( ipFeatureClass ) )
								(*SnapType) = sindyeSnapNode;
						}

						/// ヒットタイプの特定
						if( bCheckHitType == TRUE ) {

							ATLASSERT( HitType != NULL );

							ipFeatureSnapAgent->get_HitType( HitType );
						}
					}
				}
						
			} 
		}
	}

	return bSnapped;
}
/////////////////////////////////////////////////////////////////////////////
//
// イベント接続に関する関数
//
/////////////////////////////////////////////////////////////////////////////
// IActiveView に接続
DWORD CArcHelper::ConnectToActiveView( IUnknownPtr ipUnk, DWORD& dwCookie ) const
{
  if (GetActiveView() == NULL)
    return (DWORD)0;

	//Avoid multiple connects
	if (dwCookie)
		return dwCookie;

	//Connect to active view events
	HRESULT hr = AtlAdvise(GetActiveView(), ipUnk, IID_IActiveViewEvents, &dwCookie);
	if (FAILED(hr)) {
		::MessageBox(::GetActiveWindow(),
			_T("Could not connect to ActiveView events"),
			_T("Command Implementing Events"), MB_OK);

		return (DWORD)0;
	}
	return dwCookie;
}

// IActiveView の接続解除
DWORD CArcHelper::DisconnectFromActiveView( DWORD& dwCookie ) const
{
	if ( dwCookie == 0 || GetActiveView() == NULL )
		return dwCookie;

	//Disconnect from ActiveView events
	HRESULT hr = AtlUnadvise(GetActiveView(), IID_IActiveViewEvents, dwCookie);
	if (FAILED(hr)) {
		::MessageBox(::GetActiveWindow(),
			_T("Could not disconnect from ActiveView events"),
			_T("Command Implementing Events"), MB_OK);

		return dwCookie;
	}
	dwCookie = (DWORD)0;

	return dwCookie;
}
