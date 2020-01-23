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

#pragma once
#include "util.h"
#include "OutputLog.h"
#include "HeightInfoMgr.h"
#include "Properties.h"

class HeightNodeCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out            [in] ログ出力管理クラスの参照
	 * @param dist_threshold [in] 微小距離閾値(m)
	 * @param coordinateTolerance [in] DB許容値(度)
	 * @param heightNodeMgr  [in] 重なりリンク高さノード設定管理クラス
	 * @param dataType       [in] データタイプ
	 */
	HeightNodeCheckFunction(COutPut& out, double dist_threshold, double coordinateTolerance, const HeightInfoMgr& heightNodeMgr, sindy::schema::walk_type::type_class::ECode dataType) :
		m_output(out)
		, m_dist_threshold(dist_threshold)
		, m_coordinateTolerance(coordinateTolerance)
		, m_heightNodeMgr(heightNodeMgr)
		, m_dataType(dataType)
	{}

	/**
	 * @brief 重なりリンク高さノードの全チェック
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param segmentAttr     [in] リンク内属性群クラス
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param heightNode      [in] 重なりリンクの高さノード群クラス
	 * @param nodeRTree       [in] 道路ノードのRTree
	 * @param segmentAttrRTree[in] リンク内属性のRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 * @param heightNodeRTree [in] 重なりリンクの高さノードのRTree
	 * @param checktarget     [in] チェック対象
	 * @param isRelease		  [in] Releaseモード(デフォルト:false)
	 */
	void checkHeightNode(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& segmentAttr,
		const FeaturesPack& walkLink,
		const FeaturesPack& heightNode,
		const BGRTree& nodeRTree,
		const BGRTree& segmentAttrRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget,
		bool isRelease = false		
		);

	/**
	 * @brief releaseモード用の全チェック
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param segmentAttr     [in] リンク内属性群クラス
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param heightNode      [in] 重なりリンクの高さノード群クラス
	 * @param roadNodeRTree   [in] 道路ノードのRTree
	 * @param segmentAttrRTree[in] リンク内属性のRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 * @param heightNodeRTree [in] 重なりリンクの高さノードのRTree
	 * @param checktarget     [in] チェック対象
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& segmentAttr,
		const FeaturesPack& walkLink,
		const FeaturesPack& heightNode,
		const BGRTree& roadNodeRTree,
		const BGRTree& segmentAttrRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget
	)
	{
		checkHeightNode( ipMeshGeo, roadLink, segmentAttr, walkLink, heightNode,
			roadNodeRTree, segmentAttrRTree, walkNodeRTree, heightNodeRTree, checktarget, true );
	}

	/**
	 * @brief otherモード用の全チェック
	 * @param [in] ipMeshGeo       都市地図メッシュ形状
	 * @param [in] heightNode 重なりリンクの高さノード群クラス
	 */
	void checkOther(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& heightNode
	) const;

	/**
	 * @brief 例外モード用のチェック
	 * @param [in] heightNode 重なりリンクの高さノード群クラス
	 */
	void checkException(
		const FeaturesPack& heightNode
	);

protected:
	/**
	 * @brief	重なりリンク高さノードの情報
	 */
	typedef struct height_node_info {
		long linkID;
		long linkIDHeight;
		long linkIDTable;
		height_node_info() {
			linkID = 0;
			linkIDHeight = 0;
			linkIDTable = 0;
		};
		bool operator==(const height_node_info& cmp) {
			if (linkID == cmp.linkID && linkIDHeight == cmp.linkIDHeight && linkIDTable == cmp.linkIDTable)
				return true;
			return false;
		};
	}HEIGHT_NODE_INFO;

	/**
	 * @brief HEIGHT_NODEの形状が不正かチェック
	 * @note エラーコード:1017
	 * @param [in] ipTable  HEIGHT_NODEテーブル
	 */
	void checkHeightNodeGeometry(const ITablePtr& ipTable) const;

	/**
	 * @brief リンクIDが不正な値かチェック
	 * @param [in] link1ID       リンク1のID
	 * @param [in] link2ID       リンク2のID
	 */
	void checkLinkID(
		long  link1ID,
		long  link2ID
	) const;

	/**
	 * @brief ポイント同士HEIGHT_NODEの距離が近いかチェック
	 * @param [in] heightNodeObject   チェック対象の 横断ポイント
	 * @param [in] heightNode         重なりリンクの高さノード群クラス
	 * @param [in] heightNodeRTree    重なりリンクの高さノードのRTree
	 */
	void checkNearPoint(
		const GeoClass& heightNodeObject,
		const FeaturesPack& heightNode,
		const BGRTree& heightNodeRTree
	) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	double m_dist_threshold; //!< 微小距離閾値(m)
	double m_coordinateTolerance; //!< DB許容値(度)
	const HeightInfoMgr& m_heightNodeMgr; //!< 重なりリンク高さノード設定管理クラス
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};
