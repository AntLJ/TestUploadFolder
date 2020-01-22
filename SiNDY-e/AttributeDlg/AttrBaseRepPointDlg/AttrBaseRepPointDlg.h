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

// AttrBaseRepPointDlg.h : CAttrBaseRepPointDlg �̐錾

#ifndef _ATTRBASEREPPOINTDLG_
#define _ATTRBASEREPPOINTDLG_

#include "resource.h"
#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrBaseRepPointDlg
class CAttrBaseRepPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrBaseRepPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_ADDRCODE, EN_UPDATE, OnUpdateAddrCode)
	COMMAND_HANDLER(IDC_EDIT_ADMINNAME, EN_UPDATE, OnUpdateAdminnName)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_ADDRCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADMINNAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ZIPCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg ���b�Z�[�W�n���h��
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRCODE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_ADMINNAME));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_ZIPCODE));

		if( ReadDBPathFromReg() )
			m_bDoAutoInput = TRUE;

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}

	/**
	 * �Z���R�[�h���̓G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 *
	 * �G�f�B�b�g�{�b�N�X�̕��������삳���ꍇ�A�����ŏ���
	 */
	LRESULT OnUpdateAddrCode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			// 11���R�[�h�p�e�L�X�g�o�b�t�@
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				INT nTextLen = lstrlen( lpszText ) + 1;
				CString strText( lpszText );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );

				USES_CONVERSION;

				for( LONG i = 0; i < lLen; i++ )
				{
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

					if( ( isasciinumber( lpstr[0] ) || ( 0x41 <= (unsigned char)((lpstr[0])&0xff) && (unsigned char)(lpstr[0]&0xff) <= 0x46 ) ) )
					{
						strRet += A2CT( lpstr );
					}
				}
				LONG lCheckLen = CString_GetLength2( strRet );
				if( lLen != lCheckLen )
				{

					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// ���͈ʒu�̏C��
					INT nDiff = CString( lpszText ).GetLength() - strRet.GetLength();
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}

				
				// 11���ڂ���͂����疼�̂������\��
				if( m_bDoAutoInput           // ���W�X�g���ɃL�[������
					&& lLen          == 11             // 11�������͂���Ă���
					&& edit.GetSel() != 0              // �S�I������Ă��Ȃ��i����͌������O���邽�߁j
				  )
				{
					CString strAddName;
					if( SearchNameFromFile( strRet, strAddName ) )
					{
						SetDlgItemText( IDC_EDIT_ADMINNAME, strAddName );
					}
					else
					{
						SetDlgItemText( IDC_EDIT_ADMINNAME, _T(" ") );
					}
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}
	

	/**
	 * ��\�_���̃G�f�B�b�g�{�b�N�X�p�C�x���g�n���h��
	 *
	 * �G�f�B�b�g�{�b�N�X�̕��������삳���ꍇ�A�����ŏ���
	 */
	LRESULT OnUpdateAdminnName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CString strAdminnName;
		CEdit edit( hWndCtl );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		edit.GetWindowText( strAdminnName.GetBuffer(nTextLen), nTextLen );

		// ���͂Ȃ��̏ꍇ���p�X�y�[�����Ă���
		if( strAdminnName == _T("") )
			SetDlgItemText( IDC_EDIT_ADMINNAME, _T(" ") );

		m_cControlRel.ControlChanged( wID );
		SetButton( Changed() );
		InvalidateRect( NULL, FALSE );
		return 0;
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
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg �����o�֐���`
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

	CAttrBaseRepPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	BOOL SearchNameFromFile( const CString strAddCode, CString &strAddName );
	BOOL ReadDBPathFromReg();
////////////////////////////////////////////////////////////////////////////
//
// CAttrBaseRepPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3;//!< �e�R���g���[���E�B���h�E
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
	CString             m_strTableFileName;         //!< �Ή��\�t�@�C���ւ̃t���p�X
	BOOL                m_bDoAutoInput;             //!< �������͂��邩�ǂ���
};

#endif // _ATTRBASEREPPOINTDLG_

