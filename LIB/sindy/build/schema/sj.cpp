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

#include "stdafx.h"
#include <sindy/schema/sj.h>

namespace sindy {

namespace schema {

namespace sj{

const _TCHAR kRooftopTableName[]     = _T("ROOFTOP");

const _TCHAR kWalkwayTableName[]     = _T("WALKWAY");

const _TCHAR kOtherLCTableName[]     = _T("OTHER_LC");

const _TCHAR kRailwayTableName[]     = _T("RAILWAY_GROUND");

const _TCHAR kRoadTableName[]        = _T("ROAD_POLYGON");

const _TCHAR kPredominantTableName[] = _T("PREDOMINANT_LC");

const _TCHAR kLCCLS[] = _T("LCCLS");
const _TCHAR kLUCLS[] = _T("LUCLS");
const _TCHAR kRoadF[] = _T("ROAD_F");


/// 地下鉄リンク
namespace subway_link {
	const _TCHAR kTableName[]     = _T("SUBWAY_LINK");     //!< テーブル名

	const _TCHAR kFromNodeID[]    = _T("FROM_NODE_ID");    //!< 始点ノードID
	const _TCHAR kToNodeID[]      = _T("TO_NODE_ID");      //!< 終点ノードID
	const _TCHAR kRailwayLineID[] = _T("RAILWAY_LINE_ID"); //!< 鉄道路線ID
}

/// 地下鉄ノード
namespace subway_node {
	const _TCHAR kTableName[]     = _T("SUBWAY_NODE");       //!< テーブル名

	const _TCHAR kNodeClassC[]    = _T("NODECLASS_C");       //!< ノード種別
	const _TCHAR kJoinLineInfo[]  = _T("JOIN_LINE_INFO_ID"); //!< 乗り入れ路線ID
	const _TCHAR kStationNumber[] = _T("STATION_NUMBER");    //!< 駅番号
}

/// 国立・国定公園ポリゴン
namespace nationalpark_site {
	const _TCHAR kTableName[] = _T("NATIONALPARK_SITE"); //!< テーブル名

	const _TCHAR kNparkID[]   = _T("NPARKID");           //!< 公園関連テーブルID
}

///国立・国定公園関連テーブル
namespace nationalpark_table {
	const _TCHAR kTableName[]  = _T("NATIONALPARK_TABLE"); //!< テーブル名

	const _TCHAR kParkTypeC[]  = _T("PARKTYPE_C");         //!< 公園種別コード
	const _TCHAR kParkNumber[] = _T("PARKNUMBER");         //!< 公園番号
	const _TCHAR kName[]       = _T("NAME");               //!< 名称
}

// 都市地図注記背景ポリゴン
namespace city_anno_site {
	const _TCHAR kTableName[]  = _T("CITY_ANNO_SITE");  //!< テーブル名

	const _TCHAR kCityAnnoID[] = _T("CITYANNO_ID"); //!< 都市注記ObjectID
	const _TCHAR kAnnoClassC[] = _T("ANNOCLASS_C"); //!< 都市注記種別コード
	const _TCHAR kCitySiteID[] = _T("CITYSITE_ID"); //!< 都市背景ObjectID
	const _TCHAR kBGClassC[]   = _T("BGCLASS_C");   //!< 都市背景種別コード
}

// 中縮注記背景紐付きテーブル
namespace rel_base_anno_site {

	const _TCHAR kTableName[]  = _T("REL_BASE_ANNO_SITE"); //!< テーブル名

	const _TCHAR kBaseAnnoID[] = _T("BASEANNO_ID");        //!< 中縮注記ID
	const _TCHAR kAnnoClassC[] = _T("ANNOCLASS_C");        //!< 中縮注記種別コード
	const _TCHAR kBaseSiteID[] = _T("BASESITE_ID");        //!< 中縮背景ObjectID
	const _TCHAR kBGClassC[]   = _T("BGCLASS_C");          //!< 中縮背景種別コード
}

// 建物ビル名称共通部分
namespace buildingname_common {
	const _TCHAR kName[] = _T("NAME");						//!< 名称
	const _TCHAR kSourceType[] = _T("SOURCETYPE_C");		//!< 出典コード
}

// 建物ビル名称共通部分（付加情報）
namespace buildingname_common_addinfo {
	const _TCHAR kNameSeparated[] = _T("NAME_SEPARATED");	//!< 名称（分割）
	const _TCHAR kYomiSeparated[] = _T("YOMI_SEPARATED");	//!< ヨミ（分割）
	const _TCHAR kBldgClass1[] = _T("BLDGCLASS1_C");		//!< 建物種別コード1
	const _TCHAR kBldgClass2[] = _T("BLDGCLASS2_C");		//!< 建物種別コード2
	const _TCHAR kBldgClass3[] = _T("BLDGCLASS3_C");		//!< 建物種別コード3
	const _TCHAR kFloors[] = _T("FLOORS");					//!< 階数
	const _TCHAR kSuitable[] = _T("SUITABLE_F");			//!< ビル名称妥当フラグ
	const _TCHAR kFixClass[] = _T("FIXCLASS_C");			//!< 種別確定コード
}

// 建物ビル名称素材ポイント
namespace buildingname_src_point {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_SRC_POINT"); //!< テーブル名

	const _TCHAR kOrgName1[] = _T("ORGNAME1");				//!< 加工前名称1
	const _TCHAR kOrgName2[] = _T("ORGNAME2");				//!< 加工前名称2
	const _TCHAR kKana[] = _T("KANA");						//!< カナ
	const _TCHAR kAddr[] = _T("ADDR");						//!< 住所
	const _TCHAR kOrgAddr[] = _T("ORGADDR");				//!< 成形前住所
	const _TCHAR kAddrCode[] = _T("ADDRCODE");				//!< 住所コード
	const _TCHAR kAddrLon[] = _T("ADDR_LON");				//!< 住所経度
	const _TCHAR kAddrLat[] = _T("ADDR_LAT");				//!< 住所緯度
	const _TCHAR kRepType[] = _T("REPTYPE_C");				//!< 地番代表点複合コード
	const _TCHAR kAddrLevel[] = _T("ADDRLEVEL_C");			//!< 住所レベルコード
	const _TCHAR kPinPoint[] = _T("PINPOINT_F");			//!< ピンポイントフラグ
	const _TCHAR kMultiSameBldg[] = _T("MULTISAMEBLDG_F");	//!< 同一家形内複数住所存在フラグ
	const _TCHAR kSameAddr[] = _T("SAMEADDR_F");			//!< 同一iPC住所複数存在フラグ
	const _TCHAR kSameAddrCount[] = _T("SAMEADDR_COUNT");	//!< 同一iPC住所数
	const _TCHAR kAddrVer[] = _T("ADDR_VER");				//!< 住所解析元XYDB名
	const _TCHAR kSourceName[] = _T("SOURCENAME");			//!< 出典元
	const _TCHAR kSourceDate[] = _T("SOURCEDATE");			//!< 出典資料入手時期
	const _TCHAR kSourceID[] = _T("SOURCE_ID");				//!< 出典資料ID
	const _TCHAR kBuilding1Div1[] = _T("BUILDING1_DIV1");	//!< 用途区分1
	const _TCHAR kBuilding1Div2[] = _T("BUILDING1_DIV2");	//!< 用途区分2
	const _TCHAR kBuilding2Div1[] = _T("BUILDING2_DIV1");	//!< 建物棟用途区分1
	const _TCHAR kBuilding2Div2[] = _T("BUILDING2_DIV2");	//!< 建物棟用途区分2
	const _TCHAR kHeight[] = _T("HEIGHT");					//!< 建物高さ
	const _TCHAR kFloors[] = _T("FLOORS");					//!< 地上階数
	const _TCHAR kBasementFloors[] = _T("BASEMENTFLOORS");	//!< 地下階数
	const _TCHAR kBuildingArea[] = _T("BUILDINGAREA");		//!< 建物面積
	const _TCHAR kAllArea[] = _T("ALLAREA");				//!< 述べ面積
	const _TCHAR kSrcStartDate[] = _T("SRC_STARTDATE");		//!< 素材仕様開始年月日
	const _TCHAR kSrcUpdateDate[] = _T("SRC_UPDATEDATE");	//!< 素材更新年月日
	const _TCHAR kAnyPlaceID[] = _T("ANYPLACE_ID");			//!< AnyPlaceID
	const _TCHAR kUpdateDate[] = _T("UPDATEDATE");			//!< 更新年月日
	const _TCHAR kAdoption[] = _T("ADOPTION_C");			//!< 採用コード
}

// 建物ビル名称素材付加情報
namespace buildingname_src_addinfo {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_SRC_ADDINFO");		//!< テーブル名

	const _TCHAR kSourceName[] = _T("SOURCENAME");			//!< 出典元
	const _TCHAR kSourceID[] = _T("SOURCE_ID");				//!< 出典資料ID
}

// 建物ビル名称ポイント
namespace buildingname_point {
	const _TCHAR kTableName[] = _T("BUILDINGNAME_POINT");	//!< テーブル名

	const _TCHAR kBuildingOID[] = _T("BUILDING_OID");		//!< 建物ポリゴンOID
	const _TCHAR kSrcOID[] = _T("SRC_OID");					//!< 参照素材OID
	const _TCHAR kSurveyDate[] = _T("SURVEYDATE");			//!< 調査日
	const _TCHAR kAnyPlaceID[] = _T("ANYPLACE_ID");			//!< AnyPlaceID
	const _TCHAR kPriority[] = _T("PRIORITY_C");			//!< 信頼コード
	const _TCHAR kMatchPer[] = _T("MATCH_PER");				//!< マッチング率
	const _TCHAR kContentsCode[] = _T("CONTENTS_CODE");		//!< 注記POIコンテンツコード
	const _TCHAR kContentsSeq[] = _T("CONTENTS_SEQ");		//!< 注記POIコンテンツ内シーケンス
}

// 建物ビル名称紐付きテーブル
namespace rel_buildingname {
	const _TCHAR kTableName[] = _T("REL_BUILDINGNAME");			//!< テーブル名
	const _TCHAR kBuildingNameOID[] = _T("BUILDINGNAME_OID");	//!< 建物ビル名称OID
	const _TCHAR kSrcOID[] = _T("SRC_OID");						//!< 参照素材OID
}

namespace railway_link {
	const _TCHAR kTableName[]     = _T("RAILWAY_LINK");			//!< テーブル名

	const _TCHAR kFromNodeOID[]   = _T("FROM_NODE_ID");			//!< 始点ノードのOID
	const _TCHAR kToNodeOID[]     = _T("TO_NODE_ID");			//!< 終点ノードのOID
	const _TCHAR kRailwayClass_C[] = _T("RAILWAYCLASS_C");		//!< 鉄道種別
	const _TCHAR kDispScale_C[]    = _T("DISPSCALE_C");			//!< スケール表示種別
	const _TCHAR kUnderGround_F[] = _T("UNDERGROUND_F");		//!< 地下フラグ
}

namespace rel_railway_link {
	const _TCHAR kTableName[]      = _T("REL_RAILWAY_LINK");	//!< テーブル名

	const _TCHAR kRwLinkID[]       = _T("RW_LINK_ID");			//!< 鉄道リンクのOID
	const _TCHAR kRailwayLineID[]  = _T("RAILWAY_LINE_ID");		//!< 鉄道路線のOID
}

namespace railway_node {
	const _TCHAR kTableName[]      = _T("RAILWAY_NODE");		//!< テーブル名

	const _TCHAR kNodeClass_C[]	   = _T("NODECLASS_C");			//!< ノード種別
}

namespace rel_railway_node {
	const _TCHAR kTableName[]      = _T("REL_RAILWAY_NODE");	//!< テーブル名

	const _TCHAR kRwNodeID[]       = _T("RW_NODE_ID");			//!< 鉄道ノードのOID
	const _TCHAR kJoinLineInfoID[] = _T("JOIN_LINE_INFO_ID");	//!< 乗り入れ路線のOID
	const _TCHAR kStationNo[]      = _T("STATION_NUMBER");		//!< 駅番号
}

namespace railway_color {
	const _TCHAR kTableName[]      = _T("RAILWAY_COLOR");		//!< テーブル名

	const _TCHAR kRailwayLineID[]  = _T("RAILWAY_LINE_ID");		//!< 鉄道路線のOID
	const _TCHAR kColorName[]      = _T("COLOR_NAME");			//!< 色名称
	const _TCHAR kColorNameEng[]   = _T("COLOR_NAME_ENG");		//!< 色名称(英語)
	const _TCHAR kColorCode[]      = _T("COLOR_CODE");			//!< カラーコード
}

namespace station_detail {
	const _TCHAR kTableName[]      = _T("STATION_DETAIL");		//!< テーブル名

	const _TCHAR kPolygonType_C[]  = _T("POLYGONTYPE_C");		//!< ポリゴン種別
}

namespace midzoom_platform {
	const _TCHAR kTableName[]      = _T("MIDZOOM_PLATFORM");    //!< テーブル名
}

namespace highzoom_station {
	const _TCHAR kTableName[]      = _T("HIGHZOOM_STATION");    //!< テーブル名
}

namespace midzoom_station {
	const _TCHAR kTableName[]      = _T("MIDZOOM_STATION");     //!< テーブル名
}

namespace lowzoom_platform {
	const _TCHAR kTableName[]      = _T("LOWZOOM_PLATFORM");    //!< テーブル名
}

namespace rel_station_site {
	const _TCHAR kTableName[]      = _T("REL_STATION_SITE");    //!< テーブル名

	const _TCHAR kGroupID[]        = _T("GROUP_ID");		    //!< 駅ID
	const _TCHAR kLayerNo_C[]      = _T("LAYERNO_C");		    //!< レイヤー番号
	const _TCHAR kLayerOID[]       = _T("LAYER_OID");		    //!< レイヤー内OID
}

namespace groupid_master {
	const _TCHAR kTableName[]      = _T("GROUPID_MASTER");    //!< テーブル名

	const _TCHAR kStationName[]    = _T("STATION_NAME");       //!< 駅名
}

/// 道路名称テーブル
namespace road_name_list {
	const _TCHAR kTableName[] = _T("ROAD_NAME_LIST");

	const _TCHAR kNameClass[] = _T("NAMECLASS_C");		//!< 道路名称種別
	const _TCHAR kNameKanji[] = _T("NAME_KANJI");		//!< 名称
	const _TCHAR kNameYomi[] = _T("NAME_YOMI");			//!< 読み
}

} // sj

} // schema

} // sindy
