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

// ControlDef.h: CControlDef クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLDEF_H__A2886331_0F3B_47E9_AD92_6F54DA8A0F14__INCLUDED_)
#define AFX_CONTROLDEF_H__A2886331_0F3B_47E9_AD92_6F54DA8A0F14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __SINDY_ROADQUEUEMODE__
#include "LQRowDef.h"
typedef CLQRowDef FEATUREDEF;
#else
#include "FeatureDef.h"
typedef CFeatureDef FEATUREDEF;
#endif

class CArcHelper;

// 制限タイプ
#define FOLLOW_NONE     0
#define FOLLOW_VALUE    1
#define FOLLOW_LIST     2
#define FOLLOW_STRING   4
#define FOLLOW_NOTSAME  8
#define FOLLOW_READONLY 16
#define FOLLOW_VALUEIS1 32
#define FOLLOW_VALUEREVERSE 64

// チェックタイプ
#define CHECK_NONE      0x00
#define CHECK_DISPNAVI  0x01
#define CHECK_DISPOTHER 0x02

/**
* @brief 登録するコントロールのタイプ
*/
enum CONTROLTYPE 
{
	TYPE_EDIT,			//!< エディット
	TYPE_EDITSTRING,	//!< 文字編集
	TYPE_CHECKBOX,		//!< チェックボックス
	TYPE_COMBO,			//!< コンボボックス
	TYPE_LIST,			//!< リストボックス
	TYPE_TIMEREG,		//!< 時間規制
	TYPE_OUTERROWS,		//!< TYPE_OUTERROWS
	TYPE_FEATURESGRID,	//!< TYPE_FEATURESGRID
	TYPE_FEATUREGRID,	//!< TYPE_FEATUREGRID
	TYPE_OTHER,			//!< TYPE_OTHER
	TYPE_NONE,			//!< TYPE_NONE
	TYPE_BUTTON,		//!< ボタン（中縮背景ポリゴン用）
	TYPE_OWTIMEREG,		//!< 一通時間規制
	TYPE_EDITFLOAT,		//!< FLOAT編集
	TYPE_EDITDOUBLE,	//!< DOUBLE編集
};


class CControlDef  
{
public:
	CControlDef();
	virtual ~CControlDef();

public:
	void SetControlDef( IApplication* ipApp, INT nID, HWND hWnd, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, LPCTSTR lpszFieldName = NULL, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );
	void SetControlDef( IApplication* ipApp, INT nID, HWND hWnd, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, std::list<CString> arrayFieldNames, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );

	void dump( LPCTSTR s = NULL );
	DOMAINMAP* GetDomain( LPCTSTR lpszFieldName = NULL );	//!< 固定値ドメインを返す
	void Reset();
	void Clear();
	inline void SetHWND( HWND hWnd ){ m_hWnd = hWnd; };
	void SetView();											//!< コントロールの表示状態をセット
	void GetStringValue( CString& str );
	BOOL GetOldValue( CComVariant& vaValue );
	void SetValue( CComVariant vaValue );
	BOOL GetControlValue( CComVariant& lVal );
	BOOL SetValueFromControl();
	void ChangeEnable( BOOL bEnable, BOOL bForce, BOOL bEditable );
	BOOL Changed();
	BOOL Errored();
	BOOL IsUnset();
	BOOL IsDispClass();
	BOOL IsNullable( LPCTSTR lpszFieldName = NULL );
	void SetComboBox( LONG lDispClass );
	void SetEdit();
	void SetFeaturesGrid( BOOL bMulti = FALSE );
	void SetFeatureGrid( BOOL bMulti = FALSE );
	inline void SetErrored(BOOL bIsError){m_bErrored = bIsError;};
	BOOL Click( LONG l1, LONG l2, BOOL bIsGridEditMode = FALSE );
	BOOL IsClickCelEditable( LONG l1, LONG l2 );
	void Edited( LONG l1, LONG l2 );
	void SetCheckBox();
	void SetOtherControl();
	void SetOuterRowsGrid();
	void SetLinkTimeRegGrid();

	LPCTSTR GetTableName();
	LPCTSTR GetFieldName();
	void SetValueToCtrl();								//!< コントロールに m_vaValue の値をセット
	void GetValueFromCtrl();							//!< コントロールに表示されている値を m_vaValue に格納

	void SetFeatureDefList( std::list<FEATUREDEF>* pFeatureDefList );
	void SetCurrentFeatureDefIndex( LONG lFeatIndex, LONG lTableIndex = -1, LONG lRowIndex = -1 );

	CFieldDef* GetFieldDef( LPCTSTR lpcszFieldName = NULL ); //!< CControlRel::ControlChanged() で使用するのでパブリックに
	inline void ClearFeatureDef(){ m_pFeatureDefList = NULL; m_pFeatureDef = NULL; };
private:
	void SetGridColor( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol, LONG r, LONG g, LONG b );
	void SetGridColorToRed( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol );
	void SetGridColorToDefault( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol );
	void SetGridColorToUnEdit( IVSFlexGridPtr spGrid, LONG lRow, LONG lCol );
	void va2str( VARIANT &va, CString& str );
	BOOL _Changed( FEATUREDEF* pFeatureDef );
	void _SetFeatureGrid( CFieldDef* pFieldDef, IVSFlexGridPtr spGrid, LONG lFieldCount );
	CString GetDateString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay);
	CString GetTimeString(LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin);
	CString GetPeriodString(LONG lSMonth, LONG lSDay, LONG lEMonth, LONG lEDay, LONG lSHour, LONG lSMin, LONG lEHour, LONG lEMin);
	CString l2str( LONG l );	//!< 一桁の数値の最初に0をつけて返す

public:
	INT					m_nID;				//!< コントロールID
	INT					m_type;				//!< コントロールのタイプ
	UINT				m_uiCheckType;		//!< チェックタイプ
	INT					m_nGroupFrom;		//!< 表示グループ（から）
	INT					m_nGroupTo;			//!< 表示グループ（まで）
	INT					m_nFollowCtrlID;	//!< 依存するコントロール指定用
	UINT				m_uiFollowBy;		//!< 依存タイプ
	FEATUREDEF*			m_pFeatureDef;		//!< CControlRel::ControlChanged() で使用するのでパブリックに
	BOOL				m_bAliasOrField;
	std::list<CString>		m_listFieldName;	//!< CTableEditDlg から使用したいので仕方なく public
	BOOL				m_bErrored;			//!< 表示されている値がエラーかどうかのフラグ（未調査・調査中などの場合）
	BOOL				m_bIsNotNo;
private:
	CArcHelper			m_cArcHelper;
	std::list<FEATUREDEF>*	m_pFeatureDefList;
	CString				m_strTableName;
	HWND				m_hWnd;
	LONG				m_lTableIndex;
	LONG				m_lRowIndex;

	ISiNDYRulePtr		m_ipRule;
};

#endif // !defined(AFX_CONTROLDEF_H__A2886331_0F3B_47E9_AD92_6F54DA8A0F14__INCLUDED_)
