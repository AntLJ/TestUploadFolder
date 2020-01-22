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


typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrSJPolygonDlg
class CAttrSJPolygonDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrSJPolygonDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)

ALT_MSG_MAP(IDC_COMBO_LANDCOVER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_LANDUSE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_ROADFLAG)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg メッセージハンドラ
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

			// ROAD_FLAGの値が変更された場合に特殊対応
			if( m_strClassName == sindy::schema::sj::kRoadTableName && msg == BN_CLICKED && (INT)LOWORD(wParam) == IDC_CHECK_ROADFLAG )
			{
				CButton cCheck( GetDlgItem( IDC_CHECK_ROADFLAG ) );
				// 0x0003はオン、オフ状態を調べる（つまり、オンならtrue）
				if( cCheck.GetState() & 0x0003 )
				{
					// LAND_COVER=0でない場合は、0にする（変更がかかるので、m_bChenged=trueにしておく）
					if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_vaValue.lVal != 0 )
					{
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_vaValue = 0L;
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLCCLS )->m_bChanged = true;
					}
					// LAND_USE=0でない場合は、0にする（変更がかかるので、m_bChenged=trueにしておく）
					if( GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_vaValue.lVal != 0 )
					{
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_vaValue = 0L;
						GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kLUCLS )->m_bChanged = true;
					}
				}
				// ROAD_Fに変更がかかったので、LAND_COVER、LAND_USEのコンボボックスの状態変更
				SetRoadFlag( (bool)GetCurrentFeatureDef()->GetFieldDef( m_strClassName, sindy::schema::sj::kRoadF )->m_vaValue.lVal, true );
			}
			// ダイアログを更新領域に追加します
			InvalidateRect( NULL, FALSE );
			// 変更された場合はボタンを Enable に
			SetButton( Changed() );
		}

		return 0;
	}

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg メンバ関数定義
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
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){ m_strClassName = (CString)lpcszFeatureClassName;};

	CAttrSJPolygonDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();
	/**
	 * @brief ROAD_Fが変更されたときに、LCCLS、LUCLSの状態を設定する関数
	 *
	 * @note  ROAD_Fの値が1に変更される時は、LCCLS、LUCLSは0に初期化される
	 *        また、ROAD_Fの値が0に変更される時は、LCCLS、LUCLSの変更はないため、
	 *        ROAD_Fを変更しようとする場合にはどちらにしろLCCLS、LUCLSは0にしておけばよい
	 *
	 * @param bEnalbe	[in]  ROAD_Fの値
	 * @param bReset    [in]  LCCLS、LUCLSを初期化するかどうか
	 */
	void SetRoadFlag( bool bCheck, bool bReset );

/////////////////////////////////////////////////////////////////////////////
//
// CAttrSJPolygonDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ
};
