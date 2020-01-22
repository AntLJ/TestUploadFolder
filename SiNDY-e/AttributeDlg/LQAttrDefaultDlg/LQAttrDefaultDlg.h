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

// LQAttrDefaultDlg.h : CLQAttrDefaultDlg �̐錾

#ifndef __LQATTRDEFAULTDLG_H_
#define __LQATTRDEFAULTDLG_H_

#include "ExportDlg.h"
#include "LQAttrBaseDlg.h"
#include "resource.h"

typedef CLQAttrBaseDlg LQATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CLQAttrDefaultDlg
class CLQAttrDefaultDlg : 
	public LQATTR_BASE_CLASS,
	public IDispEventImpl<IDC_GRIDCTRL, CLQAttrDefaultDlg>, // �f�t�H���g�̃O���b�h�R���g���[��
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrDefaultDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
	CHAIN_MSG_MAP(LQATTR_BASE_CLASS)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDefaultDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();

		// �f�t�H���g�p�O���b�h�R���g���[��
		CAxWindow wndGrid = GetDlgItem(IDC_GRIDCTRL);
		wndGrid.QueryControl(&m_spGrid);
		wndGrid.SetFocus();		

		m_spGrid->put_AllowUserResizing( flexResizeColumns );
		m_spGrid->put_AllowSelection( TRUE );
		m_spGrid->put_AutoSizeMouse( TRUE );
		m_spGrid->put_Editable( flexEDKbdMouse );
		m_spGrid->put_ExtendLastCol( TRUE );
		
		// �O���b�h�R���g���[������̃C�x���g���E���̂ɕK�v
		AtlAdviseSinkMap(this, TRUE);

		LQATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

		// �O���b�h�G�f�B�b�g���[�h���ǂ������`�F�b�N
		// SiNDY�̃��W�X�g���L�[���擾
		CString strRegOpenKey;
		strRegOpenKey.Format( _T("%s%s%sAttributeTool"), SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );

		// ���W�X�g���I�[�v��
		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
		if ( hOpenKey )
		{
			// �O���b�h�����ҏW���[�h�t���O�̒l���擾
			DWORD dwResult = 0;
			if( EmxGetRegDword( _T("GridEditMode"), &dwResult, hOpenKey ) == TRUE )
				m_bIsGridEditMode = (BOOL)dwResult;

			RegCloseKey( hOpenKey );
		}

		return 1;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}

	/**
	 * �_�C�A���O���j�����ꂽ�Ƃ��ɌĂ΂�܂��B
	 */
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg �C�x���g�V���N�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CLQAttrDefaultDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRIDCTRL, 0x18, OnAfterEditDefaultGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x19, OnKeyPressEditGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x1c, OnChangeEditGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x6, OnBeforeRowColChangeGrid)
	SINK_ENTRY(IDC_GRIDCTRL, 0x8, OnBeforeSelChangeGrid)
	SINK_ENTRY(IDC_GRIDCTRL, DISPID_KEYDOWN, OnKeyDown)
	SINK_ENTRY(IDC_GRIDCTRL, 0x16, OnStartEditDefaultGrid)
END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid �C�x���g�n���h��
//
/////////////////////////////////////////////////////////////////////////////
public:
	/**
	 * �S�ẴO���b�h�̃Z���ȊO�ŃL�[���͂��������ꍇ�̃C�x���g�n���h��
	 * ���݂͖��g�p
	 */
	VOID __stdcall OnKeyDown(SHORT * KeyCode, SHORT Shift)
	{
		//[Bug 4822]�O���b�h���[�h��ESC��ENTER���g����悤�Ƀn���h�����O
		if( *KeyCode == VK_TAB )
		{
			UINT uiShiftState = GetAsyncKeyState(VK_SHIFT);
			UINT uiControlState = GetAsyncKeyState(VK_CONTROL);

			if( (_rotl(uiControlState, 1 ) & 0x01) == 0x01 )
			{
				if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
					PostMessage( WM_NOTIFY, 0, -1 );
				else
					PostMessage( WM_NOTIFY, 0, -2 );
			}
			else
			{
				if( (_rotl(uiShiftState, 1 ) & 0x01) == 0x01 )
					PrevDlgCtrl();
				else
					NextDlgCtrl();
			}
		}
		else if( *KeyCode == VK_RETURN )
		{
			// �ҏW�J�n��Ԃ��擾
			esriEditState state;
			IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
			ipEditor->get_EditState( &state );

			if( state != esriStateNotEditing )
			{
				// ���^�[���L�[�������ꂽItem���X�V����
				CWindow wnd = GetFocus();
				INT iWndID = wnd.GetDlgCtrlID();
				m_cControlRel.ControlChanged(iWndID);

				if (CheckReturnKeyItem(iWndID) == FALSE) {		// �G���^�[�L�[�������ꂽItem�̃`�F�b�N
					return;				// �G���[�E��������̏ꍇ�A�ȍ~�̏����𔲂���
				}

				InvalidateRect( NULL, FALSE );

				::PostMessage( GetParent(), WM_CLOSE, 0, -1);
			}
		}
		else if( *KeyCode == VK_ESCAPE )
		{
			// ���^�[���L�[�������ꂽItem���X�V����
			CWindow wnd = GetFocus();
			INT iWndID = wnd.GetDlgCtrlID();
			if( CheckEscKeyItem(iWndID) == FALSE){
				return;
			}
			::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
		}
		return;	
	}
	/**
	 * �S�ẴO���b�h�őI���Z���̕ύX���������ꍇ�̃C�x���g�n���h��
	 *
	 * �ҏW�J�n����Ă��Ȃ���΁A�C�x���g���L�����Z������܂�
	 */
	VOID __stdcall OnBeforeSelChangeGrid(LONG OldRowSel, LONG OldColSel, LONG NewRowSel, LONG NewColSel, VARIANT_BOOL * Cancel)
	{
		// �ҏW�J�n����Ă��Ȃ��A�܂��́A�O���b�h�����ҏW���[�h�łȂ��Ƃ��́A�O���b�h�͂�����Ȃ�
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE )
			*Cancel = VARIANT_TRUE;
	}
	/**
	 * �S�ẴO���b�h�̃Z���̑I�����ύX���ꂽ�Ƃ��ɔ�������C�x���g�n���h��
	 *
	 * �ҏW�J�n����Ă��Ȃ���΁A�C�x���g���L�����Z������܂�
	 */
	VOID __stdcall OnBeforeRowColChangeGrid(LONG OldRow, LONG OldCol, LONG NewRow, LONG NewCol, VARIANT_BOOL * Cancel)
	{
		// �ҏW�J�n����Ă��Ȃ��A�܂��́A�O���b�h�����ҏW���[�h�łȂ��Ƃ��́A�O���b�h�͂�����Ȃ�
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE )
			*Cancel = VARIANT_TRUE;
	}
	/**
	 * �S�ẴO���b�h�̃Z���̓��e���ύX���ꂽ�Ƃ��ɔ�������C�x���g�n���h��
	 * ���݂͖��g�p
	 */
	VOID __stdcall OnChangeEditGrid()
	{
	}
	/**
	 * �ҏW���[�h���ɃL�[�������ꂽ�Ƃ��ɔ�������C�x���g�n���h��
	 *
	 * ���͎��̐������̔�����s���܂�
	 * ���݂͖��g�p
	 */
	VOID __stdcall OnKeyPressEditGrid(LONG Row, LONG Col, SHORT * KeyAscii)
	{
	}
	/**
	 * �W���O���b�h�ŕҏW���I��������ɔ�������C�x���g�n���h��
	 */
	VOID __stdcall OnAfterEditDefaultGrid( LONG Row, LONG Col )
	{
		AfterEditGridSel( IDC_GRIDCTRL, m_spGrid, Row, Col );
	}
	/**
	 * �W���O���b�h�ŃZ�����N���b�N���ꂽ�Ƃ��ɔ�������C�x���g�n���h��
	 */
	VOID __stdcall OnStartEditDefaultGrid(LONG Row, LONG Col, VARIANT_BOOL * Cancel)
	{
		// �ҏW�J�n����Ă��Ȃ��A�܂��́A�O���b�h�����ҏW���[�h�łȂ��Ƃ��́A�O���b�h�͂�����Ȃ�
		// [Bug4821]�ҏW����Ă͂����Ȃ����́i���Ƃ���OID�Ȃǁj�܂ŕҏW�ł��Ă��܂��o�O
		// ���̂��߂ɁAArc�I�ɕҏW�ł��Ȃ����́{SHAPE�t�B�[���h�Ȃǂ�I������Ă��Ȃ����𒲂ׂ�
		if( IsStartEditing() == FALSE || m_bIsGridEditMode == FALSE ||
			m_cControlRel.IsClickCelEditable(IDC_GRIDCTRL, Row, Col) == FALSE )
		{
			*Cancel = VARIANT_TRUE;
			return;
		}
		StartEditGridSel( IDC_GRIDCTRL, m_spGrid, Row, Col );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ CLQAttrBaseDlg::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ CLQAttrBaseDlg::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){ CLQAttrBaseDlg::SetFeatureDefList( pFeatureDefList ); };
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){};
	virtual void ClearFeatureDefs(){ CLQAttrBaseDlg::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CLQAttrDefaultDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return CLQAttrBaseDlg::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return CLQAttrBaseDlg::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	BOOL FeatureIsValid( CString& strErrMsg ){ return TRUE; };
private:
	void CreateControlRelation();
	BOOL StartEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col );
	void AfterEditGridSel( INT nID, IVSFlexGridPtr spGrid, LONG Row, LONG Col );

	/** 
	* @fn		HRESULT CreateActiveXControls(UINT nID);
	*
	* @brief	FlexGrid�̃��C�Z���X�E�B���h�E���o���Ȃ��悤�ɂ��邽�߂̊֐�
	*
	* @note		CAxDialogImpl::CreateActiveXControls���I�[�o�[���C�h��ActiveXControl�쐬�̍ۂ�
	*			FlexGrid�̃L�[�𖳗����蓖�ĂĂ���
	*/
	HRESULT CreateActiveXControls(UINT nID);

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrDefaultDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	IVSFlexGridPtr		m_spGrid;			//!< �O���b�h�\���p
	ISiNDYRulePtr		m_ipRule;			//!< ��ƖړIDB�Ή�
	BOOL				m_bIsGridEditMode;	//!< �O���b�h�����ҏW���[�h�t���O
	HWND				m_hChildWnd;		//!< �q�_�C�A���O�n���h��
};

#endif //__LQATTRDEFAULTDLG_H_
