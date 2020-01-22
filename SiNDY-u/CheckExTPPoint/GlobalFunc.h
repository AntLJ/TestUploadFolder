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

namespace gf
{

	/**
	 * @brief	ワークスペース取得
	 * @param	lpcszDBProp	[in]	接続プロパティ
	 * @return	ワークスペース
	 */
	inline IWorkspacePtr getWorkspace( LPCTSTR lpcszDBProp )
	{
		INamePtr ipName( sindy::create_workspace_name(lpcszDBProp) );
		if( NULL == ipName )
			return NULL;
		IUnknownPtr ipUnknown;
		ipName->Open( &ipUnknown );
		return ipUnknown;
	}

	/**
	 * @brief	空間検索 (+ 属性検索)
	 * @param	ipGeo			[in]	対象のジオメトリ
	 * @param	ipFeatureClass	[in]	検索元のフィーチャクラス
	 * @param	ppFeatureCursor	[out]	検索結果のカーソル
	 * @param	spatialRel		[in]	検索条件
	 * @param	lpszWhereClause	[in]	条件検索もするなら指定する(デフォルトでNULL)
	 * @param	lpszSubFields	[in]	必要なフィールド(カンマ区切り)
	 * @return	検索に一致した件数
	 */
	LONG SelectByShapeWithCount( IGeometryPtr ipGeo,
								 IFeatureClassPtr ipFeatureClass,
								 IFeatureCursor** ppFeatureCursor,
				 				 esriSpatialRelEnum spatialRel,
								 LPCTSTR lpszWhereClause = NULL,
								 LPCTSTR lpszSubFields = NULL );

}
