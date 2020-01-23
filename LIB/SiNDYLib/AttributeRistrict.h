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

/**
 * @file AttributeRistrict.h
 * @brief <b>CAttributeRistrictクラス定義ファイル</b>\n
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _ATTRIBUTERISTRICT_H_
#define _ATTRIBUTERISTRICT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

class CRow;

class SINDYLIB_API CAttributeRistrictBase
{
public:
	CAttributeRistrictBase();
	virtual ~CAttributeRistrictBase();

	virtual bool SetRistrict( LPCTSTR lpcszRistrict );
	virtual sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const = 0;
protected:
	bool	m_bWarning;		//!< 警告フラグ
};

typedef boost::shared_ptr<CAttributeRistrictBase> CSPAttributeRistrictBase;

/**
 * @class CRistrict
 * @brief <b>属性制限値を保持する純粋仮想クラス</b>\n
 */
class CAttributeRistrict : public CAttributeRistrictBase
{
public:
	CAttributeRistrict();
	virtual ~CAttributeRistrict();
	/**
	 * @brief <b>属性制限文字列をパースする</b>\n
	 * この関数で処理するのはフィールド名、警告フラグのみで、その処理が
	 * 終了後に切り取った文字列を_SetRistrict関数に渡します。
	 * @param lpcszRistrict	[in]	属性制限文字列
	 * @retval true 文字列が適切に処理された
	 * @retval false 文字列が適切に処理されなかった
	 */
	virtual bool SetRistrict( LPCTSTR lpcszRistrict );

	/// 与えられたオブジェクトが編集可能かどうかチェック
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;
protected:
	/**
	 * @brief <b>与えられた値が編集可能な値かどうかをチェック</b>\n
	 * この関数は_IsEditable関数の処理に否定条件及び警告処理を追加する関数です。
	 * 実際の値比較には_IsEditable関数を使用します。
	 * @param vaValue	[in]	比較値
	 * @retval sindyeEditableType
	 */
	sindyeEditableType IsEditable( const VARIANT& vaValue ) const;

	/**
	 * @brief <b>属性制限文字列をパースする純粋仮想関数</b>\n
	 * この関数は純粋仮想関数です。実際の処理は継承先クラスで実装
	 * する必要があります。
	 * @param lpcszRistrict	[in]	属性制限文字列（e.x.「100-200」、「3,4,5:W」）
	 */
	virtual bool _SetRistrict( LPCTSTR lpcszRistrict ) = 0;

	/**
	 * @brief <b>与えられた値が編集可能な値かどうかをチェックする純粋仮想関数</b>\n
	 * この関数は純粋仮想関数です。実際の処理は継承先クラスで実装
	 * する必要があります。\n
	 * 警告・否定条件は呼び出し元関数（IsEditable）で処理されますので、この関数では
	 * 処理しません。
	 * @param vaValue	[in]	比較値
	 * @retval sindyeEditableEditable 編集可
	 * @retval sindyeEditableNoEditable 編集不可
	 */
	virtual sindyeEditableType _IsEditable( const VARIANT& vaValue ) const = 0;
public:
	CString	m_strFieldName;	//!< フィールド名
	CString	m_strRistrict;	//!< 属性制限文字列
private:
	bool	m_bNot;			//!< 条件否定フラグ
};

/**
 * @brief <b>最大最小の属性制限値を保持するクラス</b>\n
 * フォーマット：数値-数値\n
 * 期待する文字列は「0-100」とか「0-100」とか\n
 * 数値であればOKだが、_ttofで変換できることが条件\n
 * マイナス値の場合は、-100から-1の場合「-100--1」、-11から30の場合「-11-30」\n
 */
class CAttributeRistrictMinMax : public CAttributeRistrict, public std::pair<double, double>
{
protected:
	bool _SetRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType _IsEditable( const VARIANT& vaValue ) const;
};

/**
 * @brief <b>個別属性制限値を保持するクラス</b>\n
 * フォーマット：数値[,数値[,数値[...\n
 * 期待する文字列は「3」とか「0,-1,2」とか「NULL,'あいうえお'」とか\n
 * 数値は_ttofで変換できることが条件\n
 */
class CAttributeRistrictValues : public CAttributeRistrict, public std::list<CComVariant>
{
protected:
	bool _SetRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType _IsEditable( const VARIANT& vaValue ) const;
private:
	void AddValue( LPCTSTR lpcszValue );
};

class SINDYLIB_API CAttributeRistricts : public CAttributeRistrictBase
{
public:
	CAttributeRistricts();
	virtual ~CAttributeRistricts();

	virtual sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const = 0;

	virtual bool AddRistrict( LPCTSTR lpcszRistrict );

	typedef std::list<CSPAttributeRistrictBase>::iterator iterator;
	typedef std::list<CSPAttributeRistrictBase>::const_iterator const_iterator;

	void clear();
protected:
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list<CSPAttributeRistrictBase> m_listRistrict;
#pragma warning(pop)
};

/**
 * @brief <b>AND条件で各属性制限をくくるクラス</b>\n
 * フォーマット：条件|条件|...[:w]
 */
class SINDYLIB_API CAttributeRistrictsAnd : public CAttributeRistricts
{
public:
protected:
	bool SetRistrict( LPCTSTR lpcszRistrict );
public:
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;
};

class SINDYLIB_API CAttributeRistrictsOr : public CAttributeRistricts
{
public:
	CAttributeRistrictsOr();
	virtual ~CAttributeRistrictsOr();

	CAttributeRistrictsOr( const CAttributeRistrictsOr& obj );
	CAttributeRistrictsOr& operator=( const CAttributeRistrictsOr& obj );

	bool AddRistrict( LPCTSTR lpcszRistrict );
	sindyeEditableType IsEditable( const CRow& cRow, int nLen, LPTSTR lpszMessage ) const;

/*private:
	typedef std::list< boost::shared_ptr<CAttributeRistrictBase> >::iterator iterator;
	typedef std::list< boost::shared_ptr<CAttributeRistrictBase> >::const_iterator const_iterator;
#pragma warning(push)
#pragma warning(disable: 4251)
	std::list< boost::shared_ptr<CAttributeRistrictBase> > m_listRistrict;
#pragma warning(pop)*/
};

} // sindy

#endif // _ATTRIBUTERISTRICT_H_
