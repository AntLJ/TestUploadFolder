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

#include "RelNodeInfos.h"
#include "RelNodeGridController.h"

// 再描画の依頼を受けるためのメッセージ
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); 


typedef CAttrBaseDlg ATTR_BASE_CLASS;

// CAttrRoadSignPointDlg
class CAttrRoadSignPointDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_VSFLEXGRID_NODE, CAttrRoadSignPointDlg>
{
public:
	/**
	* @brief 編集ビューのモード
	* @detail CRowsControlRelation::SetControl() の第３、第４引数として使用します。
	* @see CreateControlRelation()
	*/
	enum EditView {
		EDITVIEW_ROADSIGN_KIHON,		//!< 道路標識ポイントの基本属性（第1タブ）
		EDITVIEW_ROADSIGN_CLASS,		//!< 道路標識ポイントの道路標識種別属性（第2タブ）
	};

public:
	CAttrRoadSignPointDlg();
	~CAttrRoadSignPointDlg() {};

// メッセージマップ
public:
	BEGIN_MSG_MAP(CAttrRoadSignPointDlg)
		MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
		COMMAND_HANDLER(IDC_BUTTON_SEARCH, BN_CLICKED, OnClickedSearchButton)

		COMMAND_ID_HANDLER(ID_MENUITEM_ADD, OnClickAddMenu)
		COMMAND_ID_HANDLER(ID_MENUITEM_DELETE, OnClickDeleteMenu)

		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		//MESSAGE_HANDLER(WM_NOTIFY, OnNotify)

		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
		CHAIN_MSG_MAP(ATTR_BASE_CLASS)

	ALT_MSG_MAP(IDC_EDIT_ROADSIGN_C)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_BUTTON_SEARCH)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	// 基本タブ分
	ALT_MSG_MAP(IDC_EDIT_HORIZONTAL_ANGLE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	ALT_MSG_MAP(IDC_CHECK_VARIABLE_F)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)

	ALT_MSG_MAP(IDC_EDIT_SOURCE)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CAttrRoadSignPointDlg)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, DISPID_MOUSEUP, OnMouseUpGrid)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, grid_util::disp_id::AfterEdit, OnAfterEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_NODE, 0x01, OnCellChangedGrid)
	END_SINK_MAP()


public:
	// メッセージイベントハンドラ
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

	// メニューの「追加」「削除」ボタンクリック
	LRESULT OnClickAddMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	
	// 再描画命令を受け取ったときの処理
	LRESULT OnRefresh( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );

public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual bool Update() override;
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField ){ ATTR_BASE_CLASS::m_cControlRel.SetAliasOrField( bAliasOrField ); };
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList ){ ATTR_BASE_CLASS::SetFeatureDefList( pFeatureDefList ); };
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hWnd; };
	virtual BOOL ErrorCheck();
	virtual LONG GetTabNum(){ return m_TabNum; };
	virtual void SetTabNum(LONG lTabNum){ m_TabNum = lTabNum;}
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){
		return ::SendMessage( m_hWnd, message, wParam, lParam );
	}; 

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID);
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }
	BOOL Changed();

private:
	void CreateControlRelation();

	/**
	*  @brief 道路標識ポイント側の変更状態と、ボタンの Enable/Disable 切り替え
	*/
	void UpdateChanged();

	/**
	*  @brief 道路標識ポイントと紐づくノードとを結ぶラインの描画
	*/
	void DrawLinkageLine();

	/**
	*  @brief ポリラインの描画
	*  @param[in] polyline  描画するポリライン
	*  @param[in] color     描画色
	*/
	void DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color);

	/**
	* @brief EditControl に入力された道路標識コードを検索し、ComboBox へセットする
	* @param[in]  editControlId   道路標識コードを入力するEditControl のID
	* @param[in]  comboBoxId      道路標識コードを設定するComboBox のID
	* @retval     true            セット成功
	* @retval     false           セット失敗
	*/
	bool SetRoadsignCodeToComboBox(INT editControlId, INT comboBoxId);

	/**
	 * @brief	ウインドウマップ変更
	 * @param	nIndex	[in]	タブインデックス
	 */
	//void ChangeWindowMgrMap( INT nIndex );

	///**
	// * @brief	<基本>タブに切り替えたときのコントロール制御
	// */
	//void ChangeCtrlEnableToKihonTab();

	///**
	// * @brief	<規制>タブに切り替えたときのコントロール制御
	// */
	//void ChangeCtrlEnableToKiseiTab();

	// VS-FlexGridイベントハンドラ
	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/) {};

private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	LONG		m_TabNum;					//!< タブコントロール位置
	INT			m_ChangeType;				//!< コントロール切り替え用

	LONG		m_FeatureIndex;				//!< フィーチャインデックス保持用
	LONG		m_TableIndex;				//!< テーブルインデックス保持用
	LONG		m_RowIndex;					//!< Rowインデックス保持用
	BOOL		m_IsForce;					//!< 強制編集モード保持用
	BOOL		m_Editable;					//!< 編集できるかどうか保持用

	long m_CurrentOid;			//!< 現在ダイアログに表示している道路標識ポイントのOID
	std::unordered_map<long, RelNodeInfos> m_RelNodes;	//!< 関連テーブル情報 

	RelNodeGridController m_Grid;	//!< 関連ノードグリッドの制御
};
