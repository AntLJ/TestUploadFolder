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

#include <RowBase.h>
#include "Arguments.h"
#include "Conditions.h"

class AdditionalUpdate
{
public:
	AdditionalUpdate() : sindyHistory(false) {}
	bool sindyHistory; //!< SiNDYタイプの履歴
	std::list<ConditionExpression> conditions;
};

/// ループタイプ定義
enum LoopType : int
{
	LoopUnknown,
	LoopSRC,
	LoopDST,
};

class ConvertingDefinitionManager
{
public:
	/// コンバート実施定義がされているか
	operator bool() const
	{
		return !m_listConditions.empty();
	}

	/**
	 * @brief  XMLをを解析する
	 * @param  path  [in]  XMLファイルパス
	 * @retval
	 */
	bool  parse(const uh::tstring& path);	

	/// DST側の条件があるかどうか
	bool hasDstCondition() const;

	typedef std::pair<Condition,std::list<SPCondition>> Conditions;
	std::list<std::pair<LoopType,Conditions>> m_listConditions; //!< ループタイプ毎の各種条件
	AdditionalUpdate m_globalAdditionalUpdate; //!< 更新時追加情報（全体適用）
	std::vector<std::pair<uh::tstring,IndexList>> m_listTarget; //!< 処理対象テーブル first:テーブル名 second:カラム対応表
};
