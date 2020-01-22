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


#include "WinLib/str2.h"
#include "WinLib/charutil.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrPlacenameRepPointDlg
class CAttrPlacenameRepPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrPlacenameRepPointDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_EDIT_ELEVENCODE, EN_UPDATE, OnUpdateElevenCode)
	COMMAND_HANDLER(IDC_EDIT_NAME, EN_UPDATE, OnUpdateName)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

// メッセージマップ
ALT_MSG_MAP(IDC_EDIT_ELEVENCODE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_NAME)
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
// CAttrPlacenameRepPointDlg メッセージハンドラ
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

		// 11桁住所コードが初期値と同じ場合はOKボタンをDisableにする(Ctrl+Z対策)
		// TODO:このコードがないとCtrl+Zでエディットの文字を戻してもボタンがDisableにならない。（押しても実害はない）
		// TODO:このコードがあると「あるフィーチャを編集」→「ツリーで別のフィーチャを選択」→「OKを押す」でOnOKに行かないのでコメントアウト。
		/*
		if( nTargetControl == IDC_EDIT_ELEVENCODE){
			using namespace sindy::schema::placename_rep_point;
			CString strEdit;
			GetDlgItemText(nTargetControl, strEdit);
			CString strCityCode,strAddrCode;
			int codeLength = strEdit.GetLength();
			if(codeLength <= 5){
				strCityCode = strEdit;
				strAddrCode = _T("");
			}
			else{
				strCityCode = strEdit.Left(5);
				strAddrCode = strEdit.Right(codeLength - 5);
			}

			CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
			CString strInitCityCode,strInitAddrCode;
			strInitCityCode = getInitStr(pFeatureDef->GetFieldDef(kTableName, kCityCode));
			strInitAddrCode = getInitStr(pFeatureDef->GetFieldDef(kTableName, kAddrCode));
			if( strCityCode == strInitCityCode && strAddrCode == strInitAddrCode){
				InvalidateRect( NULL, FALSE );
				SetButton(FALSE);
			}
		}
		*/

		return 0;
	}

	/**
	 * 11桁住所コード用イベントハンドラ
	 *
	 * 11桁住所コードのエディットボックスで変更があった場合、ここで処理します
	 */
	LRESULT OnUpdateElevenCode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
		// 入力制御
		if( ::IsWindow( hWndCtl ) ){
			CEdit edit( hWndCtl );
			// 11桁コード用テキストバッファ
			INT nTextLen = edit.GetWindowTextLength() + 1;
			LPTSTR lpszText = (LPTSTR)_alloca( nTextLen * sizeof(TCHAR) );
			edit.GetWindowText( lpszText, nTextLen );

			// <null> の場合を考慮
			if( lstrcmp( lpszText, NULL_VALUE ) != 0 ){
				CString strText( lpszText );
				CString strRet;
				LONG lLen = CString_GetLength2( strText );

				// "0123456789ABCDEF"(半角)のみ入力を許可する
				for( LONG i = 0; i < lLen; i++ ){
					CString str = CString_GetAt2( strText, i );
					LPSTR lpstr = CT2A( (LPTSTR)((LPCTSTR)str) );

					if( ( isasciinumber( lpstr[0] ) ||
						( 0x41 <= (unsigned char)((lpstr[0])&0xff) &&
						(unsigned char)(lpstr[0]&0xff) <= 0x46 ) ) )
						strRet += CA2CT( lpstr );
				}

				LONG lCheckLen = CString_GetLength2( strRet );
				if( lLen != lCheckLen ){
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strRet );
					// 入力位置の修正
					INT nDiff = CString( lpszText ).GetLength() - strRet.GetLength();
					edit.SetSel( dwSelStart - nDiff, dwSelEnd - nDiff );
				}
			}
			m_cControlRel.ControlChanged( wID );
			InvalidateRect( NULL, FALSE );
		}

		using namespace sindy::schema::placename_rep_point;

		CString strElevenCode;
		GetDlgItemText(IDC_EDIT_ELEVENCODE, strElevenCode);

		// 「Grid表示->Common表示」でエディットボックスが一瞬空になり、
		// その時にこのイベントハンドラが呼び出される。
		// フラグで状態を管理して、値が書き換わるのを防ぐ。
		if(strElevenCode.IsEmpty() && m_isFirst){
			m_isFirst=false;
			return 0;
		}
		m_isFirst=false;

		CString strCityCode, strAddrCode;
		int codeLength = strElevenCode.GetLength();
		if(codeLength <= 5){
			strCityCode = strElevenCode;
			strAddrCode = _T("");
		}
		else{
			strCityCode = strElevenCode.Left(5);
			strAddrCode = strElevenCode.Right(codeLength - 5);
		}

		CFeatureDef* pFeatureDef = GetCurrentFeatureDef();
		CFieldDef* cityFieldDef = pFeatureDef->GetFieldDef( kTableName, kCityCode );
		CFieldDef* addrFieldDef = pFeatureDef->GetFieldDef( kTableName, kAddrCode );
		cityFieldDef->SetNewValue(CComVariant(strCityCode), hWndCtl);
		addrFieldDef->SetNewValue(CComVariant(strAddrCode), hWndCtl);

		if(cityFieldDef->m_bChanged || addrFieldDef->m_bChanged){
			InvalidateRect( NULL, FALSE );
			SetButton(TRUE);
		}

		return 0;
	}

	/**
	 * 代表点名称用イベントハンドラ
	 *
	 * 代表点名称のエディットボックスで変更があった場合、ここで処理します
	 */
	LRESULT OnUpdateName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){
		CString strName;
		CEdit edit( hWndCtl );
		INT nTextLen = edit.GetWindowTextLength() + 1;
		edit.GetWindowText( strName.GetBuffer(nTextLen), nTextLen );

		// 半角スペースのみの場合は空文字列を格納する
		if(strName == _T(" "))
			SetDlgItemText(IDC_EDIT_NAME, _T(""));

		m_cControlRel.ControlChanged( wID );
		SetButton( Changed() );
		InvalidateRect( NULL, FALSE);
		return 0;
	}

private:
	/**
	 * @brief  fieldDefから編集前の初期値を文字列で取得する
	 * @param  fieldDef [in] 編集前の初期値を取得したいフィールドのfieldDef
	 * @return 編集前の初期値（文字列）
	 */
	CString getInitStr(CFieldDef* fieldDef);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg メンバ関数定義
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
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrPlacenameRepPointDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrPlacenameRepPointDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ
	bool m_isFirst; //!< 初回であるか(true:初回, false:初回ではない)
};
