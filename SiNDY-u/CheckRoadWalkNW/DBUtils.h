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

#include "stdafx.h"

#include "Properties.h"

typedef std::map< CString, long> FIELD_INDEX;
typedef std::map<long, CAdapt<IFeaturePtr>> MAP_FEATURE;
typedef std::map<long, CAdapt<_IRowPtr>> MAP_ROW;

class CDBUtils
{
public:
	/**
	 * @brief	フィールドインデックスの作成
	 * @param	ipTable	[in]	作成元のテーブル
	 * @param	fieldIndex	[out]	作成したフィールドインデックス
	 * @return	bool
	 */
	static bool createFieldIndex(const ITablePtr ipTable, FIELD_INDEX& fieldIndex);

	/**
	 * @brief	ObjectIDをキーとしたフィーチャーマップの生成
	 * @param	ipFeatureClass	[in]	作成元のフィーチャークラス
	 * @param	fieldIndex	[in]	作成元のフィールドインデックス
	 * @param	ipSpatialFilter	[in]	作成時のフィルター
	 * @param	features	[out]	作成したマップ
	 * @return	bool
	 */
	static bool createFeatureMap(const IFeatureClassPtr ipFeatureClass, FIELD_INDEX fieldIndex, const ISpatialFilterPtr ipSpatialFilter, MAP_FEATURE& features);

	/**
	 * @brief	ObjectIDをキーとしたテーブルROWマップの生成
	 * @param	ipTable	[in]	作成元のテーブル
	 * @param	fieldIndex	[in]	作成元のフィールドインデックス
	 * @param	ipQueryFilter	[in]	作成時のフィルター
	 * @param	rows	[out]	作成したマップ
	 * @return	bool
	 */
	static bool createRowMap(const ITablePtr ipTable, FIELD_INDEX fieldIndex, const IQueryFilterPtr ipQueryFilter, MAP_ROW& rows);
};
