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
// MapToolBase.cpp: implementation of the CMapToolBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapToolBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapToolBase::CMapToolBase(
  WORD wBitmapID,
  UINT caption,
  UINT category,
  UINT tooltip,
  UINT message,
  UINT helpfile,
  LONG helpID,
  UINT cursorID,
  UINT cursorMoveID ) :
    CMapCommandBase(wBitmapID, caption, category, tooltip, message, helpfile, helpID),
    m_hCursor(0),
    m_hCursorMove(0),
    m_bMouseDown(false)
{
  if (cursorID)
    m_hCursor = (HCURSOR) ::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(cursorID),
      IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

  if (cursorMoveID)
    m_hCursorMove = (HCURSOR) ::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(cursorMoveID),
      IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
  else if (cursorID !=0)
    m_hCursorMove = m_hCursor; // if no move cursor supplied use the normal cursor
 
}


CMapToolBase::~CMapToolBase()
{
  if (m_hCursor!=0)
    ::DestroyCursor(m_hCursor);


  if ((m_hCursorMove!=0) && (m_hCursor != m_hCursorMove))
    ::DestroyCursor(m_hCursorMove);

  m_hCursor = 0;
  m_hCursorMove = 0;
}

STDMETHODIMP CMapToolBase::get_Cursor(OLE_HANDLE * Cursor)
{
  if (Cursor == NULL)
    return E_POINTER;

  if (m_bMouseDown)  
    *Cursor = (OLE_HANDLE)m_hCursorMove;
  else
    *Cursor = (OLE_HANDLE)m_hCursor;
  
  return S_OK;
}

STDMETHODIMP CMapToolBase::OnMouseDown(LONG Button, LONG Shift, LONG X, LONG Y)
{

  return S_OK;
}

STDMETHODIMP CMapToolBase::OnMouseMove(LONG Button, LONG Shift, LONG X, LONG Y)
{
  return S_OK;
}


STDMETHODIMP CMapToolBase::OnMouseUp(LONG Button, LONG Shift, LONG X, LONG Y)
{

  return S_OK;
}


STDMETHODIMP CMapToolBase::OnDblClick()
{
  return S_OK;
}


STDMETHODIMP CMapToolBase::OnKeyDown(LONG keyCode, LONG Shift)
{
  return S_OK;
}

STDMETHODIMP CMapToolBase::OnKeyUp(LONG keyCode, LONG Shift)
{
  return S_OK;
}

STDMETHODIMP CMapToolBase::OnContextMenu(LONG X, LONG Y, VARIANT_BOOL * handled)
{
  return S_OK;
}

STDMETHODIMP CMapToolBase::Refresh(OLE_HANDLE hDC)
{
  return S_OK;
}


STDMETHODIMP CMapToolBase::Deactivate(VARIANT_BOOL * complete)
{
  *complete = VARIANT_TRUE;
  return S_OK;
}


///////////////////////////////////////////////////////////////////
//
HRESULT CMapToolBase::SetFocusMap(LONG X, LONG Y)
{
	HRESULT hr = E_FAIL;

	if( NULL != m_ipApp )
	{
		IDocumentPtr ipDoc;
		if( SUCCEEDED( hr = m_ipApp->get_Document( &ipDoc ) ) )
		{
			IMxDocumentPtr ipMxDoc( ipDoc );
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
	}

	return hr;
}


bool CMapToolBase::MouseDown(LONG X, LONG Y)
{
  m_bMouseDown = false;

  // Update focus map to the place thats clicked on
  HRESULT hr = SetFocusMap(X, Y);
  if (hr == S_FALSE || FAILED(hr))
    return false;

  m_bMouseDown = true;

  HCURSOR hCur;
  get_Cursor((OLE_HANDLE*) &hCur);
  ::SetCursor(hCur);

  // Capture all mouse events outside the window while mouse is down
  IScreenDisplayPtr ipScreenDisplay;
  IActiveViewPtr ipActiveView;
  GetFocusMapActiveView(&ipActiveView);
  ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
  HWND hWnd;
  ipScreenDisplay->get_hWnd((UINT*) &hWnd);
  ::SetCapture(hWnd);

  return true;
}

void CMapToolBase::MouseUp()
{
  m_bMouseDown  = false;

  HCURSOR hCur;
  get_Cursor((OLE_HANDLE*) &hCur);
  ::SetCursor(hCur);

  // Release the capture
  IScreenDisplayPtr ipScreenDisplay;
  IActiveViewPtr ipActiveView;
  GetFocusMapActiveView(&ipActiveView);
  ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
  HWND hWnd;
  ipScreenDisplay->get_hWnd((UINT*) &hWnd);
  if (::GetCapture() == hWnd)
    ::ReleaseCapture();
}




