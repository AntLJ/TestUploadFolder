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

// DRMAFacilShape.h: DRMAFacilShape クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_)
#define AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 施設等形状フォーマットクラス
 */
struct DRMAFacilShapeFormat
{
// 定義
	struct Point {
		_DRMADec eClass[1];
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMADec eMeshCode[6];
	_DRMADec eId[5];

	_DRMADec eSequence[2];
	_DRMADec eBlockNo[1];

	_DRMADec ePointCount[3];
	Point ePoints[21];

	_DRMAHex eFiller2[5];

	_DRMAHex eContinue[1];
};

/**
 * @brief 施設等形状データレコードクラス
 */
class DRMAFacilShape : public DRMARecordT<DRMAFacilShapeFormat>
{
public:
// 定義
	typedef int Id;
	struct Point : public DRMAPoint, public DRMAItemConverterTemplate<DRMAFacilShapeFormat::Point>
	{
		Point(const Format& cOriginal);

		int eClass;
	};

	typedef std::vector<Point> Points;

// 構築/消滅
	DRMAFacilShape(const Format& cOrignalRecord);

// 追加
	void append(const Format& cOrignalRecord);

protected:
// 追加
	void appendPoints(const Format& cOrignalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	
	int eMeshCode; ///< ２次メッシュコード
	int eId; ///< 施設等番号

	int eSequence; ///< アイテム内レコード番号
	int eBlockNo; ///< 同一施設等内アイテム番号

	int ePointCount; ///< 補間点総数
	Points ePoints; ///< 補間点

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAFACILSHAPE_H__0E513C4E_F1FA_461A_B0FA_CC1475BDB1B4__INCLUDED_)
