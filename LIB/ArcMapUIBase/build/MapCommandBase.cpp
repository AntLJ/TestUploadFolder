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
// MapCommandBase.cpp: implementation of the CMapCommandBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapCommandBase.h"
//#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapCommandBase::CMapCommandBase(UINT bitmapID,
                           UINT caption,
                           UINT category,
                           UINT tooltip,
                           UINT message,
                           UINT helpfile,
                           LONG helpID)
{

  m_hBitmap = (bitmapID) ? (HBITMAP) ::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE) : NULL;

  if (caption)
    m_bstrCaption.LoadString(caption);

  if (category)
    m_bstrCategory.LoadString(category);

  if (caption)
  {
    // Make name from caption and category
    m_bstrName = m_bstrCaption;
    m_bstrName.Append("_");
    m_bstrName.Append(m_bstrCategory);
  }

  if (tooltip)
    m_bstrTooltip.LoadString(tooltip);
  else if (caption)
    m_bstrTooltip = m_bstrCaption;

  if (message)
    m_bstrMessage.LoadString(message);
  else if (caption)
    m_bstrMessage = m_bstrCaption;

  if (helpfile)
    m_bstrHelpfile.LoadString(helpfile);

  m_bChecked = false;
}

CMapCommandBase::~CMapCommandBase()
{

  if (m_hBitmap)
    ::DeleteObject(m_hBitmap);
  m_hBitmap = 0;

}


STDMETHODIMP CMapCommandBase::get_Caption(BSTR * Caption)
{
  *Caption = m_bstrCaption.Copy();

  return S_OK;
}

STDMETHODIMP CMapCommandBase::get_Name(BSTR * Name)
{
  *Name = m_bstrName.Copy();

  return S_OK;
}

STDMETHODIMP CMapCommandBase::get_Tooltip(BSTR * Tooltip)
{
  *Tooltip = m_bstrTooltip.Copy();

  return S_OK;
}

STDMETHODIMP CMapCommandBase::get_Message(BSTR * Message)
{
  *Message = m_bstrMessage.Copy();

  return S_OK;
}



STDMETHODIMP CMapCommandBase::get_Enabled(VARIANT_BOOL * Enabled)
{
	if (Enabled == NULL)
		return E_POINTER;
	
  // Must have focus map to be enabled
  IActiveViewPtr ipActiveView;
  GetFocusMapActiveView(&ipActiveView);

  *Enabled = (ipActiveView == 0) ? VARIANT_FALSE : VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CMapCommandBase::get_Checked(VARIANT_BOOL * Checked)
{
	if (Checked == NULL)
		return E_POINTER;
	
	*Checked = (m_bChecked) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


STDMETHODIMP CMapCommandBase::get_HelpFile(BSTR * HelpFile)
{
	if (HelpFile == NULL)
		return E_POINTER;
		
	return E_NOTIMPL;
}
STDMETHODIMP CMapCommandBase::get_HelpContextID(LONG * helpID)
{
	if (helpID == NULL)
		return E_POINTER;
		
	return E_NOTIMPL;
}

STDMETHODIMP CMapCommandBase::get_Bitmap(OLE_HANDLE * Bitmap)
{
	if (Bitmap == NULL)
		return E_POINTER;

  *Bitmap = (OLE_HANDLE) m_hBitmap;
		
	return S_OK;
}

STDMETHODIMP CMapCommandBase::get_Category(BSTR * categoryName)
{
	if (categoryName == NULL)
		return E_POINTER;

  *categoryName = m_bstrCategory.Copy();
		
	return E_NOTIMPL;
}

STDMETHODIMP CMapCommandBase::OnCreate(IDispatch * hook)
{
	m_ipApp = hook;
	return S_OK;
}


STDMETHODIMP CMapCommandBase::OnClick()
{
  return S_OK;
}



/////////////////////////////
HRESULT CMapCommandBase::GetFocusMapActiveView(IActiveView** ppActiveView)
{
	if (ppActiveView == 0)
		return E_POINTER;

	HRESULT hr = E_FAIL;

	if( NULL != m_ipApp )
	{
		IDocumentPtr ipDoc;
		if( SUCCEEDED( hr = m_ipApp->get_Document( &ipDoc ) ) )
		{
			IMxDocumentPtr ipMxDoc( ipDoc );
			if( NULL != ipMxDoc )
			{
				IMapPtr ipMap;
				if( SUCCEEDED( hr = ipMxDoc->get_FocusMap( &ipMap ) ) )
				{
					IActiveViewPtr ipView( ipMap );
					*ppActiveView = ipView.Detach();
				}
			}
			else hr = E_FAIL;
		}
	}

	return hr;
}


HRESULT CMapCommandBase::GetFocusMapScreenDisplay(IScreenDisplay** ppScreenDisplay)
{
	if (ppScreenDisplay==0)
		return E_POINTER;

	IActiveViewPtr ipActiveView;
	GetFocusMapActiveView(&ipActiveView);
	IScreenDisplayPtr ipScreenDisplay;

	if (ipActiveView)
		ipActiveView->get_ScreenDisplay(&ipScreenDisplay);

	*ppScreenDisplay = ipScreenDisplay.Detach();

	return S_OK;
}


HRESULT CMapCommandBase::ToFocusMapPoint(LONG X, LONG Y, IPoint** ppPoint)
{
	IActiveViewPtr ipActiveView;
	GetFocusMapActiveView(&ipActiveView);

	if (ipActiveView == 0)
		return E_FAIL;

	HRESULT hr = S_OK;

	IScreenDisplayPtr ipScreenDisplay;
	IDisplayTransformationPtr ipDisplayTrans;

	hr = ipActiveView->get_ScreenDisplay(&ipScreenDisplay);
	if (ipScreenDisplay)
		hr = ipScreenDisplay->get_DisplayTransformation(&ipDisplayTrans);

	if (ipDisplayTrans)
		hr = ipDisplayTrans->ToMapPoint(X, Y, ppPoint);

	return hr;
}

