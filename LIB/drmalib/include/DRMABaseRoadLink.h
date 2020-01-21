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

// DRMABaseRoadLink.h: DRMABaseRoadLink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_)
#define AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 基本道路リンクフォーマットクラス
 */
struct DRMABaseRoadLinkFormat
{
// 定義
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Road {
		_DRMADec eRoadClass[1];
		_DRMADec eRoadNo[4];
		_DRMADec eKubun[1];
	};

	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// フォーマット
	_DRMADec eRecordId[2];
	LinkId eId;
	_DRMADec eSequence[2];

	_DRMADec eManager[1];
	Road eMain;

	_DRMADec eDuplicateCount[2];
	Road eDuplicates[3];

	_DRMADec eAdminCode[5];
	_DRMADec eLength[5];
	_DRMADec eClass[1];
	_DRMADec eBeacon[1];

	_DRMADec eCarOnly[1];
	_DRMADec eToll[1];
	_DRMADec ePassage[1];
	_DRMADec eWeatherRegulation[1];
	_DRMADec eCarWeight[1];
	_DRMADec eCarHeight[1];
	_DRMADec eCarWidth[1];
	
	_DRMADec eWidth[1];
	_DRMADec eLane[1];
	_DRMADec eRoadWidth[3];
	_DRMADec eMinRoadWidth[3];
	_DRMADec eMedianWidth[3];
	_DRMADec eMedianLength[5];
	
	_DRMADec eTrafic[4];
	_DRMADec eTripSpeed[4];

	_DRMADec eRegulation[1];
	_DRMADec eRegulationCond[1];
	_DRMADec eNoPassing[1];
	_DRMADec eSpeedRegulation[1];
	
	_DRMADec eSpetialVehicle[1];
	_DRMADec eAttr[1];
	_DRMADec ePointCount[3];
	Point ePoints[16];

	_DRMAHex eFiller1[2];

	_DRMADec eShapeSource[1];
	_DRMADec eDesignate[1];

	_DRMAHex eContinue[1];
};

/**
 * @brief 基本道路リンクレコードクラス
 */
class DRMABaseRoadLink : public DRMARecordT<DRMABaseRoadLinkFormat>
{
public:
// 定義
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct Road : public DRMAItemConverterTemplate<DRMABaseRoadLinkFormat::Road> {
		Road();
		Road(const Format& cOriginal);

		int eRoadClass; ///< 道路種別コード
		int eRoadNo; ///< 路線番号
		int eKubun; ///< 主・従道路区分コード
	};

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMABaseRoadLinkFormat::Point> {
		Point(const Format& cOriginal);
	};

	typedef std::vector<Road> Duplicates;
	typedef std::vector<Point> Points;

// 構築/消滅
	DRMABaseRoadLink(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendDuplicates(const Format& cOriginalRecord);
	void appendPoints(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	LinkId eId; ///< リンク番号
	int eSequence; ///< アイテム内レコード番号

	int eManager; ///< 管理者コード
	Road eMain; ///< 主路線

	int eDuplicateCount; ///< 重用路線総数
	Duplicates eDuplicates; ///< 重用路線

	int eAdminCode; ///< 行政区域コード
	int eLength; ///< リンク長（計算長）（m）
	int eClass; ///< リンク種別コード
	int eBeacon; ///< ビーコン有無コード

	int	eCarOnly; ///< 自動車専用道路コード
	int	eToll; ///< 有料道路コード
	int	ePassage; ///< リンク通行可、不可コード
	int	eWeatherRegulation; ///< 異常気象時通行規制区間種別コード
	int	eCarWeight; ///< 車両重量制限有無コード
	int	eCarHeight; ///< 車両高さ制限有無コード
	int	eCarWidth; ///< 車両幅制限有無コード
	
	int	eWidth; ///< 道路幅員区分コード
	int	eLane; ///< 車線数コード
	int	eRoadWidth; ///< 車道幅員（0.1m）
	int	eMinRoadWidth; ///< 最小車道部幅員（0.1m）
	int	eMedianWidth; ///< 中央帯幅員（0.1m）
	int	eMedianLength; ///< 中央帯設置延長（m）
	
	int	eTrafic; ///< 12時間交通量（100台）
	int	eTripSpeed; ///< 旅行速度（ピーク時）
	
	int	eRegulation; ///< 交通規制種別コード
	int	eRegulationCond; ///< 交通規制条件種別コード
	int	eNoPassing; ///< 追越禁止コード
	int	eSpeedRegulation; ///< 規制速度コード
	
	int	eSpetialVehicle; ///< 特車通行システム対象コード
	int	eAttr; ///< リンク内属性有無コード
	int	ePointCount; ///< 補完点総数
	Points ePoints; ///< 補完点正規化座標

	int	eShapeSource; ///< 形状データ取得資料コード
	int	eDesignate; ///< 一般国道・指定区間該当コード

	int	eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADLINK_H__642DB6D1_8788_419E_B922_AF64E94768FD__INCLUDED_)
