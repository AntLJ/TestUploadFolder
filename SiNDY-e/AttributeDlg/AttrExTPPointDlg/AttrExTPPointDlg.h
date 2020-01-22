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


#ifndef _ATTREXTPPOINTDLG_
#define _ATTREXTPPOINTDLG_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrExTPPointDlg
class CAttrExTPPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrExTPPointDlg();
	~CAttrExTPPointDlg()
	{
		::DeleteObject( m_hFont );
	}
/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrExTPPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_EDIT_ADDRCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDRNAME)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KATAGAKI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_KEISAI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_POINTSTAT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ADDRGP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_INFOSRC)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR3)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ADDR4)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[���쐬
		CreateControlRelation();

		// �T�u�N���X��
		m_wnd0.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRCODE));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_EDIT_ADDRNAME));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_KATAGAKI));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_KEISAI));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_COMBO_POINTSTAT));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CHECK_ADDRGP));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_EDIT_INFOSRC));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR1));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR2));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR3));
		m_wnd11.SubclassWindow(GetDlgItem(IDC_EDIT_ADDR4));

		m_wnd1.SetFont( m_hFont );
		m_wnd2.SetFont( m_hFont );
		m_wnd3.SetFont( m_hFont );
		m_wnd4.SetFont( m_hFont );
		m_wnd7.SetFont( m_hFont );
		m_wnd8.SetFont( m_hFont );
		m_wnd9.SetFont( m_hFont );
		m_wnd10.SetFont( m_hFont );
		m_wnd11.SetFont( m_hFont );

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

		// 20���R�[�h�𕪉������G�f�B�b�g�{�b�N�X�̔w�i�F��ύX
		HWND hWndAddr1,hWndAddr2,hWndAddr3,hWndAddr4;
		hWndAddr1 = CWindow( GetDlgItem( IDC_EDIT_ADDR1 ) );
		hWndAddr2 = CWindow( GetDlgItem( IDC_EDIT_ADDR2 ) );
		hWndAddr3 = CWindow( GetDlgItem( IDC_EDIT_ADDR3 ) );
		hWndAddr4 = CWindow( GetDlgItem( IDC_EDIT_ADDR4 ) );

		if( hWndAddr1 == hWnd || hWndAddr2 == hWnd || hWndAddr3 == hWnd || hWndAddr4 == hWnd )
		{
			if ( !m_hBrush2 )
				m_hBrush2 = CreateSolidBrush( RGB(204,236,255) );

			// Get the DC provided and set the appropriate attributes

			SetBkMode(hdc, TRANSPARENT );
			SetBkColor(hdc, RGB(150,240,255));

			// Return a handle to the brush
			HBRUSH far* hbr = (HBRUSH far*) m_hBrush2;
			return ((DWORD) hbr);
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
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// �R���{�{�b�N�X�̃��X�g�{�b�N�X�̒����𒲐�
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// �R���{�{�b�N�X�A�G�f�B�b�g�{�b�N�X�A�`�F�b�N�{�b�N�X�̃��b�Z�[�W����
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// �_�C�A���O���X�V�̈�ɒǉ����܂�
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg �����o�֐���`
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
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strFeatureClassName = lpcszFeatureClassName;};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	void DevideAddrCode();
	void SetToolTip(INT nTargetItem);

////////////////////////////////////////////////////////////////////////////
//
// CAttrExTPPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,m_wnd5,m_wnd6,
						m_wnd7,m_wnd8,m_wnd9,m_wnd10,m_wnd11;//!< �e�R���g���[���E�B���h�E
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
	HFONT				m_hFont;					//!< �����t�H���g
	HBRUSH				m_hBrush2;					//!< �R���g���[���̐F�ύX�p�u���V
	CString				m_strFeatureClassName;
};

#endif // _ATTREXTPPOINTDLG_
