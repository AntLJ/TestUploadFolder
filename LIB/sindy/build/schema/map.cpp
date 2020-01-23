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
#include <sindy/schema/map.h>

namespace sindy {

namespace schema {

namespace scaledisp_flag {
	const TCHAR kSC1Disp[]				= _T("DISPSCALE1_F");			//!< スケール1表示フラグ
	const TCHAR kSC2Disp[]				= _T("DISPSCALE2_F");			//!< スケール2表示フラグ
	const TCHAR kSC3Disp[]				= _T("DISPSCALE3_F");			//!< スケール3表示フラグ
	const TCHAR kSC4Disp[]				= _T("DISPSCALE4_F");			//!< スケール4表示フラグ
} // scaledisp_flag

namespace scale_class {
	const TCHAR kSC1BgClass[]			= _T("SC1BGCLASS_C");			//!< スケール1背景種別コード
	const TCHAR kSC2BgClass[]			= _T("SC2BGCLASS_C");			//!< スケール2背景種別コード
	const TCHAR kSC3BgClass[]			= _T("SC3BGCLASS_C");			//!< スケール3背景種別コード
	const TCHAR kSC4BgClass[]			= _T("SC4BGCLASS_C");			//!< スケール4背景種別コード
} // scale_class

namespace claimpoly {
	const TCHAR kClaimType[]			= _T("CLAIMTYPE_C");			//!< クレームタイプコード
	const TCHAR kClaimNo[]				= _T("CLAIM_NO");				//!< クレーム番号
	const TCHAR kComFirm[]				= _T("COMFIRM_F");				//!< 常時確認フラグ
	const TCHAR kRemarks[]				= _T("USELESS_F");				//!< コメント、備考
	const TCHAR kUseLess[]				= _T("REMARKS");				//!< 不要フラグ
} // claimpoly

namespace building {
	const TCHAR kTableName[]			= _T("BUILDING");				//!< テーブル名
	const TCHAR kBldClass[]				= _T("BLDCLASS_C");				//!< 建物種別コード
	const TCHAR kCorridor[]				= _T("CORRIDOR_F");				//!< 渡り廊下フラグ
	const TCHAR kNoWall[]				= _T("NOWALL_F");				//!< 無壁舎フラグ
	const TCHAR kGeospaceID[]			= _T("GEOSPACE_ID");			//!< GEOSPACE_ID
}

namespace building_line {
	const TCHAR kTableName[]			= _T("BUILDING_LINE");			//!< テーブル名
	const TCHAR kBldClass[]				= _T("BLDCLASS_C");				//!< 建物ライン種別コード
}

namespace building_step {
	const TCHAR kTableName[]			= _T("BUILDING_STEP");			//!< テーブル名
	const TCHAR kFloors[]				= _T("FLOORS");					//!< 階数
	const TCHAR kCheck[]				= _T("CHECK_F");				//!< 階数情報確認フラグ
	const TCHAR kBuildingID[]           = _T("BUILDING_ID");            //!< 建物ID
}

namespace city_site {
	const TCHAR kTableName[]			= _T("CITY_SITE");				//!< テーブル名
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景種別コード
}

namespace city_line {
	const TCHAR kTableName[]			= _T("CITY_LINE");				//!< テーブル名
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ライン種別コード
}

namespace city_station {
	const TCHAR kTableName[]			= _T("CITY_STATION");			//!< テーブル名
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< 鉄道駅種別コード
}

namespace city_railway {
	const TCHAR kTableName[]			= _T("CITY_RAILWAY");			//!< テーブル名
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kRailwayClass[]			= _T("RAILWAYCLASS_C");			//!< 鉄道種別コード
}

namespace underground_site {
	const TCHAR kTableName[]			= _T("UNDERGROUND_SITE");		//!< テーブル名
	const TCHAR kUgClass[]				= _T("UGCLASS_C");				//!< 地下背景種別
}

namespace underground_line {
	const TCHAR kTableName[]			= _T("UNDERGROUND_LINE");		//!< テーブル名
	const TCHAR kUgClass[]				= _T("UGCLASS_C");				//!< 地下背景ライン種別
}

namespace city_admin {
	const TCHAR kTableName[]			= _T("CITY_ADMIN");				//!< テーブル名
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< 市区町村コード
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 住所コード
	const TCHAR kAddrCode2[]			= _T("ADDRCODE2");				//!< 住所コード2
	const TCHAR kTmpCode[]				= _T("TMPCODE_F");				//!< 仮住所コードフラグ
	const TCHAR kGaikuFugo[]			= _T("GAIKUFUGO");				//!< 街区符号
	const TCHAR kExtGaikuFugo[]			= _T("EXTGAIKUFUGO");			//!< 拡張街区符号
	const TCHAR kAddrClass[]			= _T("ADDRCLASS_C");			//!< 住所整備種別
	const TCHAR kAreaClass[]			= _T("AREACLASS_C");			//!< エリア判別種別
	const TCHAR kManage[]				= _T("MANAGE_F");				//!< 管理フラグ
	const TCHAR kDetach[]				= _T("DETACH_F");				//!< 飛地フラグ
	const TCHAR kColorCode[]			= _T("COLORCODE");				//!< 街区色付与用色属性
	const TCHAR kWorkColor[]			= _T("WORKCOLOR");				//!< 街区色付与作業用
	const TCHAR kConvType[]				= _T("CONVTYPE_C");				//!< 住所変換モード
}

namespace city_rep_point {
	const TCHAR kTableName[]			= _T("CITY_REP_POINT");			//!< テーブル名
	const TCHAR kChibanNo[]				= _T("CHIBAN_NO");				//!< 地番
	const TCHAR kAnno[]					= _T("ANNO_F");					//!< 地番注記フラグ
	const TCHAR kPointClass[]			= _T("POINTCLASS_C");			//!< ポイント種別
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 住所１１桁コード
	const TCHAR kRepType[]				= _T("REPTYPE_C");				//!< 
}

namespace cityplanmap {
	const TCHAR kTableName[]			= _T("CITYPLANMAP");			//!< テーブル名
	const TCHAR kMapID[]				= _T("MAPID");					//!< 図葉ID
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< 行政区域コード
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< 行政名
	const TCHAR kMapName[]				= _T("MAPNAME");				//!< 図葉名
	const TCHAR kMapNo[]				= _T("MAPNO");					//!< 図葉NO
	const TCHAR kPubDate[]				= _T("PUB_DATE");				//!< 作成年度
}

namespace base_site {
	const TCHAR kTableName[]			= _T("BASE_SITE");				//!< テーブル名
}

namespace base_line {
	const TCHAR kTableName[]			= _T("BASE_LINE");				//!< テーブル名

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ライン種別コード
}

namespace base_station {
	const TCHAR kTableName[]			= _T("BASE_STATION");			//!< テーブル名

	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kStationClass[]			= _T("STATIONCLASS_C");			//!< 鉄道駅種別コード
}

namespace base_railway {
	const TCHAR kTableName[]			= _T("BASE_RAILWAY");			//!< テーブル名
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kRailwayClass[]			= _T("RAILWAYCLASS_C");			//!< 鉄道種別コード
}

namespace base_land {
	const TCHAR kTableName[]			= _T("BASE_LAND");				//!< テーブル名
}

namespace base_admin {
	const TCHAR kTableName[]			= _T("BASE_ADMIN");				//!< テーブル名
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< 市区町村コード
}

namespace base_rep_point {
	const TCHAR kTableName[]			= _T("BASE_REP_POINT");			//!< テーブル名
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 住所１１桁コード
	const TCHAR kAdminName[]			= _T("ADMINNAME");				//!< 代表点名称
	const TCHAR kZipCode[]				= _T("ZIPCODE");				//!< 郵便番号
}

namespace base_contour {
	const TCHAR kTableName[]			= _T("BASE_CONTOUR");			//!< テーブル名
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< 段彩種別コード
}

namespace middle_site {
	const TCHAR kTableName[]			= _T("MIDDLE_SITE");			//!< テーブル名
}

namespace middle_line {
	const TCHAR kTableName[]			= _T("MIDDLE_LINE");			//!< テーブル名
	const TCHAR kUnderGround[]			= _T("UNDERGROUND_F");			//!< 地下フラグ
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ライン種別コード
}

namespace middle_contour {
	const TCHAR kTableName[]			= _T("MIDDLE_CONTOUR");			//!< テーブル名
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< 段彩種別コード
}

namespace top_site {
	const TCHAR kTableName[]			= _T("TOP_SITE");				//!< テーブル名
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< トップ背景種別コード
}

namespace top_line {
	const TCHAR kTableName[]			= _T("TOP_LINE");				//!< テーブル名
	const TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ライン種別コード
}

namespace top_contour {
	const TCHAR kTableName[]			= _T("TOP_CONTOUR");			//!< テーブル名
	const TCHAR kCntClass[]				= _T("CNTCLASS_C");				//!< 段彩種別コード
}

namespace ortho_area {
	const TCHAR kTableName[]			= _T("ORTHO_AREA");				//!< テーブル名
	const TCHAR kTileName[]				= _T("TILE_NAME");				//!< 
}

namespace adminarea {
	const TCHAR kTableName[]			= _T("ADMINAREA");				//!< テーブル名
	const TCHAR kCityCode[]				= _T("CITYCODE");				//!< 市区町村コード
	const TCHAR kCityName[]				= _T("CITYNAME");				//!< 市区町村名
}

namespace aerial_photo {
	const TCHAR kTableName[]			= _T("AERIAL_PHOTO");			//!< テーブル名
	const TCHAR kMeshCode[]				= _T("MESHCODE");				//!< 都市地図メッシュコード
	const TCHAR kBinderName[]			= _T("BINDERNAME");				//!< バインダー名
	const TCHAR kCourse[]				= _T("COURSE");					//!< 撮影コース
	const TCHAR kPhotoNo[]				= _T("PHOTONO");				//!< 写真番号
	const TCHAR kShootYear[]			= _T("SHOOTYEAR");				//!< 写真撮影年
	const TCHAR kShootDate[]			= _T("SHOOTDATE");				//!< 写真撮影年月日
	const TCHAR kOwnerCode[]			= _T("OWNERCODE");				//!< 所在地
}

namespace daikei_point {
	const TCHAR kTableName[]			= _T("DAIKEI_POINT");			//!< テーブル名
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 住所コード
	const TCHAR kAddrStr[]				= _T("ADDRSTR");				//!< 住所文字列
	const TCHAR kSrcType[]				= _T("SRCTYPE");				//!< データソースタイプ
	const TCHAR kDupType[]				= _T("DUPTYPE");				//!< 重複タイプ
}

namespace parea {
	const TCHAR kTableName[]			= _T("PAREA");					//!< テーブル名
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< PAREA住所コード
	const TCHAR kRepPointCode[]			= _T("REPPOINTCODE");			//!< 代表点コード
	const TCHAR kUpdateCode[]			= _T("UPDATECODE");				//!< 更新コード
	const TCHAR kTmpCode[]				= _T("TMPCODE_F");				//!< 仮コードフラグ
}

namespace gou_point {
	const TCHAR kTableName[]			= _T("GOU_POINT");				//!< テーブル名
	const TCHAR kGouNo[]				= _T("GOU_NO");					//!< 号番号
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 20桁住所コード
	const TCHAR kBuildRF[]				= _T("BUILD_RF");				//!< 建物関連フラグ
	const TCHAR kBuildID[]				= _T("BUILD_ID");				//!< 対応建物ID
	const TCHAR kExpGouNo[]				= _T("EXPGOUNO_F");				//!< 拡張番号フラグ
	const TCHAR kTmpCheck[]				= _T("TMP_CHECK_F");			//!< 確認フラグ
	const TCHAR kTmpFloors[]			= _T("TMP_FLOORS");				//!< 階数情報
	const TCHAR kGouType[]				= _T("GOUTYPE");				//!< 号タイプ
	const TCHAR kPriority[]				= _T("PRIORITY_F");				//!< 優先フラグ
	const TCHAR kPrecisionCode[]		= _T("PRECISION_CODE");			//!< 住所一致桁数
	const TCHAR kConfirm[]				= _T("CONFIRM_C");				//!< 住所確認ステータスコード
	const TCHAR kHimawari[]				= _T("HIMAWARI_F");				//!< ひまわりフラグ
}

namespace klink_point {
	const TCHAR kTableName[]			= _T("KLINK_POINT");			//!< テーブル名
	const TCHAR kAddrCodeOrg[]			= _T("ADDRCODE_ORG");			//!< 初期22桁コード
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< 最新22桁コード
	const TCHAR kKatagakiName[]			= _T("KATAGAKINAME");			//!< タウンページ方書名称
	const TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< タウンページ掲載名称
	const TCHAR kInfoSrc[]				= _T("INFOSRC_C");				//!< 情報提供元コード
	const TCHAR kInputSeido[]			= _T("INPUTSEIDO_C");			//!< 座標精度種別コード
	const TCHAR kReleaseSeido[]			= _T("RELEASESEIDO_C");			//!< 座標精度種別コード
	const TCHAR kBuildName[]			= _T("BUILDNAME");				//!< ビル名称
	const TCHAR kBuildInfoSrc[]			= _T("BUILDINFOSRC_C");			//!< ビル情報提供元コード
	const TCHAR kMaxFloors[]			= _T("MAX_FLOORS");				//!< 最上階数
	const TCHAR kFloor[]				= _T("FLOOR");					//!< 階数
	const TCHAR kNequal[]				= _T("NEQUAL_F");				//!< 11桁不一致フラグ
}

namespace claimpoly_addr {
	const TCHAR kTableName[]			= _T("CLAIMPOLY_ADDR");			//!< テーブル名
}

namespace claimpoly_map {
	const TCHAR kTableName[]			= _T("CLAIMPOLY_MAP");			//!< テーブル名
}

namespace cs_addr_point {
	const TCHAR kTableName[]			= _T("CS_ADDR_POINT");			//!< テーブル名
	const TCHAR kAddrCode[]				= _T("ADDRCODE");				//!< ２０桁住所コード
}

namespace city_admin_c {
	const TCHAR kTableName[]			= _T("CITY_ADMIN_C");			//!< テーブル名
	const TCHAR kCityCode[]				= _T("ADDRCODE");				//!< 市区町村コード
}

namespace city_admin_pref {
	const TCHAR kTableName[]			= _T("CITY_ADMIN_PREF");		//!< テーブル名
	const TCHAR kCityCode[]				= _T("ADDRCODE");				//!< 市区町村コード
}

namespace extp_point {
	const _TCHAR kTableName[]			= _T("EXTP_POINT");				//!< テーブル名

	const _TCHAR kAddrCode[]			= _T("ADDRCODE");				//!< 20桁住所コード
	const _TCHAR kAddrName[]			= _T("ADDRNAME");				//!< 住所名
	const _TCHAR kKatagakiName[]		= _T("KATAGAKINAME");			//!< タウンページ方書名称
	const _TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< タウンページ掲載名称
	const _TCHAR kAddPoint[]			= _T("ADDPOINT_F");				//!< 住所検索ポイント化フラグ
	const _TCHAR kInfoSrc[]				= _T("INFOSRC_C");				//!< 初期座標提供元コード
	const _TCHAR kPointStat[]			= _T("POINTSTAT_C");			//!< 調査結果コード
}

namespace teltp_point {
	const _TCHAR kTableName[]			= _T("TELTP_POINT");			//!< テーブル名

	const _TCHAR kTelNum[]				= _T("TEL_NUM");				//!< 電話番号
	const _TCHAR kKatagakiName[]		= _T("KATAGAKINAME");			//!< タウンページ方書名称
	const _TCHAR kKeisaiName[]			= _T("KEISAINAME");				//!< タウンページ掲載名称
	const _TCHAR kAddrCode[]			= _T("ADDRCODE");				//!< 11桁住所コード
	const _TCHAR kAddrName[]			= _T("ADDRNAME");				//!< 住所名
	const _TCHAR kPointStat[]			= _T("POINTSTAT_C");			//!< 調査結果コード
	const _TCHAR kSequence[]			= _T("SEQUENCE");				//!< シーケンス番号
	const _TCHAR kAddXY[]				= _T("ADDXY_C");				//!< 座標付与種別
}

// 都市建物屋上施設
namespace building_roof {
	const _TCHAR kTableName[]			= _T("BUILDING_ROOF");            //!< テーブル名

	const _TCHAR kRfClass[]				= _T("RFCLASS_C");				 //!< 屋上施設種別
}
// 都市エリア
namespace city_area {
	const _TCHAR kTableName[]			= _T("CITY_AREA");				//!< テーブル名

	const _TCHAR kAreaClass[]			= _T("AREACLASS_C");			//!< エリア種別
}

// 都市ポリゴン分割ライン
namespace city_sprit_line {
	const _TCHAR kTableName[]			= _T("CITY_SPLIT_LINE");		//!< テーブル名

	const _TCHAR kSpritClass[]			= _T("SPLITCLASS_C");			//!< 分割ライン種別
}

// 都市トンネル坑口
namespace city_tunnel {
	const _TCHAR kTableName[]			= _T("CITY_TUNNEL");			//!< テーブル名

	const _TCHAR kTunnelClass[]			= _T("TUNNELCLASS_C");			//!< トンネル種別
}

// 都市道路ポリゴン
namespace city_road {
	const _TCHAR kTableName[]			= _T("CITY_ROAD");				//!< テーブル名

	const _TCHAR kRoadClass[]			= _T("ROADCLASS_C");			//!< 道路ポリゴン種別
}

// 都市料金所分離帯ポリゴン
namespace city_divider {
	const _TCHAR kTableName[]           = _T("CITY_DIVIDER");           //!< テーブル名

	const _TCHAR kDividerClass[]        = _T("DIVIDERCLASS_C");         //!< 料金所分離帯種別
}

// 高架下背景ライン
namespace city_line_under {
	const _TCHAR kTableName[]			= _T("CITY_LINE_UNDER");		//!< テーブル名

	const _TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ライン種別
}

// 高架下背景ポリゴン
namespace city_site_under {
	const _TCHAR kTableName[]			= _T("CITY_SITE_UNDER");		//!< テーブル名

	const _TCHAR kBgClass[]				= _T("BGCLASS_C");				//!< 背景ポリゴン種別
}

// 地下通路・地下街ポリゴン
namespace underground_area
{
	const _TCHAR kTableName[] = _T("UNDERGROUND_AREA"); //!< テーブル名

} // underground_area

// 郵便番号ポリゴン
namespace zipcode_area
{
	const _TCHAR kTableName[] = _T("ZIPCODE_AREA"); //!< テーブル名
	const _TCHAR kZipCode1[] = _T("ZIPCODE1"); //!< テーブル名
	const _TCHAR kZipCode2[] = _T("ZIPCODE2"); //!< テーブル名
}

// 郵便番号ポリゴン住所紐付けテーブル
namespace rel_addr
{
	const _TCHAR kTableName20[] = _T("REL_ADDR20");
	const _TCHAR kTableName23[] = _T("REL_ADDR23");
	const _TCHAR kZipCode[] = _T("ZIPCODE");
	const _TCHAR kAddrCode[] = _T("ADDRCODE");
	const _TCHAR kModifyStatus[] = _T("MODIFYSTATUS_C");
}


} // schema

} // sindy
