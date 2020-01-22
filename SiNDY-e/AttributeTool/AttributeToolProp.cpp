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

// AttributeToolProp.cpp : CAttributeToolProp �̃C���v�������e�[�V����
#include "stdafx.h"
#include "AttributeTool.h"
#include "AttributeToolProp.h"

/////////////////////////////////////////////////////////////////////////////
// CAttributeToolProp

STDMETHODIMP CAttributeToolProp::Apply(void)
{
	return QueryObject(CComVariant((IUnknown*) m_ipCommand));
}
STDMETHODIMP CAttributeToolProp::Show(UINT nCmdShow)
{
	if ((m_ipCommand != 0) && (m_hWnd != 0) && ( !m_bIsSetted ))
	{
		SetDialog( m_ipCommand );
	}

	return IPropertyPageImpl<CAttributeToolProp>::Show(nCmdShow);
}
STDMETHODIMP CAttributeToolProp::SetObjects(ULONG nObjects, IUnknown **ppUnk)
{
	for (ULONG i=0; i < nObjects; i++)
	{
		IAttributeToolObjPtr ipCommand(ppUnk[i]);
		if (ipCommand != 0)
			m_ipCommand = ipCommand;
	}

	return IPropertyPageImpl<CAttributeToolProp>::SetObjects(nObjects, ppUnk);
}
// IPropertyPageContext
STDMETHODIMP CAttributeToolProp::get_Priority(LONG * Priority)
{
	if (Priority == NULL)
		return E_POINTER;

	// Return the priority of the property page	
	*Priority = PROP_PRIORITY_ATTRIBUTETOOL;

	return S_OK;
}
STDMETHODIMP CAttributeToolProp::Applies(VARIANT unkArray, VARIANT_BOOL * Applies)
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
STDMETHODIMP CAttributeToolProp::CreateCompatibleObject(VARIANT kind, VARIANT * pNewObject)
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
STDMETHODIMP CAttributeToolProp::QueryObject(VARIANT theObject)
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
STDMETHODIMP CAttributeToolProp::GetHelpFile(LONG controlID, BSTR * HelpFile)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolProp::GetHelpId(LONG controlID, LONG * helpID)
{
	return S_OK;
}
STDMETHODIMP CAttributeToolProp::Cancel()
{
	return S_OK;
}

void CAttributeToolProp::SetDialog( IAttributeToolObjPtr ipCommand )
{
	// �{���Ȃ�΁A���݂̃c�[���̏�Ԃ���l���擾����ׂ��ł��邪�AAttributeDlg���ɂ��̂悤��
	// �C���^�[�t�F�C�X��p�ӂ��Ă��Ȃ����߁A���W�X�g���o�R�Œl�����o���v���p�e�B���Z�b�g����B
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		// ���C���̎�����n�C���C�g�\�����邩�ǂ����H
		DWORD dwResult;
		// ���[���̏ꍇ�ɏ�ɐ��������`�F�b�N���邩�ǂ���
		if( EmxGetRegDword( _T("IsLineHilight"), &dwResult, hOpenKey ) )
			::SendMessage(GetDlgItem(IDC_CHECK_LINEHILIGHT), BM_SETCHECK, dwResult, 0L );
		else
			::SendMessage(GetDlgItem(IDC_CHECK_LINEHILIGHT), BM_SETCHECK, BST_UNCHECKED, 0L );

		// ���C���̍\���_���n�C���C�g�\�����邩�ǂ����H
		if( EmxGetRegDword( _T("IsVertexHilight"), &dwResult, hOpenKey ) )
			::SendMessage(GetDlgItem(IDC_CHECK_VERTEXHILIGHT), BM_SETCHECK, dwResult, 0L );
		else
			::SendMessage(GetDlgItem(IDC_CHECK_VERTEXHILIGHT), BM_SETCHECK, BST_UNCHECKED, 0L );

		CString strTemp;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboR"), &dwResult, hOpenKey ) )
		{
			strTemp.Format( _T("%d"), dwResult );
			SetDlgItemText( IDC_EDIT_R, strTemp );
		}
		else
		{
			strTemp.Format( _T("214") );
			SetDlgItemText( IDC_EDIT_R, strTemp );
		}
		// �R���{�{�b�N�X�̔w�i�F��G�l
		if( EmxGetRegDword( _T("ComboG"), &dwResult, hOpenKey ) )
		{
			strTemp.Format( _T("%d"), dwResult );
			SetDlgItemText( IDC_EDIT_G, strTemp );
		}
		else
		{
			strTemp.Format( _T("211") );
			SetDlgItemText( IDC_EDIT_G, strTemp );
		}

		// �R���{�{�b�N�X�̔w�i�F��B�l
		if( EmxGetRegDword( _T("ComboB"), &dwResult, hOpenKey ) )
		{
			strTemp.Format( _T("%d"), dwResult );
			SetDlgItemText( IDC_EDIT_B, strTemp );
		}
		else
		{
			strTemp.Format( _T("206") );
			SetDlgItemText( IDC_EDIT_B, strTemp );
		}

		RegCloseKey( hOpenKey );
	}

	SetDirty(FALSE);
	m_bIsSetted = TRUE;
	
}
void CAttributeToolProp::GetDialog( IAttributeToolObjPtr ipCommand )
{
	// ��̂̑����ҏW�c�[���ł́AIAttributeTool�C���^�[�t�F�[�X(put_IsLineHilight��)��p���āA
	// AttributeTool���Ƀv���p�e�B�̕ύX�������������Ƃ�m�点�Ă������A���݂́A���C���̃n�C���C�g��
	// �R���{�{�b�N�X�̔w�i�F�Ȃǂ́AAttributeDlg���ōs���Ă���AAttributeDlg���ɃC���^�[�t�F�C�X��
	// �p�ӂ��Ă��Ȃ��i�g�����Ƃ��ł��Ȃ��̂ō폜�����j�̂ŁA���W�X�g���Ɋe��l��ݒ肵�Ă����A
	// AttributeDlg���ł�����ǂݍ��ގd�l�ɂ��Ă���BAttributeToolPropLQ�ł́A������IAttributeTool��
	// �C���^�[�t�F�C�X��p���Đݒ肵�Ă���B�����Ƃ�肽���ꍇ�ɂ́A��������Q�l�ɂ��ꂽ���B
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey ) 
	{
		// ���C���̎�����n�C���C�g�\�����邩�ǂ����H
		INT nLaneHilight = (INT)::SendMessage(GetDlgItem(IDC_CHECK_LINEHILIGHT), BM_GETCHECK, 0, 0L);
		EmxSetRegDword( _T("IsLineHilight"), (DWORD)nLaneHilight, hOpenKey );

		// ���C���̍\���_���n�C���C�g�\�����邩�ǂ����H
		INT nVertexHilight = (INT)::SendMessage(GetDlgItem(IDC_CHECK_VERTEXHILIGHT), BM_GETCHECK, 0, 0L);
		EmxSetRegDword( _T("IsVertexHilight"), (DWORD)nVertexHilight, hOpenKey );

		CString		strTmp;
		BSTR		bstrR, bstrG, bstrB;
		bstrR = bstrG = bstrB = NULL;
		
		// �R���{�{�b�N�X�̔w�i�F��ݒ�
		USES_CONVERSION;
		GetDlgItemText( IDC_EDIT_R, bstrR );
		long lR = _ttol(OLE2T(bstrR));
		EmxSetRegDword( _T("ComboR"), (DWORD)lR, hOpenKey );
		SysFreeString(bstrR);

		GetDlgItemText( IDC_EDIT_G, bstrG );
		long lG = _ttol(OLE2T(bstrG));
		EmxSetRegDword( _T("ComboG"), (DWORD)lG, hOpenKey );
		SysFreeString(bstrG);	

		GetDlgItemText( IDC_EDIT_B, bstrB );
		long lB = _ttol(OLE2T(bstrB));
		EmxSetRegDword( _T("ComboB"), (DWORD)lB, hOpenKey );
		SysFreeString(bstrB);	

		RegCloseKey( hOpenKey );
	}

	m_bIsSetted = FALSE;
}
