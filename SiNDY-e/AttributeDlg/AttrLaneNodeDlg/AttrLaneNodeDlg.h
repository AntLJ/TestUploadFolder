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
// CAttrLaneNodeDlg
class CAttrLaneNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_NQ, CAttrLaneNodeDlg>
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneNodeDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrLaneNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

// [SiNDYChildDlg] メッセージマップ
ALT_MSG_MAP(IDC_COMBO_NODECLASS_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SEQUENCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_STOP_POINT_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_CHECK_COVERING_F)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_SPECULATION_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE_ID)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnRefresh(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	/**
	 * IDC_EDIT_NAME 用イベントハンドラ
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
	 */
	LRESULT OnUpdateCrossName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	/**
	 * IDC_EDIT_YOMI 用イベントハンドラ
	 * ここで、文字入力のチェックをおこない、不適切な文字が入力された場合には
	 * 入力できないようにします。
	 * @see	SiNDY-e\AttributeDlg\AttrRoadNodeDlg を参考
	 */
	LRESULT OnUpdateCrossYomi(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);



BEGIN_SINK_MAP(CAttrLaneNodeDlg)
	SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::BeforeMouseDown, OnGridMouseDown)
	SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
	SINK_ENTRY(IDC_GRID_NQ, grid_util::disp_id::AfterEdit, OnAfterEdit)
END_SINK_MAP()

	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL * Cancel);
	void __stdcall OnGridMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneNodeDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual bool Update();
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
	

	CAttrLaneNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }
	BOOL Changed();

private:
	void CreateControlRelation();
	void SetToolTip(INT nTargetItem);

	/**
	 * グリッドを生成する
	 */
	void createGrid();

	/**
	*  @brief  ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
	*  @note   グリッドで管理している関連テーブルの情報は、ControlRelで更新できないため、
	*          変更情報のフラグだけ更新する。フラグを更新しないとDBが更新されない
	*/
	void updateChanged();

	/**
	*  @brief  グリッドで選択しているノード列のラインをハイライトする
	*/
	void highlightLine();

/////////////////////////////////////////////////////////////////////////////
//
// CAttrLaneNodeDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass;	//!< 各コントロールウィンドウ
	HFONT							m_hFont;		//!< 文字フォント
	std::map<long, CString> m_laneGroupDomain;		//!< 車線グループ属性コード値ドメイン(KEY: DomainCode, ELEM: DomainName)
	static const CString UNDEF_NAME;				//!< 未設定時の表示名
	static const long UNDEF_OID;					//!< 未設定のOID値

	// ノード列グリッドコントロール (key: 車線ノードのOID, value: ノード列グリッド)
	std::map<long, TableDataGrid> m_nqDataGridMap;	
	long m_currentOid;		//!< 現在ダイアログに表示している車線ノードのOID
};
