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

#include "WinLib/Str2.h"	
#include "TableDataGrid.h"

// �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));


typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrUpdownNodeDlg
class CAttrUpdownNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_NQ, CAttrUpdownNodeDlg>
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownNodeDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrUpdownNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

// [SiNDYChildDlg] ���b�Z�[�W�}�b�v
ALT_MSG_MAP(IDC_CHECK_STOP_POINT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_YOMI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

	// ���b�Z�[�W�n���h��
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);


	BEGIN_SINK_MAP(CAttrUpdownNodeDlg)
		SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::BeforeMouseDown, OnGridMouseDown)
		SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
		SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::AfterEdit, OnAfterEdit)
	END_SINK_MAP()

	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);
	void __stdcall OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownNodeDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual bool Update();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrUpdownNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }
	BOOL Changed();

private:
	void CreateControlRelation();
	void SetToolTip(INT nTargetItem);

	/**
	* �O���b�h�𐶐�����
	*/
	void createGrid();

	/**
	*  @brief  �{�^����Enable/Disable�̍X�V�A�֘A�e�[�u���̍X�V���̍X�V
	*  @note   �O���b�h�ŊǗ����Ă���֘A�e�[�u���̏��́AControlRel�ōX�V�ł��Ȃ����߁A
	*          �ύX���̃t���O�����X�V����B�t���O���X�V���Ȃ���DB���X�V����Ȃ�
	*/
	void updateChanged();

	/**
	*  @brief  �O���b�h�őI�����Ă���m�[�h��̃��C�����n�C���C�g����
	*/
	void highlightLine();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownNodeDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E
	HFONT							m_hFont;		//!< �����t�H���g

	// �m�[�h��O���b�h�R���g���[�� (key: �Ԑ��m�[�h��OID, value: �m�[�h��O���b�h)
	std::map<long, TableDataGrid> m_nqDataGridMap;
	long m_currentOid;		//!< ���݃_�C�A���O�ɕ\�����Ă���Ԑ��m�[�h��OID
};
