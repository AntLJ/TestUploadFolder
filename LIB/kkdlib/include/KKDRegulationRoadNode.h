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

// KKDRegulationRoadNode.h: KKDRegulationRoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_)
#define AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDVersion.h"
#include "KKDRecord.h"

/**
 * @brief 交通規制道路ノードデータフォーマットクラス
 */
struct KKDRegulationRoadNodeFormat
{
// 定義
	struct AppertainNode {
		_DRMADec eMeshDirection[1];
		_DRMAHex eId[5];
	};

	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct IntersectionId {
		_DRMADec eMeshCode[6];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
	};

	struct Name {
		_DRMADec eKanjiLength[2];
		_DRMAKanji eKanji[15];
		_DRMADec eKanaLength[2];
		_DRMAKana eKana[30];
	};

	struct IntersectionRegulation {
		_DRMAHex eOppositeNodeMeshDirection[1];
		_DRMAHex eOppositeNodeId[5];
		IntersectionId eOppositeIntersectionId;
		_DRMADec eRegulationLane[1];
		_DRMADec ePassage[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
	};

	struct ConnectLink {
		_DRMADec eConnectNodeMeshDirection[1];
		_DRMAHex eConnectNodeId[5];
		IntersectionId eConnectIntersectionId;
		Name eIntersectionName;
		_DRMADec eRegulationCount[2];
		IntersectionRegulation eIntersectionRegulations[2];
#if KKD_VERSION >= 101
		_DRMADec eIntersectionNameKanjiResearchLevel[1];
		_DRMADec eIntersectionNameKanaResearchLevel[1];
		_DRMADec eIntersectionResearchDate[4];
#endif // KKD_VERSION >= 101
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMADec ePrefectureCode[2];
	_DRMADec eStationCode[4];
	_DRMADec eCrossId[5];

	_DRMADec eSignal[1];

	_DRMADec eSequence[2];

	_DRMAHex eId[5];

	_DRMADec eAppertainNodeCount[2];
	AppertainNode eAppertainNodes[5];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[1];

#if KKD_VERSION >= 101
	_DRMAHex eFiller[5];
#else
	_DRMAHex eFiller[11];
#endif // KKD_VERSION >= 101

	_DRMADec eContinue[1];
};

/**
 * @brief 交通規制道路ノードデータレコードクラス
 */
class KKDRegulationRoadNode : public KKDRecordT<KKDRegulationRoadNodeFormat>
{
public:
	typedef int Id;

	/// 所属ノード
	struct AppertainNode : public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);
		AppertainNode(int nMeshDirection, int nId);

		bool operator < (const AppertainNode& rNode) const;

		int eMeshDirection; ///< 所属ノード該当メッシュコード
		int eId; ///< 所属ノード番号
	};

	typedef DRMAPoint Point ;
	typedef KKDIntersectionId IntersectionId;

	/// 交差点名称
	struct Name {
		int eKanjiLength; ///< 交差点名称
		char eKanji[31]; ///< 交差点名称漢字
		int eKanaLength; ///< カナ文字数
		char eKana[31]; ///< 交差点名称カナ
	};

	/// 交差点通行規制
	struct IntersectionRegulation : public KKDIntersectionRegulation, public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::IntersectionRegulation>{
		IntersectionRegulation(const Format& cOriginal);
	};

	/// 接続リンク
	struct ConnectLink : public DRMAItemConverterTemplate<KKDRegulationRoadNodeFormat::ConnectLink> {
		typedef std::vector<IntersectionRegulation> IntersectionRegulations;
		ConnectLink(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendIntersectionRegulations(const Format& cOriginal);

	public:
		int eConnectNodeMeshDirection; ///< 接続ノード該当メッシュコード
		int eConnectNodeId; ///< 接続ノード番号
		IntersectionId eConnectIntersectionId; ///< 接続交差点番号
		Name eIntersectionName; ///< 交差点名称
		int eRegulationCount; ///< 規制数
		IntersectionRegulations eIntersectionRegulations; ///< 交差点通行規制
#if KKD_VERSION >= 101
		int eIntersectionNameKanjiResearchLevel; ///< 交差点名称漢字調査レベル
		int eIntersectionNameKanaResearchLevel; ///< 交差点名称カナ調査レベル
		int eIntersectionResearchDate; ///< 交差点名称調査年月
#endif // KKD_VERSION >= 101
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectLink> ConnectLinks;

// 構築/消滅
	KKDRegulationRoadNode(const Format& cOriginalRecord);

	void append(const Format& cOriginalRecord);
protected:
// 追加
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectLinks(const Format& cOriginalRecord);

public:
//プロパティ
//	int eRecordId; ///< レコードID

	int ePrefectureCode; ///< 都道府県コード
	int eStationCode; ///< 署コード
	int eCrossId; ///< 署内交差点番号

	int eSignal; ///< 信号機有無コード

	int eSequence; ///< アイテム内レコード番号

	int eId; ///< ノード番号

	int eAppertainNodeCount; ///< 代表ノード所属ノード数
	AppertainNodes eAppertainNodes; ///< 所属ノード

	Point ePoint; ///< 正規化座標

	int eClass; ///< 交差点ノード種別コード

	int eConnectLinkCount; ///< 接続リンク本数
	ConnectLinks eConnectLinks; ///< 接続リンク

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_KKDREGULATIONROADNODE_H__3140CE5A_8191_4014_BA70_BA3BC3BF2FA4__INCLUDED_)
