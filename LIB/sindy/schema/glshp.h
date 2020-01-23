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

#ifndef	_GLSHP_H_
#define	_GLSHP_H_

// グローバルSHP　スキーマ定義

namespace glshp
{
namespace schema
{
namespace objectid
{
	extern const _TCHAR kObjectId[];			//!< OBJECTID
}
namespace base_table
{
	using namespace glshp::schema::objectid;
	extern const _TCHAR kCountry_C[];			//!< 国コード
}
namespace base_feature
{
	using namespace glshp::schema::base_table;
	extern const _TCHAR kShape[];				//!< 形状フィールド名
}
namespace scale_class
{
	extern const _TCHAR kSc1Class_C[];			//!< スケール1背景種別
	extern const _TCHAR kSc2Class_C[];			//!< スケール2背景種別
	extern const _TCHAR kSc3Class_C[];			//!< スケール3背景種別
	extern const _TCHAR kSc4Class_C[];			//!< スケール4背景種別
}
namespace disp_scale
{
	extern const _TCHAR kDispScale1_F[];		//!< スケール1表示フラグ
	extern const _TCHAR kDispScale2_F[];		//!< スケール2表示フラグ
	extern const _TCHAR kDispScale3_F[];		//!< スケール3表示フラグ
	extern const _TCHAR kDispScale4_F[];		//!< スケール4表示フラグ
}
namespace lang_type
{
	extern const _TCHAR kOnJpn[];				//!< メイン名称(日本語)
	extern const _TCHAR kAnJpn[];				//!< サブ名称(日本語)
	extern const _TCHAR kOnEng[];				//!< メイン名称(英語)
	extern const _TCHAR kAnEng[];				//!< サブ名称(英語)
	extern const _TCHAR kOnTha[];				//!< メイン名称(タイ語)
	extern const _TCHAR kAnTha[];				//!< サブ名称(タイ語)
	extern const _TCHAR kOnInd[];				//!< メイン名称(インドネシア語)
	extern const _TCHAR kAnInd[];				//!< サブ名称(インドネシア語)
	extern const _TCHAR kOnMay[];				//!< メイン名称(マレー語)
	extern const _TCHAR kAnMay[];				//!< サブ名称(マレー語)
	extern const _TCHAR kOnVie[];				//!< メイン名称(ベトナム語)
	extern const _TCHAR kAnVie[];				//!< サブ名称(ベトナム語)
}
namespace under_f
{
	extern const _TCHAR kUnder_F[];				//!< 地下フラグ
}

///// 地図系 /////
	// LAND_USE
	namespace land_use
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(ランドユース)
	}
	// LAND_COVER
	namespace land_cover
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(ランドカバー)
	}
	// WATERWAY
	namespace waterway
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(水系)
	}
	// ISLAND
	namespace island
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::scale_class;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(島)
	}
	// 背景ライン
	namespace base_line
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< 識別子(背景ライン)

		extern const _TCHAR kBgClass_C[];		//!< 背景ライン種別
	}
	// PROVINCE
	namespace province
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(行政界(州・地方))

		extern const _TCHAR kAdminCode[];		//!< 行政界コード
	}
	// DISTRICT
	namespace district
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(行政界(地区))

		extern const _TCHAR kAdminCode[];		//!< 行政界コード
	}
	// SUB DISTRICT
	namespace sub_district
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(行政界(分区))

		extern const _TCHAR kAdminCode[];		//!< 行政界コード
	}
	// POSTAL DISTRICT
	namespace postal_district
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< 識別子(郵便番号)

		extern const _TCHAR kPostCode[];		//!< 郵便番号コード
	}
	// BASE_RAILWAY
	namespace base_railway
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< 識別子(鉄道路線)

		extern const _TCHAR kRalCls_C[];		//!< 鉄道ライン種別
	}
	// BASE_STATION
	namespace base_station
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::disp_scale;
		using namespace glshp::schema::lang_type;
		using namespace glshp::schema::under_f;

		extern const _TCHAR kSuffix[];			//!< 識別子(駅舎)

		extern const _TCHAR kStaCls_C[];		//!< 駅舎種別
	}
	// BUILDING
	namespace building
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(建物)

		extern const _TCHAR kBldCls_C[];		//!< 建物種別
		extern const _TCHAR kFloors[];			//!< 階数情報
		extern const _TCHAR kCorridor_F[];		//!< 渡り廊下フラグ
	}
	// CITY_STATION
	namespace city_station
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(地下駅舎)

		extern const _TCHAR kStaCls_C[];		//!< 地下駅舎種別
	}
///// 道路系 /////
namespace linkid
{
	extern const _TCHAR kLinkId[];				//!< 道路リンクID
}
namespace infid
{
	extern const _TCHAR kInfId[];				//!< InfID
}
namespace link_queue
{
	using namespace glshp::schema::base_table;
	using namespace glshp::schema::linkid;
	using namespace glshp::schema::infid;

	extern const _TCHAR kSequence[];		//!< シーケンス番号
}
namespace inf_table
{
	using namespace glshp::schema::base_table;
	using namespace glshp::schema::infid;
}
namespace reg_condition
{
	using namespace glshp::schema::base_table;

	extern const _TCHAR kFromMonth[];			//!< 開始月
	extern const _TCHAR kFromDay[];				//!< 開始日
	extern const _TCHAR kToMonth[];				//!< 終了月
	extern const _TCHAR kToDay[];				//!< 終了日
	extern const _TCHAR kFromHour[];			//!< 開始時
	extern const _TCHAR kFromMin[];				//!< 開始分
	extern const _TCHAR kToHour[];				//!< 終了時
	extern const _TCHAR kToMin[];				//!< 終了分
	extern const _TCHAR kDayOfWeek[];			//!< 曜日指定
}

	// 道路リンク
	namespace road_link
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(道路リンク)

		extern const _TCHAR kFromNodeId[];		//!< 始点ノードID
		extern const _TCHAR kToNodeId[];		//!< 終点ノードID
		extern const _TCHAR kRoadCls_C[];		//!< 道路種別
		extern const _TCHAR kNaviCls_C[];		//!< 経路種別
		extern const _TCHAR kLinkCls_C[];		//!< リンク種別
		extern const _TCHAR kWidth_C[];			//!< 幅員区分
		extern const _TCHAR kIsland_F[];		//!< 離島フラグ
		extern const _TCHAR kRoadNo[];			//!< 道路番号
		extern const _TCHAR kLaneCount[];		//!< レーン数
		extern const _TCHAR kNoPassage_C[];		//!< 通行禁止コード
		extern const _TCHAR kOneway_C[];		//!< 一方通行コード
		extern const _TCHAR kVclReg_RF[];		//!< 車種別規制関連フラグ
		extern const _TCHAR kLegalSpd_C[];		//!< 法定速度コード
	}
	// 道路ノード
	namespace road_node
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< 識別子(道路ノード)

		extern const _TCHAR kObjectId2[];		//!< OBJECTID2
		extern const _TCHAR kCountry_2C[];		//!< 国コード2
		extern const _TCHAR kNodeCls_C[];		//!< ノード種別
	}
	// リンク内属性
	namespace segment_attr
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::linkid;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク内属性)

		extern const _TCHAR kLinkAttr_C[];		//!< リンク内属性種別
	}
	// 通行禁止
	namespace nopassage_condition
	{
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< 識別子(通行禁止)
	}
	// 一方通行
	namespace oneway_condition
	{
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< 識別子(一方通行)

		extern const _TCHAR kLinkDir[];			//!< リンク方向
	}
	// 車種別規制
	namespace vehicle_reg
	{
		using namespace glshp::schema::base_table;
		using namespace glshp::schema::linkid;
		using namespace glshp::schema::reg_condition;

		extern const _TCHAR kSuffix[];			//!< 識別子(車種別規制)

		extern const _TCHAR kRegType_C[];		//!< 車種別規制状態
		extern const _TCHAR kHeight[];			//!< 高さ制限
		extern const _TCHAR kWidth[];			//!< 最大幅制限
		extern const _TCHAR kWeight[];			//!< 重量制限
		extern const _TCHAR kLength[];			//!< 長さ制限
		extern const _TCHAR kLoadage[];			//!< 積載重量制限
		extern const _TCHAR kDanger_F[];		//!< 危険貨物規制
	}
	// 車種別規制タイプ
	namespace vehicle_type
	{
		using namespace glshp::schema::base_table;

		extern const _TCHAR kSuffix[];			//!< 識別子(車種別規制タイプ)

		extern const _TCHAR kRegId[];			//!< 車種別規制情報のID
		extern const _TCHAR kExclude_F[];		//!< 除外条件フラグ
		extern const _TCHAR kRegName[];			//!< 規制条件名称
		extern const _TCHAR kPsgr_F[];			//!< 乗用車フラグ
		extern const _TCHAR kTaxi_F[];			//!< タクシーフラグ
		extern const _TCHAR kPbBus_F[];			//!< 公共バスフラグ
		extern const _TCHAR kPrBus_F[];			//!< プライベートバスフラグ
		extern const _TCHAR kTruck_F[];			//!< 輸送トラックフラグ
	}
	// リンク列規制
	namespace turnreg_inf
	{
		using namespace reg_condition;
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク列規制情報)
	}
	namespace turnreg_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク列)
	}
	// 方面案内
	namespace dirguide_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< 識別子(方面案内情報)

		extern const _TCHAR kGuideCls_C[];		//!< 案内種別
		extern const _TCHAR kName1[];			//!< 方面案内名称(第一言語)
		extern const _TCHAR kName2[];			//!< 方面案内名称(第二言語)
		extern const _TCHAR kNameSeq[];			//!< 名称シーケンス
		extern const _TCHAR kBoardPtn_C[];		//!< 看板色
	}
	namespace dirguide_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク列)
	}
	// レーン
	namespace lane_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< 識別子(レーン情報)

		extern const _TCHAR kLaneDir[];			//!< レーン方向
		extern const _TCHAR kAppLane[];			//!< 直近車線数
		extern const _TCHAR kRightChg[];		//!< 右車線変化量
		extern const _TCHAR kLeftChg[];			//!< 左車線変化量
		extern const _TCHAR kPassable[];		//!< 通行可・不可情報
	}
	namespace lane_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク列)
	}
	// 強制誘導
	namespace guide_inf
	{
		using namespace glshp::schema::inf_table;

		extern const _TCHAR kSuffix[];			//!< 識別子(強制誘導情報)

		extern const _TCHAR kGuideType_C[];		//!< 案内コード
	}
	namespace guide_lq
	{
		using namespace glshp::schema::link_queue;

		extern const _TCHAR kSuffix[];			//!< 識別子(リンク列)
	}
///// POI /////
	// POI
	namespace poi
	{
		using namespace glshp::schema::base_feature;
		using namespace glshp::schema::lang_type;

		extern const _TCHAR kSuffix[];			//!< 識別子(POI)

		extern const _TCHAR kCat_C[];			//!< カテゴリコード
		extern const _TCHAR kSubCat_C[];		//!< サブカテゴリコード
		extern const _TCHAR kChainMst_C[];		//!< チェーンコード
		extern const _TCHAR kAdmin_C[];			//!< 行政界コード
		extern const _TCHAR kHouseNum[];		//!< ハウスナンバー
		extern const _TCHAR kActAddr[];			//!< Actual Address
		extern const _TCHAR kTelNum[];			//!< 代表電話番号

		extern const _TCHAR kRName_Jpn[];		//!< 道路名称(日本語)
		extern const _TCHAR kRName_Eng[];		//!< 道路名称(英語)
		extern const _TCHAR kRName_Tha[];		//!< 道路名称(タイ語)
		extern const _TCHAR kRName_Ind[];		//!< 道路名称(インドネシア語)
		extern const _TCHAR kRName_May[];		//!< 道路名称(マレー語)
		extern const _TCHAR kRName_Vie[];		//!< 道路名称(ベトナム語)
	}
	// HNP
	namespace hnp
	{
		using namespace glshp::schema::base_feature;

		extern const _TCHAR kSuffix[];			//!< 識別子(POI)

		extern const _TCHAR kType_C[];			//!< ハウスナンバー種別コード
		extern const _TCHAR kNumber[];			//!< ハウスナンバー
		extern const _TCHAR kAdmin_C[];			//!< 行政界コード
		extern const _TCHAR kLinkID[];			//!< 道路リンクID
	}

}	// schema
}	// glshp
#endif	//_GLSHP_H_
