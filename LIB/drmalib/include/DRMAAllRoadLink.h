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

// DRMAAllRoadLink.h: DRMAAllRoadLink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_)
#define AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 全道路リンクデータフォーマットクラス
 */
struct DRMAAllRoadLinkFormat
{
// 定義
	struct LinkId {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

	struct BaseLinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
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
	_DRMADec eRoadClass[1];
	_DRMADec eAdminCode[5];
	_DRMADec eLength[5];
	_DRMADec eWidth[1];
	_DRMADec eLane[1];
	_DRMADec eRegulation[1];
	_DRMADec eRegulationCond[1];
	BaseLinkId eBaseLinkId;
	
	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMADec eAttr[1];

	_DRMAHex eFiller1[3];

	_DRMAHex eContinue[1];
};

/**
 * @brief 全道路リンクデータレコードクラス
 */
class DRMAAllRoadLink : public DRMARecordT<DRMAAllRoadLinkFormat>
{
public:
// 定義
	typedef DRMAAllLinkId LinkId;
	typedef DRMABaseLinkId BaseLinkId;
	typedef LinkId Id;

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMAAllRoadLinkFormat::Point> {
		Point(const Format& cOriginal);
	};

	typedef std::vector<Point> Points;

// 構築/消滅
	DRMAAllRoadLink(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendPoints(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	LinkId eId; ///< リンク番号
	int eSequence; ///< アイテム内レコード番号

	int eManager; ///< 管理者コード
	int eRoadClass; ///< 道路種別コード
	int eAdminCode; ///< 行政区域コード
	int eLength; ///< リンク長（計算長）（m）
	int	eWidth; ///< 道路幅員区分コード
	int	eLane; ///< 車線数コード
	int	eRegulation; ///< 交通規制種別コード
	int	eRegulationCond; ///< 交通規制条件種別コード

	BaseLinkId eBaseLinkId; ///< 対応基本道路リンク番号

	int	ePointCount; ///< 補完点総数
	Points ePoints; ///< 補完点正規化座標

	int eAttr; ///< リンク内属性有無コード

	int	eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAALLROADLINK_H__9FD248E9_0187_49E0_A85A_F897AE3760D9__INCLUDED_)
