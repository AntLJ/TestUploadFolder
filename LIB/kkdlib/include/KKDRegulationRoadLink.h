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

// KKDRegulationRoadLink.h: KKDRegulationRoadLink クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_)
#define AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief 交通規制道路リンクデータフォーマットクラス
 */
struct KKDRegulationRoadLinkFormat
{
// 定義
	struct IntersectionId {
		_DRMADec eMeshCode[6];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
	};

	struct IntersectionIdPair {
		IntersectionId eIntersection1Id;
		IntersectionId eIntersection2Id;
	};

	struct LinkId {
		_DRMAHex eNode1Id[5];
		_DRMAHex eNode2Id[5];
	};

	struct Condition {
		_DRMADec eClass[1];
		_DRMADec eCarType[2];
		_DRMADec eWeekday[2];
		_DRMADec eTime[8];
		_DRMADec eDate[8];
		_DRMADec eCarWeight[3];
		_DRMADec eCarWidth[3];
		_DRMADec eCarHeight[3];
	};

	struct NodesInMesh {
		typedef _DRMAHex NodeId[5];
		_DRMADec eMeshCode[6];
		_DRMADec eNodeCount[2];
		NodeId eNodeIds[20];
	};

// フォーマット
	_DRMADec eRecordId[2];

	IntersectionIdPair eId;
	LinkId eLinkId;

	_DRMADec eSequence[2];

	_DRMADec eIllNavigation[1];
	
	_DRMADec eConditionCount[2];
	Condition eConditions[3];

	_DRMADec eNodeCount[2];
	_DRMADec eMeshCount[2];
	NodesInMesh eNodesInMeshs[1];

	_DRMAHex eFiller1[2];

	_DRMADec eContinue[1];
};

/**
 * @brief 交通規制道路リンクデータレコードクラス
 */
class KKDRegulationRoadLink : public KKDRecordT<KKDRegulationRoadLinkFormat>
{
public:
// 定義
	typedef KKDIntersectionId IntersectionId;
	typedef KKDIntersectionIdPair IntersectionIdPair;
	typedef KKDLinkId LinkId; ///< リンク番号
	typedef KKDIntersectionIdPair Id;

	/// 交通規制データ
	struct Condition : public DRMAItemConverterTemplate<KKDRegulationRoadLinkFormat::Condition> {
		Condition(const Format& cOriginal);

		int eClass; ///< 交通規制種別コード
		int eCarType; ///< 車種指定
		int eWeekday; ///< 曜日指定
		int eTime; ///< 時間指定（始、終時刻）
		int eDate; ///< 月日指定（始、終日時）
		int eCarWeight; ///< 重量指定（0.1t）
		int eCarWidth; ///< 車幅指定（0.1m）
		int eCarHeight; ///< 車高指定（0.1m）
	};

	/// メッシュ
	struct NodesInMesh : public DRMAItemConverterTemplate<KKDRegulationRoadLinkFormat::NodesInMesh> {
		typedef int NodeId;
		typedef std::vector<NodeId> NodeIds;

		NodesInMesh(const Format& cOriginal);

		bool append(const Format& cOriginal);

	protected:
		bool appendNodeIds(const Format& cOriginal);

	public:
		int eMeshCode; ///< ２次メッシュコード
		int eNodeCount; ///< メッシュ内ノード数
		NodeIds eNodeIds; ///< ノード番号
	};

	typedef std::vector<Condition> Conditions;
	typedef std::vector<NodesInMesh> NodesInMeshs;

// 構築/消滅
	KKDRegulationRoadLink(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);

protected:
// 追加
	void appendConditions(const Format& cOriginalRecord);
	void appendNodesInMeshs(const Format& cOrignalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID

	KKDIntersectionIdPair eId; ///< 交差点番号ペア
	LinkId eLinkId; ///< リンク番号

	int eSequence; ///< アイテム内レコード番号

	int eIllNavigation; ///< 交通誘導不適区間コード
	
	int eConditionCount; ///< 規制数
	Conditions eConditions; ///< 交通規制データ

	int eNodeCount; ///< リンク内ノード数
	int eMeshCount; ///< ２次メッシュ数
	NodesInMeshs eNodesInMeshs; ///< メッシュ

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId; ///< 当該レコードID
};

#endif // !defined(AFX_KKDREGULATIONROADLINK_H__56F1C323_3F29_4D07_8F68_C72B5EBA97FA__INCLUDED_)
