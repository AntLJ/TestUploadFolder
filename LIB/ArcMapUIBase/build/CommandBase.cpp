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
// MapCommandBase.cpp: implementation of the CCommandBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommandBase::CCommandBase(UINT bitmapID/* = 0U*/,
                           _U_STRINGorID caption/* = 0U*/,
						   _U_STRINGorID name/* = 0U*/,
                           _U_STRINGorID category/* = 0U*/,
                           _U_STRINGorID tooltip/* = 0U*/,
                           _U_STRINGorID message/* = 0U*/,
                           _U_STRINGorID helpfile/* = 0U*/,
                           UINT helpID/* = 0U*/)
{
	Init();
	SetInfo( bitmapID, caption, name, category, tooltip, message, helpfile, helpID );
}

CCommandBase::~CCommandBase()
{
	Clear();
}

void CCommandBase::Init()
{
	m_hBitmap = NULL;
	m_lHelpID = 0;
	m_bChecked = false;
}

void CCommandBase::Clear()
{
	if( m_hBitmap ) DeleteObject(m_hBitmap);
	m_hBitmap = NULL;
	if( m_bstrCaption ) m_bstrCaption.Empty();
	if( m_bstrCategory ) m_bstrCategory.Empty();
	if( m_bstrTooltip ) m_bstrTooltip.Empty();
	if( m_bstrName ) m_bstrName.Empty();
	if( m_bstrMessage ) m_bstrMessage.Empty();
	if( m_bstrHelpfile ) m_bstrHelpfile.Empty();
}

void CCommandBase::SetInfo( UINT bitmapID, const _U_STRINGorID& caption, const _U_STRINGorID& name, const _U_STRINGorID& category, const _U_STRINGorID& tooltip, const _U_STRINGorID& message, const _U_STRINGorID& helpfile, UINT helpID )
{
	m_hBitmap = (bitmapID) ? (HBITMAP) ::LoadImage(ATL::_AtlBaseModule.GetResourceInstance(), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE) : NULL;

	if( ! m_bstrCaption.LoadString( (const UINT)caption.m_lpstr ) )
		m_bstrCaption = caption.m_lpstr;

	if( ! m_bstrCategory.LoadString( (const UINT)category.m_lpstr ) )
		m_bstrCategory = category.m_lpstr;

	if( ! m_bstrName.LoadString( (const UINT)name.m_lpstr ) )
		m_bstrName = name.m_lpstr;
	if( ! ( 0 < m_bstrName.Length() ) )
	{
		// Make name from caption and category
		m_bstrName = m_bstrCaption;
		m_bstrName.Append("_");
		m_bstrName.Append(m_bstrCategory);
	}

	if( ! m_bstrTooltip.LoadString( (const UINT)tooltip.m_lpstr ) )
		m_bstrTooltip = tooltip.m_lpstr;
	if( ! ( 0 < m_bstrTooltip.Length() ) )
		m_bstrTooltip = m_bstrCaption;

	if( ! m_bstrMessage.LoadString( (const UINT)message.m_lpstr ) )
		m_bstrMessage = message.m_lpstr;
	if( ! ( 0 < m_bstrMessage.Length() ) )
		m_bstrMessage = m_bstrCaption;

	if( ! m_bstrHelpfile.LoadString( (const UINT)helpfile.m_lpstr ) )
		m_bstrHelpfile = helpfile.m_lpstr;

	m_lHelpID = helpID;

	m_bChecked = false;
}

STDMETHODIMP CCommandBase::get_Caption(BSTR * Caption)
{
	*Caption = m_bstrCaption.Copy();

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Name(BSTR * Name)
{
	*Name = m_bstrName.Copy();

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Tooltip(BSTR * Tooltip)
{
	*Tooltip = m_bstrTooltip.Copy();

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Message(BSTR * Message)
{
	*Message = m_bstrMessage.Copy();

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Enabled(VARIANT_BOOL * Enabled)
{
	if (Enabled == NULL)
		return E_POINTER;

	// Must have focus map to be enabled
	*Enabled = ( GetFocusMap() == NULL ) ? VARIANT_FALSE : VARIANT_TRUE;

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Checked(VARIANT_BOOL * Checked)
{
	if (Checked == NULL)
		return E_POINTER;
	
	*Checked = (m_bChecked) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}


STDMETHODIMP CCommandBase::get_HelpFile(BSTR * HelpFile)
{
	if (HelpFile == NULL)
		return E_POINTER;

	*HelpFile = m_bstrHelpfile.Copy();

	return S_OK;
}
STDMETHODIMP CCommandBase::get_HelpContextID(LONG * helpID)
{
	if (helpID == NULL)
		return E_POINTER;

	*helpID = m_lHelpID;

	return S_OK;
}

STDMETHODIMP CCommandBase::get_Bitmap(OLE_HANDLE * Bitmap)
{
	if (Bitmap == NULL)
		return E_POINTER;

	*Bitmap = (OLE_HANDLE) m_hBitmap;
		
	return S_OK;
}

STDMETHODIMP CCommandBase::get_Category(BSTR * categoryName)
{
	if (categoryName == NULL)
		return E_POINTER;

	*categoryName = m_bstrCategory.Copy();
		
	return S_OK;
}

STDMETHODIMP CCommandBase::OnCreate(IDispatch * hook)
{
	m_cApp = (IApplicationPtr)hook;
	return S_OK;
}


STDMETHODIMP CCommandBase::OnClick()
{
	return S_OK;
}

/////////////////////////////
IDocumentPtr CCommandBase::GetDocument() const
{
	return m_cApp.GetDocument();
}
IMapPtr CCommandBase::GetFocusMap() const
{
	return m_cApp.GetFocusMap();
}

IScreenDisplayPtr CCommandBase::GetFocusMapScreenDisplay() const
{
	return m_cApp.GetFocusMapScreenDisplay();
}


IPointPtr CCommandBase::ToFocusMapPoint(LONG X, LONG Y) const
{
	return m_cApp.ToMapPoint( (IActiveViewPtr)m_cApp.GetFocusMap(), X, Y );
}

IUnknownPtr CCommandBase::GetContextItem() const
{
	IUnknownPtr ipUnk;
	IMxDocumentPtr ipDoc( GetDocument() );
	if( NULL != ipDoc )
		ipDoc->get_ContextItem( &ipUnk );

	return ipUnk;
}
