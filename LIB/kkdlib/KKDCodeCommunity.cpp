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

// KKDCodeCommunity.cpp: KKDCodeCommunity クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "KKDCodeCommunity.h"

const int kkd::regulation_road_node::kRecordId = kkd::kRegulationRoadNodeId;

/// 交差点ノード種別コードドメイン名一覧
const char* kkd::codes::b::kDomains[] = {
	"",
	"実交差点",
	"ダミー交差点",
	"ダミーノード"
};

/// 交差点通行コードドメイン名一覧
const char* kkd::codes::c::kDomains[] = {
	"未調査",
	"通行可",
	"通行不可",
	"道路条件により通行不可",
	"",
	"",
	"",
	"条件付き通行不可"
};

/// 車種コードドメイン名一覧
const char* kkd::codes::car_type::kDomains[] = {
	"",
	"車両",
	"車両（タクシーを除く）",
	"車両（タクシー・二輪を除く）",
	"車両（タクシー・貨物を除く）",
	"車両（バスを除く）",
	"車両（バス・二輪を除く）",
	"車両（バス・タクシーを除く）",
	"車両（バス・タクシー・二輪を除く）",
	"車両（バス・タクシー・自二輪を除く）",
	"車両（マイクロを除く）",
	"車両（マイクロ・タクシー・二輪を除く）",
	"車両（自二輪を除く）",
	"車両（観光バスを除く）",
	"車両（高速道路出入車両を除く）",
	"車両（大貨を除く）",
	"車両（大型・大特を除く）",
	"車両（大型・大特・タクシーを除く）",
	"車両（大型を除く）",
	"車両（大型バス・自二輪を除く）",
	"車両（大型バス・タクシーを除く）",
	"車両（二輪を除く）",
	"車両（二輪・タクシーを除く）",
	"車両（普通を除く）",
	"車両（大型バス・普通を除く）",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"二輪",
	"二輪（１２５ｃｃ以下を除く）",
	"二輪（７００ｃｃ以下を除く）",
	"二輪（７００ｃｃ以上を除く）",
	"自二輪",
	"自二輪（１２５ｃｃ以下を除く）",
	"自二輪（７００ｃｃ以下を除く）",
	"自二輪（７００ｃｃ以上を除く）",
	"",
	"",
	"乗用",
	"",
	"",
	"普貨",
	"",
	"普通",
	"普通（タクシーを除く）",
	"タクシー",
	"普乗",
	"普乗（タクシーを除く）",
	"貨物",
	"マイクロ",
	"バス",
	"大型バス",
	"大貨",
	"",
	"大貨等",
	"",
	"大特",
	"",
	"大型",
	"大型（マイクロを除く）",
	"大型（バスを除く）",
	"大型（大型バスを除く）",
	"",
	"",
	"",
	"最大積載量２ｔ以上の貨物",
	"最大積載量２．５ｔ以上の貨物",
	"最大積載量３ｔ以上の貨物",
	"最大積載量２．５ｔ以上の貨物",
	"最大積載量４ｔ以上の貨物",
	"最大積載量４．５ｔ以上の貨物",
	"",
	"",
	"",
	"",
	"総重量４ｔ以上",
	"総重量５ｔ以上",
	"総重量１４ｔ以上",
	"総重量１６ｔ以上",
	"総重量２０ｔ以上",
	"総重量８ｔ以上",
	"総重量８ｔ以上",
	"",
	"",
	"危険物積載車両"
};

/// 車種コードドメイン名一覧
const char* kkd::codes::weekday::kDomains[] = {
	"",
	"日・休日",
	"日・休日を除く",
	"休日",
	"休日を除く",
	"平日（月～土）",
	"日曜",
	"月曜",
	"火曜",
	"水曜",
	"木曜",
	"金曜",
	"土曜",
	"土日・休日",
	"土日・休日を除く",
	"全曜日",
	"日曜を除く",
	"日休の前日"
};

/// 交通規制レーンコードドメイン名一覧
const char* kkd::codes::regulation_lane::kDomains[] = {
	"未調査",
	"車線１",
	"車線２",
	"車線３",
	"車線４",
	"車線５",
	"車線６",
	"全車線"
};
