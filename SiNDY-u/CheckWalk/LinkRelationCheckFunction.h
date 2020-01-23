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

class LinkRelationCheckFunction
{
public:
	/**
	 * @brief コンストラクタ
	 * @param out                   [in] ログ出力管理クラスの参照
	 * @param modufyFilter          [in] 編集履歴フィルタ
	 * @param buddy_dist_threshold  [in] リンク紐付チェック用距離閾値
	 * @param buddy_angle_threshold [in] リンク紐付チェック用角度閾値
	 */
	LinkRelationCheckFunction(
		COutPut& out
		, CComVariant modifyFilter
		, double buddy_dist_threshold
		, double buddy_angle_threshold
		):
		m_output(out)
		, m_modifyFilter(modifyFilter)
		, m_buddy_dist_threshold(buddy_dist_threshold)
		, m_buddy_angle_threshold(buddy_angle_threshold)
	{}

	/**
	 * @brief リンク紐付テーブルの全チェック
	 * @param meshcode        [in] チェック中の都市地図メッシュコード
	 * @param ipMeshGeo       [in] チェック中の都市地図メッシュの形状
	 * @param roadLink        [in] 周辺9メッシュの道路リンク群クラス
	 * @param walkLink        [in] 周辺9メッシュの歩行者リンク群クラス
	 * @param linkRelation    [in] リンク紐付けテーブルの全レコード
	 * @param bLinkRelation   [in] link_relationモードで実行するか否か
	 */
	void checkLinkRelation(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelation,
		bool bLinkRelation
		);

	/** 
	 * @brief humancar,walkableモード用のリンク紐付テーブル
	 * @param meshcode        [in] チェック中の都市地図メッシュコード
	 * @param ipMeshGeo       [in] チェック中の都市地図メッシュの形状
	 * @param roadLink        [in] 周辺9メッシュの道路リンク群クラス
	 * @param walkLink        [in] 周辺9メッシュの歩行者リンク群クラス
	 * @param linkRelation    [in] リンク紐付けテーブルの全レコード
	 */
	void checkHumanCarOrWalkable(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelationMap
		);

	// TODO: checkLinkRelationと機能をまとめられるところはまとめる
	/**
	 * @brief releaseモード用のリンク紐付テーブルチェック
	 * @param ipMeshGeo       [in] チェック中の都市地図メッシュの形状
	 * @param roadLink        [in] 周辺9メッシュの道路リンク群クラス
	 * @param walkLink        [in] 周辺9メッシュの歩行者リンク群クラス
	 * @param linkRelation    [in] リンク紐付けテーブルの全レコード
	 */
	void checkRelease(
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadLink,
		const FeaturesPack& walkLink,
		const MultiRowsPack& linkRelationMap
		);

private:

	/**
	 * @brief リンク同士の距離が離れすぎていないかチェック
	 * @note エラーコード:1206
	 * @param walkLinkObj   [in] チェック対象の歩行者リンクのオブジェクト
	 * @param roadLinkID    [in] チェック対象の歩行者リンクに紐付く道路リンクのOID
	 * @param roadLinkObj   [in] チェック対象の道路リンクのオブジェクト
	 * @param objectId      [in] チェック中のリンク紐付けテーブルのレコードの情報
	 */
	void checkLinkDistance(
		const GeoClass& walkLinkObj,
		const long roadLinkID,
		const GeoClass& roadLinkObj,
		const long objectId);

	/**
	 * @brief リンク同士の角度が大きすぎないかチェック
	 * @note エラーコード:1207
	 * @param walkLinkObj   [in] チェック対象の歩行者リンクのオブジェクト
	 * @param roadLinkID    [in] チェック対象の歩行者リンクに紐付く道路リンクのOID
	 * @param roadLinkObj   [in] チェック対象の道路リンクのオブジェクト
	 * @param objectId      [in] チェック中のリンク紐付けテーブルのレコードの情報
	 */
	void checkLinkAngle(
		const GeoClass& walkLinkObj,
		const long roadLinkID,
		const GeoClass& roadLinkObj,
		const long objectId );
	/**
	 * @brief	リンク紐付テーブルの関連リンク群チェック
	 * @param	meshcode	[in]	メッシュコード
	 * @param	baseLinkObj	[in]	基準リンク
	 * @param   fieldMap    [in]    道路リンクフィールドマップ
	 * @param	relLinks	[in]	関連リンクのリスト
	 * @param	maxDistance	[in/out]	基準リンク、関連リンク間の最大距離
	 * @param   bRelease    [in] releaseモードで実行されているか否か
	 */
	void checkLinkRelationGroup(
		const GeoClass& baseLinkObj,
		const sindy::CFieldMap& fieldMap,
		const std::map<long, IGeometryPtr>& relLinks,
		double& maxDistance,
		bool bRelease
		);


	/**
	 * @brief	指定された2つのGeoClassが接触していないか
	 * @param	geo1	[in]	GeoClass1
	 * @param	geo2	[in]	GeoClass2
	 * @param	distance	[in]	GeoClass1のバッファ距離(m)
	 * @retval	true	接触していない
	 * @retval	false	接触している
	 */
	bool IsDisjoint(
		const GeoClass& geo1,
		const GeoClass& geo2,
		double distance
		);

	/**
	 * @brief	指定されたmapのkeyをカンマ区切りで結合する。
	 * @param	target	[in]	結合対象のmap
	 * @retval	カンマ区切りのkeyの文字列
	 */
	CString JoinKeyValue(
		const std::map<long, IGeometryPtr>& target
		);

	/**
	 * @brief	指定されたリンクのリストがつながっているか
	 * @param	links	[in]	リンクのリスト
	 * @retval	true	つながっている
	 * @retval	false	つながっていない
	 */
	bool ConnectLinks(
		const std::map<long, IGeometryPtr>& links
		);

	/**
	 * @brief	指定されたPolylineをFROM/TOのみのLineとみなし、2つのPolylineの角度差を求める
	 * @param	geo1	[in]	PolylineのGeoClass1
	 * @param	geo2	[in]	PolylineのGeoClass2
	 * @param	angle	[out]	角度差
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool ContainAngluarDiff(
		const GeoClass& geo1,
		const GeoClass& geo2,
		double angle
		);

	/**
	 * @brief	指定されたPolylineをFROM/TOのみのLineとみなし角度を求める
	 * @param	ipGeometry	[in]	Polyline
	 * @retval	角度(-180°～180°)
	 */
	double GetAngle(
		const IGeometryPtr& ipGeometry
		);

	/**
	* @brief	buddyをもつ歩行禁止の道路リンクのエラー出力
	* @param	walkableErrRoadLinks	[in]	出力対象の道路リンクIDセット
	* @param    roadLink    [in]    道路リンク群クラス
	* @param	meshcode	[in]	メッシュコード
	*/
	void outputWalkableErr(
		const std::set<long>& walkableErrRoadLinks,
		const FeaturesPack& roadLink,
		const CString& meshcode );

protected:
	COutPut& m_output; //!< ログ出力用インスタンスへの参照
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	double m_buddy_dist_threshold; //!< リンク紐付チェック用距離閾値
	double m_buddy_angle_threshold; //!< リンク紐付チェック用角度閾値

};
