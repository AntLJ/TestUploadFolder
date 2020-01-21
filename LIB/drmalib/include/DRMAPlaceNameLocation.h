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

// DRMAPlaceNameLocation.h: DRMAPlaceNameLocation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_)
#define AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief 地名等表示位置データフォーマットクラス
 */
struct DRMAPlaceNameLocationFormat
{
// 定義
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ShortName {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[10];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[20];
	};

	struct LongName {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[30];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[60];
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMAHex eFiller1[1];

	_DRMADec eId[4];
	
	_DRMAHex eFiller2[4];

	_DRMADec eClass[2];
	_DRMADec eAdminCode[5];

	_DRMAHex eFiller3[1];

	_DRMADec eDispLevel[1];
	_DRMADec eAngle[3];

	Point ePoint;
	ShortName eShortName;
	LongName eLongName;

	_DRMAHex eFiller[55];
};

/**
 * @brief 地名等表示位置データレコードクラス
 */
class DRMAPlaceNameLocation : public DRMARecordT<DRMAPlaceNameLocationFormat>
{
public:
// 定義
	typedef DRMAPoint Point;
	typedef DRMAShortName ShortName;
	typedef DRMALongName LongName;
	typedef int Id;

// 構築/消滅
	DRMAPlaceNameLocation(const Format& cOriginalRecord);

// プロパティ
//	int eRecordId; ///< レコードID

	int eId; ///< アイテム番号
	
	int eClass; ///< 地名等種別コード
	int eAdminCode; ///< 行政区域コード

	int eDispLevel; ///< 表示レベル参考コード
	int eAngle; ///< 地名等表示参考角度

	Point ePoint; ///< 地名等表示参考位置正規化座標
	ShortName eShortName; ///< 表示名称
	LongName eLongName; ///< 正式名称

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAPLACENAMELOCATION_H__0FE68254_FE7F_4395_BE17_9DBE31C0BB9B__INCLUDED_)
