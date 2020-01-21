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

// KKDAdmin.h: KKDAdmin クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_)
#define AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "KKDRecord.h"

/**
 * @brief 管理データフォーマットクラス
 */
struct KKDAdminFormat
{
// 定義
	struct LayerDate {
		typedef _DRMADec Date[6];
		Date eRegulationRoadNode;
		Date eRegulationRoadLink;
		Date eLaneRegulationRoadNode;
		Date eLaneRegulationRoadLink;
		Date eLaneRegulationIntersectionNode;
	};

	struct LayerCount {
		typedef _DRMADec Count[5];
		Count eRegulationRoadNode;
		Count eRegulationRoadLink;
		Count eLaneRegulationRoadNode;
		Count eLaneRegulationRoadLink;
		Count eLaneRegulationIntersectionNode;
	
	};

	struct MaxItemId {
		typedef _DRMAHex Id[5];
		Id eBaseRoadNode;
		Id eAllRoadNode1;
		Id eAllRoadNode2;
		Id eOtherNode1;
		Id eOtherNode2;
	};

// フォーマット
	_DRMADec eRecordId[2];
	_DRMADec eMeshCode[6];
	_DRMADec eEditionNo[4];
	
	_DRMADec eDrmaEditionNo[4];

	LayerDate eUpdateDate;
	LayerDate eModDate;
	LayerCount eRecordCount;
	LayerCount eItemCount;

	_DRMAKanji eCopyright[25];

	MaxItemId eMaxItemId;

	_DRMAHex eFiller1[55];
};

/**
 * @brief 管理データレコードクラス
 */
class KKDAdmin : public KKDRecordT<KKDAdminFormat>
{
public:
// 定義
	struct LayerDate {
		typedef int Date;
		Date eRegulationRoadNode; ///< 交通規制道路ノードデータ
		Date eRegulationRoadLink; ///< 交通規制道路リンクデータ
		Date eLaneRegulationRoadNode; ///< レーン規制道路ノードデータ
		Date eLaneRegulationRoadLink; ///< レーン規制道路リンクデータ
		Date eLaneRegulationIntersectionNode; ///< レーン規制集合交差点ノードデータ
	};

	struct LayerCount {
		typedef int Count;
		Count eRegulationRoadNode; ///< 交通規制道路ノードデータ
		Count eRegulationRoadLink; ///< 交通規制道路リンクデータ
		Count eLaneRegulationRoadNode; ///< レーン規制道路ノードデータ
		Count eLaneRegulationRoadLink; ///< レーン規制道路リンクデータ
		Count eLaneRegulationIntersectionNode; ///< レーン規制集合交差点ノードデータ
	
	};

	struct MaxItemId {
		typedef int Id;
		Id eBaseRoadNode; ///< 基本道路ノード番号
		Id eAllRoadNode1; ///< 全道路ノード番号(1)
		Id eAllRoadNode2; ///< 全道路ノード番号(2)
		Id eOtherNode1; ///< その他ノード番号(1)
		Id eOtherNode2; ///< その他ノード番号(2)
	};
// 構築/消滅
	KKDAdmin(const Format& cOriginalRecord);


// フォーマット
//	int eRecordId; ///< レコードID
	int eMeshCode; ///< ２次メッシュコード
	int eEditionNo; ///< 版番号
	
	int eDrmaEditionNo; ///< 使用DRMAデータベース版番号

	LayerDate eUpdateDate; ///< データ別基準更新年月日
	LayerDate eModDate; ///< データ別修正年月日
	LayerCount eRecordCount; ///< データ別レコード数
	LayerCount eItemCount; ///< データ別アイテム件数

	char eCopyright[51]; ///< 著作権表示

	MaxItemId eMaxItemId; ///< 使用済み最大アイテム番号

// 静的プロパティ
	static const int sRecordId;
};

#endif // !defined(AFX_KKDADMIN_H__1BB20BF4_F126_41E7_BBDA_EE7FFA7BA1D9__INCLUDED_)
