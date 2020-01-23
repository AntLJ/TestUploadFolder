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

/**
 *	INコードを取り扱うヘッダ
 *	@author	Fusatoshi Abe
 *	@date	2003/09/16		新規作成
 */

#ifndef	___IN_ATTR_DEF_H___
#define	___IN_ATTR_DEF_H___

#include <string>

namespace in
{
	namespace disp_param
	{
		enum code
		{
			string					= 0x10,			// 文字列
			line					= 0x30,			// 線
			area					= 0x50			// 面
		};
	};

	namespace road
	{
		namespace authenticity
		{
			enum code
			{
				valid			= 1,
				invalid			= 0
			};
		};

		// 道路管理者
		namespace manager
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				jh				= 1,					// 日本道路公団
				public3			= 2,					// 道路公団3社
				other_public	= 3,					// 道路公社
				country			= 4,					// 国
				pref			= 5,					// 都道府県
				city			= 6,					// 指定市
				other_city		= 7,					// 他の市町村(含東京23区)
				others			= 8						// その他の管理者
			};
		};

		// リンク種別
		namespace link_class
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				hibunri_honsen	= 1,					// 本線(上下非分離)
				bunri_honsen	= 2,					// 本線(上下分離)
				junction		= 3,					// 連結路(本線間の渡り線)
				crossing		= 4,					// 交差点内リンク
				ramp			= 5,					// 連結路(ランプ)リンク
				side			= 6,					// 本線と同一路線の側道リンク
				sa				= 7,					// SA等側線リンク
				bicycle			= 8,					// 自転車道等
				connection		= 9,					// 本線側道接続リンク
				roundabout		= 10					// 環状交差点リンク
			};
		};

		// バイパスフラグ
		namespace bypass
		{
			using namespace authenticity;
		};

		// 自動車専用道路フラグ
		namespace caronly
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				caronly			= 1,					// 自動車専用道路に該当
				not_caronly		= 2						// 自動車専用道路に非該当
			};
		};

		// 道路幅員
		namespace width
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				over_13m		= 1,					// 幅員13.0m以上
				under_13m		= 2,					// 幅員5.5m～13.0m未満
				over_3m			= 3,					// 幅員3.0m～5.5m未満
				under_3m		= 4						// 幅員3.0m未満
			};
		};

		// 交通規制種別コード
		namespace rest_class
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				no_rest			= 1,					// 規制なし
				no_passage		= 2,					// 通行禁止(条件なし)
				no_passage2		= 3,					// 通行禁止(条件あり)
				oneway_order	= 4,					// 一方通行(正方向・条件なし)
				oneway_reverse	= 5,					// 一方通行(逆方向・条件なし)
				oneway_order2	= 6,					// 一方通行(正方向・条件あり)
				oneway_reverse2	= 7,					// 一方通行(逆方向・条件あり)
				oneway_both		= 8						// 一方通行(正逆切り替えあり)
			};
		};

		// 表示種別コード
		namespace road_class
		{
			enum code
			{
				highway2		= 0x05001000,			// 都市間高速道路
				highway			= 0x05001100,			// 都市間高速道路(有料)
				city_highway	= 0x05002100,			// 都市高速道路(有料)
				kokudo			= 0x05003000,			// 一般国道
				kokudo2			= 0x05003100,			// 一般国道(有料)
				pref_mlocal		= 0x05004000,			// 主要地方道(都府県)
				pref_mlocal2	= 0x05004100,			// 主要地方道(都府県)(有料)
				city_mlocal		= 0x05005000,			// 主要地方道(指定市)
				city_mlocal2	= 0x05005100,			// 主要地方道(指定市)(有料)
				pref_road		= 0x05006000,			// 一般都道府県道
				pref_road2		= 0x05006100,			// 一般都道府県道(有料)
				city_road		= 0x05007000,			// 指定市の一般市道
				city_road2		= 0x05007100,			// 指定市の一般市道(有料)
				other_road		= 0x05009000,			// その他の道路
				other_road2		= 0x05009100,			// その他の道路(有料)
				other_road3		= 0x05008000,			// 格下げその他道
				detail_road		= 0x05009010,			// 細街路1
				detail_road2	= 0x05009020,			// 細街路2
				detail_road3	= 0x05009030,			// 細街路3
				dummy_link		= 0x0500d000,			// ダミーリンク
				dummy_link2		= 0x0500d010,			// 庭園路
				dummy_link3		= 0x0500d020,			// マッチング用リンク
				dummy_link4		= 0x0500d030,			// PEC用マッチングリンク
				dummy_link5		= 0x0500d100,			// BT用マッチングリンク
				ferry			= 0x0500f100,			// フェリー航路
				parking			= 0x0500e100,			// 駐車場内道路
				parking_shape	= 0x0500e000			// 駐車場外形
			};
		};

		// 経路種別コード
		namespace navi_class
		{
			enum code
			{
				highway			= 0x05001100,			// 都市間高速道路
				city_highway	= 0x05002100,			// 都市高速道路
				kokudo			= 0x05003000,			// 一般国道
				kokudo2			= 0x05003100,			// 一般国道(有料)
				pref_mlocal		= 0x05004000,			// 主要地方道(都府県)
				pref_mlocal2	= 0x05004100,			// 主要地方道(都府県)(有料)
				city_mlocal		= 0x05005000,			// 主要地方道(指定市)
				city_mlocal2	= 0x05005100,			// 主要地方道(指定市)(有料)
				pref_road		= 0x05006000,			// 一般都道府県道
				pref_road2		= 0x05006100,			// 一般都道府県道(有料)
				city_road		= 0x05007000,			// 指定市の一般市道
				city_road2		= 0x05007100,			// 指定市の一般市道(有料)
				other_road		= 0x05009000,			// その他の道路
				other_road2		= 0x05009100,			// その他の道路(有料)
				other_road3		= 0x05008000,			// 格下げその他道
				detail_road		= 0x05009010,			// 細街路1
				detail_road2	= 0x05009020,			// 細街路2
				detail_road3	= 0x05009030,			// 細街路3
				dummy_link3		= 0x0500d020,			// マッチング用リンク
				dummy_link4		= 0x0500d030,			// PEC用マッチングリンク
				dummy_link5		= 0x0500d100,			// BT用マッチングリンク
				ferry			= 0x0500f100,			// フェリー航路
				parking			= 0x0500e100,			// 駐車場内道路
				connectway		= 0x0500c000,			// 接続道路
				kkd_route		= 0x0500b000			// KKD経路誘導不適区間
			};
		};

		// 広域フラグ
		namespace wide_area
		{
			enum code
			{
				not_wide_area	= 0x00000000,			// 広域に存在しない
				wide_area		= 0x00000001,			// 広域に存在する
			};
		};

		// 駐車場種別
		namespace parking_class
		{
			enum code
			{
				unknown			= 0,					// 不明
				solid			= 1,					// 立体駐車場
				under			= 2,					// 地下駐車場
				both			= 3,					// 立体かつ地下駐車場
				tower			= 5,					// タワーパーキング
				auto_tower		= 11,					// 自動回転タワーパーキング
				flat			= 15					// 平面駐車場
			};
		};

		// 駐車場リンク種別
		namespace parkinglink_class
		{
			enum code
			{
				unknown			= 0,					// 未調査・作業中
				enter			= 1,					// 入口リンク
				exit			= 2,					// 出口リンク
				entrance		= 3,					// 出入口リンク
				premises		= 4,					// 構内道路リンク
				junction		= 5						// レベル間の渡りリンク
			};
		};

		// VICS区分
		namespace vics_class
		{
			enum code
			{
				highway			= 1,					// 高速道路
				city_highway	= 2,					// 都市高速道路
				street			= 3,					// 一般道路
				others			= 4						// その他
			};
		};

		// 傾斜フラグ
		namespace grad
		{
			enum code
			{
				unknown			= 0,					// 未調査
				up				= 1,					// 上り
				down			= 2,					// 下り
				flat			= 3						// 平坦
			};
		};

		// リンク内属性コード
		namespace segment_attr
		{
			enum code
			{
				tollgate			= 0x00000040,		// 料金所
				dirtroad			= 0x00000020,		// 未舗装
				underpass			= 0x00000010,		// アンダーパス
				crossing			= 0x00000008,		// 踏切
				doumon				= 0x00000004,		// 洞門等
				tunnel				= 0x00000002,		// トンネル
				bridge				= 0x00000001		// 橋・高架
			};

		};

		// ノード種別
		namespace node_class
		{
			enum code
			{
				unknown				= 0x01000000,		// 未調査・作業中
				crossing			= 0x01000001,		// 交差点ノード
				terminate			= 0x01000002,		// 行き止まり点ノード
				dummy				= 0x01000003,		// ダミーノード
				edge				= 0x01000004,		// 区画変更点ノード
				attr_change			= 0x01000005		// 属性変化点ノード
			};
		};

		// 信号機フラグ
		namespace signal
		{
			using namespace authenticity;
		};

		// 道路高さ有効・無効フラグ
		namespace height
		{
			using namespace authenticity;
		};

		// 交通規制条件コード
		namespace rest_cond
		{
			enum code
			{
				unknown					= 0,			// 未調査・作業中
				car						= 1,			// 車種のみ
				time					= 2,			// 時刻のみ
				day						= 3,			// 曜日のみ
				car_and_time			= 4,			// 車種及び時刻
				car_and_day				= 5,			// 車種及び曜日
				time_and_day			= 6,			// 時刻及び曜日
				all						= 7,			// 車種、時刻及び曜日
				others					= 8				// その他の条件
			};
		};

		// ノード規制種別
		namespace turn_class
		{
			enum code
			{
				turn_class				= 0x01000000	// ノード規制種別
			};
		}

		// 曜日指定コード
		namespace dayofweek
		{
			enum code
			{
				unknown					= 0,			// 未調査・作業中
				holiday					= 1,			// 日・休日
				except_holiday			= 2,			// 日・休日を除く
				public_holiday			= 3,			// 休日(日曜は除く)
				except_public_holiday	= 4,			// 休日を除く
				weekday					= 5,			// 平日
				sunday					= 6,			// 日曜
				monday					= 7,			// 月曜
				tuesday					= 8,			// 火曜
				wednesday				= 9,			// 水曜
				thursday				= 10,			// 木曜
				friday					= 11,			// 金曜
				saturday				= 12,			// 土曜
				weekend					= 13,			// 土日・休日
				except_weekend			= 14,			// 土日・休日を除く
				allday					= 15,			// 全曜日
				except_sunday			= 16,			// 日曜日を除く
				pre_holiday				= 17			// 日・休日の前日
			};
		};

		// 交差点名称種別コード
		namespace crossname_class
		{
			enum code
			{
				crossname				= 0x20015000
			};
		};

		// 規制種別
		namespace regulation_class
		{
			enum code
			{
				unknown					= 0,			// 未調査・作業中
				link					= 1,			// リンク規制
				node					= 2,			// ノード規制
				node_array				= 3				// ノード列規制
			};
		};

		// 規制条件
		namespace regulation_cond
		{
			enum code
			{
				unknown					= 0,			// 未調査
				no_condition			= 1,			// 通常規制(条件なし)
				with_condition			= 2				// 条件付き規制
			};
		};

		// 通行規制
		namespace passage_regulation
		{
			enum code
			{
				no_regulation			= 1,			// 規制なし
				no_passage				= 2,			// 通行禁止
				oneway_order			= 4,			// 一方通行(正方向)
				oneway_reverse			= 5,			// 一方通行(逆方向)
				bad_passage				= 6,			// 通行不可
				relation_passage		= 7,			// 通行禁止(関係者以外)
				etc_only				= 8				// ETC専用
			};
		};

		// ノード情報種別コード
		namespace node_info_class
		{
			enum code
			{
				crossname_nodir			= 0x20015001,	// 交差点名称(方向性なし)
				crossname_dir			= 0x20015002,	// 交差点名称(方向性あり)
				highway_ic_entrance		= 0x01000010,	// 高速道路IC入口ノード(一般道からの入路ノード)
				highway_ic_enter		= 0x01000011,	// 高速道路IC入路
				highway_ic_exit			= 0x01000012,	// 高速道路IC出路
				jct_enter				= 0x01000041,	// 入路JCTノード
				jct_exit				= 0x01000042,	// 出路JCTノード
				sapa_enter				= 0x01000051,	// SAPA入路
				sapa_exit				= 0x01000052,	// SAPA出路
				highway_dirguide		= 0x01000060,	// 方面案内(高速道路上)
				general_dirguide		= 0x01000070,	// 案内看板(一般道方面案内)
				noguide					= 0x01000080,	// ノーガイド
				guide_code				= 0x01000090,	// 強制誘導(コード)
				guide_string			= 0x01000091,	// 強制誘導(文字列)
				illust_file				= 0x01000101,	// イラストファイル名(誘導専用)
				illust_file2			= 0x01000102,	// イラストファイル名(ハイウェイモード専用)
				illust_file3			= 0x01000103	// イラストファイル名(誘導、ハイウェイモード用)
			};
		};

		// 地先種別
		namespace dirguide_class
		{
			enum code
			{
				general					= 1,			// 地先(一般)
				highway					= 2,			// 地先(有料)
				kokudo_no				= 3,			// 国道番号
				kendo_no				= 4,			// 県道番号
				others					= 5				// その他
			};
		};

		// 強制誘導
		namespace guide_class
		{
			enum code
			{
				noguide					= 0x00,			// 道なり(方面案内なし)
				straight				= 0x01,			// 直進(12時方向)
				right_front				= 0x02,			// 右前方向(1時方向)
				right_ahead				= 0x03,			// 右手前方向(2時方向)
				right					= 0x04,			// 右折(3時方向)
				right_behind			= 0x05,			// 右斜め後ろ方向(4時方向)
				right_back				= 0x06,			// 右戻る方向(5時方向)
				back					= 0x07,			// 戻る方向(6時方向)
				left_back				= 0x08,			// 左戻る方向(7時方向)
				left_behind				= 0x09,			// 左斜め後ろ方向(8時方向)
				left					= 0x0a,			// 左折(9時方向)
				left_ahead				= 0x0b,			// 左手前方向(10時方向)
				left_front				= 0x0c			// 左前方向(11時方向)
			};
		};

		// リンク拡張レイヤキーコード
		namespace link_info_class
		{
			enum code
			{
				kubun					= 1,			// 主従区分コード
				speed					= 2,			// 規制速度コード
				trip					= 3,			// 旅行速度
				island					= 4,			// 浮島許容フラグ
				area					= 6,			// 細街路エリア判別フラグ
				lane					= 7,			// 片側車線数
			};
		};

		// キーID
		namespace lext_key
		{
			enum code
			{
				kubun_code				= 1,			// 主従区分コード
				limit_speed				= 2,			// 規制速度
				trip_speed				= 3,			// 旅行速度
				island_flag				= 4,			// 浮島許容フラグ
				area_flag				= 6,			// 細街路エリアフラグ
				oneside_lane			= 7,			// 片側車線数
			};
		};

		// 主従区分コード
		namespace kubun_code
		{
			enum code
			{
				unknown					= 0,			// 未調査・作業中
				main_line				= 1,			// 主路線
				sub_line				= 2				// 従路線
			};
		};

		// 規制速度
		namespace limit_speed
		{
			enum code
			{
				unknown					= 0,			// 未調査
				limit_under30km			= 1,			// 30km/h未満
				limit_40km				= 2,			// 40km/h
				limit_50km				= 3,			// 50km/h
				limit_60km				= 4,			// 60km/h
				limit_70km				= 5,			// 70km/h
				limit_80km				= 6,			// 80km/h
				limit_100km				= 7				// 100km/h
			};
		};

		// 浮島フラグ
		namespace island_flag
		{
			using namespace authenticity;
		};

		// 細街路エリアフラグ
		namespace area_flag
		{
			enum code
			{
				unknown					= 0x00000000,	// 未調査
				year_1998				= 0x00000001,	// 1998年度
				year_1999				= 0x00000002,	// 1999年度
				year_2000_spr			= 0x00000004,	// 2000年度春
				year_2000_aut			= 0x00000008,	// 2000年度秋
				year_2001				= 0x00000010,	// 2001年度
				year_2002				= 0x00000020,	// 2002年度
				year_2003				= 0x00000040,	// 2003年度
				year_2004				= 0x00000080,	// 2004年度
				year_2005				= 0x00000100,	// 2005年度
				year_2006				= 0x00000200,	// 2006年度
				year_2007				= 0x00000400	// 2007年度
			};

			enum kisei_chousa
			{
				no_rest					= 0x00000000,	// 規制調査なし
				rest					= 0x80000000	// 規制調査あり
			};
		};
	};
};

#endif	//___IN_ATTR_DEF_H___
