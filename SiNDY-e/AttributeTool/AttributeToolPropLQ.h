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

// AttributeToolPropLQ.h : CAttributeToolPropLQ �̐錾

#ifndef __ATTRIBUTETOOLPROPLQ_H_
#define __ATTRIBUTETOOLPROPLQ_H_

#include "resource.h"       // ���C�� �V���{��
#include "AttributeTool.h"

EXTERN_C const CLSID CLSID_AttributeToolPropLQ;
_COM_SMARTPTR_TYPEDEF(IAttributeToolObj, __uuidof(IAttributeToolObj));

/////////////////////////////////////////////////////////////////////////////
// CAttributeToolPropLQ
class ATL_NO_VTABLE CAttributeToolPropLQ :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAttributeToolPropLQ, &CLSID_AttributeToolPropLQ>,
	public IPropertyPageImpl<CAttributeToolPropLQ>,
	public CDialogImpl<CAttributeToolPropLQ>,
	public IPropertyPageContext
{
public:
	CAttributeToolPropLQ() 
	{
		m_dwTitleID = IDS_TITLEAttributeToolPropLQ;
		m_dwHelpFileID = IDS_HELPFILEAttributeToolPropLQ;
		m_dwDocStringID = IDS_DOCSTRINGAttributeToolPropLQ;

		m_bIsSetted = FALSE;
	}

	enum {IDD = IDD_ATTRIBUTETOOLPROPLQ};

DECLARE_REGISTRY_RESOURCEID(IDR_ATTRIBUTETOOLPROPLQ)

DECLARE_PROTECT_FINAL_CONSTRUCT()

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolPropLQ COM�}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_COM_MAP(CAttributeToolPropLQ) 
	COM_INTERFACE_ENTRY(IPropertyPage)
	COM_INTERFACE_ENTRY(IPropertyPageContext)
END_COM_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolPropLQ �J�e�S���}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_CATEGORY_MAP(CAttributeToolPropLQ)
	IMPLEMENTED_CATEGORY(__uuidof(CATID_iPCSiNDYProperty))
END_CATEGORY_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolPropLQ ���b�Z�[�W�}�b�v��`
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttributeToolPropLQ)
	CHAIN_MSG_MAP(IPropertyPageImpl<CAttributeToolPropLQ>)
	COMMAND_HANDLER(IDC_RADIO_SELECTLASTSEQ, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_RADIO_PERFECTSELECT, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_CHECK_LANECONSISTENCY, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_CHECK_LANECONSISTENCYW, BN_CLICKED, OnChange)
	COMMAND_HANDLER(IDC_CHECK_LANEBITEDITABLE, BN_CLICKED, OnChange)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolPropLQ �C�x���g�n���h��
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
// IPropertyPage �C���^�[�t�F�[�X��`
//
/////////////////////////////////////////////////////////////////////////////
public:
	STDMETHOD(Apply)(void);
	STDMETHOD(Show)(UINT nCmdShow);
	STDMETHOD(SetObjects)(ULONG nObjects, IUnknown **ppUnk);
/////////////////////////////////////////////////////////////////////////////
//
// IPropertyPageContext �C���^�[�t�F�[�X��`
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
// CAttributeToolPropLQ �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	void SetDialog( IAttributeToolObjPtr ipCommand );
	void GetDialog( IAttributeToolObjPtr ipCommand );
/////////////////////////////////////////////////////////////////////////////
//
// CAttributeToolPropLQ �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	IAttributeToolObjPtr	m_ipCommand;
	BOOL					m_bIsSetted;
};

#endif //__ATTRIBUTETOOLPROPLQ_H_
