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

#ifndef	_SHP05_H_
#define	_SHP05_H_

namespace shp05 {

namespace schema
{
	extern LPCTSTR kObjectId;
	extern LPCTSTR kShape;

	namespace annotation
	{
		extern LPCTSTR kAnnoClass_c;
		extern LPCTSTR kNameString;
		extern LPCTSTR kDispScale;
		extern LPCTSTR kFontSize;
		extern LPCTSTR kMark;
	}

	namespace admin
	{
		extern LPCTSTR kSuffix;

		extern LPCTSTR kCityCode;
	}

	namespace topmap
	{
		// 背景ポリゴン
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// 背景ライン
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}
		// 段彩
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace middlemap
	{
		// 背景ポリゴン
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// 背景ライン
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// 段彩
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;
	}

	namespace basemap
	{
		// 背景ポリゴン
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSC4BgClass_c;
			extern LPCTSTR kSC3BgClass_c;
			extern LPCTSTR kSC2BgClass_c;
			extern LPCTSTR kSC1BgClass_c;
		}
		// 背景ライン
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// 段彩
		namespace contour
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kCntClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// 鉄道
		namespace railway
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRalClass_c;
			extern LPCTSTR kSC1Disp_f;
			extern LPCTSTR kSC2Disp_f;
			extern LPCTSTR kSC3Disp_f;
			extern LPCTSTR kSC4Disp_f;
			extern LPCTSTR kUnder_f;
		}
		// 中縮駅舎
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
			extern LPCTSTR kUnder_f;
		}
		// 行政界名称
		namespace admin_table
		{
			extern LPCTSTR kCityCode;
			extern LPCTSTR kPrefName;
			extern LPCTSTR kCityName;
			extern LPCTSTR kCityYomi;
		}
	}

	namespace citymap
	{
		// 背景ポリゴン
		namespace back_poly
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
			extern LPCTSTR kColorCode;
		}
		// 背景ライン
		namespace back_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBgClass_c;
		}

		using namespace annotation;

		using namespace admin;

		// 鉄道
		namespace railway
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRalClass_c;
			extern LPCTSTR kUnder_f;
		}
		// 建物
		namespace building
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldClass_c;
			extern LPCTSTR kCorridor_f;
		}
		// 地下駅舎
		namespace station
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kStaClass_c;
		}
		// 建物装飾線
		namespace bld_line
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kBldId;
		}
		// 建物ビル名	
		namespace bld_name
		{
			extern LPCTSTR kSuffix;
			
			extern LPCTSTR kBldId;
			extern LPCTSTR kBldName;
		}
	}

	namespace road
	{
		namespace link_id
		{
			extern LPCTSTR kLinkId;
		}

		namespace link_dir
		{
			extern LPCTSTR kLinkDir;
		}

		namespace kind {
			extern LPCTSTR kKind_c;
		}

		// 道路リンク
		namespace road_link
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kFromNodeId;
			extern LPCTSTR kToNodeId;
			extern LPCTSTR kRoadClass_c;
			extern LPCTSTR kNaviClass_c;
			extern LPCTSTR kLinkClass_c;
			extern LPCTSTR kManager_c;
			extern LPCTSTR kWidth_c;
			extern LPCTSTR kWideArea_f;
			extern LPCTSTR kIsland_f;
			extern LPCTSTR kBypass_f;
			extern LPCTSTR kCaronly_f;
			extern LPCTSTR kRoadNo;
			extern LPCTSTR kRoadCode;
			extern LPCTSTR kLaneCount;
			extern LPCTSTR kNoPassage_c;
			extern LPCTSTR kOneway_c;
		}
		// リンク内属性リンク化対応版道路リンク
		namespace road_link_a
		{
			using namespace shp05::schema::road::road_link;

			extern LPCTSTR kSuffix;

			extern LPCTSTR kDivId;
			extern LPCTSTR kFromDivId;
			extern LPCTSTR kToDivId;
			extern LPCTSTR kRoadAttr_c;
			
			namespace road_attr {
				enum ECode {
					kBridge			= 1, ///< 橋・高架
					kTunnel			= 2, ///< トンネル
					kDoumon			= 4, ///< 洞門
					kRailCross		= 8, ///< 踏切
					kUnderPass		= 16, ///< アンダーパス
					kDirtRoad		= 32, ///< 未舗装道路
				};
			}

		}

		// 道路ノード
		namespace road_node
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kSignal_f;
			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}

		// リンク内属性リンク化対応版道路ノード
		namespace road_node_a
		{
			using namespace shp05::schema::road::road_node;

			extern LPCTSTR kSuffix;

			extern LPCTSTR kDivId;
		}

		// リンク内属性
		namespace segment_attr
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			extern LPCTSTR kLinkAttr_c;
			
			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< 橋・高架
					kTunnel			= 2, ///< トンネル
					kDoumon			= 3, ///< 洞門
					kRailCross		= 4, ///< 踏切
					kUnderPass		= 5, ///< アンダーパス
					kDirtRoad		= 7  ///< 未舗装道路
				};
			}
		}
		namespace segment_attr_vehicle
		{
			using namespace shp05::schema::road::segment_attr;

			extern LPCTSTR kSuffix;

			namespace link_attr {
				enum ECode {
					kBridge			= 1, ///< 橋・高架
					kTunnel			= 2, ///< トンネル
					kDoumon			= 3, ///< 洞門
					kRailCross		= 4, ///< 踏切
					kUnderPass		= 5, ///< アンダーパス
					kDirtRoad		= 7,  ///< 未舗装道路
					kTunnelSquare      = 100, ///< トンネル（四角）
					kTunnelCircle      = 101, ///< トンネル（丸）
					kRailCrossKamaboko = 102 ///< 踏切（かまぼこ型）
				};
			}
		}

		namespace reg_condition
		{
			extern LPCTSTR kFromMonth;
			extern LPCTSTR kFromDay;
			extern LPCTSTR kToMonth;
			extern LPCTSTR kToDay;
			extern LPCTSTR kFromHour;
			extern LPCTSTR kFromMin;
			extern LPCTSTR kToHour;
			extern LPCTSTR kToMin;
			extern LPCTSTR kDayOfWeek;
		}
		// 通行禁止
		namespace nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace reg_condition;
		}
		// 一方通行
		namespace oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace reg_condition;
		}
		// 車種別規制
		namespace vehicle_reg
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace reg_condition;

			extern LPCTSTR kRegType_c;
			extern LPCTSTR kHeight;
			extern LPCTSTR kWidth;
			extern LPCTSTR kWeight;
			extern LPCTSTR kLength;
			extern LPCTSTR kLoadage;
			extern LPCTSTR kDanger_f;

			namespace reg_type {
				enum ECode {
					kOneway    = 0,            //!< 一方通行（正方向）
					kROneway   = 1,            //!< 一方通行（逆方向）
					kNoPassage = 2,            //!< 通行禁止
				};

			}
		}
		// 車種タイプ
		namespace vehicle_type
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kRegId;
			extern LPCTSTR kVehicleRegCode;
			extern LPCTSTR kExclude_f;
		}
		// 車種別規制マスタ
		namespace vehicle_reg_master
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kVehicleRegCode;
			extern LPCTSTR kVehicleRegName;
			extern LPCTSTR kOgataJoyo_f;
			extern LPCTSTR kTokuteiChuJo_f;
			extern LPCTSTR kChugataJoyo_f;
			extern LPCTSTR kFutsuJoyo_f;
			extern LPCTSTR kFutsuJoyoKei_f;
			extern LPCTSTR kOgataKamotsu_f;
			extern LPCTSTR kTokuteiChuKa_f;
			extern LPCTSTR kChugataKamotsu_f;
			extern LPCTSTR kFutsuKamotsu_f;
			extern LPCTSTR kFutsuKamotsuKei_f;
			extern LPCTSTR kOgataTokushu_f;
			extern LPCTSTR kKogataTokushu_f;
			extern LPCTSTR kOgataBike_f;
			extern LPCTSTR kFutsuBike_f;
			extern LPCTSTR kKogataBike_f;
			extern LPCTSTR kTaxi_f;
			extern LPCTSTR kBus_f;
			extern LPCTSTR kMiniBike_f;
			extern LPCTSTR kTrolley_f;
			extern LPCTSTR kBicycle_f;
			extern LPCTSTR kExceptBicycle_f;
		}

		namespace link_queue
		{
			extern LPCTSTR kFromLinkId;
			extern LPCTSTR kToLinkId;
			extern LPCTSTR kMidCount;
			extern LPCTSTR kMidLinkId;
		}
		// リンク列規制
		namespace turnreg
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			using namespace reg_condition;
		}
		// 方面案内
		namespace dir_guide
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kGuideClass;
			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
			extern LPCTSTR kBoardPtn;
			extern LPCTSTR kBoardSeq;
		}
		// レーン情報
		namespace lane
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kLaneDir;
			extern LPCTSTR kAppLane;
			extern LPCTSTR kRightChange;
			extern LPCTSTR kLeftChange;
			extern LPCTSTR kPassable;
		}
		// 強制誘導
		namespace guide
		{
			extern LPCTSTR kSuffix;

			using namespace link_queue;
			extern LPCTSTR kGuideType;
		}
		// VICS
		namespace vics
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kVicsMesh;
			extern LPCTSTR kVicsId;
			extern LPCTSTR kVicsClass;
			using namespace link_dir;
			using namespace link_id;
		}
		// 進入方向別交差点
		namespace intersection
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNameKanji;
			extern LPCTSTR kNameYomi;
		}
		// 道路名称
		namespace road_code
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kRoadType;
			extern LPCTSTR kRoadCode;
			extern LPCTSTR kRoadName;
			extern LPCTSTR kRoadYomi;

			/// 路線種別コード定義
			namespace road_type {
				enum ECode {
					kHighway     =   1, //!< 高速道路
					kToll        =   2, //!< 有料道路
					kCityHighway =   3, //!< 首都高速道路
					kHanshin     =   4, //!< 阪神高速道路
					kNagoya      =   5, //!< 名古屋都市高速道路
					kFukuoka     =   6, //!< 福岡都市高速道路
					kKitakyu     =   7, //!< 北九州都市高速道路
					kHiroshima   =   8, //!< 広島高速道路
					kFerry       =   9, //!< フェリー航路
					kNormal      =  10, //!< 一般街道名称
					kNormalToll  =  11, //!< 一般有料道路
					kWalk		 = 101, //!< 歩行者通路名称
				};
			}
		}
		// 道路・歩行者リンク
		namespace road_walk_link
		{
			extern LPCTSTR kSuffix;

			using namespace shp05::schema::road::road_link;
			extern LPCTSTR kWalkClass_c;
			extern LPCTSTR kFloorMove_c;
			extern LPCTSTR kWalkAttr_c;
			extern LPCTSTR kWalkable_c;
			extern LPCTSTR kSurface_c;
			extern LPCTSTR kWheelChair_c;
		}
		// リンク内属性リンク化対応版道路・歩行者リンク
		namespace road_walk_link_a
		{
			using namespace shp05::schema::road::road_walk_link;
			
			extern LPCTSTR kDivId;
			extern LPCTSTR kFromDivId;
			extern LPCTSTR kToDivId;
			extern LPCTSTR kRoadAttr_c;
			
			namespace road_attr {
				enum ECode {
					kBridge			= 1, ///< 橋・高架
					kTunnel			= 2, ///< トンネル
					kDoumon			= 4, ///< 洞門
					kRailCross		= 8, ///< 踏切
					kUnderPass		= 16, ///< アンダーパス
					kDirtRoad		= 32, ///< 未舗装道路
				};
			}
		}
		// 道路・歩行者ノード
		namespace road_walk_node
		{
			extern LPCTSTR kSuffix;

			using namespace shp05::schema::road::road_node;
			extern LPCTSTR kWalkNodeClass_c;
		}
		// リンク内属性リンク化対応版道路・歩行者ノード
		namespace road_walk_node_a
		{
			using namespace shp05::schema::road::road_walk_node;
			extern LPCTSTR kDivId;
		}
		
		// 道路・歩行者通行禁止
		namespace road_walk_nopassage_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace kind;
			using namespace reg_condition;
		}
		// 道路歩行者一方通行
		namespace road_walk_oneway_condition
		{
			extern LPCTSTR kSuffix;

			using namespace link_id;
			using namespace link_dir;
			using namespace kind;
			using namespace reg_condition;
		}
		// 道路・歩行者ノード－駅出入口POI関連テーブル
		namespace rel_sgp
		{
			extern LPCTSTR kSuffix;

			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// 道路・歩行者ノード－一般POI関連テーブル
		namespace rel_walk_node_general_poi
		{
			extern LPCTSTR kNodeId;
			extern LPCTSTR kPoiId;
		}
		// 道路リンク横断可否テーブル
		namespace roadcrossing
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kCrs_f;
			extern LPCTSTR kPedeZone_f;
		}

		// 道路・歩行者リンク関連付けテーブル
		namespace buddy
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kBaseLinkId;
			extern LPCTSTR kBaseDivId;
			extern LPCTSTR kRelLinkId;
			extern LPCTSTR kRelDivId;
			extern LPCTSTR kSide_c;

			namespace side {
				enum ECode {
					kRight		= 1, ///< 右
					kLeft		= 2, ///< 左
				};
			}
		}

		// 道路・歩行者リンク相対高さテーブル
		namespace rel_height
		{
			extern LPCTSTR kTableName;

			extern LPCTSTR kLinkId;
			extern LPCTSTR kLinkDivId;
			extern LPCTSTR kFromLevel;
			extern LPCTSTR kToLevel;
		}

		// 汎用向け道路・歩行者リンク相対高さレイヤ
		namespace height_node
		{
			extern LPCTSTR kLayerName;

			extern LPCTSTR kLinkId1;
			extern LPCTSTR kDivId1;
			extern LPCTSTR kRelHeight1;
			extern LPCTSTR kLinkId2;
			extern LPCTSTR kDivId2;
			extern LPCTSTR kRelHeight2;
		}
		

		namespace kind {
			enum ECode {
				kRoad			= 1, ///< 道路
				kWalk			= 2, ///< 歩行者
			};
		}
		namespace walkable {
			enum ECode
			{
				kNotWalk  = 0,  ///< 歩行禁止
				kHumanCar = 1,  ///< 人車共用
				kWalkOnly = 2,  ///< 歩行者専用
			};
		} // walkable_class
			
	}
	// 道路標高データ
	namespace road_alt
	{
		extern LPCTSTR kSuffix;

		extern LPCTSTR kLinkId;
		extern LPCTSTR kNodeId;
		extern LPCTSTR kSequence;
		extern LPCTSTR kHeightF;
	}

}

}
#endif	//_SHP05_H_
