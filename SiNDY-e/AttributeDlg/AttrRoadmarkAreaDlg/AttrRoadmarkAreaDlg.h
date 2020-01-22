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

#include "RelLinkInfos.h"
#include "RelLinkGridController.h"

// 再描画の依頼を受けるためのメッセージ
const unsigned int WM_REFRESHFEATURE = RegisterWindowMessage(_T("WM_REFRESHFEATURE")); 


typedef CAttrBaseDlg ATTR_BASE_CLASS;

// CAttrRoadmarkAreaDlg
class CAttrRoadmarkAreaDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_VSFLEXGRID_LANE_LINK, CAttrRoadmarkAreaDlg>
{
public:
	CAttrRoadmarkAreaDlg();
	~CAttrRoadmarkAreaDlg() {};

// メッセージマップ
public:
	BEGIN_MSG_MAP(CAttrRoadmarkAreaDlg)
		MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
		MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
		COMMAND_ID_HANDLER(ID_MENUITEM_ADD, OnClickAddMenu)
		COMMAND_ID_HANDLER(ID_MENUITEM_DELETE, OnClickDeleteMenu)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_REFRESHFEATURE, OnRefresh)
		CHAIN_MSG_MAP(ATTR_BASE_CLASS)
	END_MSG_MAP()

	BEGIN_SINK_MAP(CAttrRoadmarkAreaDlg)
		SINK_ENTRY(IDC_VSFLEXGRID_LANE_LINK, DISPID_MOUSEUP, OnMouseUpGrid)
		SINK_ENTRY(IDC_VSFLEXGRID_LANE_LINK, grid_util::disp_id::BeforeEdit, OnBeforeEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_LANE_LINK, grid_util::disp_id::AfterEdit, OnAfterEdit)
		SINK_ENTRY(IDC_VSFLEXGRID_LANE_LINK, 0x01, OnCellChangedGrid)
	END_SINK_MAP()


public:
	// メッセージイベントハンドラ
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedSearchButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

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
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){}
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){
		return ::SendMessage( m_hWnd, message, wParam, lParam );
	}; 

	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL CheckReturnKeyItem(INT nClassID) { return CAttrBaseDlg::CheckReturnKeyItem(nClassID); };
	BOOL CheckEscKeyItem(INT nClassID){ return CAttrBaseDlg::CheckEscKeyItem(nClassID); }
	BOOL Changed();

private:
	void CreateControlRelation();

	/**
	*  @brief 道路標示エリア側の変更状態と、ボタンの Enable/Disable 切り替え
	*/
	void UpdateChanged();

	/**
	*  @brief 道路標示エリアから紐づく車線リンクへを結ぶラインの描画
	*/
	void DrawLinkageLine();

	/**
	*  @brief ポリラインの描画
	*  @param[in] polyline  描画するポリライン
	*  @param[in] color     描画色
	*/
	void DrawPolyLine(const IPolylinePtr& polyline, const DWORD& color);

	void __stdcall OnBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnAfterEdit(LONG Row, LONG Col);
	void __stdcall OnMouseUpGrid(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	void __stdcall OnCellChangedGrid(/*LONG Row, LONG Col*/) {};

private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	long m_currentOid;			//!< 現在ダイアログに表示している道路標示エリアのOID
	std::unordered_map<long, RelLinkInfos> m_relLinks;	//!< 関連テーブル情報 

	RelLinkGridController m_grid;	//!< 関連ノードグリッドの制御
};
