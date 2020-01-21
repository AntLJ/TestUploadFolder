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

// DRMABaseRoadNode.h: DRMABaseRoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_)
#define AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 基本道路ノードデータフォーマットクラス
 */
struct DRMABaseRoadNodeFormat
{
// 定義
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ConnectLink {
		_DRMAHex eNodeId[4];
		_DRMADec ePassageCode[8];
		_DRMADec eAngle[3];
	};

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[10];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[20];
	};

	struct ConnectFerry {
		_DRMADec eJointMeshCode[6];
		_DRMAHex eJointNodeId[4];
	};

// フォーマット
	_DRMADec eRecordId[2];
	_DRMAHex eId[4];
	_DRMADec eSequence[2];

	Point ePoint;
	
	_DRMADec eAltitude[3];
	_DRMADec eClass[1];

	_DRMADec eJointMeshCode[6];
	_DRMAHex eJointNodeId[4];

	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[8];

	Name eName;
	_DRMADec eConnectFerryCount[2];
	ConnectFerry eConnectFerries[5];

	_DRMAHex eFiller[6];

	_DRMAHex eContinue[1];
};

/**
 * @brief 基本道路ノードデータレコードクラス
 */
class DRMABaseRoadNode : public DRMARecordT<DRMABaseRoadNodeFormat>
{
public:
// 定義
	typedef DRMAPoint Point;
	typedef DRMAShortName Name;
	typedef int Id;

	struct ConnectLink : public DRMAItemConverterTemplate<DRMABaseRoadNodeFormat::ConnectLink> {
		ConnectLink(const Format& cOriginal);

		int eNodeId; ///< 接続ノード番号
		int ePassageCode; ///< 交差点通行コード
		int eAngle; ///< 接続角度（度）
	};

	struct ConnectFerry  : public DRMAItemConverterTemplate<DRMABaseRoadNodeFormat::ConnectFerry> {
		ConnectFerry(const Format& cOriginal);

		int eJointMeshCode; ///< 航路接続２次メッシュコード
		int eJointNodeId; ///< 航路接続ノード番号
	};

	typedef std::vector<ConnectLink> ConnectLinks;
	typedef std::vector<ConnectFerry> ConnectFerries;

// 構築/消滅
	DRMABaseRoadNode(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendConnectLinks(const Format& cOriginalRecord);
	void appendConnectFerries(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID
	int eId; ///< ノード番号
	int eSequence; ///< アイテム内レコード番号

	Point ePoint; ///< 正規化座標

	int eAltitude; ///< 標高
	int eClass; ///< ノード種別コード

	int eJointMeshCode; ///< 隣接２次メッシュコード
	int eJointNodeId; ///< 隣接２次メッシュ接合ノード番号

	int eConnectLinkCount; ///< 接続リンク本数
	ConnectLinks eConnectLinks; ///< 接続リンク

	Name eName; ///< 交差点名称
	int eConnectFerryCount; ///< フェリー接続航路総数
	ConnectFerries eConnectFerries; ///< フェリー接続航路
	
	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMABASEROADNODE_H__EF04ED2C_7F7C_444F_9767_5AE2AAA23ED5__INCLUDED_)
