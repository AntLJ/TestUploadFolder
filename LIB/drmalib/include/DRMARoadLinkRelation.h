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

// DRMARoadLinkRelation.h: DRMARoadLinkRelation クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_)
#define AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 基本道路リンク・全道路リンク対応フォーマットクラス
 */
struct DRMARoadLinkRelationFormat
{
// 定義
	struct LinkId {
		_DRMAHex eNode1Id[4];
		_DRMAHex eNode2Id[4];
	};

	struct AllLink {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

// フォーマット
	_DRMADec eRecordId[2];
	LinkId eId;
	_DRMADec eSequence[2];

	_DRMADec eAllLinkCount[3];
	AllLink eAllLinks[24];

	_DRMAHex	eContinue[1];
};

/**
 * @brief 基本道路リンク・全道路リンク対応レコードクラス
 */
class DRMARoadLinkRelation : public DRMARecordT<DRMARoadLinkRelationFormat>
{
public:
// 定義
	typedef DRMABaseLinkId LinkId;
	typedef LinkId Id;

	struct AllLink : public DRMAAllLinkId, public DRMAItemConverterTemplate<DRMARoadLinkRelationFormat::AllLink> {
		AllLink(const Format& cOriginal);
	};

	typedef std::vector<AllLink> AllLinks;

// 構築/消滅
	DRMARoadLinkRelation(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendAllLinks(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	LinkId eId; ///< 基本道路リンク番号
	int eSequence; ///< アイテム内レコード番号

	int eAllLinkCount; ///< 全道路リンク総数
	AllLinks eAllLinks; ///< 全道路リンク番号

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAROADLINKRELATION_H__494296E1_C08B_467D_969C_71E6942CC55F__INCLUDED_)
