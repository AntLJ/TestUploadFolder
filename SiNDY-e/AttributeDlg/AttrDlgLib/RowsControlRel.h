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

/*! \file RowsControlRel.h
	\brief CRowsControlRel クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#if !defined(AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_)
#define AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlRel.h"

/*! \class CRowsControlRel
	\brief コントロールとフィーチャ、フィールド、テーブルを関連付けるクラス

	\b Example:

	SetArcHelper( pHelper );

	ArcObjects ヘルパークラスのポインタを渡して初期化します。

	SetControl();

	先にコントロールを登録します。

	SetFeature();

	後でフィーチャを登録します。
*/
class CRowsControlRel  
{
public:
	CRowsControlRel();
	virtual ~CRowsControlRel();

public:
	void SetControl( INT nID, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, LPCTSTR lpszFieldName = NULL, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );
	void SetControl( INT nID, CONTROLTYPE type, INT nGroupFrom, INT nGroupTo, std::list<CString> arrayFieldNames, UINT uiCheckType = NULL, 
		LPCTSTR lpszTableName = NULL, LPCTSTR lpszOuterKey = NULL, INT nFollowCtrlID = -1, UINT uiFollowBy = 0x00, INT nRelType = 0 );

	void SetArcHelper( IApplication* ipApp );
	_IRowPtr SetFeatures( _ISetPtr ipSet );
	void SetFeature( IFeaturePtr ipFeature );
	_IRowPtr GetFeature( LONG lIndex );
	void Clear();
	void Reset();
	void Store( BOOL bDoOperation );
	BOOL Changed();
	CFeatureDef* GetCurrentFeatureDef();
	void SetView( LONG lFeatureIndex, LONG lTableIndex = -1, LONG lRowIndex = -1 );
public:
	CControlRel			    m_pControlRel;		//!< コントロールリスト保持クラス
	std::list<CFeatureDef>	m_listFeatureDef;	//!< フィーチャリスト保持クラス
	
private:
	CArcHelper			    m_cArcHelper;		//!< ArcObjects ヘルパークラスポインタ
	LONG				    m_lFeatureDefIndex;	//!< 現在ダイアログに表示されているフィーチャのインデックス番号保持用
};

#endif // !defined(AFX_ROWSCONTROLREL_H__343F14A0_BCF0_4BA7_ACFA_B0E09E3CAE6D__INCLUDED_)
