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
* @file define.h
*
* @brief SiNDY関連の固定値定義ファイル
*/
#ifndef __DEFINE_MXCOMMANDS
#define __DEFINE_MXCOMMANDS 1	//!< 重複インクルード防止

#include "define_enum.h" // enum 定義

//#define TOPOLOGY_EDIT 15	// 共有編集の最大値（オブジェクト数） CFeedbackFeatureSelection、CFeatureSelectionを作成したのでいらない
//#define MAX_LAYER     20    // 読み込むレイヤ数の最大値（SnapEnvObj.h参照） CSnapCollectionを作成したのでいらない
/// レイヤ名の最大長（SearchToolDlg.h  QueryBy* 参照）
//#define MAX_LAYERNAME_LEN 256	
/// 検索結果のリスト表示最大数（SearchDlg.[h,cpp] 参照）
//#define MAX_SEARCHED_ITEM 100	
// オブジェクトのフィールド値は 4KB に制限（AttrDlg.cpp参照）
#define MAX_FIELD_VALUE   4096	
//#define MAX_FIELD_NAME    256	// オブジェクトのフィールド名最大値（AttrGridCtrlObj.cpp参照）
//#define MAX_SUBDOMAIN_LEN 8192	// 一つのカラムに対するサブドメインの総合長（AttrGridCtrlObj.cpp参照）

const static INT MAX_WORKPURPOSE_LENGTH = 512;		//!< 「作業目的」の一つ当たりの最大バイト数
const static INT MAX_WORKPURPOSEKEYS_KENGTH = 4092;	//!< 「作業目的」ファイルのキーの総合バイト数
/// カスタムアプリケーションで使用する場合にはコメントを外してください
//#ifndef USE_MAPCONTROL
//#define USE_MAPCONTROL 1
//#endif // ifndef USE_MAPCONTROL
/// カスタムアプリケーションで ISnapEnvObj を使用する場合はコメントを外してください
//#ifndef USE_SNAPENV
//#define USE_SNAPENV 1
//#endif // ifndef USE_SNAPENV

/// レイヤのエイリアス名の定義
//#define ALIAS_NODE_NAME   (_T("node"))
//#define ALIAS_ROAD_NAME   (_T("road"))

// レイヤのテーブル名の定義
const static LPCTSTR NODE_TABLE_NAME			= (_T("ROAD_NODE"));		//!< ノードテーブル名称
const static LPCTSTR ROAD_TABLE_NAME			= (_T("ROAD_LINK"));		//!< 道路リンクテーブル名称
const static LPCTSTR WNODE_TABLE_NAME			= (_T("WALK_NODE"));		//!< 歩行者用ノードテーブル名称
const static LPCTSTR WROAD_TABLE_NAME			= (_T("WALK_LINK"));		//!< 歩行者用道路リンクテーブル名称
const static LPCTSTR PLINK_TABLE_NAME			= (_T("PEC_LINK"));			//!< 駐車場リンクテーブル名称
const static LPCTSTR INTERSECTION_TABLE_NAME	= (_T("INTERSECTION"));		//!< 統合交差点テーブル名称
const static LPCTSTR BASE_LAND_TABLE_NAME		= (_T("BASE_LAND"));		//!< 中縮陸地テーブル名称
const static LPCTSTR BASE_CONTOUR_TABLE_NAME	= (_T("BASE_CONTOUR"));		//!< BASE_CONTOURテーブル名称
const static LPCTSTR MIDDLE_CONTOUR_TABLE_NAME	= (_T("MIDDLE_CONTOUR"));	//!< MIDDLE_CONTOURテーブル名称
const static LPCTSTR MIDDLE_LINE_TABLE_NAME		= (_T("MIDDLE_LINE"));		//!< MIDDLE_LINEテーブル名称
const static LPCTSTR MIDDLE_SITE_TABLE_NAME		= (_T("MIDDLE_SITE"));		//!< MIDDLE_SITEテーブル名称
const static LPCTSTR BASESITE_TABLE_NAME		= (_T("BASE_SITE"));		//!< 中縮背景テーブル名称
const static LPCTSTR BASELINE_TABLE_NAME		= (_T("BASE_LINE"));		//!< 中縮背景ラインテーブル名称
const static LPCTSTR BASERAILWAY_TABLE_NAME		= (_T("BASE_RAILWAY"));		//!< 中縮鉄道テーブル名称
const static LPCTSTR BASESTATION_TABLE_NAME		= (_T("BASE_STATION"));		//!< 中縮駅舎ポリゴンテーブル名称
const static LPCTSTR GRAD_TABLE_NAME			= ( _T("GRAD"));			//!< 傾斜テーブル名称
const static LPCTSTR SEGMENTATTR_TABLE_NAME		= ( _T("SEGMENT_ATTR"));	//!< リンク内属性テーブル名称
const static LPCTSTR CITY_LINE_TABLE_NAME		= ( _T("CITY_DISP_LINE"));	//!< 都市地図注記表示位置テーブル名称
const static LPCTSTR CITY_ANNOTATION_TABLE_NAME	= ( _T("CITY_ANNOTATION"));	//!< 都市地図注記真位置テーブル名称
const static LPCTSTR BUILDING_LINE_TABLE_NAME	= ( _T("BUILDING_LINE"));	//!< 都市地図家形ライン（装飾線）テーブル名称
const static LPCTSTR BUILDING_TABLE_NAME		= ( _T("BUILDING"));		//!< 都市地図家形テーブル名称
const static LPCTSTR BUILDING_STEP_TABLE_NAME	= ( _T("BUILDING_STEP"));	//!< 都市地図建物階数テーブル名称
const static LPCTSTR CITYLINE_TABLE_NAME		= ( _T("CITY_LINE"));		//!< 都市地図背景ラインテーブル名称
const static LPCTSTR CITYSITE_TABLE_NAME		= ( _T("CITY_SITE"));		//!< 都市地図背景ポリゴンテーブル名称
const static LPCTSTR CITYRAILWAY_TABLE_NAME		= ( _T("CITY_RAILWAY"));	//!< 都市地図鉄道テーブル名称
const static LPCTSTR CITYSTATION_TABLE_NAME		= ( _T("CITY_STATION"));	//!< 都市地図駅舎テーブル名称
const static LPCTSTR ADMINPOLY_TABLE_NAME		= ( _T("ADMIN_POLY"));		//!< 行政界ポリゴンテーブル名称
const static LPCTSTR BASE_SC1_LINE_TABLE_NAME	= ( _T("B_SC1DISP_LINE"));	//!< 中縮S1注記表示位置テーブル名称
const static LPCTSTR BASE_SC2_LINE_TABLE_NAME	= ( _T("B_SC2DISP_LINE"));	//!< 中縮S2注記表示位置テーブル名称
const static LPCTSTR BASE_SC3_LINE_TABLE_NAME	= ( _T("B_SC3DISP_LINE"));	//!< 中縮S3注記表示位置テーブル名称
const static LPCTSTR BASE_SC4_LINE_TABLE_NAME	= ( _T("B_SC4DISP_LINE"));	//!< 中縮S4注記表示位置テーブル名称
const static LPCTSTR BASE_ANNOTATION_TABLE_NAME	= ( _T("BASE_ANNOTATION"));	//!< 中縮注記真位置テーブル名称
const static LPCTSTR MIDDLE_SC1_LINE_TABLE_NAME	= ( _T("M_SC1DISP_LINE"));	//!< ミドルS1注記表示位置テーブル名称
const static LPCTSTR MIDDLE_SC2_LINE_TABLE_NAME	= ( _T("M_SC2DISP_LINE"));	//!< ミドルS2注記表示位置テーブル名称
const static LPCTSTR MIDDLE_SC3_LINE_TABLE_NAME	= ( _T("M_SC3DISP_LINE"));	//!< ミドルS3注記表示位置テーブル名称
const static LPCTSTR MIDDLE_SC4_LINE_TABLE_NAME	= ( _T("M_SC4DISP_LINE"));	//!< ミドルS4注記表示位置テーブル名称
const static LPCTSTR MIDDLE_ANNOTATION_TABLE_NAME	= ( _T("MIDDLE_ANNOTATION"));	//!< ミドル注記真位置テーブル名称
const static LPCTSTR TOP_SC1_LINE_TABLE_NAME	= ( _T("T_SC1DISP_LINE"));	//!< トップS1注記表示位置テーブル名称
const static LPCTSTR TOP_SC2_LINE_TABLE_NAME	= ( _T("T_SC2DISP_LINE"));	//!< トップS2注記表示位置テーブル名称
const static LPCTSTR TOP_SC3_LINE_TABLE_NAME	= ( _T("T_SC3DISP_LINE"));	//!< トップS3注記表示位置テーブル名称
const static LPCTSTR TOP_SC4_LINE_TABLE_NAME	= ( _T("T_SC4DISP_LINE"));	//!< トップS4注記表示位置テーブル名称
const static LPCTSTR TOP_ANNOTATION_TABLE_NAME	= ( _T("TOP_ANNOTATION"));	//!< トップ注記真位置テーブル名称
const static LPCTSTR LOADPOINT_TABLE_NAME		= ( _T("ROAD_POINT"));		//!< roadpoint, normalpointテーブル名称
const static LPCTSTR HWYNODE_TABLE_NAME			= ( _T("HWY_NODE"));		//!< hwynode、tollnodeテーブル名称
const static LPCTSTR GOU_POINT_TABLE_NAME		= ( _T("GOU_POINT"));		//!< 号ポイントテーブル名称
const static LPCTSTR BASE_REP_POINT_TABLE_NAME	= ( _T("BASE_REP_POINT"));	//!< 中縮代表点テーブル名称
const static LPCTSTR BASEMESH_TABLE_NAME		= ( _T("BASEMESH"));		//!< 2次メッシュテーブル名称
const static LPCTSTR CITYMESH_TABLE_NAME		= ( _T("CITYMESH"));		//!< 都市地図メッシュテーブル名称
const static LPCTSTR MIDDLEMESH_TABLE_NAME		= ( _T("MIDDLEMESH"));		//!< ミドルメッシュテーブル名称
const static LPCTSTR TOPMESH_TABLE_NAME			= ( _T("TOPMESH"));			//!< トップメッシュテーブル名称
const static LPCTSTR BASE_ADMIN_TABLE_NAME		= ( _T("BASE_ADMIN"));		//!< 中縮行政界テーブル名称
const static LPCTSTR CITY_ADMIN_TABLE_NAME		= ( _T("CITY_ADMIN"));		//!< 都市地図行政界テーブル名称
const static LPCTSTR ACCESS_POINT_TABLE_NAME	= ( _T("ACCESS_POINT"));	//!< 高速・有料道路検索関係ポイントテーブル名称
const static LPCTSTR HIGHWAY_NODE_TABLE_NAME	= ( _T("HIGHWAY_NODE"));	//!< ハイウェイ関連分岐合流・料金所ポイントテーブル名称
const static LPCTSTR KLINK_POINT_TABLE_NAME		= ( _T("KLINK_POINT"));		//!< 家形リンクポイントテーブル名称
const static LPCTSTR FACILINFO_TABLE_NAME		= ( _T("FACIL_INFO_POINT"));//!< 施設情報ポイントテーブル名称
const static LPCTSTR SAPA_INFO_TABLE_NAME		= ( _T("SAPA_INFO_TABLE")); //!< 施設情報名称
const static LPCTSTR DISP_ONLY_LINK_TABLE_NAME	= ( _T("DISP_ONLY_LINK"));	//!< 表示専用道路テーブル名称
const static LPCTSTR PARKING_SHAPE_TABLE_NAME	= ( _T("PARKING_SHAPE"));	//!< 駐車場外形ポリゴンテーブル名称
const static LPCTSTR SURVEY_POINT_TABLE_NAME	= ( _T("SURVEY_POINT"));	//!< 現地調査ポイントテーブル名称
const static LPCTSTR CS_ADDRPOINT_TABLE_NAME	= ( _T("CS_ADDR_POINT"));	//!< 住所クレーム対応ポイントテーブル名称
const static LPCTSTR CLAIMPOLY_ADDR_TABLE_NAME	= ( _T("CLAIMPOLY_ADDR"));	//!< クレームポリゴン（住所）テーブル名称
const static LPCTSTR CLAIMPOLY_MAP_TABLE_NAME	= ( _T("CLAIMPOLY_MAP"));	//!< クレームポリゴン（地図）テーブル名称
const static LPCTSTR TOP_SITE_TABLE_NAME		= ( _T("TOP_SITE"));		//!< トップ背景テーブル名称
const static LPCTSTR TOP_LINE_TABLE_NAME		= ( _T("TOP_LINE"));		//!< トップラインテーブル名称
const static LPCTSTR TOP_CONTOUR_TABLE_NAME		= ( _T("TOP_CONTOUR"));		//!< トップ段彩テーブル名称
const static LPCTSTR CITY_REP_POINT_TABLE_NAME	= ( _T("CITY_REP_POINT"));	//!< 都市地図代表点テーブル名称
const static LPCTSTR HIMA_LOG_TABLE_NAME		= ( _T("HIMA_LOG"));		//!< ひまわりログテーブル名称

/// 関連テーブルのテーブル名称の定義
const static LPCTSTR DUPLILINK_TABLE_NAME		= ( _T("DUPLI_LINK"));
const static LPCTSTR HIGHWEYTEXT_TABLE_NAME		= ( _T("HIGHWAY_TEXT"));
const static LPCTSTR SAPAINFO_TABLE_NAME		= ( _T("SAPA_INFO_TABLE"));
const static LPCTSTR TOLLINFO_TABLE_NAME		= ( _T("TOLL_INFO_TABLE"));
const static LPCTSTR TIME_NOPASSAGE_TABLE_NAME	= ( _T("TIME_NOPASSAGE"));
const static LPCTSTR TIME_ONEWAY_TABLE_NAME		= ( _T("TIME_ONEWAY"));
const static LPCTSTR INF_TURNREG_TABLE_NAME		= ( _T("INF_TURNREG"));
const static LPCTSTR LQ_TURNREG_TABLE_NAME		= ( _T("LQ_TURNREG"));
const static LPCTSTR LQ_VICS_TABLE_NAME			= ( _T("LQ_VICS"));
const static LPCTSTR INF_VICS_TABLE_NAME		= ( _T("INF_VICS"));
const static LPCTSTR INF_BYWAY_TABLE_NAME		= ( _T("INF_BYWAY"));
const static LPCTSTR INF_ROUTE_TABLE_NAME		= ( _T("INF_ROUTE"));
const static LPCTSTR INF_INTERNAVI_TABLE_NAME	= ( _T("INF_INTERNAVI"));
const static LPCTSTR INF_IPCVICS_TABLE_NAME		= ( _T("INF_IPCVICS")); // [Bug 6473]Inf_ipcVics、Lq_ipcVicsに対応して欲しい
const static LPCTSTR INF_INTERSECTION_TABLE_NAME	= ( _T("INF_INTERSECTION")); // 進入方向別交差点名称

/// 参照テーブルのテーブル名称の定義
const static LPCTSTR REF_ROADCODE_TABLE_NAME	= ( _T("ROAD_CODE_LIST"));
const static LPCTSTR REF_ANNOCLASS_TABLE_NAME	= ( _T("ANNOTATION_CLASS"));
const static LPCTSTR REF_ALLEY_TABLE_NAME		= ( _T("ALLEY_MAINTE_LIST"));
const static LPCTSTR REF_ANNOBASE_TABLE_NAME	= ( _T("ANNO_BASE_CLASS"));
const static LPCTSTR REF_ANNOCITY_TABLE_NAME	= ( _T("ANNO_CITY_CLASS"));
const static LPCTSTR REF_ANNOMID_TABLE_NAME		= ( _T("ANNO_MIDDLE_CLASS"));
const static LPCTSTR REF_BACKGROUND_TABLE_NAME	= ( _T("BACKGROUND_CLASS"));

/// フィールド名称
const static LPCTSTR OPERATOR_FIELD				= ( _T( "OPERATOR"));
const static LPCTSTR MODIFYDATE_FIELD			= ( _T( "MODIFYDATE"));
const static LPCTSTR PURPOSE_C_FIELD			= ( _T( "PURPOSE_C"));
const static LPCTSTR UPDATETYPE_CFIELD			= ( _T( "UPDATETYPE_C"));
const static LPCTSTR UPDATE_CFIELD				= ( _T( "UPDATE_C"));
const static LPCTSTR USERCLAIM_F_FIELD			= ( _T( "USERCLAIM_F"));
const static LPCTSTR FIELDSURVEY_F_FIELD		= ( _T( "FIELDSURVEY_F"));
const static LPCTSTR FROM_FIELD					= ( _T( "FROM_NODE_ID"));
const static LPCTSTR TO_FIELD					= ( _T( "TO_NODE_ID"));
const static LPCTSTR NODECLASS_FIELD			= ( _T( "NODECLASS_C"));
const static LPCTSTR OBJECTID_FIELD				= ( _T("OBJECTID"));		// ObjectID
const static LPCTSTR ANNO_TARGET_STR_C_FIELD	= ( _T("TARGET_STR_C"));	// 長期表示レイヤーの注記対象番号フィールド
const static LPCTSTR ANNO_ID_FIELD				= ( _T("ANNO_ID"));			// 注記表示レイヤーの注記ID
const static LPCTSTR ANNO_FONT_SIZE_FIELD		= ( _T("FONTSIZE"));		// 注記表示レイヤーのフォントサイズ
const static LPCTSTR ANNO_CLASS_C_FIELD			= ( _T("ANNOCLASS_C"));		// 種別コード
const static LPCTSTR ANNO_DISPTYPE_C_FIELD		= ( _T("DISPTYPE_C"));		// 注記表示タイプ
const static LPCTSTR ANNO_SC1DISPTYPE_C_FIELD	= ( _T("SC1DISPTYPE_C"));	// S1注記表示タイプ
const static LPCTSTR ANNO_SC2DISPTYPE_C_FIELD	= ( _T("SC2DISPTYPE_C"));	// S2注記表示タイプ
const static LPCTSTR ANNO_SC3DISPTYPE_C_FIELD	= ( _T("SC3DISPTYPE_C"));	// S3注記表示タイプ
const static LPCTSTR ANNO_SC4DISPTYPE_C_FIELD	= ( _T("SC4DISPTYPE_C"));	// S4注記表示タイプ
const static LPCTSTR ANNO_NAME_STRING1_FIELD	= ( _T("NAME_STRING1"));	// 文字列フィールド1
const static LPCTSTR ANNO_NAME_STRING2_FIELD	= ( _T("NAME_STRING2"));	// 文字列フィールド2
const static LPCTSTR ANNO_NAME_STRING3_FIELD	= ( _T("NAME_STRING3"));	// 文字列フィールド3 (中縮用)
const static LPCTSTR ANNO_S1DISPRULE_FIELD		= ( _T("S1DISPRULE"));		// S1注記記号ルール
const static LPCTSTR ANNO_S2DISPRULE_FIELD		= ( _T("S2DISPRULE"));		// S2注記記号ルール
const static LPCTSTR ANNO_S3DISPRULE_FIELD		= ( _T("S3DISPRULE"));		// S3注記記号ルール
const static LPCTSTR ANNO_S4DISPRULE_FIELD		= ( _T("S4DISPRULE"));		// S4注記記号ルール
const static LPCTSTR ANNO_S5DISPRULE_FIELD		= ( _T("S5DISPRULE"));		// S5(都市地図)注記記号ルール
const static LPCTSTR ANNO_TRUE_POSITION_F_FIELD = ( _T("TRUEPOSITION_F"));	// 真位置位置
const static LPCTSTR ANNO_EXCEPT_SCALE_F_FIELD	= ( _T("EXCEPTSCALE_F"));	// 中抜け
const static LPCTSTR ANNO_DOUBLE_ANNO_F_FIELD	= ( _T("DOUBLEANNO_F"));	// 二行注記許可
const static LPCTSTR ANNO_COPYPASTE_F_FIELD		= ( _T("COPYPASTE_F"));		// コピー許可
const static LPCTSTR ANNO_IDENTICALDATA_F_FIELD	= ( _T("IDENTICALDATA_F"));	// 同一データ存在許可
const static LPCTSTR ONEWAY_C_FIELD				= ( _T("ONEWAY_C"));
const static LPCTSTR NOPASSAGE_C_FIELD			= ( _T("NOPASSAGE_C"));
const static LPCTSTR DAYOFWEEK1_C_FIELD			= ( _T("DAYOFWEEK1_C"));
const static LPCTSTR DAYOFWEEK2_C_FIELD			= ( _T("DAYOFWEEK2_C"));
const static LPCTSTR DAYOFWEEK3_C_FIELD			= ( _T("DAYOFWEEK3_C"));
const static LPCTSTR DAYOFWEEK4_C_FIELD			= ( _T("DAYOFWEEK4_C"));
const static LPCTSTR TURNREG_ID_FIELD			= ( _T("TURNREG_ID"));
const static LPCTSTR VICS_ID_FIELD				= ( _T("VICS_ID"));
const static LPCTSTR VICSCLASS_C_FIELD			= ( _T("VICSCLASS_C"));
const static LPCTSTR VICS_LINK_ID_FIELD			= ( _T("VICS_LINK_ID"));
const static LPCTSTR BACKROAD_F_FIELD			= ( _T("BACKROAD_F"));
const static LPCTSTR BYWAY_F_FIELD				= ( _T("BYWAY_F"));
const static LPCTSTR ROUTECLASS_C_FIELD			= ( _T("ROUTECLASS_C"));

/// フィールド名称(注記ルール関連)
const static LPCTSTR ANNO_CLASS_FIELD				= ( _T("CLASS"));				// 注記種別
const static LPCTSTR ANNO_MS_NO_FIELD				= ( _T("MS_NO"));				// 原稿番号
const static LPCTSTR ANNO_CLASS_NAME_FIELD			= ( _T("CLASS_NAME"));			// 注記種別名称
const static LPCTSTR ANNO_TOP_FLAG_FIELD			= ( _T("TOP_FLAG"));			// トップ使用可
const static LPCTSTR ANNO_MIDDLE_FLAG_FIELD			= ( _T("MIDDLE_FLAG"));			// ミドル使用可
const static LPCTSTR ANNO_BASE_FLAG_FIELD			= ( _T("BASE_FLAG"));			// 注縮使用可
const static LPCTSTR ANNO_CITY_FLAG_FIELD			= ( _T("CITY_FLAG"));			// 都市地図使用可
const static LPCTSTR ANNO_DISP_TYPE_FIELD			= ( _T("DISP_TYPE"));			// 注記表示タイプ
const static LPCTSTR ANNO_T_S1DISPRULE_FIELD		= ( _T("T_S1DISPRULE"));		// トップS1注記記号ルール
const static LPCTSTR ANNO_T_S2DISPRULE_FIELD		= ( _T("T_S2DISPRULE"));		// トップS2注記記号ルール
const static LPCTSTR ANNO_T_S3DISPRULE_FIELD		= ( _T("T_S3DISPRULE"));		// トップS3注記記号ルール
const static LPCTSTR ANNO_T_S4DISPRULE_FIELD		= ( _T("T_S4DISPRULE"));		// トップS4注記記号ルール
const static LPCTSTR ANNO_T_TRUE_POSITION_F_FIELD	= ( _T("T_TRUEPOSITION_F"));	// トップ真位置位置
const static LPCTSTR ANNO_T_FONTSIZE_FIELD			= ( _T("T_FONTSIZE"));			// トップフォントサイズ
const static LPCTSTR ANNO_T_EXCEPT_SCALE_F_FIELD	= ( _T("T_EXCEPTSCALE_F"));		// トップ中抜け
const static LPCTSTR ANNO_T_DOUBLE_ANNO_F_FIELD		= ( _T("T_DOUBLEANNO_F"));		// トップ二行注記許可
const static LPCTSTR ANNO_T_COPYPASTE_F_FIELD		= ( _T("T_COPYPASTE_F"));		// トップコピー許可
const static LPCTSTR ANNO_T_IDENTICALDATA_F_FIELD	= ( _T("T_IDENTICALDATA_F"));	// トップ同一データ存在許可
const static LPCTSTR ANNO_M_S1DISPRULE_FIELD		= ( _T("M_S1DISPRULE"));		// ミドルS1注記記号ルール
const static LPCTSTR ANNO_M_S2DISPRULE_FIELD		= ( _T("M_S2DISPRULE"));		// ミドルS2注記記号ルール
const static LPCTSTR ANNO_M_S3DISPRULE_FIELD		= ( _T("M_S3DISPRULE"));		// ミドルS3注記記号ルール
const static LPCTSTR ANNO_M_S4DISPRULE_FIELD		= ( _T("M_S4DISPRULE"));		// ミドルS4注記記号ルール
const static LPCTSTR ANNO_M_TRUE_POSITION_F_FIELD	= ( _T("M_TRUEPOSITION_F"));	// ミドル真位置位置
const static LPCTSTR ANNO_M_FONTSIZE_FIELD			= ( _T("M_FONTSIZE"));			// ミドルフォントサイズ
const static LPCTSTR ANNO_M_EXCEPT_SCALE_F_FIELD	= ( _T("M_EXCEPTSCALE_F"));		// ミドル中抜け
const static LPCTSTR ANNO_M_DOUBLE_ANNO_F_FIELD		= ( _T("M_DOUBLEANNO_F"));		// ミドル二行注記許可
const static LPCTSTR ANNO_M_COPYPASTE_F_FIELD		= ( _T("M_COPYPASTE_F"));		// ミドルコピー許可
const static LPCTSTR ANNO_M_IDENTICALDATA_F_FIELD	= ( _T("M_IDENTICALDATA_F"));	// ミドル同一データ存在許可
const static LPCTSTR ANNO_B_S1DISPRULE_FIELD		= ( _T("B_S1DISPRULE"));		// 中縮S1注記記号ルール
const static LPCTSTR ANNO_B_S2DISPRULE_FIELD		= ( _T("B_S2DISPRULE"));		// 中縮S2注記記号ルール
const static LPCTSTR ANNO_B_S3DISPRULE_FIELD		= ( _T("B_S3DISPRULE"));		// 中縮S3注記記号ルール
const static LPCTSTR ANNO_B_S4DISPRULE_FIELD		= ( _T("B_S4DISPRULE"));		// 中縮S4注記記号ルール
const static LPCTSTR ANNO_B_TRUE_POSITION_F_FIELD	= ( _T("B_TRUEPOSITION_F"));	// 中縮真位置位置
const static LPCTSTR ANNO_B_FONTSIZE_FIELD			= ( _T("B_FONTSIZE"));			// 中縮フォントサイズ
const static LPCTSTR ANNO_B_EXCEPT_SCALE_F_FIELD	= ( _T("B_EXCEPTSCALE_F"));		// 中縮中抜け
const static LPCTSTR ANNO_B_DOUBLE_ANNO_F_FIELD		= ( _T("B_DOUBLEANNO_F"));		// 中縮二行注記許可
const static LPCTSTR ANNO_B_COPYPASTE_F_FIELD		= ( _T("B_COPYPASTE_F"));		// 中縮コピー許可
const static LPCTSTR ANNO_B_IDENTICALDATA_F_FIELD	= ( _T("B_IDENTICALDATA_F"));	// 中縮同一データ存在許可
const static LPCTSTR ANNO_C_SDISPRULE_FIELD			= ( _T("C_SDISPRULE"));			// 都市地図注記記号ルール
const static LPCTSTR ANNO_C_TRUE_POSITION_F_FIELD	= ( _T("C_TRUEPOSITION_F"));	// 都市地図真位置位置
const static LPCTSTR ANNO_C_FONTSIZE_FIELD			= ( _T("C_FONTSIZE"));			// 都市地図フォントサイズ
const static LPCTSTR ANNO_C_EXCEPT_SCALE_F_FIELD	= ( _T("C_EXCEPTSCALE_F"));		// 都市地図中抜け
const static LPCTSTR ANNO_C_DOUBLE_ANNO_F_FIELD		= ( _T("C_DOUBLEANNO_F"));		// 都市地図二行注記許可
const static LPCTSTR ANNO_C_COPYPASTE_F_FIELD		= ( _T("C_COPYPASTE_F"));		// 都市地図コピー許可
const static LPCTSTR ANNO_C_IDENTICALDATA_F_FIELD	= ( _T("C_IDENTICALDATA_F"));	// 都市地図同一データ存在許可


/// フィールド名称（背景関連）
const static LPCTSTR BACKGROUND_CLASS				=( _T("CLASS"));			// 種別コード
const static LPCTSTR BACKGROUND_CLASS_NAME			=( _T("CLASS_NAME"));		// 背景種別名称
const static LPCTSTR BACKGROUND_MS_NO				=( _T("MS_NO"));			// 原稿番号
const static LPCTSTR BACKGROUND_LAYERID				=( _T("LAYERID"));			// レイヤID

/// NULL OK なフィールドのNULL値定義他
#define NULL_VALUE     (_T("<null>"))
#define EMPTY_VALUE    (_T("<empty>"))
#define TRUE_VALUE     (_T("<true>"))
#define FALSE_VALUE    (_T("<false>"))
#define GEOMETRY_VALUE (_T("<shape>"))

// ワーニングレベル
const static int WARNLEVEL_NONE   = 0;	//!< 権限なし？のワーニングレベル
const static int WARNLEVEL_LOW    = 1;	//!< 下位権限ワーニングレベル
const static int WARNLEVEL_MIDDLE = 2;	//!< 中位権限ワーニングレベル
const static int WARNLEVEL_HIGH   = 3;	//!< 上位権限ワーニングレベル

// 注記定数
//#ifdef _USE_ANNOTATION_TRICKY_DEFINE
#define ANNO_CITY_1PT			0.000008950		// 都市地図注記文字幅(1pt)
#define ANNO_BASE_S4_1PT		0.000035808		// 中縮S4注記文字幅(1pt)
#define ANNO_BASE_S3_1PT		0.000071608		// 中縮S3注記文字幅(1pt)
#define ANNO_BASE_S2_1PT		0.000143233		// 中縮S2注記文字幅(1pt)
#define ANNO_BASE_S1_1PT		0.000286458		// 中縮S1注記文字幅(1pt)
#define ANNO_MIDDLE_S4_1PT		0.000572800		// ミドルS4注記文字幅(1pt)
#define ANNO_MIDDLE_S3_1PT		0.001145866		// ミドルS3注記文字幅(1pt)
#define ANNO_MIDDLE_S2_1PT		0.002291733		// ミドルS2注記文字幅(1pt)
#define ANNO_MIDDLE_S1_1PT		0.004583333		// ミドルS1注記文字幅(1pt)
#define ANNO_TOP_S4_1PT			0.009166666		// トップS4注記文字幅(1pt)	前のを倍にしただけ。もともと合ってないのでチョーてきとー
#define ANNO_TOP_S3_1PT			0.018333332		// トップS3注記文字幅(1pt)	前のを倍にしただけ。もともと合ってないのでチョーてきとー
#define ANNO_TOP_S2_1PT			0.036666664		// トップS2注記文字幅(1pt)	前のを倍にしただけ。もともと合ってないのでチョーてきとー
#define ANNO_TOP_S1_1PT			0.073333328		// トップS1注記文字幅(1pt)	前のを倍にしただけ。もともと合ってないのでチョーてきとー
//#endif // ifdef _USE_ANNOTATION_TRICKY_DEFINE

#define ANNO_NONE_MULTI			1.363682647		// 文字間隔なし乗数
#define ANNO_HALF_MULTI			1.931841325		// 半角乗数
#define ANNO_FULL_MULTI			2.613646288		// 全角乗数
#define ANNO_DOUBLE_MULTI		3.977328935		// 二倍角乗数
//#define ANNO_ROAD_MULTI			1.705074488		// 道路番号・主要地方道番号乗数 <- [bug 3974] 何で1.7倍するの？？わけわからんので廃止

#define ANNO_POINT_DIFF_MULTI	1.092295158		// フォントサイズ差の乗数 ((文字幅*乗数)*(xPoint-12Point))+(文字幅*12Point) がxPointの文字の文字幅

#define	ANNO_HEIGHT_MULTI		1.261034048		// 注記縦間隔乗数(1pt)

#define ANNO_CITY_SMALL_MAKR_RADIUS		5.460893855251		// 都市地図注記小記号半径(Point) [記号と文字列までの距離]
#define ANNO_CITY_MARK_RADIUS			10.907821228503		// 都市地図注記記号半径(Point)
#define ANNO_BASE_SMALL_MARK_RADIUS		5.5186714682		// 中縮注記小記号半径(Point)
#define ANNO_BASE_MARK_RADIUS			8.1396951808		// 中縮注記記号半径(Point)
#define ANNO_MIDDLE_SMALL_MARK_RADIUS	5.5186714682		// ミドル注記小記号半径(Point)
#define ANNO_MIDDLE_MARK_RADIUS			8.1396951808		// ミドル注記記号半径(Point)
#define ANNO_TOP_SMALL_MARK_RADIUS		5.5186714682		// トップ注記小記号半径(Point)
#define ANNO_TOP_MARK_RADIUS			8.1396951808		// トップ注記記号半径(Point)
#define ANNO_MARK_MULTI					0.8					// 記号サイズ乗数
//#define ANNO_MARK_TO_LEFT_MULTI			1.4					// 記号の左距離乗数(CETでは、右と左で、左の方が遠い為) <- [bug 3039] 根拠が無いので廃止

#define ANNO_CITY_SCALE			3125.0		// 都市地図基準スケール
#define ANNO_BASE_S4_SCALE		12500.0		// 中縮S4基準スケール
#define ANNO_BASE_S3_SCALE		25000.0		// 中縮S3基準スケール
#define ANNO_BASE_S2_SCALE		50000.0		// 中縮S2基準スケール
#define	ANNO_BASE_S1_SCALE		100000.0	// 中縮S1基準スケール
#define ANNO_MIDDLE_S4_SCALE	200000.0	// ミドルS4基準スケール
#define ANNO_MIDDLE_S3_SCALE	400000.0	// ミドルS3基準スケール
#define ANNO_MIDDLE_S2_SCALE	800000.0	// ミドルS2基準スケール
#define	ANNO_MIDDLE_S1_SCALE	1600000.0	// ミドルS1基準スケール
#define ANNO_TOP_S4_SCALE		3200000.0	// トップS4基準スケール
#define ANNO_TOP_S3_SCALE		6400000.0	// トップS3基準スケール
#define ANNO_TOP_S2_SCALE		12800000.0	// トップS2基準スケール
#define	ANNO_TOP_S1_SCALE		25600000.0	// トップS1基準スケール

// 使用するレジストリキー
#define ROOT_KEY			HKEY_LOCAL_MACHINE
#define SOFTWARE_KEY		(_T("Software\\"))
#define INSTALL_KEY			(_T("INCREMENT P CORPORATION\\SiNDY-e\\"))
#define INSTALLROOT_KEY		(_T("InstallRoot"))
#define TOOLS_KEY			(_T("Tools\\"))		// Tools\各ツール名とすることを前提 HKEY_CURRENT_USER以下に書き込み
#define PATH_KEY			(_T("Path"))
#define INIFILEPATH_KEY		(_T("InitFileName"))
#define CLSID_KEY			(_T("clsid"))
#define DEFAULTSYMBOL_KEY	(_T("デフォルト"))
#define DEFAULT_KEY			(_T("unset"))		/// ダミー

#define	WARNING_SECTION		(_T("Warning"))
#define	LEVEL_KEY			(_T("Level"))

// ヘルプID
//#include "HelpID.h"

//#define CURRENTUSER_KEY (_T("SOFTWARE\\Increment P Corporation\\SiNDY-e\\CurrentUser"))
//#define WORKNAME_KEY (_T("SOFTWARE\\Increment P Corporation\\SiNDY-e\\Work"))

//
// IMultiSymbolRenderer で使用する描画モード
#define _MULTISYMBOL_VERTEX		1	//!< 構成点描画モード
#define _MULTISYMBOL_ONEWAY		2	//!< 一方通行描画モード
#define _MULTISYMBOL_ANNOTATION	4	//!< 注記描画モード
#define _MULTISYMBOL_NOLRTURN	8	//!< 右左折禁止描画モード
#define _MULTISYMBOL_GUIDE		16	//!< 強制誘導描画モード
#define _MULTISYMBOL_DIRGUIDE	32	//!< 方面案内描画モード
#define _MULTISYMBOL_VICS		64	//!< VICS描画モード
#define _MULTISYMBOL_LANE		128	//!< レーン情報描画モード
#define _MULTISYMBOL_BYWAY		256	//!< 抜け道描画モード
#define _MULTISYMBOL_S1ANNOPOINT	512	    //!< S1真位置描画モード
#define _MULTISYMBOL_S2ANNOPOINT	1024	//!< S2真位置描画モード
#define _MULTISYMBOL_S3ANNOPOINT	2048	//!< S3真位置描画モード
#define _MULTISYMBOL_S4ANNOPOINT	4096	//!< S4真位置描画モード
#define _MULTISYMBOL_ROUTE			8192	//!< アクセス／乗り継ぎ道描画モード
#define _MULTISYMBOL_UTURN			16384	//!< Uターン可能箇所描画モード
#define _MULTISYMBOL_NOPASSAGE		32768	//!< 通行禁止規制描画モード
#define _MULTISYMBOL_INTERNAVI		65536	//!< インターナビVICS描画モード
#define _MULTISYMBOL_ENDPOINT		131072	//!< 構成点描画モード
#define _MULTISYMBOL_IPCVICS		262144	//!< IPC VICS描画モード [Bug 6473]Inf_ipcVics、Lq_ipcVicsに対応して欲しい
#define _MULTISYMBOL_INTERSECTION	524288	//!< 進入方向別交差点描画モード

// ツール
enum EDITOR_TOOL
{
	EDITTOOL,			//!< フィーチャ移動ツール
	SKETCHTOOL,			//!< フィーチャ作成ツール
	DELETETOOL,			//!< フィーチャ削除ツール
	ATTRIBUTETOOL,		//!< フィーチャ属性編集ツール
};

// LQ 用
class ROWDEF
{
public:
	ROWDEF() : oid(-1), flg(-1), direction(-1), sequence(-1) {};
	~ROWDEF(){};
	LONG			oid;		// IRow、IFeature 用
	_IRowPtr		obj;		// IRow、IFeature 用
	IGeometryPtr	shp;		// IFeature 用
	LONG			flg;		// LQ のタイプ保持用
#if _ATL_VER < 0x0800
	CString			name;		// テーブル名など
#else
	ATL::CString    name;		// テーブル名など
#endif // if _ATL_VER < 0x0800
	LONG			direction;	// LINKDIR_C 格納用（道路リンクを格納する場合のみ）
	LONG			sequence;	// SEQUENCE 格納用（道路リンクを格納する場合のみ）
};

// 描画用
struct SYMBOL {
	ISimpleLineSymbolPtr			piNormalSymbol;
	ISimpleLineSymbolPtr			piNormalSymbolNarrow;
	ICartographicLineSymbolPtr		piArrowSymbol;
	ICartographicLineSymbolPtr		piArrowSymbolNarrow;
};

struct LQ {
	BOOL			bDrawAllow;
	BOOL			bIsNarrow;
	BOOL			bIsDispID;
	LONG			ID;
	COLORREF		ID_Color;
	COLORREF		Color;
	DOUBLE			Size;
	IGeometryPtr	piGeom;
	LONG			DrawMode;
};

inline bool __stdcall operator<(const ROWDEF& c1, const ROWDEF& c2){ return ( c1.sequence < c2.sequence ); };
inline bool __stdcall operator>(const ROWDEF& c1, const ROWDEF& c2){ return ( c1.sequence > c2.sequence ); };

// 色定義
const static COLORREF _COLOR_SELECTEXTENT = RGB(  100,   72,   41 );	//!< オーバービュー・リンク列編集ツールの選択範囲色

// Program ID
const static LPCTSTR EDITTOOL_UID		= _T("EditTool.EditToolObj");
const static LPCTSTR DELETETOOL_UID		= _T("DeleteTool.DeleteToolObj");
const static LPCTSTR SKETCHTOOL_UID		= _T("SketchTool.SketchToolObj");
const static LPCTSTR ATTRTOOL_UID		= _T("AttributeTool.AttributeToolObj");
const static LPCTSTR LQEDITTOOL_UID		= _T("LQEditTool.LQEditToolObj");
const static LPCTSTR LQATTRTOOL_UID		= _T("LQAttributeTool.LQAttributeToolObj");
const static LPCTSTR LQSKETCHTOOL_UID	= _T("LQSketchTool.LQSketchToolObj");
const static LPCTSTR LQDELETETOOL_UID	= _T("LQDeleteTool.LQDeleteToolObj");
const static LPCTSTR SEARCHTOOL_UID		= _T("SearchTool.SearchToolObj");
const static LPCTSTR COPYTOOL_UID		= _T("FeatureCopyTool.FeatureCopyToolObj");
const static LPCTSTR MEARGETOOL_UID		= _T("MergeTool.MergeToolObj");
//const static LPCTSTR ERRJMPTOOL_UID		= _T("ErrorJumpTool.ErrorJumpToolObj");
const static LPCTSTR LOGHANDLETOOL_UID		= _T("LogHandleTool.LogHandleToolObj");
const static LPCTSTR SPLITTOOL_UID		= _T("SplitTool.SplitToolObj");
const static LPCTSTR OVERVIEWTOOL_UID	= _T("OverViewTool.OverViewToolObj");
const static LPCTSTR VIEWVERTOOL_UID	= _T("ViewVersionTool.ViewVersionToolObj");
const static LPCTSTR CHGSYMBOL_UID		= _T("ChangeSymbolMenu.ChangeSymbolMultiSpatialObj");
const static LPCTSTR GRADWIZA_UID		= _T("GradWizard.GradWizard");
const static LPCTSTR GRADEDITCMD_UID	= _T("GradEditTool.GradEditCmd");
const static LPCTSTR LDRASTERCMD_UID	= _T("LoadRasterCommand.LoadRasterCommandObj");
const static LPCTSTR MSICMD_UID			= _T("MSICommand.MSICommandObj");
const static LPCTSTR LOCALMENU_UID		= _T("LocalEditMenu.LocalEditMultiObj");
const static LPCTSTR LOCALEDIT_UID		= _T("LocalEditMgrCommand.LocalEditMgrCommandObj");
const static LPCTSTR MAP_UID			= _T("Map.Start");
const static LPCTSTR MULTI_UID			= _T("MultiRenderer.MultiRendererObj");
const static LPCTSTR ROADSNAP_UID		= _T("RoadIntersectionSnap.RoadIntersectionSnapAgent");
const static LPCTSTR BOOKMARKMENU_UID	= _T("MapBookMarkMenu.MapBookMarkMenuObj");
const static LPCTSTR ADDRESSBAR_UID		= _T("AddressToolBar.AddressToolBarObj");
const static LPCTSTR ATTRDLG_UID		= _T("AttributeDlg.AttributeDlgObj");
const static LPCTSTR FEATSELECTDLG_UID	= _T("FeatureSelectDlg.FeatureSelectDlgObj");
const static LPCTSTR TXTSTAMPER_UID		= _T("TextStamper.TextStamperObj");
const static LPCTSTR SNAPTOOL_UID		= _T("sindyEditor.SiNDYSnapCommand");
const static LPCTSTR V3DVIEWTOOL_UID	= _T("V3DViewTool.V3DViewToolObj");
const static LPCTSTR DRVSURVEYTOOL_UID	= _T("DrvSurvey.DrvSrvyToolObj");
const static LPCTSTR CHGSYMBOLVIEW_UID	= _T("ChangeSymbolViewTool.ChangeSymbolViewToolObj");
const static LPCTSTR TRAFFICREGULATIONTOOL_UID	= _T("TrafficRegulationTool.TrafficRegulationToolObj");
const static LPCTSTR CONVHPGLTOOL_UID	= _T("Conv.ConvHpglObj");
const static LPCTSTR LOCATIONVIEWTOOL_UID	= _T("LocationView.LocationViewToolObj");
const static LPCTSTR SPOTVIEW_UID		= _T("SpotView.SpotViewObj");
const static LPCTSTR TRACKLOGMGR_UID	= _T("TrackLogMgr.TrackLogMgrObj");
const static LPCTSTR TRACKPOINTVIEW_UID	= _T("TrackPointView.TrackPointViewObj");
const static LPCTSTR TRACKLOGSEARCH_UID	= _T("TrackLogSearch.TrackLogSearchObj");

#define PROP_PRIORITY_COMMON				10000;
#define PROP_PRIORITY_ATTRIBUTETOOL			5000;
#define PROP_PRIORITY_EDITTOOL				4999;
#define PROP_PRIORITY_DELETETOOL			4900;
#define PROP_PRIORITY_SKETCHTOOL			4800;
#define PROP_PRIORITY_COPYTOOL				4700;
#define PROP_PRIORITY_TRAFFICREGULATIONTOOL	4600;
#define PROP_PRIORITY_LQDELETETOOL			3900;
#define PROP_PRIORITY_LQSKETCHTOOL			3800;
#define PROP_PRIORITY_LQATTRIBUTETOOL		3700;
#define PROP_PRIOTIRY_RULE					10;

void _cdecl DEBUGPRINT( LPCTSTR lpszFormat, ...);
inline void _cdecl DEBUGPRINT(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);

	int nBuf;
#ifndef UNICODE
	char szBuffer[512];

	nBuf = _vsnprintf_s(szBuffer, sizeof(szBuffer), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer)); //Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringA(szBuffer);
#else
	WCHAR szBuffer[512];

	nBuf = _vsnwprintf_s(szBuffer, sizeof(szBuffer) / sizeof(WCHAR), lpszFormat, args);
	ATLASSERT(nBuf < sizeof(szBuffer));//Output truncated as it was > sizeof(szBuffer)

	OutputDebugStringW(szBuffer);
#endif
	va_end(args);
}

#endif
