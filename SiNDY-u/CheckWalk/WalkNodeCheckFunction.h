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
#include "Properties.h"

class WalkNodeCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out                [in] ログ出力管理クラスの参照
	 * @param modufyFilter       [in] 編集履歴フィルタ
	 * @param walkLinkClassMap   [in] 歩行者種別のコード値ドメインマップ
	 * @param walkNodeClassMap   [in] ノード種別のコード値ドメインマップ
	 * @param dist_threshold     [in] 微小距離閾値(m)
	 * @param neighbor_threshold [in] 近傍距離閾値(m)
	 * @param refer_id_threshold [in] 関連ID距離閾値(m)
	 * @param dataType           [in] データタイプ
	 * @param coordinateTolerance [in] 座標許容値
	 */
	WalkNodeCheckFunction(COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkLinkClassMap
		, const std::map<long, CString>& walkNodeClassMap
		, double dist_threshold
		, double neighbor_threshold
		, double refer_id_threshold
		, sindy::schema::walk_type::type_class::ECode dataType
		, double coordinateTolerance
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkLinkClassMap(walkLinkClassMap)
		, m_walkNodeClassMap(walkNodeClassMap)
		, m_dist_threshold(dist_threshold)
		, m_neighbor_threshold(neighbor_threshold)
		, m_refer_id_threshold(refer_id_threshold)
		, m_dataType(dataType)
		, m_coordinateTolerance(coordinateTolerance)
	{}

	/**
	 * @brief 歩行者ノード用の全チェック
	 * @param meshcode        [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param linkMap         [in] 道路リンクの属性・形状を格納したマップ
	 * @param nodeMap         [in] 道路ノードの属性・形状を格納したマップ
	 * @param walkLinkMap     [in] 歩行者リンクの属性・形状を格納したマップ
	 * @param walkNodeMap     [in] 歩行者ノードの属性・形状を格納したマップ
	 * @param gateMap         [in] 駅出入口の属性・形状を格納したマップ
	 * @param poiPointMap     [in] 地下街出入口の属性・形状を格納したマップ
	 * @param scrambleArea    [in] スクランブルエリアの属性・形状を格納したマップ
	 * @param heightNode      [in] 重なりリンクの高さノードの属性・形状を格納したマップ
	 * @param linkRTree       [in] 道路リンクのRTree
	 * @param nodeRTree       [in] 道路ノードのRTree
	 * @param walkLinkRTree   [in] 歩行者リンクのRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 * @param scrambleAreaRTree   [in] スクランブルエリアのRTree
	 * @param heightNodeRTree [in] 重なりリンクの高さノードのRTree
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param baseMeshes      [in] ベースメッシュ<メッシュコード,形状>
	 * @param bOther          [in] otherモードとして実行するか否か
	 * (ohterモードで実行すると、特定のチェックが実行されなくなる。
	 * 詳細は開発文書を参照のこと)
	 */
	void CheckWalkNode(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& stationGate,
		const FeaturesPack& poiPoint,
		const FeaturesPack& scrambleArea,
		const FeaturesPack& heightNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& scrambleAreaRTree,
		const BGRTree& heightNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		const std::map<CString, IGeometryPtr>& baseMeshes,
		bool bOther
	);

	/**
	 * @brief walkableモード用のチェック
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param roadNode        [in] 道路ノード群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param linkRTree       [in] 道路リンクのRTree
	 * @param nodeRTree       [in] 道路ノードのRTree
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void checkWalkable(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief releaseモード用の全チェック
	 * @param meshcode        [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param roadNode        [in] 道路ノード群クラス
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param stationGate     [in] 駅出入口群クラス
	 * @param poiPoint        [in] 地下街出入口群クラス
	 * @param heightNode      [in] 重なりリンクの高さノードの群クラス
	 * @param linkRTree       [in] 道路リンクのRTree
	 * @param nodeRTree       [in] 道路ノードのRTree
	 * @param walkLinkRTree   [in] 歩行者リンクのRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 * @param heightNodeRTree [in] 重なりリンクの高さノードのRTree
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param baseMeshes      [in] ベースメッシュ<メッシュコード,形状>
	 */
	void checkRelease(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadlink,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& stationGate,
		const FeaturesPack& poiPoint,
		const FeaturesPack& heightNode,
		const BGRTree& linkRTree,
		const BGRTree& nodeRTree,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		const std::map<CString, IGeometryPtr>& baseMeshes
	);

protected:
	enum CheckType {
		All,
		Other,
		Release
	};
	/**
	 * @brief ノード形状が不正であるか否か
	 * @param targetNode [in] チェック対象のノード形状
	 * @param walkNodeX [out] ノードの経度
	 * @param walkNodeY [out] ノードの緯度
	 * @retval true  形状が不正である
	 * @retval false 形状が不正ではない
	 */
	bool isIllegalShape(
		const IGeometryPtr& targetNode,
		double& walkNodeX,
		double& walkNodeY
	);

	/**
	 * @brief ノード形状が不正であるかチェック
	 * @note エラーコード:102
	 * @param walkNodeObj [in] チェック対象のノードオブジェクト
	 * @param walkNodeX   [out] ノードの経度
	 * @param walkNodeY   [out] ノードの緯度
	 */
	void checkNodeGeometry(
		const GeoClass& walkNodeObj,
		double& walkNodeX,
		double& walkNodeY);

	/**
	 * @brief ノードがメッシュの四隅にないかチェック
	 * @note エラーコード:103
	 * @param walkNodeX   [in] チェック対象のノードの経度
	 * @param walkNodeY   [in] チェック対象のノードの緯度
	 */
	void checkBorderCorner(
		double walkNodeX,
		double walkNodeY);

	/**
	 * @brief メッシュ境界上に存在するノードの種別が正しいかチェック
	 * @note エラーコード:104,105
	 * @param walkNodeObj   [in] チェック対象のノードオブジェクト
	 , @param walkNodeClass [in] チェック対象のノード種別
	 * @param basePolyline  [in] ベースメッシュの枠線
	 */
	void checkBorderLine(
		const GeoClass& walkNodeObj,
		long walkNodeClass,
		const IPolylinePtr& basePolyline);

	/**
	 * @brief 道路ノード上にある歩行者リンクの種別が道路NW接続ノードになっているかチェック
	 * @note エラーコード:107
	 * @param walkNodeObj   [in] チェック対象のノードオブジェクト
	 , @param walkNodeClass [in] チェック対象のノード種別
	 * @param roadNode      [in] 道路ノード群クラス
	 * @param roadNodeRTree [in] 道路ノードのRTree
	 */
	void checkRoadNWConnectNode(
		const GeoClass& walkNodeObj,
		long walkNodeClass,
		const FeaturesPack& roadNode,
		const BGRTree& roadNodeRTree);

	/**
	 * @brief 歩行者ノードが歩行者リンクに接続しているかチェック
	 * @note エラーコード:106
	 * @param walkNodeObj   [in] チェック対象のノードオブジェクト
	 * @param walkLink      [in] 歩行者リンク群クラス
	 * @param walkLinkRTree [in] 歩行者リンクのRTree
	 */
	void checkWalkLinkConnectNode(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree);

	/**
	 * @brief リンクとの接続状態に関わるチェック全般
	 * @param walkNodeObj	  [in] チェック対象のノードオブジェクト
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNodeClass   [in] チェック対象のノード種別
	 * @param roadNodeID      [in] チェック対象の道路ノード対応ID
	 * @param walkClassIndex  [in] WalkClassのフィールドインデックス
	 * @param walkNodeClassIndex [in] 歩行者ノード種別インデックス
	 * @param fromNodeIdIndex [in] 始点ノードOIDインデックス
	 * @param toNodeIdIndex	  [in] 終点ノードOIDインデックス
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param roadLinkRTree   [in] 道路リンクのRTree
	 * @param roadNodeRTree   [in] 道路ノードのRTree
	 * @param checkType       [in] チェックモード
	 * (モードによって、特定のチェックが実行されなくなる。
	 */
	void checkConenctLink(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkNode,
		const FeaturesPack& walkLink,
		long walkNodeClass,
		long roadNodeID,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const NODEIDMAP& walkNodeLinkIDs,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		CheckType checkType);


	/**
	 * @brief 端点ノードが他のリンクに接触していないかチェック
	 * @note エラーコード:111
	 * @param walkNodeObj	  [in] チェック対象のノードオブジェクト
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkLinkRTree   [in] 歩行者リンクのRTree
	 * @param walkNodeClass   [in] チェック対象のノード種別
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void checkTouchOtherLink(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		long walkNodeClass,
		const NODEIDMAP& walkNodeLinkIDs);

	/**
	 * @brief ノードが他のノードに重なるかチェック
	 * @note エラーコード:112
	 * @param walkNodeObj	  [in] チェック対象のノードオブジェクト
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 * @param walkNodeX   [in] チェック対象のノードの経度
	 * @param walkNodeY   [in] チェック対象のノードの緯度
	 */
	void checkTouchOtherNode(
		const GeoClass& walkNodeObj,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree,
		double walkNodeX,
		double walkNodeY);

	/**
	 * @brief 歩行者ノードに重なる道路ノードを検索
	 * @param roadNodeObj   [in] チェック対象の道路ノードの情報
	 * @param walkNodeObj   [in] チェック対象の歩行者ノードの情報
	 * @param roadNodeMap   [in] 道路ノードの情報
	 * @param roadNodeRTree [in] 道路ノードのRTree
	 * @retval true  歩行者ノードに重なる道路ノードが存在する
	 * @retval false 歩行者ノードに重なる道路ノードは存在しない
	 */
	bool findOverlapRoadNode(
		GeoClass& roadNodeObj
		, const GeoClass& walkNodeObj
		, const std::map<long, GeoClass>& roadNodeMap
		, const BGRTree& roadNodeRTree
		, double range = 0.1
	) const;

	/**
	 * @brief 歩行可能種別に関わるチェック
	 * @param roadNodeObj        [in] チェック対象の道路ノードの情報
	 * @param connectWalkLinkCnt [in] 歩行者ノードに接続しているリンクの数
	 * @param roadLink           [in] 道路リンク群クラス
	 * @param roadLinkRTree      [in] 道路ノードのRTree
	 */
	bool checkWalkableConnect(
		const GeoClass& roadNodeObj
		, long connectWalkLinkCnt
		, const sindy::CFieldMap& fieldMap
		, const FeaturesPack& roadLink
		, const BGRTree& roadLinkRTree
	);

	/**
	 * @brief 歩行者ノードの関連ポイントチェック
	 * @param walkNode   [in] チェック対象の歩行者ノード
	 * @param fieldMap　 [in] 歩行者ノードのフィールドマップ
	 * @param checkField [in] チェック対象のフィールド名
	 * @param point      [in] チェック対象群クラス
	 * @param bRelase    [in] relaseモード用のチェックをするか否か
	 */
	void CheckWalkNodeWithPoint(
		const GeoClass& walkNode,
		const sindy::CFieldMap& fieldMap,
		const CString& checkField,
		const FeaturesPack& point,
		bool bRelease
	);

	/**
	* @brief 道路ノードとの整合性チェック
	* @note	 エラーコード:113,116,117,120,121
	* @param walkNodeClass		[in] 歩行者ノード属性
	* @param roadNodeID　		[in] 道路ノードOID
	* @param nodeNameIndex		[in] 道路ノードの交差点名称インデックス
	* @param walkNodeRoadIndex	[in] 歩行者ノードの道路ノード対応IDインデックス
	* @param walkNodeObj		[in] 歩行者ノードオブジェクト
	* @param roadNode			[in] 道路ノード群クラス
	* @param walkNode			[in] 歩行者リンク群クラス
	* @param walkNodeRTree		[in] 歩行者ノードのRTree
	* @param bRelase			[in] relaseモード用のチェックをするか否か
	*/
	void compairRoadNodeCheck(
		long walkNodeClass,
		long roadNodeID,
		long nodeNameIndex,
		long walkNodeRoadIndex,
		const GeoClass& walkNodeObj,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree,
		bool isForRelease
	);

	/**
	* @brief 改札内に改札外リンクがないかチェック
	* @note	 エラーコード:132,133,134
	* @param walkLink			[in] 歩行者リンク群クラス
	* @param walkNode			[in] 歩行者ノード群クラス
	* @param walkClassIndex		[in] 歩行者属性のインデックス
	* @param walkNodeClassIndex	[in] 歩行者ノード属性のインデックス
	* @param fromNodeIdIndex	[in] 始点ノードOIDインデックス
	* @param toNodeIdIndex		[in] 終点ノードOIDインデックス
	* @param links			    [in] 対象ノードに接続しているリンクリスト
	* @param walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	*/
	void internalTicketGateConenctCheck(
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const std::set<long>& links,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	* @brief リンクが道路ノードに接続しているか（再帰）
	* @param linkID			    [in] チェック対象の歩行者リンクのOID
	* @param walkClassIndex		[in] 歩行者属性のインデックス
	* @param walkNodeClassIndex	[in] 歩行者ノード属性のインデックス
	* @param fromNodeIdIndex	[in] 始点ノードOIDインデックス
	* @param toNodeIdIndex		[in] 終点ノードOIDインデックス
	* @param walkLink			[in] 歩行者リンク群クラス
	* @param walkNode			[in] 歩行者ノード群クラス
	* @param walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param inspectedIDs		[in] 検査済み歩行者リンクのOIDリスト
	* @return	true	道路ノードに接続している
	* @return	false	道路ノードに接続していない
	*/
	bool isConnectedRoadNode(
		long linkID,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	* @brief ノードに接続しているリンクが道路ノードに接続しているか（再帰）
	* @param walkNodeObj		[in] チェック対象の歩行者ノードオブジェクト
	* @param walkClassIndex		[in] 歩行者属性のインデックス
	* @param walkNodeClassIndex	[in] 歩行者ノード属性のインデックス
	* @param fromNodeIdIndex	[in] 始点ノードOIDインデックス
	* @param toNodeIdIndex		[in] 終点ノードOIDインデックス
	* @param walkLink			[in] 歩行者リンク群クラス
	* @param walkNode			[in] 歩行者ノード群クラス
	* @param walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param inspectedIDs		[in] 検査済み歩行者リンクのOIDリスト
	* @return	true	道路ノードに接続している
	* @return	false	道路ノードに接続していない
	*/
	bool isConnectedRoadNodeAtNode(
		const GeoClass& walkNodeObj,
		long walkClassIndex,
		long walkNodeClassIndex,
		long fromNodeIdIndex,
		long toNodeIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	 * @brief 道路リンク上にあるのに道路リンク接続ノードになっているかチェック
	 * @note エラーコード:135
	 * @param [in] walkNodeObject チェック対象のノードオブジェクト
	 , @param [in] walkNodeClass  チェック対象のノード種別
	 * @param [in] roadLink       道路リンク群クラス
	 * @param [in] roadLinkRTree  道路リンクのRTree
	 */
	void checkRoadLinkConnectNode(
		const GeoClass& walkNodeObject,
		long walkNodeClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree) const;

	/**
	 * @brief 道路リンク接続ノードが道路リンク交差箇所にあるかチェック
	 * @note エラーコード:137
	 * @param [in] walkNodeObject チェック対象の歩行者ノードオブジェクト
	 , @param [in] walkNodeClass  チェック対象の歩行者ノード種別
	 * @param [in] roadLink       道路リンク群クラス
	 * @param [in] roadLinkRTree  道路リンクのRTree
	 * @param [in] roadNodeMap    道路ノードの情報
	 * @param [in] roadNodeRTree  道路ノードのRTree
	 */
	void checkRoadLinkCrossNode(
		const GeoClass& walkNodeObject,
		long walkNodeClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree,
		const std::map<long, GeoClass>& roadNodeMap,
		const BGRTree& roadNodeRTree) const;

	/**
	 * @brief スクランブル交差点なのにスクランブルエリアポリゴンが無いかチェック
	 * @param [in] walkNodeObject     チェック対象の歩行者ノード
	 * @param [in] walkLink           歩行者リンク群クラス
	 * @param [in] walkLinkRTree      歩行者リンクRTree
	 * @param [in] scrambleArea       スクランブルエリア群クラス
	 * @param [in] scrambleAreaRTree  スクランブルエリアRTree
	 */
	void checkWalkNodeCrossWalkLink(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree
	) const;

	/**
	 * @brief ポイント同士（WALK_NODE、HEIGHT_NODE）の距離が近いかチェック
	 * @param [in] walkNodeObject     チェック対象の 横断ポイント
	 * @param [in] walkNode           歩行者ノード群クラス
	 * @param [in] heightNode         重なりリンクの高さノード群クラス
	 * @param [in] walkNodeRTree      歩行者ノードRTree
	 * @param [in] heightNodeRTree    重なりリンクの高さノードのRTree
	 */
	void checkNearPoint(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkNode,
		const FeaturesPack& heightNode,
		const BGRTree& walkNodeRTree,
		const BGRTree& heightNodeRTree
	) const;

	/**
	 * @brief ロジカルコネクションが複数接続しているかチェック
	 * @note エラーコード:139
	 * @param [in] walkNodeObject チェック対象のノードオブジェクト
	 * @param [in] walkLink       歩行者リンク群クラス
	 * @param [in] walkLinkRTree  歩行者リンクのRTree
	 */
	void checkConnectWalkLink(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree) const;

	/**
	* @brie ロジカルコネクションが鋭角に接続しているかチェック
	* @note エラーコード:140
	* @param [in] walkNodeObject チェック対象のノードオブジェクト
	* @param [in] walkLink       歩行者リンク群クラス
	* @param [in] walkLinkRTree  歩行者リンクのRTree
	*/
	void checkConnectWalkLinkAngle(
		const GeoClass& walkNodeObject,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree)const;

	/**
	 * @brief 中心点の隣接点の座標が取得
	 * @param [in] ipGeometry           中心点の形状
	 * @param [in] ipGeometryCompare    隣接点の形状
	 * @param [in] x                    Xの座標
	 * @param [in] y                    Yの座標
	 */
	void getCoordinate(
		const IGeometryPtr& ipGeometry,
		const IGeometryPtr& ipGeometryCompare,
		double& x,
		double& y) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	const std::map<long, CString>& m_walkLinkClassMap; //!< 歩行者種別のコード値ドメインマップ
	const std::map<long, CString>& m_walkNodeClassMap; //!< ノード種別のコード値ドメインマップ
	double m_dist_threshold;     //!< 微小距離閾値(m)
	double m_neighbor_threshold; //!< 近傍距離閾値(m)
	double m_refer_id_threshold; //!< 関連ID距離閾値(m)
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
	double m_coordinateTolerance;//!< 座標許容値
};
