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

//// WinLib
#include "AdviseAdapter.h"
#include "macro_util.h"
// SiNDYLib
#include "SiNDYeSettings.h"
//// ArcMapUIBase
#include "AdviseAdapterImpl.h"
#include "IActiveViewEventsImpl.h"
#include "IDocumentEventsImpl.h"
#include "ToolPointer.h"
#include "ToolMultiPolyline.h"

/**
 * @class CToolPointerBase
 * @biref �r���[��ł̃}�E�X����ɂ��\���������肷��N���X
 * @li IDocumentEvents�����ڑ��E�ؒf
 * @li IActiveViewEvents�����ڑ��E�ؒf
 * @li �T�[�`�G���A�`��
 * @li �A�v���P�[�V�����I���ʒm�iIDllThreadManager::OnShutdown�j
 * @li �f�t�H���g�L�[�{�[�h�V���[�g�J�b�g�̒񋟁i�X�N���[���E�Y�[���E�ĕ`��j
 * 
 * �^�C�v�I�v�V����
 * @li MouseUp�ł̌`��g���[�X�itype_line_trace�j
 */
template <class T>
class CToolPointerBase :
	public CToolBase,
	public CIDocumentEventsImpl<T>,
	public CIActiveViewEventsImpl<T>,
	public IDllThreadManager
{
public:
	CToolPointerBase(UINT bitmapID = 0U,
            _U_STRINGorID caption = 0U,
			_U_STRINGorID name = 0U,
            _U_STRINGorID category = 0U,
            _U_STRINGorID tooltip = 0U,
            _U_STRINGorID message = 0U,
            _U_STRINGorID helpfile = 0U,
            UINT helpID = 0U,
            _U_CURSORorID cursor = 0U,
			_U_CURSORorID cursorMove = 0U,
			bool bDeleteCursor = true ) :
	CToolBase(bitmapID, caption, name, category, tooltip, message, helpfile, helpID, cursor, cursorMove, bDeleteCursor),
		m_pToolPointer(NULL), m_lThreadMgrCookie(0)/*, m_bMouseDown(false)*/, m_emType(type_point), m_bUseSnap(false)
	{
	}
	virtual ~CToolPointerBase(void)
	{
	}

	// IDocumentEvents
	STDMETHODIMP ActiveViewChanged()
	{
		CIActiveViewEventsImpl<T>::Unadvise();
		CIActiveViewEventsImpl<T>::Advise( (IActiveViewPtr)m_cApp.GetFocusMap() );
		return S_OK;
	}
	STDMETHOD(CloseDocument)()
	{
		CIActiveViewEventsImpl<T>::Unadvise();
		return S_OK;
	}
	STDMETHOD(MapsChanged)()
	{
		CIActiveViewEventsImpl<T>::Unadvise();
		CIActiveViewEventsImpl<T>::Advise( (IActiveViewPtr)m_cApp.GetFocusMap() );
		return S_OK;
	}
	STDMETHOD(NewDocument)()
	{
		CIActiveViewEventsImpl<T>::Unadvise();
		CIActiveViewEventsImpl<T>::Advise( (IActiveViewPtr)m_cApp.GetFocusMap() );
		return S_OK;
	}
	STDMETHOD(OpenDocument)()
	{
		CIActiveViewEventsImpl<T>::Unadvise();
		CIActiveViewEventsImpl<T>::Advise( (IActiveViewPtr)m_cApp.GetFocusMap() );
		return S_OK;
	}

	// IActiveViewEvents
	STDMETHODIMP AfterDraw(IDisplay * Display, esriViewDrawPhase phase)
	{
		if( m_pToolPointer )
		{
			bool bHandled = false;
			m_pToolPointer->AfterDraw( Display, phase, bHandled );
		}

		return S_OK;
	}

	// ICommand
	STDMETHODIMP OnCreate(IDispatch* hook)
	{
		return CToolBase::OnCreate(hook);
	}
	STDMETHODIMP OnClick()
	{
		HRESULT hr = CToolBase::OnClick();

		// �C�x���g�ڑ�
		CIDocumentEventsImpl<T>::Advise( m_cApp.GetDocument() );
		CIActiveViewEventsImpl<T>::Advise( (IActiveViewPtr)m_cApp.GetFocusMap() );

		// ThreadMgr�ɓo�^���ADeactivate���Ă΂�Ȃ��܂�
		// ArcMap���I�������ꍇ�ɔ�����
		T* p = static_cast<T*>(this);
		((IMultiThreadedApplicationPtr)(IApplication*)m_cApp)->RegisterThreadManager( (IDllThreadManagerPtr)p->GetUnknown(), &m_lThreadMgrCookie );

		return hr;
	}

	// ITool
	STDMETHODIMP Deactivate(VARIANT_BOOL * complete)
	{
		OnShutdown();

		// ThreadMgr�ւ̓o�^�͉������Ă���
		((IMultiThreadedApplicationPtr)(IApplication*)m_cApp)->UnregisterThreadManager( m_lThreadMgrCookie );
		m_lThreadMgrCookie = 0;

		return CToolBase::Deactivate(complete);
	}

	STDMETHODIMP OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		// �}�E�X�X�e�[�^�X�̕ύX���K����ɏ�������K�v����
		HRESULT hr = CToolBase::OnMouseDown( Button, Shift, X, Y );

		bool bHandled = false;
		if( m_pToolPointer )
		{
			IPointPtr ipPoint( m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y ) );
			if( m_bUseSnap )
				m_cApp.Snap( ipPoint );
			hr = m_pToolPointer->OnMouseDown( Button, Shift, ipPoint, bHandled );
		}

		return hr;
	}
	STDMETHODIMP OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		HRESULT hr = S_OK; // �Ԃ�l

		bool bHandled = false;
		if( m_pToolPointer )
		{
			IPointPtr ipPoint( m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y ) );
			if( m_bUseSnap )
				m_cApp.Snap( ipPoint );
			hr = m_pToolPointer->OnMouseMove( Button, Shift, ipPoint, bHandled );
		}

		// �h���b�O����������̂ŕK���������K�v
		hr = CToolBase::OnMouseMove( Button, Shift, X, Y );

		return hr;
	}
	STDMETHODIMP OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
	{
		HRESULT hr = S_OK; // �Ԃ�l

		bool bHandled = false;
		if( IsMouseDown() ) // MouseDown���Ȃ��̂Ȃ�󂯕t���Ȃ�
		{
			if( m_pToolPointer )
			{
				IPointPtr ipPoint( m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y ) );
				if( m_bUseSnap )
					m_cApp.Snap( ipPoint );
				hr = m_pToolPointer->OnMouseUp( Button, Shift, ipPoint, bHandled );
			}
		}

		// �}�E�X�X�e�[�^�X�̕ύX�ȂǕK���u�Ō�Ɂv��������K�v������
		hr = CToolBase::OnMouseUp( Button, Shift, X, Y );

		return hr;
	}
	STDMETHODIMP OnDblClick()
	{
		HRESULT hr = S_OK; // �Ԃ�l

		bool bHandled = false;
		if( m_pToolPointer )
			hr = m_pToolPointer->OnDblClick( bHandled );

		if( ! bHandled )
			hr = CToolBase::OnDblClick();

		return hr;
	}
	STDMETHODIMP OnKeyDown(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK; // �Ԃ�l

		bool bHandled = false;
		if( m_pToolPointer )
			hr = m_pToolPointer->OnKeyDown(keyCode, Shift, bHandled);

		if( ! bHandled )
			hr = CToolBase::OnKeyDown(keyCode, Shift);

		return hr;
	}
	STDMETHODIMP OnKeyUp(LONG keyCode, LONG Shift)
	{
		HRESULT hr = S_OK; // �Ԃ�l

		bool bHandled = false;
		if( m_pToolPointer )
			hr = m_pToolPointer->OnKeyUp(keyCode, Shift, bHandled);

		if( ! bHandled )
			hr = CToolBase::OnKeyUp(keyCode, Shift);

		return hr;
	}
	STDMETHODIMP OnContextMenu(LONG X, LONG Y, VARIANT_BOOL* handled)
	{
		HRESULT hr = S_OK; // �Ԃ�l
		if( m_pToolPointer )
		{
			hr = m_pToolPointer->OnContextMenu(X, Y, handled);
			if( ! handled )
				hr = CToolBase::OnContextMenu(X, Y, handled);
		}
		return hr;
	}
	// IDllThreadManager
	STDMETHODIMP OnShutdown()
	{
		// �C�x���g�ؒf
		CIDocumentEventsImpl<T>::Unadvise();
		CIActiveViewEventsImpl<T>::Unadvise();

		init();
		return S_OK;
	}
public:
	IGeometryPtr GetSearchArea( IPoint* ipPoint, ISpatialReference* ipSpRef = NULL ) const {
		if( ! m_pToolPointer )
		{
			_ASSERTE( ! m_pToolPointer );
			return IGeometryPtr();
		}

		return m_pToolPointer->GetSearchArea( ipPoint, ipSpRef );
	}
protected:
	void init()
	{
		if( m_pToolPointer )
		{
			delete m_pToolPointer;
			m_pToolPointer = NULL;
		}
	}
	void setMode( mode_type emType, bool bWithArea=false )
	{
		init();

		switch( emType )
		{
			case type_envelope:   m_pToolPointer = new CToolEnvelope( m_cApp, bWithArea );  break;
			case type_polygon:    m_pToolPointer = new CToolPolygon( m_cApp, bWithArea );   break;
			case type_polyline:   m_pToolPointer = new CToolPolyline( m_cApp, bWithArea );  break;
			case type_multi_polyline:   m_pToolPointer = new CToolMultiPolyline( m_cApp, bWithArea );  break;
			case type_area:       m_pToolPointer = new CToolArea( m_cApp, bWithArea );      break;
			//case type_sketch:     m_pToolPointer = new CToolSketch( m_cApp );    break;
			case type_line_trace: m_pToolPointer = new CToolLineTrace( m_cApp, bWithArea ); break;
			case type_realtime_line_trace: m_pToolPointer = new CToolRealTimeLineTrace( m_cApp, bWithArea ); break;
			case type_real_line_trace:	m_pToolPointer = new CToolRealLineTrace( m_cApp ); break;
			case type_real_line_trace2:	m_pToolPointer = new CToolRealLineTrace2( m_cApp ); break;
			case type_point:
			default:              m_pToolPointer = new CToolPoint( m_cApp );     break;
		}

		m_emType = emType;
	}
	mode_type getMode() { return m_emType; }
	/**
	 * @brief ���������ݒ�
	 * @bug   [Bug 10374]�g���[�X�\�ȃ��C���f�[�^���A���s�ړ���i[MANAGEMENT_ID]is not Null)�̃I�u�W�F�N�g�Ɍ��肷��
	 * @param lpcszWhere [in] ��������
	*/
	void setWhere(LPCTSTR lpcszWhere)
	{
		if ( m_pToolPointer )
			m_pToolPointer->setWhere( lpcszWhere );
	}
protected:
	CToolPointer* m_pToolPointer; //!< �}�E�X�|�C���^�I�u�W�F�N�g
	long m_lThreadMgrCookie; //!< ShutDown�C�x���g�ߑ��p
	mode_type m_emType;
	bool m_bUseSnap; //!< �X�i�b�v���g�p���邩�ǂ����i�f�t�H���g�Ffalse�j
};
