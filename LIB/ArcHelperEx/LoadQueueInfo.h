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

// LoadQueueInfo.h: CLoadQueueInfo クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_)
#define AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <list>
#include <algorithm>
#include "define.h"

/// リンク列情報のタイプ
enum sindyeLoadQueueInfoType {
	sindyeLoadQueueInfoInfoTableName,			//!< INFO_* テーブル名称
	sindyeLoadQueueInfoAliasName,				//!< エイリアス名称
	sindyeLoadQueueInfoLQTableName,				//!< LQ_* テーブル名称
	sindyeLoadQueueInfoLQRFRoadFieldName,		//!< 道路リンクの属性にある *_LQRF フィールド名称
//	sindyeLpadQueueInfoRelInfoLQFieldName,		//!< LQ_* テーブルの属性にある *_ID フィールド名称
	sindyeLoadQueueInfoColor,					//!< リンク列表示色
};

/// リンク列のタイプ
enum sindyeRoadQueueType
{
	sindyeRoadQueueDirguide,					//!< 方面案内
	sindyeRoadQueueGuide,						//!< 強制誘導
	sindyeRoadQueueVics,						//!< VICS
	sindyeRoadQueueByway,						//!< 抜け道
	sindyeRoadQueueTurnreg,						//!< 右左折禁止
	sindyeRoadQueueLane,						//!< レーン
	sindyeRoadQueueRoute,						//!< アクセス／乗り継ぎ道
	sindyeRoadQueueUTurn,						//!< Uターン
	sindyeRoadQueueInternavi,					//!< インターナビVICS
	sindyeRoadQueueIPCVics,						//!< iPCVICS(bug 6477)
	sindyeRoadQueueIntersection					//!< 進入方向別交差点
};

/// リンク列ハイライト色
const static COLORREF _COLOR_LQFLASH        = RGB( 0x00, 0xff, 0xff );
const static COLORREF _COLOR_LQFLASH_YELLOW = RGB( 0xff, 0xff, 0x00 ); // bug9405で追加

/// フィールド名称（リンク列関連）
const static LPCTSTR INF_ID_FIELD				= ( _T("INF_ID"));		// beta7 から sindyeLpadQueueInfoRelInfoLQFieldName は INF_ID に統一
const static LPCTSTR LINK_ID_FIELD				= ( _T("LINK_ID"));
const static LPCTSTR LINKDIR_C_FIELD			= ( _T("LINKDIR_C"));
const static LPCTSTR SEQUENCE_FIELD				= ( _T("SEQUENCE"));
const static LPCTSTR MESHCODE_FIELD				= ( _T("MESHCODE"));
const static LPCTSTR LASTLINK_F_FIELD			= ( _T("LASTLINK_F"));

// リンク列の開始・終了
const static INT sindyeLoadQueueStart			= sindyeRoadQueueDirguide;
const static INT sindyeLoadQueueEnd				= sindyeRoadQueueIntersection;

// チェックタイプ
enum sindyeCheckType
{
	sindyeCheckOneway,			//!< 一方通行
	sindyeCheckNoPassage,		//!< 通行禁止
	sindyeCheckNoPassage_AP,	//!< 通行禁止（関係者以外）
	sindyeCheckNoPassage_NG,	//!< 通行禁止（通行不可）
	sindyeDirguide,				//!< 方面案内
	sindyeGuide,				//!< 強制誘導
	sindyeVics,					//!< VICS
	sindyeByway,				//!< 抜け道
	sindyeTurnreg,				//!< 右左折禁止（条件なし）
	sindyeTurnreg_CN,			//!< 右左折禁止（条件あり）
	sindyeLane,					//!< レーン
	sindyeRoute,				//!< アクセス／乗り継ぎ道（乗り継ぎ道以外）
	sindyeRoute_TR,				//!< アクセス／乗り継ぎ道（乗り継ぎ道）
	sindyeUTurn,				//!< Uターン
	sindyeInternavi,			//!< インターナビVICS
	sindyeIPCVics,				//!< iPCVICS(bug 6477)
	sindyeIntersection			//!< 進入方向別交差点
};

// 操作
enum sindyeCheckOperation {
	sindyeCheckOperation_ONE,		// １リンク毎
	sindyeCheckOperation_END,		// 確定時
};

// 条件が複数あるので、右左折禁止（条件なし）と右左折禁止（条件あり）、アクセス道と乗り継ぎ道
// 右左折禁止（条件なし）とアクセス道はPrimaryを使用
// 右左折禁止（条件あり）と乗り継ぎ道はSecondaryを使用
// その他はPrimaryしか使用しない
const struct sindyeCheckConditionOperation {
	sindyeCheckOperation	Primary;		// 
	sindyeCheckOperation	Secondary;		// 
};

// 一方通行
const struct sindyeOnewayCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeOnewayCheckOperation CheckOnewayOperation[sindyeLoadQueueEnd + 1] = {
	// 					(なし,通過)					(なし,逆送)					(あり,通過)					(あり,逆送)					(なし,通過)					(なし,逆送)					(あり,通過)					(あり,逆送)
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 通行禁止
const struct sindyeNoPassCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPassCheckOperation CheckNoPassOperation[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり						なし						あり
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 通行禁止関係者以外
const struct sindyeNoPass_APCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPass_APCheckOperation CheckNoPass_APOperation[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり						なし						あり
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_END},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_END}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 通行禁止通行不可
const struct sindyeNoPass_NGCheckOperation {
	sindyeCheckOperation	Primary[2];		// 
	sindyeCheckOperation	Secondary[2];		// 
};

const struct sindyeNoPass_NGCheckOperation CheckNoPass_NGOperation[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり						なし						あり
	/* Dirguide */		{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* NoLRTurn */		{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Lane */			{{sindyeCheckOperation_END,	sindyeCheckOperation_END},	{sindyeCheckOperation_END,	sindyeCheckOperation_END}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 方面案内
const struct sindyeDirguideCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeDirguideCheckOperation CheckDirguideOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},	{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 強制誘導
const struct sindyeGuideCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeGuideCheckOperation CheckGuideOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// VICS
const struct sindyeVicsCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeVicsCheckOperation CheckVicsOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 抜け道
const struct sindyeBywayCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeBywayCheckOperation CheckBywayOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 右左折禁止条件なし
const struct sindyeNoLRTurnCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeNoLRTurnCheckOperation CheckNoLRTurnOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 右左折禁止条件あり
const struct sindyeNoLRTurn_CNCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeNoLRTurn_CNCheckOperation CheckNoLRTurn_CNOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_END,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// レーン
const struct sindyeLaneCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeLaneCheckOperation CheckLaneOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// アクセス道
const struct sindyeRouteCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeRouteCheckOperation CheckRouteOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 乗り継ぎ道
const struct sindyeRoute_TRCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeRoute_TRCheckOperation CheckRoute_TROperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// Uターン
const struct sindyeUTurnCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeUTurnCheckOperation CheckUTurnOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_END,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// インターナビVICS
const struct sindyeInternaviCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeInternaviCheckOperation CheckInternaviOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// iPCVICS
const struct sindyeiPCVICSCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeiPCVICSCheckOperation CheckiPCVICSOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// Intersection
const struct sindyeIntersectionCheckOperation {
	sindyeCheckOperation	Primary[4];		// 
	sindyeCheckOperation	Secondary[4];		// 
};

const struct sindyeIntersectionCheckOperation CheckIntersectionOperation[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向			部分包含・同方向			部分包含・逆方向			完全包含・同方向			完全包含・逆方向
	/* Dirguide */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Guide */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Vics */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Byway */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* NoLRTurn */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Lane */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Route */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* UTurn */			{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* InternaviVICS */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* iPCVICS */		{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
	/* Intersection */	{{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE},{sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE,	sindyeCheckOperation_ONE}},	
};

// 動作
enum sindyeCheckAction {
	sindyeCheckAction_NG,			// 不許可
	sindyeCheckAction_OK,			// 許可
	sindyeCheckAction_MSG,			// 確認
};

// 条件が複数あるので、右左折禁止（条件なし）と右左折禁止（条件あり）、アクセス道と乗り継ぎ道
// 右左折禁止（条件なし）とアクセス道はPrimaryを使用
// 右左折禁止（条件あり）と乗り継ぎ道はSecondaryを使用
// その他はPrimaryしか使用しない
const struct sindyeCheckConditionAction {
	sindyeCheckAction	Primary;		// 
	sindyeCheckAction	Secondary;		// 
};

// 一方通行
const struct sindyeOnewayCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeOnewayCheckAction CheckOnewayAction[sindyeLoadQueueEnd + 1] = {
	// 					(なし,通過)				(なし,逆送)				(あり,通過)				(あり,逆送)				(なし,通過)				(なし,逆送)				(あり,通過)					(あり,逆送)
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG},	{sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG}},
};

// 通行禁止
const struct sindyeNoPassCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPassCheckAction CheckNoPassAction[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり					なし					あり
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// 通行禁止関係者以外
const struct sindyeNoPass_APCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPass_APCheckAction CheckNoPass_APAction[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり					なし					あり
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// 通行禁止不可
const struct sindyeNoPass_NGCheckAction {
	sindyeCheckAction	Primary[2];		// 
	sindyeCheckAction	Secondary[2];		// 
};

const struct sindyeNoPass_NGCheckAction CheckNoPass_NGAction[sindyeLoadQueueEnd + 1] = {
	// 					なし						あり					なし					あり
	/* Dirguide */		{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Guide */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* NoLRTurn */		{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Lane */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* Route */			{{sindyeCheckAction_MSG,	sindyeCheckAction_MSG},	{sindyeCheckAction_MSG,	sindyeCheckAction_MSG}},	
	/* UTurn */			{{sindyeCheckAction_NG,		sindyeCheckAction_NG},	{sindyeCheckAction_NG,	sindyeCheckAction_NG}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,		sindyeCheckAction_MSG},	{sindyeCheckAction_NG,	sindyeCheckAction_MSG}},	
};

// 方面案内
const struct sindyeDirguideCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeDirguideCheckAction CheckDirguideAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// 強制誘導
const struct sindyeGuideCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeGuideCheckAction CheckGuideAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// VICS
const struct sindyeVicsCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeVicsCheckAction CheckVicsAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// 抜け道
const struct sindyeBywayCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeBywayCheckAction CheckBywayAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// 右左折禁止条件なし
const struct sindyeNoLRTurnCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeNoLRTurnCheckAction CheckNoLRTurnAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// 右左折禁止条件あり
const struct sindyeNoLRTurn_CNCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeNoLRTurn_CNCheckAction CheckNoLRTurn_CNAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向			部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_MSG,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,		sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// レーン
const struct sindyeLaneCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeLaneCheckAction CheckLaneAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// アクセス道
const struct sindyeRouteCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeRouteCheckAction CheckRouteAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// 乗り継ぎ道
const struct sindyeRoute_TRCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeRoute_TRCheckAction CheckRoute_TRAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG,	sindyeCheckAction_NG}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// Uターン
const struct sindyeUTurnCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeUTurnCheckAction CheckUTurnAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_MSG,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// インターナビVICS
const struct sindyeInternaviCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeInternaviCheckAction CheckInternaviAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},		// [bug7113]Internavi VICSとIPC VICSの共存OK
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// iPCVICS
const struct sindyeiPCVICSCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeiPCVICSCheckAction CheckiPCVICSAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},		// [bug7113]Internavi VICSとIPC VICSの共存OK
	/* iPCVICS */		{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
};

// Intersection
const struct sindyeIntersectionCheckAction {
	sindyeCheckAction	Primary[4];		// 
	sindyeCheckAction	Secondary[4];		// 
};

const struct sindyeIntersectionCheckAction CheckIntersectionAction[sindyeLoadQueueEnd + 1] = {
	// 					部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向		部分包含・同方向		部分包含・逆方向		完全包含・同方向		完全包含・逆方向
	/* Dirguide */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Guide */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Vics */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Byway */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* NoLRTurn */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Lane */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Route */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* UTurn */			{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* InternaviVICS */	{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* iPCVICS */		{{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK},	{sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK,	sindyeCheckAction_OK}},	
	/* Intersection */	{{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK},	{sindyeCheckAction_NG,	sindyeCheckAction_OK,	sindyeCheckAction_NG,	sindyeCheckAction_OK}},	
};

// ----------------------------------------------------------------------------
//!< リンク列規制ルールエラーコード
// ----------------------------------------------------------------------------
enum sindyeCheckErrorCode {
	sindyeCheckErrorCode_Duplicate,				// リンク列が重複してしまいます！
	sindyeCheckErrorCode_Set_Dirguide,				// 既に 方面案内 が設定されています！
	sindyeCheckErrorCode_Set_Guide,				// 既に 強制誘導 が設定されています！
	sindyeCheckErrorCode_Set_Vics,					// 既に VICS が設定されています！
	sindyeCheckErrorCode_Set_Byway,				// 既に 抜け道 が設定されています！
	sindyeCheckErrorCode_Set_Turnreg,				// 既に 右左折禁止 が設定されています！
	sindyeCheckErrorCode_Set_Lane,					// 既に レーン情報 が設定されています！
	sindyeCheckErrorCode_Set_Route,				// 既に アクセス／乗り継ぎ道 が設定されています！
	sindyeCheckErrorCode_Set_UTurn,				// 既に Uターン が設定されています！
	sindyeCheckErrorCode_Set_Internavi,			// 既に インターナビVICS が設定されています！
	sindyeCheckErrorCode_Set_ShapeDirection,		// 既に %s [同一形状 / 同一方向] が設定されています！
	sindyeCheckErrorCode_Set_Shape,				// 既に %s [同一形状] が設定されています！
	sindyeCheckErrorCode_Set_Direction,			// 既に %s [同一方向] が設定されています！
	sindyeCheckErrorCode_OutCourse1,				// VICS が設定されている 道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！
	sindyeCheckErrorCode_OutCourse2,				// インターナビVICS が設定されている 道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！
	sindyeCheckErrorCode_OutCourse3,				// VICS/インターナビVICS が設定されている 道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！
	sindyeCheckErrorCode_OutCourse_Vics,			// 経路外 の道路に VICS を設定することはできません！
	sindyeCheckErrorCode_OutCourse_Internavi,		// 経路外 の道路に インターナビVICS を設定することはできません！

	sindyeCheckErrorCode_Oneway,			// %s が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して 通過 です。
	sindyeCheckErrorCode_Oneway_Msg,		// %s が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して 通過 です。

	sindyeCheckErrorCode_NoPassage,				// %s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_AP,				// %s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_NG,				// %s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_Msg,			// %s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_AP_Msg,			// %s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_NG_Msg,			// %s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して 通過 です。

	sindyeCheckErrorCode_Oneway_LQ,		// %s[INF_ID:%d] が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して 通過 です。
	sindyeCheckErrorCode_Oneway_LQ_Msg,	// %s[INF_ID:%d] が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して 逆送 です。

	sindyeCheckErrorCode_NoPassageLQ,				// %s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_AP_LQ,			// %s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_NG_LQ,			// %s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して 通過 です。
	sindyeCheckErrorCode_NoPassageLQ_Msg,			// %s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_AP_LQ_Msg,		// %s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して 通過 です。
	sindyeCheckErrorCode_NoPassage_NG_LQ_Msg,		// %s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して 通過 です。

	sindyeCheckErrorCode_LQ,					// %s が リンク列規制エラー[%s] です！\n・道路リンク[OBJECTID:%d] の %s[INF_ID:%d] に対して %s です。
	sindyeCheckErrorCode_LQ_Msg,				// %s が リンク列規制エラー[%s] です！\n・道路リンク[OBJECTID:%d] の %s[INF_ID:%d] に対して %s です。

	sindyeCheckErrorCode_Duplicate_VICSID,			// %s[INF_ID:%d] は メッシュコード、リンク番号、種別が同じ %s が存在します！
	sindyeCheckErrorCode_AdjoinDisconnect_Node,	// 対象ノードがメッシュ境界にあります！
	sindyeCheckErrorCode_NoChange_VICS,			// %s は 交差点で変化しなければいけません!
	sindyeCheckErrorCode_Other,
};

const static LONG ErrorCode_MAX = sindyeCheckErrorCode_Other;

//!< リンク列規制ルールエラーデータ
const struct sindyeCheckErrorType {
	INT				MsgType;
	UINT			Type;
	UINT			Level;
	TCHAR			Message[256];
};

const struct sindyeCheckError {
	sindyeCheckErrorType		Type;
};

const struct sindyeCheckError Error[ErrorCode_MAX + 1] = {
#ifndef SINDY_FOR_ENGLISH
	// 	 (タイプ),						(レベル),		(メッセージ)
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("リンク列が重複してしまいます！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に 方面案内 が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に 強制誘導 が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に VICS が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に 抜け道 が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に 右左折禁止 が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に レーン が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に アクセス／乗り継ぎ道 が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に Uターン が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に インターナビVICS が設定されています！") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に %s [同一形状 / 同一方向] が設定されています！") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に %s [同一形状] が設定されています！") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("既に %s [同一方向] が設定されています！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("VICS が設定されている道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("インターナビVICS が設定されている道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("VICS/インターナビVICS が設定されている道路リンク[OBJECTID:%d]を 経路外 に設定することはできません！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("経路外 の道路に VICS を設定することはできません！") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("経路外 の道路に インターナビVICS を設定することはできません！") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して %s です。") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して %s です。") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して %s です。") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して %s です。") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s[INF_ID:%d] が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s[INF_ID:%d] が リンク列規制エラー[一方通行] です！\n・道路リンク[OBJECTID:%d] の 一方通行 に対して %s です。") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して %s です。") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して %s です。") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止] です！\n・道路リンク[OBJECTID:%d] の 通行禁止 に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止（関係者以外）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（関係者以外） に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[通行禁止（通行不可）] です！\n・道路リンク[OBJECTID:%d] の 通行禁止（通行不可） に対して %s です。") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s が リンク列規制エラー[%s] です！\n・道路リンク[OBJECTID:%d] の %s[INF_ID:%d] に対して %s です。") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s が リンク列規制エラー[%s] です！\n・道路リンク[OBJECTID:%d] の %s[INF_ID:%d] に対して %s です。") },

	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s[INF_ID:%d]　は メッシュコード、リンク番号、種別が同じ %s が存在します！") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s の 対象ノード がメッシュ境界にあります！") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s は 交差点で変化しなければいけません!") },
	{ 0, MB_ICONWARNING | MB_OK,		0,	_T("想定外エラーです。") },
#else
	// 	 (タイプ),						(レベル),		(メッセージ)
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("The link queue will duplicate!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Signpost has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Forced guidance has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("VICS has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Byway has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Turn restriction has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Lane has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Access/connection road has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("U-turn has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Internavi VICS has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same shape/Same direction] has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same shape] has already been set!") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("[Same direction] has already been set!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with Internavi VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the road link with VICS/Internavi VICS [OBJECTID:%d] to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the VICS to the link excluded from the route calculation!") },
	{ 0, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("You can not set the Internavi VICS to the link excluded from the route calculation!") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("It is %s to the one-way of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [one-way] of the road link [OBJECTID:%d]!\n It is %s to the one-way.") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction.") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic (Authorizsed personnel only)] of the road link [OBJECTID:%d]! \n It is %s to the Close-to-traffic (Authorizsed personnel only).") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation (impassable)] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction (Impassable).") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction.") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic (Authorizsed personnel only)] of the road link [OBJECTID:%d]! \n It is %s to the Close-to-traffic (Authorizsed personnel only).") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error to [Close-to-traffic regulation (impassable)] of the road link [OBJECTID:%d]! \n It is %s to the close-to-traffic restriction (Impassable).") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s[INF_ID:%d] is a link queue restriction error [one-way]! \n・It is %s to the one-way of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s[INF_ID:%d] is a link queue restriction error [one-way]! \n・It is %s to the one-way of the road link [OBJECTID:%d].") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic regulation]! \n・It is %s to the close-to-traffic restriction of the road link [OBJECTID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic (Authorizsed personnel only)]! \n・It is %s to the Close-to-traffic (Authorizsed personnel only) of the road link [OBJECTID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [Close-to-traffic regulation (impassable)]! \n・It is %s to the close-to-traffic restriction (Impassable) of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic regulation]! \n・It is %s to the close-to-traffic restriction of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic (Authorizsed personnel only)]! \n・It is %s to the Close-to-traffic (Authorizsed personnel only) of the road link [OBJECTID:%d].") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [Close-to-traffic regulation (impassable)]! \n・It is %s to the close-to-traffic restriction (Impassable) of the road link [OBJECTID:%d].") },

	{ 2, MB_ICONEXCLAMATION | MB_OK, 	0,	_T("%s is a link queue restriction error [%s]! \n・Road_Link[OBJECTID:%d]\n・%s[INF_ID:%d]\n It is %s.") },
	{ 1, MB_ICONQUESTION | MB_OKCANCEL, 0,	_T("%s is a link queue restriction error [%s]! \n・Road_Link[OBJECTID:%d]\n・%s[INF_ID:%d]\n It is %s.") },

	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%ｓ of the same mesh code, link number and type exists in %s[INF_ID:%d].") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("Target node of %s is on the mesh boundary line.") },
	{ 2, MB_ICONEXCLAMATION | MB_OK,	0,	_T("%s must be changed at the intersection.") },
	{ 0, MB_ICONWARNING | MB_OK,		0,	_T("Unexpected error") },
#endif
};

class CLoadQueueInfo
{
public:
	CLoadQueueInfo();
	virtual ~CLoadQueueInfo();
	void SetData();
	static CString GetLoadQueueInfo( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType, sindyeLoadQueueInfoType enumValueType );
	static CString GetLoadQueueInfo( sindyeRoadQueueType enumLQType, sindyeLoadQueueInfoType enumValueType );
	static CString GetLoadQueueInfo( LONG lDrawMode, sindyeLoadQueueInfoType enumValueType );
	static LONG GetLoadQueueDrawMode( sindyeRoadQueueType enumLQType );
	static LONG GetLoadQueueDrawMode( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType );
	static sindyeRoadQueueType GetLoadQueueType( LPCTSTR lpcszKey, sindyeLoadQueueInfoType enumKeyType );
	static sindyeRoadQueueType GetLoadQueueType( LONG lDrawMode );
	static sindyeRoadQueueType GetLoadQueueType( DWORD color );
	static LONG GetLoadQueueCheckType( sindyeRoadQueueType enumLQType );
	static sindyeRoadQueueType GetLoadQueueTypeByCheckType( LONG lCheckType );
	static DWORD GetLoadQueueColor( sindyeRoadQueueType enumLQType );

	// チェック操作を取得
	// SourceChkType:0=一方通行
	//               1=通行禁止
	//               2=通行禁止（関係者以外）
	//               3=通行禁止（通行不可）
	//               4=方面案内
	//               5=強制誘導
	//               6=VICS
	//               7=抜け道
	//               8=右左折禁止（条件なし）
	//               9=右左折禁止（条件あり）
	//               10=レーン
	//               11=アクセス道
	//               12=乗り継ぎ道
	//               13=Uターン
	//               14=インターナビVICS
	//               15=iPC VICS
	//               16=進入方向別交差点
	// DestChkType:0=条件なし・通過,条件なし,部分包含・同方向
	//             1=条件なし・逆送,条件あり,部分包含・逆方向
	//             2=条件あり・通過,完全包含・同方向
	//             3=条件あり・逆送,完全包含・逆方向
	static sindyeCheckConditionOperation GetCheckOperation(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType);

	// チェック動作を取得
	// SourceChkType:0=一方通行
	//               1=通行禁止
	//               2=通行禁止（関係者以外）
	//               3=通行禁止（通行不可）
	//               4=方面案内
	//               5=強制誘導
	//               6=VICS
	//               7=抜け道
	//               8=右左折禁止（条件なし）
	//               9=右左折禁止（条件あり）
	//               10=レーン
	//               11=アクセス道
	//               12=乗り継ぎ道
	//               13=Uターン
	//               14=インターナビVICS
	//               15=iPC VICS
	//               16=進入方向別交差点
	// DestChkType:0=条件なし・通過,条件なし,部分包含・同方向
	//             1=条件なし・逆送,条件あり,部分包含・逆方向
	//             2=条件あり・通過,完全包含・同方向
	//             3=条件あり・逆送,完全包含・逆方向
	static sindyeCheckConditionAction GetCheckAction(LONG lDrawMode, sindyeCheckType SourceChkType, long DestChkType);

	// エラータイプ取得
	static sindyeCheckErrorType GetError(sindyeCheckErrorCode ErrorCode);

	// [Bug8968] 追加
	// リンク列が短縮可能かどうかを取得（下記であればtrue）
	// 短縮可能：方面案内,強制誘導,右左折禁止,レーン,Uターン,進入方向別交差点
	static bool IsShortenerRoadQueue(sindyeRoadQueueType enumLQType);

private:
	static std::map<CString, INT>* GetInfoMap( sindyeLoadQueueInfoType enumKeyType );
	static std::map<CString, INT> m_mapInfoTableName;
	static std::map<CString, INT> m_mapAliasName;
	static std::map<CString, INT> m_mapLQTableName;
	static std::map<CString, INT> m_mapLQRFRoadFieldName;
//	static std::map<CString, INT> m_mapRelInfoLQFieldName;
	static std::map<LONG, INT>    m_mapDrawMode;
	static std::map<LONG, sindyeRoadQueueType>    m_mapCheckType;			// sindyeCheckTypeとsindyeLoadQueueInfoTypeの変換のため
	static std::map<DWORD, INT>   m_mapColor;

	static std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionOperation> > >	m_mapCheckOperation;
	static std::map< LONG, std::map< LONG, std::map<long,sindyeCheckConditionAction> > >	m_mapCheckAction;
	static std::map< LONG, sindyeCheckErrorType >	m_mapCheckError;
};

#endif // !defined(AFX_LOADQUEUEINFO_H__994546F6_42EA_4D7B_8BE7_3A20A990E2BE__INCLUDED_)
