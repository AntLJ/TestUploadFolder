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

// KKDLaneRegulationRoadNode.h: KKDLaneRegulationRoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_)
#define AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief レーン規制道路ノードデータフォーマットクラス
 */
struct KKDLaneRegulationRoadNodeFormat
{
// 定義
	struct AppertainNode {
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
		_DRMADec eNodeMeshDirection[1];
		_DRMAHex eNodeId[5];
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

	struct UTurnRegulation {
		_DRMADec eClass[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
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
		UTurnRegulation eUTurnRegulation;
		_DRMADec eRegulationCount[2];
		_DRMADec eLaneCount[1];
		_DRMADec eApproachLaneCount[1];
		IntersectionRegulation eIntersectionRegulations[2];
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMADec ePrefectureCode[2];
	_DRMADec eStationCode[4];
	_DRMADec eCrossId[5];

	_DRMADec eFiller1[1];

	_DRMADec eSequence[2];

	_DRMAHex eId[5];

	_DRMADec eAppertainNodeCount[2];
	AppertainNode eAppertainNodes[4];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectLinkCount[2];
	ConnectLink eConnectLinks[1];

	_DRMAHex eFiller[13];

	_DRMADec eContinue[1];
};

/**
 * @brief レーン規制道路ノードデータレコードクラス
 */
class KKDLaneRegulationRoadNode : public KKDRecordT<KKDLaneRegulationRoadNodeFormat>
{
public:
// 定義
	typedef int Id;

	/// 所属交差点
	struct AppertainNode : public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);

		bool operator < (const AppertainNode& rNode) const;

		int ePrefectureCode; ///< 都道府県コード
		int eStationCode; ///< 署コード
		int eCrossId; ///< 署内交差点番号
		int eNodeMeshDirection; ///< 所属ノードメッシュ番号
		int eNodeId; ///< 所属ノード番号
	};

	/// 正規化座標
	typedef DRMAPoint Point;

	/// 交差点番号
	typedef KKDIntersectionId IntersectionId;

	/// Ｕターン規制有無
	struct UTurnRegulation {
		int eClass; ///< 規制コード
		int eCarType; ///< 車種指定
		int eWeekday; ///< 曜日指定
		int eTime; ///< 時間指定
		int eDate; ///< 月日指定
	};

	/// 交差点通行規制
	struct IntersectionRegulation : public KKDIntersectionRegulation, public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::IntersectionRegulation> {
		IntersectionRegulation(const Format& cOriginal);
	};

	/// 接続リンク
	struct ConnectLink : public DRMAItemConverterTemplate<KKDLaneRegulationRoadNodeFormat::ConnectLink> {
		typedef std::vector<IntersectionRegulation> IntersectionRegulations;
		ConnectLink(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendIntersectionRegulations(const Format& cOriginal);

	public:
		int eConnectNodeMeshDirection; ///< 接続ノード該当メッシュコード
		int eConnectNodeId; ///< 接続ノード番号
		IntersectionId eConnectIntersectionId; ///< 接続交差点番号
		UTurnRegulation eUTurnRegulation; ///< Ｕターン規制有無
		int eRegulationCount; ///< 規制数
		int eLaneCount; ///< レーン数（流入レーン数）
		int eApproachLaneCount; ///< 直近レーン数
		IntersectionRegulations eIntersectionRegulations; ///< 接続リンク
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectLink> ConnectLinks;

// 構築/消滅
	KKDLaneRegulationRoadNode(const Format& cOriginalRecord);

	void append(const Format& cOriginalRecord);
protected:
// 追加
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectLinks(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID

	int ePrefectureCode; ///< 都道府県コード
	int eStationCode; ///< 署コード
	int eCrossId; ///< 署内交差点番号

	int eSequence; ///< アイテム内レコード番号

	int eId; ///< ノード番号

	int eAppertainNodeCount; ///< 代表交差点所属ノード数
	AppertainNodes eAppertainNodes; ///< 所属ノード

	Point ePoint; ///< 正規化座標

	int eClass; ///< 交差点ノード種別コード

	int eConnectLinkCount; ///< 接続リンク本数
	ConnectLinks eConnectLinks; ///< 接続リンク

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_KKDLANEREGULATIONROADNODE_H__438D5FEE_77CC_4BCA_AFCA_E3A0410A7424__INCLUDED_)
