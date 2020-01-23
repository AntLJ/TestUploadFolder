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
class RoadLinkCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out               [in] ログ出力管理クラスの参照
	 * @param modufyFilter      [in] 編集履歴フィルタ
	 * @param m_walkableMap     [in] 歩行者可能種別のコード値ドメインマップ
	 * @param dataType           [in] データタイプ
	 */
	RoadLinkCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, const std::map<long, CString>& walkableMap
		, sindy::schema::walk_type::type_class::ECode dataType
	) :
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_walkableMap(walkableMap)
		, m_dataType(dataType)
	{}

	/**
	 * @brief 道路リンクの全チェック
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param roadNode        [in] 道路ノード群クラス
	 * @param roadNodeLinkIDs [in] 道路ノードに接続する道路リンクのID対応表
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param walkLinkRTree   [in] 歩行者リンクのRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 */
	void CheckRoadLink(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const NODEIDMAP& roadNodeLinkIDs,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief walkableモード用のチェック
	 * @param ipMeshGeo       [in] 都市地図メッシュ形状
	 * @param roadLink        [in] 道路リンク群クラス
	 * @param roadNode        [in] 道路ノード群クラス
	 * @param roadNodeLinkIDs [in] 道路ノードに接続する道路リンクのID対応表
	 * @param walkLink        [in] 歩行者リンク群クラス
	 * @param walkNode        [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param walkLinkRTree   [in] 歩行者リンクのRTree
	 * @param walkNodeRTree   [in] 歩行者ノードのRTree
	 */
	void checkWalkable(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& roadNode,
		const NODEIDMAP& roadNodeLinkIDs,
		const FeaturesPack& walkLink,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeLinkIDs,
		const BGRTree& walkLinkRTree,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief otherモード用のチェック
	 * @param ipMeshGeo   [in] 都市地図メッシュ形状
	 * @param roadLink    [in] 道路リンク群クラス
	 * @param roadNodeLinkIDs [in] 道路ノードに接続する道路リンクのID対応表
	 */
	void checkOther(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const NODEIDMAP roadNodeLinkIDs
	);


	/**
	 * @brief releaseモード用のチェック
	 * @param ipMeshGeo   [in] 都市地図メッシュ形状
	 * @param roadLink    [in] 道路リンク群クラス
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink
	);

private:
	/**
	 * @brief walkableの値が妥当かチェックし、エラーログを出力する
	 * @param linkObj  [in] チェック対象の道路リンクの情報
	 * @param fieldMap [in] 道路リンクのフィールドマップ
	 */
	void checkWalkableField(
		const GeoClass& linkObj
		, const sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief walkable以外の属性値が妥当かチェック
	 * @param linkObj [in] チェック対象の道路リンクの情報
	 * @param fieldMap [in] 道路リンクのフィールドマップ
	 */
	void checkOtherField(
		const GeoClass& linkObj
		, const sindy::CFieldMap& fieldMap);

	/**
	 * @brief 歩道がつながっていないが妥当かチェック
	 * @node　エラーコード 511
	 * @param [in] linkId             チェック対象の道路リンクのID
	 * @param [in] linkObj            チェック対象の道路リンクの情報
	 * @param [in] connectFromLinkIds チェック対象の道路リンクのfrom側に接続している道路リンクのID
	 * @param [in] connectToLinkIds   チェック対象の道路リンクのto側に接続している道路リンクのID
	 * @param [in] roadLink           道路リンク群クラス
	 */
	void checkNotConnectedSideWalk(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink);

	/**
	 * @brief walkable以外の属性値が妥当かチェック
	 * @param linkId             [in] チェック対象の道路リンクのID
	 * @param linkObj            [in] チェック対象の道路リンクの情報
	 * @param connectFromLinkIds [in]
	 * チェック対象の道路リンクのfrom側に接続している道路リンクのID
	 * @param connectToLinkIds   [in]
	 * チェック対象の道路リンクのto側に接続している道路リンクのID
	 * @param roadLink           [in] 道路リンク群クラス
	 * @param roadNode           [in] 道路ノード群クラス
	 * @param roadNodeLinkIDs    [in] 道路ノードに接続する道路リンクのID対応表
	 * @param walkLink           [in] 歩行者リンク群クラス
	 * @param walkNode           [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs    [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param walkNodeRTree      [in] 歩行者ノードのRTree
	 * @param meshcode           [in] 処理中の都市地図メッシュコード
	 * @param ipMeshGeo	         [in] チェック範囲
	 */
	void checkHumanCarSandwichedBetweenNotWalk(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink
		, FeaturesPack& roadNode
		, NODEIDMAP& roadNodeLinkIDs
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& walkNodeLinkIDs
		, BGRTree& walkNodeRTree
		, const IGeometryPtr& ipMeshGeo);

	/**
	 * @brief walkable以外の属性値が妥当かチェック
	 * @param linkId             [in] チェック対象の道路リンクのID
	 * @param linkObj            [in] チェック対象の道路リンクの情報
	 * @param connectFromLinkIds [in]
	 * チェック対象の道路リンクのfrom側に接続している道路リンクのID
	 * @param connectToLinkIds   [in]
	 * チェック対象の道路リンクのto側に接続している道路リンクのID
	 * @param roadLink           [in] 道路リンク群クラス
	 * @param roadNode           [in] 道路ノード群クラス
	 * @param roadNodeLinkIDs    [in] 道路ノードに接続する道路リンクのID対応表
	 * @param walkLink           [in] 歩行者リンク群クラス
	 * @param walkNode           [in] 歩行者ノード群クラス
	 * @param walkNodeLinkIDs    [in] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param walkLinkRTree      [in] 歩行者リンクのRTree
	 * @param walkNodeRTree      [in] 歩行者ノードのRTree
	 * @param ipMeshGeo	         [in] チェック範囲
	 */
	void checkBeforeAfterLinkWalkable(
		long linkId
		, const GeoClass& linkObj
		, const std::set<long>& connectFromLinkIds
		, const std::set<long>& connectToLinkIds
		, FeaturesPack& roadLink
		, FeaturesPack& roadNode
		, NODEIDMAP& roadNodeLinkIDs
		, FeaturesPack& walkLink
		, FeaturesPack& walkNode
		, NODEIDMAP& walkNodeLinkIDs
		, BGRTree& walkLinkRTree
		, BGRTree& walkNodeRTree
		, const IGeometryPtr& ipMeshGeo);

	/**
	 * @brief	メッシュ外の接続道路リンクを取得
	 * @detail チェック基準範囲の9都市地図メッシュ外への探索に伴い、<br>
	 * チェック範囲は拡張され、対応のリンク＆ノード情報も更新される。<br>
	 * @param	nodeId	         [in/out] 対象歩行者ノードID（探索開始時は起点ノード、終了時は終点ノード）
	 * @param   linkId           [in]     対象リンクID
	 * @param	ipMeshGeo	     [in/out] チェック範囲
	 * @param   roadLink         [in/out] 道路リンク群クラス
	 * @param   roadNode         [in/out] 道路ノード群クラス
	 * @param   roadNodeLinkIDs  [in/out] 道路ノードに接続する歩行者リンクのID対応表
	 * @param   walkLink         [in/out] 歩行者リンク群クラス
	 * @param   walkNode         [in/out] 歩行者ノード群クラス
	 * @param   walkNodeLinkIDs  [in/out] 歩行者ノードに接続する歩行者リンクのID対応表
	 * @param	targetLinkIDs	 [in/out] 対象リンクID群
	 * @return	取得したレコード
	 * @retval	nullptr	取得に失敗
	 */
	std::set<long> getAdjacentRoadLink(
		long& nodeId,
		const long linkId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		NODEIDMAP& roadNodeLinkIDs,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs,
		std::set<long>& targetLinkIDs);

	/**
	 * @brief	対象ノードの所在するメッシュの情報を追加する
	 * @param	targetNodeId     [in] 対象歩行者ノードID
	 * @param	ipMeshGeo	     [in] チェック範囲
	 * @param   roadLink         [in/out] 道路リンク群クラス
	 * @param   roadNode         [in/out] 道路ノード群クラス
	 * @param   roadNodeLinkIDs  [in/out] 道路ノードに接続する歩行者リンクのID対応表
	 * @param   walkLink         [in/out] 歩行者リンク群クラス
	 * @param   walkNode         [in/out] 歩行者ノード群クラス
	 * @param   walkNodeLinkIDs  [in/out] 歩行者ノードに接続する歩行者リンクのID対応表
	 */
	void addMeshInfo(
		const long targetNodeId,
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		NODEIDMAP& roadNodeLinkIDs,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs);

	/**
	 * @brief	歩道がつながっていないの判定
	 * @param	[in] isDirect     　　　　　　接続リンクの向き
	 * @param	[in] linkWalkableCode	      チェック対象のWALKABLE_Cの値
	 * @param   [in] connectedWalkableCode    接続リンクのWALKABLE_Cの値
	 * @retval	true  エラー
	 * @retval	false エラーではない
	 */
	bool isConnectSideWalkError(bool isDirect, long linkWalkableCode, long connectedWalkableCode);

	/**
	 * @brief	歩行可能種別コードが不正な値かチェック
	 * @param	[in] walkableClass      歩行可能種別コード
	 */
	void checkWalkableClass(
		long  walkableClass
	) const;

	/**
	 * @brief メッシュ境界で歩行可能種別が変わっているかチェック
	 * @param [in] connectFromLinkIds  チェック対象の道路リンクに接続している道路リンクのID
	 * @param [in] roadLink            道路リンク群クラス
	 * @param [in] nodeClass           ノード種別コード
	 * @param [in] walkableClass       歩行可能種別コード
	 */
	void checkOverMeshWalkableClass(
		const std::set<long>& connectLinkIds,
		const FeaturesPack& roadLink,
		long nodeClass,
		long walkableClass
	) const;

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	const std::map<long, CString>& m_walkableMap; //!< 歩行可能種別のコード値ドメイン名
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};
