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

// AttrDRMALinkDlg.h : CAttrDRMALinkDlg �̐錾

#ifndef _ATTRDRMALINKDLG_H_
#define _ATTRDRMALINKDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrDRMALinkDlg
class CAttrDRMALinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
private:
	/**
	 * �ҏW�r���[�̃��[�h
	 *
	 * ��ɓ��H������ҏW����ۂɂǂ̃R���g���[����\��������悢���̎w�j�Ƃ��Ďg�p���܂��B
	 * CRowsControlRelation::SetControl() �̑�R�A��S�����Ƃ��Ďg�p���܂��B
	 * @see CreateControlRelation()
	 */
	enum EditView {
		EDITVIEW_DRMA_KIHON,		/**< ���H�̊�{�����i��P�^�u�j */
		EDITVIEW_DRMA_TUIKA,		/**< ���H�̒ǉ������i��Q�^�u�j */
		EDITVIEW_DRMA_KOTUKISEI,	/**< ���H�̌�ʋK�������i��R�^�u�j */
		EDITVIEW_DRMA_ROSEN,		/**< ���H�̘H�������i��S�^�u�j */
		EDITVIEW_DRMA_DRMA,			/**< ���H��DRMA�����i��T�^�u�j */
	};
/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
public:
BEGIN_MSG_MAP(CAttrDRMADlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * �_�C�A���O�쐬����ۂɈ�ԍŏ��ɌĂ΂�܂��B�����Ń_�C�A���O�̏����������Ă�������
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// �R���g���[��
		CreateControlRelation();

		CString strTemp;  //!< pszText�ɓn����������ꎞ�I�Ɋi�[����ϐ�
		
		// ���H�����ҏW�p�^�u�R���g���[���ɃA�C�e����ǉ����܂�
		HWND hwndTab = GetDlgItem( IDC_TAB_DRMAATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_EXTENTION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_TUIKA, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_REGULATION);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_KOTUKISEI, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NUMBER);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_ROSEN, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_DRMA);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_DRMA_DRMA, (LPARAM)(&pTabCtrlItem));
		
		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);
		ChangeWindowMgrMap( m_iChangeType );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
	}
	/**
	 * WM_NOTIFY �p�C�x���g�n���h��
	 *
	 * �ȉ��̏������s���܂�
	 * �E�^�u�̐؂�ւ�������������
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_DRMAATTR ) {
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// �R���g���[���؂�ւ�
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lDRMATabNum = m_iChangeType;	// ���H�^�u�̏ꍇ�̂݃^�u��ID��ێ�

				bHandled = TRUE;
				return 1;
			}
		}
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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			INT nID = (INT)LOWORD(wParam);
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}

		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg �����o�֐���`
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
	virtual BOOL ErrorCheck(){return TRUE;};
	virtual LONG GetTabNum();
	virtual void SetTabNum(LONG lTabNum);
	virtual void SetFeatureClassName( LPCTSTR lpcszFeatureClassName ){ m_strFeatureClassName = (CString)lpcszFeatureClassName;};

	CAttrDRMALinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void ChangeWindowMgrMap( INT iIndex );
	void CreateControlRelation();
	HRESULT CreateActiveXControls(UINT nID);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrDRMALinkDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	LONG				m_lDRMATabNum;				//!< ���H�����^�u�̕\���ʒu�ۑ��p
	INT					m_iChangeType;				//!< �R���g���[���؂�ւ��p

private:
	HWND				m_hChildWnd;				//!< �q�_�C�A���O�n���h��
	CString				m_strFeatureClassName;		//!< �t�B�[�`���N���X��
};

#endif // _ATTRDRMALINKDLG_H_
