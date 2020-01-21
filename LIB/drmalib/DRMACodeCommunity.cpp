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

// DRMACodeCommunity.cpp: DRMACodeCommunity クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "DRMACodeCommunity.h"

const int drma::admin1::kRecordId = drma::kAdmin1Id;
const int drma::admin2::kRecordId = drma::kAdmin2Id;
const int drma::base_road_node::kRecordId = drma::kBaseRoadNodeId;
const int drma::base_road_link::kRecordId = drma::kBaseRoadLinkId;
const int drma::base_road_link_attr::kRecordId = drma::kBaseRoadLinkAttrId;
const int drma::road_link_relation::kRecordId = drma::kRoadLinkRelationId;
const int drma::beacon_location::kRecordId = drma::kBeaconLocationId;
const int drma::all_road_node::kRecordId = drma::kAllRoadNodeId;
const int drma::all_road_link::kRecordId = drma::kAllRoadLinkId;
const int drma::site::kRecordId = drma::kSiteId;
const int drma::admin_location::kRecordId = drma::kAdminLocationId;
const int drma::railway_location::kRecordId = drma::kRailwayLocationId;
const int drma::facil_location::kRecordId = drma::kFacilLocationId;
const int drma::facil_shape::kRecordId = drma::kFacilShapeId;
const int drma::place_name_location::kRecordId = drma::kPlaceNameLocationId;
const int drma::all_road_link_attr::kRecordId = drma::kAllRoadLinkAttrId;

/// ノード種別コードドメイン名一覧
const char* drma::codes::b::kDomains[] = {
	"",
	"交差点ノード",
	"行き止まりノード",
	"ダミー点ノード",
	"区画辺交点ノード",
	"属性変化点ノード",
	"交通管制上必要なノード"
};

/// 交差点通行コードドメイン名一覧
const char* drma::codes::c::kDomains[] = {
	"未調査",
	"通行可",
	"通行不可",
	"条件付（車種）通行可",
	"条件付（時刻、曜日）通行可",
	"条件付（時刻、曜日、車種）通行可"
};

/// 管理者コードドメイン名一覧
const char* drma::codes::d::kDomains[] = {
	"未調査",
	"日本道路公団",
	"道路３公団",
	"道路公社",
	"国",
	"都道府県",
	"指定市",
	"他の市町村等（含東京23区）",
	"その他の管理者"
};

/// 道路種別コードドメイン名一覧
const char* drma::codes::e::kDomains[] = {
	"未調査",
	"高速自動車道路",
	"都市高速道路（含指定都市高速道路）",
	"一般国道",
	"主要地方道（都道府県道）",
	"主要地方道（指定市道）",
	"一般都道府県道",
	"指定市の一般市道",
	"",	// [8]は抜けているため
	"その他道路"
};

/// 主・従道路区分コードドメイン名一覧
const char* drma::codes::f::kDomains[] = {
	"未調査",
	"主道路",
	"従道路"
};

/// リンク種別コードドメイン名一覧
const char* drma::codes::h::kDomains[] = {
	"未調査",
	"本線（上下線非分離）リンク",
	"本線（上下分離）リンク",
	"連結路（本線間の渡り線）リンク",
	"交差点内リンク",
	"連結路（ランプ）リンク",
	"本線と同一路線の側道リンク",
	"ＳＡ等側線リンク",
	"自転車道等リンク",
	"本線側道接続リンク"
};

/// リンク通行可・自動車不可コードドメイン名一覧
const char* drma::codes::i::kDomains[] = {
	"未調査",
	"自動車通行可",
	"自動車通行不可",
	"未供用（含工事中）",
	"計画決定"
};

/// 異常気象時通行規制区間種別コードドメイン名一覧
const char* drma::codes::j::kDomains[] = {
	"未調査",
	"規制無",
	"雨量規制有",
	"雪規制有",
	"その他規制有"
};

/// 道路幅員区分コードドメイン名一覧
const char* drma::codes::k::kDomains[] = {
	"未調査",
	"幅員13.0m以上",
	"幅員5.5m以上～13.0m未満",
	"幅員3.0m以上～5.5m未満",
	"幅員3.0m未満"
};

/// 車線数コードドメイン名一覧
const char* drma::codes::l::kDomains[] = {
	"未調査",
	"1車線",
	"2車線",
	"3車線",
	"4車線",
	"5車線",
	"6車線異常"
};

/// 交通規制種別コードドメイン名一覧
const char* drma::codes::m::kDomains[] = {
	"未調査",
	"規制無し",
	"通行禁止（条件無）",
	"通行禁止（条件付）",
	"一方通行(正方向、条件無)",
	"一方通行(逆方向、条件無)",
	"一方通行(正方向、条件付)",
	"一方通行(逆方向、条件付)",
	"一方通行(正逆切替え有り)"
};

/// 交通規制条件種別コードドメイン名一覧
const char* drma::codes::n::kDomains[] = {
	"未調査",
	"車種のみ",
	"時刻のみ",
	"曜日のみ",
	"車種及び時刻",
	"車種及び曜日",
	"曜日及び時刻",
	"車種、時刻及び曜日",
	"その他の条件"
};

/// 規制速度コードドメイン名一覧
const char* drma::codes::o::kDomains[] = {
	"未調査",
	"30km/h未満",
	"40km/h",
	"50km/h",
	"60km/h",
	"70km/h",
	"80km/h",
	"100km/h"
};

/// 形状データ取得資料コードドメイン名一覧
const char* drma::codes::o10::kDomains[] = {
	"未調査",
	"91年度以降道路管理者等資料",
	"縮尺1/2.5万地形図",
	"90年度以前道路管理者等資料",
	"縮尺1/1万以上の地形図",
	"土地区画整理事業者等資料",
	"市町村道整理事業者資料",
	"農道・林道・港湾整備事業者資料",
	"現地調査"
};

/// 属性種別コードドメイン名一覧
const char* drma::codes::p::kDomains[] = {
	"",
	"橋・高架",
	"トンネル",
	"洞門等",
	"踏切",
	"他の施設をアンダーパス",
	"歩道橋",
	"料金所（ETC無し）",
	"道路通称名",
	"料金所（ETC有り）",
	"料金所（ETC専用）",
	"バイパス道路名",
};

/// 水系種別コードドメイン名一覧
const char* drma::codes::r::kDomains[] = {
	"",
	"面表示水系",
	"線表示水系"
};

/// 線種コードドメイン名一覧
const char* drma::codes::s::kDomains[] = {
	"意味なし",
	"実境界線",
	"区画辺線",
	"仮想線"
};

/// 鉄道種別コードドメイン名一覧
const char* drma::codes::u::kDomains[] = {
	"未調査",
	"新幹線",
	"JR線",
	"JR線以外（普通鉄道）",
	"JR線以外（路面鉄道",
	"JR線以外（モノレール）",
	"JR線以外（新交通）",
	"その他"
};

/// 該当有無コードドメイン名一覧
const char* drma::codes::y::kDomains[] = {
	"未調査・意味なし",
	"該当あり",
	"該当無し"
};
