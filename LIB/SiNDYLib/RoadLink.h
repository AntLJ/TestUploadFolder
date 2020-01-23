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
 * @file RoadLink.h
 * @brief <b>道路リンククラス定義</b>\n
 * @author 
 * @version $Id$
 */

#ifndef _ROADLINK_H_
#define _ROADLINK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ErrorCode.h"
#include "Feature.h"

namespace sindy {
namespace road_link {

/**
 * @class CRoadLink
 * @brief 道路リンクフィーチャクラス
 */
class SINDYLIB_API CRoadLink : public CFeature  
{
	ROWCLASS_CONSTRUCTOR( CRoadLink, road_link )
	//@{ @name 属性取得関数

	/**
	 * @brief From_NodeID を返す
	 * @retval -1 エラー又はNULL
	 */
	long GetFromNodeID() const
	{
		return GetLongValueByFieldName( schema::road_link::kFromNodeID, -1 );
	}

	/**
	 * @brief To_NodeID を返す
	 * @retval -1 エラー又はNULL
	 */
	long GetToNodeID() const
	{
		return GetLongValueByFieldName( schema::road_link::kToNodeID, -1 );
	}

	/**
	 * @brief 道路表示種別を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::road_class::ECode GetRoadClassCode() const
	{
		return (schema::road_link::road_class::ECode)GetLongValueByFieldName( schema::road_link::kRoadClass, -1 );
	}

	/**
	 * @brief 道路経路種別を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::navi_class::ECode GetNaviClassCode() const
	{
		return (schema::road_link::navi_class::ECode)GetLongValueByFieldName( schema::road_link::kNaviClass, -1 );
	}

	/**
	 * @brief 主リンク種別を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::link_class::ECode GetMainLinkClassCode() const
	{
		return (schema::road_link::link_class::ECode)GetLongValueByFieldName( schema::road_link::kMainLinkClass, -1 );
	}

	/**
	 * @brief 副リンク種別を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::link_class::ECode GetSubLinkClassCode() const
	{
		return (schema::road_link::link_class::ECode)GetLongValueByFieldName( schema::road_link::kSubLinkClass, -1 );
	}

	/**
	 * @brief 行政コードを返す
	 * @retval -1 エラー
	 * @retval -1以外 行政コード
	 */
	long GetAdminCode() const
	{
		return GetLongValueByFieldName( schema::road_link::kAdminCode, -1 );
	}

	/**
	 * @brief 道路管理者を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::road_manager::ECode GetRoadManagerCode() const
	{
		return (schema::road_link::road_manager::ECode)GetLongValueByFieldName( schema::road_link::kRoadManager, -1 );
	}

	/**
	 * @brief 道路幅員区分を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::road_width::ECode GetRoadWidthCode() const
	{
		return (schema::road_link::road_width::ECode)GetLongValueByFieldName( schema::road_link::kRoadWidth, -1 );
	}

	/**
	 * @brief 上下線種別コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::up_down_class::ECode GetUpDownClassCode() const
	{
		return (schema::road_link::up_down_class::ECode)GetLongValueByFieldName( schema::road_link::kUpDownClass, -1 );
	}

	/**
	 * @brief バイパスフラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 バイパスフラグ
	 */
	long GetBypassFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kBypass, false );
	}

	/**
	 * @brief 自動車専用道フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 自動車専用道フラグ
	 */
	long GetCarOnlyFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kCarOnly, false );
	}

	/**
	 * @brief 離島フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 離島フラグ
	 */
	long GetIslandFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kIsland, false );
	}

	/**
	 * @brief 国道・県道番号を返す
	 * @retval -1 エラー
	 * @retval -1以外 国道・県道番号
	 */
	long GetRoadNo() const
	{
		return GetLongValueByFieldName( schema::road_link::kRoadNo, -1 );
	}

	/**
	 * @brief 路線コードを返す
	 * @retval -1 エラー
	 * @retval -1以外 路線コード
	 */
	long GetRoadCode() const
	{
		return GetLongValueByFieldName( schema::road_link::kRoadCode, -1 );
	}

	/**
	 * @brief 通行禁止種別コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::no_passage::ECode GetNoPassageCode() const
	{
		return (schema::road_link::no_passage::ECode)GetLongValueByFieldName( schema::road_link::kNoPassage, -1 );
	}

	/**
	 * @brief 通行禁止時間規制関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 通行禁止時間規制関連フラグ
	 */
	long GetNoPassageRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kNoPassageRF, false );
	}

	/**
	 * @brief 一方通行種別コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::oneway::ECode GetOnewayCode() const
	{
		return (schema::road_link::oneway::ECode)GetLongValueByFieldName( schema::road_link::kOneway, -1 );
	}

	/**
	 * @brief 一方通行時間規制関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 一方通行時間規制関連フラグ
	 */
	long GetOnewayRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kOnewayRF, false );
	}

	/**
	 * @brief DRMA旅行速度を返す
	 * @retval -1 エラー
	 * @retval -1以外 DRMA旅行速度
	 */
	long GetTripSpeed() const
	{
		return GetLongValueByFieldName( schema::road_link::kTripSpeed, -1 );
	}

	/**
	 * @brief DRMA規制速度を返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::limit_speed::ECode GetLimitSpeedCode() const
	{
		return (schema::road_link::limit_speed::ECode)GetLongValueByFieldName( schema::road_link::kLimitSpeed, -1 );
	}

	/**
	 * @brief 主従道路区分コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::kubun::ECode GetKubunCode() const
	{
		return (schema::road_link::kubun::ECode)GetLongValueByFieldName( schema::road_link::kKubun, -1 );
	}

	/**
	 * @brief リンク車線数を返す
	 * @retval -1 エラー
	 * @retval -1以外 リンク車線数
	 */
	long GetLaneCount() const
	{
		return GetLongValueByFieldName( schema::road_link::kLaneCount, -1 );
	}

	/**
	 * @brief 右側増減車線数を返す
	 * @retval -1 エラー
	 * @retval -1以外 右側増減車線数
	 */
	long GetRightChange() const
	{
		return GetLongValueByFieldName( schema::road_link::kRightChange, -1 );
	}

	/**
	 * @brief 左側増減車線数を返す
	 * @retval -1 エラー
	 * @retval -1以外 左側増減車線数
	 */
	long GetLeftChange() const
	{
		return GetLongValueByFieldName( schema::road_link::kLeftChange, -1 );
	}

	/**
	 * @brief 車線数メンテフラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 車線数メンテフラグ
	 */
	long GetLaneValidFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kLaneValid, false );
	}

	/**
	 * @brief 細街路作業年度コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::alley_mainte::ECode GetAlleyMainteCode() const
	{
		return (schema::road_link::alley_mainte::ECode)GetLongValueByFieldName( schema::road_link::kAlleyMainte, -1 );
	}

	/**
	 * @brief 重用路線関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 重用路線関連フラグ
	 */
	long GetDupLinkRelFlag() const
	{
		return GetBoolValueByFieldName( schema::road_link::kDupliLinkRF, false );
	}

	/**
	 * @brief 右左折禁止リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 右左折禁止リンク列関連フラグ
	 */
	long GetTurnRegLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kTurnRegLQRF, false );
	}

	/**
	 * @brief 方面案内リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 方面案内リンク列関連フラグ
	 */
	long GetDirGuideLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kDirGuideLQRF, false );
	}

	/**
	 * @brief 強制誘導リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 強制誘導リンク列関連フラグ
	 */
	long GetGuideLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kGuideLQRF, false );
	}

	/**
	 * @brief レーン情報リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 レーン情報リンク列関連フラグ
	 */
	long GetLaneLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kLaneLQRF, false );
	}

	/**
	 * @brief VICSリンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 VICSリンク列関連フラグ
	 */
	long GetVICSLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kVICSLQRF, false );
	}

	/**
	 * @brief 抜け道裏道リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 抜け道裏道リンク列関連フラグ
	 */
	long GetBywayLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kBywayLQRF, false );
	}

	/**
	 * @brief アクセス道乗り継ぎ道リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 アクセス道乗り継ぎ道リンク列関連フラグ
	 */
	long GetRouteLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kRouteLQRF, false );
	}

	/**
	 * @brief Uターン可能リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 Uターン可能リンク列関連フラグ
	 */
	long GetUTurnLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kUturnLQRF, false );
	}

	/**
	 * @brief インターナビリンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 インターナビリンク列関連フラグ
	 */
	long GetInterNaviLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kInternaviLQRF, false );
	}

	/**
	 * @brief iPCVICSリンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 iPCVICSリンク列関連フラグ
	 */
	long GetiPCVICSLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kIpcVicsLQRF, false );
	}

	/**
	 * @brief 進入方向別交差点リンク列関連フラグを返す
	 * @retval -1 エラー
	 * @retval -1以外 進入別交差点リンク列関連フラグ
	 */
	long GetIntersectionLQRelFlag() const
	{
		return GetLongValueByFieldName( schema::road_link::kIntersectionLQRF, false );
	}

	/**
	 * @brief 駐車場種別コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::parking_class::ECode GetParkingClassCode() const
	{
		return (schema::road_link::parking_class::ECode)GetLongValueByFieldName( schema::road_link::kParkingClass, -1 );
	}

	/**
	 * @brief 駐車場リンク種別コードを返す
	 * @retval -1 エラー又はNULL
	 */
	schema::road_link::parking_link_class::ECode GetParkingLinkClassCode() const
	{
		return (schema::road_link::parking_link_class::ECode)GetLongValueByFieldName( schema::road_link::kParkingLinkClass, -1 );
	}

	/**
	 * @brief 駐車場階層を返す
	 * @retval -1 エラー
	 * @retval -1以外 駐車場階層
	 */
	long GetFloorLevel() const
	{
		return GetLongValueByFieldName( schema::road_link::kFloorLevel, -1 );
	}

	/**
	 * @brief 駐車場階層間レベルを返す
	 * @retval -1 エラー
	 * @retval -1以外 駐車場階層間レベル
	 */
	long GetMiddleFloorLevel() const
	{
		return GetLongValueByFieldName( schema::road_link::kMiddleFloorLevel, -1 );
	}
	
	//@}

	//@{ @name 属性設定関数

	/**
	 * @brief From_NodeID を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetFromNodeID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kFromNodeID, lValue );
	}

	/**
	 * @brief 道路表示種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetToNodeID(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kToNodeID, lValue );
	}

	/**
	 * @brief 道路表示種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadClassCode(schema::road_link::road_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadClass, emCode );
	}

	/**
	 * @brief 道路経路種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNaviClassCode(schema::road_link::navi_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kNaviClass, emCode );
	}

	/**
	 * @brief 主リンク種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetMainLinkClassCode(schema::road_link::link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kMainLinkClass, emCode );
	}

	/**
	 * @brief 副リンク種別を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetSubLinkClassCode(schema::road_link::link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kSubLinkClass, emCode );
	}

	/**
	 * @brief 行政コードを設定する
	 * @brief NULL 許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetAdminCode(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kAdminCode, lValue );
	}

	/**
	 * @brief 道路管理者を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadManagerCode(schema::road_link::road_manager::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadManager, emCode );
	}

	/**
	 * @brief 道路幅員区分を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadWidthCode(schema::road_link::road_width::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadWidth, emCode );
	}

	/**
	 * @brief 上下線種別コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetUpDownClassCode(schema::road_link::up_down_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kUpDownClass, emCode );
	}

	/**
	 * @brief バイパスフラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetBypassFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kBypass, bFlag );
	}

	/**
	 * @brief 自動車専用道フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetCarOnlyFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kCarOnly, bFlag );
	}

	/**
	 * @brief 離島フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetIslandFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIsland, bFlag );
	}

	/**
	 * @brief 国道・県道番号を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadNo(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadNo, lValue );
	}

	/**
	 * @brief 路線コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRoadCode(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRoadCode, lValue );
	}

	/**
	 * @brief 通行禁止種別コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNoPassageCode(schema::road_link::no_passage::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kNoPassage, emCode );
	}

	/**
	 * @brief 通行禁止時間規制関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetNoPassageRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kNoPassageRF, bFlag );
	}

	/**
	 * @brief 一方通行種別コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetOnewayCode(schema::road_link::oneway::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kOneway, emCode );
	}

	/**
	 * @brief 一方通行時間規制関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetOnewayRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kOnewayRF, bFlag );
	}

	/**
	 * @brief DRMA旅行速度を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetTripSpeed(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kTripSpeed, lValue );
	}

	/**
	 * @brief DRMA規制速度を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetLimitSpeedCode(schema::road_link::limit_speed::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kLimitSpeed, emCode );
	}

	/**
	 * @brief 主従道路区分コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetKubunCode(schema::road_link::kubun::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kKubun, emCode );
	}

	/**
	 * @brief リンク車線数を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneCount(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kLaneCount, lValue );
	}

	/**
	 * @brief 右側増減車線数を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRightChange(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kRightChange, lValue );
	}

	/**
	 * @brief 左側増減車線数を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetLeftChange(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kLeftChange, lValue );
	}

	/**
	 * @brief 車線数メンテフラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneValidFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kLaneValid, bFlag );
	}

	/**
	 * @brief 細街路作業年度コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetAlleyMainteCode(schema::road_link::alley_mainte::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kAlleyMainte, emCode );
	}

	/**
	 * @brief 重用路線関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetDupLinkRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kDupliLinkRF, bFlag );
	}

	/**
	 * @brief 右左折禁止リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetTurnRegLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kTurnRegLQRF, bFlag );
	}

	/**
	 * @brief 方面案内リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetDirGuideLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kDirGuideLQRF, bFlag );
	}

	/**
	 * @brief 強制誘導リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetGuideLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kGuideLQRF, bFlag );
	}

	/**
	 * @brief レーン情報リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetLaneLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kLaneLQRF, bFlag );
	}

	/**
	 * @brief VICSリンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetVICSLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kVICSLQRF, bFlag );
	}

	/**
	 * @brief 抜け道裏道リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetBywayLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kBywayLQRF, bFlag );
	}

	/**
	 * @brief アクセス道乗り継ぎ道リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetRouteLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kRouteLQRF, bFlag );
	}

	/**
	 * @brief Uターン可能リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetUTurnLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kUturnLQRF, bFlag );
	}

	/**
	 * @brief インターナビリンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetInterNaviLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kInternaviLQRF, bFlag );
	}

	/**
	 * @brief iPCVICSリンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetiPCVICSLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIpcVicsLQRF, bFlag );
	}

	/**
	 * @brief 進入方向別交差点リンク列関連フラグを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetIntersectionLQRelFlag(bool bFlag)
	{
		return SetBoolValueByFieldName( schema::road_link::kIntersectionLQRF, bFlag );
	}

	/**
	 * @brief 駐車場種別コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingClassCode(schema::road_link::parking_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kParkingClass, emCode );
	}

	/**
	 * @brief 駐車場リンク種別コードを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetParkingLinkClassCode(schema::road_link::parking_link_class::ECode emCode)
	{
		return SetLongValueByFieldName( schema::road_link::kParkingLinkClass, emCode );
	}

	/**
	 * @brief 駐車場階層を設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetFloorLevel(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kFloorLevel, lValue );
	}

	/**
	 * @brief 駐車場階層間レベルを設定する
	 * @brief NULL 不許可
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外の sindyErrCode
	 */
	errorcode::sindyErrCode SetMiddleFloorLevel(long lValue)
	{
		return SetLongValueByFieldName( schema::road_link::kMiddleFloorLevel, lValue );
	}
	
	//@}

	//@{ @name チェックコード
	/**
	 * @brief <b>整合性をチェックする</b>\n
	 * 道路リンクを編集した後に必ずチェックを行ってください。
	 * このメソッドでチェックされる項目は以下のとおりです。
	 * @li 
	 * @li 
	 * @li 
	 * @param cRule			[in]	整合性チェック用ルール
	 * @param cErrs			[out]	エラー情報格納用コンテナ
	 * @return sindyErrCode
	 */
	virtual errorcode::sindyErrCode CheckLogic( const CLogicRule& cRule, CErrorObjects& cErrs ) const;

	/*
	 * @brief 両端に道路ノードが存在しているか
	 * @retval 成功　sindyErr_NoErr
	 * @retval 失敗　それ以外のエラー
	 */
	errorcode::sindyErrCode CheckNodeID() const;
	//@}
	/**
	 * @brief <b>FROM側の端点を取得する</b>\n
	 * return CPoint
	 */
	CPoint GetFromPoint() const;

	/**
	 * @brief <b>TO側の端点を取得する</b>\n
	 * return CPoint
	 */
	CPoint GetToPoint() const;

	bool IsPECLink() const;
};

} // road_link

} // sindy

#endif // _ROADLINK_H_
