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
 * @file AnnoGlobals.h
 * @brief 注記系外部関数
 * @author 地図ＤＢ制作部開発グループ 古川貴宏
 * 
 * ここに記述されている関数はExport対象外です
 *
 * @version $Id$
 */
#ifndef _ANNOGLOBALS_H_
#define _ANNOGLOBALS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AnnotationCommunity.h"
#include "RuleCommunity.h"
#include "ErrorCode.h"
#include "TableType.h"

namespace sindy {
/**
 * @brief <b>注記種別から注記種別表示名を取得する</b>
 * 注記種別表示名はannotation_code_labelネームスペース内に
 * 定義されています。
 * @param emCode	[in]	注記種別コード
 * @return 注記種別表示名
 */
LPCTSTR SINDYLIB_API AheAnnoCode2CodeName( schema::annotation::annotation_code::ECode emCode );

/**
 * @brief <b>テーブルタイプからスケールコードに変換する</b>
 * @param emCode [in] テーブルタイプ（注記系のみ）
 * @retval sindyAnnotationScale スケールコード
 * @retval -1 テーブルタイプが不正
 */
sindyAnnotationScale SINDYLIB_API AheTableType2AnnotationScale( sindyTableType::ECode emCode );

/**
 * @brief <b>テーブルタイプから注記グループに変換する</b>
 * @param emCode [in] テーブルタイプ（注記系のみ）
 * @retval sindyAnnotationGroup 注記グループ
 * @retval -1 テーブルタイプが不正
 */
sindyAnnotationGroup SINDYLIB_API AheTableType2AnnotationGroup( sindyTableType::ECode emCode );

/**
 * @brief <b>表示位置テーブルタイプから真位置テーブルタイプを取得する</b>
 * @param emDispTypeTableType [in] 表示位置又は真位置テーブルタイプ
 * @return 真位置テーブルタイプ
 */
sindyTableType::ECode SINDYLIB_API AheTableType2AnnoPointTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>表示位置テーブルタイプからスケールタイプ文字列に変換する</b>\n
 * @note この関数はCStringを返すため、DLLでは非公開です
 * @param emDispTableType		[in]			表示位置テーブルタイプ
 * @return スケールタイプ文字列（"S1"とか"S2"とか。都市地図の場合には空文字になります）
 */
CString AheTableType2ScaleTypeString( sindyTableType::ECode emDispTableType );

/**
 * @brief <b>グループを考慮してメッシュコードを矩形に変換する</b>\n
 * @warning グループとメッシュコードの桁数の整合が取れない場合、
 * メッシュコードの桁数を優先します（というか、グループ指定はミドルしか対応していません）
 * @param lMeshCode [in] メッシュコード
 * @param emGroup   [in] 注記グループ
 * @param ipSpRef   [in] 空間参照
 * @return メッシュ矩形
 */
IEnvelopePtr SINDYLIB_API AheMeshCode2Envelope( long lMeshCode, sindyAnnotationGroup emGroup, ISpatialReference* ipSpRef );

/**
 * @brief <b>渡されたテーブルタイプが真位置テーブルタイプかどうかを検査する</b>
 * @param emDispTypeTableType [in] テーブルタイプ
 * @retval true 真位置テーブルタイプである
 * @retval false 真位置テーブルタイプではない
 */
bool SINDYLIB_API AheIsAnnoPointTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>渡されたテーブルタイプが表示位置テーブルタイプかどうかを検査する</b>
 * @param emDispTypeTableType [in] テーブルタイプ
 * @retval true 表示位置テーブルタイプである
 * @retval false 表示位置テーブルタイプではない
 */
bool SINDYLIB_API AheIsAnnoLineTableType( sindyTableType::ECode emTableType );

/**
 * @brief <b>注記文字列をチェック・修復する</b>\n
 *
 * bFixがTRUEの場合、正常に修復されると返値は常にsindyErr_NoErrです。\n
 *
 * この関数では、以下のチェックを行います\n
 *
 * [rule_check_1_2_1_1E]  注記文字列の正当性評価（ただし、名称3かどうかのチェックはしない）\n
 * [rule_check_1_2_1_2E]  不要\n
 * [rule_check_1_2_1_3E]  不要\n
 * [rule_check_1_2_1_4E]  ？（山岳標高値の整数有効桁数は3以下？んなわけない）\n
 * [rule_check_1_2_1_5E]  インターチェンジの場合は文字列最後が「ＩＣ」、且つ文字列数が4以上\n
 * [rule_check_1_2_1_6E]  都市地図注記以外の場合、ジャンクションは文字列最後が「ＪＣ」、且つ文字列数が4以上\n
 * [rule_check_1_2_1_7E]  フェリーターミナルの場合は文字列最後が「ＦＴ」、且つ文字列数が4以上\n
 * [rule_check_1_2_1_8E]  上記3つと重複するので不要\n
 * [rule_check_1_2_1_9E]  全角0は先頭に来てはいけない\n
 * [rule_check_1_2_1_10E] 不要\n
 * [rule_check_1_2_1_11E] 都市地図注記の場合、ジャンクションは文字列最後が「ＪＣＴ」でなければならない\n
 * [rule_check_1_2_1_12E] 主要地方道番号の場合は且つ1～151まで（表示タイプのチェックは他でやる）\n
 * [rule_check_1_2_1_13E] 国道番号の場合は1～58、101～108、112～507\n
 * [rule_check_1_2_1_14E] 全角長音記号は全角ひらがな・全角カタカナの後限定（ハイフンのチェックはどうしたらよいかわからない…）\n
 * [rule_check_1_2_1_15E] 山岳標高値の場合は小数点第一位までは存在する必要がある
 *
 * @bug 修復する機能はまだ実装されていません
 *
 * @param lpcszString	[in]			注記文字列
 * @param lStringLen	[in]			注記文字数（フィールド値）
 * @param emAnnoCode	[in]			注記種別
 * @param bIsCity		[in,optional]	都市地図注記かどうか（DEFAULT:true）
 * @param bIsString3	[in,optional]	注記文字列3かどうか（DEFAULT:false）
 * @param bFix			[in,optional]	修復する場合はTRUE（DEFAULT:false）
 * @param pstrFixString	[out,optional]	修復された文字列が入る（DEFAULT:NULL）
 *
 * @return sindyErrCode
 */
errorcode::sindyErrCode SINDYLIB_API AheCheckOrFixAnnotationString( LPCTSTR lpcszString, long lStringLen, schema::annotation::annotation_code::ECode emAnnoCode, bool bIsCity = true, bool bIsString3 = false, bool bFix = false, CString* pstrFixString = NULL );

/**
 * @brief <b>表示位置レイヤタイプから1ptあたりの距離を算出する</b>\n
 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
 * @param bCalcX		[in]	X座標値を取得するならtrue、Y座標値を取得するならfalse
 * @return 1ptあたりの距離（単位：緯度経度）
 */
double SINDYLIB_API AheCalcDispLineType21PTLen( sindyTableType::ECode emTableType, bool bCalcX );

/**
 * @brief <b>表示位置レイヤタイプから1pxあたりの距離を算出する</b>\n
 * @param emTableType	[in]	注記表示位置レイヤテーブルタイプ
 * @param bCalcX		[in]	X座標値を取得するならtrue、Y座標値を取得するならfalse
 * @return 1pxあたりの距離（単位：緯度経度）
 */
double SINDYLIB_API AheCalcDispLineType21PXLen( sindyTableType::ECode emTableType, bool bCalcX );

/**
 * @brief <b>個々の文字列矩形から全体のバウンディングボックスを計算する</b>\n
 * pStringsBoundsに含まれる個々の矩形はそれぞれ5点で形成される必要があります。
 * 例えば、lStringCountが5の場合は5x5で25点で形成されます。
 * また、それぞれは閉じたポリゴンでなければなりません。
 * @param pStringsBounds		[in]	個々の文字列矩形
 * @param lStringCount			[in]	文字数
 * @param emLineType			[in]	ラインタイプ
 * @param lSecondLineStartIndex	[in]	二行注記の開始インデックス番号
 * @param ipSpRef				[in]	空間参照
 * @return IGeometryPtr
 */
IGeometryPtr SINDYLIB_API AheCalcStringBounds( const WKSPoint* pStringsBounds, long lStringCount, sindyAnnotationLineType emLineType, long lSecondLineStartIndex, ISpatialReference* ipSpRef );

/**
 * @brief <b>表示タイプとスケール表示ルールの論理性をチェックする</b>\n
 * 表示タイプと表示ルールが、論理的に正しい組み合わせかどうかをチェックします。
 * @param emDispType			[in]	表示タイプ
 * @param emScaleDispRule		[in]	スケール表示ルール
 * @retval true 論理的に正しい組み合わせ
 * @retval false 論理的に間違った組み合わせ
 */
bool SINDYLIB_API AheCheckDispTypeCodeAndScaleDispRule( schema::annotation::disp_type::ECode emDispType, schema::annotation_class::scaledisp_rule::ECode emScaleDispRule );

/**
 * @brief <b>表示タイプと上位の表示タイプの論理性をチェックする</b>\n
 * 現在の表示タイプとそれの上位に当たる表示タイプの組み合わせをチェックします。
 * このチェックは都市地図レイヤでは不要です。
 * @see AheCheckDispTypeNakanuke
 * @param emDispType			[in]	チェック対象表示タイプ
 * @param emUpperDispType		[in]	上位スケールの表示タイプ（emDispType==S3ならemUpperDispType==S4となる）
 * @retval true 論理的に正しい組み合わせ
 * @retval false 論理的に間違った組み合わせ
 */
bool SINDYLIB_API AheCheckDispTypeAndUpperDispType( schema::annotation::disp_type::ECode emDispType, schema::annotation::disp_type::ECode emUpperDispType );

/**
 * @brief <b>各スケールの表示タイプの論理性をチェックする</b>\n
 * 俗に言う「中抜け」のチェックです。
 * このチェックは、中抜け可の種別（@see CAnnotationClass::GetExistScale）及び都市地図レイヤでは不要です。
 * @note pRetScaleは必ず4つ以上のbool配列で指定してください。
 * @see AheCheckDispTypeCodeAndDispRule
 * @param emDispTypeS1			[in]			S1表示タイプ
 * @param emDispTypeS2			[in]			S2表示タイプ
 * @param emDispTypeS3			[in]			S3表示タイプ
 * @param emDispTypeS4			[in]			S4表示タイプ
 * @param pRetScale				[out,optional]	S1～S4のどれがエラーかを示すフラグ。例えば、S1がエラーならpRetScale[0]==falseとなる（デフォルト：NULL）
 * @retval true 中抜けしていない
 * @retval false 中抜けしている
 */
bool SINDYLIB_API AheCheckDispType( schema::annotation::disp_type::ECode emDispTypeS1, schema::annotation::disp_type::ECode emDispTypeS2,
								   schema::annotation::disp_type::ECode emDispTypeS3, schema::annotation::disp_type::ECode  emDispTypeS4, bool* pRetScale = NULL );

#ifdef SINDYLIB_CALC_TIME
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLineTime;		//!< 一行注記のバウンディングボックスの作成にかかる総時間
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLine1Time;		//!< 一行注記のバウンディングボックスの作成にかかる総時間（前半）
extern SINDYLIB_API double g_dAheCalcStringBounds_OneLine2Time;		//!< 一行注記のバウンディングボックスの作成にかかる総時間（後半）
extern SINDYLIB_API double g_dAheCalcStringBounds_DoubleLineTime;	//!< 二行注記のバウンディングボックスの作成にかかる総時間
extern SINDYLIB_API double g_dAheCalcStringBounds_LinerLineTime;	//!< 線状注記のバウンディングボックスの作成にかかる総時間

void SINDYLIB_API AheCalcStringBoundsTraceTime();
void SINDYLIB_API AheCalcStringBoundsClearTime();
#endif // ifdef SINDYLIB_CALC_TIME

} // namespace sindy

#endif // _ANNOGLOBALS_H_
