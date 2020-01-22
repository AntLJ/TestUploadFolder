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
#include "InvolvedRoadLink.h"
#include "TableDataGrid.h"

// 再描画の依頼を受けるためのメッセージ
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); //!< 再描画命令を受けるためのメッセージ

/// 選択されている上下線リンクに紐付いている道路リンク－上下線リンク関連レコード
/// キー1:選択されている上下線リンクのOID, キー2:道路リンクのOID, 値:道路リンク－上下線リンク関連レコード
typedef std::map<long, std::map<long, _IRowPtr>> RelRoadLink_UpdownLink;
/// 紐付いている道路リンクを管理するマップ
/// キー1:選択されている上下線リンクのOID, キー2:道路リンクのOID, 値:TODO
typedef std::map<long, std::map<long, InvolvedRoadLink>> RoadLinkList;
/// InvolvedRoadLinkのマップ
/// キー1:OID, キー2:道路リンクのOID, 値:TODO
typedef std::map<long, InvolvedRoadLink> InvolvedRoadLinkMap;

typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrUpdownLinkDlg
class CAttrUpdownLinkDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_NODELIST, CAttrUpdownLinkDlg>
{
	
private:
	/**
	* 編集ビューのモード
	*
	* 編集する際にどのコントロールを表示したらよいかの指針として使用します。
	* CRowsControlRelation::SetControl() の第３、第４引数として使用します。
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_KIHON,		//!< 基本属性（第１タブ）
		EDITVIEW_NODE,		//!< ノード列属性（第２タブ）
	};

public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrUpdownLinkDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

// [SiNDYChildDlg] メッセージマップ

// タブキー入力をサブクラス化したコントロールに渡すためのマクロ
#define ALT_MSG_MAP_(id) \
	ALT_MSG_MAP(id)\
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	\
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

ALT_MSG_MAP_(IDC_EDIT_UPDOWN_GROUP_ID)
ALT_MSG_MAP_(IDC_EDIT_GROUP_C)
ALT_MSG_MAP_(IDC_EDIT_LANE_COUNT)
ALT_MSG_MAP_(IDC_EDIT_RIGHT_CHANGE)
ALT_MSG_MAP_(IDC_EDIT_LEFT_CHANGE)
ALT_MSG_MAP_(IDC_COMBO_MAX_LEGAL_SPEED_C)
ALT_MSG_MAP_(IDC_COMBO_MIN_LEGAL_SPEED_C)
ALT_MSG_MAP_(IDC_COMBO_TRAVEL_DIRECTION_C)
ALT_MSG_MAP_(IDC_COMBO_ROAD_STRUCTURE_C)
ALT_MSG_MAP_(IDC_COMBO_ONEWAY_C)
ALT_MSG_MAP_(IDC_CHECK_RUBBING_F)
ALT_MSG_MAP_(IDC_CHECK_TUNNEL_F)
ALT_MSG_MAP_(IDC_COMBO_SPECULATION_C)
ALT_MSG_MAP_(IDC_EDIT_SOURCE)

ALT_MSG_MAP(IDC_TAB_UPDOWNATTR)


END_MSG_MAP()

BEGIN_SINK_MAP(CAttrUpdownLinkDlg)
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
// CAttrUpdownLinkDlg メッセージハンドラ
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
		wndGrid.QueryControl(&m_nqUpdownNodeGrid);

		// グリッドコントロールからのイベントを拾うのに必要
		AtlAdviseSinkMap(this, true);

		CString strTemp;  //!< pszTextに渡す文字列を一時的に格納する変数

		// 道路属性編集用タブコントロールにアイテムを追加します
		HWND hwndTab = GetDlgItem( IDC_TAB_UPDOWNATTR );
		ATLASSERT( ::IsWindow( hwndTab ) );
		TC_ITEM pTabCtrlItem;
		pTabCtrlItem.mask = TCIF_TEXT;
		strTemp = AheLoadString(IDS_BASE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_KIHON, (LPARAM)(&pTabCtrlItem));
		strTemp = AheLoadString(IDS_NODE);
		pTabCtrlItem.pszText = const_cast<LPTSTR>(strTemp.GetString());
		::SendMessage(hwndTab, TCM_INSERTITEM, EDITVIEW_NODE, (LPARAM)(&pTabCtrlItem));

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
		if( wParam == IDC_TAB_UPDOWNATTR ) {
			
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

private:

/////////////////////////////////////////////////////////////////////////////
//
// CAttrUpdownLinkDlg メンバ関数定義
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
	virtual bool Update() override;
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrUpdownLinkDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

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

	/**
	* @brief   Check Data Changes
	*
	* @retval  true:changed  false:no changed
	*/
	bool Changed();

private:

	/**
	 * @brief  子ダイアログを実際に作成します
	 */
	void CreateControlRelation();

	/**
	 * @brief  ツールチップを作成します
	 * @param  nTargetItem  [in]  ツールチップを作成するコントロールアイテムのID
	 */
	void SetToolTip(INT nTargetItem);

	/**
	 * セグメント属性値の更新
	 */
	void UpdateGroupC();

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
	 * 道路リンクOID、セグメントOID、ペア上下線リンクOID
	 */
	void UpdateUnDefOID();

	/**
	 * @brief  初期化処理
	 */
	void init();

	/**
	 * @brief  TOCにある指定したテーブル名のITableを取得
	 * @param  lpcszName   [in]  テーブル名称
	 * @param  lpcszOwner  [in]  オーナー名称
	 * @return テーブル
	 */
	ITablePtr getSameTable(LPCTSTR lpcszName, LPCTSTR lpcszOwner);

	/**
	 * @brief  道路リンクを取得
	 * @return 道路リンク
	 */
	IFeatureClassPtr getRoadLinkFC();

	/**
	 * @brief  現在選択されているフィーチャに紐付く関連レコード(元値)を返却する
	 * @return 関連レコード(元値)
	 */
	std::map<long, _IRowPtr>& getCurrentRelRoadLinkOrg();

	/**
	 * @brief  現在選択されているフィーチャに紐付いている道路リンクの関連レコードを返却する
	 * @return 関連レコード(左側)
	 */
	InvolvedRoadLinkMap& getCurrentRelRoadLinkList();

	/**
	 * @brief  周辺検索を行った道路リンクの関連レコードを返却する
	 * @return 関連レコード(右側)
	 */
	InvolvedRoadLinkMap& getCurrentSearchedList();

	/**
	 * @brief  道路リンク－上下線リンク関連レコードに変更があったか
	 * @retval	true	変更あり
	 * @retval	false	変更なし
	 */
	bool isChangeRefTbl();

	/**
	 * @brief  道路リンク方向コードに変更があるか
	 * @param  oid  [in]  調べる関連レコードのOID
	 * @retval	true	変更あり
	 * @retval	false	変更なし
	 */
	bool isChangeDirectionC(long oid);

	/**
	 * @brief  コード値ドメインを取得する
	 * @param  ipMap           [in]  コード値ドメインを取得するIMap
	 * @param  lpcszTableName  [in]  コード値ドメインを取得するテーブル
	 * @param  lpcszFieldName  [in]  コード値ドメインを取得するフィールド
	 * @return
	 */
	std::map<CString,long> getDomainCodeValue( IMap* ipMap ,LPCTSTR lpcszTableName, LPCTSTR lpcszFieldName );


	/**
	* @brief   Change Window Manage Map
	*
	* @param   iIndex      [in]   Window Index
	*/	
	void ChangeWindowMgrMap( int iIndex );

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
// CAttrUpdownLinkDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:

	IFeatureClassPtr m_roadLinkFeatureClass;		//!< 道路リンクのフィーチャークラス

	std::map<int, CContainedWindow> m_mapSubClass;  //!< 各コントロールウィンドウ
	HFONT							m_hFont;		//!< 文字フォント
	std::map<long, CString> m_segAttrDomain;		//!< セグメント属性コード値ドメイン(KEY: DomainCode, ELEM: DomainName)
	static const CString UNDEF_NAME;				//!< 未設定時の表示名
	static const long UNDEF_OID;					//!< 未設定のOID値

	IGeometryPtr m_ipSelectedLine;					//!< 紐付いている道路リンクリストで選択された道路リンク
	IGeometryPtr m_ipSearchedSelectedLine;			//!< 周辺検索された道路リンクリストで選択された道路リンク

	RelRoadLink_UpdownLink m_relRoadLinkOrgs;		//!< 選択されている上下線リンクに紐付いている道路リンク－上下線リンク関連レコード
													//   キー:OID 値:道路リンク－上下線リンク関連レコード
	RoadLinkList m_relRoadLinkLists;				//!< 編集対象オブジェクトごとの上下線リンクに紐付いている道路リンク
	RoadLinkList m_searchedRoadLinkLists;			//!< 編集対象オブジェクトごとの周辺検索を行った道路リンク

	std::map<long, CString> m_GIDMap;				//!< 上下線リンクのGID変換マップ キー:OID 値:GID

	std::map<CString,long> m_directionCDomain;		//!< 道路リンク方向コードのドメイン値

	IVSFlexGridPtr		            m_nqUpdownNodeGrid;	//!< 上下線リンク用のグリッド		
	std::map<long, TableDataGrid>   m_nqDataGridMap;	//!< ノード列グリッドコントロール (key: 上下線ノードのOID, value: ノード列グリッド)
	long                            m_currentOid;       //!< 現在ダイアログに表示している上下線ノードのOID

	long	                        m_lFeatureIndex;	//!< 上下線リンクダイアログ側でのフィーチャインデックス保持用
	long	                        m_lTableIndex;		//!< 上下線リンクダイアログ側でのテーブルインデックス保持用
	long	                        m_lRowIndex;		//!< 上下線リンクダイアログ側でのRowインデックス保持用
	bool	                        m_bForce;			//!< 上下線リンクダイアログ側での強制編集モード保持用
	bool	                        m_bEditable;		//!< 上下線リンクダイアログ側での編集できるかどうか保持用
	long	                        m_lCurrentIndex;	//!< 上下線ノード列の0カラム目色変更戻し用インデックス番号保持用
public:
	int					            m_iChangeType;		//!< コントロール切り替え用
	bool                            m_gridSelect;       //!< グリッドの選択フラグ
};
