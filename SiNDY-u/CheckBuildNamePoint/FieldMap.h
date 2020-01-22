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
	 * @param	ipTable [in] フィールドマップを作成するITablePtr
	 */
	CFieldMap( const ITablePtr& ipTable ){
		CreateFieldMap( ipTable );
	}

	/**
	 * @brief	コンストラクタ
	 * @param	ipFields [in] フィールドマップを作成するIFieldsPtr
	 */
	CFieldMap( const IFieldsPtr& ipFields ){
		CreateFieldMap( ipFields );
	}

	~CFieldMap(void){};

	/**
	 * @brief	フィールドマップ作成
	 * @param	ipTable [in] フィールドマップを作成するITablePtr
	 */
	void CreateFieldMap( const ITablePtr& ipTable );

	/**
	 * @brief	フィールドマップ作成
	 * @param	ipFields [in] フィールドマップを作成するIFieldsPtr
	 */
	void CreateFieldMap( const IFieldsPtr& ipFields );

	/**
	 * @brief	フィールドインデックス取得
	 * @param	lpcszField [in] フィールド名
	 * @retval	long フィールドインデックス( 失敗したら -1 )
	 */
	long GetFieldIndex( LPCTSTR lpcszField ) const 
	{
		auto itr = m_mapField.find( lpcszField );
		return itr != m_mapField.end()? itr->second : -1;
	}

	typedef std::map<CString, long>           FIELDMAP;
	typedef std::map<CString, long>::iterator FIELDMAPITR;

	/**
	 * @brief   フィールドマップ取得
	 * @retval  FIELDMAP フィールドマップ
	 */
	FIELDMAP GetFieldMap()
	{
		return m_mapField;
	}

private:

	FIELDMAP    m_mapField; //!< フィールドマップ
};
