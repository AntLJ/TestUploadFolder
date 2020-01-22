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

#pragma once

#include "RelativeLinks.h"


typedef CAttrBaseDlg ATTR_BASE_CLASS;
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< 再描画命令を受けるためのメッセージ
/////////////////////////////////////////////////////////////////////////////
// CAttrHeightNode
class CAttrHeightNode : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrHeightNode)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER( IDC_COMBO_HEIGHT1, CBN_SELCHANGE, OnHeightChange )
	COMMAND_HANDLER( IDC_COMBO_HEIGHT2, CBN_SELCHANGE, OnHeightChange )
	COMMAND_HANDLER( IDC_COMBO_HEIGHT1, CBN_SETFOCUS, OnHeightFocus )
	COMMAND_HANDLER( IDC_COMBO_HEIGHT2, CBN_SETFOCUS, OnHeightFocus )
	COMMAND_HANDLER( IDC_COMBO_HEIGHT1, CBN_KILLFOCUS, OnHeightKillFocus )
	COMMAND_HANDLER( IDC_COMBO_HEIGHT2, CBN_KILLFOCUS, OnHeightKillFocus )
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)

ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HEIGHT1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_HEIGHT2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
// 以下のコントロールのサブクラス化がないとなぜか落ちるようになったので追加
ALT_MSG_MAP(IDC_COMBO_LAYER1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LAYER2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ID1)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ID2)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode メッセージハンドラ
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
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}


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
		if( ( msg == CBN_SELCHANGE ) || ( msg == BN_CLICKED ) || ( msg == EN_UPDATE )  ) {
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );
			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

private:
	
	/**
	 * @brief 再描画命令を受け取ったときの処理
	 */
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	/**
	 * @brief 高さが変更されたときの処理
	 */
	LRESULT OnHeightChange(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief フォーカスを取得したときの処理
	 */
	LRESULT OnHeightFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief フォーカスが外れたときの処理
	 */
	LRESULT OnHeightKillFocus(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * @brief コントロール背景色変更
	 * @note 本来の役割ではないが、論理チェックエラーがある際に「OK」を潰すのに使用する。
	 */
	LRESULT OnCtlColorEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode メンバ関数定義
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
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrHeightNode();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();
	/**
	* @brief 歩行者種別をエディットボックスに反映する
	* @note 歩行者種別は自前で管理する必要がある。
	*/
	void SetWalkclassToCtrl();
	
	/**
	* @brief m_relativeLinksで管理する値を書くコントロールに反映する
	* @note 新規の場合は作り直す必要があるので結局必要。。
	*/
	void ApplyToCtrl();

	/**
	* @brief 高さのコンボボックスを作る
	* @note リストはベタ書き
	* @param id [in] コントロールID
	* @return height [in] 作成と同時に選択しておく高さ（-2 ～ 2）
	*/
	void CreateHeightCombo( UINT id, long height );
	
	/**
	* @brief コントロールの有効・無効
	* @note 高さ以外はeditingによらず常に編集不可にする
	* @param editing [in] 編集開始しているか
	*/
	void EnableControls( bool editing );

	/**
	* @brief 「OK」ボタンの有効・無効
	* @param changed [in] 有効にするならTRUE
	*/
	void SetButton( BOOL changed );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrHeightNode メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ
	CRelativeLinks m_relativeLinks;    //<! 交差する2リンクを管理する 
	linkNo::eCode m_focusedHeight;     //<! フォーカスされているリンク（ID1 or ID2 の記憶用）
	bool m_forceCancel;                //<! 不正な場所に作成された場合に保存させない用
};
