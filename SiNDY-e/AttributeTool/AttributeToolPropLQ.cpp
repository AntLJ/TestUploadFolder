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

// AttributeToolPropLQ.cpp : CAttributeToolPropLQ のインプリメンテーション
#include "stdafx.h"
#include "AttributeTool.h"
#include "AttributeToolPropLQ.h"

/////////////////////////////////////////////////////////////////////////////
// CAttributeToolPropLQ

STDMETHODIMP CAttributeToolPropLQ::Apply(void)
{
	return QueryObject(CComVariant((IUnknown*) m_ipCommand));
}
STDMETHODIMP CAttributeToolPropLQ::Show(UINT nCmdShow)
{
	if ((m_ipCommand != 0) && (m_hWnd != 0) && ( !m_bIsSetted ))
	{
		SetDialog( m_ipCommand );
	}

	return IPropertyPageImpl<CAttributeToolPropLQ>::Show(nCmdShow);
}
STDMETHODIMP CAttributeToolPropLQ::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	for (ULONG i=0; i < nObjects; i++)
	{
		IAttributeToolObjPtr ipCommand(ppUnk[i]);
		if (ipCommand != 0)
			m_ipCommand = ipCommand;
	}

	return IPropertyPageImpl<CAttributeToolPropLQ>::SetObjects(nObjects, ppUnk);
}
// IPropertyPageContext
STDMETHODIMP CAttributeToolPropLQ::get_Priority(LONG * Priority)
{
	if (Priority == NULL)
		return E_POINTER;

	// Return the priority of the property page	
	*Priority = PROP_PRIORITY_ATTRIBUTETOOL;

	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::Applies(VARIANT unkArray, VARIANT_BOOL * Applies)
{
	if (Applies == NULL)
		return E_POINTER;

	// Default Applies to False
	*Applies = VARIANT_FALSE;

	// Check if the array contains an IEditToolObj interface
	if (V_VT(&unkArray) != (VT_ARRAY|VT_UNKNOWN)) return E_INVALIDARG;

	SAFEARRAY *saArray;
	saArray = unkArray.parray;
	HRESULT hr = SafeArrayLock(saArray);
	if (FAILED(hr)) return hr;

	IUnknownPtr *ippUnk;
	hr = SafeArrayAccessData(saArray,reinterpret_cast<void**> (&ippUnk));
	if (FAILED(hr)) return hr;

	// Loop through the data in the safe array and try an QI
	// for an object that implements the IEditToolObj Interface
	long lNumElements = saArray->rgsabound->cElements;
	for (long i = 0; i < lNumElements; i++)
	{
		// Attempt to QI to IAttributeToolObjPtr
		IAttributeToolObjPtr ipCommand(ippUnk[i]);
		if (ipCommand != 0)
		{
			// If the QI was successful we have an object that supports
			// Interface and the property page therefore applies
			*Applies = VARIANT_TRUE;
			break;
		}
	}

	hr = SafeArrayUnaccessData(saArray);
	if (FAILED(hr)) return hr;
	hr = SafeArrayUnlock(saArray);
	if (FAILED(hr)) return hr;

	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::CreateCompatibleObject(VARIANT kind, VARIANT * pNewObject)
{
	if (pNewObject == NULL)
		return E_POINTER;

	// This method looks at the kind variant which contains a
	// IUnknown pointer an object and checks if that object
	// Implements the ISliverPolygonRenderer interface.
	// If it does we return the interface in the pNewObject
	// Variant.  If not we create a new instance of the
	// SliverPolygonRenderer coclass and return that in the
	// pNewObject variant.  If the object in the kind varaint
	// supports an interface we do know about then we could
	// copy settings from the renderer onto your new renderer.

	IAttributeToolObjPtr ipCommand(kind.punkVal);
	if (ipCommand == 0)
	{
		IAttributeToolObjPtr ipAttributeCommand(CLSID_AttributeToolObj);
		if (ipAttributeCommand == 0) return E_FAIL;

		::VariantClear(pNewObject);
		pNewObject->vt = VT_UNKNOWN;
		pNewObject->punkVal = ipAttributeCommand;
		(pNewObject->punkVal)->AddRef();
	}
	else
	{
		*pNewObject = kind;
		(pNewObject->punkVal)->AddRef();
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::QueryObject(VARIANT theObject)
{
	// Macro to enable text conversions, see W2A
	USES_CONVERSION;

	// Check if we have an ISliverPolygonRenderer
	// If we do, apply the setting from the page.
	// renderer.
	CComVariant vObject(theObject);
	if (vObject.vt != VT_UNKNOWN) return E_UNEXPECTED;

	// Try and QI to ISliverPolygonRenderer
	IAttributeToolObjPtr ipCommand(vObject.punkVal);
	if (ipCommand != 0)
	{
		// Read the contents of the property page and update
		// the renderer with the new data.

		// Read the EditBox to get the sliver area value
		GetDialog( ipCommand );
	}

	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::GetHelpFile(LONG controlID, BSTR * HelpFile)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::GetHelpId(LONG controlID, LONG * helpID)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolPropLQ::Cancel()
{
	return S_OK;
}

void CAttributeToolPropLQ::SetDialog( IAttributeToolObjPtr ipCommand )
{
	// 方面、レーンの場合に完璧に取得するかどうか
	VARIANT_BOOL vaPerfectSelect;
	ipCommand->get_PerfectSelect( &vaPerfectSelect );
	::SendMessage(GetDlgItem(IDC_RADIO_PERFECTSELECT), BM_SETCHECK, (vaPerfectSelect == VARIANT_TRUE ? BST_CHECKED : BST_UNCHECKED), 0L);

	// 方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
	VARIANT_BOOL vaSelectLastSequence;
	ipCommand->get_SelectLastSequence( &vaSelectLastSequence );
	::SendMessage(GetDlgItem(IDC_RADIO_SELECTLASTSEQ), BM_SETCHECK, (vaSelectLastSequence == VARIANT_TRUE ? BST_CHECKED : BST_UNCHECKED), 0L);

	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		DWORD dwResult;
		// レーンの場合に常に整合性をチェックするかどうか
		if( EmxGetRegDword( _T("LaneConsistency"), &dwResult, hOpenKey ) )
			::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCY), BM_SETCHECK, dwResult, 0L );
		else
			::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCY), BM_SETCHECK, BST_CHECKED, 0L );

		// 整合性をチェックしない場合に警告を出すかどうか
		if( EmxGetRegDword( _T("LaneConsistencyw"), &dwResult, hOpenKey ) )
			::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCYW), BM_SETCHECK, dwResult, 0L );
		else
			::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCYW), BM_SETCHECK, BST_UNCHECKED, 0L );

		// レーンのビット操作を許可するかどうか
		if( EmxGetRegDword( _T("LaneBitEditable"), &dwResult, hOpenKey ) )
			::SendMessage(GetDlgItem(IDC_CHECK_LANEBITEDITABLE), BM_SETCHECK, dwResult, 0L );
		else
			::SendMessage(GetDlgItem(IDC_CHECK_LANEBITEDITABLE), BM_SETCHECK, BST_UNCHECKED, 0L );

		RegCloseKey( hOpenKey );
	}

	SetDirty(FALSE);
	m_bIsSetted = TRUE;
}
void CAttributeToolPropLQ::GetDialog( IAttributeToolObjPtr ipCommand )
{
	// 方面、レーンの場合に完璧に取得するかどうか
	INT nPerfectSelect = (INT)::SendMessage(GetDlgItem(IDC_RADIO_PERFECTSELECT), BM_GETCHECK, 0, 0L);
	ipCommand->put_PerfectSelect( (nPerfectSelect == 0 ? VARIANT_FALSE : VARIANT_TRUE) );

	// 方面、レーンの場合に矩形選択した道路リンクを流入リンクとするリンク列を選択するかどうか
	INT nSelectLastSequence = (INT)::SendMessage(GetDlgItem(IDC_RADIO_SELECTLASTSEQ), BM_GETCHECK, 0, 0L);
	ipCommand->put_SelectLastSequence( (nSelectLastSequence == 0 ? VARIANT_FALSE : VARIANT_TRUE) );

	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) {
		// レーンの場合に常に整合性をチェックするかどうか
		INT nLaneConsistency = (INT)::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCY), BM_GETCHECK, 0, 0L);
		EmxSetRegDword( _T("LaneConsistency"), (DWORD)nLaneConsistency, hOpenKey );

		// レーンの場合に常に整合性をチェックするかどうか
		INT nLaneConsistencyw = (INT)::SendMessage(GetDlgItem(IDC_CHECK_LANECONSISTENCYW), BM_GETCHECK, 0, 0L);
		EmxSetRegDword( _T("LaneConsistencyw"), (DWORD)nLaneConsistencyw, hOpenKey );

		// レーンのビット操作を許可するかどうか
		INT nLaneBitEditable = (INT)::SendMessage(GetDlgItem(IDC_CHECK_LANEBITEDITABLE), BM_GETCHECK, 0, 0L );
		EmxSetRegDword( _T("LaneBitEditable"), (DWORD)nLaneBitEditable, hOpenKey );

		RegCloseKey( hOpenKey );
	}

	m_bIsSetted = FALSE;
}
