/*
 Copyright 1995-2004 ESRI

 All rights reserved under the copyright laws of the United States.

 You may freely redistribute and use this sample code, with or without modification.

 Disclaimer: THE SAMPLE CODE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED 
 WARRANTIES, INCLUDING THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ESRI OR 
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 INTERRUPTION) SUSTAINED BY YOU OR A THIRD PARTY, HOWEVER CAUSED AND ON ANY 
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT ARISING IN ANY 
 WAY OUT OF THE USE OF THIS SAMPLE CODE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 SUCH DAMAGE.

 For additional information contact: Environmental Systems Research Institute, Inc.

 Attn: Contracts Dept.

 380 New York Street

 Redlands, California, U.S.A. 92373 

 Email: contracts@esri.com
*/
// MapCommandBase.h: interface for the CMapCommandBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMANDBASE_H__DD5718A6_49C2_4C33_B5FE_4290134B52AF__INCLUDED_)
#define AFX_COMMANDBASE_H__DD5718A6_49C2_4C33_B5FE_4290134B52AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 
// Provides basic implementation of a command that requires a focus map
// Inherit this into your command object, then override any required functions
//
// Default behaviour
//   Enabled - True if there is a focus map
//   Checked - False
//   Name    - Combination of Caption and category
//   Caption - supplied from constructor
//   Tooltip - defaults to Caption if not supplied
//   Message - defaults to Caption is not supplied
//   OnCreate - Initialises hook
//   all others just return S_OK
//
// The constructor will initialise the base class with bitmaps and
// strings specified by resource ID's
//
class CMapCommandBase : public _ICommand
{
public:
	CMapCommandBase(UINT bitmapID = NULL, 
               UINT caption = NULL,     
               UINT category = NULL,
               UINT tooltip = NULL,
               UINT message = NULL,
               UINT helpfile = NULL,
               LONG helpID = 0);
	virtual ~CMapCommandBase();

  public:
// ICommand
	STDMETHOD(get_Enabled)(VARIANT_BOOL * Enabled);
	STDMETHOD(get_Checked)(VARIANT_BOOL * Checked);
	STDMETHOD(get_Name)(BSTR * Name);
	STDMETHOD(get_Caption)(BSTR * Caption);
	STDMETHOD(get_Tooltip)(BSTR * Tooltip);
	STDMETHOD(get_Message)(BSTR * Message);
	STDMETHOD(get_HelpFile)(BSTR * HelpFile);
	STDMETHOD(get_HelpContextID)(LONG * helpID);
	STDMETHOD(get_Bitmap)(OLE_HANDLE * Bitmap);
	STDMETHOD(get_Category)(BSTR * categoryName);
	STDMETHOD(OnCreate)(IDispatch * hook);
	STDMETHOD(OnClick)();
  
protected:
  IApplicationPtr	m_ipApp;
  HBITMAP			m_hBitmap;
  CComBSTR			m_bstrCaption;
  CComBSTR			m_bstrCategory;
  CComBSTR			m_bstrName;
  CComBSTR			m_bstrTooltip;
  CComBSTR			m_bstrMessage;
  CComBSTR			m_bstrHelpfile;
  LONG				m_lHelpID;
  bool				m_bChecked;

  //
  // Helper functions
  //
  HRESULT GetFocusMapActiveView(IActiveView** ppActiveView);
  HRESULT GetFocusMapScreenDisplay(IScreenDisplay** ppScreenDisplay);
  HRESULT ToFocusMapPoint(LONG X, LONG Y, IPoint** ppPoint);
};

#endif // !defined(AFX_COMMANDBASE_H__DD5718A6_49C2_4C33_B5FE_4290134B52AF__INCLUDED_)
