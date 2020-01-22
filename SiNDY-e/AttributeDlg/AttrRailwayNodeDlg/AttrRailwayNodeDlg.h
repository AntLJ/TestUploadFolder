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

#include <atlhost.h>

#include "LineInfoController.h"

#include <ArcHelperEx/Map.h>
#include <memory>


typedef CAttrBaseDlg ATTR_BASE_CLASS;
/////////////////////////////////////////////////////////////////////////////
// CAttrRailwayNodeDlg
class CAttrRailwayNodeDlg : 
	public ATTR_BASE_CLASS,
	public CExportDlg2,
	public IDispEventImpl<IDC_GRID_LINE_INFO, CAttrRailwayNodeDlg>,
	public IDispEventImpl<IDC_GRID_AROUND_STA, CAttrRailwayNodeDlg>
{
	
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrRailwayNodeDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_CTLCOLOREDIT, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORBTN, OnCtlColorEdit)
	MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorEdit)
	COMMAND_HANDLER(IDC_BUTTON_APPLY, BN_CLICKED, OnClickedApplyButton)
	COMMAND_ID_HANDLER(ID_MENUITEM_LINE_INFO_DELETE, OnClickDeleteMenu)
	MESSAGE_HANDLER(WM_COMMAND, OnCommand)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)

ALT_MSG_MAP(IDC_EDIT_SOURCE)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
ALT_MSG_MAP(IDC_COMBO_NODE_C)
	MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)	
	MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
END_MSG_MAP()

	// イベントハンドラ
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClickedApplyButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickDeleteMenu(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);


/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg シンクマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAttrRailwayNodeDlg)
	SINK_ENTRY(IDC_GRID_LINE_INFO, grid_util::disp_id::BeforeEdit, OnLineInfoBeforeEdit)
	SINK_ENTRY(IDC_GRID_LINE_INFO, grid_util::disp_id::AfterEdit, OnLineInfoAfterEdit)
	SINK_ENTRY(IDC_GRID_LINE_INFO, DISPID_MOUSEUP, OnLineInfoMouseUp)
	SINK_ENTRY(IDC_GRID_AROUND_STA, grid_util::disp_id::BeforeEdit, OnAroundStaBeforeEdit)
END_SINK_MAP()

	// 路線リストグリッドのイベントハンドラ
	void __stdcall OnLineInfoBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);
	void __stdcall OnLineInfoAfterEdit(LONG Row, LONG Col);
	void __stdcall OnLineInfoMouseUp(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y);
	// 周辺駅リストグリッドのイベントハンドラ
	void __stdcall OnAroundStaBeforeEdit(LONG Row, LONG Col, VARIANT_BOOL* Cancel);



/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg メンバ関数定義
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
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};
	virtual BOOL ErrorCheck() override;
	virtual bool Update();

	CAttrRailwayNodeDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	BOOL Changed();
	BOOL CheckReturnKeyItem(INT nClassID){ return TRUE; }
	BOOL CheckEscKeyItem(INT nClassID){ return TRUE; }

private:
	void CreateControlRelation();

	/**
 	 * @brief TOCから指定レイヤを取得
	 * @param layerName [in]     TOCから取得するレイヤ名
	 * @param stCount   [in,out] TOC上に存在するレイヤ名の数を取得
	 * @param ipMap     [in]     IMapPtr
	 */
	ILayerPtr getLayer( const CString& layerName, int& stCount, const IMapPtr& ipMap );

	/**
	 *	@brief  ボタンのEnable/Disableの更新、関連テーブルの更新情報の更新
	 *  @note   グリッドで管理している関連テーブルの情報は、ControlRelで更新できないため、\n
	 *          変更情報のフラグだけ更新する。フラグを更新しないとDBが更新されない
	 */
	void updateChanged();

	/**
	 *	@brief  ノード種別と関連テーブルの整合成チェック
	 *	@param  cGridMgr
	 */
	bool checkNodeClass( const CLineInfoController& cLineInfoCntr, const CComVariant& vaNodeType, long lOidi );


/////////////////////////////////////////////////////////////////////////////
//
// CAttrRailwayNodeDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	std::map<int, CContainedWindow> m_mapSubClass; //!< 各コントロールウィンドウ

	typedef std::map<long, CLineInfoController> LineInfoMap; 

	LineInfoMap m_LineInfoList;				//!< オブジェクトIDと関連テーブル制御グリッドのマップ
	long	m_CurrentOid;					//!< 現在表示中のフィーチャのOID

};
