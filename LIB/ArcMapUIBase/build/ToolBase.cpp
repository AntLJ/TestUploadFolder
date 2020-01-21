/*
Copyright 1995-2004 ESRI
All rights reserved under the copyright laws of the United States.
You may freely redistribute and use this sample code, with or without modification.
Disclaimer:  THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ESRI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) SUSTAINED BY YOU OR A THIRD PARTY, HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ARISING IN ANY WAY OUT OF THE USE OF THIS SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

For additional information contact:  Environmental Systems Research Institute, Inc.
Attn:  Contracts Dept.
380 New York Street
Redlands, California, U.S.A. 92373 

Email: contracts@esri.com
*/
// MapToolBase.cpp: implementation of the CToolBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ToolBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CToolBase::CToolBase(
  UINT wBitmapID,
  _U_STRINGorID caption,
  _U_STRINGorID name,
  _U_STRINGorID category,
  _U_STRINGorID tooltip,
  _U_STRINGorID message,
  _U_STRINGorID helpfile,
  UINT helpID,
  _U_CURSORorID cursor,
  _U_CURSORorID cursorMove,
  bool bDeleteCursor/* = true*/ ) :
    CCommandBase(wBitmapID, caption, name, category, tooltip, message, helpfile, helpID),
    m_hCursor(cursor),
    m_hCursorMove(cursorMove),
	m_bDeleteCursor(bDeleteCursor),
	m_emMouseStatus(uiMouseEventNothing),
	m_bEnableMButtonScroll(true),	// ArcMap9.3.1����K�v�Ȃ�
	m_bEnableOverDrag(true)
{
	if( ! cursorMove )
		m_hCursorMove = m_hCursor;

	m_cMouseDownPoint.x = m_cMouseDownPoint.y = 0;
}


CToolBase::~CToolBase()
{
	if( m_bDeleteCursor )
	{
		if (m_hCursor!=0)
			::DestroyCursor(m_hCursor);

		if ((m_hCursorMove!=0) && (m_hCursor != m_hCursorMove))
			::DestroyCursor(m_hCursorMove);
	}

	m_hCursor = 0;
	m_hCursorMove = 0;
}

STDMETHODIMP CToolBase::OnClick()
{
	m_emMouseStatus = uiMouseEventNothing;
	return CCommandBase::OnClick();
}

STDMETHODIMP CToolBase::get_Cursor(OLE_HANDLE * Cursor)
{
	if (Cursor == NULL)
		return E_POINTER;

#pragma warning(push)
#pragma warning(disable : 4311)
	if (IsMouseDown() && m_hCursorMove)  
		*Cursor = (OLE_HANDLE)m_hCursorMove;
	else
		*Cursor = (OLE_HANDLE)m_hCursor;
#pragma warning(pop)

	return S_OK;
}

STDMETHODIMP CToolBase::OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// �}�E�X�������W�ێ�
	m_cMouseDownPoint.x = X;
	m_cMouseDownPoint.y = Y;

	if( ! MouseDown(Button, X, Y) ) // �L���v�`���J�n�E�{�^���X�e�[�^�X�X�V�E�t�H�[�J�X�}�b�v�؂�ւ�
		return S_OK;

	// �{�^���̎�ނŏ�������
	switch( m_emMouseStatus )
	{
		// ���{�^���̓h���b�O�ŃX�N���[��
		case uiMouseMButtonDown:
			if( m_bEnableMButtonScroll )
			{
				m_spZooMove.reset( new ZooMoveController( (IActiveViewPtr)m_cApp.GetFocusMap(), (IPageLayoutPtr)m_cApp.GetFocusMap() ? true : false ) );
				m_spZooMove->changeMode(ZooMoveController::kMoveMode, m_cMouseDownPoint );
				ATLTRACE(_T("[CToolBase] ���{�^���h���b�O�J�n���܂���\n") );
			}
			break;
		default: break;
	}

	return S_OK;
}

STDMETHODIMP CToolBase::OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// �{�^����������Ă���Ƃ��̂݃L���v�`�����J�n����
	// �������A���{�^���̂݉�����Ă���Ƃ��͕K�v�Ȃ��̂ł��Ȃ�
	if( m_bEnableOverDrag && 2 <= Button )
		MessageLoop( Button );

	return S_OK;
}


STDMETHODIMP CToolBase::OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{
	// �{�^���̎�ނŏ�������
	switch( m_emMouseStatus )
	{
		case uiMouseMButtonDown:
			if( m_bEnableMButtonScroll )
			{
				if( m_spZooMove )
					m_spZooMove->apply();
				m_spZooMove.reset( (ZooMoveController*)NULL );
			}
			break;
		default:
			break;
	}

	MouseUp(Button); // �L���v�`���I���E�{�^���X�e�[�^�X�X�V

	return S_OK;
}


STDMETHODIMP CToolBase::OnDblClick()
{
	(LONG&)m_emMouseStatus |= uiMouseDoubleClick; // �_�u���N���b�N���ꂽ

	return S_OK;
}


STDMETHODIMP CToolBase::OnKeyDown(LONG keyCode, LONG Shift)
{
	return S_OK;
}

STDMETHODIMP CToolBase::OnKeyUp(LONG keyCode, LONG Shift)
{
	return S_OK;
}

STDMETHODIMP CToolBase::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL * handled)
{
	return S_OK;
}

STDMETHODIMP CToolBase::Refresh(OLE_HANDLE hDC)
{
	return S_OK;
}


STDMETHODIMP CToolBase::Deactivate(VARIANT_BOOL * complete)
{
	*complete = VARIANT_TRUE;
	m_emMouseStatus = uiMouseEventNothing;
	return S_OK;
}

///////////////////////////////////////////////////////////////////
//
void CToolBase::MessageLoop( long Button )
{
	static bool g_bMessageCapturing = false; // ���b�Z�[�W���L���v�`������Ă��邩�ǂ���
	if( ! g_bMessageCapturing )
	{
		// ���b�Z�[�W�L���v�`����������OnMouseMove������
		// �����邽��
		g_bMessageCapturing = true;

		// ���{�^���ȊO�͒n�}��ʊO�ւ̃h���b�O��F�߂Ă���Ȃ�
		// �̂ŁA���b�Z�[�W������肵�ė����Ă��iSetCapture�͂Ȃ�ׂ����O�Łj
		HWND hWnd = m_cApp.GetFocusMapHWND();
 		SetCapture( hWnd );

		bool bDoLoop = true;
		ATLTRACE(_T("[CToolBase] �}�E�X�C�x���g�L���v�`���J�n���܂���\n") );
		MSG msg = {0};
		do {
			while( ::PeekMessage( &msg, 0, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE ) )
			{
				ScreenToClient( hWnd, &msg.pt );
				long shift = ( (msg.wParam & MK_SHIFT) ? 1 : 0 ) | ( (msg.wParam & VK_CONTROL) ? 2 : 0 ) | ( (1 == HIWORD(GetKeyState(VK_MENU))) ? 4 : 0 );
				switch( msg.message )
				{
					case WM_MOUSEMOVE:
					{
						// �{�^���̎�ނŏ�������
						switch( m_emMouseStatus )
						{
							case uiMouseMButtonDown:
								if( m_bEnableMButtonScroll )
								{
									// [Bug 5483]VC8�ɂȂ��������Ȃ̂��A�}�E�X�����Ă��Ȃ��Ă��AWM_MOUSEMOVE���Ă΂�邽�߁A
									// �{���ɍ��W�����Ă��邩���ׂ�
									if( !( m_cMouseDownPoint.x == msg.pt.x && m_cMouseDownPoint.y == msg.pt.y ) )
									{
										if( m_spZooMove )
										{
											m_spZooMove->onMouseMove(msg.pt);
											ATLTRACE(_T("[CToolBase] �n�}�h���b�O\n"));
										}
									}
								}
								break;
							default:
								break;
						}

						// �p�����OnMouseMove�𓮍삳����
						// ���ʓI�ɁA�ŏ���OnMouseMove��1�񔼌Ă΂�邱�ƂɂȂ�B
						// �c�蔼���͂��̃��b�Z�[�W���[�v���I�������Ƃ��ɌĂ΂��̂ŁA���ӂ��K�v
						OnMouseMove( Button, shift, msg.pt.x, msg.pt.y );
						break;
					}
					case WM_MBUTTONUP:
					case WM_RBUTTONUP:
						bDoLoop = false;
						OnMouseUp( Button, shift, msg.pt.x, msg.pt.y );
						break;
					// �ړ��ȊO�̃}�E�X���b�Z�[�W�̓h���b�O��
					// �I���Ƃ���
					default:
						if( m_bEnableMButtonScroll )
						{
							if( m_spZooMove )
								m_spZooMove->cancel(); // ���h���b�O�L�����Z��
						}
						bDoLoop = false;
						break;
				}
			}
			// ���̑��̃��b�Z�[�W�̓X���[
			PeekMessage( &msg, 0, 0, 0, PM_NOREMOVE );
		}
		while( bDoLoop && WaitMessage() );

		g_bMessageCapturing = false;
		ATLTRACE(_T("[CToolBase] �}�E�X�C�x���g�L���v�`���I�����܂���\n") );
		ReleaseCapture();
	}
}

///////////////////////////////////////////////////////////////////
//
// �}�E�X�̉��������󋵂��Z�b�g����
void CToolBase::SetMouseStatus( LONG Button, bool bIsDown )
{
	if( bIsDown )
		(LONG&)m_emMouseStatus |= Button;
	else {
		(LONG&)m_emMouseStatus &= ~Button;
		(LONG&)m_emMouseStatus &= ~(uiMouseDoubleClick); // �_�u���N���b�N�t���O�͏���
	}
	// �{���͏�L�݂̂Ń}�E�X�X�e�[�^�X���X�V�������Ƃ��낾���A
	// �Ⴆ�Β��{�^��DOWN->�E�{�^��DOWN->�E�{�^��UP->�R���e�L�X�g���j���[��Œ��{�^��UP
	// �Ƃ����ꍇ�ɂ́A�Ō�̒��{�^��UP���R���e�L�X�g���j���[���ɍs���Ă��܂��A�������
	// �����Ă��Ȃ��B
	// ���傤���Ȃ��̂�GetKeyState�Ŏ��ۂ̏�Ԃ��擾���邱�Ƃɂ���i���Ⴀ��̏�������Ȃ������j
	(int&)m_emMouseStatus = ( 0 > GetKeyState( VK_LBUTTON ) ) ? m_emMouseStatus | uiMouseLButtonDown : m_emMouseStatus & ~uiMouseLButtonDown;
	(int&)m_emMouseStatus = ( 0 > GetKeyState( VK_MBUTTON ) ) ? m_emMouseStatus | uiMouseMButtonDown : m_emMouseStatus & ~uiMouseMButtonDown;
	(int&)m_emMouseStatus = ( 0 > GetKeyState( VK_RBUTTON ) ) ? m_emMouseStatus | uiMouseRButtonDown : m_emMouseStatus & ~uiMouseRButtonDown;

	ATLTRACE(_T("[CToolBase] [�{�^���X�e�[�^�X]%s%s%s%s [�C�x���g]%s\n"), 
		m_emMouseStatus & uiMouseEventNothing ? _T("�Ȃ�") : _T(""),
		m_emMouseStatus & uiMouseLButtonDown ? _T("��") : _T(""), 
		m_emMouseStatus & uiMouseMButtonDown ? _T("��") : _T(""), 
		m_emMouseStatus & uiMouseRButtonDown ? _T("�E") : _T(""),
		bIsDown ? _T("��") : _T("��") );
}

HRESULT CToolBase::SetFocusMap(LONG X, LONG Y)
{
	HRESULT hr = E_FAIL;
	if( m_cApp != NULL )
	{
		IMxDocumentPtr ipMxDoc( m_cApp.GetDocument() );
		if( NULL != ipMxDoc )
		{
			IActiveViewPtr ipActiveView;
			if( SUCCEEDED( hr = ipMxDoc->get_ActiveView( &ipActiveView ) ) )
			{
				IPageLayoutPtr ipPageLayout( ipActiveView );
				if (ipPageLayout != 0 && ipActiveView!=0)
				{
					IPointPtr ipPoint;
					IScreenDisplayPtr ipScreenDisplay;
					IDisplayTransformationPtr ipDisplayTrans;
					IMapPtr ipHitMap;

					hr = ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
					if (ipScreenDisplay)
						hr = ipScreenDisplay->get_DisplayTransformation(&ipDisplayTrans);

					if (ipDisplayTrans)
						hr = ipDisplayTrans->ToMapPoint(X, Y, &ipPoint);

					if (ipPoint)
						hr = ipActiveView->HitTestMap(ipPoint, &ipHitMap);

					if (ipHitMap != 0)
					{
						IMapPtr ipOldMap;
						hr = ipActiveView->get_FocusMap(&ipOldMap);
						if (ipOldMap != ipHitMap)
						{
							// Clicked on a differnt map, change the focus map
							hr = ipActiveView->putref_FocusMap(ipHitMap);
							ipActiveView->PartialRefresh(esriViewGraphics,0,0);
						}
					}
					else
					hr = S_FALSE; // Didn't hit a map
				}
				else
					hr = S_OK; // Not a page layout, so focus map does not apply
			}
		}
		else hr = E_FAIL;
	}
	return hr;
}


bool CToolBase::MouseDown(LONG Button, LONG X, LONG Y)
{
	// Update focus map to the place thats clicked on
	HRESULT hr = SetFocusMap(X, Y);
	if (hr == S_FALSE || FAILED(hr))
		return false;

	SetMouseStatus( Button, true );

	HCURSOR hCur = 0;
	get_Cursor((OLE_HANDLE*) &hCur);
	::SetCursor(hCur);

	return true;
}

void CToolBase::MouseUp(LONG Button)
{
	SetMouseStatus( Button, false );

	HCURSOR hCur = 0;
	get_Cursor((OLE_HANDLE*) &hCur);
	::SetCursor(hCur);
}




