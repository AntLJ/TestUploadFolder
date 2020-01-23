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
#include "DatabaseControl.h"
#include "Properties.h"

class CCheckWalk;
class PedxingCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param [in] out                       ログ出力管理クラスの参照
	 * @param [in] modufyFilter              編集履歴フィルタ
	 * @param [in] dataType                  チェック対象のデータタイプ
	 * @param [in] sindyTolerance_threshold  SiNDY許容値(m)
	 */
	PedxingCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, sindy::schema::walk_type::type_class::ECode dataType
		, double sindyTolerance_threshold):
		m_output( out )
		, m_modifyFilter(modifyFilter)
		, m_dataType(dataType)
		, m_sindyTolerance_threshold(sindyTolerance_threshold)
	{}

	/**
	 * @brief 歩行者リンクの全てのチェックを実行
	 * @param [in] meshcode            処理中の都市地図メッシュコード
	 * @param [in] ipMeshGeo           都市地図メッシュ形状
	 * @param [in] pedxing             横断ポイント群クラス
	 * @param [in] walkLink            歩行者リンク群クラス
	 * @param [in] walkNode            歩行者ノード群クラス
	 * @param [in] roadLink            道路リンク群クラス
	 * @param [in] roadNode            道路ノード群クラス
	 * @param [in] scrambleArea        スクランブルエリア群クラス
	 * @param [in] heightNode          重なりリンクの高さノード群クラス
	 * @param [in] pedxingRTree        横断ポイントRTree
	 * @param [in] walkLinkRTree       歩行者リンクRTree
	 * @param [in] walkNodeRTree       歩行者ノードRTree
	 * @param [in] roadLinkRTree       道路リンクRTree
	 * @param [in] roadNodeRTree       道路ノードRTree	 
	 * @param [in] scrambleAreaRTree   スクランブルエリアRTree
	 * @param [in] heightNodeRTree     重なりリンクの高さノードのRTree
	 * @param [in] checktarget         チェック対象
	 */
	void check( 
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& pedxing,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& heightNode,
		const BGRTree& pedxingRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,		
		const BGRTree& scrambleAreaRTree,
		const BGRTree& heightNodeRTree,
		properties::check_target checktarget
		) const ;	

	/**
	 * @brief 例外モード用のチェック
	 * @param [in] pedxing             横断ポイント群クラス
	 */
	void checkException(
		const FeaturesPack& pedxing
		);

protected:
		
	/**
	 * @brief 横断ポイント形状が不正であるかチェック
	 * @note エラーコード:138
	 * @param [in] ipTable  横断ポイントテーブル
	 */
	void checkPedxingGeometry( const ITablePtr& ipTable ) const;

	/**
	 * @brief 横断ポイントがスクランブルエリア内にあるかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] scrambleArea       スクランブルエリア群クラス
	 * @param [in] scrambleAreaRTree  スクランブルエリアRTree
	 */
	void checkPedxingOnScrambleArea(
		const GeoClass& pedxingObject,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree	
	) const;

	/**
	 * @brief 横断ポイントが道路リンク上に無いかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] roadLink           道路リンク群クラス
	 * @param [in] roadLinkRTree      道路リンクRTree
	 */
	void checkPedxingOnRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree	
		) const;

	/**
	 * @brief	交差箇所のポイントを取得する。
	 * @param	[in] baseGeoClass	基準のGeoClass
	 * @param   [in] crossObjects   交差なるポリゴンのリスト
	 * @retval true  交差箇所の距離はSiNDY許容値より小きいのポイントが存在する
	 * @retval false 交差箇所の距離はSiNDY許容値より小きいのポイントは存在しない
	 */
	bool getCrossPoint(
		const GeoClass& baseGeoClass,
		const std::vector<GeoClass>& crossObjects
		) const;

	/**
	 * @brief 横断ポイントが道路リンク交差箇所にあるかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] roadLink           道路リンク群クラス
	 * @param [in] roadLinkRTree      道路リンクRTree
	 * @param [in] roadNodeMap        道路ノード群情報
	 * @param [in] roadNodeRTree      道路ノードRTree
	 */
	void checkPedxingCrossRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree,
		const std::map<long, GeoClass>& roadNodeMap,
		const BGRTree& roadNodeRTree
		) const;

	/**
	* @brief 横断ポイントに重なるノードを検索
	* @param pedxingObject [in] チェック対象の横断ポイントの情報
	* @param nodeMap       [in] ノードの情報
	* @param nodeRTree     [in] ノードのRTree
	* @retval true  歩行者ノードに重なるノードが存在する
	* @retval false 歩行者ノードに重なるノードは存在しない
	*/
	bool findOverlapNode(
		const GeoClass& pedxingObject,
		const std::map<long, GeoClass>& nodeMap,
		const BGRTree& nodeRTree
		) const;

	/**
	 * @brief 横断ポイントが歩行者リンク交差箇所にあるかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] walkLink           歩行者リンク群クラス
	 * @param [in] walkLinkRTree      歩行者リンクRTree
	 * @param [in] walkNodeMap        歩行者ノード群情報
	 * @param [in] walkNodeRTree      歩行者ノードRTree
	 */
	void checkPedxingCrossWalkLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		const std::map<long, GeoClass>& walkNodeMap,
		const BGRTree& walkNodeRTree
		) const;

	/**
	 * @brief 横断ポイントに関係ない歩行者リンクが存在するかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] walkLink           歩行者リンク群クラス
	 * @param [in] walkLinkRTree      歩行者リンクRTree
	 * @param [in] pedxingClass       歩行者ノード種別
	 */
	void checkPedxingWithWalkLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		long  pedxingClass
		) const;

	/**
	 * @brief 横断タイプコードが不正な値かチェック
	 * @param [in] pedxingClass       歩行者ノード種別
	 */
	void checkPedxingClass(
		long  pedxingClass
		) const;

	/**
	 * @brief ポイント同士（PEDXING、HEIGHT_NODE）の距離が近いかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] pedxing            横断ポイント群クラス
	 * @param [in] heightNode         重なりリンクの高さノード群クラス
	 * @param [in] pedxingRTree       横断ポイントRTree
	 * @param [in] heightNodeRTree    重なりリンクの高さノードのRTree
	 */
	void checkNearPoint(
		const GeoClass& pedxingObject,
		const FeaturesPack& pedxing,	
		const FeaturesPack& heightNode,
		const BGRTree& pedxingRTree,
		const BGRTree& heightNodeRTree
		) const;

	/**
	 * @brief 意味なしPEDXINGかチェック
	 * @param [in] pedxingObject      チェック対象の 横断ポイント
	 * @param [in] roadLink           道路リンク群クラス
	 * @param [in] walkLink           歩行者リンク群クラス
	 * @param [in] roadLinkRTree      道路リンクRTree
	 * @param [in] walkLinkRTree      歩行者リンクRTree
	 */
	void checkCrossRoadLink(
		const GeoClass& pedxingObject,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const BGRTree& roadLinkRTree,
		const BGRTree& walkLinkRTree
		) const;

protected:
	COutPut& m_output;                                                   //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter;                                          //!< 編集履歴フィルタ
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< データタイプ
	double	m_sindyTolerance_threshold;                                  //!< SiNDY許容値(m)
};
