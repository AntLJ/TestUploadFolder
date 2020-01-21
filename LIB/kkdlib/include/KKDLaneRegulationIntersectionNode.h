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

// KKDLaneRegulationIntersectionNode.h: KKDLaneRegulationIntersectionNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_)
#define AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "KKDRecord.h"

/**
 * @brief レーン規制集合交差点ノードデータフォーマットクラス
 */
struct KKDLaneRegulationIntersectionNodeFormat
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

	struct ConnectIntersection {
		_DRMADec eMeshCode[6];
		_DRMAHex eConnectIntersectionNodeId[5];
		_DRMADec ePrefectureCode[2];
		_DRMADec eStationCode[4];
		_DRMADec eCrossId[5];
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
	AppertainNode eAppertainNodes[5];

	Point ePoint;

	_DRMADec eClass[1];

	_DRMADec eConnectIntersectionCount[2];
	ConnectIntersection eConnectIntersections[5];

	_DRMAHex eFiller[24];

	_DRMADec eContinue[1];
};

/**
 * @brief レーン規制集合交差点ノードデータレコードクラス
 */
class KKDLaneRegulationIntersectionNode : public KKDRecordT<KKDLaneRegulationIntersectionNodeFormat>
{
public:
// 定義
	typedef int Id;

	/// 所属交差点
	struct AppertainNode : public DRMAItemConverterTemplate<KKDLaneRegulationIntersectionNodeFormat::AppertainNode> {
		AppertainNode(const Format& cOriginal);

		int ePrefectureCode; ///< 都道府県コード
		int eStationCode; ///< 署コード
		int eCrossId; ///< 署内交差点番号
		int eNodeMeshDirection; ///< 所属ノードメッシュ番号
		int eNodeId; ///< 所属ノード番号
	};

	/// 正規化座標
	typedef DRMAPoint Point;

	/// 交差点
	struct ConnectIntersection : public DRMAItemConverterTemplate<KKDLaneRegulationIntersectionNodeFormat::ConnectIntersection> {
		ConnectIntersection(const Format& cOriginal);

		int eMeshCode; ///< ２次メッシュコード
		int eConnectIntersectionNodeId; ///< 接続交差点ノード番号
		int ePrefectureCode; ///< 都道府県コード
		int eStationCode; ///< 署コード
		int eCrossId; ///< 署内交差点番号
	};

	typedef std::vector<AppertainNode> AppertainNodes;
	typedef std::vector<ConnectIntersection> ConnectIntersections;

// 構築/消滅
	KKDLaneRegulationIntersectionNode(const Format& cOriginalRecord);

// 追加
	void append(const Format& cOriginalRecord);
protected:
// 追加
	void appendAppertainNodes(const Format& cOriginalRecord);
	void appendConnectIntersections(const Format& cOriginalRecord);

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

	int eConnectIntersectionCount; ///< 接続交差点数
	ConnectIntersections eConnectIntersections; ///< 接続交差点

	int eContinue; ///< 継続フラグ

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_KKDLANEREGULATIONINTERSECTIONNODE_H__A34AB649_7903_4A10_82E8_FC7722A80F28__INCLUDED_)
