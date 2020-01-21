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

// DRMAFacilLocation.h: DRMAFacilLocation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_)
#define AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief 施設等位置データフォーマットクラス
 */
struct DRMAFacilLocationFormat
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

	struct AdjoinBaseNode {
		_DRMADec eMeshCode[6];
		_DRMAHex eId[4];
	};

	struct AdjoinBaseLink {
		struct LinkId {
			_DRMAHex eNode1Id[4];
			_DRMAHex eNode2Id[4];
		};

		_DRMADec eMeshCode[6];
		LinkId eId;
		_DRMADec eLinkDir[1];
	};

// フォーマット
	_DRMADec eRecordId[2];
	_DRMADec eId[5];
	_DRMADec eClass[6];
	_DRMADec eAdminCode[5];
	_DRMADec eShapeCount[1];

	_DRMAHex eFiller1[4];

	Point ePoint;

	ShortName eShortName;
	LongName eLongName;

	_DRMADec eAdjoinBaseNodeCount[1];
	AdjoinBaseNode eAdjoinBaseNodes[2];

	_DRMADec eAdjoinBaseLinkCount[1];
	AdjoinBaseLink eAdjoinBaseLinks[2];

	_DRMAHex eFiller2[3];
};

/**
 * @brief 施設等位置データレコードクラス
 */
class DRMAFacilLocation : public DRMARecordT<DRMAFacilLocationFormat>
{
public:
// 定義
	typedef DRMAPoint Point;
	typedef DRMAShortName ShortName;
	typedef DRMALongName LongName;
	typedef int Id;

	struct AdjoinBaseNode {
		int eMeshCode; ///< ２次メッシュコード
		int eId; ///< ノード番号
	};

	struct AdjoinBaseLink {
		typedef DRMABaseLinkId LinkId;

		int eMeshCode; ///< ２次メッシュコード
		LinkId eId; ///< リンク番号
		int eLinkDir; ///< 対応リンク方向
	};

// 構築/消滅
	DRMAFacilLocation(const Format& cOriginalRecord);

// プロパティ
//	int eRecordId; ///< レコードID
	int eId; ///< 施設等番号
	int eClass; ///< 施設等種別コード
	int eAdminCode; ///< 行政区域コード
	int eShapeCount; ///< 形状データアイテム総数

	Point ePoint; ///< 正規化座標

	ShortName eShortName; ///< 表示名称
	LongName eLongName; ///< 正式名称

	int eAdjoinBaseNodeCount; ///< 隣接基本道路ノード総数
	AdjoinBaseNode eAdjoinBaseNodes[2]; ///< 隣接基本道路ノード

	int eAdjoinBaseLinkCount; ///< 隣接基本道路リンク総数
	AdjoinBaseLink eAdjoinBaseLinks[2]; ///< 隣接基本道路リンク

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAFACILLOCATION_H__CAD33E15_4916_4AA4_AB0F_F1B97EB0C303__INCLUDED_)
