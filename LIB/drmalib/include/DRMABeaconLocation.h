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

// DRMABeaconLocation.h: DRMABeaconLocation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_)
#define AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief ビーコン位置データフォーマットクラス
 */
struct DRMABeaconLocationFormat
{
// 定義
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[30];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[60];
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMADec eItemNo[4];
	_DRMADec eId[8];
	_DRMADec eClass[2];
	_DRMADec eApplication[2];

	Point ePoint;

	_DRMAHex eFiller1[2];

	LinkId eLinkId;
	_DRMADec eLinkDir[1];
	Name eName;

	_DRMAHex eFiller2[93];
};

/**
 * @brief ビーコン位置データレコードクラス
 */
class DRMABeaconLocation : public DRMARecordT<DRMABeaconLocationFormat>
{
public:
// 定義
	typedef DRMAPoint Point;
	typedef DRMABaseLinkId LinkId;
	typedef DRMALongName Name;
	typedef int Id;

// 構築/消滅
	DRMABeaconLocation(const Format& cOriginalRecord);

// プロパティ
//	int eRecordId; ///< レコードID

	int eItemNo; ///< ビーコンアイテム番号
	int eId; ///< 送信ビーコン番号
	int eClass; ///< ビーコン種別コード
	int eApplication; ///< ビーコン用途別コード

	Point ePoint; ///< 正規化座標

	LinkId eLinkId; ///< 設置リンク番号
	int eLinkDir; ///< 対応リンク方向コード
	Name eName; ///< ビーコン設置地点名

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABEACONLOCATION_H__FBCDA139_8762_4D38_80D6_32EF0C693611__INCLUDED_)
