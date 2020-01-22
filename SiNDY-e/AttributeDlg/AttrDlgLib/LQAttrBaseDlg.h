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

// LQAttrBaseDlg.h : CLQAttrBaseDlg の宣言

#ifndef __LQATTRBASEDLG_H_
#define __LQATTRBASEDLG_H_
#include <atlhost.h>

#include <atlapp.h>                             // WTL（atlgdi.h、atlcrack.h）を使うため
#include <atlgdi.h>                             // CDC                 （WinMgrで使用）
#include <atlcrack.h>                           // メッセージ系
#include "LQRowDef.h"
#include "ControlRel.h"
#include "SizingFramework/include/WinMgr.h"

class CArcHelper;

#ifndef __DEFINE_WM_FEATURECHANGED__
#define __DEFINE_WM_FEATURECHANGED__
const unsigned int WM_FEATURECHANGED = RegisterWindowMessage(_T("WM_FEATURECHANGED"));	// フィーチャに変更があった場合
#endif
/////////////////////////////////////////////////////////////////////////////
// CLQAttrBaseDlg
class CLQAttrBaseDlg : 
	public CAxDialogImpl<CLQAttrBaseDlg>
{
public:
	CLQAttrBaseDlg( WINRECT* pWinMap, WORD resourceID );
	~CLQAttrBaseDlg();

	const int IDD;

BEGIN_MSG_MAP(CLQAttrBaseDlg)
	MESSAGE_HANDLER(WM_WINMGR, OnWinMgr)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
END_MSG_MAP()

	// 各種イベントハンドラ
	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnWinMgr(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCode(INT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL)
	{
		HWND hWnd = CAxDialogImpl::Create(hWndParent, dwInitParam);
		// 親ウィンドウハンドルを確保しておく
		m_hAttributeDlgWnd = hWndParent;
		return hWnd;
	}
	void SetFeatureDefList( std::list<CLQRowDef>* pFeatureDefList );
	virtual BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, BOOL bForce, BOOL bEditable ) = 0;
	virtual BOOL CheckReturnKeyItem(INT nClassID) = 0;
	virtual	BOOL CheckEscKeyItem(INT nClassID) = 0;
	virtual BOOL ErrorCheck() = 0;
	inline void SetArcHelper( IApplication* ipApp ){ m_cArcHelper.Init((IApplicationPtr)ipApp); m_cControlRel.SetArcHelper( ipApp ); };
	inline void SetAttributeDlgHWND( HWND hWnd ){ m_hAttributeDlgWnd = hWnd; };
	inline void ResetFeatures(){ m_cControlRel.ResetFeatures(); };
	inline void ClearFeatureDefs(){ m_cControlRel.ClearFeatureDefs(); };
	inline CString GetClassName(){ return m_strClassName; };
	inline void	SetAliasOrField( BOOL bAliasOrField ){ m_cControlRel.SetAliasOrField( bAliasOrField ); };
	BOOL CheckAllItemsError();

protected:
	BOOL SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex, LONG lRowIndex, INT view, BOOL bForce, BOOL bEditable );
	BOOL IsStartEditing();
	BOOL IsEditableFeature();
	BOOL Changed();
	BOOL SetButton( BOOL bChanged );
	CLQRowDef* GetCurrentFeatureDef();
	VOID SelectDlgItem(INT nClassID);
	void SetIMEMode(BOOLEAN bIsOn);
	void SetComboboxList(INT nClassID);
	void SetDlgStatus(INT nID, BOOL bIsEnable, BOOL bIsEditBox = FALSE);
	int AttrMessageBox( LPCTSTR lpcszMsg, LPCTSTR lpcszCap, UINT uType, UINT uLevel );
	ISiNDYRulePtr GetRule(){ if( m_ipRule == NULL ) m_ipRule.CreateInstance( CLSID_SiNDYRuleObj ); return m_ipRule; };

protected:
	WORD					 m_wResource;
	CWinMgr					 m_winMgr;
	CArcHelper				 m_cArcHelper;
	CString					 m_strClassName;
	ISiNDYRulePtr			 m_ipRule;
	CWindow					 m_wndTmp;
	CControlRel				 m_cControlRel;
	std::list<CLQRowDef>*	 m_pFeatureDefList;
	LONG					 m_lFeatureDefIndex;
	BOOL					 m_bFinishInit;
	HWND					 m_hAttributeDlgWnd;
	HBRUSH					 m_hChangedBrush, m_hErrorBrush,m_hBrushCombo;	//!< コモンコントロールの色変更用ブラシ
	long					 m_lComboR, m_lComboG, m_lComboB;
};

#endif //__LQATTRBASEDLG_H_
