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

// DRMAAllRoadNode.h: DRMAAllRoadNode クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_)
#define AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "DRMARecord.h"

/**
 * @brief 全道路ノードデータフォーマットクラス
 */
struct DRMAAllRoadNodeFormat
{
// 定義
	struct Point {
		_DRMADec eX[5];
		_DRMADec eY[5];
	};

	struct ConnectLink {
		_DRMAHex eNodeId[5];
	};

// フォーマット
	_DRMADec eRecordId[2];

	_DRMAHex eId[5];
	Point ePoint;
	_DRMADec eClass[1];
	_DRMADec eJointMeshCode[6];
	_DRMAHex eJointNodeId[5];
	
	_DRMADec eConnectLinkCount[1];
	ConnectLink eConnectLinks[8];

	_DRMAHex eFiller[186];
};

/**
 * @brief 全道路ノードデータレコードクラス
 */
class DRMAAllRoadNode : public DRMARecordT<DRMAAllRoadNodeFormat>
{
public:
// 定義
	typedef DRMAPoint Point;
	typedef int Id;

	struct ConnectLink : public DRMAItemConverterTemplate<DRMAAllRoadNodeFormat::ConnectLink> {
		ConnectLink(const Format& cOriginal);

		int eNodeId;
	};

	typedef std::vector<ConnectLink> ConnectLinks;

// 構築/消滅
	DRMAAllRoadNode(const Format& cOriginalRecord);

protected:
// 追加
	void appendConnectLinks(const Format& cOriginalRecord);

public:
// プロパティ
//	int eRecordId; ///< レコードID

	int eId; ///< ノード番号
	Point ePoint; ///< 正規化座標
	int eClass; ///< ノード種別コード
	int eJointMeshCode; ///< 隣接２次メッシュコード
	int eJointNodeId; ///< 隣接２次メッシュ接合ノード番号
	
	int eConnectLinkCount; ///< 接続リンク本数
	ConnectLinks eConnectLinks; ///< 接続ノード番号

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAALLROADNODE_H__AD105E2A_3CA9_458F_994B_C6E29006B2BB__INCLUDED_)
