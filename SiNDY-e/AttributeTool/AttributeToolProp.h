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

// AttributeToolProp.h : CAttributeToolProp の宣言

#ifndef __ATTRIBUTETOOLPROP_H_
#define __ATTRIBUTETOOLPROP_H_

#include "resource.h"       // メイン シンボル
#include "AttributeTool.h"

EXTERN_C const CLSID CLSID_AttributeToolProp;
_COM_SMARTPTR_TYPEDEF(IAttributeToolObj, __uuidof(IAttributeToolObj));
/////////////////////////////////////////////////////////////////////////////
// CAttributeToolProp
class ATL_NO_VTABLE CAttributeToolProp :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAttributeToolProp, &CLSID_AttributeToolProp>,
	public IPropertyPageImpl<CAttributeToolProp>,
	public CDialogImpl<CAttributeToolProp>,
	public IPropertyPageContext
{
public:
	CAttributeToolProp() 
	{
		m_dwTitleID = IDS_TITLEAttributeToolProp;
		m_dwHelpFileID = IDS_HELPFILEAttributeToolProp;
		m_dwDocStringID = IDS_DOCSTRINGAttributeToolProp;

		m_bIsSetted = FALSE;
	}

	enum {IDD = IDD_ATTRIBUTETOOLPROP};

DECLARE_REGISTRY_RESOURCEID(IDR_ATTRIBUTETOOLPROP)

DECLARE_PROTECT_FINAL_CONSTRUCT()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp COMマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_COM_MAP(CAttributeToolProp) 
	COM_INTERFACE_ENTRY(IPropertyPage)
	COM_INTERFACE_ENTRY(IPropertyPageContext)
END_COM_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp カテゴリマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_CATEGORY_MAP(CAttributeToolProp)
	IMPLEMENTED_CATEGORY(__uuidof(CATID_iPCSiNDYProperty))
END_CATEGORY_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp メッセージマップ定義
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttributeToolProp)
	CHAIN_MSG_MAP(IPropertyPageImpl<CAttributeToolProp>)
	COMMAND_HANDLER(IDC_CHECK_LINEHILIGHT, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_CHECK_VERTEXHILIGHT, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_EDIT_R, EN_CHANGE, OnChange)
	COMMAND_HANDLER(IDC_EDIT_G, EN_CHANGE, OnChange)
	COMMAND_HANDLER(IDC_EDIT_B, EN_CHANGE, OnChange)
END_MSG_MAP()
// ハンドラのプロトタイプ:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp イベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( m_bIsSetted == TRUE ) 
			SetDirty(TRUE);

		return 0;
	}
/////////////////////////////////////////////////////////////////////////////
//
// IPropertyPage インターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(Apply)(void);
	STDMETHOD(Show)(UINT nCmdShow);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
/////////////////////////////////////////////////////////////////////////////
//
// IPropertyPageContext インターフェース定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(get_Priority)(LONG * Priority);
	STDMETHOD(Applies)(VARIANT unkArray, VARIANT_BOOL * Applies);
	STDMETHOD(CreateCompatibleObject)(VARIANT kind, VARIANT * pNewObject);
	STDMETHOD(QueryObject)(VARIANT theObject);
	STDMETHOD(GetHelpFile)(LONG controlID, BSTR * HelpFile);
	STDMETHOD(GetHelpId)(LONG controlID, LONG * helpID);
	STDMETHOD(Cancel)();
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	void SetDialog( IAttributeToolObjPtr ipCommand );
	void GetDialog( IAttributeToolObjPtr ipCommand );
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolProp メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	IAttributeToolObjPtr	m_ipCommand;
	BOOL					m_bIsSetted;
};

#endif //__ATTRIBUTETOOLPROP_H_
