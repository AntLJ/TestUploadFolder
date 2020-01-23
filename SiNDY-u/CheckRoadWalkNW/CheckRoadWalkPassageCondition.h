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

class CCheckRoadWalkPassageCondition : public CCheckRoadWalkNode
{
public:
	CCheckRoadWalkPassageCondition(void);
	~CCheckRoadWalkPassageCondition(void);


	/**
	 * @brief	NPWフォーマットチェック
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
	 * @param	linkID	[in]	リンクID
	 * @param	ipRow	[in]	取得対象のRow
	 * @param	fieldName	[in]	取得対象のフィールド名
	 * @param	fieldIndex	[in]	取得対象のフィールドインデックス
	 * @param	value	[out]	取得した値
	 * @return	bool
	 */
	virtual bool getValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, CComVariant& value);

	/**
	 * @brief	値をマップに設定
	 * @param	dbFieldName	[in]	DBフィールド名
	 * @param	lValue		[in]	設定する値
	 * @param	mapValue	[out]	値のマップ(キーはDBフィールド名)
	 */
	virtual void setValue(const CString& dbFieldName, const long& lValue, MAP_VALUE& mapValue);

	/**
	 * @brief	値をマップに設定
	 * @param	dbFieldName	[in]	DBフィールド名
	 * @param	varValue	[in]	設定する値
	 * @param	mapValue	[out]	取得した値のマップ(キーはDBフィールド名)
	 */
	virtual bool setValue(const CString& dbFieldName, const CComVariant& varValue, MAP_VALUE& mapValue);


	/**
	 * @brief	道路：固定値比較、歩行者：固定値比較
	 * @param	linkID	[in]	リンクID
	 * @param	ipRow	[in]	チェック対象のレコード
	 * @param	fieldName	[in]	チェック対象のフィールド名
	 * @param	fieldIndex	[in]	チェック対象のフィールドインデックス
	 * @param	fixedTimeValue	[in]	比較する固定値(道路)
	 * @param	fixedWalkValue	[in]	比較する固定値(歩行者)
	 * @return	bool
	 */
	virtual bool checkFixedValue(long linkID, const _IRowPtr& ipRow, const CString& fieldName, const FIELD_INDEX& fieldIndex, long fixedTimeValue, long fixedWalkValue);

	/**
	 * @brief	エラーメッセージの作成と追加
	 * @param	linkID	[in]	リンクID
	 * @param	fieldName	[in]	フィールド名
	 * @param	message	[in]	エラーメッセージ
	 * @param	count	[in]	エラー対象の値の件数(これ以降にエラー対象の値を複数指定)
	 */
	using CCheckRoadWalkNode::pushColumnErrorMessage;
	virtual void pushColumnErrorMessage(long linkID, const CString& filedName, const CString& message, int count, ...);
};

