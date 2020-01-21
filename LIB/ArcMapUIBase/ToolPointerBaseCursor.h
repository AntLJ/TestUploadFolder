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
	\brief	CToolPointerBase�{�J�[�\������N���X
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
		// �f�t�H���g�J�[�\��
		m_hDefaultCursor = (HCURSOR)::LoadImage( NULL, MAKEINTRESOURCE(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED );
	}
	operator CToolPointerBaseCursor<T>& () {
		return *((CToolPointerBaseCursor<T>*)this);
	}

	virtual ~CToolPointerBaseCursor()
	{
		// �J�[�\���j��
		for ( std::map< int, HCURSOR >::iterator it = m_mapCursors.begin(); it != m_mapCursors.end(); ++it ) {
			if ( it->second != NULL )
				DestroyCursor( it->second );
		}
	};

	operator HCURSOR()
	{
		return m_bUseDefaultCursor ? m_hDefaultCursor : m_mapCursors[m_NowMode];
	}

	/// ToolPointer����蒼��
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

		// �J�[�\���ύX
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		HRESULT hr = CToolPointerBase<T>::OnMouseUp(Button, Shift, X, Y);

		// �J�[�\���ύX
		static_cast<T*>(this)->SelectCursor();

		if ( m_pToolPointer ) {
			IGeometryPtr ipGeom = m_pToolPointer->GetShape();

			if ( ipGeom ) {
				if ( !IsInvalidateGeometry( ipGeom ) )
					ipGeom = m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y );

				if ( ipGeom ) {
					// �`��m��
					static_cast<T*>(this)->FinalShape(ipGeom);

					// �����ɕێ����Ă������j���������̂ō�蒼���B
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

		// �J�[�\���ύX
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnKeyDown(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK;

		if ( keyCode != VK_SPACE )
			hr = CToolPointerBase<T>::OnKeyDown(keyCode, Shift);

		// �J�[�\���ύX
		static_cast<T*>(this)->SelectCursor();

		return hr;
	}
	STDMETHODIMP OnKeyUp(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK; // �Ԃ�l

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

		// �J�[�\���ύX
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
		\brief	���݃��[�h�擾
		\retval	���[�h
	*/
	int GetCursorMode() const { return m_NowMode; };
	/*!
		\fn		SetCursorMode(int Mode)
		\brief	���݃��[�h�ݒ�
		\param	Mode	[in]	���[�h
	*/
	void SetCursorMode(int Mode) { m_NowMode = Mode; };
	/*!
		\fn		SetUseDefault(bool bDefault=false)
		\brief	�f�t�H���g�J�[�\���g�p�t���O�ݒ�
		\param	bDefault	[in]	�f�t�H���g�J�[�\���g�p�t���O true:�f�t�H���g�J�[�\���g�p, false:�ݒ�J�[�\���g�p
	*/
	void SetUseDefault(bool bDefault=false) { m_bUseDefaultCursor = bDefault; };
	/*!
		\fn		SetCursor(int Mode, UINT ID)
		\brief	�J�[�\���ݒ�
		\param	Mode	[in]	���[�h
		\param	ID	[in]	���\�[�XID
	*/
	void SetCursor(int Mode, UINT ID)
	{
		HCURSOR hCursor = (HCURSOR)::LoadImage( _AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(ID), IMAGE_CURSOR, 0, 0, LR_DEFAULTCOLOR );

		// ���Ƀ��[�h���g�p���Ă�����J�[�\���폜
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
		\brief	�t�B�[�h�o�b�N�m�莞�ɌĂ΂�鏃�����z�֐�
		\param	ipGeom	[in]	�`��
	*/
	virtual void FinalShape(IGeometry* ipGeom) = 0;
	/*!
		\fn		SelectCursor()
		\brief	�J�[�\���ύX�̂��߂ɌĂ΂�鏃�����z�֐�
	*/
	virtual void SelectCursor() = 0;
	/*!
		\fn		IsInvalidateGeometry(IGeometry* ipGeom) const
		\brief	�`�󂪗L�����ǂ���
		\retval bool
	*/
	bool IsInvalidateGeometry(IGeometry* ipGeom) { return ::IsInvalidateGeometry( ipGeom ); }
private:
	mode_type				m_emSketchType;
	bool					m_bUseDefaultCursor;	//!< �f�t�H���g�J�[�\���g�p�t���O
	int						m_NowMode;				//!< ���݂̃��[�h
	std::map<int,HCURSOR>	m_mapCursors;			//!< �}�E�X�J�[�\���Q
	HCURSOR					m_hDefaultCursor;		//!< �f�t�H���g�J�[�\��
	double					m_dbMaxArea;			//!< �����l�i�ʐϕ����q�j0�ȉ�:�����Ȃ�
	WKSPoint				m_wksArea[2];
};
