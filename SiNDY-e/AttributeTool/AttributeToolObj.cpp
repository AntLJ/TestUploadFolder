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

// AttributeToolObj.cpp : CAttributeToolObj のインプリメンテーション
#include "stdafx.h"
#include <algorithm>
#include "AttributeTool.h"
#include "AttributeToolObj.h"
#include <WinLib/WaitCursor.h>
#include "Map.h"

/////////////////////////////////////////////////////////////////////////////
//
// IActiveViewEvents インターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::ContentsChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ContentsCleared()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ItemAdded(VARIANT Item)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ItemDeleted(VARIANT Item)
{
  return S_OK;	
}
STDMETHODIMP CAttributeToolObj::ItemReordered(VARIANT Item, LONG toIndex)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::SelectionChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::ViewRefreshed(IActiveView * View, esriViewDrawPhase phase, VARIANT data, IEnvelope * Envelope)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
{
	switch (phase)
	{
		case esriViewNone:
			break;
		case esriViewBackground:
			break;
		case esriViewGeography:
			break;
		case esriViewGeoSelection:
			break;
		case esriViewGraphics:
			break;
		case esriViewGraphicSelection:
			break;
		case esriViewForeground:
			if( m_bDrawingSelectArea )
			{
				OLE_HANDLE hDC = 0;
				if( Display )
					Display->get_hDC( &hDC );
				m_cFeedback.ResetSearchArea( hDC );
			}
			break;
		default:
			break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::AfterItemDraw(SHORT Index, IDisplay * Display, esriDrawPhase phase)
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::FocusMapChanged()
{
  return S_OK;
}
STDMETHODIMP CAttributeToolObj::SpatialReferenceChanged()
{
  return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// ICommandインターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_Enabled(VARIANT_BOOL * Enabled)
{
	if (Enabled == NULL)
		return E_POINTER;

	// 属性表示編集ツールは、常にEnable
	*Enabled = VARIANT_TRUE;
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Checked(VARIANT_BOOL * Checked)
{
	if (Checked == NULL)
		return E_POINTER;
		
	*Checked = VARIANT_FALSE;	// Tool is not checked
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Name(BSTR * Name)
{
	if (Name == NULL)
		return E_POINTER;
		
	if( m_lSubType == 1 )
		*Name = ::SysAllocString(L"SiNDYe_AttributeTool");
	else if( m_lSubType == 2 )
		*Name = ::SysAllocString(L"SiNDYe_LQAttributeTool");

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Caption(BSTR * Caption)
{
	if (Caption == NULL)
		return E_POINTER;
	
	CComBSTR bstrName, bstrLQName;
	bstrName.LoadString( IDS_NAME );
	bstrLQName.LoadString( IDS_LQNAME );
	if( m_lSubType == 1 )
		*Caption = ::SysAllocString( bstrName );
	else if( m_lSubType == 2 )
		*Caption = ::SysAllocString( bstrLQName );

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Tooltip(BSTR * Tooltip)
{
	if (Tooltip == NULL)
		return E_POINTER;
		
	CComBSTR bstrToolTip, bstrLQToolTip;
	bstrToolTip.LoadString( IDS_TOOLTIP );
	bstrLQToolTip.LoadString( IDS_LQTOOLTIP );
	if( m_lSubType == 1 )
		*Tooltip = ::SysAllocString(bstrToolTip);
	else if( m_lSubType == 2 )
		*Tooltip = ::SysAllocString(bstrLQToolTip);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Message(BSTR * Message)
{
	if( !Message )
		return E_POINTER;

	CComBSTR bstrMessage, bstrLQMessage;
	bstrMessage.LoadString( IDS_MESSAGE );
	bstrLQMessage.LoadString( IDS_LQMESSAGE );
	if( m_lSubType == 1 )
		*Message = ::SysAllocString(bstrMessage);
	else if( m_lSubType == 2 )
		*Message = ::SysAllocString(bstrLQMessage);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_HelpFile(BSTR * HelpFile)
{
	if( !HelpFile )
		return E_POINTER;

	(*HelpFile) = ::SysAllocString(_HELPFILENAME);
		
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_HelpContextID(LONG * helpID)
{
	if( !helpID )
		return E_POINTER;
		
	if( m_lSubType == 1 )
		(*helpID) = _HELPID_ATTRIBUTETOOL;
	else if( m_lSubType == 2 )
		(*helpID) = _HELPID_LQATTRIBUTETOOL;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Bitmap(OLE_HANDLE * Bitmap)
{
	if( !Bitmap )
		return E_POINTER;
		
	if( m_lSubType == 1 )
		*Bitmap = (OLE_HANDLE) m_bitmap;
	else if( m_lSubType == 2 )
		*Bitmap = (OLE_HANDLE) m_LQbitmap;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::get_Category(BSTR * categoryName)
{
	if( !categoryName )
		return E_POINTER;
		
	*categoryName = ::SysAllocString(L"SiNDY-e Tool");
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnCreate(IDispatch * hook)
{
	// ArcMapからでも使用できるように
	IDispatchPtr ipDisp( hook );
	m_cArcHelper.Init( ipDisp );
	LoadProperty();
	return S_OK;
} 
STDMETHODIMP CAttributeToolObj::OnClick()
{
	// DocumentEvents, ActiveViewEventsに接続
	m_cDocumentEvents = m_cArcHelper.GetDocument();
	m_cDocumentEvents.Advise( GetUnknown() );
	m_cActiveViewEvents = m_cArcHelper.GetActiveView();
	m_cActiveViewEvents.Advise( GetUnknown() );

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// ICommandSubTypeインターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::SetSubType(LONG SubType)
{
	m_lSubType = SubType;
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::GetCount(LONG * Count)
{
	*Count = 2;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// IToolインターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_Cursor(OLE_HANDLE * Cursor)
{
	if( !Cursor )
		return E_POINTER;

	*Cursor = (OLE_HANDLE)(*m_pCurPointer);

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// クリックポイント取得
	m_lMouseDownX = X;
	m_lMouseDownY = Y;

	// 選択範囲のフィードバック中ならフィードバックをとめる
	if( m_bDrawingSelectArea )
	{
		// 選択矩形の描画停止
		m_cFeedback.StopDrawSearchArea();
		m_bDrawingSelectArea = false;
	}

	if (Button == 1) {	
		// クリックされた点を現在の座標系で取得
		AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &m_ipMouseDownPoint );	
		m_bIsMouseDown = true;
	}
	else if( Button == 2 )
	{
		// 左クリックが押されていないときだけ、右クリックをオンにする
		if( !m_bIsMouseDown )
			m_bIsRightDown = true;
		// 右クリックで矩形選択キャンセルしたときには、左ボタンを押していても、押していない状態にしておく
		m_bIsMouseDown = false;
		if( m_ipFeedbackEnv )
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
		}
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IPointPtr ipPoint;
	AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);

	if( m_bFreeSelectMode )
	{
		if( m_lSubType == 1 )
			m_pCurPointer = &m_curFreeSel;
		else if( m_lSubType == 2 )
			m_pCurPointer = &m_LQcurFreeSel;
	}

	if( m_bIsMouseDown )
	{
		/// マウスポインタ変更（矩形選択）
		if( !m_bFreeSelectMode )
		{
			if( m_lSubType == 1 )
				m_pCurPointer = &m_curMove;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcurMove;
		}

		/// 選択矩形描画
		if( !m_bFreeSelectMode )
		{
			if( Button == 1 || Button == 3)
			{
				if (m_ipFeedbackEnv == NULL)
				{
					m_ipFeedbackEnv.CreateInstance(CLSID_NewEnvelopeFeedback);
					m_ipFeedbackEnv->putref_Display(m_cArcHelper.GetScreenDisplay());
					m_ipFeedbackEnv->Start(m_ipMouseDownPoint);
				}
				m_ipFeedbackEnv->MoveTo(ipPoint);
			}
		}
	} 
	else
	{
		// マウスポインタ
		if( !m_bFreeSelectMode )
		{
			if( m_lSubType == 1 )
				m_pCurPointer = &m_cursor;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcursor;
		}

		// 選択領域矩形描画
		if( !m_bFreeSelectMode )
		{
			if( m_bDrawingSelectArea )
			{
				// 描画
				if( !m_cFeedback.DrawSearchArea( ipPoint ) )
					m_cFeedback.StartDrawSearchArea( m_cArcHelper.GetScreenDisplay(), ipPoint, AheGetTolerance() );
			}
			else
			{
				m_cFeedback.StartDrawSearchArea( m_cArcHelper.GetScreenDisplay(), ipPoint, AheGetTolerance() );
				m_bDrawingSelectArea = true;
			}
		}
		else
			m_cSketchFeedback.MoveTo( ipPoint );
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// コンテキストメニューを出すかどうか
	// OnMouseUpの最後でやると自由選択モードのときに最後の1点を消したときに
	// IsStartingFeedback()がfalseになっており、コンテキストメニューが出てしまうため、先頭で判定しておく
	// また、左クリックを押していなくて、右クリックを押しているときだけにする（まー、当たり前なのだが、これが重要）
	if( Button == 2 && m_cSketchFeedback.IsStartingFeedback() == FALSE && !m_bIsMouseDown && m_bIsRightDown)
		m_bShowContextMenu = true;
	else
		m_bShowContextMenu = false;

	if( m_bIsRightDown && Button == 2)
		m_bIsRightDown = false;

	// 念のためにマウスが押されているかどうかちゃんとチェック（ダブルクリックのときなどを考慮して）
	IPointPtr ipPoint;
	AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);
	if( m_bIsMouseDown && Button == 1)
	{
		m_bIsMouseDown = false;
		if( !m_bFreeSelectMode )
		{
			// 選択アイテムの個数をここでリセット
			m_lSearchItemNum = 0;
			if( m_lSubType == 1 )
			{
				m_pCurPointer = &m_cursor;
				OnAttrMouseUp(Button, Shift, X, Y );
			}
			else if( m_lSubType == 2 )
			{	
				m_pCurPointer = &m_LQcursor;
				OnLQMouseUp(Button, Shift, X, Y );
			}
		}
		else
		{
			// ポリゴン選択1点目はフィードバック開始処理
			if( !m_cSketchFeedback.IsStartingFeedback() )
			{
				m_cSketchFeedback.SetDisplay( m_cArcHelper.GetScreenDisplay() );
				m_cSketchFeedback.Start( ipPoint, sindyeSketch_Polygon );
			}
			// エリアポイント追加（2点目以降）
			else
				m_cSketchFeedback.AddPoint( ipPoint );
		}
	}
	// 右クリックで点削除。RemovePointに渡す点は消す点ではなくて、今いる点を渡す。ちゃんと、最後の点を消してくれる
	if( Button == 2 && m_bFreeSelectMode == true )
		m_cSketchFeedback.RemovePoint( ipPoint );

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnDblClick()
{
	if( m_bFreeSelectMode )
	{
		// フィードバックから形状を取得
		// 右ダブルクリックでも一応問題なしにしておく
		m_ipSelectGeom = m_cSketchFeedback.Stop();
		if( m_ipSelectGeom )
		{
			((IPolygonPtr)m_ipSelectGeom)->SimplifyPreserveFromTo();
			LONG lRingCount;
			((IPolygonPtr)m_ipSelectGeom)->get_ExteriorRingCount( &lRingCount );
			
			// 不正形状
			CComBSTR bstrSelectInvalid, bstrToolName;
			if( m_lSubType == 1 )
				bstrToolName.LoadString( IDS_NAME );
			else
				bstrToolName.LoadString( IDS_LQNAME );
			bstrSelectInvalid.LoadString( IDS_SELECTINVALID );
			if( lRingCount > 1 )
				AheMessageBox( CString( bstrSelectInvalid ), CString( bstrToolName ), MB_OK, WARNLEVEL_LOW );
			else if( lRingCount == 1 )
			{
				// 選択アイテムの個数をここでリセット
				m_lSearchItemNum = 0;
				if( m_lSubType == 1 )
				{
					m_pCurPointer = &m_cursor;
					OnAttrMouseUp(0, m_lShift, 0, 0 );
				}
				else if( m_lSubType == 2 )
				{	
					m_pCurPointer = &m_LQcursor;
					OnLQMouseUp(0, m_lShift, 0, 0 );
				}
			}
		}
		// 渡し終わったら初期化しておく
		m_ipSelectGeom = NULL;
	}
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnKeyDown(LONG keyCode, LONG Shift)
{
	BOOL blHandled = false;
	IEnvelopePtr ipSearchExtent;
	switch( keyCode )
	{
	case VK_SHIFT:
		m_lShift = 1;
		break;
	case VK_SPACE:
		// フラグとマウスカーソル変更
		if( m_bFreeSelectMode )
		{
			m_bFreeSelectMode = false;
			if( m_lSubType == 1 )
				m_pCurPointer = &m_cursor;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcursor;
		}
		else
		{
			m_bFreeSelectMode = true;
			if( m_lSubType == 1 )
				m_pCurPointer = &m_curFreeSel;
			else if( m_lSubType == 2 )
				m_pCurPointer = &m_LQcurFreeSel;
		}
		SetCursor( *m_pCurPointer );
		// 選択範囲のフィードバック中ならフィードバックをとめる
		if( m_bDrawingSelectArea )
		{
			// 選択矩形の描画停止
			m_cFeedback.StopDrawSearchArea();
			m_bDrawingSelectArea = false;
		}
		// ここでbreakしない、初期化も行っておく
	case VK_ESCAPE:
		// フィードバック途中ならスペースキー押されても、フィードバックとめる
		// 選択矩形の描画停止
		if( m_ipFeedbackEnv )
		{
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
		}
		if( m_cSketchFeedback.IsStartingFeedback() )
			m_cSketchFeedback.Stop();
		// 初期化
		m_bIsMouseDown = false;
		m_bIsRightDown = false;
		m_ipSelectGeom = NULL;
		break;
	default:
		AheZooMoveKeyDownProc( m_cArcHelper.GetMap(), keyCode, Shift, &blHandled);
		break;
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnKeyUp(LONG keyCode, LONG Shift)
{
	if( keyCode == VK_SHIFT )
		m_lShift = 0;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL * handled)
{
	if (handled == NULL)
		return E_POINTER;

	if ( m_bShowContextMenu )
		(*handled) = VARIANT_FALSE;
	else
		(*handled) = VARIANT_TRUE;

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::Refresh(OLE_HANDLE hDC)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::Deactivate(VARIANT_BOOL * complete)
{
	if( !complete )
		return E_POINTER;

	// ツールの切替の際にダイアログを開いていたら、ダイアログを閉じなくてはいけない
	if( m_ipAttributeDlg != NULL )
		m_ipAttributeDlg->Close();

	// IActiveViewEvents接続解除
	m_cActiveViewEvents.Unadvise();
	// IDocumentEvents接続解除
	m_cDocumentEvents.Unadvise();

	// フィードバックは必ず止めること
	if( m_bDrawingSelectArea ) {
		// 選択矩形の描画停止
		m_cFeedback.StopDrawSearchArea();
		m_bDrawingSelectArea = false;
	}
	
	*complete = VARIANT_TRUE;
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
//
// IDocumentEventsインターフェース
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::ActiveViewChanged()
{
	// データビュー⇔レイアウトビューなどのビューの切り替えがあった場合に、ActiveViewEventsを設定しなおす
	// IActiveViewEvents接続解除
	m_cActiveViewEvents.Unadvise();
	// IActiveViewEvents接続しなおし
	m_cActiveViewEvents = m_cArcHelper.GetActiveView();
	m_cActiveViewEvents.Advise( GetUnknown() );

	// フィードバックしていたら止める
	m_cFeedback.StopDrawSearchArea();

	return S_OK;
}
STDMETHODIMP CAttributeToolObj::MapsChanged()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::NewDocument()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::OpenDocument()
{
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::BeforeCloseDocument(VARIANT_BOOL * abortClose)
{
	if (abortClose == NULL)
		return S_OK;
		
	return S_OK;
}
STDMETHODIMP CAttributeToolObj::CloseDocument()
{
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolObjメンバ関数
//
/////////////////////////////////////////////////////////////////////////////
// フィーチャ選択専用のOnMouseUp関数
void CAttributeToolObj::OnAttrMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IGeometryPtr ipGeom;
	if( !m_bFreeSelectMode )
	{
		IPointPtr ipPoint;
		AheQueryMapPoint( m_cArcHelper.GetScreenDisplay(), X, Y, &ipPoint);

		// 矩形選択
		if( m_ipFeedbackEnv ) 
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
			ipGeom = ipSearchExtent;
		}

		// MouseDown,MouseUpの座標の差が小さい時（X,Yの差の合計値が2以下）には、矩形選択ではなく
		// クリックしたことにしておく。[Bug4319]たまに選択できないときがあるへの対応
		if( abs( m_lMouseDownX - X ) + abs( m_lMouseDownY - Y ) <= 2 )
		{
			UINT unTorelance = AheGetTolerance();
			// Arc8.2のバグに対応するため[Bug2458]、1.5倍くらいの範囲で検索する
			// Arc9でも、ましにはなったが完全には修正されていないため、このままにしておく
			unTorelance = unTorelance + ( unTorelance / 2 );
			// 選択エリアとして設定されているpixel値を、現在の拡大モードで変換する
			DOUBLE dSearchRadius = AheConvertPixelsToMapUnits( m_cArcHelper.GetScreenDisplay(), unTorelance );
			// 選択フィーチャ格納コンポーネットの初期化
			ITopologicalOperatorPtr ipTopo( ipPoint );
			// 入力されたポイントを選択エリア半径でバッファし、選択エリアを作成
			ipTopo->Buffer( dSearchRadius, &ipGeom );
		}
	}
	else
		ipGeom = m_ipSelectGeom;

	// [Bug 4872]選択直後に固まるというバグがここに起因するものなのかどうかわからないが、何らかの原因で、
	// 選択形状がとってこれない場合に、この先に進むと固まりそうなため、ここでエラーチェックしておく。
	VARIANT_BOOL vaIsEmpty = VARIANT_TRUE;
	if( ipGeom )
		ipGeom->get_IsEmpty( &vaIsEmpty );
	if( !ipGeom || vaIsEmpty )
		return;

	// フィーチャ選択中にはカーソルを砂時計にしておく（このif文抜けたら自動的に、前のカーソルに戻る）
	CWaitCursor cWaitCursor;

	// 検索対象のレイヤリストを取得する
	std::list< CAdapt< ILayerPtr > > listLayers = GetSearchLayer(false);
	// 1つもレイヤなかったら終了
	if( listLayers.size() < 1 ) 
		return;

	// 取得してきたレイヤでフィーチャ検索
	bool bSelected = false;
	for( std::list< CAdapt< ILayerPtr > >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
	{
		bool bTempSelected = SelectByShapeFromLayer( ipGeom, (*it).m_T );
		if( bTempSelected && !bSelected )
			bSelected = true;
		// [Bug 5343]対応で、選択したフィーチャが1000を超えたら、ダイアログ閉じてしまう
		if( m_lSearchItemNum > 1000 )
		{
			CComBSTR bstrOverMessage, bstrToolName;
			bstrToolName.LoadString( IDS_NAME );
			bstrOverMessage.LoadString( IDS_SELECTITEMOVER );
			AheMessageBox( CString( bstrOverMessage ), bstrToolName, MB_OK, WARNLEVEL_LOW );
			GetAttributeDlg()->Close();
			return;
		}
	}

	if( bSelected )
	{
		// 属性表示
		GetAttributeDlg()->put_DoOperation(VARIANT_TRUE);
		GetAttributeDlg()->DoModeless();
	}
}

//リンク列選択専用のOnMouseUp関数
void CAttributeToolObj::OnLQMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	IGeometryPtr ipGeom;
	if( !m_bFreeSelectMode )
	{
		// クリックではリンク列は拾えない仕様（いいのかな）
		if( m_ipFeedbackEnv ) 
		{
			IEnvelopePtr ipSearchExtent;
			m_ipFeedbackEnv->Stop(&ipSearchExtent);
			m_ipFeedbackEnv = NULL;
			ipGeom = ipSearchExtent;
		}
		// クリックした場合にはそのまま終了
		if( !ipGeom )
			return;
	}
	else
		ipGeom = m_ipSelectGeom;

	// [Bug 4872]選択直後に固まるというバグがここに起因するものなのかどうかわからないが、何らかの原因で、
	// 選択形状がとってこれない場合に、この先に進むと固まりそうなため、ここでエラーチェックしておく。
	VARIANT_BOOL vaIsEmpty = VARIANT_TRUE;
	if( ipGeom )
		ipGeom->get_IsEmpty( &vaIsEmpty );
	if( !ipGeom || vaIsEmpty )
	{
		CComBSTR bstrErrorSearchGeom, bstrToolName;
		bstrErrorSearchGeom.LoadString( IDS_ERRORSEARCHGEOM );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrErrorSearchGeom, bstrToolName, MB_OK, WARNLEVEL_LOW );
		return;
	}

	// フィーチャ選択中にはカーソルを砂時計にしておく（このif文抜けたら自動的に、前のカーソルに戻る）
	CWaitCursor cWaitCursor;

	// 検索対象のレイヤリストを取得する
	std::list< CAdapt< ILayerPtr > > listLayers = GetSearchLayer(true);
	// 1つも道路リンクなかったら終了
	if( listLayers.size() < 1 ) 
		return;

	// 取得したすべての道路リンクを検索しリンク列見つけてくる
	bool bFind = false, bIsSelected = false;
	for( std::list< CAdapt< ILayerPtr > >::iterator it = listLayers.begin(); it != listLayers.end(); ++it )
	{
		IFeatureRendererPtr ipRend;
		((IGeoFeatureLayerPtr)it->m_T)->get_Renderer( &ipRend );

		// [Bug 6042] シンボル変更ビューツールのリンク列編集ボタンに対応して欲しい
		long lMode = GetEditLinkQueueMode( ipRend );

		// リンク列の種類ごとにループをまわす
		for( long i = sindyeLoadQueueStart; i <= sindyeLoadQueueEnd; i++ )
		{
			// リンク列が表示されているかどうかチェック
			bool bIsHit = false;
			if( m_cLQInfo.GetLoadQueueDrawMode( (sindyeRoadQueueType)i ) & lMode )
			{
				bFind = true;
				// 選択した範囲のジオメトリの中にリンク列があるかどうか調べる
				bool bIsShift = false;
				if( Shift == 1 )
					bIsShift = true;
				bIsHit = SelectTargetLQs( AheGetFeatureClass( it->m_T ), ipGeom, (sindyeRoadQueueType)i, bIsShift );
			}
			if( bIsHit )
				bIsSelected = true;
		}
	}
	
	// ダイアログ表示
	if( bFind )
	{
		GetAttributeDlg()->SetMode( Shift );
		GetAttributeDlg()->put_DoOperation(VARIANT_TRUE);
		GetAttributeDlg()->DoModeless();
	}
	else
	{
		CComBSTR bstrDisplayLinkQueue, bstrToolName;
		bstrDisplayLinkQueue.LoadString( IDS_DISPLAYLINKQUEUE );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrDisplayLinkQueue, bstrToolName, MB_OK, WARNLEVEL_HIGH );
		return;
	}

	if( !bIsSelected )
	{
		CComBSTR bstrNotSelectLinkQueue, bstrToolName;
		bstrNotSelectLinkQueue.LoadString( IDS_NOTSELECTEDLINKQUEUE );
		bstrToolName.LoadString( IDS_LQNAME );
		AheMessageBox( bstrNotSelectLinkQueue, bstrToolName, MB_OK, WARNLEVEL_LOW );
		return;
	}
}

// 検索されるべきレイヤの一覧を取得する
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::GetSearchLayer(bool bIsRoadLinkOnly)
{
	std::list< CAdapt< ILayerPtr > > listLayers;

	IMxDocumentPtr ipMxDoc = m_cArcHelper.GetDocument();
	IContentsViewPtr ipContentsView;
	ipMxDoc->get_ContentsView( 0, &ipContentsView );

	// 選択アイテムを取得
	std::list< CAdapt<ILayerPtr> > listSelectedLayer;
	CComVariant vaSelectedItem;
	ipContentsView->get_SelectedItem( &vaSelectedItem );
	// [Bug 5424]TOCで非表示レイヤを選択していると、見えている異なるレイヤを選択してしまうバグで選択しているけど、
	// _GetSearchLayerでは非表示レイヤは無視して選択0としてしまうため、選択しているかどうかをフラグで判断しなくちゃいけない
	bool bSelectItem = false;// レイヤ選択しているかどうか
	if( VT_UNKNOWN == vaSelectedItem.vt)
	{
		// 複数レイヤ選択している場合
		_ISetPtr ipSet( vaSelectedItem );
		if( ipSet )
		{
			bSelectItem = true;
			IUnknownPtr ipUnk;
			ipSet->Reset();
			while( ipSet->Next( &ipUnk ) == S_OK )
			{
				// グループレイヤ選択している場合には、その中のレイヤを取得
				std::list< CAdapt<ILayerPtr> > listTempLayer = _GetSearchLayer( ipUnk, bIsRoadLinkOnly );
				for( std::list< CAdapt<ILayerPtr> >::iterator it = listTempLayer.begin(); it != listTempLayer.end(); ++it )
					listSelectedLayer.push_back( *it );
			}
		}
		// ひとつだけレイヤを選択している場合
		else if( ((ILayerPtr)vaSelectedItem) )
		{
			bSelectItem = true;
			listSelectedLayer = _GetSearchLayer( (ILayerPtr)vaSelectedItem, bIsRoadLinkOnly );
		}
	}
	
	// [Bug 5407]属性編集ダイアログの表示をTOCのレイヤ順にしてほしいに対応
	// ipContentsView->get_SelectedItemで取ってきたアイテムはTOCの上から順番とは限らないため、
	// 一度上の処理で確保しておいてから、全ループをまわす
	// ジオフィーチャレイヤだけを取得する
	IMapPtr ipMap = m_cArcHelper.GetMap();
	IUIDPtr ipUID( CLSID_UID );
	ipUID->put_Value( CComVariant(L"{E156D7E5-22AF-11D3-9F99-00C04F6BC78E}") ); // IGeoFeatureLayer
	// すべての見えているフィーチャレイヤで検索かける
	IEnumLayerPtr ipEnumLayer;
	// IMap::get_Layersは上から順番に持ってきてくれているようだ。
	if( SUCCEEDED( ipMap->get_Layers( ipUID, VARIANT_TRUE, &ipEnumLayer ) ) )
	{
		ipEnumLayer->Reset();
		ILayerPtr ipLayer;
		while( S_OK == ipEnumLayer->Next( &ipLayer ) )
		{
			if( IsVisibleLayer( ipLayer ) )
			{
				// リンク列選択の場合には道路リンクだけをリストにいれる
				if( bIsRoadLinkOnly )
				{
					if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
					{
						// レイヤ選択していなかったら、そのままリストへ
						if( !bSelectItem )
							listLayers.push_back( ipLayer );
						else
						{
							// 選択しているレイヤでループまわす
							bool bIsFind = false;
							for( std::list< CAdapt<ILayerPtr> >::iterator it = listSelectedLayer.begin(); it != listSelectedLayer.end(); ++it )
							{
								if( it->m_T == ipLayer )
								{
									bIsFind = true;
									break;
								}
							}
							if( bIsFind )
								listLayers.push_back( ipLayer );
						}
					}
				}
				else
				{
					// レイヤ選択していなかったら、そのままリストへ
					if( !bSelectItem )
						listLayers.push_back( ipLayer );
					else
					{
						// 選択しているレイヤでループまわす
						bool bIsFind = false;
						for( std::list< CAdapt<ILayerPtr> >::iterator it = listSelectedLayer.begin(); it != listSelectedLayer.end(); ++it )
						{
							if( it->m_T == ipLayer )
							{
								bIsFind = true;
								break;
							}
						}
						if( bIsFind )
							listLayers.push_back( ipLayer );
					}
				}
			}
		}
	}
	return listLayers;
}

// 選択しているレイヤから検索用レイヤのリストを作成する
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::_GetSearchLayer(ILayerPtr ipSelectedLayer, bool bIsRoadLinkOnly)
{
	std::list< CAdapt< ILayerPtr > > listLayers;
	// 選択されているレイヤがグループレイヤかそうでないかで場合わけ
	ICompositeLayerPtr ipCompositeLayer( ipSelectedLayer );
	if( !ipCompositeLayer )
	{
		IGeoFeatureLayerPtr ipLayer = ipSelectedLayer;
		if( ipLayer )
		{
			if( IsVisibleLayer( ipLayer ) )
			{
				if( bIsRoadLinkOnly )
				{
					// 選択したレイヤが道路リンク、かつ、レイヤ見えていないといけない
					if( AheGetFeatureClassName( ipLayer ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
						listLayers.push_back( (ILayerPtr)ipLayer );
				}
				else
					listLayers.push_back( (ILayerPtr)ipLayer );
			}
		}
	}
	// 選択したレイヤがグループレイヤの場合
	else
	{
		if( IsVisibleLayer( ipCompositeLayer ) )
		{
			// グループレイヤ以下の見えているレイヤを取得
			std::list< CAdapt< ILayerPtr > > listGroupLayer = GetAllLayerUnderGroup( ipCompositeLayer );
			// リンク列選択の場合には道路リンクだけを返す
			if( bIsRoadLinkOnly )
			{
				for( std::list< CAdapt< ILayerPtr > >::iterator itLayer = listGroupLayer.begin(); itLayer != listGroupLayer.end(); ++itLayer )
				{
					if( AheGetFeatureClassName( (*itLayer).m_T ).CompareNoCase( ROAD_TABLE_NAME ) == 0 )
						listLayers.push_back( (*itLayer).m_T );
				}
			}
			else
				listLayers = listGroupLayer;
		}
	}
	return listLayers;
}

// レイヤが見えているかどうか調べる
bool CAttributeToolObj::IsVisibleLayer( ILayerPtr ipLayer )
{
	bool bRet = false;
	IMapLayersPtr ipMapLayers( m_cArcHelper.GetMap() );
	VARIANT_BOOL vbVisible = VARIANT_FALSE;
	ipMapLayers->IsLayerVisible( ipLayer, &vbVisible );
	if( vbVisible )
		bRet = true;
	return bRet;
}

// グループレイヤに入っている見えているフィーチャレイヤを取得する
std::list< CAdapt<ILayerPtr> > CAttributeToolObj::GetAllLayerUnderGroup(ICompositeLayerPtr ipCompositeLayer)
{
	std::list< CAdapt<ILayerPtr> > listLayer;
	if( !ipCompositeLayer )
		return listLayer;

	long lCompositeLayerNum = 0;
	ipCompositeLayer->get_Count( &lCompositeLayerNum );
	for(long i = 0; i < lCompositeLayerNum; i++)
	{
		ILayerPtr	ipLayer;
		ipCompositeLayer->get_Layer( i, &ipLayer );
		ICompositeLayerPtr ipTempCompositeLayer( ipLayer );
		if( ipTempCompositeLayer )
		{
			// 再帰で自分自身呼び出す
			std::list< CAdapt<ILayerPtr> > listTempLayer = GetAllLayerUnderGroup( ipTempCompositeLayer );
			std::copy(listTempLayer.begin(), listTempLayer.end(), std::back_inserter(listLayer));
		}
		else
		{
			// フィーチャレイヤのみを入れる
			IFeatureLayerPtr ipFeatureLayer;
			if( ipLayer->QueryInterface( &ipFeatureLayer ) == S_OK )
			{
				// 見えているフィーチャだけにしておく
				if( IsVisibleLayer( ipFeatureLayer ) )
					listLayer.push_back( (ILayerPtr)ipFeatureLayer );
			}
		}
	}
	return listLayer;
}

// レイヤを検索して属性ダイアログにセットする
bool CAttributeToolObj::SelectByShapeFromLayer( IGeometryPtr ipGeom, ILayerPtr ipLayer )
{
	bool bSelected = false;
	// 空間参照あわせる
	IGeoDatasetPtr ipGeoDataset = ipLayer;
	ISpatialReferencePtr ipSpRef;
	if( ipGeoDataset )
		ipGeoDataset->get_SpatialReference( &ipSpRef );
	if( ipSpRef != NULL )
	{
		ipGeom->putref_SpatialReference( ipSpRef );
		ipGeom->SnapToSpatialReference();
	}

	// TOC でレイヤが選択されている場合はそのレイヤ内のみで検索
	IFeatureCursorPtr ipCursor = AheSelectByShapeFromLayer( ipGeom, ipLayer, esriSpatialRelIntersects );
	if( AddFeatureToDialog(ipCursor) )
		bSelected = true;

	return bSelected;
}

// 道路リンクについているリンク列を検索する
bool CAttributeToolObj::SelectTargetLQs( IFeatureClassPtr ipRoadClass, IGeometryPtr ipGeom, sindyeRoadQueueType type, bool bLastLink )
{
	// 渡されたレイヤからワークスペース取得
	IWorkspacePtr ipWorkspace;
	IDatasetPtr ipDataset( ipRoadClass );
	if( ipDataset )
		ipDataset->get_Workspace( &ipWorkspace );
	if( !ipWorkspace )
		return false;

	CString strOwnerName = AheGetFeatureClassOwnerName( ipDataset );
	CString strInfoTableName,strLQTableName;
	if( !strOwnerName.IsEmpty() )
	{
		strOwnerName += _T(".");
		strInfoTableName = strOwnerName;
		strLQTableName = strOwnerName;
	}

	CLoadQueueInfo cLQInfo;
	strInfoTableName += cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoInfoTableName );
	strLQTableName += cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoLQTableName );

	ITablePtr ipInfoTable = GetTable(ipRoadClass, strInfoTableName, sindy::schema::inf_ipcvics::kTableName );
	ITablePtr ipLQTable = GetTable(ipRoadClass, strLQTableName, sindy::schema::lq_ipcvics::kTableName );

	long lDrawMode = -1;
	// 格納用リスト
	std::list<ROWDEF> listInfoRows, listLQRows, listLQRoads, listRoads;
	// 道路
	CString strRoadRelFieldName = cLQInfo.GetLoadQueueInfo( type, sindyeLoadQueueInfoLQRFRoadFieldName );
	if( ( type == sindyeRoadQueueLane || type == sindyeRoadQueueDirguide || type == sindyeRoadQueueTurnreg ) &&
		( m_bPerfectSelect == TRUE || m_bIsSelectedLastSequence == TRUE ) )
	{
		if( m_bPerfectSelect && !bLastLink )
		{
			std::list<ROWDEF> listInfoRowsTmp, listLQRowsTmp, listLQRoadsTmp, listRoadsTmp;

			SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRowsTmp, listRoads );
			AheSelectLQRowByInfoRows( listInfoRowsTmp, ipLQTable, listLQRowsTmp );
			// SEQUENCE=1 を持った LQ の道路リンク ID を取得する
			for( std::list<ROWDEF>::iterator it = listLQRowsTmp.begin(); it != listLQRowsTmp.end(); it++ )
			{
				CComVariant vaSequence = AheGetAttribute( it->obj, SEQUENCE_FIELD );
				if( vaSequence.vt == VT_I4 && vaSequence.lVal == 1 )
				{
					CComVariant vaRoadID = AheGetAttribute( it->obj, LINK_ID_FIELD );
					if( vaRoadID.vt == VT_I4 )
					{
						ROWDEF def;
							
						def.flg = lDrawMode;
						def.oid = vaRoadID.lVal;
						def.sequence = 1;

						listRoadsTmp.push_back( def );
					}
					else
						ATLASSERT( vaRoadID.vt == VT_I4 );
				}
			}
			AheSelectInfoRowByRoadIDs( listRoadsTmp, ipInfoTable, ipLQTable, lDrawMode, listInfoRows );
		}
		// 流出リンクのみのリンク列を選択する場合
		else if( m_bIsSelectedLastSequence || bLastLink )
			SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRows, listRoads, 1, bLastLink );
	}
	else
		SelectInfoRowByShape( ipRoadClass, ipInfoTable, ipLQTable, ipGeom, lDrawMode, listInfoRows, listRoads );

	AheSelectLQRowByInfoRows( listInfoRows, ipLQTable, listLQRows );
	AheSelectLQRoad( ipRoadClass, listLQRows, listRoads, listLQRoads );
	
	// AheGetLQRoadGeometry()を呼ぶ前にlistLQRowsをシーケンスでソートしておく必要有り！！
	listLQRows.sort();

	// ダイアログにリンク列とジオメトリをセット
	bool bIsHit = false;
	for( std::list<ROWDEF>::iterator it = listInfoRows.begin(); it != listInfoRows.end(); it++ )
	{
		BOOL bFixFlow = FALSE;
		IGeometryPtr ipRoadGeom = AheGetLQRoadGeometry( it->oid, listLQRows, listLQRoads, &bFixFlow );
		if( !bFixFlow )
		{
			ATLASSERT( bFixFlow == TRUE );
			ATLTRACE(_T("CLQAttributeToolObj::SelectTargetLQs() : リンク列の方向を決定できませんでした（OBJECTID = %d）\n"), it->oid );
		}
		// 関連する LQ を取得してセットする
		_ISetPtr ipLQRows( CLSID_Set );
		for( std::list<ROWDEF>::iterator itLQs = listLQRows.begin(); itLQs != listLQRows.end(); itLQs++ )
		{
			CComVariant vaValue = AheGetAttribute( itLQs->obj, INF_ID_FIELD );
			if( vaValue.vt == VT_I4 && vaValue.lVal == it->oid )
				ipLQRows->Add( itLQs->obj );
		}
		GetAttributeDlg()->SetItem( it->obj, ipLQRows, ipRoadGeom, VARIANT_FALSE );
		bIsHit = true;
		m_lSearchItemNum++;
	}

	return bIsHit;
}

// 属性ダイアログが作成されていなかったら作成する
IAttributeDialogPtr CAttributeToolObj::GetAttributeDlg()
{
	if( !m_ipAttributeDlg )
	{
		// 属性表示ダイアログ作成
		m_ipAttributeDlg.CreateInstance( CLSID_AttributeDlgObj );
		m_ipAttributeDlg->Init( m_cArcHelper.GetApp() );
	}

	return m_ipAttributeDlg;
}

// 属性ダイアログに選択したフィーチャを渡す
bool CAttributeToolObj::AddFeatureToDialog( IFeatureCursorPtr ipFeatureCursor )
{
	bool bSelected = false;
	if( ipFeatureCursor ) 
	{
		IFeaturePtr ipFeature;
		while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
		{
			// [Bug4768]見えているフィーチャのみ追加する
			if( !AheIsVisibleFeature( m_cArcHelper.GetMap(), ipFeature ) )
				continue;

			// 属性ダイアログにフィーチャを渡す
			// 第4引数の値はLQダイアログで使うため、属性ダイアログの場合はTRUEでもFALSEでもなんでもよい
			GetAttributeDlg()->SetItem( ipFeature, NULL, NULL, VARIANT_FALSE );
			bSelected = true;
			m_lSearchItemNum++;
		}
	}

	return bSelected;
}

// 属性編集のプロパティの値が変更されたら、レジストリに登録する\n
void CAttributeToolObj::WriteProperty( BOOL bIsPerfectSelect, BOOL bIsSelectedLastSequence )
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		// 方面、レーンの場合に完璧に取得するかどうか
		if( bIsPerfectSelect == TRUE )
			EmxSetRegDword( _T("IsPerfectSelect"), (DWORD)m_bPerfectSelect, hOpenKey );
		// 方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
		if( bIsSelectedLastSequence == TRUE )
			EmxSetRegDword( _T("IsSelectedLastSequence"), (DWORD)m_bIsSelectedLastSequence, hOpenKey );

		RegCloseKey( hOpenKey );
	}
}

// 起動時にレジストリに登録されているプロパティの内容を読み取る
void CAttributeToolObj::LoadProperty()
{
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		DWORD dwResult;
		// 方面、レーンの場合に完璧に取得するかどうか
		if( EmxGetRegDword( _T("IsPerfectSelect"), &dwResult, hOpenKey ) == TRUE )
			m_bPerfectSelect = (BOOL)dwResult;
		// 方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
		if( EmxGetRegDword( _T("IsSelectedLastSequence"), &dwResult, hOpenKey ) == TRUE )
			m_bIsSelectedLastSequence = (BOOL)dwResult;

		RegCloseKey( hOpenKey );
	}
}

/////////////////////////////////////////////////////////////////////////////
//
// IAttributeToolObj インターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CAttributeToolObj::get_PerfectSelect(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = ( m_bPerfectSelect == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::put_PerfectSelect(VARIANT_BOOL newVal)
{
	m_bPerfectSelect = ( newVal == VARIANT_TRUE ? TRUE : FALSE );
	WriteProperty( TRUE, FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::get_SelectLastSequence(VARIANT_BOOL *pVal)
{
	if( !pVal )
		return E_POINTER;

	*pVal = ( m_bIsSelectedLastSequence == TRUE ? VARIANT_TRUE : VARIANT_FALSE );

	return S_OK;
}

STDMETHODIMP CAttributeToolObj::put_SelectLastSequence(VARIANT_BOOL newVal)
{
	m_bIsSelectedLastSequence = ( newVal == VARIANT_TRUE ? TRUE : FALSE );
	WriteProperty( FALSE, TRUE );

	return S_OK;
}

// 編集可能なリンク列のモード取得
// [Bug 6042] シンボル変更ビューツールのリンク列編集ボタンに対応して欲しい
long CAttributeToolObj::GetEditLinkQueueMode(IFeatureRenderer* ipRednerer)
{
	IRoadRendererPtr ipRoadR( ipRednerer );

	long lDrawMode = 0;

	if ( ipRoadR )
		ipRoadR->get_EditMode( &lDrawMode );	// 編集モード取得

	// リンク列に関係ないモードは除外。
	lDrawMode = lDrawMode & ~(_MULTISYMBOL_ENDPOINT | _MULTISYMBOL_VERTEX | _MULTISYMBOL_ONEWAY | _MULTISYMBOL_NOPASSAGE | _MULTISYMBOL_ANNOTATION | _MULTISYMBOL_S1ANNOPOINT | _MULTISYMBOL_S2ANNOPOINT | _MULTISYMBOL_S3ANNOPOINT | _MULTISYMBOL_S4ANNOPOINT);

	if ( ipRoadR ) {
		if ( lDrawMode <= 0 ) {	// 編集OKなモードがないってことは選択していないから表示モードを取得
			ipRoadR->get_DrawMode( &lDrawMode );	// 表示モード取得

			// リンク列に関係ないモードは除外。
			lDrawMode = lDrawMode & ~(_MULTISYMBOL_ENDPOINT | _MULTISYMBOL_VERTEX | _MULTISYMBOL_ONEWAY | _MULTISYMBOL_NOPASSAGE | _MULTISYMBOL_ANNOTATION | _MULTISYMBOL_S1ANNOPOINT | _MULTISYMBOL_S2ANNOPOINT | _MULTISYMBOL_S3ANNOPOINT | _MULTISYMBOL_S4ANNOPOINT);
		}
	}

	return lDrawMode;
}

// リンク列テーブル取得
ITablePtr CAttributeToolObj::GetTable(const ITablePtr& ipTable, LPCTSTR lpcszName, LPCTSTR lpcszExName)
{
	CString strTable( lpcszName );

	// オーナ名付きのテーブル名の時はオーナ名を取る
	if( _tcschr( lpcszName, _T('.') ) )
	{
		int nPeriodIndex = strTable.Find( _T('.'), 0 );
		strTable = strTable.Right( strTable.GetLength() - nPeriodIndex - 1 );
	}

	// [Bug 6429]ronlyで接続している道路のリンク列が表示されない
	// 道路と同じオーナーのテーブルをオープンする
	ITablePtr ipRet = AheOpenSameOwnerTable( ipTable, strTable );

	if ( ! ipRet )
	{
		// [Bug 10196]別のサーバにあるiPC_VICSを表示できるようにする
		// TOCに道路と同じデータセットのINF_***が読み込まれているか確認
		// 現状はIPCVICS時のみにする
		if ( 0 == strTable.CompareNoCase( lpcszExName ) )
			ipRet = GetSameTable(strTable);
	}

	return ipRet;
}

// TOCにある指定したテーブル名をのITableを取得
ITablePtr CAttributeToolObj::GetSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner)
{
	// TOCに読み込まれているLQ_IPCVICSを読み込む
	CMap cMap( m_cArcHelper.GetFocusMap() );
	ITableCollectionPtr ipTables( (IMap*)cMap );

	if ( ! ipTables )
		return nullptr;

	CString strOwner( lpcszOwner );
	strOwner.MakeLower();

	long nCount = 0;
	ipTables->get_TableCount( &nCount );

	for ( long i = 0; i < nCount; ++i )
	{
		ITablePtr ipTable;
		ipTables->get_Table( i, &ipTable );

		if ( ! ipTable )
			continue;

		CComBSTR bstrName;
		AheGetLayerTableName( (IMap*)cMap, ipTable, &bstrName );

		CString strName( bstrName );
		if ( 0 == strName.CompareNoCase( lpcszName ) )
		{
			if ( lpcszOwner && _tcslen(lpcszOwner) > 0 )
			{
				if ( GetOwnerName(ipTable).CompareNoCase( strOwner ) == 0 )
					return ipTable;
			}
			else
				return ipTable;
		}
	}

	return nullptr;
}

// オーナ名を取得
CString CAttributeToolObj::GetOwnerName(const ITablePtr& ipTable)
{
	CString strName( AheGetFeatureClassOwnerName(ipTable) );

	if ( strName.IsEmpty() )
	{
		// 空の時はパスを取得
		IWorkspacePtr ipWork( AheGetWorkspace( ipTable ) );
		if ( ! ipWork )
			return strName;

		CComBSTR bstrName;
		ipWork->get_PathName( &bstrName );
		strName = bstrName;
	}

	return strName;
}

long CAttributeToolObj::SelectInfoRowByShape( const IFeatureClassPtr& ipRoadClass, const ITablePtr& ipInfoTable, const ITablePtr& ipLQTable, const IGeometryPtr& ipGeom, long lDrawMode, std::list<ROWDEF>& listInfoRows, std::list<ROWDEF>& listRoads, long lSequence/* = -1*/, bool bLastLink )
{
	if( ! ipRoadClass || ! ipInfoTable || ! ipLQTable || ! ipGeom )
		return 0;

	// 道路リンクフィーチャを選択
	IFeatureCursorPtr ipFeatureCursor = AheSelectByShape( ipGeom, ipRoadClass, esriSpatialRelIntersects );
	if( ! ipFeatureCursor )
		return 0;

	// LQ テーブルを検索するためのクエリフィルタ作成
	IFeaturePtr ipFeature;
	long lCount = 0;

	while( ipFeatureCursor->NextFeature( &ipFeature ) == S_OK && ipFeature )
	{
		long lOID = -1;
		ipFeature->get_OID( &lOID );

		IGeometryPtr ipGeom;
		ipFeature->get_ShapeCopy( &ipGeom );

		ROWDEF rowDef;
		rowDef.oid = lOID;
		rowDef.shp = ipGeom;
		rowDef.obj = ipFeature;
		rowDef.flg = lDrawMode;

		if( lSequence != -1 )
			rowDef.sequence = lSequence;

		listRoads.push_back( rowDef );
	}

	return AheSelectInfoRowByRoadIDs( listRoads, ipInfoTable, ipLQTable, lDrawMode, listInfoRows, bLastLink );
}
