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
 *	GDBQuery.h
 *	@author	Fusatoshi Abe
 *	@date	2003/09/26		新規作成
 */

#ifndef	___GDB_QUERY_H___
#define	___GDB_QUERY_H___

#include <string>

extern HRESULT GDBSearch(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, IFeatureCursor **ipFeatureCursor);
extern HRESULT GDBSearch(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, _ICursor **ipCursor);

//extern HRESULT GDBSelect(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, ISelectionSet **ipSelectionSet);
//extern HRESULT GDBSelect(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, ISelectionSet **ipSelectionSet);

extern HRESULT GDBCount(IFeatureClassPtr ipFeatureClass, IQueryFilterPtr ipQueryFilter, long *cCount);
extern HRESULT GDBCount(ITablePtr ipTable, IQueryFilterPtr ipQueryFilter, long *cCount);

/**
 * @brief 対象フィーチャクラスにWHERE句の条件で、検索をかけ、カーソルを取得する
 * @param ipFeatureClass [in] 対象フィーチャクラス
 * @param cWhereClause   [in] WHERE句
 * @param ipFeatureCursor [out] 取得カーソル
 * @param strSubFields   [in] SubFields
 * @retval S_OK 取得成功
 */
extern HRESULT GDBSearchByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, IFeatureCursor **ipFeatureCursor, const std::string& strSubFields = "");

extern HRESULT GDBSearchByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, _ICursor **ipCursor);

//extern HRESULT GDBSelectByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, ISelectionSet **ipSelectionSet);
//extern HRESULT GDBSelectByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, ISelectionSet **ipSelectionSet);

extern HRESULT GDBCountByWhereClause(IFeatureClassPtr ipFeatureClass, const std::string &cWhereClause, long *cCount);
extern HRESULT GDBCountByWhereClause(ITablePtr ipTable, const std::string &cWhereClause, long *cCount);

#endif	//___GDB_QUERY_H___
