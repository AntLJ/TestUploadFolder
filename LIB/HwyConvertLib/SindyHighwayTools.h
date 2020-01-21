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
 *	各種ユーティリティ
 *	@author	Fusatoshi Abe
 *	@date	2003/10/30		新規作成
 *  @file HwyConvertLib開発に伴い、SindyHighwayCheckのソースがSindyHighwayConvertでも使われていたのを解消するため、
 *        その共有部分をSindyHighwayCheckからHwyConvertLibに移管した。
 */
#ifndef	___SINDY_HIGHWAY_TOOLS_H___
#define	___SINDY_HIGHWAY_TOOLS_H___

#include "SindyHighwayInfo.h"

namespace sindy_highway_tools
{
	const long CONNECTION_LINK_ADDITIONAL_ROADCODE = 200000;	///< 「本線接続部リンク」の路線コードは、本線リンクの路線コードより200000大きい
	const long FREEZONE_ADDITIONAL_ROADCODE = 400000;			///< 「無料区間」のリンクの路線コードは、本線リンクの路線コードより400000大きい
} // namespace sindy_highway_tools

// TODO: 関数コメントをちゃんと全部書こう。
/**
 * @brief 対象リンクがHWYの本線リンクであるか否かを判定する
 * @note	Main_LinkClass_Cが「本線(上下分離)リンク」「本線(上下非分離)リンク」のいずれかで、
 *			Road_Codeが「0または400000以上600000未満」ではないリンクを「HWYの本線リンク」と見なす
 * @param[in]	ipLink	対象リンク
 * @return	判定結果
 **/
bool isMainLineLink(IFeature* ipLink);

/**
 *	@brief	本線追跡チェックの対象とするか否かの判定
 *	@param	ipLink	[in]	判定対象リンク
 *	@return	与えられたリンクが本線追跡チェックの対象であるか否か
 **/
bool isMainLineTraceTarget(IFeature* ipLink);

/**
 * @brief JCTの渡り線リンクであるか否かを判定する
 * @param[in]	ipLink	対象リンク
 * @return	判定結果
 **/
bool isJctLink(IFeature* ipLink);

// MAIN_LINKCLASS_Cが「連結路(本線間の渡り線)」であるかチェック
bool isNewJctLink(IFeature* ipLink);

// MAIN_LINKCLASS_Cが「連結路(ランプ)リンク」であるかチェック
bool isRampLink(IFeature* ipLink);

// MAIN_LINKCLASS_Cが「SA等側線リンク」であるかチェック
bool isSaLink(IFeature* ipLink);

// MAIN_LINKCLASS_Cが「本線と同一路線の側道リンク」であるかチェック
bool isSideLineLink(IFeature* ipLink);

// MAIN_LINKCLASS_Cが「交差点内リンク」であるかチェック
bool isCrossingLink(IFeature* ipLink);

// ROADCLASS_Cが有料道路であるかチェック
bool isTollRoad(IFeature* ipLink);

// 0でないROAD_CODEを持っているかチェック
bool isRoadCode(IFeature* ipLink);

// ROAD_CODEがHWY本線同士の接続部リンクであるかチェック
bool isJointZoneLink(IFeature* ipLink);

// ROAD_CODEが無料区間の本線リンクであるかチェック
bool isTollFreeZoneLink(IFeature* ipLink);

/**
 * @brief 対象リンクが一般道であるか否かを判定する（[Bug7176]で追加）
 * @param[in]	cpLink	対象リンク
 * @return	判定結果
 */
bool isGeneralRoad( const IFeaturePtr cpLink );

/**
 *	@brief	路線コードを比較して、同じ路線であるか判定する
 *	@note	本線の接続部リンクと無料区間は、通常の本線と同じ路線と見なす
 */
bool isSameLine(long aRoadCode1, long aRoadCode2);

/**
 *	@brief	前後の分岐数が0かどうかを判定する
 *	@retval	true	分岐なし
 *	@retval	false	1つ以上の分岐あり
 */
bool isZeroBranch(_IRowPtr ipHighwayTxt);

/**
 * @brief 対象リンクの対象ノードとは反対側のノードを取得する（[Bug7176]で追加）
 * @param[in]	cpLink	対象リンク
 * @param[in]	cNodeID	対象ノード
 * @param[in]	cpRoadInfo	道路情報
 * @return	取得したノード
 */
IFeaturePtr getOppositeNode( IFeaturePtr cpLink, long cNodeID, SindyRoadInfo* cpRoadInfo );

#endif	//___SINDY_HIGHWAY_TOOLS_H___
