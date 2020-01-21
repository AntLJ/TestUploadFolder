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

// DRMAAdmin2.h: DRMAAdmin2 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_)
#define AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief 管理データ（その２）フォーマットクラス
 */
struct DRMAAdmin2Format
{
// 定義
	struct LayerCount {
		typedef _DRMADec Code[5];

		Code eBaseRoadNode;
		Code eBaseRoadLink;
		Code eBaseRoadLinkAttr;
		Code eBeaconLocation;
		Code eAllRoadNode;
		Code eAllRoadLink;
		Code eSite;
		Code eAdminLocation;
		Code eRailwayLocation;
		Code eFacilLocation;
		Code eFacilShape;
		Code ePlaceNameLocation;
		Code eRoadLinkRelation;
		Code eAllRoadLinkAttr;
	};

	struct MaxItemId {
		_DRMAHex eBaseRoadNode[4];
		_DRMAHex eAllRoadNode1[5];
		_DRMAHex eAllRoadNode2[5];
		_DRMADec eSite[4];
		_DRMADec eAdminLocation[4];
		_DRMADec eRailwayLocation[4];
		_DRMADec eFacil[5];
		_DRMADec ePlaceNameLocation[4];
		_DRMADec eBeacon[4];
	};

// フォーマット
	_DRMADec eRecordId[2];

	LayerCount eRecordCount;

	_DRMAHex eFiller1[10];

	LayerCount eItemCount;
	
	_DRMAHex eFiller2[10];

	_DRMAKanji eCopyright[23];

	MaxItemId eMaxItemId;

	_DRMAHex eFiller3[9];

};

/**
 * @brief 管理データ（その２）レコードクラス
 */
class DRMAAdmin2 : public DRMARecordT<DRMAAdmin2Format>
{
public:
// 定義
	struct LayerCount {
		int eBaseRoadNode; ///< 基本道路ノードデータ
		int eBaseRoadLink; ///< 基本道路リンクデータ
		int eBaseRoadLinkAttr; ///< 基本道路リンク内属性データ
		int eBeaconLocation; ///< ビーコン位置データ
		int eAllRoadNode; ///< 全道路ノードデータ
		int eAllRoadLink; ///< 全道路リンクデータ
		int eSite; ///< 水系データ
		int eAdminLocation; ///< 行政界位置データ
		int eRailwayLocation; ///< 鉄道位置データ
		int eFacilLocation; ///< 施設等位置データ
		int eFacilShape; ///< 施設等形状データ
		int ePlaceNameLocation; ///< 地名等表示位置データ
		int eRoadLinkRelation; ///< 基本道路リンク・全道路リンク対応データ
		int eAllRoadLinkAttr; ///< 全道路リンク内属性
	};

	struct MaxItemId {
		int eBaseRoadNode; ///< 基本道路ノード番号
		int eAllRoadNode1; ///< 全道路ノード番号1
		int eAllRoadNode2; ///< 全道路ノード番号2
		int eSite; ///< 水系データアイテム番号
		int eAdminLocation; ///< 行政界位置データアイテム番号
		int eRailwayLocation; ///< 鉄道位置データアイテム番号
		int eFacil; ///< 施設等番号
		int ePlaceNameLocation; ///< 地名等表示位置データアイテム番号
		int eBeacon; ///< ビーコンアイテム番号
	};

// 構築・消滅
	DRMAAdmin2(const Format& cOriginalRecord);

// プロパティ
//	int eRecordId; ///< レコードID

	LayerCount eRecordCount; ///< データ別レコード数
	LayerCount eItemCount; ///< データ別アイテム件数

	char eCopyright[47]; ///< 著作権表示

	MaxItemId eMaxItemId; ///< 使用済み最大アイテム番号

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMIN2_H__8FE1A9AF_533A_46EA_A1B9_2EC70EE6B6A3__INCLUDED_)
