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

// AttrClaimPolyMapDlg.h : CAttrClaimPolyMapDlg �̐錾

#ifndef __ATTRCLAIMPOLYMAPDLG_H_
#define __ATTRCLAIMPOLYMADPLG_H_

#include "resource.h"
#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrClaimPolyMapDlg
class CAttrClaimPolyMapDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrClaimPolyMapDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_CLAIMNO, EN_UPDATE, OnUpdateClaimNo)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_CLAIMTYPE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_CLAIMNO)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_COMFIRM)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_USELESS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_REMARKS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg ���b�Z�[�W�n���h��
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_CLAIMTYPE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_CLAIMNO));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_CHECK_COMFIRM));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_CHECK_USELESS));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_REMARKS));

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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
		if( msg == EN_SETFOCUS && nTargetControl == IDC_EDIT_REMARKS)
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

		// ����������t�H�[�J�X�O�ꂽ��IME��؂�
		if( nTargetControl == IDC_EDIT_REMARKS && msg == EN_KILLFOCUS)
			SetIMEMode( FALSE );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
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

				if (CheckReturnKeyItem(iWndID) == FALSE) 		// �G���^�[�L�[�������ꂽItem�̃`�F�b�N
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
			if( CheckEscKeyItem(iWndID) == FALSE)
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
	LRESULT OnUpdateClaimNo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				LPCTSTR lpcszText = lpszText;
				INT nTextLen = lstrlen( lpcszText ) + 1;
				LPTSTR lpszText2;

				lpszText2 = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );	// �����̈���m��
				// �S�Ĕ��p�ɂ��Ă��܂�
				LCMapString( GetUserDefaultLCID(), LCMAP_HALFWIDTH, lpcszText, nTextLen, lpszText2, nTextLen );

				CString strText( lpszText2 );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );

				USES_CONVERSION;

				INT  nOcnt   = 0;		// �o�͕�����
				BOOL bMinus  = FALSE;
				BOOL bPeriod = FALSE;
				for( LONG i = 0; i < lLen; i++ )
				{
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

					// ���p�̎��A����������
					// ����
					if ( isasciinumber( lpstr[0] ) )
					{
						;
					}
					// �啶���p��
					else if (isasciialphabet_large(lpstr[0])) {
						;
					}
					// �������p��
					else if (isasciialphabet_small(lpstr[0])) {
						;
					}
					// �J���}
					else if ( 0x2c == (unsigned char)((lpstr[0])&0xff) ){
						;
					}
					// �}�C�i�X
					else if ( isasciiminus(lpstr[0]) ){
						;
					}
					else
						continue;

					strRet += A2CT( lpstr );
					nOcnt++;
				}				
							
				if( strRet.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strRet );
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}


/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg �����o�֐���`
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
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrClaimPolyMapDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrClaimPolyMapDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4;	//!< �e�R���g���[���E�B���h�E
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
};

#endif //__ATTRCLAIMPOLYMAPDLG_H_
