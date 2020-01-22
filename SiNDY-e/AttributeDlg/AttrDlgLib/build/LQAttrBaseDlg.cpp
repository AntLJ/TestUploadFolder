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

// LQAttrBaseDlg.cpp : CLQAttrBaseDlg �̃C���v�������e�[�V����
#include "StdAfx.h"
#include "../LQAttrBaseDlg.h"
#include <imm.h>

#include "ArcHelper.h"
/////////////////////////////////////////////////////////////////////////////
// CLQAttrBaseDlg
CLQAttrBaseDlg::CLQAttrBaseDlg( WINRECT* pWinMap, WORD resourceID )
	: m_winMgr(pWinMap), IDD(resourceID)
{
	m_bFinishInit = TRUE;
	m_pFeatureDefList = NULL;

	m_hChangedBrush = CreateSolidBrush( RGB(255,168,126) );
	m_hErrorBrush = CreateSolidBrush( RGB(255,0,0) );

	//�@�R���{�{�b�N�X��Disable�̎��̔w�i�F�̐ݒ�����W�X�g������擾
	m_lComboR = 214;
	m_lComboG = 211;
	m_lComboB = 206;
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		DWORD dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboR"), &dwResult, hOpenKey ) )
			m_lComboR = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboG"), &dwResult, hOpenKey ) )
			m_lComboG = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboB"), &dwResult, hOpenKey ) )
			m_lComboB = (long)dwResult;
		RegCloseKey( hOpenKey );
	}
	m_hBrushCombo = CreateSolidBrush(RGB(m_lComboR,m_lComboG,m_lComboB));
}


CLQAttrBaseDlg::~CLQAttrBaseDlg()
{
	if( m_hChangedBrush != NULL )
		DeleteObject( m_hChangedBrush );
	if( m_hErrorBrush != NULL )
		DeleteObject( m_hErrorBrush );
	if( m_hBrushCombo != NULL )
		DeleteObject( m_hBrushCombo );
}

LRESULT CLQAttrBaseDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//�@�R���{�{�b�N�X��Disable�̎��̔w�i�F�̐ݒ�����W�X�g������擾
	m_lComboR = 214;
	m_lComboG = 211;
	m_lComboB = 206;
	CString strRegOpenKey;
	strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
	HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
	if ( hOpenKey )
	{
		DWORD dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboR"), &dwResult, hOpenKey ) )
			m_lComboR = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboG"), &dwResult, hOpenKey ) )
			m_lComboG = (long)dwResult;
		// �R���{�{�b�N�X�̔w�i�F��R�l
		if( EmxGetRegDword( _T("ComboB"), &dwResult, hOpenKey ) )
			m_lComboB = (long)dwResult;
		RegCloseKey( hOpenKey );
	}
	// �E�B���h�E�}�l�[�W���ɂ��Ĕz�u
	m_winMgr.InitToFitSizeFromCurrent(this);
	m_winMgr.CalcLayout(this);
	m_winMgr.SetWindowPositions(this);

	// �������I��
	m_bFinishInit = TRUE;

	return 0;  // �V�X�e���Ƀt�H�[�J�X��ݒ肳���܂�
}

LRESULT CLQAttrBaseDlg::OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ATLASSERT(lParam);
	NMWINMGR& nmw = *(NMWINMGR*)lParam;

	bHandled = FALSE;
	return 0;
}

LRESULT CLQAttrBaseDlg::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MINMAXINFO* pMinMaxInfo = reinterpret_cast<MINMAXINFO*>( lParam);

	m_winMgr.GetMinMaxInfo( this, pMinMaxInfo);
	bHandled = TRUE;

	return 0;
}
/**
* �_�C�A���O�̃T�C�Y�ύX���������Ƃ��̃C�x���g�n���h��
*/
LRESULT CLQAttrBaseDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	INT iWidth	= LOWORD(lParam);
	INT iHeight = HIWORD(lParam);

	m_winMgr.CalcLayout(iWidth, iHeight, this);
	m_winMgr.SetWindowPositions(this);

	bHandled = TRUE;

	return 0;
}
/**
* TAB�L�[�ł̃R���g���[���ړ��A�y�і��L�[�̏㉺�ŃR���{�{�b�N�X�̃A�C�e���̕ύX�̃G�~�����[�g
*/
LRESULT CLQAttrBaseDlg::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == VK_TAB )
	{
		UINT uiKeyState = GetAsyncKeyState(VK_SHIFT);
		if( (_rotl(uiKeyState, 1 ) & 0x01) == 0x01 )
			PrevDlgCtrl();
		else
			NextDlgCtrl();

		bHandled = TRUE;
		return 1;
	}
	else if( wParam == VK_RETURN )
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

			InvalidateRect( NULL, FALSE );

			::PostMessage( GetParent(), WM_CLOSE, 0, -1);
			bHandled = TRUE;
			return 1;
		}
	}
	else if( wParam == VK_ESCAPE )
	{
		::PostMessage( GetParent(), WM_CLOSE, 0, 0 );
		bHandled = TRUE;
		return 1;			
	}

	bHandled = FALSE;	// �����I�ɐݒ肷��K�v�A��
	return 0;
}

// ���[�_���̎��ɃL�[��߂܂���
LRESULT CLQAttrBaseDlg::OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}

BOOL CLQAttrBaseDlg::IsStartEditing()
{
	return m_cArcHelper.IsStartEditing();
}

BOOL CLQAttrBaseDlg::IsEditableFeature()
{
	sindyeEditableType enumEditableType;
	CComBSTR bstrMsg;
	BOOL bEditable = FALSE;

	CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
	if( pFeatureDef != NULL )
	{
		GetRule()->IsEditableFeature( pFeatureDef->GetFeature(), GetCurrentFeatureDef()->GetShape(), sindyeEditAttribute, &enumEditableType, &bstrMsg );
		switch( enumEditableType )
		{
			case sindyeEditableEditable: 
				bEditable = TRUE; 
				break;
			case sindyeEditableWarnning:
				if( pFeatureDef->GetWarnDelete() )
					bEditable = TRUE;
				break;
			default:
				break;
		}
	}
	
	return bEditable;	
}

void CLQAttrBaseDlg::SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList )
{
	m_pFeatureDefList = pFeatureDefList;
	m_cControlRel.SetFeatureDefList( pFeatureDefList );
}

BOOL CLQAttrBaseDlg::SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, INT view, BOOL bForce, BOOL bEditable )
{ 
	m_bFinishInit = FALSE;

	m_lFeatureDefIndex = lFeatureIndex;
	m_cControlRel.SetCurrentFeatureDefIndex( lFeatureIndex, lTableIndex, lRowIndex ); 
	m_cControlRel.Visible( view, bForce, bEditable );

	m_bFinishInit = TRUE;

	return TRUE;
}

/**
 * ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 *
 * @return ���ݕ\������Ă���t�B�[�`���̃��b�p�N���X�̃|�C���^��Ԃ�
 */
CLQRowDef* CLQAttrBaseDlg::GetCurrentFeatureDef()
{
	if( m_pFeatureDefList != NULL )
	{
		std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin();
		advance( it, m_lFeatureDefIndex );

		return &(*it);
	}
	else
		return NULL;
}
/**
 * �ύX�����邩�ǂ����̔���
 *
 * return �ύX������ꍇ�� TRUE�A�Ȃ���� FALSE
 */
BOOL CLQAttrBaseDlg::Changed()
{
	if( m_pFeatureDefList != NULL )
	{
		for( std::list<CLQRowDef>::iterator it = m_pFeatureDefList->begin(); it != m_pFeatureDefList->end(); it++ )
		{
			if( it->Changed() ) 
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CLQAttrBaseDlg::SetButton( BOOL bChanged )
{
	::SendMessage( GetParent(), WM_FEATURECHANGED, NULL, (LONG)bChanged );

	return TRUE;
}

BOOL CLQAttrBaseDlg::CheckReturnKeyItem(INT nClassID)
{
	return TRUE;
}

BOOL CLQAttrBaseDlg::CheckEscKeyItem(INT nClassID)
{
	return TRUE;
}

BOOL CLQAttrBaseDlg::ErrorCheck()
{
	return TRUE;
}

/**
 * �w��G�f�B�b�g�{�b�N�X��S�I����Ԃɂ���
 *
 * @param
 *
 * @return	TRUE : ����
 */
VOID CLQAttrBaseDlg::SelectDlgItem(INT nClassID)
{
	CEdit	cString1(GetDlgItem(nClassID));
	cString1.SetSel(0,-1,TRUE);

    LPTSTR lpszRoadCode = (LPTSTR)_alloca((cString1.GetWindowTextLength() + 1) * sizeof(TCHAR));        //!< ���������������m��
    cString1.GetWindowText( lpszRoadCode, cString1.GetWindowTextLength() + 1 );

	if(lstrcmpi(lpszRoadCode, _T("<NULL>")) == 0)
	{
		cString1.SetWindowText(_T(""));
	}
}

VOID CLQAttrBaseDlg::SetComboboxList(INT nClassID)
{
	CComboBox cCombo(GetDlgItem(nClassID));
	LONG lListNum = cCombo.GetCount();

	if( lListNum >= 20 )
	{
		RECT rect;
		INT i = cCombo.GetWindowRect(&rect);
		int nHeight = cCombo.GetItemHeight(0);
		int nWinWidth = rect.right - rect.left;
		int nWinHeight = rect.bottom - rect.top + nHeight * 21;
		cCombo.SetWindowPos(NULL, 0, 0, nWinWidth, nWinHeight, SWP_NOMOVE|SWP_NOZORDER|SWP_HIDEWINDOW);
		cCombo.ShowWindow(SW_SHOW);
	}
}

/**
 * IME�𐧌䂷��
 * 
 * @param bIsOn		[in]	IME����t���O True : On , False : OFF
 */
void CLQAttrBaseDlg::SetIMEMode(BOOLEAN bIsOn)
{
	IScreenDisplayPtr 			ipScreen = m_cArcHelper.GetScreenDisplay();
	HWND hWnd;
	ipScreen->get_hWnd( reinterpret_cast< OLE_HANDLE*>( &hWnd));

	HIMC hImc;
	hImc = ::ImmGetContext( hWnd);
	if( hImc)
	{
		::ImmSetOpenStatus( hImc, bIsOn);
		::ImmReleaseContext( hWnd, hImc);
	}
}

void CLQAttrBaseDlg::SetDlgStatus(INT nID, BOOL bIsEnable, BOOL bIsEditBox)
{
	if( !bIsEditBox )
	{
		// �R���{�{�b�N�X�ɂ̓��[�h�I�����[�������Ȃ����߁AENABLE��DISABLE���ŃZ�b�g����K�v������
		::EnableWindow( GetDlgItem( nID ), bIsEnable);
	}
	else
	{
		if( bIsEnable )
			::SendMessage(  GetDlgItem( nID ), EM_SETREADONLY, FALSE, 0L );
		else
			::SendMessage(  GetDlgItem( nID ), EM_SETREADONLY, TRUE, 0L );
	}
}

int CLQAttrBaseDlg::AttrMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel )
{
	int iRet = AheMessageBox( NULL, lpcszMsg, lpcszCap, uType|MB_TASKMODAL, uLevel );
	return iRet;
}
