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

// DRMAAdmin1.h: DRMAAdmin1 クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_)
#define AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DRMARecord.h"

/**
 * @brief 管理データ（その１）フォーマットクラス
 */
struct DRMAAdmin1Format
{
// 定義
	struct LayerDate {
		typedef _DRMADec Code[6];

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

// フォーマット
	_DRMADec eRecordId[2];
	_DRMADec eMeshCode[6];
	_DRMADec eEditionNo[4];
	
	_DRMADec eScale[5];
	_DRMAKanji eMeshName[10];
	_DRMADec eRevisionYear[2];
	_DRMADec eModYear[2];

	_DRMADec eBias[3];
	
	_DRMADec eBottomDistance[5];
	_DRMADec eRightDistance[5];
	_DRMADec eTopDistance[5];
	_DRMADec eLeftDistance[5];

	LayerDate eUpdateDate;

	_DRMAHex eFiller1[12];

	LayerDate eModDate;

	_DRMADec eLastModDate[6];

	_DRMAHex eFiller2[6];
};

/**
 * @brief 管理データ（その１）レコードクラス
 */
class DRMAAdmin1 : public DRMARecordT<DRMAAdmin1Format> {
public:
// 定義
	struct LayerDate {
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

// 構築/消滅
	DRMAAdmin1(const Format& cOriginalRecord);

// プロパティ
//	int eRecordId; ///< レコードID
	int eMeshCode; ///< ２次メッシュコード
	int eEditionNo; ///< 版番号
	
	int eScale; ///< 縮尺
	char eMeshName[21]; ///< 図名
	int eRevisionYear; ///< 改測年度
	int eModYear; ///< 修正年度

	int eBias; ///< 地磁気偏角（西偏）
	
	int eBottomDistance; ///< 区画下辺
	int eRightDistance; ///< 区画右辺
	int eTopDistance; ///< 区画上辺
	int eLeftDistance; ///< 区画左辺

	LayerDate eUpdateDate; 	///< データ別基準・更新年月日
	LayerDate eModDate;	///< データ別修正年月日

	int eLastModDate; ///< 最終修正年月日

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_DRMAADMIN1_H__BC8C113C_F4C7_4D9B_B269_0B095BC3F781__INCLUDED_)
