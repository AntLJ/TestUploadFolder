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

#include <sindy/workspace.h>

typedef std::map< long, CString > DOMAINMAP;	/// ドメインマップ

namespace gf
{
	/**
	 * @brief	PGDBモードか否か
	 * @return	bool
	 */
	inline bool IsPGDBMode()
	{
		return NULL != _tgetenv(_T("PGDB_MODE"));
	}

	/**
	 * @brief	ワークスペース取得
	 * @param	lpcszDBProp	[in]	接続プロパティ
	 * @return	IWorkspace
	 */
	inline IWorkspacePtr GetWorkspace( LPCTSTR lpcszDBProp )
	{
		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( NULL == ipName )
			return NULL;
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		return ipUnknown;
	}

	/**
	 * @brief	ドメイン値・ドメイン名マップ作成
	 * @param	ipTable			[in]	ドメインフィールドを持つテーブル
	 * @param	lpcszFieldName	[in]	ドメインを持つフィールド名
	 * @param	map				[in,out]ドメインマップ（無い場合はサイズ0のドメインマップを返す）
	 * @return	bool
	 */
	bool GetDomainMap( ITablePtr ipTable, LPCTSTR lpcszFieldName, DOMAINMAP* map );

	/**
	 * @brief	空間検索 (+ 属性検索)
	 * @param	ipGeo			[in]	対象のジオメトリ
	 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
	 * @patam	ipFeatureCursor	[out]	フィーチャカーソル
	 * @param	pCount			[out]	検索結果レコード数
	 * @param	spatialRel		[in]	検索条件
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @return	bool
	 */
	bool SelectByShapeWithCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, IFeatureCursorPtr& ipFeatureCursor, long* pCount,
								 esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause = NULL, LPCTSTR lpszSubFields = NULL );

	/**
	 * @brief	空間検索 (+ 属性検索)
	 * @note	件数のみ
	 * @param	ipGeo			[in]	対象のジオメトリ
	 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
	 * @param	spatialRel		[in]	検索条件
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @return	レコード件数(エラー発生時 -1)
	 */
	long SelectByShapeOnlyCount( IGeometryPtr ipGeo, IFeatureClassPtr ipFeatureClass, esriSpatialRelEnum spatialRel, LPCTSTR lpszWhereClause = NULL );
}
