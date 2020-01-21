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
 *	GDBSpatialSearch.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		新規作成
 */

#ifndef	___GDB_SPATIAL_SEARCH_H___
#define	___GDB_SPATIAL_SEARCH_H___

#include <string>

extern HRESULT GDBCreateSpatialFilter(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISpatialFilter **ipSpatialFilter);

/**
 * @brief 対象フィーチャクラスにWHERE句で、検索をかけ、カーソルを取得する
 * @param ipFeatureClass [in] 対象フィーチャクラス
 * @param ipGeometry     [in] 空間検索で使用するジオメトリ
 * @param cSpatRel       [in] SpatialReference
 * @param ipFeatureCursor [out] 取得カーソル
 * @param strSubFields   [in] SubFields
 * @retval S_OK 取得成功
 */
extern HRESULT GDBSearchByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, IFeatureCursor **ipFeatureCursor, const std::string& strSubFields = "");
//extern HRESULT GDBSelectByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, ISelectionSet **ipSelectionSet);
extern HRESULT GDBCountByGeometry(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, long *cCount);

extern HRESULT GDBSearchByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, IFeatureCursor **ipFeatureCursor);
//extern HRESULT GDBSelectByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, ISelectionSet **ipSelectionSet);
extern HRESULT GDBCountByGeometryAndWhereClause(IFeatureClassPtr ipFeatureClass, IGeometryPtr ipGeometry, esriSpatialRelEnum cSpatRel, const std::string &cWhereClause, esriSearchOrder cSearchOrder, long *cCount);

#endif	//___GDB_SPATIAL_SEARCH_H___
