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
#include "CheckRoadWalkNode.h"

#define D_WALK_ROAD_NAME	101

class CCheckRoadCode : public CCheckRoadWalkNode
{
public:
	CCheckRoadCode(void);
	~CCheckRoadCode(void);

	/**
	 * @brief	road_codeフォーマットチェック
	 * @param	ipTable	[in]	チェック対象のテーブル
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	dbCtrl	[in,out]	比較対象のDB情報
	 * @return	bool
	 */
	virtual bool checkFormat(const ITablePtr& ipTable, const FIELD_INDEX& fieldIndex, CDatabaseController& dbCtrl);

protected:
	/**
	 * @brief	指定されたレコードから値の取得
	 * @note	取得に失敗した場合、エラーメッセージを追加する
	 * @param	roadcode	[in]	路線コード
	 * @param	ipRow	[in]	取得対象のレコード
	 * @param	fieldName	[in]	取得対象のフィールド名
	 * @param	fieldIndex	[in]	取得対象のフィールドインデックス
	 * @param	value	[out]	取得した値
	 * @return	bool
	 */
	virtual bool getValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value);

	/**
	 * @brief	DB比較チェック
	 * @param	roadcode	[in]	路線コード
	 * @param	ipRow	[in]	チェック対象のレコード
	 * @param	fieldName	[in]	チェック対象のフィールド名
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	mapDbValue	[in]	比較対象のDBレコードの値map
	 * @param	dbWalkfieldName	[in]	比較対象の歩行者フィールド名
	 * @param	dbRoadfieldName	[in]	比較対象の道路フィールド名
	 * @param	walkRecord	[in]	歩行者か否か
	 * @return	bool
	 */
	virtual bool checkDBValue(long roadcode, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex,
								  const MAP_VALUE& mapDbValue, const CString& dbWalkFieldName, const CString& dbRoadFieldName, bool walkRecord);

};

