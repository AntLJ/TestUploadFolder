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


#include "WinLib/Str2.h"
#include "TableDataGrid.h"

// 再描画の依頼を受けるためのメッセージ
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE"));

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrLaneLinkDlg
class CAttrLaneLinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_NODELIST, CAttrLaneLinkDlg>
{

private:
	/**
	* 編集ビューのモード
	*
	* 車線リンク属性を編集する際にどのコントロールを表示したらよいかの指針として使用します。
	* CRowsControlRelation::SetControl() の第３、第４引数として使用します。
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_LANE_KIHON,		//!< 車線リンクの基本属性（第１タブ）
		EDITVIEW_LANE_NODE,		    //!< 車線リンクのノード列属性（第２タブ）
	};

public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrLaneLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

// [SiNDYChildDlg] メッセージマップ
ALT_MSG_MAP(IDC_EDIT_FROM_NODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_TO_NODE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROAD_LINK_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE_GROUP_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE_GROUP_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_ROAD_STRUCTURE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE_NUMBER)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_LANE_COUNT)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_MAX_LEGAL_SPEED_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_MIN_LEGAL_SPEED_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_TRAVEL_DIRECTION_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_ROAD_STRUCTURE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_RIGHT_CHANGE_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_RIGHT_PROTRUSION_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_RUBBING_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_TUNNEL_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP(IDC_TAB_LANEATTR)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	
END_MSG_MAP()

BEGIN_SINK_MAP(CAttrLaneLinkDlg)
	SINK_ENTRY(IDC_GRID_NODELIST, DISPID_MOUSEUP, OnGridMouseUp)
	SINK_ENTRY(IDC_GRID_NODELIST, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
	SINK_ENTRY(IDC_GRID_NODELIST, grid_util::disp_id::AfterEdit, OnAfterEdit)

END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid イベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
	/**
	* @brief   Before Edit
	* @param   Row     [in]  Row Num
	* @param   Col     [in]  Col Num
	* @param   Cancel  [in]  Cancel Code
	*/
	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);

	/**
	* @brief   MouseUp Event on Grid
	* @param   Button    [in]  Button Code
	* @param   Shift     [in]  Shift Code
	* @param   X         [in]  X Coordinate
	* @param   Y         [in]  Y Coordinate
	* @param   Cancel    [in]  Cancel Code
	*/
	void __stdcall OnGridMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);

	/**
	* @brief   After Edit
	* @param   Row     [in]  Row Num
	* @param   Col     [in]  Col Num
	*/
	void __stdcall OnAfterEdit(LONG Row, LONG Col);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール
		CreateControlRelation();

		// ノード列グリッドコントロール
		CAxWindow wndGrid = GetDlgItem(IDC_GRID_NODELIST);
		wndGrid.QueryControl(&m_nqLaneNodeGrid);

		// グリッドコントロールからのイベントを拾うのに必要
		AtlAdviseSinkMap(this, true);

		CString strTemp;  //!< pszTextに渡す文字列を一時的に格納する変数

		// 道路属性編集用タブコントロールにアイテムを追加します
		HWND hwndTab = GetDlgItem( IDC_TAB_LANEATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_LANE_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NODE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_LANE_NODE, (LPARAM)(&pTabCtrlItem));

		// サブクラス化
		for( auto& it : m_mapSubClass)
		{
			it.second.SubclassWindow( GetDlgItem( it.first ) );
		}

		::SendMessage(hwndTab, TCM_SETCURSEL, m_iChangeType, 0L);

		ChangeWindowMgrMap( m_iChangeType );

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

	/**
	 * IDC_EDIT_NAME 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 *
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
	 */
	LRESULT OnUpdateCrossName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strName = AheConvertToIntersectionName( lpszText, TRUE );

				if( strName.Compare( lpszText ) != 0)
				{
					INT dwSelStart = 0, dwSelEnd = 0;
					edit.GetSel( dwSelStart, dwSelEnd );
					edit.SetWindowText( strName );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strName );
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
	 * IDC_EDIT_YOMI 用イベントハンドラ
	 *
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 *
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
	 */
	LRESULT OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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
				CString strYomi = AheConvertToIntersectionName( lpszText, FALSE );
				if( strYomi.Compare( lpszText ) !=0 )
				{
					INT nSelStart = 0, nSelEnd = 0;
					edit.GetSel( nSelStart, nSelEnd );
					edit.SetWindowText( strYomi );
					// 入力位置の修正
					INT nDiff = CString_GetLength2( CString( lpszText ) ) - CString_GetLength2( strYomi );
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
	* @brief   WM_NOTIFY 用イベントハンドラ
	* @note 以下の処理を行います・タブの切り替えが発生した時
	*
	* @param   uMsg      [in]   Msg ID
	* @param   wParam    [in]   Control ID
	* @param   lParam    [in]   Control Event
	* @param   bHandled  [out]  The return of processing
	* @retval  1:成功  0:失敗
	*/	
	LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if( wParam == IDC_TAB_LANEATTR ) {		

			LPNMHDR pnmh = (LPNMHDR)lParam;
			if( pnmh->code == TCN_SELCHANGE ) {
				// コントロール切り替え
				m_iChangeType = TabCtrl_GetCurSel( pnmh->hwndFrom );				
				
				m_cControlRel.Visible( m_iChangeType );
				ChangeWindowMgrMap( m_iChangeType );
			
				ATTR_BASE_CLASS::SetCurrentFeatureDefIndex( m_lFeatureIndex, m_lTableIndex, m_lRowIndex, m_iChangeType, m_bForce, m_bEditable );
				
				bHandled = TRUE;
								
				return 1;
			}
		}
		
		return 0;
	}

	/**
	* @brief   WM_REFRESHFEATURE 用イベントハンドラ
	* @note    再描画イベント
	*
	* @param   uMsg      [in]   Msg ID
	* @param   wParam    [in]   Control ID
	* @param   lParam    [in]   Control Event
	* @param   bHandled  [out]  The return of processing
	*/	
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	* @brief   WM_DESTROY 用イベントハンドラ
	* @note    Tear down connection points
	*
	* @param   uMsg      [in]   Msg ID
	* @param   wParam    [in]   Control ID
	* @param   lParam    [in]   Control Event
	* @param   bHandled  [out]  The return of processing
	*/	
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg メンバ関数定義
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
	
	/**
	* @brief   Update Grid Data
	*
	* @retval  true:成功  false:失敗
	*/	
	virtual bool Update() override;

	/**
	* @brief   Check Data Changes
	*
	* @retval  true:changed  false:no changed
	*/
	bool Changed();

	CAttrLaneLinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	/**
	* @brief   Change Window Manage Map
	*
	* @param   iIndex      [in]   Window Index
	*/	
	void ChangeWindowMgrMap( int iIndex );
	void CreateControlRelation();
	void SetToolTip(INT nTargetItem);
	/**
	 * セグメント属性値の更新
	 */
	void UpdateLaneGroup();
	void GetRoadLinkID();

	/**
	 * コード値ドメイン名の設定
	 *
	 * @param	update	設定済みの場合に再取得するか
	 *
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool SetDomain(bool update = false);
	/**
	 * 未設定のOIDのコントロール更新
	 *
	 * セグメントOID、上下線リンクOID
	 */
	void UpdateUnDefOID();
		
	/**
	* @brief   グリッドを生成する
	*
	*/
	void createGrid();

	/**
	*  @brief  ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
	*  @note   グリッドで管理している関連テーブルの情報は、ControlRelで更新できないため、
	*          変更情報のフラグだけ更新する。フラグを更新しないとDBが更新されない
	*
	*/
	void updateChanged();

	/**
	*  @brief  右クリックすると <削除> ポップアップ
	*
	* @param   nID      [in]  Control ID
	* @param   ipGrid   [in]  Grid control
	* @param   x        [in]  X Coordinate
	* @param   y        [in]  Y Coordinate
	*/
	void gridMenu( int nID, const IVSFlexGridPtr& ipGrid, float x, float y);

	/**
	* @brief  グリッドで選択しているノード列のラインをハイライトする
	*
	*/
	void highlightLine();

	/**
	* @brief  get Point Geometry
	*
	* @param [out]  ipFromPoint       From Point Geometry
	* @param [out]  ipToPoint         To Point Geometry
	*/
	void getSequenceGeometry(IGeometryPtr& ipFromPoint, IGeometryPtr& ipToPoint);

	/**
	*  @brief  ノード列のジャンプ,中心へ移動
	*  @param [in] ipFromPoint        始点
	*  @param [in] ipToPoint          終点
	*/
	void moveToCenter(const IGeometryPtr& ipFromPoint, const IGeometryPtr& ipToPoint);

	/**
	* @brief  レジストリからデータを読み込み
	*
	*/
	void readReg();
/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneLinkDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass;	    //!< 各コントロールウィンドウ
	HFONT							m_hFont;		    //!< 文字フォント
	std::map<long, CString>         m_laneGroupDomain;	//!< 車線グループ属性コード値ドメイン(KEY: DomainCode, ELEM: DomainName)
	static const CString            UNDEF_NAME;			//!< 未設定時の表示名
	static const long               UNDEF_OID;			//!< 未設定のOID値

	IVSFlexGridPtr		            m_nqLaneNodeGrid;	//!< 車線リンク用のグリッド		
	
	std::map<long, TableDataGrid>   m_nqDataGridMap;	//!< ノード列グリッドコントロール (key: 車線ノードのOID, value: ノード列グリッド)
	long                            m_currentOid;       //!< 現在ダイアログに表示している車線ノードのOID

	long	                        m_lFeatureIndex;	//!< 車線リンクダイアログ側でのフィーチャインデックス保持用
	long	                        m_lTableIndex;		//!< 車線リンクダイアログ側でのテーブルインデックス保持用
	long	                        m_lRowIndex;		//!< 車線リンクダイアログ側でのRowインデックス保持用
	bool	                        m_bForce;			//!< 車線リンクダイアログ側での強制編集モード保持用
	bool	                        m_bEditable;		//!< 車線リンクダイアログ側での編集できるかどうか保持用
	long	                        m_lCurrentIndex;	//!< 車線ノード列の0カラム目色変更戻し用インデックス番号保持用
public:
	int					            m_iChangeType;		//!< コントロール切り替え用
	bool                            m_gridSelect;       //!< グリッドの選択フラグ
};
