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

// DRMALoad.h: DRMALoad クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

/**
 * @file DRMALoad.h
 * @brief DRMAデータ統合読み込みクラス定義ヘッダファイル
 */
#if !defined(AFX_DRMALOAD_H__FD588142_08D7_4823_97FD_B854644E4069__INCLUDED_)
#define AFX_DRMALOAD_H__FD588142_08D7_4823_97FD_B854644E4069__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#include "DRMACodeCommunity.h"
#include "DRMALoadProtocol.h"
#include "DRMALayer.h"

/**
 * @brief DRMAデータ統合読み込みクラス
 *
 * ".mt"ファイルが所持する全てのレコードの情報を取得・保持する。
 */
template <
	typename TBaseRoadNodeLayer, 
	typename TBaseRoadLinkLayer,
	typename TBaseRoadLinkAttrLayer,
	typename TRoadLinkRelationLayer,
	typename TBeaconLocationLayer,
	typename TAllRoadNodeLayer,
	typename TAllRoadLinkLayer,
	typename TSiteLayer,
	typename TAdminLocationLayer,
	typename TRailwayLocationLayer,
	typename TFacilLocationLayer,
	typename TFacilShapeLayer,
	typename TPlaceNameLocationLayer,
	typename TAllRoadLinkAttrLayer
>
class DRMALoadT : public DRMALoadProtocol
{
public:
	typedef TBaseRoadNodeLayer DRMABaseRoadNodeLayer;
	typedef TBaseRoadLinkLayer DRMABaseRoadLinkLayer;
	typedef TBaseRoadLinkAttrLayer DRMABaseRoadLinkAttrLayer;
	typedef TRoadLinkRelationLayer DRMARoadLinkRelationLayer;
	typedef TBeaconLocationLayer DRMABeaconLocationLayer;
	typedef TAllRoadNodeLayer DRMAAllRoadNodeLayer;
	typedef TAllRoadLinkLayer DRMAAllRoadLinkLayer;
	typedef TSiteLayer DRMASiteLayer;
	typedef TAdminLocationLayer DRMAAdminLocationLayer;
	typedef TRailwayLocationLayer DRMARailwayLocationLayer;
	typedef TFacilLocationLayer DRMAFacilLocationLayer;
	typedef TFacilShapeLayer DRMAFacilShapeLayer;
	typedef TPlaceNameLocationLayer DRMAPlaceNameLocationLayer;
	typedef TAllRoadLinkAttrLayer DRMAAllRoadLinkAttrLayer;

// 取得
	const DRMAAdmin1Layer& admin1Layer() const { return mAdmin1Layer; }
	const DRMAAdmin2Layer& admin2Layer() const { return mAdmin2Layer; }
	const DRMABaseRoadNodeLayer& baseRoadNodeLayer() const { return mBaseRoadNodeLayer; }
	const DRMABaseRoadLinkLayer& baseRoadLinkLayer() const { return mBaseRoadLinkLayer; }
	const DRMABaseRoadLinkAttrLayer& baseRoadLinkAttrLayer() const { return mBaseRoadLinkAttrLayer; }
	const DRMARoadLinkRelationLayer& roadLinkRelationLayer() const { return mRoadLinkRelationLayer; }
	const DRMABeaconLocationLayer& beaconLocationLayer() const { return mBeaconLocationLayer; }
	const DRMAAllRoadNodeLayer& allRoadNodeLayer() const { return mAllRoadNodeLayer; }
	const DRMAAllRoadLinkLayer& allRoadLinkLayer() const { return mAllRoadLinkLayer; }
	const DRMASiteLayer& siteLayer() const { return mSiteLayer; }
	const DRMAAdminLocationLayer& adminLocationLayer() const { return mAdminLocationLayer; }
	const DRMARailwayLocationLayer& railwayLocationLayer() const { return mRailwayLocationLayer; }
	const DRMAFacilLocationLayer& facilLocationLayer() const { return mFacilLocationLayer; }
	const DRMAFacilShapeLayer& facilShapeLayer() const { return mFacilShapeLayer; }
	const DRMAPlaceNameLocationLayer& placeNameLocationLayer() const { return mPlaceNameLocationLayer; }
	const DRMAAllRoadLinkAttrLayer& allRoadLinkAttrLayer() const { return mAllRoadLinkAttrLayer; }

protected:
// プロトコルの実装
	bool append(int cRecordId, const char* cOriginalRecord)
	{
		// レコードIDで処理を振り分け
		switch(cRecordId) {
		case drma::kAdmin1Id: // 管理データ（その１）
			mAdmin1Layer.append(reinterpret_cast<const DRMAAdmin1Format&>(*cOriginalRecord));
			break;
		case drma::kAdmin2Id: // 管理データ（その２）
			mAdmin2Layer.append(reinterpret_cast<const DRMAAdmin2Format&>(*cOriginalRecord));
			break;
		case drma::kBaseRoadNodeId: // 基本道路ノードデータ
			mBaseRoadNodeLayer.append(reinterpret_cast<const DRMABaseRoadNodeFormat&>(*cOriginalRecord));
			break;
		case drma::kBaseRoadLinkId: // 基本道路リンクデータ
			mBaseRoadLinkLayer.append(reinterpret_cast<const DRMABaseRoadLinkFormat&>(*cOriginalRecord));
			break;
		case drma::kBaseRoadLinkAttrId: // 基本道路リンク内属性データ
			mBaseRoadLinkAttrLayer.append(reinterpret_cast<const DRMABaseRoadLinkAttrFormat&>(*cOriginalRecord));
			break;
		case drma::kRoadLinkRelationId: // 基本道路リンク・全道路リンク対応データ
			mRoadLinkRelationLayer.append(reinterpret_cast<const DRMARoadLinkRelationFormat&>(*cOriginalRecord));
			break;
		case drma::kBeaconLocationId: // ビーコン位置データ
			mBeaconLocationLayer.append(reinterpret_cast<const DRMABeaconLocationFormat&>(*cOriginalRecord));
			break;
		case drma::kAllRoadNodeId: // 全道路ノードデータ
			mAllRoadNodeLayer.append(reinterpret_cast<const DRMAAllRoadNodeFormat&>(*cOriginalRecord));
			break;
		case drma::kAllRoadLinkId: // 全道路リンクデータ
			mAllRoadLinkLayer.append(reinterpret_cast<const DRMAAllRoadLinkFormat&>(*cOriginalRecord));
			break;
		case drma::kSiteId: // 水系データ
			mSiteLayer.append(reinterpret_cast<const DRMASiteFormat&>(*cOriginalRecord));
			break;
		case drma::kAdminLocationId: // 行政界位置データ
			mAdminLocationLayer.append(reinterpret_cast<const DRMAAdminLocationFormat&>(*cOriginalRecord));
			break;
		case drma::kRailwayLocationId: // 鉄道位置データ
			mRailwayLocationLayer.append(reinterpret_cast<const DRMARailwayLocationFormat&>(*cOriginalRecord));
			break;
		case drma::kFacilLocationId: // 施設等位置データ
			mFacilLocationLayer.append(reinterpret_cast<const DRMAFacilLocationFormat&>(*cOriginalRecord));
			break;
		case drma::kFacilShapeId: // 施設等形状データ
			mFacilShapeLayer.append(reinterpret_cast<const DRMAFacilShapeFormat&>(*cOriginalRecord));
			break;
		case drma::kPlaceNameLocationId: // 地名等表示位置データ
			mPlaceNameLocationLayer.append(reinterpret_cast<const DRMAPlaceNameLocationFormat&>(*cOriginalRecord));
			break;
		case drma::kAllRoadLinkAttrId: // 全道路リンク内属性データ
			mAllRoadLinkAttrLayer.append(reinterpret_cast<const DRMAAllRoadLinkAttrFormat&>(*cOriginalRecord));
			break;
		default:
			return false;
		}

		return true;
	}

	void clear()
	{
		mAdmin1Layer.clear();
		mAdmin2Layer.clear();
		mBaseRoadNodeLayer.clear();
		mBaseRoadLinkLayer.clear();
		mBaseRoadLinkAttrLayer.clear();
		mRoadLinkRelationLayer.clear();
		mBeaconLocationLayer.clear();
		mAllRoadNodeLayer.clear();
		mAllRoadLinkLayer.clear();
		mSiteLayer.clear();
		mAdminLocationLayer.clear();
		mRailwayLocationLayer.clear();
		mFacilLocationLayer.clear();
		mFacilShapeLayer.clear();
		mPlaceNameLocationLayer.clear();
		mAllRoadLinkAttrLayer.clear();
	}

// プロパティ
	DRMAAdmin1Layer mAdmin1Layer;
	DRMAAdmin2Layer mAdmin2Layer;
	DRMABaseRoadNodeLayer mBaseRoadNodeLayer;
	DRMABaseRoadLinkLayer mBaseRoadLinkLayer;
	DRMABaseRoadLinkAttrLayer mBaseRoadLinkAttrLayer;
	DRMARoadLinkRelationLayer mRoadLinkRelationLayer;
	DRMABeaconLocationLayer mBeaconLocationLayer;
	DRMAAllRoadNodeLayer mAllRoadNodeLayer;
	DRMAAllRoadLinkLayer mAllRoadLinkLayer;
	DRMASiteLayer mSiteLayer;
	DRMAAdminLocationLayer mAdminLocationLayer;
	DRMARailwayLocationLayer mRailwayLocationLayer;
	DRMAFacilLocationLayer mFacilLocationLayer;
	DRMAFacilShapeLayer mFacilShapeLayer;
	DRMAPlaceNameLocationLayer mPlaceNameLocationLayer;
	DRMAAllRoadLinkAttrLayer mAllRoadLinkAttrLayer;
};

typedef DRMALoadT<
	DRMABaseRoadNodeLayer,
	DRMABaseRoadLinkLayer,
	DRMABaseRoadLinkAttrLayer,
	DRMARoadLinkRelationLayer,
	DRMABeaconLocationLayer,
	DRMAAllRoadNodeLayer,
	DRMAAllRoadLinkLayer,
	DRMASiteLayer,
	DRMAAdminLocationLayer,
	DRMARailwayLocationLayer,
	DRMAFacilLocationLayer,
	DRMAFacilShapeLayer,
	DRMAPlaceNameLocationLayer,
	DRMAAllRoadLinkAttrLayer
> DRMALoad;

/**
 * @example example.cpp
 *
 * Dドライブのルートディレクトリ直下に存在する「DRMA」ディレクトリより533945のデータを読み込み、
 * 交差点名称を持つノードの情報を標準出力に出力する。
 */

#endif // !defined(AFX_DRMALOAD_H__FD588142_08D7_4823_97FD_B854644E4069__INCLUDED_)
