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

#define FN_LONGITUDE	_T("LONGITUDE")
#define FN_LATITUDE		_T("LATITUDE")
#define FN_CITYMESH		_T("CITYMESH")
#define FN_BASEMESH		_T("BASEMESH")
#define FN_MIDDLEMESH	_T("MIDDLEMESH")
#define FN_MESHX		_T("MESHX")
#define FN_MESHY		_T("MESHY")
#define FN_POINTS		_T("POINTS")	// [Bug 5691]
#define FN_ISLINEAR		_T("ISLINEAR")	// [Bug 5691]
#define FN_LENGTH		_T("LENGTH")	// [Bug 7086]
#define FN_AREA			_T("AREA")		// [Bug 7086]
#define FN_DUMMY		_T("DUMMY")		// Ver0.8より

/**
 * @class	CFieldInfo
 * @brief	出力フィールド情報
 */
class CFieldInfo
{
public:
	CFieldInfo( const CString& strName, LONG lFID, bool bDomain = false, bool bShapeField = false, bool bDummy = false, CString strDummyVal = _T("") )
		: m_strName(strName), m_lFID(lFID), m_bDomain(bDomain), m_bShapeField(bShapeField), m_bDummy(bDummy), m_strDummyVal(strDummyVal)
	{
	}

	CString	m_strName;		//!< フィールド名
	LONG	m_lFID;			//!< フィールドインデックス
	bool	m_bDomain;		//!< ドメイン名出力？
	bool	m_bShapeField;	//!< 位置情報フィールド？
	bool	m_bDummy;		//!< ダミーフィールド？(Ver0.8より)
	CString m_strDummyVal;	//!< ダミーフィールド出力情報(Ver0.8より)
};

typedef std::list<CFieldInfo>	FIELDLIST;

/**
 * @class	CInitFile
 * @brief	設定ファイル管理クラス
 */
class CInitFile
{
public:
	CInitFile(void) : m_nShapeFieldCount(0), m_nMeshLevel(2){};
	~CInitFile(void){};

	/**
	 * @brief	初期化
	 * @param	lpcszFile	[in]	ファイル名
	 * @param	ipTable		[in]	テーブル
	 * @param	bShape		[in]	テーブルに図形を含むかどうか？
	 * @param	bSkip		[in]	項目が無い場合エラーとしない(スキップして出力する)
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool init( LPCTSTR lpcszFile, const ITablePtr& ipTable, const bool& bShape, const bool& bSkip );

	/**
	 * @brief	フィールドリスト取得
	 * @return	フィールドリスト
	 */
	const FIELDLIST& getFieldList(){ return m_listField; }

	/**
	 * @brief	位置情報フィールド指定数
	 * @return	位置情報フィールド数
	 */
	int getShapeFieldCount() const{ return m_nShapeFieldCount; }

	/**
	 * @brief	メッシュレベル取得
	 * @return	メッシュレベル
	 */
	int getMeshLevel() const{ return m_nMeshLevel; }

private:

	/**
	 * @brief	位置情報に関連するフィールド？
	 * @param	strField	[in]	フィールド名
	 * @retval	true	処理成功
	 * @retval	false	処理失敗
	 */
	bool isShapeField( const CString& strField )
	{
		static LPCTSTR g_lpcszShapeField[] = { FN_ISLINEAR, FN_LONGITUDE, FN_LATITUDE, 
											   FN_CITYMESH, FN_BASEMESH, FN_MIDDLEMESH, FN_MESHX, FN_MESHY, FN_POINTS, 
											   FN_LENGTH, FN_AREA, NULL };
		for( int i=0; g_lpcszShapeField[i] != NULL; ++i )
		{
			if( strField == g_lpcszShapeField[i] )
				return true;
		}
		return false;
	}

private:
	
	FIELDLIST	m_listField;			//!< フィールドリスト

	int			m_nShapeFieldCount;		//<! 位置情報フィールド数
	int			m_nMeshLevel;			//!< メッシュXY出力する際にメッシュレベル(crd_cnvにならって1,2,64)
};
