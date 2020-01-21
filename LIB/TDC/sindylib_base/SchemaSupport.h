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

#include <boost/shared_ptr.hpp>
#include "TableType.h"

namespace sindy {
class CFieldMap;
class CTableNameString;
typedef boost::shared_ptr<CFieldMap> CSPFieldMap;
typedef boost::shared_ptr<CTableNameString> CSPTableNameString;

class CSchemaSupport
{
public:
	CSchemaSupport();
	virtual ~CSchemaSupport();

	CSchemaSupport( const CSchemaSupport& obj );
	CSchemaSupport( sindyTableType::ECode emType, CSPTableNameString spNames, CSPFieldMap spFields );

	const CSchemaSupport& operator=( const CSchemaSupport& obj );

	void SetFieldMap( CSPFieldMap cFields ) { m_spFields = cFields; }
	void SetNameString( CSPTableNameString cNames ) { m_spNames = cNames; }
	virtual CSPFieldMap GetFieldMap() const { return m_spFields; }
	virtual CSPTableNameString GetNameString() const { return m_spNames; }

	CSPFieldMap GetFieldMap( IFields* ipFields ) const;
	CSPTableNameString GetNameString( ITable* ipTable ) const;

	/**
	 * @brief <b>テーブルタイプを取得する</b>\n
	 * @return sindyTableType::ECode
	 */
	sindyTableType::ECode GetTableType() const;
	void SetTableType( sindyTableType::ECode emTableType ) { m_emTableType = emTableType; }

	/**
	 * @brief <b>テーブル名を返す</b>\n
	 * @see CTableNameString::GetTableName()
	 * @retval LPCTSTR
	 */
	LPCTSTR GetTableName() const;

	/**
	 * @brief <b>テーブルオーナー名を取得する</b>\n
	 * 返される文字列は必ず大文字です。
	 * @return テーブルオーナー名
	 */
	LPCTSTR GetOwnerName() const;

	/**
	 * @brief <b>オーナー名付きテーブル名を取得する</b>\n
	 * 「オーナー名」.「テーブル名」の形式で返します。
	 * テーブル名だけでなくオーナー名も必要な場合はこちらを使用してください\n
	 * 返される文字列は必ず大文字です。
	 * @return オーナー名付きテーブル名
	 */
	LPCTSTR GetOwnerTableName() const;

	/**
	 * @brief <b>OBJECTIDのフィールドインデックス番号を取得する</b>\n
	 * @retval 0未満 エラー
	 * @retval 0以上 インデックス番号
	 */
	long GetOIDFieldIndex() const;

	/**
	 * @brief フィールド名からインデックス番号を取得する
	 * @param lpcszFieldName [in] フィールド名
	 * @retval -1 フィールド名が存在しない
	 * @retval -1以外 フィールドインデックス番号
	 */
	long FindField( LPCTSTR lpcszFieldName ) const;
protected:
	mutable sindyTableType::ECode	m_emTableType;	//!< 格納されているフィーチャのテーブル識別子
	mutable CSPTableNameString		m_spNames;		//!< テーブル名キャッシュ用（shared ptr）
	mutable CSPFieldMap				m_spFields;		//!< フィールド対応表（shared ptr）
};

typedef boost::shared_ptr<CSchemaSupport> CSPSchemaSupport;

} // sindy
