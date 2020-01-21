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

#pragma once

#include "ToolPointerBase.h"

/*!
	\class	CToolPointerBaseCursor
	\brief	CToolPointerBase＋カーソル操作クラス
*/
template<class T>
class CToolPointerBaseCursor : public CToolPointerBase<T>
{
public:
	CToolPointerBaseCursor(UINT bitmapID = 0U,
            _U_STRINGorID caption = 0U,
			_U_STRINGorID name = 0U,
            _U_STRINGorID category = 0U,
            _U_STRINGorID tooltip = 0U,
            _U_STRINGorID message = 0U,
            _U_STRINGorID helpfile = 0U,
            UINT helpID = 0U,
            UINT cursorID = 0U,
			UINT cursorMoveID = 0U) :
	CToolPointerBase(bitmapID, caption, name, category, tooltip, message, helpfile, helpID, cursorID, cursorMoveID),
		m_bUseDefaultCursor(false), m_NowMode(0), m_emSketchType(type_polygon), m_dbMaxArea(0.0f)
	{
		// デフォルトカーソル
		m_hDefaultCursor = (HCURSOR)::LoadImage( NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED );
	}
	operator CToolPointerBaseCursor<T>& () {
		return *((CToolPointerBaseCursor<T>*)this);
	}

	virtual ~CToolPointerBaseCursor()
	{
		// カーソル破棄
		for ( std::map< int, HCURSOR >::iterator it = m_mapCursors.begin(); it != m_mapCursors.end(); ++it ) {
			if ( it->second != NULL )
				DestroyCursor( it->second );
		}
	};

	operator HCURSOR()
	{
		return m_bUseDefaultCursor ? m_hDefaultCursor : m_mapCursors[m_NowMode];
	}

	/// ToolPointerを作り直す
	void ResetToolPointer()
	{
		if ( m_pToolPointer ) {
			delete m_pToolPointer;
			m_pToolPointer = NULL;
		}

		switch ( m_emType )
		{
		case type_area:
		case type_envelope:
			setMode( type_area );
			break;
		case type_polyline:
		case type_polygon:
			setMode( m_emSketchType, true );
			break;
		default:
			setMode(m_emType);
			break;
		}
	}

	STDMETHODIMP get_Cursor(OLE_HANDLE * Cursor)
	{
		if ( Cursor == NULL )
			return E_POINTER;

		*Cursor = (OLE_HANDLE)(HCURSOR)(*this);

		return S_OK;
	};

	STDMETHODIMP OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		HRESULT hr = CToolPointerBase<T>::OnMouseDown(Button, Shift, X, Y);

		// カーソル変更
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		HRESULT hr = CToolPointerBase<T>::OnMouseUp(Button, Shift, X, Y);

		// カーソル変更
		static_cast<T*>(this)->SelectCursor();

		if ( m_pToolPointer ) {
			IGeometryPtr ipGeom = m_pToolPointer->GetShape();

			if ( ipGeom ) {
				if ( !IsInvalidateGeometry( ipGeom ) )
					ipGeom = m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y );

				if ( ipGeom ) {
					// 形状確定
					static_cast<T*>(this)->FinalShape(ipGeom);

					// 内部に保持している情報を破棄したいので作り直す。
					ResetToolPointer();
				}
			}
		}

		return hr;
	}
	STDMETHODIMP OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		if ( m_pToolPointer ) {
			if ( m_emType == type_area ) {
				IPointPtr ipPoint = m_pToolPointer->GetShape();

				if ( ipPoint ) {
					ipPoint->QueryCoords( &m_wksArea[0].X, &m_wksArea[0].Y );

					setMode( type_envelope );

					if ( m_pToolPointer ) {
						bool bHandled;

						m_pToolPointer->OnMouseDown( Button, Shift, ipPoint, bHandled );
					}
				}
			}
		}

		HRESULT hr = S_OK;

		if ( m_emType == type_envelope ) {
			bool bIslimit = true;

			if ( m_dbMaxArea > 0.0f ) {
				m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y )->QueryCoords( &m_wksArea[1].X, &m_wksArea[1].Y );

				crd_cnv crdcnv;

				double dbArea = crdcnv.GetDist( m_wksArea[0].X, m_wksArea[0].Y, m_wksArea[0].X, m_wksArea[1].Y ) * crdcnv.GetDist( m_wksArea[0].X, m_wksArea[1].Y, m_wksArea[1].X, m_wksArea[1].Y );

				if ( dbArea * 0.000001f >= m_dbMaxArea )
					bIslimit = false;
			}

			if ( bIslimit )
				hr = CToolPointerBase<T>::OnMouseMove(Button, Shift, X, Y);
		} else
			hr = CToolPointerBase<T>::OnMouseMove(Button, Shift, X, Y);

		// カーソル変更
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnKeyDown(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK;

		if ( keyCode != VK_SPACE )
			hr = CToolPointerBase<T>::OnKeyDown(keyCode, Shift);

		// カーソル変更
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnKeyUp(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK; // 返り値

		if ( m_pToolPointer ) {
			bool bHandled = false;

			hr = m_pToolPointer->OnKeyUp(keyCode, Shift, bHandled);

			if( ! bHandled )
				hr = CToolBase::OnKeyUp(keyCode, Shift);
		}

		switch ( keyCode ) {
			case VK_ESCAPE:
				if ( m_emType == type_envelope )
					setMode( type_area );

				break;
			case VK_SPACE:	// SPACE
				switch ( m_emType ) {
					case type_area:
					case type_envelope:
						setMode( m_emSketchType, true );
						break;
					case type_polygon:
					case type_polyline:
						setMode( type_area ); 
						break;
				}

				break;
		}

		// カーソル変更
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	// IActiveViewEvents
	STDMETHODIMP AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
	{
		return CToolPointerBase<T>::AfterDraw(Display, phase);
	}

	/*!
		\fn		GetCursorMode()
		\brief	現在モード取得
		\retval	モード
	*/
	int GetCursorMode() const { return m_NowMode; };
	/*!
		\fn		SetCursorMode(int Mode)
		\brief	現在モード設定
		\param	Mode	[in]	モード
	*/
	void SetCursorMode(int Mode) { m_NowMode = Mode; };
	/*!
		\fn		SetUseDefault(bool bDefault=false)
		\brief	デフォルトカーソル使用フラグ設定
		\param	bDefault	[in]	デフォルトカーソル使用フラグ true:デフォルトカーソル使用, false:設定カーソル使用
	*/
	void SetUseDefault(bool bDefault=false) { m_bUseDefaultCursor = bDefault; };
	/*!
		\fn		SetCursor(int Mode, UINT ID)
		\brief	カーソル設定
		\param	Mode	[in]	モード
		\param	ID	[in]	リソースID
	*/
	void SetCursor(int Mode, UINT ID)
	{
		HCURSOR hCursor = (HCURSOR)::LoadImage( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(ID), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR );

		// 既にモードが使用していたらカーソル削除
		if ( m_mapCursors.find( Mode ) != m_mapCursors.end() ) {
			if ( m_mapCursors[Mode] != NULL )
				DestroyCursor( m_mapCursors[Mode] );
		}

		m_mapCursors[Mode] = hCursor;
	};
	void SetSketchMode(mode_type emType) { m_emSketchType = emType; }
	mode_type GetSketchMode() const { return m_emSketchType; }
	void SetMaxArea(double dbArea) { m_dbMaxArea = dbArea; }

	/*!
		\fn		FinalShape(IGeometry* ipGeom)
		\brief	フィードバック確定時に呼ばれる純粋仮想関数
		\param	ipGeom	[in]	形状
	*/
	virtual void FinalShape(IGeometry* ipGeom) = 0;
	/*!
		\fn		SelectCursor()
		\brief	カーソル変更のために呼ばれる純粋仮想関数
	*/
	virtual void SelectCursor() = 0;
	/*!
		\fn		IsInvalidateGeometry(IGeometry* ipGeom) const
		\brief	形状が有効かどうか
		\retval bool
	*/
	bool IsInvalidateGeometry(IGeometry* ipGeom) { return ::IsInvalidateGeometry( ipGeom ); }
private:
	mode_type				m_emSketchType;
	bool					m_bUseDefaultCursor;	//!< デフォルトカーソル使用フラグ
	int						m_NowMode;				//!< 現在のモード
	std::map<int,HCURSOR>	m_mapCursors;			//!< マウスカーソル群
	HCURSOR					m_hDefaultCursor;		//!< デフォルトカーソル
	double					m_dbMaxArea;			//!< 制限値（面積平方㎞）0以下:制限なし
	WKSPoint				m_wksArea[2];
};
