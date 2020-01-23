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
#include "util.h"
#include "OutputLog.h"

class WalkCodeListCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out           [in] ログ出力管理クラスの参照
	 * @param modufyFilter  [in] 編集履歴フィルタ
	 */
	WalkCodeListCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
	{}

	/**
	 * @brief 歩行者路線コードテーブルの全チェック
	 * @param walkCodeSet [in] チェック対象である
	 * WALK_CODE_LISTテーブルのレコードのOID
	 * @param walkCode    [in] 歩行者路線コードの情報
	 */
	void checkWalkCodeList(
		const std::set<long>& walkCodeSet,
		const RowsPack& walkCode
		);

	// TODO: checkWalkCodeListと統合可能な箇所を統合
	/**
	 * @brief releaseモード用のチェック
	 * @param walkCode    [in] 歩行者路線の情報
	 */
	void checkRelease(
		const RowsPack& walkCode
	);

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
};
