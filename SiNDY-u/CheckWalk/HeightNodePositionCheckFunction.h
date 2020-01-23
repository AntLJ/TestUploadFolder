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

/**
 * @brief HEIGT_NODEが作成されているべき箇所に存在するかどうかチェックする
 * @detail このチェックだけで、1都市地図メッシュの処理時間が1分半ほどかかる。
 * 分離し易いように一クラスとして定義。
 */
class HeightNodePositionCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out           [in] ログ出力管理クラスの参照
	 * @param heightNodeMgr [in] 重なりリンク高さノード設定管理クラス
	 * @param coordinateTolerance [in] DB許容値(度)
	 */
	HeightNodePositionCheckFunction(
		COutPut& out,
		const HeightInfoMgr& heightNodeMgr,
		double coordinateTolerance ) :
		m_output( out ),
		m_heightNodeMgr( heightNodeMgr ),
		m_coordinateTolerance( coordinateTolerance )

	{}

	/**
	 * @brief	HEIGHT_NODEの位置をチェック
	 * @param	meshcode    [in] チェック対象メッシュコード
	 * @param	meshGeom    [in] チェック対象のジオメトリ
	 * @param	roadLink    [in] チェック参照用道路リンク群クラス
	 * @param	walkLink    [in] チェック参照用歩行者リンク群クラス
	 * @param	walkNode    [in] チェック参照用歩行者ノード群クラス
	 * @param	heightNode      [in] チェック参照用重なりリンクの高さノード群クラス
	 * @param	heightNodeRTree [in] チェック参照用重なりリンクの高さノードRTree
	 */
	void checkPosition(
		const CString& meshcode,
		const IGeometryPtr& meshGeom,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& heightNode,
		const BGRTree& heightNodeRTree
		);

protected:
	/**
	 * @brief  処理のタイプ
	 */
	enum TYPE : unsigned
	{
		ROAD = 0,	//!< 道路
		WALK = 1,	//!< 歩行者
	};

	/**
	 * @brief  対象のリンクがHEIGHT_NODE作成可能な属性か
	 * @param  geoClass [in] リンクの属性・形状
	 * @param  type     [in] 歩行者 or 道路か
	 * @return 作成の可否
	 */
	bool isCreatableAttribute(
		const GeoClass& geoClass,
		const sindy::CFieldMap& fieldMap,
		TYPE type
	);

	/**
	 * @brief 二つの形状の交点を算出し、
	 * その位置にHEIGHT_NODE、および、WALK_NODEが存在しないならエラーとする
	 * @param base            [in] 形状1
	 * @param target          [in] 形状2
	 * @param walkNodeUnion   [in] WALK_NODEの形状をUnionした形状（マルチポイント）
	 * @param heightNodeUnion [in] HEIGHT_NODEの形状をUnionした形状（マルチポイント）
	 * @param heightNodeRTree [in] HEIGHT_NODEのRTree
	 * @param meshcode        [in] チェック対象メッシュコード
	 * @param meshGeom        [in] チェック対象のジオメトリ
	 */
	void checkCrossPoint(
		const IGeometryPtr& base,
		const IGeometryPtr& target,
		const IGeometryPtr& walkNodeUnion,
		const IGeometryPtr& heightNodeUnion, 
		const BGRTree& heightNodeRTree,
		const CString& meshcode,
		const IGeometryPtr& meshGeom
		);

protected:
	COutPut& m_output;						//!< ログ出力用インスタンスへの参照
	const HeightInfoMgr& m_heightNodeMgr;	//!< 重なりリンク高さノード設定管理クラス
	double m_coordinateTolerance;			//!< DB許容値(度)
	ISpatialReferencePtr m_ipSP;			//!< 空間参照
};
