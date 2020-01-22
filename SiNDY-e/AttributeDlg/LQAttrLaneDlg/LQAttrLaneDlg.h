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

// LQAttrLaneDlg.h : CLQAttrLaneDlg の宣言

#ifndef __LQATTRLANEDLG_H_
#define __LQATTRLANEDLG_H_

#include "ExportDlg.h"
#include "LQAttrCommonBaseDlg.h"
#include "resource.h"

#include "../FeatureTree.h"
#include "LevelOID.h"

#include "WinLib/str2.h"
#include "WinLib/charutil.h"

#include <sindy/schema.h>		// [Bug9400]で追加
#include <boost/foreach.hpp>	// [Bug9400]で追加
#include <boost/assign.hpp>		// [Bug9400]で追加

const static TCHAR _NPAS_ = _T('0');
const static TCHAR _PASS_ = _T('1');
const static TCHAR _RPAS_ = _T('2');
const unsigned int WM_DISABLECONTROL  = RegisterWindowMessage(_T("WM_DISABLECONTROL"));       //!< 再描画の依頼を受けるためのメッセージ

typedef CLQAttrCommonBaseDlg LQATTR_COMMON_BASE_CLASS;

/// レーン方向コードのコード値のうち、使用するものの別名（[Bug9400]で追加）
namespace lane_dir_alias
{
	using namespace sindy::schema::inf_lane;

	enum ECode
	{
		kStraight = lane_dir::k0,	///< 前
		kRight    = lane_dir::k90,	///< 右
		kLeft     = lane_dir::k270,	///< 左
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
		::DeleteObject( m_hDirectionNull );	// [Bug9403]で追加
		::DeleteObject( m_hPatternA );		// [Bug9400]で追加
		::DeleteObject( m_hPatternB );		// [Bug9400]で追加
		::DeleteObject( m_hPatternC );		// [Bug9400]で追加
		::DeleteObject( m_hPatternD );		// [Bug9400]で追加
		::DeleteObject( m_hPatternE );		// [Bug9400]で追加
		::DeleteObject( m_hPatternF );		// [Bug9400]で追加
		::DeleteObject( m_hPatternA2 );		// [Bug9400]で追加
		::DeleteObject( m_hPatternB2 );		// [Bug9400]で追加
		::DeleteObject( m_hPatternC2 );		// [Bug9400]で追加
		::DeleteObject( m_hPatternD2 );		// [Bug9400]で追加
		::DeleteObject( m_hPatternE2 );		// [Bug9400]で追加
		::DeleteObject( m_hPatternF2 );		// [Bug9400]で追加
		::DeleteObject( m_hDir_Working );	// [Bug9403]で追加
		::DeleteObject( m_hDir_0 );	// [Bug9403]で追加
		::DeleteObject( m_hDir_45 );// [Bug9403]で追加
		::DeleteObject( m_hDir_90 );// [Bug9403]で追加
		::DeleteObject( m_hDir_135 );// [Bug9403]で追加
		::DeleteObject( m_hDir_180 );// [Bug9403]で追加
		::DeleteObject( m_hDir_225 );// [Bug9403]で追加
		::DeleteObject( m_hDir_270 );// [Bug9403]で追加
		::DeleteObject( m_hDir_315 );// [Bug9403]で追加
		::DeleteObject( m_hDir_Null );// [Bug9403]で追加

		::DeleteObject( m_hFont );
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg メッセージマップ
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
ALT_MSG_MAP(IDC_BUTTON_PATTERN_A)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_B)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_C)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_D)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_E)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_PATTERN_F)						// [Bug9400]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_WORKING)						// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_0)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_45)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_90)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_135)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_180)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_225)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_270)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_315)							// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_BUTTON_DIR_NULL)						// [Bug9403]で追加
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()
/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_bInitializing = TRUE;
		m_bOnInitDlg = TRUE;
		m_bInitialCheckedLaneConsistency = FALSE;
		// プロパティをレジストリから読み込み
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
		// コントロール
		CreateControlRelation();
		// サブクラス化
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
		m_wndPatternA.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_A ) );		// [Bug9400]で追加
		m_wndPatternB.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_B ) );		// [Bug9400]で追加
		m_wndPatternC.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_C ) );		// [Bug9400]で追加
		m_wndPatternD.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_D ) );		// [Bug9400]で追加
		m_wndPatternE.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_E ) );		// [Bug9400]で追加
		m_wndPatternF.SubclassWindow( GetDlgItem( IDC_BUTTON_PATTERN_F ) );		// [Bug9400]で追加
		m_wndDir_Working.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_WORKING ) );	// [Bug9403]で追加
		m_wndDir_0.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_0 ) );	// [Bug9403]で追加
		m_wndDir_45.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_45 ) );	// [Bug9403]で追加
		m_wndDir_90.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_90 ) );	// [Bug9403]で追加
		m_wndDir_135.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_135 ) );	// [Bug9403]で追加
		m_wndDir_180.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_180 ) );	// [Bug9403]で追加
		m_wndDir_225.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_225 ) );	// [Bug9403]で追加
		m_wndDir_270.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_270 ) );	// [Bug9403]で追加
		m_wndDir_315.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_315 ) );	// [Bug9403]で追加
		m_wndDir_Null.SubclassWindow( GetDlgItem( IDC_BUTTON_DIR_NULL ) );	// [Bug9403]で追加

		// フォント設定
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
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
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

				// <null> の場合を考慮
				if( lstrcmp( lpszText, NULL_VALUE ) != 0 )
				{
					CString strLanePassable = ConvertToLanePassageStr( lpszText );
					if( strLanePassable.Compare( lpszText ) != 0 )
					{
						INT dwSelStart = 0, dwSelEnd = 0;
						edit.GetSel( dwSelStart, dwSelEnd );
						edit.SetWindowText( strLanePassable );
						// 入力位置の修正
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
		// [Bug 5139]OnInitDlgの前に、メッセージがきてしまい、ConvertToLRChangeに入って
		// エラーメッセージがでてしまうため、ダイアログ作成されたかどうかを確認する
		if( !m_bOnInitDlg )
			return 0;

		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			ConvertToLRChange( wID, hWndCtl );
		else
			bHandled = FALSE;

		return 0;
	}

	/**
	 * @brief 「左n右m」のいずれかのボタンが押された時の処理
	 * @note	[Bug9400]対応時に、他の箇所でも使うためにOnButtonClick()の一部を関数化。
	 * @param[in]	cControlID	押されたボタンのコントロールID
	 * @retval	false	「左n右m」系ではないコントロールIDが指定されたので処理失敗
	 * @retval	true	処理成功（「左N右N」系のコントロールIDが指定されれば必ず成功する）
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
		// 増減値表示しなおし
		TCHAR strChange[10];
		::SetWindowText( GetDlgItem( IDC_EDIT_LANELCHANGE ), _ltot( lLChange, strChange, 10 ) );
		::SetWindowText( GetDlgItem( IDC_EDIT_LANERCHANGE ), _ltot( lRChange, strChange, 10 ) );
		// レーンのボタン作り直し
		CreateLaneButton();
		// 再描画
		SetButton( Changed() );

		return true;
	}

	/**
	 * @brief 角度により決定したLaneDir_C（前・右・左のいずれか）を返す
	 * @note	[Bug9400]対応時に、他の箇所でも使うためにOnButtonClick()の一部を関数化。
	 * @param[in]	cAngle	角度
	 * @return	LaneDir_C
	 */
	long LaneDirFromAngle( const long cAngle ) const
	{
		using namespace lane_dir_alias;

		// [bug 2040] 流出第一リンクとの角度で判定するように修正
		if      (160 <= cAngle && cAngle <= 200) { return kStraight; }	// 前
		else if ( cAngle < 160 )                 { return kRight;    }	// 右
		else                                     { return kLeft;     }	// 左
	}

	/**
	 * @brief 方向自動入力ボタンが押された時の処理
	 * @note	[Bug9400]対応時に、他の箇所でも使うためにOnButtonClick()の一部を関数化。
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
					LONG lVal = LaneDirFromAngle( it->m_lAngle );	// [Bug9400]対応時に関数化

					// 値をセット
					pFieldDef->SetNewValue( lVal, m_hWnd );
				}
				else
					_ASSERTE( pFieldDef );
			}
			else
				_ASSERTE( pFeatDef );
		}
		// 方向ビットマップセット
		SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// ボタン再描画
		SetButton( Changed() );
	}

	LRESULT OnButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
		if( m_lLastLink == 1 )
			return 0;

		if (! m_bInitializing)
		{
			switch (wID)
			{
				// 増減用ボタン
				case IDC_BUTTON_L1R0:
				case IDC_BUTTON_L0R0:
				case IDC_BUTTON_L0R1:
					if (! ClickedLnRmButton( wID ))		// [Bug9400]対応時に関数化
						{ return 0; }
					break;
				// 自動方向決定ボタン
				case IDC_BUTTON_AUTODIR:
					ClickedAutoDirButton();				// [Bug9400]対応時に関数化
					break;
				// パターン入力ボタン（[Bug9400]で追加）
				case IDC_BUTTON_PATTERN_A:
				case IDC_BUTTON_PATTERN_B:
				case IDC_BUTTON_PATTERN_C:
				case IDC_BUTTON_PATTERN_D:
				case IDC_BUTTON_PATTERN_E:
				case IDC_BUTTON_PATTERN_F:
					SetControlsByPattern( wID );
					break;
				// レーン方向入力ボタン（[Bug9403]で追加）
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
	 * @brief レーン方向ボタンの入力を反映させる [Bug9304]で追加
	 * @param[in]	cControlID	押されたボタンのコントロールID
	 */
	void SetDirectionByPattern( const WORD cControlID )
	{
		CComVariant	vaNewVal;	// 新しい方向
		switch(cControlID)
		{
			case IDC_BUTTON_DIR_WORKING:  // 未調査・作業中
				vaNewVal = 0;
				break;
			case IDC_BUTTON_DIR_0:        // 前方向(0)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k0;
				break;
			case IDC_BUTTON_DIR_45:       // 右斜め前方向(45)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k45;
				break;
			case IDC_BUTTON_DIR_90:       // 右方向(90)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k90;
				break;
			case IDC_BUTTON_DIR_135:      // 右斜め後ろ方向(135)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k135;
				break;
			case IDC_BUTTON_DIR_180:      // 後ろ方向(180)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k180;
				break;
			case IDC_BUTTON_DIR_225:      // 左斜め後ろ方向(225)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k225;
				break;
			case IDC_BUTTON_DIR_270:      // 左方向(270)
				vaNewVal = sindy::schema::inf_lane::lane_dir::k270;
				break;
			case IDC_BUTTON_DIR_315:      // 左斜め前方向(315)
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
				// 値をセット
				pFieldDef->SetNewValue( vaNewVal, m_hWnd );
			}
			else
				_ASSERTE( pFieldDef );
		}
		else
			_ASSERTE( pFeatDef );

		// 方向ビットマップセット
		SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// ボタン再描画
		SetButton( Changed() );
	}


	/**
	 * @brief 押されたパターン入力ボタンに応じて、他のコントロール群に値をセットする（[Bug9400]で追加）
	 * @note	・左右「増減」と「直近車線数」は、好き勝手に変えると不整合が起こるので、既存の処理のみの組み合わせによって状態を変える。
	 *			・「レーン方向」は、「方向自動入力」ボタンが押された時と同じ処理を行うことで変える。
	 *			・「車線通行状態」を変える処理は既存ではなかったので直接変える処理を追加。変更しても他への悪影響はないと判断。
	 * @param[in]	cControlID	押されたボタンのコントロールID
	 */
	void SetControlsByPattern( const WORD cControlID )
	{
		// 左右増減と直近車線数の値を変更する
		switch (cControlID)
		{
			case IDC_BUTTON_PATTERN_A:
			case IDC_BUTTON_PATTERN_E:
			case IDC_BUTTON_PATTERN_F:
				if (! ClickedLnRmButton( IDC_BUTTON_L0R0 )) { ATLASSERT(0); return; }	// 「L0R1」は押せない場合があるので、まずいつでも押せる「L0R0」を押した事にする
				LaneAppChanged( TRUE, 2 );												// 「左0、右0」であれば、直近を2にしても問題ない（はず）
				if (! ClickedLnRmButton( IDC_BUTTON_L0R1 )) { ATLASSERT(0); return; }	// 「直近2、左0、右0」であれば、「L0R1」を押しても問題ない（はず）
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

		// 方向自動入力ボタンが押された時と同じ処理を行い、LaneDir_Cを更新する
		ClickedAutoDirButton();

		// 同グループに属する各オブジェクトの車線通行状態を変更する
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
		// 被選択オブジェクトの車線通行状態の値をダイアログに反映
		const CString aCurrentLanePassable = GetCurrentFeatureDef()->GetFieldDef( m_strInfoTableName, sindy::schema::inf_lane::kLanePassable )->m_vaValue.bstrVal;
		OnChangeLanePassable( aCurrentLanePassable );

		// レーンのボタン群を作り直す
		CreateLaneButton();
		// 再描画
		SetButton( Changed() );
	}

	/**
	 * @brief レーンのパターンとレーン方向コードに応じて、「車線通行状態」に設定すべき文字列を返す（[Bug9400]で追加）
	 * @note	staticメンバ関数にしてあるのは、メンバ変数と関係ない事を明示するため。
	 * @param[in]	cControlID	レーンの当該パターンに対応するボタンのコントロールID
	 * @param[in]	cLaneDir	レーン方向コード（当該パターンに含まれる方向のうち、どの方向か）
	 * @return	「車線通行状態」に設定すべき文字列（決定できなかった場合は空文字列）
	 */
	static CString LanePassableForPattern( const WORD cControlID, const long cLaneDir )
	{
		using namespace lane_dir_alias;

		typedef WORD ControlID;
		typedef long LaneDir;
		typedef CString LanePassableStr;
		typedef std::map< std::pair< ControlID, LaneDir >, LanePassableStr > LanePassableTable;	///< 「パターン＋方向」と「車線通行状態」の対応表

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
			ATLASSERT(0);		// 対応表にない組み合わせが指定されるのはおかしい
			return CString();
		}
		return it->second;
	}

	LRESULT OnLaneButtonClick(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
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

					// オーナ名もちゃんとつけて渡さないといけない
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

			// 流入ノードIDの値が変更されたときのみ流入ノードIDも渡す
			LPTSTR lpszInNodeID = _T("-1");
			if( wID == IDC_COMBO_INNODEID )
			{
				CComboBox comboNode( GetDlgItem( IDC_COMBO_INNODEID ) );
				INT nIndexNode = comboNode.GetCurSel();
				lpszInNodeID = (LPTSTR)_alloca( ( comboNode.GetLBTextLen( nIndexNode ) + 1 ) * sizeof(TCHAR) );
				comboNode.GetLBText( nIndexNode, lpszInNodeID );
			}

			LONG lOID = ChangeINLinkAndNodeID( _ttol( lpszInLinkID ), _ttol( lpszInNodeID ) );

			// オーナ名もちゃんとつけて渡さないといけない
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
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
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
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
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
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
			SetLaneDirectionBitmap( GetCurrentFeatureDef() );

		// ボタン再描画
		SetButton( Changed() );

		bHandled = FALSE;

		return 0;
	}
	LRESULT OnDeltaposSpin(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		// [Bug3106]対応により、終端リンクのみの選択の時にはなにもできないようにする
		if( m_lLastLink == 1 )
			return 0;
		if( !m_bInitializing )
		{
			LPNMUPDOWN lpnmud = (LPNMUPDOWN)pnmh;
			if( lpnmud != NULL )
			{
				CUpDownCtrl cSpin = GetDlgItem( idCtrl );							// スピンコントロール
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


	// モーダルの時にキーを捕まえる
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

	/**
	 * WM_COMMAND 用イベントハンドラ
	 *
	 * 各コモンコントロールで変更があった場合、ここで処理します
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// SetCurrentFeatureDefIndex() の最中はキャンセル（タイミングによってはおかしくなるため）
		if( !m_bFinishInit ) return 0;

		INT msg = HIWORD(wParam);						// 操作メッセージ
		INT	nTargetControl = (INT)LOWORD(wParam);		// 操作対象コントロール
		// エディットボックスの時に全選択にする
		if( msg == EN_SETFOCUS )
			SelectDlgItem( nTargetControl );
		// コンボボックスのリストボックスの長さを調節
		if( msg == CBN_DROPDOWN )
			SetComboboxList( nTargetControl );

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) 
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		bHandled = FALSE;
		return 0;
	}

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg メンバ関数定義
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
	 * @brief 増減の「左」または「右」の値が変更された時の処理
	 * @param[in]	wID			変更された側に対応するエディットコントロールのコントロールID
	 * @param[in]	bUseArg		nLRChangeの値が有効であるか否か
	 * @param[in]	nLRChange	変更後の値
	 * @return	無意味（常にTRUE）
	 */
	BOOL LRChanged( INT wID, BOOL bUseArg = FALSE, INT nLRChange = -100 );

	/**
	 * @brief 「直近車線数」の値が変更された時の処理
	 * @param[in]	nNewLaneApp		nLRChangeの値が有効であるか否か
	 * @param[in]	nNewLaneApp	変更後の値
	 * @return	無意味（常にTRUE）
	 */
	BOOL LaneAppChanged( BOOL bUseArg = FALSE, INT nNewLaneApp = 0 );

	/**
	 * @brief パターン入力ボタンを有効化すべきか否かを判定する（[Bug9400]で追加）
	 * @param[in]	cControlID	パターン入力ボタンのコントロールID
	 * @return	判定結果
	 */
	bool AvailabilityOfPatternButton( const long cControlID ) const;

	/**
	 * @brief パターン入力ボタン群の状態（Enabled/Disabledと画像）を適切に設定する（[Bug9400]で追加）
	 * @param[in]	cIsEditable	編集可能かどうか
	 */
	void SetStateOfPatternButtons( bool cIsEditable );

/////////////////////////////////////////////////////////////////////////////
//
// CLQAttrLaneDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	CLoadQueueInfo			m_cLQInfo;
	CContainedWindow		m_wndLaneInLink, m_wndLaneInNode, m_wndLaneApp, m_wndLaneRChange, m_wndLaneLChange, m_wndLanePass, m_wndLaneGroup;
	CContainedWindow		m_wndLaneBtn1, m_wndLaneBtn2, m_wndLaneBtn3, m_wndSource;
	CContainedWindow		m_wndPatternA, m_wndPatternB, m_wndPatternC, m_wndPatternD, m_wndPatternE, m_wndPatternF;	///< パターン入力ボタン（[Bug9400]で追加）
	CContainedWindow		m_wndDir_0, m_wndDir_45, m_wndDir_90, m_wndDir_135, m_wndDir_180, m_wndDir_225, m_wndDir_270, m_wndDir_315, m_wndDir_Null, m_wndDir_Working;	///< [Bug9403]で追加
	HBITMAP					m_hStraightBmp, m_hStraightGBmp, m_hRightTurnBmp, m_hRightTurnGBmp, m_hLeftTurnBmp, m_hLeftTurnGBmp;
	HBITMAP					m_hAllLaneBitmap;
	HBITMAP					m_hDirection, m_hDirectionN, m_hDirectionNE, m_hDirectionE, m_hDirectionSE, m_hDirectionS, m_hDirectionSW, m_hDirectionW, m_hDirectionNW, m_hDirectionNull;
	HBITMAP					m_hPatternA, m_hPatternB, m_hPatternC, m_hPatternD, m_hPatternE, m_hPatternF;		///< パターン入力ボタン（Enabled時）用の画像（[Bug9400]で追加）
	HBITMAP					m_hPatternA2, m_hPatternB2, m_hPatternC2, m_hPatternD2, m_hPatternE2, m_hPatternF2;	///< パターン入力ボタン（Disabled時）用の画像（[Bug9400]で追加。Enabled用の画像を、ボタンをDisabledにする事によってグレースケール化すると意味の分からないアイコンになるので、Disabled時専用の画像を用意した。）
	HBITMAP					m_hDir_0, m_hDir_45, m_hDir_90, m_hDir_135, m_hDir_180, m_hDir_225, m_hDir_270, m_hDir_315, m_hDir_Null, m_hDir_Working; // [Bug9403]で追加
	list<CLevelOID> 		m_listSameINFeature;	//!< 同じ流入リンクを持つレーン格納用
//	CSize					m_cLaneStateSize;
	BOOL					m_bInitializing;			//!< OnInitDialog 中は TRUE
	BOOL					m_bOnInitDlg;
	HFONT					m_hFont;

	CString					m_strOldLChange, m_strOldRChange, m_strOldAppLane;	//!< 右左折増減及び直近車線数文字の保持
	static CString			m_strInfoTableName;
	static CString			m_strAppLaneFieldName;
	static CString			m_strLanePassFieldName;
	static CString			m_strRChangeFieldName;
	static CString			m_strLChangeFieldName;

	// プロパティ
	BOOL					m_bInitialCheckedLaneConsistency;	//!< ダイアログを開いたときに一度だけチェックをするためのフラグ
	BOOL					m_bCheckLaneConsistency;	//!< レーンの整合性を見るかどうか
	BOOL					_m_bCheckLaneConsistency;	//!< 整合性を見るかどうかを一度しか聞いてこないようにするため
	BOOL					m_bCheckLaneConsistencyw;	//!< 整合チェックをしない場合に警告を出すかどうか（TRUE:出さない）
	BOOL					m_bLaneBitEditable;			//!< レーンのビット操作ができるかどうか
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
	long					m_lLastLink;
};

#endif //__LQATTRLANEDLG_H_
