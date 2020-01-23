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
#include "schema.h"

tFieldSchema g_CSDBFieldDef[] = {
{ 	_T("ID"),	_T( "管理番号" ),	esriFieldTypeString,	14,	0,	VARIANT_FALSE },
{ 	_T( "STATUS" ),	_T( "ステータス" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TYPE" ),	_T( "不具合対象データ" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "ERR_TITLE" ),	_T( "件名（概略）" ),	esriFieldTypeString,	4000,	0,	VARIANT_FALSE },
{ 	_T( "TDC_F" ),	_T( "TDC担当フラグ" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "WORK_F" ),	_T( "制作工程フラグ" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "INQ1" ),	_T( "調査結果_(テキスト)" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "SIRIUS_ID" ),	_T( "SiRiUS-ID" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "POI_ID" ),	_T( "POI背番号" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "LOC01_LAT" ),	_T( "不具合発生地点_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC01_LONG" ),	_T( "不具合発生地点_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LAT" ),	_T( "出発地_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LONG" ),	_T( "出発地_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LAT" ),	_T( "目的地_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LONG" ),	_T( "目的地_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC04_LAT" ),	_T( "立寄地_緯度" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "LOC04_LONG" ),	_T( "立寄地_経度" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01FCT" ),	_T( "No1:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01DTL" ),	_T( "No1:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_01TGT" ),	_T( "No1:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02FCT" ),	_T( "No2:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02DTL" ),	_T( "No2:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_02TGT" ),	_T( "No2:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03FCT" ),	_T( "No3:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03DTL" ),	_T( "No3:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_03TGT" ),	_T( "No3:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04FCT" ),	_T( "No4:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04DTL" ),	_T( "No4:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_04TGT" ),	_T( "No4:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05FCT" ),	_T( "No5:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05DTL" ),	_T( "No5:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ2_05TGT" ),	_T( "No5:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE }
};

tFieldSchema g_NAVIFieldDef[] = {
{ 	_T("ID"),	_T( "管理番号" ),	esriFieldTypeString,	14,	0,	VARIANT_FALSE },
{ 	_T( "STATUS" ),	_T( "ステータス" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TYPE" ),	_T( "不具合対象データ" ),	esriFieldTypeString,	255,	0,	VARIANT_FALSE },
{ 	_T( "ERR_TITLE" ),	_T( "件名（概略）" ),	esriFieldTypeString,	4000,	0,	VARIANT_FALSE },
{ 	_T( "TDC_F" ),	_T( "TDC担当フラグ" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "WORK_F" ),	_T( "制作工程フラグ" ),	esriFieldTypeSmallInteger,	1,	0,	VARIANT_FALSE },
{ 	_T( "INQ1" ),	_T( "症状詳細" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "SIRIUS_ID" ),	_T( "SiRiUS-ID" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "POI_ID" ),	_T( "POI背番号" ),	esriFieldTypeString,	255,	0,	VARIANT_TRUE },
{ 	_T( "LOC01_LAT" ),	_T( "不具合発生地点_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC01_LONG" ),	_T( "不具合発生地点_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LAT" ),	_T( "出発地_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC02_LONG" ),	_T( "出発地_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LAT" ),	_T( "目的地_緯度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC03_LONG" ),	_T( "目的地_経度" ),	esriFieldTypeDouble,	13,	9,	VARIANT_TRUE },
{ 	_T( "LOC04_LAT" ),	_T( "立寄地_緯度" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "LOC04_LONG" ),	_T( "立寄地_経度" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01FCT" ),	_T( "No1:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01DTL" ),	_T( "No1:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_01TGT" ),	_T( "No1:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02FCT" ),	_T( "No2:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02DTL" ),	_T( "No2:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_02TGT" ),	_T( "No2:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03FCT" ),	_T( "No3:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03DTL" ),	_T( "No3:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_03TGT" ),	_T( "No3:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04FCT" ),	_T( "No4:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04DTL" ),	_T( "No4:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_04TGT" ),	_T( "No4:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05FCT" ),	_T( "No5:原因の説明" ),		esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05DTL" ),	_T( "No5:対策方針テキスト" ),			esriFieldTypeString,	4000,	0,	VARIANT_TRUE },
{ 	_T( "INQ_05TGT" ),	_T( "No5:対策予定データ" ),	esriFieldTypeString,	4000,	0,	VARIANT_TRUE }
};
