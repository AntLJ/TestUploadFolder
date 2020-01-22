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
 * @file	GlobalFunc.h
 * @brief	良く使う関数郡定義ファイル
 */

// 一般的な関数
namespace gf
{
	/**
	 * @brief	ワークスペース取得(何故か接続失敗することもあるので5回リトライ)
	 * @note	SDE PGDB SHAPE いずれもＯＫ
	 * @param	lpcszDBProp	[in]	ＤＢ接続プロパティ
	 * @return	ワークスペース
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief	空間検索 (+ 属性検索)
	 * @param	ipGeo			[in]	対象のジオメトリ
	 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
	 * @param	pCount			[out]	検索レコード数([Bug 6175]件数取得しないなら、NULL指定)
	 * @param	spatialRel		[in]	検索条件
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @return	フィーチャカーソル(検索レコード数 = 0なら、NULL返す)
	 */
	IFeatureCursorPtr SelectByShape( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, long* pCount = NULL,
									 esriSpatialRelEnum spatialRel = esriSpatialRelIntersects,
									 LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief	属性検索
	 * @param	ipTable			[in]	検索元のフィーチャクラス
	 * @param	pCount			[out]	検索レコード数([Bug 6175]件数取得しないなら、NULL指定)
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @return	フィーチャカーソル(検索レコード数 = 0なら、NULL返す)
	 */
	_ICursorPtr SelectByAttr( ITablePtr ipTable, long* pCount = NULL, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );
}

// 住所系に特化した一般的な関数
namespace addr_func
{
	static crd_cnv g_cCrdCnv;

	/**
	 * @brief	半角数字?
	 * @param	strData	[in]	データ
	 * @return	bool
	 */
	bool IsNumStr( const CString& strData );

	/**
	 * @brief	通常の地番?
	 * @param	strPnt	[in]	地番名称
	 * @return	bool
	 */
	inline bool IsNormalPnt( const CString& strPnt ){ return IsNumStr(strPnt) && strPnt != _T("0") && _tstoi(strPnt) > 0 && _tstoi(strPnt) < 100000; }

	/**
	 * @brief	通常の号番号?
	 * @param	strGou	[in]	号番号
	 * @return	bool
	 */
	inline bool IsNormalGou( const CString& strGou ){ return IsNumStr(strGou) && strGou != _T("0") && _tstoi(strGou) > 0 && _tstoi(strGou) < 10000; }

	/**
	 * @brief	CGouRec比較関数(代表住所選択用)
	 * @param	lhs	[in]	号レコード1
	 * @param	rhs	[in]	号レコード2
	 * @return	bool
	 */
	bool CompareGouRec( const CGouRec& lhs, const CGouRec& rhs );

	/**
	 * @brief	号レコード位置的な優先度決定関数
	 * @note	より東・より北にあるものを優先
	 * @param	lhs	[in]	GouRec1
	 * @param	rhs	[in]	GouRec2
	 * @retval	-1 : 左側を優先
	 * @retval	 0 : 同一位置
	 * @retval	 1 : 右側を優先
	 */
	int CompareGouRecPos( const CGouRec& lhs, const CGouRec& rhs );

	/**
	 * @brief	号レコード間の距離算出
	 * @param	lhs	[in]	GouRec1
	 * @param	rhs	[in]	GouRec2
	 * @return	距離
	 */
	inline double GetDistGouRec( const CGouRec& lhs, const CGouRec& rhs )
	{
		return g_cCrdCnv.GetMeshDist( lhs.m_cMXY.m_nMeshCode, lhs.m_cMXY.m_nX, lhs.m_cMXY.m_nY, rhs.m_cMXY.m_nMeshCode, rhs.m_cMXY.m_nX, rhs.m_cMXY.m_nY );
	}

	/**
	 * @brief	半角英数字を全角化
	 * @param	lpcszData	[in]	データ
	 * @return	変換データ(失敗したら、そのまま返す)
	 */
	inline CString GetZenkakuData( LPCTSTR lpcszData )
	{
		TCHAR szConvData[100] = _T("");
		int nRet = ::LCMapString( ::GetUserDefaultLCID(), LCMAP_FULLWIDTH, lpcszData, _tcslen(lpcszData), szConvData, sizeof(szConvData) );
		return nRet? szConvData : lpcszData;
	}

}
