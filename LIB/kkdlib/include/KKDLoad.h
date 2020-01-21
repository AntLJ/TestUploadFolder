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

// KKDLoad.h: KKDLoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

/**
 * @file
 * @brief KKDデータ統合読み込みクラス定義ヘッダファイル
 */
#if !defined(AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_)
#define AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KKDCodeCommunity.h"
#include "KKDLoadProtocol.h"
#include "KKDLayer.h"

/**
 * @brief KKDデータ統合読み込みクラス
 */
template <
	typename TRegulationRoadNodeLayer,
	typename TRegulationRoadLinkLayer,
	typename TLaneRegulationRoadNodeLayer,
	typename TLaneRegulationRoadLinkLayer,
	typename TLaneRegulationIntersectionNodeLayer
>
class KKDLoadT : public KKDLoadProtocol  
{
public:
// 取得
	typedef TRegulationRoadNodeLayer KKDRegulationRoadNodeLayer;
	typedef TRegulationRoadLinkLayer KKDRegulationRoadLinkLayer;
	typedef TLaneRegulationRoadNodeLayer KKDLaneRegulationRoadNodeLayer;
	typedef TLaneRegulationRoadLinkLayer KKDLaneRegulationRoadLinkLayer;
	typedef TLaneRegulationIntersectionNodeLayer KKDLaneRegulationIntersectionNodeLayer;

	const KKDAdminLayer& adminLayer() const { return mAdminLayer; }
	const KKDRegulationRoadNodeLayer& regulationRoadNodeLayer() const { return mRegulationRoadNodeLayer; }
	const KKDRegulationRoadLinkLayer& regulationRoadLinkLayer() const { return mRegulationRoadLinkLayer; }
	const KKDLaneRegulationRoadNodeLayer& laneRegulationRoadNodeLayer() const { return mLaneRegulationRoadNodeLayer; }
	const KKDLaneRegulationRoadLinkLayer& laneRegulationRoadLinkLayer() const { return mLaneRegulationRoadLinkLayer; }
	const KKDLaneRegulationIntersectionNodeLayer& laneRegulationIntersectionNodeLayer() const { return mLaneRegulationIntersectionNodeLayer; }

protected:
// プロトコルの実装
	bool append(int cRecordId, const char* cOriginalRecord)
	{
		// レコードIDで処理を振り分け
		switch(cRecordId) {
		case kkd::kAdminId: // 管理データ
			mAdminLayer.append(reinterpret_cast<const KKDAdminFormat&>(*cOriginalRecord));
			break;
		case kkd::kRegulationRoadNodeId: // 交通規制道路ノードデータ
			mRegulationRoadNodeLayer.append(reinterpret_cast<const KKDRegulationRoadNodeFormat&>(*cOriginalRecord));
			break;
		case kkd::kRegulationRoadLinkId: // 交通規制道路リンクデータ
			mRegulationRoadLinkLayer.append(reinterpret_cast<const KKDRegulationRoadLinkFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationRoadNodeId: // レーン規制道路ノードデータ
			mLaneRegulationRoadNodeLayer.append(reinterpret_cast<const KKDLaneRegulationRoadNodeFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationRoadLinkId: // レーン規制道路リンクデータ
			mLaneRegulationRoadLinkLayer.append(reinterpret_cast<const KKDLaneRegulationRoadLinkFormat&>(*cOriginalRecord));
			break;
		case kkd::kLaneRegulationIntersectionNodeId: // レーン規制集合交差点ノードデータ
			mLaneRegulationIntersectionNodeLayer.append(reinterpret_cast<const KKDLaneRegulationIntersectionNodeFormat&>(*cOriginalRecord));
			break;
		default:
			return false;
		}

		return true;
	}

	void clear()
	{
		mAdminLayer.clear();
		mRegulationRoadNodeLayer.clear();
		mRegulationRoadLinkLayer.clear();
		mLaneRegulationRoadNodeLayer.clear();
		mLaneRegulationRoadLinkLayer.clear();
		mLaneRegulationIntersectionNodeLayer.clear();
	}

protected:
// プロパティ
	KKDAdminLayer mAdminLayer;
	KKDRegulationRoadNodeLayer mRegulationRoadNodeLayer;
	KKDRegulationRoadLinkLayer mRegulationRoadLinkLayer;
	KKDLaneRegulationRoadNodeLayer mLaneRegulationRoadNodeLayer;
	KKDLaneRegulationRoadLinkLayer mLaneRegulationRoadLinkLayer;
	KKDLaneRegulationIntersectionNodeLayer mLaneRegulationIntersectionNodeLayer;
};

typedef KKDLoadT<
	DRMAContinualLayerTemplate<KKDRegulationRoadNode>,
	DRMAContinualLayerTemplate<KKDRegulationRoadLink>,
	DRMAContinualLayerTemplate<KKDLaneRegulationRoadNode>,
	DRMAContinualLayerTemplate<KKDLaneRegulationRoadLink>,
	DRMAContinualLayerTemplate<KKDLaneRegulationIntersectionNode>
> KKDLoad;

/**
 * @example example.cpp
 *
 * Dドライブのルートディレクトリ直下に存在する「kkd」ディレクトリより533945のKKDデータを読み込み、
 * 全交通規制道路ノードの各種情報を出力する。
 */

#endif // !defined(AFX_KKDLOAD_H__F0FADD20_8296_49BB_82F0_2DE86ADF638E__INCLUDED_)
