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
#include "OutputFormat.h"
#include "ConvertTable.h"
#include "FieldDef.h"
#include "BaseRow.h"

/**
 * @brief 文字列変換後のデータチェック
 */
class CCheckString : CBaseRow
{
public:
	CCheckString(){}
	~CCheckString(){}

public:
	/**
	 * @brief 出力文字のチェック関数
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDef [in] フィールド対応表の情報
	 * @param strValue [in] 対象データ
	 * @retval true チェックエラーなし
	 * @retval false チェックエラーあり
	 */
	bool checkOutputString(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue);

private:
	
	/**
	 * @brief NULLチェック
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDef [in] フィールド対応表の情報
	 * @param canNull [in] NULL許可
	 * @param strValue [in] 対象データ
	 * @retval true NULL値でないorNULL許可でNULL値
	 * @retval false NULL許可なのにNULL値
	 */
	bool checkNull(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, bool canNull, const CString& strValue);

	/**
	 * @brief 列幅チェック
	 * @param ipRow [in] レコードデータ（エラー出力用）
	 * @param cConvTableInfo [in] 変換対象テーブル情報（エラー出力用）
	 * @param cFieldDef [in] フィールド対応表の情報
	 * @param strValue [in] 対象データ
	 * @retval true 列幅OK
	 * @retval false 列幅NG
	 */
	bool checkLength(const _IRowPtr& ipRow, const CConvTableInfo& cConvTableInfo, const CFieldDefInfo& cFieldDef, const CString& strValue);

	/**
	 * @brief 条件チェック
	 * @param setCheckValue [in] チェック条件
	 * @param strValue [in] 対象データ
	 * @param bValid [in] true=有効値チェック, false=エラー値チェック
	 * @retval true 条件に含まれる
	 * @retval false 条件に含まれない
	 */
	bool checkValue(const std::set<CString>& setCheckValue, const CString& strValue, bool bValid);

	/**
	 * @brief 範囲条件チェック
	 * @param cFieldDef [in] フィールド対応表の情報
	 * @param setRange [in] チェック条件
	 * @param strValue [in] 対象データ
	 * @param bValid [in] true=有効値チェック, false=エラー値チェック
	 * @retval true 条件に含まれる
	 * @retval false 条件に含まれない
	 */
	bool checkRangeValue(const CFieldDefInfo& cFieldDef, const std::set<CRange>& setRange, const CString& strValue, bool bValid);

};

