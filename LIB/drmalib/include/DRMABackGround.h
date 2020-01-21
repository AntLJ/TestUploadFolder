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

// DRMABackGround.h: DRMABackGround クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_)
#define AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 背景系データフォーマットクラス
 */
struct DRMABackGroundFormat
{
// 定義
	struct Point {
		_DRMADec eClass[1];
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// フォーマット
	_DRMADec eRecordId[2];
	
	_DRMAHex eFiller1[7];

	_DRMADec eId[4];
	_DRMADec eSequence[2];
	_DRMADec eClass[1];

	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMAHex eFiller2[5];

	_DRMAHex eContinue[1];
};

/**
 * @brief 背景系データレコードクラス
 */
class DRMABackGround : public DRMARecordT<DRMABackGroundFormat>
{
public:
// 定義
	typedef int Id;

	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMABackGroundFormat::Point>
	{
		Point(const Format& cOriginal);

		int eClass;
	};

	typedef std::vector<Point> Points;

// 構築/消滅
	DRMABackGround(const Format& cOrignalRecord);

// 追加
	void append(const Format& cOrignalRecord);

protected:
// 追加
	void appendPoints(const Format& cOrignalRecord);

public:
// プロパティ
//	int eRecordId;
	
	int eId;
	int eSequence;
	int eClass;

	int ePointCount;
	Points ePoints;

	int eContinue;
};


#endif // !defined(AFX_DRMABACKGROUND_H__6252B827_B34A_4CF3_96E8_7D4BABB92A9C__INCLUDED_)
