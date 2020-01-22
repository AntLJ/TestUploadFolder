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

// AttrHimaLogDlg.h : CAttrHimaLogDlg �̐錾

#ifndef __ATTRHIMALOGDLG_H_
#define __ATTRHIMALOGDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrHimaLogDlg
class CAttrHimaLogDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrHimaLogDlg();
	~CAttrHimaLogDlg()
	{
		::DeleteObject( m_hFont1 );
		::DeleteObject( m_hFont2 );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrHimaLogDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_LOG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_WORKER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SHEETID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnClickSheetID)
ALT_MSG_MAP(IDC_EDIT_KIGOU)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENTYOU_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_GENTYOU_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KIZON_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KIZON_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DISP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_DISP_NAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TDCSTATUS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RESULT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_MATCHING)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_RESULT_SHA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_REMARKS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();
		// �T�u�N���X��
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_LOG));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_WORKER));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_SHEETID));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KIGOU));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_GENTYOU_C));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_EDIT_GENTYOU_NAME));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_KIZON_C));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_KIZON_NAME));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_DISP_C));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_DISP_NAME));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_TDCSTATUS));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_EDIT_RESULT));
		m_wnd12.SubclassWindow(GetDlgItem(IDC_EDIT_MATCHING));
		m_wnd13.SubclassWindow(GetDlgItem(IDC_EDIT_RESULT_SHA));
		m_wnd14.SubclassWindow(GetDlgItem(IDC_EDIT_REMARKS));

		m_wnd2.SetFont( m_hFont1 );
		m_wnd5.SetFont( m_hFont2 );
		m_wnd7.SetFont( m_hFont2 );
		m_wnd9.SetFont( m_hFont2 );

		CRegKey cRegKey;
		cRegKey.Open(HKEY_CURRENT_USER, _T("Software\\INCREMENT P CORPORATION\\SiNDY-e\\Tools\\LogHandleTool\\Himawari"));
		
		TCHAR buffer[4096];
		DWORD dwSize = sizeof(buffer) / sizeof(TCHAR);
		if(cRegKey.QueryValue(buffer, _T("RasterPath"), &dwSize) == ERROR_SUCCESS) {
			m_strRasterPath = buffer;
		}

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * �R�����R���g���[���̐F�ύX�p�C�x���g�n���h��
	 *
	 * �t�B�[���h�l�̕ύX���������ꍇ�A�����ŃR�����R���g���[���̐F���ς��܂�
	 */
	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		HDC hdc = (HDC) wParam;    // handle to display context 
		HWND hWnd = (HWND) lParam; // handle to static control 

		CControlDef* def = m_cControlRel.GetControlDef( hWnd );

		if( hWnd == GetDlgItem( IDC_EDIT_SHEETID ) )
		{
			SetTextColor( hdc, RGB(0,0,255) );
			if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
			{
				if( !def->Changed() )
					SetBkMode(hdc, TRANSPARENT );
				else
					SetBkMode(hdc, RGB(255,168,126) );
			}
			else
				SetBkMode(hdc, TRANSPARENT );

			return (long)1;
			return (long)GetSysColorBrush( COLOR_WINDOW );
		}

		return ATTR_BASE_CLASS::OnCtlColorEdit( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * WM_COMMAND �p�C�x���g�n���h��
	 *
	 * �e�R�����R���g���[���ŕύX���������ꍇ�A�����ŏ������܂�
	 * ���݂̓G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�A�R���{�{�b�N�X�̏������s���܂�
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() �̍Œ��̓L�����Z���i�^�C�~���O�ɂ���Ă͂��������Ȃ邽�߁j
		if( !m_bFinishInit ) return 0;


		INT msg = HIWORD(wParam);						// ���상�b�Z�[�W
		INT	nTargetControl = (INT)LOWORD(wParam);		// ����ΏۃR���g���[��

		// �G�f�B�b�g�{�b�N�X�̎��ɑS�I���ɂ���
		if( msg == EN_SETFOCUS && nTargetControl != IDC_EDIT_REMARKS)
			SelectDlgItem( nTargetControl );

		// �R�����g�̃G�f�B�b�g�{�b�N�X�͑S�I���ɂ��Ȃ�
		if( msg == EN_SETFOCUS && nTargetControl == IDC_EDIT_REMARKS )
		{
			SetIMEMode( TRUE );
			CEdit	cString1(GetDlgItem(IDC_EDIT_REMARKS));
			cString1.SetSel(0,0,TRUE);

			LPTSTR lpszRoadCode = (LPTSTR)_alloca((cString1.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< ���������������m��
			cString1.GetWindowText( lpszRoadCode, cString1.GetWindowTextLength() + 1 );

			if(lstrcmpi(lpszRoadCode, _T("<NULL>")) == 0)
				cString1.SetWindowText(_T(""));
		}

		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			if( nTargetControl == IDC_EDIT_REMARKS )
				SetIMEMode( FALSE );
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * TAB�L�[�ł̃R���g���[���ړ��A�y�і��L�[�̏㉺�ŃR���{�{�b�N�X�̃A�C�e���̕ύX�̃G�~�����[�g
	 */
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CWindow wnd = GetFocus();
		INT iWndID = wnd.GetDlgCtrlID();

		if( wParam == VK_TAB )
		{
			if( iWndID == IDC_EDIT_REMARKS )
				return 1;

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

			bHandled = TRUE;
			return 1;
		}
		else if( wParam == VK_RETURN && iWndID != IDC_EDIT_REMARKS)
		{
			// �ҏW�J�n��Ԃ��擾
			esriEditState state = esriStateNotEditing;
			IEditorPtr ipEditor( m_cArcHelper.GetEditor() );
			ipEditor->get_EditState( &state );

			if( state != esriStateNotEditing )
			{
				// ���^�[���L�[�������ꂽItem���X�V����
				CWindow wnd = GetFocus();
				INT iWndID = wnd.GetDlgCtrlID();
				m_cControlRel.ControlChanged(iWndID);

				if( !CheckReturnKeyItem(iWndID) ) 		// �G���^�[�L�[�������ꂽItem�̃`�F�b�N
					return 1;				// �G���[�E��������̏ꍇ�A�ȍ~�̏����𔲂���

				InvalidateRect( NULL, FALSE );

				::PostMessage( GetParent(), WM_CLOSE, 0, -1);
				bHandled = TRUE;
				return 1;
			}
		}
		else if( wParam == VK_ESCAPE )
		{
			// ���^�[���L�[�������ꂽItem���X�V����
			CWindow wnd = GetFocus();
			INT iWndID = wnd.GetDlgCtrlID();
			if( !CheckEscKeyItem(iWndID) )
				return 1;

			::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
			bHandled = TRUE;
			return 1;			
		}

		bHandled = FALSE;	// �����I�ɐݒ肷��K�v�A��
		return 0;
	}

	/**
	 * IDC_EDIT_CLAIMNO �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnClickSheetID(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CEdit cEdit( GetDlgItem( IDC_EDIT_SHEETID ));

		INT nTextLen = cEdit.GetWindowTextLength() + 1;
		if( nTextLen > 0 && !m_strRasterPath.IsEmpty() )
		{
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			cEdit.GetWindowText( lpszText, nTextLen );
			CString strFullNumber = lpszText;

			LPCTSTR szSuffixs[2] = { _T("tif"), _T("TIF") };

			for(int i = 0; i < 2; i++) {
				CString strFilePath;
				strFilePath.Format(_T("%s\\%s\\%s.%s"), m_strRasterPath, strFullNumber.Left(5), strFullNumber, szSuffixs[i]);
				if(reinterpret_cast<int>(::ShellExecute(NULL, NULL, strFilePath, NULL, NULL, SW_SHOWNORMAL)) > 32)
					break;
			}
		}
		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE;};
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrHimaLogDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10,m_wnd11,m_wnd12,m_wnd13,m_wnd14;	//!< �e�R���g���[���E�B���h�E
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
	CString				m_strRasterPath;
	HFONT				m_hFont1,m_hFont2;
};

#endif //__ATTRHIMALOGDLG_H_
