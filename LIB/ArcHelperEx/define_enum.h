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

/**
* @file define.h
*
* @brief SiNDY関連の固定値定義ファイル
*/
#ifndef __SINDY_ARCHELPER_DEFINE_ENUM
#define __SINDY_ARCHELPER_DEFINE_ENUM

#include "sindy/schema/road.h" 

/**
 * 道路ノード種別コード
 *
 * @note 道路ノードの種別と一致させる必要があります
 * @see Documents/SiNDY-b/data_model/SiNDY道路属性パラメータ.xls
 */
enum sindyeNodeClass
{
	sindyeNodeNoDef				= 0,	//!< 意味ナシノード
	sindyeNodeIntersection		= 1,	//!< 交差点ノード
	sindyeNodeEndPoint			= 2,	//!< 端点ノード
	sindyeNodeAttrChanged		= 3,	//!< 属性変化点ノード
	sindyeNodeExistAttr			= 4,	//!< 属性付きノード
	sindyeNodeConnectPecLink	= 5,	//!< PECリンク接続ノード
	sindyeNodeNQStartPoint		= 6,	//!< ノード列基点ノード
	sindyeHWYChargePoint		= 7,	//!< 料金所ノード
	sindyeRelHWY				= 8,	//!< ハイウェイ関係ノード
	sindyeOnMesh				= 9,	//!< 2次メッシュ区画辺上ノード
	sindyeCheckSameID			= 10,	//!< 同一リンクID防止ノード
	sindyePECNode				= 11,	//!< PECノード
	sindyeOnMeshPEC				= 12,	//!< 2次メッシュ区画辺上PECノード
	sindyeDammyNode				= 13,	//!< ダミー施設ノード（TF用）
};

/**
 * 歩行者ノード種別コード
 *
 * @note 歩行者ノードの種別と一致させる必要があります
 */
enum sindyeWalkNodeClass
{
	// ここのenumは使わずにschemaを使うこと
	sindyeWalkNodeCommon			= sindy::schema::walk_node::node_class::kPublic,	        //!< 一般ノード
	sindyeWalkNodeEndPoint			= sindy::schema::walk_node::node_class::kTerminal,	        //!< 端点ノード
	sindyeWalkNodeAttrChanged		= sindy::schema::walk_node::node_class::kAlter,	            //!< 属性遷移点ノード
	sindyeWalkNodeOnRoadNode		= sindy::schema::walk_node::node_class::kRoadNW,	        //!< 道路ＮＷ接続ノード
	sindyeWalkNodeStation			= sindy::schema::walk_node::node_class::kGate,	            //!< 駅出入口ノード
	sindyeWalkNodeUndergroundGate	= sindy::schema::walk_node::node_class::kUndergroundGate,	//!< 地下街出入口ノード
	sindyeWalkNodeElevetor			= sindy::schema::walk_node::node_class::kElevator,	        //!< エレベータノード
	sindyeWalkNodeOnMesh			= sindy::schema::walk_node::node_class::kOutline,	        //!< 図郭上ノード
};

/**
* @brief 編集モード
*/
enum sindyeEditMode
{
	sindyeNone        = 0,	//!< 通常モード
	sindyeViewOnly    = 1,	//!< 確認モード
	sindyeViewSearch  = 2,	//!< 確認検索モード
	sindyeFullControl = 3	//!< フルコントロールモード
};
/**
* @brief フィーチャ判定用
*/
enum sindyeFeatureKinds
{
	sindyeUnknownFeature = 0,	//!< 不明なフィーチャ
	sindyeNodeFeature    = 1,	//!< ノードフィーチャ
	sindyeRoadFeature    = 2,	//!< 道路フィーチャ
	sindyePointFeature   = 3,	//!< ポイントフィーチャ
	sindyeLineFeature    = 4,	//!< ラインフィーチャ
	sindyePolygonFeature = 5	//!< ポリゴンフィーチャ
};
/**
* @brief ヒットテスト用
*/
enum sindyeFeatureHitType
{
	sindyeHitNone        = 0,	//!< どこにもヒットしない
	sindyeHitEndPoint    = 1,	//!< 両端点のみにヒットした
	sindyeHitVertex      = 2,	//!< 両端点以外の頂点にヒットした
	sindyeHitBoundary    = 3	//!< 頂点を含まない点にヒットした
};
/**
* @brief スナップ用
*/
enum sindyeSnappingType
{
	sindyeSnapNone             = 0,	//!< なし
	sindyeSnapNode             = 1,	//!< ノードへのスナップ
	sindyeSnapRoad             = 2,	//!< 道路へのスナップ
	sindyeSnapRoadIntersection = 4,	//!< 立体交差点
	sindyeSnapAny              = 64	//!< それ以外の何か。
};

/**
* @brief 更新タイプ情報
*
* @note 「変更コード」ドメインと同じにすること
*/
enum sindyUpdateType
{
	sindyUpdateTypeNoUpdate = 0,	//!< 移行時既存
	sindyUpdateTypeCreate   = 1,	//!< 新規作成
	sindyUpdateTypeDelete   = 2,	//!< 削除
	sindyUpdateTypeShape    = 3,	//!< 形状変更
	sindyUpdateTypeProperty = 4,	//!< 属性変更
};

// 形状チェック結果レベル（強制力の強い順に並べること）
enum sindyeGeometryCheck
{
	sindyeGeometryNoCheck,			//!< チェックできない
	sindyeGeometryOK,				//!< OK
	sindyeGeometryWarnning,			//!< ワーニング
	sindyeGeometryError,			//!< エラー
};

// ジオメトリ比較演算用
enum sindyeSpatialRel
{
	sindyeSpatialRelNone		= 0,	//!< 関連性なし
	sindyeSpatialRelTouch		= 1,	//!< 接触
	sindyeSpatialRelOverlap		= 2,	//!< オーバーラップ
	sindyeSpatialRelCross		= 4,	//!< 交差
	sindyeSpatialRelWithin		= 8,	//!< 含まれる
	sindyeSpatialRelContain		= 16,	//!< 含んでいる
	sindyeSpatialRelDisjoint	= 32,	//!< 分離
	sindyeSpatialRelEqual		= 64,	//!< 等しい
};

#endif
