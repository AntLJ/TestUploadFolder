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

/*! \file RowDef.h
	\brief CRowDef クラスのインターフェース
	\author INCREMENTP CORP. R&D技術センター第４開発グループ 古川貴宏
	\date 2002-03-05
*/
#if !defined(AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_)
#define AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FieldDef.h"

/*!	\class CRowDef 
	\brief _IRow のラッパークラス

	このクラスは _IRow と一対一の関係になります。
	ただし、使用しやすいようにいくつかのメンバ変数を追加してあります。
	また、このクラスを元に CFeatureDef と RelationalTableDef が定義されています。
	
	通常はこのクラスをそのまま使用することはなく、常に派生したクラスを使用することになります。
 */
class CRowDef : public std::list<CFieldDef>
{
public:
	CRowDef();
	virtual ~CRowDef();

	void Clear();
	void Reset();
	BOOL Store( BOOL bDoOperation );
	BOOL Changed();
	LPCTSTR GetTableName();
	void SetDeleteFlag( BOOL bDelete = TRUE );
	void SetCreateFlag();
	void SetRow( _IRowPtr ipRow );
	void SetTable( ITablePtr ipTable );
	LONG GetRowOID();
	inline _IRowPtr GetRow(){ return m_ipRow; };				//!< このクラスの大元である _IRow を返す
	inline BOOL IsDeleted(){ return m_bDeleted; };
	CFieldDef* GetFieldDef( LPCTSTR lpszFieldName );
	inline BOOL GetDeleteFlag(){ return m_bDeleteFlag; };	//!< このレコードが削除対象かどうかを示す m_bDeleteFlag を返す
	inline BOOL GetCreateFlag(){ return m_bCreateFlag; };	//!< このレコードが新規作成対象かどうかを示す m_bCreateFlag を返す
protected:
	void Delete();
	void Create();
protected:
	CString m_strTableName;			//!< m_ipTable のテーブル名
	_IRowPtr m_ipRow;				//!< 元になる _IRow インターフェースポインタ
	ITablePtr m_ipTable;			//!< m_ipRow が存在する ITable インターフェースポインタ
	BOOL m_bDeleteFlag;				//!< 外部テーブルでレコードを削除する場合、このフラグを立てる
	BOOL m_bCreateFlag;				//!< 外部テーブルにレコードを追加する場合、このフラグを立てる
	BOOL m_bDeleted;				//!< Delete() の後は必ずこのフラグが立つ
};

#endif // !defined(AFX_ROWDEF_H__C7B7C414_7D23_4C0E_864A_80AE6CE75C9C__INCLUDED_)
