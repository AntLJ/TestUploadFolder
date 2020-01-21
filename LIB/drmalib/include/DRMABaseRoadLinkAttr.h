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

// DRMABaseRoadLinkAttr.h: DRMABaseRoadLinkAttr クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_)
#define AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 基本道路リンク内属性フォーマットクラス
 */
struct DRMABaseRoadLinkAttrFormat
{
// 定義
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct Attr {
		struct Point {
			_DRMADec eX[5];
			_DRMADec eY[5];
		};

		struct Name {
			_DRMADec eKanjiLength[2];
			_DRMAKanji eKanji[10];
			_DRMADec eKanaLength[2];
			_DRMAKana eKana[20];
		};

		_DRMADec eClass[2];
		_DRMADec eDispLevel[1];
		_DRMADec eBeginPointNo[3];
		_DRMADec ePreBegin[1];
		_DRMADec eEndPointNo[3];
		_DRMADec eEpiEnd[1];
		_DRMADec eLength[5];
		
		// 属性名称表示参考位置正規化座標
		Point ePoint;
		
		// 属性名称
		Name eName;
	};

	struct Attr2 {
		_DRMADec eHeavyNoPassage[1];
		_DRMADec eFacilManager[4];
	};

// フォーマット
	_DRMADec eRecordId[2];
	LinkId eId;

	_DRMAHex eFiller1[2];

	_DRMADec eSequence[2];

	_DRMADec eAttrCount[2];
	Attr eAttrs[3];
	Attr2 eAttr2s[3];

	_DRMAHex eFiller2[14];

	_DRMADec eContinue[1];
};

/**
 * @brief 基本道路リンク内属性レコードクラス
 */
class DRMABaseRoadLinkAttr : public DRMARecordT<DRMABaseRoadLinkAttrFormat>
{
public:
// 定義
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct Attr : public DRMAItemConverterTemplate<DRMABaseRoadLinkAttrFormat::Attr> {
		typedef DRMAPoint Point;
		typedef DRMAShortName Name;

		Attr(const Format& cOriginal);

		int eClass; ///< 属性種別コード
		int eDispLevel; ///< 表示レベル参考コード
		int eBeginPointNo; ///< 始点補完点番号
		int ePreBegin; ///< 始点側接続有無コード
		int eEndPointNo; ///< 終点補完点番号
		int eEpiEnd; ///< 終点側接続有無コード
		int eLength; ///< 属性延長
		
		// 属性名称表示参考位置正規化座標
		Point ePoint;
		
		// 属性名称
		Name eName;
	};

	struct Attr2 : public DRMAItemConverterTemplate<DRMABaseRoadLinkAttrFormat::Attr2> {
		Attr2(const Format& cOriginal);

		int eHeavyNoPassage; ///< 大型車通行不可コード
		int eFacilManager; ///< 
	};

	typedef std::vector<Attr> Attrs;
	typedef std::vector<Attr2> Attr2s;

// 構築/消滅
	DRMABaseRoadLinkAttr(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendAttrs(const Format& cOriginalRecord);
	void appendAttr2s(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	LinkId eId; ///< リンク番号
	int eSequence; ///< アイテム内レコード番号

	int eAttrCount; ///< リンク内属性総数
	Attrs eAttrs; ///< リンク内属性
	Attr2s eAttr2s; ///< リンク内属性'

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADLINKATTR_H__99174725_1B65_40CB_B711_C24E50480E8F__INCLUDED_)
