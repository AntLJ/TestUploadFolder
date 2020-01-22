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

// AttrFacilInfoPointDlg.h : CAttrFacilInfoPointDlg �̐錾

#ifndef _ATTRFACILINFOPOINTDLG_
#define _ATTRFACILINFOPOINTDLG_

#include "resource.h"
#include "../../HwyEditTool/HwyHelper.h"
#include "../../HwyEditTool/HwyObjects.h"
#include "../../LIB/WinLib/str2.h"

typedef CHetCVDMap CCodeValueDomain;	//!< �R�[�h�h���C�����i�[����
typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrFacilInfoPointDlg
class CAttrFacilInfoPointDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
	CAttrFacilInfoPointDlg();
	~CAttrFacilInfoPointDlg()
	{
		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrFacilInfoPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_COMBO_ROADCLASS_REF, CBN_SELCHANGE, OnSelchangeRsel_class)
	COMMAND_HANDLER(IDC_COMBO_ROADNAME_REF, CBN_SELCHANGE, OnSelchangeRsel_Name)
	COMMAND_HANDLER(IDC_EDIT_FACILKANJI, EN_UPDATE, OnUpdateFacilKanji)
	COMMAND_HANDLER(IDC_EDIT_FACILKANA, EN_UPDATE, OnUpdateFacilYomi)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
ALT_MSG_MAP(IDC_COMBO_ROADCLASS_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROADNAME_REF)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FACILKANJI)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_FACILKANA)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMYFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_FACILCLASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SEARCHFACIL)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_AUTOPOSITION_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_DUMMY)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg ���b�Z�[�W�n���h��
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
		m_wnd0.SubclassWindow(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		m_wnd1.SubclassWindow(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		m_wnd2.SubclassWindow(GetDlgItem(IDC_EDIT_SEQUENCE));
		m_wnd3.SubclassWindow(GetDlgItem(IDC_EDIT_FACILKANJI));
		m_wnd4.SubclassWindow(GetDlgItem(IDC_EDIT_FACILKANA));
		m_wnd5.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMYFLAG));
		m_wnd6.SubclassWindow(GetDlgItem(IDC_COMBO_FACILCLASS));
		m_wnd7.SubclassWindow(GetDlgItem(IDC_CHECK_AUTOPOSITION_F));
		m_wnd9.SubclassWindow(GetDlgItem(IDC_CHECK_DUMMY));
		m_wnd8.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wnd10.SubclassWindow(GetDlgItem(IDC_COMBO_SEARCHFACIL));

		m_wnd3.SetFont( m_hFont );
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
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

	/**
	 * IDC_EDIT_FACILKANJI �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnUpdateFacilKanji(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				// ���p�������S�p�����ϊ��A�g�p�֎~�������̃`�F�b�N
				CString strHoumenKanji = AheConvertToFacilInfoName( lpszText, TRUE );
				// �A���t�@�x�b�g���������啶���ϊ�
				int nTextLen = lstrlen(strHoumenKanji) + 1;
				LPTSTR lpszChecked = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
				LCMapString( GetUserDefaultLCID(), LCMAP_UPPERCASE, strHoumenKanji, -1, lpszChecked, nTextLen );
				strHoumenKanji = lpszChecked;
				
				// �`�F�b�N�̌��ʏC�����s��ꂽ�ꍇ�A�\���ɔ��f����
				if( strHoumenKanji.Compare( lpszText ) != 0 )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strHoumenKanji );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenKanji);
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * IDC_EDIT_FACILKANA �p�C�x���g�n���h��
	 *
	 * �����ŁA�������͂̃`�F�b�N�������Ȃ��A�s�K�؂ȕ��������͂��ꂽ�ꍇ�ɂ�
	 * ���͂ł��Ȃ��悤�ɂ��܂��B
	 */
	LRESULT OnUpdateFacilYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strHoumenYomi = AheConvertToFacilInfoName( lpszText, FALSE );
				if( strHoumenYomi.Compare( lpszText ) != 0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strHoumenYomi );
					// ���͈ʒu�̏C��
					INT nDiff = CString_GetLength2(CString( lpszText )) - CString_GetLength2(strHoumenYomi);
					edit.SetSel( nSelStart - nDiff, nSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}

		return 0;
	}

	/**
	 * �H����ʎQ�Ɨp���ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H�����̎Q�Ɨp�ƁA�H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_class(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled)
	{
		// �I�����ꂽ�H����ʂ̎擾
		CComboBox cbLineClass = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADCLASS_REF));
		int nIdx = cbLineClass.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nLineClass = cbLineClass.GetItemData(nIdx);
		// �H���R�[�h�̍ĕ`��
		if(! RedrawRoadNameList(0))
			return 0;
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

	/**
	 * �H�����̂��ύX���ꂽ�Ƃ��̃C�x���g�n���h��
	 *
	 * �H���R�[�h�̒l��ύX���܂�
	 */
	LRESULT OnSelchangeRsel_Name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// �I�����ꂽ�H���R�[�h�̎擾
		CComboBox cbRoadName = CComboBoxT<CWindow>(GetDlgItem(IDC_COMBO_ROADNAME_REF));
		int nIdx = cbRoadName.GetCurSel();
		if(nIdx == CB_ERR)
			return 0;
		m_nRoadCode = cbRoadName.GetItemData(nIdx);
		// �H���R�[�h�̐ݒ�
		if(! SetRoadCode(m_nRoadCode))
			return 0;

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp );
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

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();
	bool SetLineClassList(LONG nLineClass);
	bool GetLineClass(int* pClass);
	bool SetLineClass(int nClass);
	bool RedrawRoadNameList(LONG nRoadCode);
	bool GetRoadCode(int* pRoadCode);
	bool SetRoadCode(int nRoadCode);
	
	ITablePtr GetRoadCodeTable();
	bool GetRoadCodeSet(CHetRoadCodeSet* pCodeSet);

////////////////////////////////////////////////////////////////////////////
//
// CAttrFacilInfoPointDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CContainedWindow	m_wnd0,m_wnd1,m_wnd2,m_wnd3,m_wnd4,
						m_wnd5,m_wnd6,m_wnd7,m_wnd8,m_wnd9,
						m_wnd10;

	CString		m_cRoadCode;				//!< ������ł̘H���R�[�h
	HFONT		m_hFont;

	// �n�C�E�F�C�w���p�[�N���X
	CHwyHelper  m_cHwyHelper;				//!< �n�C�E�F�C�w���p�[�N���X

	// ���̒l
	LONG m_lOrgRoadSequence;				//!< �ŏ��̘H�����V�[�P���X
	LONG m_lOrgRoadCode;					//!< �ŏ��̘H���R�[�h

	// �_�C�A���O�̒l�ۑ��p�f�[�^
	LONG m_nLineClass;						//!< ���ݑI�𒆂̘H����ʃR�[�h
	LONG m_nRoadCode;						//!< ���ݑI�𒆂̘H���R�[�h
	LONG m_nRoadSequence;					//!< ���ݑI�𒆂̘H�����V�[�P���X

	// �n�C�E�F�C�ҏW�p�e��f�[�^�̃R���e�i
	CHetRoadCodeSet m_cRoadCodeSet;			//!< �H���R�[�h���R���e�i
	HWND				m_hChildWnd;		//!< �q�_�C�A���O�n���h��
};

#endif // _ATTRFACILINFOPOINTDLG_

