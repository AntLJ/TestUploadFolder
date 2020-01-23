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

#ifndef SINDY_SCHEMA_POI_H_
#define SINDY_SCHEMA_POI_H_

#include <sindy/schema/common.h>

namespace sindy {

namespace schema {

// POI鉄道会社テーブル
namespace railway_corp {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kCorpCode[];		//!< 会社コード
	extern const _TCHAR kName[];			//!< 会社名
	extern const _TCHAR kYomi[];			//!< 会社名読み
	extern const _TCHAR kExName[];			//!< 会社略称
	extern const _TCHAR kExYomi[];			//!< 会社略称読み

} // railway_corp

// POI鉄道路線テーブル
namespace railway_line {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kCorpID[];			//!< 鉄道会社ＩＤ
	extern const _TCHAR kLineCode[];		//!< 路線コード
	extern const _TCHAR kName[];			//!< 路線名
	extern const _TCHAR kYomi[];			//!< 路線名読み
	extern const _TCHAR kExName[];			//!< 路線略称
	extern const _TCHAR kExYomi[];			//!< 路線略称読み
	extern const _TCHAR kTypeID[];			//!< 種別ＩＤ

	namespace type_code {
		enum ECode {
			kJR				= 1,		//!< ＪＲ
			kPrivate		= 2,		//!< 私鉄
			kThirdSector	= 3,		//!< 第３セクター
			kSubway			= 4,		//!< 地下鉄
			kNewrail		= 5,		//!< 新交通
			kMonorail		= 6,		//!< モノレール
			kStreetrail		= 7,		//!< 路面鉄道
			kRopeway		= 8,		//!< ロープウェイ
			kCableCar		= 9			//!< ケーブルカー
		};
	} // typecode

} // railway_line

// 乗り入れ路線テーブル
namespace join_line_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kLineID[];		//!< 路線ID
	extern const _TCHAR kLineSeq[];		//!< 路線内シーケンス
	extern const _TCHAR kInfoID[];		//!< 駅情報ID
	extern const _TCHAR kSubName[];		//!< 補足名称
	extern const _TCHAR kSubYomi[];		//!< 補足名称読み
	extern const _TCHAR kStationID[];	//!< 駅ポイントID
	extern const _TCHAR kContentsSeq[];	//!< コンテンツ内シーケンス

} // join_line_info

// 駅情報マスタテーブル
namespace station_info {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kCorpID[];		//!< 鉄道会社ID
	extern const _TCHAR kName[];		//!< 駅名
	extern const _TCHAR kYomi[];		//!< 駅名読み
	extern const _TCHAR kAddrCode[];	//!< 住所コード
	extern const _TCHAR kTel[];			//!< 電話番号
	extern const _TCHAR kAnAreaCode[];	//!< 市外局番

} // station_info

// 駅(乗り入れ路線)と出入口の紐付けテーブル
namespace join_station_gate {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kJLineID[];		//!< 乗り入れ路線ID
	extern const _TCHAR kGateID[];		//!< 駅出入口ID
	extern const _TCHAR kContentsSeq[];	//!< コンテンツ内シーケンス

} // join_station_gate

// POI鉄道駅ポイント
namespace station_point {	
	extern const _TCHAR kTableName[];	//!< テーブル名

	using namespace ::sindy::schema::ipc_table;
	extern const _TCHAR kGuideID[];		//!< 誘導ポイントID

} // station_point

// POI鉄道出入口ポイント
namespace station_gate_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kName[];			//!< 出入口名称
	extern const _TCHAR kYomi[];			//!< 出入口名称読み
	
	extern const _TCHAR kGuideID[];		//!< 誘導ポイントID

} // station_gate_point

// 誘導ポイント
namespace station_guide_point {	
	using namespace ::sindy::schema::ipc_table;
	extern const _TCHAR kTableName[];	//!< テーブル名
	extern const _TCHAR kTollRoad[];	//!< 有料道路フラグ

} // station_guide_point

// 駅(乗り入れ路線)と歩行者ＰＯＩの紐付けテーブル
namespace join_station_walk {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];	//!< テーブル名

	extern const _TCHAR kJLineID[];		//!< 乗り入れ路線ID
	extern const _TCHAR kWalkPoiID[];	//!< POIポイントID

} // join_station_gate

// 歩行者ＰＯＩポイント
namespace walk_poi_point {
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kPOIClass[];		//!< 歩行者POI種別
	extern const _TCHAR kName[];			//!< 名称
	extern const _TCHAR kYomi[];			//!< 名称読み
	extern const _TCHAR kFloorLevel[];		//!< 階層値
	extern const _TCHAR kIndoor[];			//!< 施設内フラグ
	extern const _TCHAR kPicName[];			//!< 画像ファイル名

	namespace poi_class {
		enum ECode {
			kNone			=		 0,	//!< 未調査・作業中
			kTaxi			=		 1, //!< タクシー乗り場
			kMeetingSpot	=		 2, //!< 待ち合わせ場所
		};
	} // poi_class

} // walk_poi_point

// チェーン店マスターコードテーブル
namespace poi_mstchn
{
	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[];		//!< テーブル名

	extern const _TCHAR kChainCode[];		//!< チェーン店マスターコード（8桁）
	extern const _TCHAR kChainCodeOld[];	//!< チェーン店マスターコード（6桁）
	extern const _TCHAR kName[];			//!< 店舗名称
	extern const _TCHAR kYomi[];			//!< 店舗名称読み
} // poi_mstchn

// 鉄道分類コード
namespace traintype {

	using namespace ::sindy::schema::ipc_table;

	extern const _TCHAR kTableName[]; //!< テーブル名

	extern const _TCHAR kTypeCode[]; //!< 鉄道分類コード
	extern const _TCHAR kName[]; //!< 名称
	extern const _TCHAR kYomi[]; //!< 読み
} // traintype

} // schema

} // sindy

#endif // ifndef SINDY_SCHEMA_ANNOTATION_H_
