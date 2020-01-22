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

// AttrWalkLinkDlgDlg.h : CAttrWalkLinkDlgDlg �̐錾

#ifndef __ATTRWALKLINKDLG_H_
#define __ATTRWALKLINKDLG_H_

#include "resource.h"
#include "../../../LIB/WinLib/str2.h"
#include "../../../LIB/WinLib/charutil.h"
#include "BuddyRoadMgr.h"
#include "Option.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< �ĕ`�施�߂��󂯂邽�߂̃��b�Z�[�W


/////////////////////////////////////////////////////////////////////////////
// CAttrWalkLinkDlg
class CAttrWalkLinkDlg :
	public ATTR_BASE_CLASS,
	public CExportDlg2
{
public:
	CAttrWalkLinkDlg();
	~CAttrWalkLinkDlg()
	{
	}

	/**
	* �ҏW�r���[�̃��[�h
	*
	* CRowsControlRelation::SetControl() �̑�R�A��S�����Ƃ��Ďg�p���܂��B
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_WALK_KIHON,		//!< ���s�҃����N�̊�{�����i��1�^�u�j
		EDITVIEW_WALK_KISEI,		//!< ���s�҃����N�̋K�������i��2�^�u�j
	};
/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlgDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrWalkLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	COMMAND_HANDLER(IDC_EDIT_FLOORLEVEL, EN_UPDATE, OnUpdateFloorLevelEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCHROADCODE, BN_CLICKED, OnClickedSearchButton)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH, BN_CLICKED, OnClickedArroundSearch)
	COMMAND_HANDLER(IDC_BUTTON_ROAD_ACCEPT, BN_CLICKED, OnClickedAddBuddy)
	COMMAND_HANDLER(IDC_BUTTON_ROAD_REMOVE, BN_CLICKED, OnClickedRemoveBuddy)
	COMMAND_HANDLER( IDC_LIST_ROAD, LBN_SELCHANGE, OnListSelChange )
	COMMAND_HANDLER( IDC_LIST_ROADSEARCH, LBN_SELCHANGE, OnListSelChange )
	COMMAND_HANDLER( IDC_LIST_ROAD, LBN_DBLCLK, OnListDblClk )
	COMMAND_HANDLER( IDC_LIST_ROADSEARCH, LBN_DBLCLK, OnListDblClk )
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)

// �^�u�L�[���͂��T�u�N���X�������R���g���[���ɓn�����߂̃}�N��
#define ALT_MSG_MAP_(id) \
ALT_MSG_MAP(id)\
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	\
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

// ��{�^�u��
ALT_MSG_MAP_(IDC_COMBO_WALKCLASS)
ALT_MSG_MAP_(IDC_EDIT_ROADCODE)
ALT_MSG_MAP_(IDC_BUTTON_SEARCHROADCODE)
ALT_MSG_MAP_(IDC_CHECK_AUTOWALK)
ALT_MSG_MAP_(IDC_CHECK_STAIR)
ALT_MSG_MAP_(IDC_CHECK_SLOPE)
ALT_MSG_MAP_(IDC_CHECK_ESCALATOR)
ALT_MSG_MAP_(IDC_CHECK_ARCADE)
ALT_MSG_MAP_(IDC_CHECK_OPEN_AREA)
ALT_MSG_MAP_(IDC_CHECK_SCRAMBLE)
ALT_MSG_MAP_(IDC_EDIT_FLOORLEVEL)
ALT_MSG_MAP_(IDC_EDIT_PLATFORM)
ALT_MSG_MAP_(IDC_COMBO_FLOORMOVE)
ALT_MSG_MAP_(IDC_COMBO_MATERIAL)
ALT_MSG_MAP_(IDC_CHECK_TUNNEL)
ALT_MSG_MAP_(IDC_BUTTON_SEARCH)
ALT_MSG_MAP_(IDC_EDIT_SEARCHRANGE)
ALT_MSG_MAP_(IDC_LIST_ROADSEARCH)
ALT_MSG_MAP_(IDC_BUTTON_ROAD_REMOVE)
ALT_MSG_MAP_(IDC_BUTTON_ROAD_ACCEPT)
ALT_MSG_MAP_(IDC_LIST_ROAD)

// �K���^�u��
ALT_MSG_MAP_(IDC_COMBO_ONEWAY)
ALT_MSG_MAP_(IDC_COMBO_NOPASSAGE)

ALT_MSG_MAP_(IDC_EDIT_SOURCE)
ALT_MSG_MAP_(IDC_TAB_WALKLINK)

END_MSG_MAP()

#undef ALT_MSG_MAP_

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg ���b�Z�[�W�n���h��
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
		for( auto& subclass : m_mapSubClass)
		{
			subclass.second.SubclassWindow( GetDlgItem( subclass.first ) );
		}

		// �^�u�R���g���[���̍쐬
		CTabCtrl cTab( GetDlgItem( IDC_TAB_WALKLINK ) );
		cTab.AddItem( AheLoadString( IDS_BASE ) );
		cTab.AddItem( AheLoadString( IDS_REGULATION ) );

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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// �ύX���ꂽ���ǂ����`�F�b�N���A�ύX����Ă����瑼�̃R���g���[���ɂ����f������
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// [bug 5552] 2�����b�V�����E�����ׂ��ł�����s�҃����N�̏ꍇ�͔��Α��̃����N������������
			SyncChangedAttrToOverMeshLink( nTargetControl );
			InvalidateRect( NULL, FALSE );
			// �ύX���ꂽ�ꍇ�̓{�^���� Enable ��
			SetButton( Changed() );
		}
		return 0;
	}


	/**
	 * WM_NOTIFY �p�C�x���g�n���h��
	 *
	 * �ȉ��̏������s���܂�
	 * �E�^�u�̐؂�ւ�������������
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if( IDC_TAB_WALKLINK == wParam )
		{
			InvalidateRect( NULL, FALSE );//�ĕ`��
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( TCN_SELCHANGE == pnmh->code )
			{
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );	// �R���g���[���̐؂�ւ�

				m_cControlRel.Visible(m_iChangeType);
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;				// �^�u�ʒu�ۑ�

				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_lTabNum, m_bForce, m_bEditable );
							
				switch( m_lTabNum )
				{
				case EDITVIEW_WALK_KIHON:
					if( IsEditableFeature() && m_cArcHelper.IsStartEditing() )
						::SetFocus( GetDlgItem(IDC_COMBO_WALKCLASS) );
					break;
				case EDITVIEW_WALK_KISEI:
					SetDlgStatus(IDC_COMBO_ONEWAY, FALSE );
					SetDlgStatus(IDC_COMBO_NOPASSAGE, FALSE );
					break;
				default:
					break;
				}
				// �_�E���R���o�[�g�̂��߂̃R���g���[���\���E��\��
				SetControlState();

				bHandled = TRUE;
				return 1;
			}
		}

		if( 0 == wParam )
		{
			InvalidateRect( NULL, FALSE );//�ĕ`��
			HWND hwndTab = GetDlgItem( IDC_TAB_WALKLINK );
			if( -2 == lParam || -1 == lParam )
			{
				// �R���g���[���؂�ւ�
				if( -1 == lParam )
				{	
					if( 1 == m_iChangeType )	
						m_iChangeType = 0;
					else
						m_iChangeType = m_iChangeType + 1;
				}
				else
				{
					if( 0 == m_iChangeType )
						m_iChangeType = 1;
					else
						m_iChangeType = m_iChangeType - 1;
				}
				::SendMessage( hwndTab, TCM_SETCURSEL, m_iChangeType, 0 );

				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;	// �^�u�̈ʒu�ۑ�
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				// �_�E���R���o�[�g�̂��߂̃R���g���[���\���E��\��
				SetControlState();
				bHandled = TRUE;
				return 1;
			}
		}

		return 0;
	}


	// FLOOR_LEVEL��FLOAT�l����邽�߂̓��ʑΉ�
	// ���܂̂Ƃ���́A���̃_�C�A���O�ɂ����Ȃ����߁AAttrBaseDlg�ɂ͂��ꂸ�A�����ŗa�����Ă���
	// �����A���ɂ��A�����悤��FLOAT�l�����悤�Ȃ��̂��o�Ă����ꍇ�ɂ�AttrBaseDlg�ɓ����
	LRESULT OnUpdateFloorLevelEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [bug 5552] 2�����b�V�����E�����ׂ��ł�����s�҃����N�̏ꍇ�͔��Α��̃����N������������
		// FLOOR_LEVEL�̕ύX��OnCommand�ɂ��ʒm����SyncChengedAttrToOverMeshLink()�𑣂�����
		bHandled = FALSE;

		InvalidateRect( NULL, FALSE );
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> �̏ꍇ���l��
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
			{
				CString strText( lpszText );
				int iLen = CString_GetLength2( strText );
				CString strRet;

				USES_CONVERSION;

				bool bIsComma = false;
				bool bAfterDecimalPoint = false;

				for( int i = 0; i < iLen; i++ )
				{
					CString str = CString_GetAt2(strText, i);
					LPSTR lpstr = T2A( (LPTSTR)((LPCTSTR)str) );

					// �s����������������������쐬
					// 1�����ڂ̓}�C�i�X������
					if( i==0 )
					{
						if( ( isasciiminus( lpstr[0] ) || isasciinumber( lpstr[0] ) ) )
							strRet += A2CT( lpstr );
					}
					// 2�����ڈȍ~�͐����A�܂��̓R���}�̂݋���
					else
					{
						if( isasciinumber( lpstr[0] ) )
						{
							// �����_�ȉ���1�܂ł���������Ȃ��B
							if( !bAfterDecimalPoint )
								strRet += A2CT( lpstr );
							if( bIsComma )
								bAfterDecimalPoint = true;
						}
						else if( isasciiperiod( lpstr[0] ) )
						{
							// �R���}��1����������Ȃ�
							if( !bIsComma )
								strRet += A2CT( lpstr );
							bIsComma = true;
						}
					}
				}

				// �s���������܂܂�Ă����ꍇ
				int lCheckLen = CString_GetLength2( strRet );
				if( iLen != lCheckLen )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// ���͈ʒu�̏C��
					INT nDiff = CString( lpszText ).GetLength() - strRet.GetLength();
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
					return FALSE;
				}
			}
			m_cControlRel.ControlChanged( wID );
			SetButton( Changed() );
			InvalidateRect( NULL, FALSE );
		}
		return 0;
	}
	/**
	 * @brief walk_code�����v�{�^���������ꂽ���̏���
	 * @note ���̃C�x���g�Ɋւ��Ă�CControlDef���ł܂��Ȃ��鏈���ł͂Ȃ��̂ŁA�����ŏ���
	 */
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( wID == IDC_BUTTON_SEARCHROADCODE )
			SetRoadName(IDC_EDIT_ROADCODE, IDC_STATIC_ROADNAME2);
		return 0;
	}
	
	/**
	 * @brief �ĕ`�施�߂��󂯎�����Ƃ��̏���
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	/**
	 * @brief ���ӌ����{�^��
	 */
	LRESULT OnClickedArroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief �o�f�B�ǉ��{�^��
	 */
	LRESULT OnClickedAddBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief �o�f�B�폜�{�^��
	 */
	LRESULT OnClickedRemoveBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief ���X�g�{�b�N�X�̑I��ύX
	 */
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief ���X�g�A�C�e���_�u���N���b�N
	 */
	LRESULT OnListDblClk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief ���X�g�{�b�N�X�̐F�ύX
	 */
	LRESULT OnCtlColorListBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual LONG GetTabNum(){ return m_lTabNum; };
	virtual void SetTabNum(LONG lTabNum){ m_lTabNum = lTabNum; };
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ) override;
	virtual void ClearFeatureDefs() override;
	virtual BOOL ErrorCheck() override;
	virtual bool Update() override;

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };

private:
	void CreateControlRelation();

	/**
	 * @brief	�_�E���R���o�[�g�̂��߂̃R���g���[���̕\���E��\���𔻒f����
	 *
	 * @note	�_�E���R���o�[�g�ł�����������A���̊֐��͂Ȃ��Ȃ�ׂ�
	 */
	void SetControlState();

	/**
	 * @brief	�E�C���h�E�}�b�v�ύX
	 * @param	nIndex	[in]	�^�u�C���f�b�N�X
	 */
	void ChangeWindowMgrMap( INT nIndex );

	/**
	 * @brief	�����l�̕ύX��2�����b�V�����ׂ������Α��̕��������N�ɂ��K�p���A���������܂��B
	 *          �R���g���[�������삳��A�l���ύX�����^�C�~���O�ŌĂяo���܂��B
	 *
	 * @param	nId	[in]	�ύX���ꂽ�t�B�[���h�ɑΉ�����R���g���[����ID
	 */
	void SyncChangedAttrToOverMeshLink( INT nId );

	/**
	* @brief �H���������s���A�H������\������
	*
	 * �H�����̃e�[�u���ł��� WALK_CODE_LIST ���猟�����A��2�����Ŏ������R���g���[���ɕ\�����܂�
	*
	 * @param nCodeID	[in]	���s�ҘH���R�[�h�������Ă���G�f�B�b�g�R���g���[����ID
	 * @param nLabelID	[in]	���s�ҘH�����̂�\������X�^�e�B�b�N�R���g���[����ID
	*/
	void SetRoadName(INT nCodeID, INT nLabelID );

	/**
	* @brief ���H�R�t����������
	*
	* LINK_RELATION�e�[�u������A���ɕR�t���Ă��铹�H�����N��T���A���X�g�{�b�N�X�ɕ\�����܂�
	*/
	void InitBuddyRinks();
	
	/**
	* @brief �����N�R�t���e�[�u���t���O�ݒ�
	*
	* �R�t��Buddy��1�ȏ゠���1���Z�b�g�B
	* 1���Ȃ����0���Z�b�g�B
	*/
	void SetLinkRF();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< �e�R���g���[���E�B���h�E

	HWND		m_hChildWnd;				//!< �q�_�C�A���O�n���h��

	LONG		m_lTabNum;					//!< �^�u�R���g���[���ʒu
	INT			m_iChangeType;				//!< �R���g���[���؂�ւ��p

	LONG		m_lFeatureIndex;			//!< �t�B�[�`���C���f�b�N�X�ێ��p
	LONG		m_lTableIndex;				//!< �e�[�u���C���f�b�N�X�ێ��p
	LONG		m_lRowIndex;				//!< Row�C���f�b�N�X�ێ��p
	BOOL		m_bForce;					//!< �����ҏW���[�h�ێ��p
	BOOL		m_bEditable;				//!< �ҏW�ł��邩�ǂ����ێ��p
	
	
	bool			m_bIsLinkOnMesh;		//!< ���s�҃����N�����b�V�����ׂ��ł��邩�ǂ����̃t���O
	CTargetItems	m_cOverMeshLinks;		//!< ���b�V�����ׂ��ł�����s�҃����N��FeatureDef�ێ��p

	CBuddyRoadMgr m_buddyRoads;             //!< �o�f�B�֌W�Ǘ��p
	COption       m_cProperty;              //!< ���W�X�g���ݒ�l

};

#endif //__ATTRWALKLINKDLG_H_
