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

// LQAttrLaneDlg.h : CLQAttrLaneDlg �̐錾

#ifndef __LQATTRLANEDLG_H_
#define __LQATTRLANEDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

#include "../FeatureTree.h"
#include "LevelOID.h"

#include "WinLib/str2.h"
#include "WinLib/charutil.h"

#include <sindy/schema.h>		// [Bug9400]�Œǉ�
#include <boost/foreach.hpp>	// [Bug9400]�Œǉ�
#include <boost/assign.hpp>		// [Bug9400]�Œǉ�

const static TCHAR _NPAS_ = _T('0');
const static TCHAR _PASS_ = _T('1');
const static TCHAR _RPAS_ = _T('2');
const unsigned int WM_DISABLECONTROL  = RegisterWindowMessage(_T("WM_DISABLECONTROL"));       //!< �ĕ`��̈˗����󂯂邽�߂̃��b�Z�[�W

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/// ���[�������R�[�h�̃R�[�h�l�̂����A�g�p������̂̕ʖ��i[Bug9400]�Œǉ��j
namespace lane_dir_alias
{
	using namespace sindy::schema::inf_lane;

	enum ECode
	{
		kStraight = lane_dir::k0,	///< �O
		kRight    = lane_dir::k90,	///< �E
		kLeft     = lane_dir::k270,	///< ��
	};
}

/////////////////////////////////////////////////////////////////////////////
// CLQAttrLaneDlg
class CLQAttrLaneDlg : 
	public LQATTR_COMMON_BASE_CLASS,
	public CExportDlg
{
public:
	CLQAttrLaneDlg();

	~CLQAttrLaneDlg()
	{
		::DeleteObject( m_hStraightBmp );
		::DeleteObject( m_hStraightGBmp );
		::DeleteObject( m_hRightTurnBmp );
		::DeleteObject( m_hRightTurnGBmp );
		::DeleteObject( m_hLeftTurnBmp );
		::DeleteObject( m_hLeftTurnGBmp );
		::DeleteObject( m_hAllLaneBitmap );
		::DeleteObject( m_hDirection );
		::DeleteObject( m_hDirectionN );
		::DeleteObject( m_hDirectionNE );
		::DeleteObject( m_hDirectionE );
		::DeleteObject( m_hDirectionSE );
		::DeleteObject( m_hDirectionS );
		::DeleteObject( m_hDirectionSW );
		::DeleteObject( m_hDirectionW );
		::DeleteObject( m_hDirectionNW );
		::DeleteObject( m_hDirectionNull );	// [Bug9403]�Œǉ�
		::DeleteObject( m_hPatternA );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternB );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternC );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternD );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternE );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternF );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternA2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternB2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternC2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternD2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternE2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hPatternF2 );		// [Bug9400]�Œǉ�
		::DeleteObject( m_hDir_Working );	// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_0 );	// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_45 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_90 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_135 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_180 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_225 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_270 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_315 );// [Bug9403]�Œǉ�
		::DeleteObject( m_hDir_Null );// [Bug9403]�Œǉ�

		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg ���b�Z�[�W�}�b�v
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CLQAttrLaneDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_HANDLER(IDC_COMBO_INLINKID, CBN_SELCHANGE, OnChangeInLinkAndNodeID )
	COMMAND_HANDLER(IDC_COMBO_INNODEID, CBN_SELCHANGE, OnChangeInLinkAndNodeID )
	COMMAND_CODE_HANDLER(BN_CLICKED, OnButtonClick)
	COMMAND_HANDLER(IDC_EDIT_LANEPASS, EN_UPDATE, OnUpdateLanePassable )
	COMMAND_HANDLER(IDC_EDIT_LANEPASS, EN_KILLFOCUS, OnLanePassableChanged )
	COMMAND_HANDLER(IDC_EDIT_LANEAPP, EN_UPDATE, OnUpdateLRChange )
	COMMAND_HANDLER(IDC_EDIT_LANERCHANGE, EN_UPDATE, OnUpdateLRChange )
	COMMAND_HANDLER(IDC_EDIT_LANELCHANGE, EN_UPDATE, OnUpdateLRChange )
	NOTIFY_HANDLER(IDC_SPIN_LANERCHANGE, UDN_DELTAPOS, OnDeltaposSpin)
	NOTIFY_HANDLER(IDC_SPIN_LANELCHANGE, UDN_DELTAPOS, OnDeltaposSpin)
	NOTIFY_HANDLER(IDC_SPIN_LANEAPP, UDN_DELTAPOS, OnDeltaposSpin)

	MESSAGE_HANDLER(WM_DISABLECONTROL, OnDisableControl)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)

	CHAIN_MSG_MAP(LQATTR_COMMON_BASE_CLASS)
	COMMAND_HANDLER(IDC_EDIT_LANEAPP, EN_KILLFOCUS, OnLaneAppChanged )
	COMMAND_HANDLER(IDC_EDIT_LANELCHANGE, EN_KILLFOCUS, OnLRChanged )
	COMMAND_HANDLER(IDC_EDIT_LANERCHANGE, EN_KILLFOCUS, OnLRChanged )
ALT_MSG_MAP(IDC_COMBO_INLINKID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_INNODEID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANEAPP)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANERCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANELCHANGE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANEPASS)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_L1R0)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_L0R0)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_L0R1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_STATIC_LANESTATE)
	COMMAND_CODE_HANDLER(BN_CLICKED, OnLaneButtonClick)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_A)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_B)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_C)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_D)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_E)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_F)						// [Bug9400]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_WORKING)						// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_0)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_45)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_90)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_135)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_180)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_225)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_270)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_315)							// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_NULL)						// [Bug9403]�Œǉ�
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg ���b�Z�[�W�n���h��
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_bInitializing = TRUE;
		m_bOnInitDlg = TRUE;
		m_bInitialCheckedLaneConsistency = FALSE;
		// �v���p�e�B�����W�X�g������ǂݍ���
		CString strRegOpenKey;
		strRegOpenKey.Format( REGOPENKEY, SOFTWARE_KEY, INSTALL_KEY, TOOLS_KEY );
		HKEY hOpenKey = EmxCreateRegKey( strRegOpenKey, HKEY_CURRENT_USER );
		if ( hOpenKey ) {
			DWORD dwResult;
			if( EmxGetRegDword( _T("LaneConsistency"), &dwResult, hOpenKey ) )
				m_bCheckLaneConsistency = (BOOL)dwResult;
			if( EmxGetRegDword( _T("LaneConsistencyw"), &dwResult, hOpenKey ) )
				m_bCheckLaneConsistencyw = (BOOL)dwResult;
			if( EmxGetRegDword( _T("LaneBitEditable"), &dwResult, hOpenKey ) )
				m_bLaneBitEditable = (BOOL)dwResult;

			RegCloseKey( hOpenKey );
		}
		// �R���g���[��
		CreateControlRelation();
		// �T�u�N���X��
		m_wndLaneInLink.SubclassWindow(GetDlgItem(IDC_COMBO_INLINKID));
		m_wndLaneInNode.SubclassWindow(GetDlgItem(IDC_COMBO_INNODEID));
		m_wndLaneApp.SubclassWindow(GetDlgItem(IDC_EDIT_LANEAPP));
		m_wndLaneRChange.SubclassWindow(GetDlgItem(IDC_EDIT_LANERCHANGE));
		m_wndLaneLChange.SubclassWindow(GetDlgItem(IDC_EDIT_LANELCHANGE));
		m_wndLanePass.SubclassWindow(GetDlgItem(IDC_EDIT_LANEPASS));
		m_wndLaneBtn1.SubclassWindow(GetDlgItem(IDC_BUTTON_L1R0));
		m_wndLaneBtn2.SubclassWindow(GetDlgItem(IDC_BUTTON_L0R0));
		m_wndLaneBtn3.SubclassWindow(GetDlgItem(IDC_BUTTON_L0R1));
		m_wndLaneGroup.SubclassWindow(GetDlgItem(IDC_STATIC_LANESTATE));
		m_wndSource.SubclassWindow(GetDlgItem(IDC_EDIT_SOURCE));
		m_wndPatternA.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_A ) );		// [Bug9400]�Œǉ�
		m_wndPatternB.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_B ) );		// [Bug9400]�Œǉ�
		m_wndPatternC.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_C ) );		// [Bug9400]�Œǉ�
		m_wndPatternD.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_D ) );		// [Bug9400]�Œǉ�
		m_wndPatternE.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_E ) );		// [Bug9400]�Œǉ�
		m_wndPatternF.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_F ) );		// [Bug9400]�Œǉ�
		m_wndDir_Working.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_WORKING ) );	// [Bug9403]�Œǉ�
		m_wndDir_0.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_0 ) );	// [Bug9403]�Œǉ�
		m_wndDir_45.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_45 ) );	// [Bug9403]�Œǉ�
		m_wndDir_90.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_90 ) );	// [Bug9403]�Œǉ�
		m_wndDir_135.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_135 ) );	// [Bug9403]�Œǉ�
		m_wndDir_180.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_180 ) );	// [Bug9403]�Œǉ�
		m_wndDir_225.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_225 ) );	// [Bug9403]�Œǉ�
		m_wndDir_270.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_270 ) );	// [Bug9403]�Œǉ�
		m_wndDir_315.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_315 ) );	// [Bug9403]�Œǉ�
		m_wndDir_Null.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_NULL ) );	// [Bug9403]�Œǉ�

		// �t�H���g�ݒ�
		m_wndLaneApp.SetFont( m_hFont );
		m_wndLaneRChange.SetFont( m_hFont );
		m_wndLaneLChange.SetFont( m_hFont );
		::SendMessage( GetDlgItem(IDC_EDIT_LANEFLOW2), WM_SETFONT, (WPARAM)m_hFont, 0L );

		LRESULT nRet = 0;
		if( m_cArcHelper.IsStartEditing() )
		{
			nRet = LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
			::SetFocus( GetDlgItem(IDC_COMBO_INLINKID) );
		}
		else
			nRet = LQATTR_COMMON_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

		m_bInitializing = FALSE;

		return nRet;
	}
	LRESULT OnUpdateLanePassable(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
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
					CString strLanePassable = ConvertToLanePassageStr( lpszText );
					if( strLanePassable.Compare( lpszText ) != 0 )
					{
						INT dwSelStart = 0, dwSelEnd = 0;
						edit.GetSel( dwSelStart, dwSelEnd );
						edit.SetWindowText( strLanePassable );
						// ���͈ʒu�̏C��
						INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strLanePassable );
						edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
					}
				}
				m_cControlRel.ControlChanged( wID );
				SetButton( Changed() );
				InvalidateRect( NULL, FALSE );
			}
		}
		else 
			bHandled = FALSE;

		return 0;

	}
	LRESULT OnUpdateLRChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug 5139]OnInitDlg�̑O�ɁA���b�Z�[�W�����Ă��܂��AConvertToLRChange�ɓ�����
		// �G���[���b�Z�[�W���łĂ��܂����߁A�_�C�A���O�쐬���ꂽ���ǂ������m�F����
		if( !m_bOnInitDlg )
			return 0;

		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			ConvertToLRChange( wID, hWndCtl );
		else
			bHandled = FALSE;

		return 0;
	}

	/**
	 * @brief �u��n�Em�v�̂����ꂩ�̃{�^���������ꂽ���̏���
	 * @note	[Bug9400]�Ή����ɁA���̉ӏ��ł��g�����߂�OnButtonClick()�̈ꕔ���֐����B
	 * @param[in]	cControlID	�����ꂽ�{�^���̃R���g���[��ID
	 * @retval	false	�u��n�Em�v�n�ł͂Ȃ��R���g���[��ID���w�肳�ꂽ�̂ŏ������s
	 * @retval	true	���������i�u��N�EN�v�n�̃R���g���[��ID���w�肳���ΕK����������j
	 */
	bool ClickedLnRmButton( const WORD cControlID )
	{
		LONG lRChange, lLChange;

		switch( cControlID )
		{
			case IDC_BUTTON_L1R0: lLChange = 1; lRChange = 0; break;
			case IDC_BUTTON_L0R1: lLChange = 0; lRChange = 1; break;
			case IDC_BUTTON_L0R0: lLChange = 0; lRChange = 0; break;
			default: return false; break;
		}
		for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
		{
			it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strLChangeFieldName )->SetNewValue( CComVariant( lLChange ), m_hWnd );
			it->m_pLQRowDef->GetFieldDef( m_strInfoTableName, m_strRChangeFieldName )->SetNewValue( CComVariant( lRChange ), m_hWnd );
		}
		// �����l�\�����Ȃ���
		TCHAR strChange[10];
		::SetWindowText( GetDlgItem( IDC_EDIT_LANELCHANGE ), _ltot( lLChange, strChange, 10 ) );
		::SetWindowText( GetDlgItem( IDC_EDIT_LANERCHANGE ), _ltot( lRChange, strChange, 10 ) );
		// ���[���̃{�^����蒼��
		CreateLaneButton();
		// �ĕ`��
		SetButton( Changed() );

		return true;
	}

	/**
	 * @brief �p�x�ɂ�茈�肵��LaneDir_C�i�O�E�E�E���̂����ꂩ�j��Ԃ�
	 * @note	[Bug9400]�Ή����ɁA���̉ӏ��ł��g�����߂�OnButtonClick()�̈ꕔ���֐����B
	 * @param[in]	cAngle	�p�x
	 * @return	LaneDir_C
	 */
	long LaneDirFromAngle( const long cAngle ) const
	{
		using namespace lane_dir_alias;

		// [bug 2040] ���o��ꃊ���N�Ƃ̊p�x�Ŕ��肷��悤�ɏC��
		if      (160 <= cAngle && cAngle <= 200) { return kStraight; }	// �O
		else if ( cAngle < 160 )                 { return kRight;    }	// �E
		else                                     { return kLeft;     }	// ��
	}

	/**
	 * @brief �����������̓{�^���������ꂽ���̏���
	 * @note	[Bug9400]�Ή����ɁA���̉ӏ��ł��g�����߂�OnButtonClick()�̈ꕔ���֐����B
	 */
	void ClickedAutoDirButton()
	{
		for( list<CLevelOID>::iterator it = m_listSameINFeature.begin(); it != m_listSameINFeature.end(); it++ )
		{
			FEATUREDEF* pFeatDef = it->m_pLQRowDef;
			if( pFeatDef != NULL )
			{
				CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLaneDir );
				if( pFieldDef != NULL )
				{
					LONG lVal = LaneDirFromAngle( it->m_lAngle );	// [Bug9400]�Ή����Ɋ֐���

					// �l���Z�b�g
					pFieldDef->SetNewValue( lVal, m_hWnd );
				}
				else
					_ASSERTE( pFieldDef );
			}
			else
				_ASSERTE( pFeatDef );
		}
		// �����r�b�g�}�b�v�Z�b�g
		SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// �{�^���ĕ`��
		SetButton( Changed() );
	}

	LRESULT OnButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;

		if (! m_bInitializing)
		{
			switch (wID)
			{
				// �����p�{�^��
				case IDC_BUTTON_L1R0:
				case IDC_BUTTON_L0R0:
				case IDC_BUTTON_L0R1:
					if (! ClickedLnRmButton( wID ))		// [Bug9400]�Ή����Ɋ֐���
						{ return 0; }
					break;
				// ������������{�^��
				case IDC_BUTTON_AUTODIR:
					ClickedAutoDirButton();				// [Bug9400]�Ή����Ɋ֐���
					break;
				// �p�^�[�����̓{�^���i[Bug9400]�Œǉ��j
				case IDC_BUTTON_PATTERN_A:
				case IDC_BUTTON_PATTERN_B:
				case IDC_BUTTON_PATTERN_C:
				case IDC_BUTTON_PATTERN_D:
				case IDC_BUTTON_PATTERN_E:
				case IDC_BUTTON_PATTERN_F:
					SetControlsByPattern( wID );
					break;
				// ���[���������̓{�^���i[Bug9403]�Œǉ��j
				case IDC_BUTTON_DIR_WORKING:
				case IDC_BUTTON_DIR_0:
				case IDC_BUTTON_DIR_45:
				case IDC_BUTTON_DIR_90:
				case IDC_BUTTON_DIR_135:
				case IDC_BUTTON_DIR_180:
				case IDC_BUTTON_DIR_225:
				case IDC_BUTTON_DIR_270:
				case IDC_BUTTON_DIR_315:
				case IDC_BUTTON_DIR_NULL:
					SetDirectionByPattern( wID );
					break;
				default:
					break;
			}
		}
		else
			bHandled = FALSE;

		return 0;	
	}

	/**
	 * @brief ���[�������{�^���̓��͂𔽉f������ [Bug9304]�Œǉ�
	 * @param[in]	cControlID	�����ꂽ�{�^���̃R���g���[��ID
	 */
	void SetDirectionByPattern( const WORD cControlID )
	{
		CComVariant	vaNewVal;	// �V��������
		switch(cControlID)
		{
			case IDC_BUTTON_DIR_WORKING:  // �������E��ƒ�
				vaNewVal = 0;
				break;
			case IDC_BUTTON_DIR_0:        // �O����(0)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k0;
				break;
			case IDC_BUTTON_DIR_45:       // �E�΂ߑO����(45)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k45;
				break;
			case IDC_BUTTON_DIR_90:       // �E����(90)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k90;
				break;
			case IDC_BUTTON_DIR_135:      // �E�΂ߌ�����(135)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k135;
				break;
			case IDC_BUTTON_DIR_180:      // ������(180)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k180;
				break;
			case IDC_BUTTON_DIR_225:      // ���΂ߌ�����(225)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k225;
				break;
			case IDC_BUTTON_DIR_270:      // ������(270)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k270;
				break;
			case IDC_BUTTON_DIR_315:      // ���΂ߑO����(315)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k315;
				break;
			case IDC_BUTTON_DIR_NULL:     // NULL
				vaNewVal.vt	= VT_NULL;
				break;
			default:
				_ASSERTE( cControlID );
		}

		FEATUREDEF* pFeatDef = GetCurrentFeatureDef();
		if( pFeatDef )
		{
			CFieldDef* pFieldDef = pFeatDef->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLaneDir );
			if( pFieldDef )
			{
				// �l���Z�b�g
				pFieldDef->SetNewValue( vaNewVal, m_hWnd );
			}
			else
				_ASSERTE( pFieldDef );
		}
		else
			_ASSERTE( pFeatDef );

		// �����r�b�g�}�b�v�Z�b�g
		SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// �{�^���ĕ`��
		SetButton( Changed() );
	}


	/**
	 * @brief �����ꂽ�p�^�[�����̓{�^���ɉ����āA���̃R���g���[���Q�ɒl���Z�b�g����i[Bug9400]�Œǉ��j
	 * @note	�E���E�u�����v�Ɓu���ߎԐ����v�́A�D������ɕς���ƕs�������N����̂ŁA�����̏����݂̂̑g�ݍ��킹�ɂ���ď�Ԃ�ς���B
	 *			�E�u���[�������v�́A�u�����������́v�{�^���������ꂽ���Ɠ����������s�����Ƃŕς���B
	 *			�E�u�Ԑ��ʍs��ԁv��ς��鏈���͊����ł͂Ȃ������̂Œ��ڕς��鏈����ǉ��B�ύX���Ă����ւ̈��e���͂Ȃ��Ɣ��f�B
	 * @param[in]	cControlID	�����ꂽ�{�^���̃R���g���[��ID
	 */
	void SetControlsByPattern( const WORD cControlID )
	{
		// ���E�����ƒ��ߎԐ����̒l��ύX����
		switch (cControlID)
		{
			case IDC_BUTTON_PATTERN_A:
			case IDC_BUTTON_PATTERN_E:
			case IDC_BUTTON_PATTERN_F:
				if (! ClickedLnRmButton( IDC_BUTTON_L0R0 )) { ATLASSERT(0); return; }	// �uL0R1�v�͉����Ȃ��ꍇ������̂ŁA�܂����ł�������uL0R0�v�����������ɂ���
				LaneAppChanged( TRUE, 2 );												// �u��0�A�E0�v�ł���΁A���߂�2�ɂ��Ă����Ȃ��i�͂��j
				if (! ClickedLnRmButton( IDC_BUTTON_L0R1 )) { ATLASSERT(0); return; }	// �u����2�A��0�A�E0�v�ł���΁A�uL0R1�v�������Ă����Ȃ��i�͂��j
				break;
			case IDC_BUTTON_PATTERN_B:
			case IDC_BUTTON_PATTERN_D:
				if (! ClickedLnRmButton( IDC_BUTTON_L0R0 )) { ATLASSERT(0); return; }
				LaneAppChanged( TRUE, 3 );
				if (! ClickedLnRmButton( IDC_BUTTON_L0R1 )) { ATLASSERT(0); return; }
				break;
			case IDC_BUTTON_PATTERN_C:
				if (! ClickedLnRmButton( IDC_BUTTON_L0R0 )) { ATLASSERT(0); return; }
				LaneAppChanged( TRUE, 2 );
				break;
			default:
				break;
		}

		// �����������̓{�^���������ꂽ���Ɠ����������s���ALaneDir_C���X�V����
		ClickedAutoDirButton();

		// ���O���[�v�ɑ�����e�I�u�W�F�N�g�̎Ԑ��ʍs��Ԃ�ύX����
		BOOST_FOREACH ( const CLevelOID& rCLevelOID, m_listSameINFeature )
		{
			FEATUREDEF* pFeatureDef = rCLevelOID.m_pLQRowDef;
			if( pFeatureDef )
			{
				CFieldDef* pFDLanePassable = pFeatureDef->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLanePassable );
				const long aLaneDir = pFeatureDef->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLaneDir )->m_vaValue.lVal;

				pFDLanePassable->SetNewValue( CComVariant( LanePassableForPattern( cControlID, aLaneDir ) ), m_hWnd );
			}
		}
		// ��I���I�u�W�F�N�g�̎Ԑ��ʍs��Ԃ̒l���_�C�A���O�ɔ��f
		const CString aCurrentLanePassable = GetCurrentFeatureDef()->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLanePassable )->m_vaValue.bstrVal;
		OnChangeLanePassable( aCurrentLanePassable );

		// ���[���̃{�^���Q����蒼��
		CreateLaneButton();
		// �ĕ`��
		SetButton( Changed() );
	}

	/**
	 * @brief ���[���̃p�^�[���ƃ��[�������R�[�h�ɉ����āA�u�Ԑ��ʍs��ԁv�ɐݒ肷�ׂ��������Ԃ��i[Bug9400]�Œǉ��j
	 * @note	static�����o�֐��ɂ��Ă���̂́A�����o�ϐ��Ɗ֌W�Ȃ����𖾎����邽�߁B
	 * @param[in]	cControlID	���[���̓��Y�p�^�[���ɑΉ�����{�^���̃R���g���[��ID
	 * @param[in]	cLaneDir	���[�������R�[�h�i���Y�p�^�[���Ɋ܂܂������̂����A�ǂ̕������j
	 * @return	�u�Ԑ��ʍs��ԁv�ɐݒ肷�ׂ�������i����ł��Ȃ������ꍇ�͋󕶎���j
	 */
	static CString LanePassableForPattern( const WORD cControlID, const long cLaneDir )
	{
		using namespace lane_dir_alias;

		typedef WORD ControlID;
		typedef long LaneDir;
		typedef CString LanePassableStr;
		typedef std::map< std::pair< ControlID, LaneDir >, LanePassableStr > LanePassableTable;	///< �u�p�^�[���{�����v�Ɓu�Ԑ��ʍs��ԁv�̑Ή��\

		static const LanePassableTable aTable = boost::assign::map_list_of
			( std::make_pair( IDC_BUTTON_PATTERN_A,		kLeft     ),	"10"  )
			( std::make_pair( IDC_BUTTON_PATTERN_A,		kStraight ),	"10"  )
			( std::make_pair( IDC_BUTTON_PATTERN_A,		kRight    ),	"01"  )
			( std::make_pair( IDC_BUTTON_PATTERN_B,		kLeft     ),	"100" )
			( std::make_pair( IDC_BUTTON_PATTERN_B,		kStraight ),	"110" )
			( std::make_pair( IDC_BUTTON_PATTERN_B,		kRight    ),	"001" )
			( std::make_pair( IDC_BUTTON_PATTERN_C,		kLeft     ),	"10"  )
			( std::make_pair( IDC_BUTTON_PATTERN_C,		kStraight ),	"11"  )
			( std::make_pair( IDC_BUTTON_PATTERN_D,		kStraight ),	"110" )
			( std::make_pair( IDC_BUTTON_PATTERN_D,		kRight    ),	"001" )
			( std::make_pair( IDC_BUTTON_PATTERN_E,		kStraight ),	"10"  )
			( std::make_pair( IDC_BUTTON_PATTERN_E,		kRight    ),	"01"  )
			( std::make_pair( IDC_BUTTON_PATTERN_F,		kLeft     ),	"10"  )
			( std::make_pair( IDC_BUTTON_PATTERN_F,		kRight    ),	"01"  );

		LanePassableTable::const_iterator it = aTable.find( std::make_pair( cControlID, cLaneDir ) );
		if (it == aTable.end())
		{
			ATLASSERT(0);		// �Ή��\�ɂȂ��g�ݍ��킹���w�肳���̂͂�������
			return CString();
		}
		return it->second;
	}

	LRESULT OnLaneButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;

		if( !m_bInitializing )
		{
			if( 1000 <= wID && wID < 2000 )
			{
				ULONG ulNum = wID - 1000;
				if( m_listSameINFeature.size() > ulNum )
				{
					LONG lOID = -1;

					list<CLevelOID>::iterator it = m_listSameINFeature.begin();
					advance( it, ulNum );
					it->m_pLQRowDef->GetFeature()->get_OID( &lOID );

					// �I�[�i���������Ƃ��ēn���Ȃ��Ƃ����Ȃ�
					_IRowPtr ipCurrentRow;
					ipCurrentRow = GetCurrentFeatureDef()->GetFeature();
					CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentRow );
					if( !strOwnerName.IsEmpty() )
						strOwnerName += _T(".") + AheLoadString( IDS_LANE );
					else
						strOwnerName =  AheLoadString( IDS_LANE );

					return ::SendMessage( GetParent(), WM_CHANGECURRENTFEATURE, (WPARAM)(LPCTSTR)strOwnerName, (LPARAM)lOID );
				}
			}
			else if( ( 2000 <= wID && wID < 3000 ) && IsStartEditing() )
			{
				CString strLanePass = GetLanePassable( GetCurrentFeatureDef() );
				LONG lOID = GetLaneOID( GetCurrentFeatureDef() );

				ATLASSERT( ! strLanePass.IsEmpty() );

				div_t divt = div( wID - 2000, 3 );
				switch( divt.rem )
				{
					case 0: strLanePass.SetAt( divt.quot, _PASS_ ); break;
					case 1: strLanePass.SetAt( divt.quot, _RPAS_ ); break;
					case 2: strLanePass.SetAt( divt.quot, _NPAS_ ); break;
					default: return 0; break;
				}
				if( CheckLaneBit( lOID, strLanePass, FALSE, FALSE ) )
				{
					OnChangeLanePassable( strLanePass );
					return 0;
				}
			}
		}
		else
			bHandled = FALSE;

		return 0;
	}
	LRESULT OnChangeInLinkAndNodeID(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( !m_bInitializing )
		{
			CComboBox comboLink( GetDlgItem( IDC_COMBO_INLINKID ) );
			INT nIndexLink = comboLink.GetCurSel();
			LPTSTR lpszInLinkID = (LPTSTR)_alloca( ( comboLink.GetLBTextLen( nIndexLink ) + 1 ) * sizeof(TCHAR) );
			comboLink.GetLBText( nIndexLink, lpszInLinkID );

			// �����m�[�hID�̒l���ύX���ꂽ�Ƃ��̂ݗ����m�[�hID���n��
			LPTSTR lpszInNodeID = _T("-1");
			if( wID == IDC_COMBO_INNODEID )
			{
				CComboBox comboNode( GetDlgItem( IDC_COMBO_INNODEID ) );
				INT nIndexNode = comboNode.GetCurSel();
				lpszInNodeID = (LPTSTR)_alloca( ( comboNode.GetLBTextLen( nIndexNode ) + 1 ) * sizeof(TCHAR) );
				comboNode.GetLBText( nIndexNode, lpszInNodeID );
			}

			LONG lOID = ChangeINLinkAndNodeID( _ttol( lpszInLinkID ), _ttol( lpszInNodeID ) );

			// �I�[�i���������Ƃ��ēn���Ȃ��Ƃ����Ȃ�
			_IRowPtr ipCurrentRow;
			ipCurrentRow = GetCurrentFeatureDef()->GetFeature();
			CString strOwnerName = AheGetFeatureClassOwnerName( ipCurrentRow );
			if( !strOwnerName.IsEmpty() )
				strOwnerName += _T(".") +  AheLoadString( IDS_LANE );
			else
				strOwnerName =  AheLoadString( IDS_LANE );

			return ::SendMessage( GetParent(), WM_CHANGECURRENTFEATURE, (WPARAM)(LPCTSTR)strOwnerName, (LPARAM)lOID );
		}
		else {
			bHandled = FALSE;
			return 0;
		}
	}
	LRESULT OnLanePassableChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( m_lLastLink == 1 )
			return 0;

		OnChangeLanePassable();

		return 0;
	}
	LRESULT OnLaneAppChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			LaneAppChanged();
		else
			bHandled = FALSE;

		return 0;
	}
	LRESULT OnLRChanged(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			LRChanged( wID );
		else
			bHandled = FALSE;

		return 0;
	}
	LRESULT OnChangeLaneClass(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// �{�^���ĕ`��
		SetButton( Changed() );

		bHandled = FALSE;

		return 0;
	}
	LRESULT OnDeltaposSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		// [Bug3106]�Ή��ɂ��A�I�[�����N�݂̂̑I���̎��ɂ͂Ȃɂ��ł��Ȃ��悤�ɂ���
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
		{
			LPNMUPDOWN lpnmud = (LPNMUPDOWN)pnmh;
			if( lpnmud != NULL )
			{
				CUpDownCtrl cSpin = GetDlgItem( idCtrl );							// �X�s���R���g���[��
				INT nCurrentPos = lpnmud->iPos + lpnmud->iDelta;
				INT nLower, nUpper;
				cSpin.GetRange( nLower, nUpper );
				if( nLower <= nCurrentPos && nCurrentPos <= nUpper )
				{
					switch( idCtrl )
					{
						case IDC_SPIN_LANERCHANGE: LRChanged( IDC_EDIT_LANERCHANGE, TRUE, nCurrentPos ); break;
						case IDC_SPIN_LANELCHANGE: LRChanged( IDC_EDIT_LANELCHANGE, TRUE, nCurrentPos ); break;
						case IDC_SPIN_LANEAPP:     LaneAppChanged( TRUE, nCurrentPos ); break;
						default: ASSERT(FALSE); break;
					}
				}
			}
		}
		else
			bHandled = FALSE;

		return 0;
	}

    LRESULT OnDisableControl( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
    {
		m_lLastLink = (long)wParam;

		return 0;
    }


	// ���[�_���̎��ɃL�[��߂܂���
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
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

		bHandled = FALSE;
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg �����o�֐���`
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ LQATTR_COMMON_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ LQATTR_COMMON_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){ LQATTR_COMMON_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){};
	virtual void ClearFeatureDefs(){ LQATTR_COMMON_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return LQATTR_COMMON_BASE_CLASS::CheckEscKeyItem(nClassID); };

	void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	BOOL FeatureIsValid( CString& strErrMsg );

private:
	void OnChangeLanePassable( LPCTSTR lpcszLanePassable = NULL );
	void CreateControlRelation();
	void CreateChangeLaneButton( LONG lCurrentLaneOID, INT nTop, INT nButtonWidth, INT nButtonHeight, LONG lLeftChange, LONG lRightChange );
	BOOL CreateLaneButton();
	void CreateLaneStateButton(  LPCTSTR lpcszLaneState, INT nLaneNum, INT nTop, INT nButtonWidth, INT nButtonHeight, INT nLeftChange, INT nRightChange );
	void SortByAngle( CLQRowDef* pRowDef, LONG lStartLinkID, list<FEATUREDEF*>& listOrg, list<CLevelOID>& listNew );
	DOUBLE GetAngle( IPolylinePtr ipPolyline, LONG lStart = 0, BOOL bIsDrawAngle = FALSE );
	DOUBLE GetAngle2( IPolylinePtr ipPolyline, LONG lStart, LONG lEnd );
//	HBITMAP GetLaneBitmapByAngle( map<LONG, LONG>& mapLevelAngle, BOOL bCurrent );
	HBITMAP GetLaneBitmapByAngle( LONG lAngle, BOOL bCurrent );
	HBITMAP CreateAllLaneBitmap( const LONG lCurrentOID, const INT nButtonLen, const INT nLaneNum );
	HBITMAP CreateLaneBitmap( HBITMAP hBitmapSrc, INT nLeft, INT nWidth );
	HBITMAP CreateStaticLaneBitmap( INT nLaneNum, INT nLeftChange, INT nRightChange );
	void SetLaneDirectionBitmap( FEATUREDEF* pFeatDef );
	LONG ChangeINLinkAndNodeID( LONG lInLinkID, LONG lInNodeID );
	CString ConvertToLanePassageStr( LPCTSTR lpcszText );
	CString ConvertToLongStr( LPCTSTR lpcszText );
	void ConvertToLRChange( WORD wID, HWND hWndCtl );
	BOOL ResetLanePassable( CString& strLanePassOrg, FEATUREDEF* pFeatDef = NULL );
	BOOL CheckLaneBit( const list<CLevelOID>& listLane );
	BOOL CheckLaneBit( LONG lTargetOID, CString& strLaneState, INT& nStart, INT& nEnd, INT& nRStart, INT& nREnd, BOOL bRecover = FALSE, BOOL bAssert = TRUE );
	inline BOOL CheckLaneBit( LONG lTargetOID, CString& strLaneState, BOOL bRecover = FALSE, BOOL bAssert = TRUE )
	{
		INT nStart, nEnd, nRStart, nREnd;
		return CheckLaneBit( lTargetOID, strLaneState, nStart, nEnd, nRStart, nREnd, bRecover, bAssert );
	}
	LONG GetLaneOID( FEATUREDEF* pFeatDef );
	LONG GetLaneInLinkID( FEATUREDEF* pFeatDef );
	LONG GetLaneOutLinkID( FEATUREDEF* pFeatDef );
	LONG GetLongValue( CRowDef* pRowDef, LPCTSTR lpcszFieldName );
	IFeatureClassPtr GetRoadLinkWithLQ( CLQRowDef* pRowDef );
	IFeaturePtr GetRoadFeature( CLQRowDef* pRowDef, LONG lOID );
	BOOL AheSetNewLaneAttribute( list<FEATUREDEF*>& listFeatureDef, list<CLevelOID>& listLevelOID );
	BOOL HasReservedLane( LONG lExeptOID, list<INT>& listBits );
	CString GetLanePassable( FEATUREDEF* pFeatDef );
	BOOL SetLanePassable( FEATUREDEF* pFeatDef, LPCTSTR lpcszLanePassable );
//	INT GetLaneBitNum();
	BOOL AddOrDeleteLaneBit( LONG lExeptOID, INT nBitNum );

	/**
	 * @brief �����́u���v�܂��́u�E�v�̒l���ύX���ꂽ���̏���
	 * @param[in]	wID			�ύX���ꂽ���ɑΉ�����G�f�B�b�g�R���g���[���̃R���g���[��ID
	 * @param[in]	bUseArg		nLRChange�̒l���L���ł��邩�ۂ�
	 * @param[in]	nLRChange	�ύX��̒l
	 * @return	���Ӗ��i���TRUE�j
	 */
	BOOL LRChanged( INT wID, BOOL bUseArg = FALSE, INT nLRChange = -100 );

	/**
	 * @brief �u���ߎԐ����v�̒l���ύX���ꂽ���̏���
	 * @param[in]	nNewLaneApp		nLRChange�̒l���L���ł��邩�ۂ�
	 * @param[in]	nNewLaneApp	�ύX��̒l
	 * @return	���Ӗ��i���TRUE�j
	 */
	BOOL LaneAppChanged( BOOL bUseArg = FALSE, INT nNewLaneApp = 0 );

	/**
	 * @brief �p�^�[�����̓{�^����L�������ׂ����ۂ��𔻒肷��i[Bug9400]�Œǉ��j
	 * @param[in]	cControlID	�p�^�[�����̓{�^���̃R���g���[��ID
	 * @return	���茋��
	 */
	bool AvailabilityOfPatternButton( const long cControlID ) const;

	/**
	 * @brief �p�^�[�����̓{�^���Q�̏�ԁiEnabled/Disabled�Ɖ摜�j��K�؂ɐݒ肷��i[Bug9400]�Œǉ��j
	 * @param[in]	cIsEditable	�ҏW�\���ǂ���
	 */
	void SetStateOfPatternButtons( bool cIsEditable );

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg �����o�ϐ���`
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndLaneInLink, m_wndLaneInNode, m_wndLaneApp, m_wndLaneRChange, m_wndLaneLChange, m_wndLanePass, m_wndLaneGroup;
	CContainedWindow		m_wndLaneBtn1, m_wndLaneBtn2, m_wndLaneBtn3, m_wndSource;
	CContainedWindow		m_wndPatternA, m_wndPatternB, m_wndPatternC, m_wndPatternD, m_wndPatternE, m_wndPatternF;	///< �p�^�[�����̓{�^���i[Bug9400]�Œǉ��j
	CContainedWindow		m_wndDir_0, m_wndDir_45, m_wndDir_90, m_wndDir_135, m_wndDir_180, m_wndDir_225, m_wndDir_270, m_wndDir_315, m_wndDir_Null, m_wndDir_Working;	///< [Bug9403]�Œǉ�
	HBITMAP					m_hStraightBmp, m_hStraightGBmp, m_hRightTurnBmp, m_hRightTurnGBmp, m_hLeftTurnBmp, m_hLeftTurnGBmp;
	HBITMAP					m_hAllLaneBitmap;
	HBITMAP					m_hDirection, m_hDirectionN, m_hDirectionNE, m_hDirectionE, m_hDirectionSE, m_hDirectionS, m_hDirectionSW, m_hDirectionW, m_hDirectionNW, m_hDirectionNull;
	HBITMAP					m_hPatternA, m_hPatternB, m_hPatternC, m_hPatternD, m_hPatternE, m_hPatternF;		///< �p�^�[�����̓{�^���iEnabled���j�p�̉摜�i[Bug9400]�Œǉ��j
	HBITMAP					m_hPatternA2, m_hPatternB2, m_hPatternC2, m_hPatternD2, m_hPatternE2, m_hPatternF2;	///< �p�^�[�����̓{�^���iDisabled���j�p�̉摜�i[Bug9400]�Œǉ��BEnabled�p�̉摜���A�{�^����Disabled�ɂ��鎖�ɂ���ăO���[�X�P�[��������ƈӖ��̕�����Ȃ��A�C�R���ɂȂ�̂ŁADisabled����p�̉摜��p�ӂ����B�j
	HBITMAP					m_hDir_0, m_hDir_45, m_hDir_90, m_hDir_135, m_hDir_180, m_hDir_225, m_hDir_270, m_hDir_315, m_hDir_Null, m_hDir_Working; // [Bug9403]�Œǉ�
	list<CLevelOID> 		m_listSameINFeature;	//!< �������������N�������[���i�[�p
//	CSize					m_cLaneStateSize;
	BOOL					m_bInitializing;			//!< OnInitDialog ���� TRUE
	BOOL					m_bOnInitDlg;
	HFONT					m_hFont;

	CString					m_strOldLChange, m_strOldRChange, m_strOldAppLane;	//!< �E���ܑ����y�ђ��ߎԐ��������̕ێ�
	static CString			m_strInfoTableName;
	static CString			m_strAppLaneFieldName;
	static CString			m_strLanePassFieldName;
	static CString			m_strRChangeFieldName;
	static CString			m_strLChangeFieldName;

	// �v���p�e�B
	BOOL					m_bInitialCheckedLaneConsistency;	//!< �_�C�A���O���J�����Ƃ��Ɉ�x�����`�F�b�N�����邽�߂̃t���O
	BOOL					m_bCheckLaneConsistency;	//!< ���[���̐����������邩�ǂ���
	BOOL					_m_bCheckLaneConsistency;	//!< �����������邩�ǂ�������x���������Ă��Ȃ��悤�ɂ��邽��
	BOOL					m_bCheckLaneConsistencyw;	//!< �����`�F�b�N�����Ȃ��ꍇ�Ɍx�����o�����ǂ����iTRUE:�o���Ȃ��j
	BOOL					m_bLaneBitEditable;			//!< ���[���̃r�b�g���삪�ł��邩�ǂ���
	ISiNDYRulePtr		m_ipRule;			//!< ��ƖړIDB�Ή�
	HWND				m_hChildWnd;		//!< �q�_�C�A���O�n���h��
	long					m_lLastLink;
};

#endif //__LQATTRLANEDLG_H_
