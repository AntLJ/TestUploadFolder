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
 * @file  GlobalFunc.h
 * @brief グローバル関数定義
 */

namespace gf
{
	/**
	 * @brief  ワークスペース取得(何故か接続失敗することもあるので5回リトライ)
	 * @note   SDE PGDB SHAPE いずれもＯＫ
	 * @param  lpcszDBProp	 [in]	ＤＢ接続プロパティ
	 * @retval IWorkspacePtr ワークスペース
	 */
	IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp );

	/**
	 * @brief  環境変数取得
	 * @param  lpcszEnv [in] 環境変数名
	 * @retval CString  指定した環境変数の情報
	 */
	inline CString GetEnvValue( const _TCHAR* lpcszEnv )
	{
		CString strTmp;
		return strTmp.GetEnvironmentVariable(lpcszEnv)? strTmp : NULL;
	}

	/**
	 * @brief  環境変数名から、ワークスペースとフィーチャクラスをオープン
	 * @param  lpcszEnv    [in]  環境変数名
	 * @param  ipWorkspace [out] ワークスペース
	 * @param  ipFC        [out] フィーチャクラス
	 * @retval true  オープン成功
	 * @retval false 失敗
	 */
	bool GetFCFromEnv( const _TCHAR* lpcszEnv, IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFC );

	/**
	 * @brief  空間検索 (+ 属性検索)
	 * @note   [Bug 6419]内部エラー発生時は、件数に-1を設定します
	 * @param  ipGeo			[in]	対象のジオメトリ
	 * @param  ipFeatureClass	[in]	検索元のフィーチャクラス
	 * @param  pCount			[out]	検索レコード数([Bug 6175]件数取得しないなら、NULL指定)
	 * @param  spatialRel		[in]	検索条件
	 * @param  lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param  lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @retval フィーチャカーソル(検索レコード数 = 0なら、NULL返す)
	 */
	IFeatureCursorPtr SelectByShape( IGeometryPtr& ipGeo, IFeatureClassPtr& ipFeatureClass, long* pCount = NULL,
									 esriSpatialRelEnum spatialRel = esriSpatialRelIntersects,
									 LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief  現在時刻を「YYYY/MM/DD hh:mm:ss」の形で取得
	 * @retval CString 時間
	 */
	inline CString GetCurrentTime()
	{
		CString strTime;
		SYSTEMTIME	st;
		GetLocalTime(&st);
		strTime.Format( _T("%04d/%02d/%02d %02d:%02d:%02d"),
						st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond );
		return strTime;
	}
}
