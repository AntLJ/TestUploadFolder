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

#include "RelNodeInfos.h"
#include "RelNodeGridController.h"


// �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); 


class CAttrCrosswalkLineDlg : 
	public CAttrBaseDlg,
	public CExportDlg2,
	public IDispEventImpl<IDC_VSFLEXGRID_NODE, CAttrCrosswalkLineDlg>
{
public:
	CAttrCrosswalkLineDlg(void);
	~CAttrCrosswalkLineDlg(void);

BEGIN_MSG_MAP(CAttrCrosswalkLineDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)

	COMMAND_ID_HANDLER(ID_MENUITEM_ADD, OnClickAddMenu)
	COMMAND_ID_HANDLER(ID_MENUITEM_DELETE, OnClickDeleteMenu)

	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)


ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrCrosswalkLineDlg)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_MOUSEUP, OnMouseUpGrid)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::AfterEdit,  OnAfterEdit)
	SINK_ENTRY(IDC_VSFLEXGRID_NODE, 0x01,           OnCellChangedGrid)
END_SINK_MAP()


public:

	// ���b�Z�[�W�n���h��
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	// ���j���[�́u�ǉ��v�u�폜�v�{�^���N���b�N
	LRESULT OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	// �ĕ`�施�߂��󂯎�����Ƃ��̏���
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual bool Update();
	virtual void SetArcHelper( IApplication* ipApp ){ CAttrBaseDlg::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ CAttrBaseDlg::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ CAttrBaseDlg::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ CAttrBaseDlg::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){
		return ::SendMessage( m_hWnd, message, wParam, lParam );
	}; 

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }
	BOOL Changed();

private:
	void CreateControlRelation();

	/**
	 *  @brief �M���@�|�C���g���̕ύX��ԂƁA�{�^���� Enable/Disable �؂�ւ�
	 */
	void UpdateChanged();

	/**
	 *  @brief ���f�������C���ƕR�Â��m�[�h�Ƃ����ԃ��C���̕`��
	 */
	void DrawLinkageLine();

	/**
	 *  @brief �|�����C���̕`��
	 *  @param[in] polyline  �`�悷��|�����C��
	 *  @param[in] color     �`��F
	 */
	void DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color);

	// VS-FlexGrid�C�x���g�n���h��
	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/);


private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	long m_CurrentOid;			//!< ���݃_�C�A���O�ɕ\�����Ă��鉡�f������OID
	std::unordered_map<long, RelNodeInfos> m_RelNodes;	//!< �֘A�e�[�u����� 

	RelNodeGridController m_Grid;	//!< �֘A�m�[�h�O���b�h�̐���

};
