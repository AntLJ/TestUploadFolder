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

#pragma once

#include "TimeTrace.h"

// グローバル変数宣言
extern CTimeTrace g_timetrace; //<! 時間計測用

using namespace std;
using namespace sindy;
using namespace sindy::schema;

/**
 * @brief   形状一致確認
 * @param   ipGeom1  [in]  比較形状1
 * @param   ipGeom2  [in]  比較形状2
 * @return  true   形状一致
 *          false  形状不一致
 */
bool IsSameGeom( const IGeometryPtr& ipGeom1, const IGeometryPtr& ipGeom2 );

CString GetTableName( const ITablePtr& ipTable );

/**
 * @brief AGOPSテーブルか
 * @note AGOPSとは、リンク内属性(segment_Attr)、傾斜(Grad)、一方通行(Oneway)、通行禁止(noPass)、簡易傾斜(Simple_Grad)の意
 * @note 平たく言えば1リンクに対応するテーブル群のこと。
 * @note 一通/通禁車種別規制(Vehicle_reg)も追加してAGOPSVとかも考えられるが、意味は通るのでAGOPSのままとする
 * @param strTableName [in] テーブル名称
 * @retval true 与えられたテーブル名はAGOPSテーブルに該当する
 * @retval false 与えられたテーブル名はAGOPSテーブルに該当しない
 */
bool IsAGOPSTable( const CString strTableName );

/**
 * @brief 車種タイプのテーブルか
 * @param strTableName [in] テーブル名称
 * @retval true 与えられたテーブル名は車種タイプのテーブルに該当する
 * @retval false 与えられたテーブル名は車種タイプのテーブルに該当しない
 */
bool IsVehicleTypeTable( const CString strTableName );

/**
 * @brief リンク列(INF)のテーブルか
 * @param strTableName [in] テーブル名称
 * @retval true 与えられたテーブル名はリンク列情報テーブル(INF)に該当する
 * @retval false 与えられたテーブル名はリンク列情報テーブル(INF)に該当しない
 */
bool IsInfTable( const CString strTableName );

/**
 * @brief リンク列(LQ)のテーブルか
 * @param strTableName [in] テーブル名称
 * @retval true 与えられたテーブル名はリンク列(LQ)のテーブルに該当する
 * @retval false 与えられたテーブル名はリンク列(LQ)のテーブルに該当しない
 */
bool IsLqTable( const CString strTableName );

CString getNowTime();


// TODO:編集保存失敗した時、エラーコードでリコンサイルするかの処理
