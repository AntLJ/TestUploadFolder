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

class CCheckWalk;
class WalkLinkCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out               [in] ログ出力管理クラスの参照
	 * @param modufyFilter      [in] 編集履歴フィルタ
	 * @param walkLinkClassMap  [in] 歩行者種別のコード値ドメインマップ
	 * @param walkCode          [in] 歩行者路線コードの情報
	 * @param walkNoPassage     [in] 歩行者通行禁止規制テーブルの情報
	 * @param walkOnewayMap     [in] 歩行者一方通行規制テーブルの情報
	 * @param dist_threshold    [in] 微小距離閾値(m)
	 * @param angle_threshold   [in] 角度閾値(度)
	 * @param coordinateTolerance [in] 座標許容値
	 * @param dataType          [in] チェック対象のデータタイプ
	 * @param microLogicalConnection  [in]  微小ロジカルコネクション閾値(m)
	 */
	WalkLinkCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkLinkClassMap
		, const RowsPack& walkCode
		, const MultiRowsPack& walkNoPassage
		, const MultiRowsPack& walkOneway
		, double dist_threshold
		, double angle_threshold
		, double coordinateTolerance
		, sindy::schema::walk_type::type_class::ECode dataType
		, double microLogicalConnection) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkLinkClassMap(walkLinkClassMap)
		, m_walkCode(walkCode)
		, m_walkNoPassage(walkNoPassage)
		, m_walkOneway(walkOneway)
		, m_dist_threshold(dist_threshold)
		, m_angle_threshold(angle_threshold)
		, m_coordinateTolerance(coordinateTolerance)
		, m_dataType(dataType)
		, m_microLogicalConnection(microLogicalConnection)
	{}

	/**
	 * @brief 歩行者リンクの全てのチェックを実行
	 * @param meshcode            [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo           [in] 都市地図メッシュ形状
	 * @param walkLink            [in] 歩行者リンク群クラス
	 * @param walkNode            [in] 歩行者ノード群クラス
	 * @param roadLink            [in] 道路リンク群クラス
	 * @param roadNode            [in] 道路ノード群クラス
	 * @param walkLinkRTree       [in] 歩行者リンクRTree
	 * @param roadLinkRTree       [in] 道路リンクRTree
	 * @param roadNodeRTree       [in] 道路ノードRTree
	 * @param walkNodeLinkIDs     [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param bOther              [in] otherモードとして実行するか否か
	 * @param scrambleArea        [in] スクランブルエリア群クラス
	 * @param scrambleAreaRTree   [in] スクランブルエリアRTree
	 * (ohterモードで実行すると、特定のチェックが実行されなくなる。
	 * 詳細は開発文書を参照のこと)
	 */
	void CheckWalkLink(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs,
		bool bOther,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree
	);

	/**
	 * @brief humancarモード用のチェックを実行
	 * @detail エラーコード 351のみ実行する
	 * @param meshcode    [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo   [in] 都市地図メッシュ形状
	 * @param walkLink    [in] 歩行者リンク群クラス
	 * @param roadLink    [in] 道路リンク群クラス
	 * @param roadLinkRTree [in] 道路リンクRTree
	 */
	void checkHumanCar(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree
	);

	// TODO: CheckWalkLinkと機能をまとめられるところはまとめる
	/**
	 * @brief releaseモード用のチェックを実行
	 * @param meshcode        [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param roadNode        [in] 道路ノード群クラス
	 * @param walkLinkRTree   [in] 歩行者リンクRTree
	 * @param roadLinkRTree   [in] 道路リンクRTree
	 * @param roadNodeRTree   [in] 道路ノードRTree
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void checkRelease(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const NODEIDMAP& walkNodeLinkIDs
	);

protected:
	/**
	 * @breif 歩行者リンクが道路リンク上にあるか判定
	 * @param findRoadLinkObj    [out] 歩行者リンク上にある道路リンクを出力
	 * @param walkLinkObj        [in]  チェック対象の歩行者リンクの属性・形状
	 * @param ipWalkLinkPointCol [in]  チェック対象の歩行者リンクの構成点
	 * @param walkLinkPointCnt   [in]  チェック対象の歩行者リンクの構成点数
	 * @param walkLinkClass      [in]  チェック対象の歩行者リンクの歩行者種別
	 * @param roadLinkSet        [in]  チェック対象の歩行者リンクのEnvelopeに重なる道路リンクID群
	 * @param roadLinkMap        [in]  道路リンクの属性・形状を格納したマップ
	 * @retval true  歩行者は道路リンク上にある
	 * @retval false 歩行者は道路リンク上にない
	 */
	bool isWalkLinkOnRoadLink(
		GeoClass& findRoadLinkObj,
		const GeoClass& walkLinkObj,
		const IPointCollectionPtr& ipWalkLinkPointCol,
		long walkLinkPointCnt,
		long walkLinkClass,
		const std::set<long>& roadLinkSet,
		const std::map<long, GeoClass>& roadLinkMap
	);

	/**
	 * @brief 歩行者リンクが歩行者リンクに含まれる場合に,属性値や包含関係が正しいかチェック
	 * @param walkLinkObj        [in] チェック対象の歩行者リンクの属性・形状
	 * @param ipWalkLinkPointCol [in] チェック対象の歩行者リンクの構成点
	 * @param walkLinkPointCnt   [in] チェック対象の歩行者リンクの構成点数
	 * @param walkLinkClass      [in] チェック対象の歩行者リンクの歩行者種別
	 * @param walkLink           [in] 歩行者リンク群クラス
	 * @param roadLink           [in] 道路リンク群クラス
	 * @param roadNode           [in] 道路ノード群クラス
	 * @param walkLinkRTree      [in] 歩行者リンクRTree
	 * @param roadLinkRTree      [in] 道路リンクRTree
	 * @param roadNodeRTree      [in] 道路ノードRTree
	 * @param meshcode           [in] 処理中の都市地図メッシュコード
	 * @param bOther             [in] otherモードとして実行するか否か
	 */
	void checkMatching(
		const GeoClass& walkLinkObj,
		const IPointCollectionPtr& ipWalkLinkPointCol,
		long walkLinkPointCnt,
		long walkLinkClass,
		const FeaturesPack& walkLink,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const BGRTree& walkLinkRTree,
		const BGRTree& roadLinkRTree,
		const BGRTree& roadNodeRTree,
		const CString& meshcode,
		bool bOther
	);

	/**
	 * @brief	交差箇所のポイントを取得する。
	 * @param	baseGeoClass	[in]	基準リンクのGeoClass
	 * @param	linkIDs	[in]	交差チェック対象リンクID
	 * @param	link   	[in]	チェック対象群クラス
	 * @param	crossPoints	[out]	交差箇所のリスト
	 * @param	isWalkLink	[in]	チェック対象が歩行者リンクか否か
	 */
	void GetCrossPoint(
		const GeoClass& baseGeoClass,
		const std::set<long>& linkIDs,
		const FeaturesPack& link,
		std::vector<GeoClass>& crossPoints,
		bool isWalkLink /* = false*/
	);

	/**
	 * @brief	指定されたリンク群内に広場フラグONのリンクが存在するかどうか
	 * @param	walkLinkList	[in]	チェック対象のリンク群
	 * @param	fieldMap        [in]	チェック対象のフィールドマップ
	 * @return	true	少なくとも1本は存在する
	 * @return	false	存在しない
	 */
	bool ExistOpenAreaFlag(
		const std::list<GeoClass>& walkLinkList
		, const sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	指定されたリンク群内に人車共用リンクが存在するかどうか
	 * @param	walkLinkList	[in]	チェック対象のリンク群
	 * @param	fieldMap        [in]	チェック対象のフィールドマップ
	 * @return	true	少なくとも1本は存在する
	 * @return	false	存在しない
	 */
	bool ExistHumanCarLink(
		const std::list<GeoClass>& walkLinkList
		, const sindy::CFieldMap& fieldMap
	);


	/**
	 * @brief	検索対象のリンク群内で、基準リンク、ノードに接続するリンク群を取得する。
	 * @param	baseLinkID	[in]	基準リンクID
	 * @param	nodeID	[in]	基準ノードID
	 * @param	searchAreaLinkSet	[in]	検索対象のリンク群
	 * @param	nodeLinkIdMap	[in]	歩行者ノードとそのノードに紐づく歩行者リンク群のMap
	 * @param	walkLink   	[in]	歩行者リンク群クラス
	 * @param	findLinkMap	[out]	みつかったリンク群
	 */
	void SearchConnectLinks(
		long baseLinkID,
		long nodeID,
		const std::set<long>& searchAreaLinkSet,
		const NODEIDMAP& nodeLinkIdMap,
		const FeaturesPack& walkLink,
		std::map<long, long>& findLinkMap
	);

	/**
	 * @brief	検索対象のリンク群内で、基準リンク、ノードから3リンク目に到達するリンク群を取得する。
	 * @param	baseLinkID	[in]	基準リンクID
	 * @param	nodeID	[in]	基準ノードID
	 * @param	searchAreaLinkSet	[in]	検索対象のリンク群
	 * @param	nodeLinkIdMap	[in]	歩行者ノードとそのノードに紐づく歩行者リンク群のMap
	 * @param	walkLinkMap	[in]	歩行者リンクのMAP
	 * @param	findLinkMap	[out]	みつかったリンク群
	 */
	void SearchThirdConnectLinks(
		long baseLinkID,
		long nodeID,
		const std::set<long>& searchAreaLinkSet,
		const NODEIDMAP& nodeLinkIdMap,
		const FeaturesPack& walkLink,
		std::map<long, long>& findLinkMap
	);

	/**
	 * @brief 地下通路リンクへ接続する歩行者リンクの階層移動種別が下る方向であるか確認し、
	 * 問題があるならエラーログを出力する
	 * @detail 地下通路リンクは本エラーチェック対象にならない。<br>
	 * 下記のいずれかの場合も下る方向と判定される。<br>
	 * ・地下通路リンク-歩行者リンク接続点のノードがエレベータノードである<br>
	 * ・接続している地下通路リンクの階層移動種別が上る方向である
	 * @param underLinkID    [in] 接続される地下通路リンクのID
	 * @param connectLinkMap [in] 接続するリンク群
	 * @param walkNodeObj    [in] 地下通路リンク-歩行者リンクの接続点にあるノードの情報を格納
	 * @param fieldMap       [in] 歩行者ノードのフィールドマップ
	 * @param walkLink       [in] 歩行者リンクの群クラス
	 * @return エラーとなる歩行者リンクIDのリスト
	 */
	std::set<long> checkFloorMoveConnectUnder(
		long underLinkID
		, const std::list<GeoClass>& connectLinkMap
		, const GeoClass& walkNodeObj
		, const sindy::CFieldMap& fieldMap
		, const FeaturesPack& walkLinkMap);

	/**
	 * @brief 歩行者リンクの構成点が他の歩行者リンクの構成点上にある場合にエラーログを出力する
	 * @param checkedLinkId [in] チェック対象の歩行者リンクのID
	 * @param walkLink      [in] 歩行者リンク群クラス
	 * @param meshcode      [in] 処理中の都市地図メッシュコード
	 */
	void checkPointOnOtherLink(
		const std::vector<long>& checkedLinkId
		, const FeaturesPack& walkLink
		, const CString& meshcode);

	/**
	 * @brief	メッシュ外の接続リンクを取得
	 * @detail チェック基準範囲の9都市地図メッシュ外への探索に伴い、<br>
	 * チェック範囲は拡張され、対応のリンク＆ノード情報も更新される。<br>
	 * @param	nodeId	     [in/out] 対象歩行者ノードID（探索開始時は起点ノード、終了時は終点ノード）
	 * @param   adjacentLink [in] 探索の起点ノードに接続するリンク
	 * @param	ipMeshGeo	 [in/out] チェック範囲
	 * @param   walkLink     [in/out] 歩行者リンク群クラス
	 * @param   walkNode     [in/out] 歩行者ノード群クラス
	 * @param   nodeLinkIDs  [in/out] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @return	取得したレコード
	 * @retval	nullptr	取得に失敗
	 */
	std::set<long> getAdjacentWalkLink(
		long& nodeId
		, const long linkId
		, const IGeometryPtr& ipMeshGeo
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& nodeLinkIDs
	);

	/**
	 * @brief	対象ノードの所在するメッシュの情報を追加する
	 * @param	targetNodeId     [in] 対象歩行者ノードID
	 * @param	ipMeshGeo	     [in] チェック範囲
	 * @param   walkLink         [in/out] 歩行者リンク群クラス
	 * @param   walkNode         [in/out] 歩行者群クラス
	 * @param   walkNodeLinkIDs  [in/out] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void addMeshInfo(
		const long targetNodeId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs);

	/**
	* @brief	属性チェック
	* @node		エラーコード 301～305,309,339
	* @param	walkLinkClass		[in] 歩行者種別
	* @param	floorMoveClass		[in] 階層移動種別
	* @param	onewayCode			[in] 一方通行種別コード
	* @param	noPassCode			[in] 通行禁止種別コード
	* @param	roadSurfaceCode		[in] 路面素材コード
	* @param	walkCode			[in] 歩行者路線コード
	* @param	isForRelease		[in] リリース向けチェックか？
	*/
	void attributesCheck(
		long walkLinkClass,
		long floorMoveClass,
		long onewayCode,
		long noPassCode,
		long roadSurfaceCode,
		long walkCode,
		bool isForRelease
	);

	/**
	* @brief	フラグ属性チェック
	* @node		エラーコード 310
	* @param	autoWalkF		[in] オートウォークフラグ
	* @param	stairF			[in] 階段フラグ
	* @param	slopeF			[in] スロープフラグ
	* @param	escalatorF		[in] エスカレータフラグ
	* @param	arcadeF			[in] 屋根付きフラグ
	* @param	openAreaF		[in] 広場フラグ
	* @param	tunnelF			[in] トンネルフラグ
	* @param	scrambleF		[in] スクランブルフラグ
	* @param	onewayRF		[in] 一方通行時間規制関連フラグ
	* @param	noPassRF		[in] 通行禁止時間規制関連フラグ
	*/
	void flagsCheck(
		long autoWalkF,
		long stairF,
		long slopeF,
		long escalatorF,
		long arcadeF,
		long openAreaF,
		long tunnelF,
		long scrambleF,
		long onewayRF,
		long noPassRF
	);

	/**
	* @brief	From/ToのIDチェック
	* @node		エラーコード 311,312,326
	* @param	walkLinkID		[in] 歩行者リンクOID
	* @param	fromWalkNodeID	[in] 始点ノードのOID
	* @param	toWalkNodeID	[in] 終点ノードのOID
	* @param	dupNodeMap		[out] 重複チェック用マップ
	*/
	void fromToIdCheck(
		long walkLinkID,
		long fromWalkNodeID,
		long toWalkNodeID,
		std::map<std::pair<long, long>, std::list<long>>& dupNodeMap
	);

	/**
	* @brief	形状を使ったチェック１
	* @node		エラーコード 313,316,317,326,350
	* @param	walkLinkObj			[in] 歩行者リンクオブジェクト
	* @param	walkNode			[in] 歩行者ノード群クラス
	* @param	walkLinkClass		[in] 歩行者種別
	* @param	fromWalkNodeID		[in] 始点ノードのOID
	* @param	toWalkNodeID		[in] 終点ノードのOID
	* @param	walkNodeClassIndex	[in] 歩行者ノード種別のインデックス
	* @param	targetWalkLinkId	[out] エラーコード360のチェック用IDリスト
	* @param	fromWalkNodeObj		[out] 始点ノードオブジェクト
	* @param	toWalkNodeObj		[out] 終点ノードオブジェクト
	*/
	bool WalkLinkCheckFunction::shapeCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkNode,
		long walkLinkClass,
		long fromWalkNodeID,
		long toWalkNodeID,
		long walkNodeClassIndex,
		std::vector<long>& targetWalkLinkId,
		GeoClass& fromWalkNodeObj,
		GeoClass& toWalkNodeObj
	);

	/**
	* @brief	形状を使ったチェック２
	* @node		エラーコード 321～325
	* @param	walkLinkObj			[in] 歩行者リンクオブジェクト
	* @param	fromNodeObj			[in] 始点ノードオブジェクト
	* @param	toNodeObj			[in] 終点ノードオブジェクト
	* @param	isForRelease		[in] リリース向けチェックか？
	*/
	void shapeCheck2(
		const GeoClass& walkLinkObj,
		const GeoClass& fromNodeObj,
		const GeoClass& toNodeObj,
		bool isForRelease
	);

	/**
	* @brief	みなし横断歩道チェック
	* @node		エラーコード 402
	* @param	walkLinkObj			[in] 歩行者リンクオブジェクト
	* @param	walkLinkClass		[in] 歩行者種別
	* @param	roadLink			[in] 道路リンク群クラス
	* @param	roadLinkRTree		[in] 道路リンクのRTree
	*/
	void inferredCrossRoadCheck(
		const GeoClass& walkLinkObj,
		long walkLinkClass,
		const FeaturesPack& roadLink,
		const BGRTree& roadLinkRTree
	);

	/**
	* @brief	属性間の整合チェック１
	* @node		エラーコード 329,330,331,334,338,346,357,401
	* @param	floorMoveClass	[in] 階層移動種別
	* @param	stairF			[in] 階段フラグ
	* @param	slopeF			[in] スロープフラグ
	* @param	escalatorF		[in] エスカレータフラグ
	* @param	onewayCode		[in] 一方通行種別コード
	* @param	autoWalkF		[in] オートウォークフラグ
	* @param	arcadeF			[in] 屋根付きフラグ
	* @param	openAreaF		[in] 広場フラグ
	* @param	tunnelF			[in] トンネルフラグ
	* @param	scrambleF		[in] スクランブルフラグ
	* @param	walkLinkClass	[in] 歩行者種別
	* @param	walkCode		[in] 歩行者路線コード
	*/
	void attributeConformityCheck(
		long floorMoveClass,
		long stairF,
		long slopeF,
		long escalatorF,
		long onewayCode,
		long autoWalkF,
		long arcadeF,
		long openAreaF,
		long tunnelF,
		long scrambleF,
		long walkLinkClass,
		long walkCode
	);

	/**
	* @brief	属性間の整合チェック２
	* @node		エラーコード 342,343,347,348,349
	* @param	onewayCode		[in] 一方通行種別コード
	* @param	noPassCode		[in] 通行禁止種別コード
	* @param	noPassRF		[in] 通行禁止時間規制関連フラグ
	* @param	onewayRF		[in] 一方通行時間規制関連フラグ
	* @param	walkLinkClass	[in] 歩行者種別
	* @param	walkLinkID		[in] 歩行者リンクOID
	*/
	void attributeConformityCheck2(
		long onewayCode,
		long noPassCode,
		long noPassRF,
		long onewayRF,
		long walkLinkClass,
		long walkLinkID
	);

	/**
	* @brief	コードの変化チェック
	* @node		エラーコード 332,354
	* @param	fromLinks		[in] 始点側歩行者リンクリスト
	* @param	toLinks			[in] 終点側歩行者リンクリスト
	* @param	walkLinkObj		[in] 基準歩行者リンクオブジェクト
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	walkLinkClass	[in] 歩行者種別
	* @param	floorMoveClass	[in] 階層移動種別
	* @param	floorLevelIndex	[in] 階層レベルインデックス
	*/
	void attributeChangeCheck(
		const std::list<GeoClass>& fromLinks,
		const std::list<GeoClass>& toLinks,
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkLink,
		long walkLinkClass,
		long floorMoveClass,
		long floorLevelIndex
	);

	/**
	* @brief	広場チェック
	* @node		エラーコード 336
	* @param	openAreaF		[in] 始点側歩行者リンクリスト
	* @param	fromLinks		[in] 始点側歩行者リンクリスト
	* @param	toLinks			[in] 終点側歩行者リンクリスト
	* @param	walkLink		[in] 歩行者リンク群クラス
	*/
	void openAreaCheck(
		long openAreaF,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink
	);

	/**
	* @brief	歩行者リンク接続チェック
	* @node		エラーコード 337
	* @param	walkLinkClass		[in] 歩行者種別
	* @param	fromWalkNodeObj		[in] 始点歩行者ノードオブジェクト
	* @param	toWalkNodeObj		[in] 終点歩行者ノードオブジェクト
	* @param	fromLinks			[in] 始点側歩行者リンクリスト
	* @param	toLinks				[in] 終点側歩行者リンクリスト
	* @param	walkLink			[in] 歩行者リンク群クラス
	* @param	walkNodeClassIndex	[in] 歩行者ノード種別のインデックス
	*/
	void connectLinkCheck(
		long walkLinkClass,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink,
		long walkNodeClassIndex
	);

	/**
	* @brief	並行する形状の場合の属性チェック
	* @node		エラーコード 355,356
	* @param	walkLinkClass	[in] 歩行者種別
	* @param	floorMoveClass	[in] 階層移動種別
	* @param	escalatorF		[in] エスカレータフラグ
	* @param	onewayCode		[in] 一方通行種別コード
	* @param	walkNodeObj		[in] 歩行者ノードオブジェクト
	* @param	fromWalkNodeID	[in] 始点ノードOID
	* @param	toWalkNodeID	[in] 終点ノードOID
	* @param	walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	walkLinkRTree	[in] 歩行者リンクのRTree
	*/
	void parallelCheck(
		long walkLinkClass,
		long floorMoveClass,
		long escalatorF,
		long onewayCode,
		const GeoClass& walkLinkObj,
		long fromWalkNodeID,
		long toWalkNodeID,
		const NODEIDMAP& walkNodeLinkIDs,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	);

	/**
	* @brief	地下属性チェック
	* @node		エラーコード 359
	* @param	walkLinkClass	[in] 歩行者種別
	* @param	walkLinkID		[in] 歩行者リンクOID
	* @param	fromWalkNodeObj	[in] 始点ノードオブジェクト
	* @param	toWalkNodeObj	[in] 終点ノードオブジェクト
	* @param	fromLinks		[in] 始点側歩行者リンクリスト
	* @param	toLinks			[in] 終点側歩行者リンクリスト
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	walkNode		[in] 歩行者ノード群クラス
	* @param	meshcode		[in] メッシュコード
	*/
	void underGroundCheck(
		long walkLinkClass,
		long walkLinkID,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const CString& meshcode
	);

	/**
	* @brief	属性的接続性チェック
	* @node		エラーコード 353
	* @param	ipMeshGeo			[in] 都市地図メッシュ形状
	* @param	walkLink			[in] 歩行者リンク群クラス
	* @param	walkNode			[in] 歩行者ノード群クラス
	* @param	walkNodeLinkIDs		[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param	walkLinkMapFor353	[in] エラーコード353用のリンクリスト
	*/
	void attributeConnectionCheck(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP walkNodeLinkIDs,
		const std::map<long, GeoClass>& walkLinkMapFor353
	);

	/**
	* @brief	同一From/Toチェック
	* @node		エラーコード 327
	* @param	dupNodeMap		[in] 重複チェック用マップ
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	meshcode		[in] メッシュコード
	*/
	void sameFromToCheck(
		const std::map<std::pair<long, long>, std::list<long>>& dupNodeMap,
		const FeaturesPack& walkLink,
		const CString& meshcode
	);

	/**
	* @brief	リンクに付いている指定IDと反対側のIDを返す
	* @node		始点なら終点、終点なら始点
	* @param	dupNodeMap		[in] 重複チェック用マップ
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	meshcode		[in] メッシュコード
	* @retval	反対のOID
	*/
	long anotherNodeID(
		const GeoClass& linkObj,
		long nodeId,
		long fromIdIndex,
		long toIdIndex
	);

	/**
	* @brief	横断歩道の終端リストを返す
	* @node		ノードから、その種別やその先のリンク種別を元に、
	*           終端として扱うノードリストを求める
	*			途中に「人車共用」と「横断歩道」のみのノードがあったら、
	*			その先を見る[bug 12633]対応
	* @param	linkObj			[in] 歩行者リンクオブジェクト
	* @param	nodeObj			[in] 歩行者ノードオブジェクト
	* @param	nodeClassIndex	[in] 歩行者ノード種別インデックス
	* @param	walkClassIndex	[in] 歩行者種別インデックス
	* @param	fromIdIndex		[in] 始点OIDインデックス
	* @param	toIdIndex		[in] 終点OIDインデックス
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	walkNode		[in] 歩行者ノード群クラス
	* @param	walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param	inspectedIDs	[in/out] 検査済みのノードIDセット
	* @retval	横断歩道の終端として扱うノードのOIDリスト
	*/
	std::list<GeoClass> searchCrossRoadNode(
		const GeoClass& linkObj,
		const GeoClass& nodeObj,
		long nodeClassIndex,
		long walkClassIndex,
		long fromIdIndex,
		long toIdIndex,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& inspectedIDs
	);

	/**
	* @brief	横断歩道の両端の関連道路ノード（交差点名称）チェック
	* @node		[bug 12633]対応 エラーコード 361
	* @param	nodeClassIndex	[in] 歩行者ノード種別インデックス
	* @param	walkClassIndex	[in] 歩行者種別インデックス
	* @param	fromIdIndex		[in] 始点OIDインデックス
	* @param	toIdIndex		[in] 終点OIDインデックス
	* @param	linkObj			[in] 歩行者リンクオブジェクト
	* @param	nodeObj			[in] 歩行者ノードオブジェクト
	* @param	walkLink		[in] 歩行者リンク群クラス
	* @param	walkNode		[in] 歩行者ノード群クラス
	* @param	walkNodeLinkIDs	[in] 歩行者ノードに接続する歩行者リンクのID対応表
	* @param	meshcode		[in] メッシュコード
	*/
	void crossRoadNodeCheck(
		long nodeClassIndex,
		long walkClassIndex,
		long fromIdIndex,
		long toIdIndex,
		const GeoClass& linkObj,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const CString& meshcode
	);

	/**
	* @brief	改札ノードの前後整合チェック
	* @node		[bug 12687]のチェック１対応
	* @param	nodeClassIndex	[in] 歩行者ノード種別インデックス
	* @param	walkClassIndex	[in] 歩行者種別インデックス
	* @param	nodeObj			[in] 歩行者ノードオブジェクト
	* @param	links			[in] ノードに接続されているリンクリスト
	*/
	bool ticketGateNodeCheck(
		long nodeClassIndex,
		long walkClassIndex,
		const GeoClass& nodeObj,
		const std::list<GeoClass> & links
	);

	/**
	* @brief	改札内リンクの両端ノードの整合チェック
	* @node		[bug 12687]のチェック１対応
	* @param	nodeClassIndex	[in] 歩行者ノード種別インデックス
	* @param	walkClassIndex	[in] 歩行者種別インデックス
	* @param	fromNodeObj		[in] 始点側歩行者ノードオブジェクト
	* @param	toNodeObj		[in] 終点側歩行者ノードオブジェクト
	* @param	fromLinks		[in] 始点側ノードに接続されているリンクリスト
	* @param	toLinks			[in] 終点側ノードに接続されているリンクリスト
	*/
	void internalTicketGateLinkCheck(
		long nodeClassIndex,
		long walkClassIndex,
		const GeoClass& fromWalkNodeObj,
		const GeoClass& toWalkNodeObj,
		const std::list<GeoClass> & fromLinks,
		const std::list<GeoClass>& toLinks
	);

	/**
	 * @brief スクランブルフラグが付与されているのにスクランブルエリア内に無いかチェック
	 * @param [in] walkLinkObj        チェック対象の歩行者リンクの属性・形状
	 * @param [in] scrambleArea       スクランブルエリア群クラス
	 * @param [in] scrambleAreaRTree  スクランブルエリアRTree
	 * @param [in] scrambleFlag       スクランブルフラグ
	 */
	void walkLinkNotOnScrambleAreaCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree,
		long scrambleFlag
	) const;

	/**
	 * @brief スクランブルエリア内なのにスクランブルフラグが付与されていないかチェック
	 * @param [in] walkLinkObj        チェック対象の歩行者リンクの属性・形状
	 * @param [in] scrambleArea       スクランブルエリア群クラス
	 * @param [in] scrambleAreaRTree  スクランブルエリアRTree
	 * @param [in] walkLinkClass      歩行者種別
	 * @param [in] scrambleFlag       スクランブルフラグ
	 */
	void walkLinkOnScrambleAreaCheck(
		const GeoClass& walkLinkObj,
		const FeaturesPack& scrambleArea,
		const BGRTree& scrambleAreaRTree,
		long walkLinkClass,
		long scrambleFlag
	) const;

	/**
	 * @brief 車椅子通行コードが不正な値かチェック
	 * @note エラーコード:365
	 * @param [in] walkWheelChair     車椅子通行コード
	 */
	void checkWheelChair(
		long  walkWheelChair
	) const;

	/**
	 * @brief のりば番号が不正な値かチェック
	 * @note エラーコード:366
	 * @param [in] walkPlatformNumber     のりば番号
	 */
	void checkPlatformNumber(
		long  walkPlatformNumber
	) const;

	/**
	 * @brief 微小ロジカルコネクションかチェック
	 * @note エラーコード:367
	 * @param [in] walkLinkObj        チェック対象の歩行者リンクの属性・形状
	 */
	void checkMicroLogicalConnection(
		const GeoClass& walkLinkObj
	) const;

	/**
	 * @brief ロジカルコネクションと交差しているかチェック
	 * @note エラーコード:368
	 * @param [in] walkLinkObj        チェック対象の歩行者リンクの属性・形状
	 * @param [in] walkLink           歩行者リンク群クラス
	 * @param [in] walkLinkRTree      歩行者リンクのRTree
	 */
	void WalkLinkCheckFunction::checkCrossWalkLink(
		const GeoClass& walkLinkObj,
		const FeaturesPack& walkLink,
		const BGRTree& walkLinkRTree
	) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	const std::map<long, CString>& m_walkLinkClassMap; //!< 歩行者種別のコード値ドメインマップ
	const RowsPack& m_walkCode; //!< 歩行者路線コードの情報
	const MultiRowsPack& m_walkNoPassage; //!< 歩行者通行禁止規制テーブル情報
	const MultiRowsPack& m_walkOneway; //!< 歩行者一方通行規制テーブルの情報
	double m_dist_threshold; //!< 微小距離閾値(m)
	double m_angle_threshold;    //!< 角度閾値(度)
	double m_coordinateTolerance;//!< 座標許容値
	sindy::schema::walk_type::type_class::ECode m_dataType;              //!< データタイプ
	double	m_microLogicalConnection;                                    //!< 微小ロジカルコネクション閾値(m)
};
