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

/*! \file ControlRel.h
	\brief CControlRel クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#if !defined(AFX_CONTROLREL_H__13DE4468_C7EF_4180_AE6B_B4BDE19152DF__INCLUDED_)
#define AFX_CONTROLREL_H__13DE4468_C7EF_4180_AE6B_B4BDE19152DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlDef.h"
/*! \class CControlRel
	\brief CControlDefのリストクラス

	\b Example:

	@see CRowsControlRel::SetFeature()
*/
class CControlRel : public std::list<CControlDef>
{
public:
	CControlRel();
	virtual ~CControlRel();

	inline void SetArcHelper( IApplication* ipApp ){ ATLASSERT(ipApp != NULL); m_cArcHelper.Init( (IApplicationPtr)ipApp); }
	inline void SetHWND( HWND hWnd ){ ATLASSERT(hWnd != NULL); m_hWnd = hWnd; }
	void ResetHWND( HWND hWnd );

	void SetCurrentFeatureDefIndex( LONG lFeatureIndex, LONG lTableIndex = -1, LONG lRowIndex = -1 );

	void SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList );

	inline std::list<FEATUREDEF>* GetFeatureDefList(){ return m_pFeatureDefList; };
	void SetControl( INT nID, CONTROLTYPE type = TYPE_OTHER, INT nGroupFrom = 0, INT nGroupTo = 0, LPCTSTR lpszFieldName = NULL, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );
	void SetControl( INT nID, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, std::list<CString> arrayFieldNames, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );

	void ControlChanged( INT nID );
	void Visible( INT nGroup, BOOL bForce = FALSE, BOOL bEditable = TRUE );
	CControlDef* GetControlDef( HWND hWnd );
	BOOL ClickOtherControl( INT nID, LONG l1, LONG l2, BOOL bIsGridEditMode = FALSE);
	BOOL IsClickCelEditable( INT nID, LONG l1, LONG l2 );
	void EditedOtherControl( INT nID, LONG l1, LONG l2 );
	void ResetFeatures();
	void Clear();
	void ClearFeatureDefs();
	void SetValue( CControlDef* def, BOOL bForce = FALSE, BOOL bEditable = TRUE );
	BOOL CheckError();
private:
	void dump();
	void ChangeVisible( CControlDef* def, BOOL bVislble = TRUE );
	BOOL GetValue( INT nID, LONG& lVal );
	LONG GetDispClass();
	BOOL GetControlValue( CControlDef* def, LONG& lVal );
public:
	void SetAliasOrField(BOOL bAliasOrField);
private:
	CArcHelper			m_cArcHelper;		//!< ArcObjects ヘルパークラス
	list<FEATUREDEF>*	m_pFeatureDefList;	//!< セットされるフィーチャのリスト
	HWND				m_hWnd;				//!< 管理するコントロールが乗っているダイアログのHWND

	ISiNDYRulePtr	m_ipRule;			//!< ルールオブジェクト
};

#endif // !defined(AFX_CONTROLREL_H__13DE4468_C7EF_4180_AE6B_B4BDE19152DF__INCLUDED_)
