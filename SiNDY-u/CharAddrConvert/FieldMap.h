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

#include <map>

/**
 * @class	CFieldMap
 * @brief	フィールドマップ
 */
class CFieldMap
{
public:
	CFieldMap(void){};

	/**
	 * @brief	コンストラクタ
	 * @param	ipTable	[in]	ITable
	 */
	CFieldMap( ITable* ipTable )
	{
		CreateFieldMap( ipTable );
	}

	/**
	 * @brief	コンストラクタ
	 * @param	ipFields	[in]	IFields
	 */
	CFieldMap( IFields* ipFields )
	{
		CreateFieldMap( ipFields );
	}

	~CFieldMap(void){};

	/**
	 * @brief	フィールドマップ作成
	 * @param	ipTable	[in]	ITable
	 */
	void CreateFieldMap( ITable* ipTable );

	/**
	 * @brief	フィールドマップ作成
	 * @param	ipFields	[in]	IFields
	 */
	void CreateFieldMap( IFields* ipFields );

	/**
	 * @brief	フィールドインデックス取得
	 * @param	lpcszField	[in]	フィールド名
	 * @return	フィールドインデックス( 失敗したら -1 )
	 */
	long GetFieldIndex( LPCTSTR lpcszField ) const
	{
		FIELDMAP::const_iterator itr = m_mapField.find( lpcszField );
		return itr != m_mapField.end()? itr->second : -1;
	}

private:

	typedef std::map<CString, long>				FIELDMAP;
	FIELDMAP		m_mapField;		//!< フィールドマップ

};
