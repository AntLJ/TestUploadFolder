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
#include <list>
#include <vector>

#include <ArcHelperEx/ActiveView.h>
#include <ArcHelperEx/AheGlobals.h>

namespace miscfunc
{
	/**
	 * @brief テーブルのフィールドに設定されているコード値ドメインのmapを取得する
	 * @note 整数型のみ対応。どっちでもいいけどコード値をキーとしたほうが重複の可能性が軽減できるので、それで
	 * @param [in] ipTable 対象テーブル
	 * @param [in] fieldName 対象フィールド
	 * @param [out] nameValueMap コード値からドメイン名を呼び出すmap
	 * @retval true ドメインmap取得成功
	 * @retval false ドメインmap取得失敗
	 */
	bool GetCodedValueDomainMap(ITablePtr ipTable, const CString& fieldName, std::map<long, CString>& valueDomainMap);

	/**
	* @brief フィールドタイプを取得する
	* @param[in]  fields       フィールド群
	* @param[in]  field_name   フィールドタイプを取得したいフィールドの名称
	* @param[out] field_type   フィールドタイプ
	* @retval true  取得成功
	* @retval false 取得失敗
	*/
	bool GetFieldType(const IFieldsPtr& fields, const CString& field_name, esriFieldType& field_type);
}