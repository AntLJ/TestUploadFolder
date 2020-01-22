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

// AttrCDSCommonDlg.h : CAttrCdsCommonDlg の宣言

#ifndef __ATTRCDSCOMMONDLG_H_
#define __ATTRCDSCOMMONDLG_H_

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrCdsCommonDlg
class CAttrCdsCommonDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCdsCommonDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_SEARCH_MSNO, BN_CLICKED, OnClickedButtonSearchMSNO)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
		break;

	case IDC_EDIT_SOURCE:
	case IDC_COMBO_CLASS:
	case IDC_EDIT_MSNO:
	case IDC_BUTTON_SEARCH_MSNO:
	case IDC_CHECK_UNDERGROUND:
	case IDC_CHECK_CORRIDOR:
	case IDC_CHECK_NOWALL:
	case IDC_EDIT_BLDNAME:
	case IDC_EDIT_BLDADDR:
	case IDC_EDIT_GEOID:
	case IDC_EDIT_NAMESTRING1:
	case IDC_EDIT_NAMESTRING2:
	case IDC_COMBO_DISPTYPE:
	case IDC_CHECK_HIMAWARI:
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg メッセージハンドラ
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

		// 種別コードのコンボボックスの値がセルチェンジしたときに、
		// 原稿番号入力の値と注記記号を変える
		if( nTargetControl == IDC_COMBO_CLASS && msg == CBN_SELCHANGE )
		{
			ShowMSNOByClass();
			UpdateDispTypeList();
		}

		// 原稿番号入力の値が変わったときに、
		// 種別コードのコンボボックスの値と注記記号を変える
		if ( nTargetControl == IDC_EDIT_MSNO && msg == EN_KILLFOCUS && m_cArcHelper.IsStartEditing() )
		{
			SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS);
			UpdateDispTypeList();
		}

		// コンボボックス、エディットボックス、チェックボックスのメッセージ処理
		if( msg == CBN_SELCHANGE || msg == BN_CLICKED || msg == EN_UPDATE )
		{
			// 変更されたかどうかチェックし、変更されていたら他のコントロールにも反映させる
			m_cControlRel.ControlChanged( (INT)LOWORD(wParam) );

			// 注記文字列が変更された場合、文字数も更新する
			if( nTargetControl == IDC_EDIT_NAMESTRING1 || nTargetControl == IDC_EDIT_NAMESTRING2 )
				AutoUpdateStringNum( nTargetControl );

			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );

			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		// GEOSPACE_IDが変更されたとき
		if( nTargetControl == IDC_EDIT_GEOID && msg == EN_KILLFOCUS && m_cArcHelper.IsStartEditing() )
			SetGeospaceID();

		return 0;
	}

	/**
	 * 種別コード検索用イベントハンドラ
	 *
	 * 検索ボタンが押された際に、原稿番号を検索
	 * 現在はエディットボックス、チェックボックス、コンボボックスの処理が行われます
	 */
	LRESULT OnClickedButtonSearchMSNO(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		SetClassByMSNO(IDC_EDIT_MSNO, IDC_COMBO_CLASS);
		// 注記記号を更新
		UpdateDispTypeList();

		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg メンバ関数定義
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
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){m_strClassName = lpcszFeatureClassName;};

	CAttrCdsCommonDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID){return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID);};

private:
	void LoadBackgroundRule();
	CString GetMSNOByClass(LONG lClass){LoadBackgroundRule(); return ATTR_BASE_CLASS::GetMSNOByClass(lClass);};
	LONG GetClassByMSNO(CString strMSNO){LoadBackgroundRule(); return ATTR_BASE_CLASS::GetClassByMSNO(strMSNO);};
	void CreateControlRelation();
	BOOL SetClassByMSNO(INT nCodeID, INT nClassID);
	void ShowMSNOByClass();
	void GetClassID(LONG *lClassID);
	void SetGeospaceID();
	BOOL CheckAnnoNameString(CFeatureDef& featureDef, CString& strErr);
	void AutoUpdateStringNum(UINT nTargetControl);
	void UpdateDispTypeList();
	void SetComboBoxSel(INT nComboID, LONG lVal);
	BOOL CheckExceedFieldSize(CFeatureDef& featureDef, CString& strErr);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCdsCommonDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<INT, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	std::map<LONG, LONG> m_mapAnnoDispType; //!< 注記種別と注記記号の対応表

	std::set<CString> m_msNoExceptTblList; //!< 原稿番号入力対象外テーブル一覧
};

#endif // __ATTRCDSCOMMONDLG_H_
