// CDS関連レイヤ用 原稿番号-種別 対応マップ
// CDS対応では、ANNOTATION_CLASSやBACKGROUND_CLASSは使用しないので、
// 内部で対応表を持たせる
// TODO: Excelで持たせた方が楽そう

// フォーマット
// BG_MAP( 原稿番号, コード, ドメイン, レイヤ名 )

// 使用例
// #define BG_MAP( msno, code, domain, layer ) if( 0 == m_strClassName.CompareNoCase(layer::kTableName) ) \
// 	                                           { BACKGROUND_RULE br = {0}; \
// 	                                             br.lClass = code; \
// 	                                             br.strClass_Name = domain; \
// 	                                             br.strMS_NO = msno; \
// 									             m_BackgroundRule.emplace_back(br); }
// #include "BackgroundClassTable.h"
// #undef BG_MAP

// CITY_SITE_GS
BG_MAP( _T("Z"), 0, _T("未調査・作業中"), city_site_gs )
BG_MAP( _T("3"), 14, _T("植栽地"), city_site_gs )
BG_MAP( _T("515"), 20, _T("河川"), city_site_gs )
BG_MAP( _T("520"), 23, _T("海"), city_site_gs )
BG_MAP( _T("522"), 24, _T("池・湖"), city_site_gs )
BG_MAP( _T("H1"), 30, _T("都市間高速（有料）"), city_site_gs )
BG_MAP( _T("R2"), 31, _T("国道"), city_site_gs )
BG_MAP( _T("R3"), 32, _T("主要地方道"), city_site_gs )
BG_MAP( _T("R4"), 33, _T("一般都道府県道"), city_site_gs )
BG_MAP( _T("6"), 34, _T("歩道"), city_site_gs )
BG_MAP( _T("16"), 35, _T("庭園路"), city_site_gs )
BG_MAP( _T("5A"), 37, _T("中央分離帯（標準）"), city_site_gs )
BG_MAP( _T("H1F"), 38, _T("都市間高速（無料）"), city_site_gs )
BG_MAP( _T("15"), 51, _T("舗装地その他敷地"), city_site_gs )
BG_MAP( _T("H2"), 70, _T("都市高速（有料）"), city_site_gs )
BG_MAP( _T("H2F"), 71, _T("都市高速（無料）"), city_site_gs )
BG_MAP( _T("H3"), 72, _T("有料道路ポリゴン"), city_site_gs )
BG_MAP( _T("R6"), 73, _T("その他幹線道路"), city_site_gs )
BG_MAP( _T("R7"), 74, _T("その他道"), city_site_gs )
BG_MAP( _T(""), 101, _T("高速道路ポリゴン（立体交差）"), city_site_gs )
BG_MAP( _T(""), 111, _T("都市高速道路ポリゴン（立体交差）"), city_site_gs )
BG_MAP( _T(""), 121, _T("国道ポリゴン（立体交差）"), city_site_gs )
BG_MAP( _T(""), 131, _T("主要地方道ポリゴン（立体交差）"), city_site_gs )
BG_MAP( _T(""), 141, _T("一般都道府県道ポリゴン（立体交差）"), city_site_gs )
BG_MAP( _T(""), 161, _T("歩道ポリゴン（全道路）"), city_site_gs )
BG_MAP( _T(""), 171, _T("有料道路ポリゴン（立体交差）"), city_site_gs )

// CITY_LINE_GS
BG_MAP( _T("Z"), 0, _T("未調査・作業中"), city_line_gs )
BG_MAP( _T("2"), 2, _T("破線表示"), city_line_gs )
BG_MAP( _T("8"), 4, _T("水門"), city_line_gs )
BG_MAP( _T("9"), 5, _T("砂防ダム"), city_line_gs )
BG_MAP( _T("KK2"), 15, _T("階段装飾線"), city_line_gs )
BG_MAP( _T("5"), 20, _T("中央分離帯（ライン）"), city_line_gs )
BG_MAP( _T("4"), 21, _T("道路背景"), city_line_gs )
BG_MAP( _T(""), 101, _T("等高線"), city_line_gs )
BG_MAP( _T(""), 111, _T("都道府県界"), city_line_gs )
BG_MAP( _T(""), 112, _T("市町村界"), city_line_gs )
BG_MAP( _T(""), 113, _T("区界"), city_line_gs )
BG_MAP( _T(""), 114, _T("大字界"), city_line_gs )
BG_MAP( _T(""), 115, _T("線小字界"), city_line_gs )
BG_MAP( _T(""), 121, _T("河川"), city_line_gs )
BG_MAP( _T(""), 122, _T("線池・湖"), city_line_gs )
BG_MAP( _T(""), 123, _T("海岸線"), city_line_gs )
BG_MAP( _T(""), 131, _T("線道路"), city_line_gs )
BG_MAP( _T(""), 132, _T("庭園路"), city_line_gs )
BG_MAP( _T(""), 133, _T("歩道橋"), city_line_gs )
BG_MAP( _T(""), 141, _T("タンク"), city_line_gs )

// CITY_RAILWAY_GS
BG_MAP( _T("Z"), 0, _T("未調査・作業中"), city_railway_gs )
BG_MAP( _T("20"), 1, _T("普通鉄道"), city_railway_gs )
BG_MAP( _T("25"), 2, _T("地下鉄"), city_railway_gs )
BG_MAP( _T("30"), 3, _T("路面鉄道"), city_railway_gs )
BG_MAP( _T(""), 101, _T("特殊軌道"), city_railway_gs )

// BUILDING_GS
BG_MAP( _T("Z"), 0, _T("未調査・作業中"), building_gs )
BG_MAP( _T("20"), 1, _T("普通鉄道駅（地上）"), building_gs )
BG_MAP( _T("25"), 2, _T("地下鉄駅（地上）"), building_gs )
BG_MAP( _T("20H"), 3, _T("地下鉄以外駅ホーム（地上）"), building_gs )
BG_MAP( _T("25H"), 4, _T("地下鉄駅ホーム（地上）"), building_gs )
BG_MAP( _T("1"), 5, _T("官公庁"), building_gs )
BG_MAP( _T("3"), 6, _T("病院"), building_gs )
BG_MAP( _T("2"), 7, _T("学校"), building_gs )
BG_MAP( _T("6"), 8, _T("高層集合住宅"), building_gs )
BG_MAP( _T("7"), 9, _T("工場・銀行・ビル"), building_gs )
BG_MAP( _T("5"), 11, _T("ホテル"), building_gs )
BG_MAP( _T("4"), 12, _T("娯楽・デパート"), building_gs )
BG_MAP( _T("9"), 13, _T("駅ビル"), building_gs )
BG_MAP( _T("12"), 16, _T("市場"), building_gs )
BG_MAP( _T("13"), 17, _T("工場"), building_gs )
BG_MAP( _T("14"), 18, _T("倉庫"), building_gs )
BG_MAP( _T("30"), 19, _T("路面鉄道駅（地上）"), building_gs )
BG_MAP( _T("16"), 26, _T("アーケード（面）"), building_gs )
BG_MAP( _T("17"), 27, _T("料金所（面）"), building_gs )
BG_MAP( _T("27"), 28, _T("新交通駅（地上）"), building_gs )
BG_MAP( _T("26"), 29, _T("モノレール駅（地上）"), building_gs )
BG_MAP( _T("28"), 30, _T("ケーブルカー駅（地上）"), building_gs )
BG_MAP( _T("18"), 31, _T("自衛隊・米軍"), building_gs )
BG_MAP( _T("10"), 32, _T("ガソリンスタンド"), building_gs )
BG_MAP( _T(""), 99, _T("テナント属性確認"), building_gs )

// CITY_STATION_GS
BG_MAP( _T("Z"), 0, _T("未調査・作業中"), city_station_gs )
BG_MAP( _T("20"), 1, _T("普通鉄道駅（地下）"), city_station_gs )
BG_MAP( _T("25"), 2, _T("地下鉄駅（地下）"), city_station_gs )
BG_MAP( _T("27"), 3, _T("新交通システム（地下）"), city_station_gs )
BG_MAP( _T("26"), 4, _T("モノレール（地下）"), city_station_gs )
BG_MAP( _T("28"), 5, _T("ケーブルカー（地下）"), city_station_gs )