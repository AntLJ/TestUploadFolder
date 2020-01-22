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

/*! \file FeatureDef.h
	\brief CFeatureDef クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/

#if !defined(AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_)
#define AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RelationalTableDef.h"

class CArcHelper;

/*! \class CFeatureDef
	\brief IFeature のラッパークラス

	このクラスは IFeature と一対一の関係になります。
	ただし、SiNDY-b でリレーションが張られていない時を考慮して、独自にリレーションテーブルを管理する
	メンバを持っています。

	\b Example:

	CFeatureDef featDef;

	CFeatureDef オブジェクトを生成します。

	featDef.SetArcHelper( pHelper );

	CArcHelper をセットします。SetFeature() する前に必ず呼ぶ必要があります。

	featDef.SetFeature( ipFeature );

	元となる IFeature インターフェースオブジェクトをセットします。
	この関数を呼ぶと、関連テーブルやフィールドを全てセットします。
*/
class CFeatureDef : public CRowDef
{
public:
	CFeatureDef();
	virtual ~CFeatureDef();

	void SetArcHelper( IApplication* ipApp );
	void SetFeature( _IRowPtr ipFeature );
	_IRowPtr GetFeature();
	list<CRelationalTableDef>* GetRelationalTableDefList();
	CRelationalTableDef* GetRelationalTableDef( LPCTSTR lpcszTableName );
	void Store( BOOL bDoOperation );
	void Clear();
	void Reset();
	void SetWarnDelete(){ m_bWarnDelete = TRUE; };
	BOOL GetWarnDelete(){ return m_bWarnDelete; };
	BOOL Changed();
	CFieldDef* GetFieldDef( LPCTSTR lpszTableName, LPCTSTR lpszRFName );
private:
	/**
	* @brief  外部関連テーブル名、フィールド名を取得する
	* @param  strTableName       [in] 関連元テーブル名
	* @param  strFieldName       [in] 関連元フィールド名
	* @param  strOuterTableName  [in] 関連先テーブル名
	* @param  strOuterFieldName  [in] 関連先フィールド名
	* @retval true  取得成功
	* @retval FALSE 取得失敗
	*/
	bool getOuterTableAndFieldName( const CString& strTableName, const CString& strFieldName, CString& strOuterTableName, CString& strOuterFieldName);

public:
	BOOL						m_bError;
	sindyeEditableType			m_sindyeEditableType;
	CComBSTR					m_bstrIsEditableFeature;
private:
	CArcHelper					m_cArcHelper;			//!< CArcHelper クラスのポインタ

	std::list<CRelationalTableDef>	m_listRelationalTable;	//!< 関連する外部テーブルのリスト
	_IRowPtr						m_ipFeature;			//!< 元となる IFeature インターフェースオブジェクト
	BOOL						m_bWarnDelete;			//!< 警告レベルを解除するならTRUE
};

#endif // !defined(AFX_FEATUREDEF_H__26B30C3E_C99F_4F1A_B306_F9BAC9CAF460__INCLUDED_)
