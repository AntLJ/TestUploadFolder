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

// AttrWalkLinkDlgDlg.h : CAttrWalkLinkDlgDlg の宣言

#ifndef __ATTRWALKLINKDLG_H_
#define __ATTRWALKLINKDLG_H_

#include "resource.h"
#include "../../../LIB/WinLib/str2.h"
#include "../../../LIB/WinLib/charutil.h"
#include "BuddyRoadMgr.h"
#include "Option.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< 再描画命令を受けるためのメッセージ


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
	* 編集ビューのモード
	*
	* CRowsControlRelation::SetControl() の第３、第４引数として使用します。
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_WALK_KIHON,		//!< 歩行者リンクの基本属性（第1タブ）
		EDITVIEW_WALK_KISEI,		//!< 歩行者リンクの規制属性（第2タブ）
	};
/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlgDlg メッセージマップ
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

// タブキー入力をサブクラス化したコントロールに渡すためのマクロ
#define ALT_MSG_MAP_(id) \
ALT_MSG_MAP(id)\
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	\
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

// 基本タブ分
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

// 規制タブ分
ALT_MSG_MAP_(IDC_COMBO_ONEWAY)
ALT_MSG_MAP_(IDC_COMBO_NOPASSAGE)

ALT_MSG_MAP_(IDC_EDIT_SOURCE)
ALT_MSG_MAP_(IDC_TAB_WALKLINK)

END_MSG_MAP()

#undef ALT_MSG_MAP_

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();

		// サブクラス化
		for( auto& subclass : m_mapSubClass)
		{
			subclass.second.SubclassWindow( GetDlgItem( subclass.first ) );
		}

		// タブコントロールの作成
		CTabCtrl cTab( GetDlgItem( IDC_TAB_WALKLINK ) );
		cTab.AddItem( AheLoadString( IDS_BASE ) );
		cTab.AddItem( AheLoadString( IDS_REGULATION ) );

		return ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );
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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE ) ) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// [bug 5552] 2次メッシュ境界線を跨いでいる歩行者リンクの場合は反対側のリンクも同じ属性に
			SyncChangedAttrToOverMeshLink( nTargetControl );
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}
		return 0;
	}


	/**
	 * WM_NOTIFY 用イベントハンドラ
	 *
	 * 以下の処理を行います
	 * ・タブの切り替えが発生した時
	 */
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if( IDC_TAB_WALKLINK == wParam )
		{
			InvalidateRect( NULL, FALSE );//再描画
			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( TCN_SELCHANGE == pnmh->code )
			{
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );	// コントロールの切り替え

				m_cControlRel.Visible(m_iChangeType);
				ChangeWindowMgrMap( m_iChangeType );
				m_lTabNum = m_iChangeType;				// タブ位置保存

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
				// ダウンコンバートのためのコントロール表示・非表示
				SetControlState();

				bHandled = TRUE;
				return 1;
			}
		}

		if( 0 == wParam )
		{
			InvalidateRect( NULL, FALSE );//再描画
			HWND hwndTab = GetDlgItem( IDC_TAB_WALKLINK );
			if( -2 == lParam || -1 == lParam )
			{
				// コントロール切り替え
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
				m_lTabNum = m_iChangeType;	// タブの位置保存
				SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_bForce, m_bEditable );

				// ダウンコンバートのためのコントロール表示・非表示
				SetControlState();
				bHandled = TRUE;
				return 1;
			}
		}

		return 0;
	}


	// FLOOR_LEVELがFLOAT値を取るための特別対応
	// いまのところは、このダイアログにしかないため、AttrBaseDlgにはいれず、ここで預かっておく
	// もし、他にも、同じようにFLOAT値を取るようなものが出てきた場合にはAttrBaseDlgに入れて
	LRESULT OnUpdateFloorLevelEdit(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		// [bug 5552] 2次メッシュ境界線を跨いでいる歩行者リンクの場合は反対側のリンクも同じ属性に
		// FLOOR_LEVELの変更をOnCommandにも通知してSyncChengedAttrToOverMeshLink()を促すため
		bHandled = FALSE;

		InvalidateRect( NULL, FALSE );
		if( ::IsWindow( hWndCtl ) )
		{
			CEdit edit( hWndCtl );
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> の場合を考慮
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

					// 不正文字を除いた文字列を作成
					// 1文字目はマイナスも許可
					if( i==0 )
					{
						if( ( isasciiminus( lpstr[0] ) || isasciinumber( lpstr[0] ) ) )
							strRet += A2CT( lpstr );
					}
					// 2文字目以降は数字、またはコンマのみ許可
					else
					{
						if( isasciinumber( lpstr[0] ) )
						{
							// 小数点以下は1までしかいれられない。
							if( !bAfterDecimalPoint )
								strRet += A2CT( lpstr );
							if( bIsComma )
								bAfterDecimalPoint = true;
						}
						else if( isasciiperiod( lpstr[0] ) )
						{
							// コンマは1つしかいれられない
							if( !bIsComma )
								strRet += A2CT( lpstr );
							bIsComma = true;
						}
					}
				}

				// 不正文字が含まれていた場合
				int lCheckLen = CString_GetLength2( strRet );
				if( iLen != lCheckLen )
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// 入力位置の修正
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
	 * @brief walk_code検索」ボタンが押された時の処理
	 * @note このイベントに関してはCControlDef等でまかなえる処理ではないので、ここで処理
	 */
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		if( wID == IDC_BUTTON_SEARCHROADCODE )
			SetRoadName(IDC_EDIT_ROADCODE, IDC_STATIC_ROADNAME2);
		return 0;
	}
	
	/**
	 * @brief 再描画命令を受け取ったときの処理
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	/**
	 * @brief 周辺検索ボタン
	 */
	LRESULT OnClickedArroundSearch(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief バディ追加ボタン
	 */
	LRESULT OnClickedAddBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief バディ削除ボタン
	 */
	LRESULT OnClickedRemoveBuddy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief リストボックスの選択変更
	 */
	LRESULT OnListSelChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief リストアイテムダブルクリック
	 */
	LRESULT OnListDblClk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief リストボックスの色変更
	 */
	LRESULT OnCtlColorListBox(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg メンバ関数定義
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
	 * @brief	ダウンコンバートのためのコントロールの表示・非表示を判断する
	 *
	 * @note	ダウンコンバート版が完了したら、この関数はなくなるべき
	 */
	void SetControlState();

	/**
	 * @brief	ウインドウマップ変更
	 * @param	nIndex	[in]	タブインデックス
	 */
	void ChangeWindowMgrMap( INT nIndex );

	/**
	 * @brief	属性値の変更を2次メッシュを跨いだ反対側の歩道リンクにも適用し、同期させます。
	 *          コントロールが操作され、値が変更されるタイミングで呼び出します。
	 *
	 * @param	nId	[in]	変更されたフィールドに対応するコントロールのID
	 */
	void SyncChangedAttrToOverMeshLink( INT nId );

	/**
	* @brief 路線検索を行い、路線名を表示する
	*
	 * 路線名称テーブルである WALK_CODE_LIST から検索し、第2引数で示されるコントロールに表示します
	*
	 * @param nCodeID	[in]	歩行者路線コードが入っているエディットコントロールのID
	 * @param nLabelID	[in]	歩行者路線名称を表示するスタティックコントロールのID
	*/
	void SetRoadName(INT nCodeID, INT nLabelID );

	/**
	* @brief 道路紐付け初期処理
	*
	* LINK_RELATIONテーブルから、既に紐付いている道路リンクを探し、リストボックスに表示します
	*/
	void InitBuddyRinks();
	
	/**
	* @brief リンク紐付けテーブルフラグ設定
	*
	* 紐付くBuddyが1つ以上あれば1をセット。
	* 1つもなければ0をセット。
	*/
	void SetLinkRF();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrWalkLinkDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	HWND		m_hChildWnd;				//!< 子ダイアログハンドル

	LONG		m_lTabNum;					//!< タブコントロール位置
	INT			m_iChangeType;				//!< コントロール切り替え用

	LONG		m_lFeatureIndex;			//!< フィーチャインデックス保持用
	LONG		m_lTableIndex;				//!< テーブルインデックス保持用
	LONG		m_lRowIndex;				//!< Rowインデックス保持用
	BOOL		m_bForce;					//!< 強制編集モード保持用
	BOOL		m_bEditable;				//!< 編集できるかどうか保持用
	
	
	bool			m_bIsLinkOnMesh;		//!< 歩行者リンクがメッシュを跨いでいるかどうかのフラグ
	CTargetItems	m_cOverMeshLinks;		//!< メッシュを跨いでいる歩行者リンクのFeatureDef保持用

	CBuddyRoadMgr m_buddyRoads;             //!< バディ関係管理用
	COption       m_cProperty;              //!< レジストリ設定値

};

#endif //__ATTRWALKLINKDLG_H_
