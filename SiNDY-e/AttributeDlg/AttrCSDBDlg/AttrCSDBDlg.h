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

// AttrCSDBDlg.h : CAttrCSDBDlg の宣言

#pragma once

#include "resource.h"

typedef CAttrBaseDlg ATTR_BASE_CLASS;

/////////////////////////////////////////////////////////////////////////////
// CAttrCSDBDlg
class CAttrCSDBDlg : 
	public ATTR_BASE_CLASS,
	public IDispEventImpl<IDC_GRIDCTRL, CAttrCSDBDlg>, // デフォルトのグリッドコントロール
	public CExportDlg
{
public:
/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg メッセージマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MSG_MAP(CAttrCSDBDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	CHAIN_MSG_MAP(ATTR_BASE_CLASS)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
END_MSG_MAP()

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg メッセージハンドラ
//
/////////////////////////////////////////////////////////////////////////////

	/**
	 * ダイアログ作成する際に一番最初に呼ばれます。ここでダイアログの初期化をしてください
	 */
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		// コントロール作成
		CreateControlRelation();

		// デフォルト用グリッドコントロール
		CAxWindow wndGrid = GetDlgItem(IDC_GRIDCTRL);
		wndGrid.QueryControl(&m_spGrid);
		wndGrid.SetFocus();		

		// グリッドコントロールからのイベントを拾うのに必要
		AtlAdviseSinkMap(this, TRUE);

		ATTR_BASE_CLASS::OnInitDialog( uMsg, wParam, lParam, bHandled );

		return 1;  // システムにフォーカスを設定させます
	}

	/**
	 * ダイアログが破棄されたときに呼ばれます。
	 */
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		AtlAdviseSinkMap(this, FALSE);

		return 0;  // システムにフォーカスを設定させます
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg イベントシンクマップ
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_SINK_MAP(CAttrCSDBDlg)
	///Make sure the Event Handlers have __stdcall calling convention
	SINK_ENTRY(IDC_GRIDCTRL, 0x5,	OnBeforeMouseDown)
	SINK_ENTRY(IDC_GRIDCTRL, 48,	OnSetupEditStyle)
	SINK_ENTRY(IDC_GRIDCTRL, 0x12,	OnAfterMoveColumn)
END_SINK_MAP()

/////////////////////////////////////////////////////////////////////////////
//
// VS-FlexGrid イベントハンドラ
//
/////////////////////////////////////////////////////////////////////////////
public:
	void __stdcall OnSetupEditStyle( long Row, long Col, VARIANT_BOOL IsCombo, long * Style, long * StyleEx )
	{
		*Style |= ES_READONLY;
	}

	void __stdcall OnBeforeMouseDown(SHORT Button, SHORT Shift, FLOAT X, FLOAT Y, VARIANT_BOOL * Cancel)
	{
		LONG lCol = 0, lRow = 0;
		m_spGrid->get_MouseCol( &lCol );
		m_spGrid->get_MouseRow( &lRow );
	}
	/**
	 * 標準グリッドで列が移動された後に発生するイベントハンドラ
	 */
	VOID __stdcall OnAfterMoveColumn  ( long Row, long Col )
	{
		RefreshGrid();
	}

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg メンバ関数定義
//
/////////////////////////////////////////////////////////////////////////////
public:
	virtual HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL);
	virtual void Delete();
	virtual void SetArcHelper( IApplication* ipApp ){ ATTR_BASE_CLASS::SetArcHelper( ipApp ); };
	virtual void SetAliasOrField( BOOL bAliasOrField );
	virtual void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList ){};
	virtual void SetFeatureDefList( std::list<CFeatureDef>* pFeatureDefList );
	virtual void ClearFeatureDefs(){ ATTR_BASE_CLASS::ClearFeatureDefs(); };
	virtual HWND GetDlg(){ return m_hChildWnd; };
	virtual LRESULT SendMessage( UINT message, WPARAM wParam, LPARAM lParam ){ return ::SendMessage( m_hWnd, message, wParam, lParam ); }; 
	virtual BOOL ErrorCheck(){ return TRUE; };
	virtual LONG GetTabNum(){ return -1; };
	virtual void SetTabNum(LONG lTabNum){};
	virtual void SetFeatureClassName(LPCTSTR lpcszFeatureClassName){};

	CAttrCSDBDlg();
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable );
	inline BOOL CheckReturnKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckReturnKeyItem(nClassID); };
	inline BOOL CheckEscKeyItem(INT nClassID) { return ATTR_BASE_CLASS::CheckEscKeyItem(nClassID); };


private:
	void CreateControlRelation();
	/** 
	* @fn		HRESULT CreateActiveXControls(UINT nID);
	*
	* @brief	FlexGridのライセンスウィンドウを出さないようにするための関数
	*
	* @note		CAxDialogImpl::CreateActiveXControlsをオーバーライドしActiveXControl作成の際に
	*			FlexGridのキーを無理くり当てている
	*/
	HRESULT CreateActiveXControls(UINT nID);

	void CreateAliasMap( _IRowPtr ipRow );

	void RefreshGrid()
	{
		long lCols = -1;
		m_spGrid->get_Cols( &lCols );
		m_spGrid->put_AutoSizeMode( flexAutoSizeColWidth );
		if( lCols > 1 )		// 「属性名称」部分は自動リサイズしない
			m_spGrid->AutoSize( 1, lCols-1 );
		m_spGrid->put_AutoSizeMode( flexAutoSizeRowHeight );
		if( lCols > 1 )		// 「属性名称」部分は自動リサイズしない
			m_spGrid->AutoSize( 1, lCols-1 );
	};

	/** 
	* @fn		void SetHiddenRow();
	*
	* @brief	<Null>値の行は非表示状態にする
	*
	* @note		決まったグループ単位で非表示のコントロールを行う
	*			
	*/
	void SetHiddenRow();
	void _SetHiddenRow(_IRowPtr ipRow, std::list<CString>& strList);

/////////////////////////////////////////////////////////////////////////////
//
// CAttrCSDBDlg メンバ変数定義
//
/////////////////////////////////////////////////////////////////////////////
private:
	IVSFlexGridPtr		m_spGrid;			//!< グリッド表示用
	ISiNDYRulePtr		m_ipRule;			//!< 作業目的DB対応
	HWND				m_hChildWnd;		//!< 子ダイアログハンドル
	LONG				m_lPosition;
	std::list<CFeatureDef> m_lstDmyFeatureDef;
	std::map< CString, CString > m_mapDB2Alias;
	std::map< CString, CString > m_mapAlias2DB;
};

