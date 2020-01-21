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
 * @file FieldMap.h
 * @brief CFieldMapクラス定義ファイル
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * $Id$
 */
#ifndef _FIELDMAP_H_
#define _FIELDMAP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "sindymacroutil.h"
#include <boost/shared_ptr.hpp>

namespace sindy {

/**
 * @class CFieldMap
 * @brief <b>フィールド名対応表</b>\n
 * このクラスは、各テーブルのフィールドインデックスと
 * フィールド名の対応を格納しておくクラスです。\n
 * 主に、CRow系のクラスでフィールド対応表をキャッシュするために
 * 使用されます。\n
 */
class SINDYLIB_API CFieldMap
{
public:
	struct _FIELD
	{
		CString name;
		CString alias;
		bool editable;
		bool nullable;
		bool isid;
		esriFieldType type;
		size_t length;
		long precision, scale;
		std::pair<CComVariant,CComVariant> range;
		std::map<CString,long> domain;
		std::map<long,CString> rdomain;
		CComVariant dvalue;
	};
	CFieldMap() : m_lOIDIndex(-1), m_lShapeIndex(-1)
	{
	}
	virtual ~CFieldMap()
	{
		clear();
	}

	CFieldMap( IFields* ipFields )
	{
		CreateFieldMap( ipFields );
	}

	CFieldMap& operator=( const CFieldMap& obj )
	{
		CopyFieldMap( *this, obj );
		return *this;
	}

	/**
	 * @brief CFieldMapをコピーする
	 * @param dest	[in]	ペースト先
	 * @param src	[in]	コピー元
	 */
	friend void CopyFieldMap( CFieldMap& dest, const CFieldMap& src );

	// TODO: 戻り値をboolにして、ErrorCode.hを使用しないようにする。
	/**
	 * @brief <b>フィールド対応表を作成する</b>\n
	 * 既存の対応表があっても新規に作り直します。
	 * @param ipFields	[in]	IFields*
	 * @return sindyErrCode
	 */
	errorcode::sindyErrCode CreateFieldMap( IFields* ipFields );

	/**
	 * @brief <b>フィールド名からフィールドインデックス番号を取得する</b>\n
	 * @param lpcszFieldName	[in]	フィールドインデックス番号
	 * @return 対応するフィールドインデックス番号がない場合は-1
	 * @note なければデバッグ版ではトレースがでます。リリース版では
	 * HasFieldと同じです。
	 */
	long FindField( LPCTSTR lpcszFieldName ) const {
		return _FindField( lpcszFieldName, true );
	}

	/**
	 * @brief <b>フィールド名からフィールドインデックス番号があれば取得する</b>\n
	 * @param lpcszFieldName	[in]	フィールドインデックス番号
	 * @return 対応するフィールドインデックス番号がない場合は-1
	 * @note FindFieldとの違いは、デバッグ版でもトレースが出ないことのみです。
	 */
	long HasField( LPCTSTR lpcszFieldName ) const {
		return _FindField( lpcszFieldName, false );
	}

	/**
	 *  @brief フィールドの付加属性を取得する
	 * 
	 * @note Indexがフィールド数を超える場合は落ちるのを防ぐために
	 * 常に一番最後のフィールド情報を返します。
	 */
	const _FIELD& GetFieldAttr( long Index ) const;

	/**
	 * @brief <b>フィールドインデックスからフィールド名を取得する</b>\n
	 * @param Index	[in]	フィールドインデックス
	 * @return 対応するフィールド名（大文字限定）
	 */
	LPCTSTR GetName( long Index ) const;

	/**
	 * @brief <b>フィールドインデックスからフィールドエイリアス名を取得する</b>\n
	 * @param Index	[in]	フィールドインデックス
	 * @return 対応するフィールドエイリアス名
	 */
	LPCTSTR GetAliasName( long Index ) const;

	/**
	 * @brief <b>フィールドインデックスからデフォルト値を取得する</b>\n
	 * @param Index [in] フィールドインデックス
	 * @return 対応するデフォルト値
	 */
	const VARIANT& GetDefaultValue( long Index ) const;

	/**
	 * @brief <b>フィールドが編集可能かどうかをチェックする</b>\n
	 * @param lIndex	[in]	フィールドインデックス
	 * @retval false	編集不可
	 * @retval true		編集可
	 */
	bool IsEditable( long lIndex ) const;

	/**
	 * @brief <b>フィールドがNULL OKかどうかをチェックする</b>\n
	 * 
	 * @param lIndex	[in]	フィールドインデックス
	 * @retval false	NOT NULL
	 * @retval true	    NULL OK
	 */
	bool IsNullable( long lIndex ) const;

	/**
	 * @brief <b>フィールドがSiNDY的に編集可能かどうかをチェックする</b>\n
	 * @param lIndex	[in]	フィールドインデックス
	 * @retval false	編集不可
	 * @retval true		編集可
	 */
	bool IsSiNDYEditable(long lIndex, bool bForCopy=false) const;

	/**
	 * @brief <b>フィールドタイプを取得する</b>\n
	 * @param lIndex	[in]	フィールドインデックス
	 * @return esriFieldType
	 */
	esriFieldType GetFieldType( long lIndex ) const;

	/**
	 * @brief <b>オブジェクトIDのインデックス番号を取得する</b>\n
	 * @return インデックス番号
	 */
	long GetOIDIndex() const { return m_lOIDIndex; }

	/**
	 * @brief <b>SHAPEのインデックス番号を取得する</b>\n
	 * @return インデックス番号
	 */
	long GetShapeIndex() const { return m_lShapeIndex; }

	/**
	 * @brief <b>フィールド数を取得する</b>\n
	 * @return フィールド数
	 */
	size_t GetFieldCount() const { return m_vecFieldName.size(); }

	/**
	 * @brief <b>コード値ドメイン対応表を取得する</b>\n
	 * @warning long値限定です。また、範囲ドメインには対応していません。
	 * @warning 必ずフィールド名が存在することを確認してください。
	 */
	const std::map<CString,long>& GetDomain( LPCTSTR lpcszFieldName ) const;

	bool IsShapeField( long lIndex ) const { return ( lIndex == m_lShapeIndex ); }
	bool IsOIDField( long lIndex ) const { return ( lIndex == m_lOIDIndex ); }

	typedef std::vector<_FIELD>::iterator iterator;
	typedef std::vector<_FIELD>::const_iterator const_iterator;
	iterator begin() { return m_vecFieldName.begin(); }
	const_iterator begin() const { return m_vecFieldName.begin(); }
	iterator end() { return m_vecFieldName.end(); }
	const_iterator end() const { return m_vecFieldName.end(); }
private:
	/**
	 * @brief <b>対応表をクリアする</b>\n
	 */
	void clear();
	long _FindField( LPCTSTR lpcszFieldName, bool bDebugTrace ) const;
private:
	long						m_lOIDIndex;		//!< OIDフィールドインデックス番号
	long						m_lShapeIndex;		//!< SHAPEフィールドインデックス番号
#pragma warning(push)
#pragma warning(disable:4251)
	std::map<CString,long>		m_mapFieldIndex;	//!< フィールド名とフィールドインデックスのmap
	std::vector<_FIELD>			m_vecFieldName;		//!< フィールド名（編集可不可情報付）のリスト（インデックス順）
#pragma warning(pop)
};

typedef boost::shared_ptr<CFieldMap> CSPFieldMap;

} // sindy

#endif // _FIELDMAP_H_
