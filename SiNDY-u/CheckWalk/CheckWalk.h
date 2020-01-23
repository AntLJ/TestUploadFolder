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

#include <TDC/sindylib_core/FieldMap.h>
#include "Arguments.h"
#include "OutputLog.h"
#include "Properties.h"
#include "DatabaseControl.h"
#include "TimeReg.h"
#include "HeightInfoMgr.h"
#include "FieldLists.h"
/**
 * @class	CCheckWalk
 * @brief	メインクラス
 */
class CCheckWalk
{
public:

	/**
	 * @brief コンストラクタ
	 * @param args [in] コマンドライン引数
	 * @param prop [in]	設定情報
	 */
	CCheckWalk(const Arguments& args, const CProperties& prop);

	/**
	 * @brief	デストラクタ
	 */
	virtual ~CCheckWalk() {}

	/**
	 * @brief	実行
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool execute();

private:
	typedef std::pair<IGeometryPtr, IGeometryPtr> GeometryPair;
	typedef std::pair<const long, std::pair<IGeometryPtr, IGeometryPtr>> CityMeshPair;
	typedef std::map<long, GeometryPair> CityMeshMap;
	typedef std::map<CString, IGeometryPtr> BaseMeshMap;
	/**
	* @brief ヘッダ情報等出力
	*/
	void OutputHeader();

	/**
	* @brief 対象メッシュ・周辺のポリゴン読み込み
	*/
	void LoadMesh();


	/**
	* @brief 都市地図メッシュ単位での処理
	*/
	void CheckByCityMesh();

	/**
	* @brief メッシュ関係なく全体での処理
	*/
	void CheckByAllMesh();

	/**
	* @brief 中縮メッシュ単位での処理
	*/
	void CheckByBaseMesh();

	/**
	* @brief 各モードの都市地図単位での処理
	* @param function [in] 各モード毎の関数のアドレス
	*/
	void CheckFunctionByCityMesh(
		err_code::ECode(CCheckWalk::*function)(const CityMeshPair&));

	/**
	 * @brief allモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode allCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief humancarモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode humancarCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief walkableモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode walkableCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief height_nodeモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode heightNodeCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief link_relationモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode linkRelationCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief underモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode underGroundCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief otherモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode otherCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief releaseモード用のチェック
	 * @param cityMeshPair [in]
	 * <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	 * @return エラーコード
	 */
	err_code::ECode releaseCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief pedxingモード用のチェック
	 * @param [in] cityMeshPair 都市地図メッシュコード
	 * @return エラーコード
	 */
	err_code::ECode pedxingCheck(const CityMeshPair& cityMeshPair);

	/**
	 * @brief ベースメッシュ単位でチェックが必要なもののチェック
	 */
	void checkBaseMesh();


	/**
	 * @brief	初期化
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool init();

	/**
	 * @brief 整数を列挙したファイルを読み込む
	 * @detail ベースメッシュが指定されている場合、そのベースメッシュが含んでいる都市地図メッシュを指定する
	 * @note 典型的にはメッシュリストファイルの読み込みに使う
	 * @param fileName      [in] リストファイル名
	 * @param integerList   [out] 出力リストのset
	 * @param cityMeshTable [in] 都市地図メッシュのテーブル
	 * @retval true 読み込み成功
	 * @retval false 読み込み失敗
	 */
	bool loadMeshList(const CString& fileName, std::set<long>& integerList, const ITablePtr& cityMeshTable);

	/**
	 * @brief	与えたメッシュ隣接9メッシュリストを作成する
	 */
	void Get9Mesh(long mesh, std::set<long>& meshlist);

	/**
	 * @brief	メッシュリストから、メッシュポリゴンを取得
	 */
	bool GetMeshPolygons(long mesh, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo);

	/**
	 * @brief	与えられたメッシュ形状内の道路リンクと道路ノードを取得
	 * @param	ipMeshGeo	[in]	メッシュポリゴンのジオメトリ
	 * @param	linkMap	[out] 取得した道路リンクMap(キー：ObjectID)
	 * @param	nodeMap	[out] 取得した道路ノードMap(キー：ObjectID)
	 * @param	segmentAttrMap	[out] 取得したリンク内属性Map(キー：ObjectID)
	 * @param	nodeLinkIDs	[out] FROM/TOノードIDをキーとしたキー値(リンクのOID)のmap
	 * @retval	true	成功
	 * @retval	false 失敗
	 */
	bool GetRoadInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& roadLink,
		FeaturesPack& roadNode,
		FeaturesPack& segmentAttr,
		NODEIDMAP& nodeLinkIDs
	);

	/**
	 * @brief	与えられたメッシュ形状内の歩行者リンクと歩行者ノードを取得
	 * @param	ipMeshGeo	[in]	メッシュポリゴンのジオメトリ
	 * @param	walkLink	[in/out] 取得した歩行者リンク群クラス
	 * @param	walkNode	[in/out] 取得した歩行者ノード群クラス
	 * @param	nodeNodeIDs	[out] FROM/TOノードIDをキーとしたノードIDのmap
	 * @param	nodeLinkIDs	[out] FROM/TOノードIDをキーとしたキー値(リンクのOID)のmap
	 * @retval	true	成功
	 * @retval	false 失敗
	 */
	bool GetWalkInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeNodeIDs,
		NODEIDMAP& walkNodeLinkIDs
	);
	/**
	* @brief	与えられたメッシュ形状内の歩行者リンクと歩行者ノードを取得
	* @param	ipMeshGeo	[in]	メッシュポリゴンのジオメトリ
	* @param	walkLink	[in/out] 取得した歩行者リンク群クラス
	* @param	walkNode	[in/out] 取得した歩行者ノード群クラス
	* @param	nodeLinkIDs	[out] FROM/TOノードIDをキーとしたキー値(リンクのOID)のmap
	* @retval	true	成功
	* @retval	false 失敗
	*/
	bool GetWalkInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& walkLink,
		FeaturesPack& walkNode,
		NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief	指定されたメッシュ内の駅出入口POIデータを取得
	 * @param	ipMeshGeo	[in]	取得するメッシュ範囲
	 * @param	stationGate	[in/out]	取得した駅出入口POI群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetStationGateInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& stationGate
	);

	/**
	 * @brief	指定されたメッシュ内のPOIポイントデータを取得
	 * @param	ipMeshGeo	[in]	取得するメッシュ範囲
	 * @param	poiPoint	[in/out]	取得したPOIポイント群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetPoiPointInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& poiPoint
	);

	/**
	 * @brief	指定されたメッシュ内の重なりリンク高さデータを取得
	 * @param	ipMeshGeo	[in]	取得するメッシュ範囲
	 * @param	heightNode	[in/out]	取得した重なりリンク高さ群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetHeightNodeInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& heightNode
	);
	/**
	 * @brief	指定されたメッシュ内の地下街ポリゴンを取得
	 * @param	ipMeshGeo	[in]	取得するメッシュ範囲
	 * @param	underArea	[in/out]	取得した地下街ポリゴン群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetUndergroundInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& underArea
	);

	/**
	 * @brief	歩行者関連テーブルからのデータ取得
	 */
	bool GetWalkRelationTables(void);

	/**
	 * @brief	フィーチャークラスのカーソルとフィールドマップを取得する
	 * @param	tableValue	[in]	取得対象のテーブル
	 * @param	fieldList	[in]	取得対象のフィールド群
	 * @param	ipMeshGeo	[in]	取得するメッシュ範囲
	 * @param	strWhere	[in]	取得条件文
	 * @param	ipCursor	[out]	取得したデータのカーソル
	 * @param	fieldMap	[out]	取得したデータのフィールドマップ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetFeatureCursorAndFieldMap(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const IGeometryPtr& ipMeshGeo,
		const CString& strWhere,
		IFeatureCursorPtr& ipCursor,
		sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	テーブルのカーソルを取得する
	 * @param	tableValue	[in]	取得対象のテーブル
	 * @param	fieldList	[in]	取得対象のフィールド群
	 * @param	ipCursor	[out]	取得したデータのカーソル
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetTableCursor(
		const table tableValue,
		const FIELD_LIST& fieldList,
		_ICursorPtr& ipCursor
	);

	/**
	 * @brief	テーブルのカーソルとフィールドマップを取得する
	 * @param	tableValue	[in]	取得対象のテーブル
	 * @param	fieldList	[in]	取得対象のフィールド群
	 * @param	ipCursor	[out]	取得したデータのカーソル
	 * @param	fieldMap	[out]	取得したデータのフィールドマップ
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetTableCursorAndFieldMap(
		const table tableValue,
		const FIELD_LIST& fieldList,
		_ICursorPtr& ipCursor,
		sindy::CFieldMap& fieldMap
	);

	/**
	 * @brief	テーブルデータを取得する
	 * @param	tableValue	[in]	取得対象のテーブル
	 * @param	fieldList	[in]	取得対象のフィールド群
	 * @param	keyFieldName	[in]	キーフィールド名
	 * @param	dataMap	[out]	取得したデータのmap
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetRelationTable(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const CString& keyFieldName,
		RowsPack& data
	);

	/**
	 * @brief	テーブルデータを取得する
	 * @param	tableValue	[in]	取得対象のテーブル
	 * @param	fieldList	[in]	取得対象のフィールド群
	 * @param	keyFieldName	[in]	キーフィールド名
	 * @param	dataMap	[out]	取得したデータのmultimap
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool GetRelationTable(
		const table tableValue,
		const FIELD_LIST& fieldList,
		const CString& keyFieldName,
		MultiRowsPack& data
	);

	/**
	* @brief	テーブルデータを取得する
	* @note    FeaturesPackのm_ipTableとm_FieldMapをセットして返す
	* @param	tableValue	[in]	取得対象のテーブル
	* @return	FeaturesPack
	*/
	FeaturesPack CreateFeaturesPack(
		table tableValue
	);

	/**
	 * @brief	道路ノードのチェック
	 * @param	meshcode	[in]	チェック対象メッシュコード
	 * @param	ipMeshGeo	[in]	チェック対象メッシュのジオメトリ
	 * @param	roadNode	[in]	チェック対象道路ノード群クラス
	 * @param	walkNode	[in]	チェック参照用歩行者ノード群クラス
	 * @param	walkNodeRTree	[in]	チェック参照用歩行者ノードRTree
	 */
	void CheckRoadNode(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& roadNode,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief	つながりチェック
	 */
	void CheckWalkRel(
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, std::pair<IGeometryPtr, IGeometryPtr>>& cityMeshMap,
		const FeaturesPack& walkNode,
		const NODEIDMAP& walkNodeNodeIDs,
		const NODEIDMAP& walkNodeLinkIDs
	);

	/**
	 * @brief	歩行者通行禁止規制テーブルのチェック(全国)
	 * @param	walkLinkOIDSet	[in]	歩行者リンクのOIDのSet
	 * @param	walkNoPassageMap	[in]	歩行者通行禁止規制テーブル
	 */
	void CheckWalkNoPassageAll(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& walkNoPassage
	);

	/**
	 * @brief	歩行者一方通行規制テーブルのチェック(全国)
	 * @param	walkLinkOIDSet	[in]	歩行者リンクのOIDのSet
	 * @param	walkNoPassageMap	[in]	歩行者一方通行規制テーブル
	 */
	void CheckWalkOnewayAll(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& walkOneway
	);

	/**
	 * @brief	時間規制関連テーブルのLinkID存在チェック
	 * @param	walkLinkOIDSet	[in]	歩行者リンクのOIDのSet
	 * @param	timeRegulationMap	[in]	時間規制関連テーブルのMap
	 * @param	errCode	[in]	出力するエラーコード
	 */
	void CheckLinkIDExist(
		const std::set<long>& walkLinkOIDSet,
		const MultiRowsPack& timeRegulation,
		const err_code::ECode errCode
	);

	/**
	 * @brief	リンク紐付テーブルのチェック(全国)
	 * @param	linkRelOIDSet	[in]	歩行者リンクのOIDのSet
	 * @param	linkRelationMap	[in]	リンク紐付テーブルのMap
	 */
	void CheckLinkRelationAll(
		const std::set<long>& linkRelOIDSet,
		const MultiRowsPack& linkRelation
	);

	/**
	 * @brief	駅出入口ポイントのチェック
	 * @param	meshcode	[in]	チェック対象メッシュコード
	 * @param	ipMeshGeo	[in]	チェック対象のジオメトリ
	 * @param	stationGatePointMap	[in]	チェック対象駅出入口ポイントMap
	 * @param	walkNodeMap	[in]	チェック参照用歩行者ノードMap
	 * @param	walkNodeRTree	[in]	チェック参照用歩行者ノードRTree
	 */
	void CheckStationGatePoint(
		const CString& meshcode,
		const IGeometryPtr& ipMeshGeo,
		const FeaturesPack& stationGatePoint,
		const FeaturesPack& walkNode,
		const BGRTree& walkNodeRTree
	);

	/**
	 * @brief	指定されたNodeIDとリンクを介して接続するNodeIDのSetを取得する。
	 * @param	nodeID	[in]	ノードID
	 * @param	walkNodeNodeIDs	[in]	ノードIDのMap
	 */
	std::set<long> GetRelNodeIDs(
		long nodeID,
		const NODEIDMAP& walkNodeNodeIDs
	);

	/**
	 * @brief	歩行者タイプテーブルからWALK_TYPEの値を取得する。
	 *
	 * @return	取得したWALK_TYPEの値
	 */
	long getWalkTypeValue();

	/**
	 * @brief	指定されたメッシュ内のスクランブルエリアデータを取得
	 * @param	ipMeshGeo       [in]        取得するメッシュ範囲
	 * @param	scrambleArea    [in/out]    取得したスクランブルエリア群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool getScrambleAreaInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& scrambleArea
	);

	/**
	 * @brief	指定されたメッシュ内の横断ポイントデータを取得
	 * @param	ipMeshGeo   [in]        取得するメッシュ範囲
	 * @param	pedxing     [in/out]    取得した横断ポイント群クラス
	 * @retval	true	成功
	 * @retval	false	失敗
	 */
	bool getPedxingInMesh(
		const IGeometryPtr& ipMeshGeo,
		FeaturesPack& pedxing
	);

	/**
	 * @brief all例外モード用のチェック
	 */
	void allCheckException();

	/**
	 * @brief other例外モード用のチェック
	 */
	void otherCheckException();

	/**
	 * @brief  height_node例外モード用のチェック
	 */
	void heightNodeCheckException();

	/**
	 * @brief release例外モード用のチェック
	 */
	void releaseCheckException();

	/**
	 * @brief pedxing例外モード用のチェック
	 */
	void pedxingCheckException();

private:

	CString	m_strStationDB; //!< 駅POI格納先DBの接続文字列
	CString	m_strRoadDB;    //!< 道路NE格納先DBの接続文字列
	CString	m_strWalkDB;    //!< 歩行者NE格納先DBの接続文字列
	CString	m_strMeshDB;    //!< メッシュ格納先DBの接続文字列
	CString	m_strPoiDB;     //!< POI格納先DBの接続文字列
	CString m_strUnderDB;   //!< 地下街ポリゴン格納先DBの接続文字列
	CString	m_strMeshList;
	CString	m_strOutput;
	double	m_dist_threshold;     //!< 微小距離閾値(m)
	double	m_neighbor_threshold; //!< 近傍距離閾値(m)
	double	m_refer_id_threshold; //!< 関連ID距離閾値(m)
	double	m_angle_threshold;    //!< 角度閾値(度)
	/**
	 * @brief	リンク紐付チェック用距離閾値
	 */
	double	m_buddy_dist_threshold;
	/**
	 * @brief	リンク紐付チェック用角度閾値
	 */
	double	m_buddy_angle_threshold;

	double	m_sindyTolerance_threshold; //!< SiNDY許容値(m)
	double  m_microLogicalConnection; //!< 微小ロジカルコネクション閾値(m)

	CString	m_strDistThreshold;
	CString	m_strNeighborThreshold;
	CString	m_strReferIDThreshold;
	CString	m_strAngleThreshold;
	CString m_filterDay;  //!< 編集履歴フィルタの日付部分の設定値を格納
	CString m_filterTime; //!< 編集履歴フィルタの時間部分の設定値を格納

	/**
	 * @brief	リンク紐付チェック用距離閾値(文字列)
	 */
	CString	m_strBuddyDistThreshold;
	/**
	 * @brief	リンク紐付チェック用角度閾値(文字列)
	 */
	CString	m_strBuddyAngleThreshold;

	/**
	 * @brief	重なりリンク高さノードチェック用設定ファイル
	 */
	uh::tstring	m_height_node_ini;
	CString	m_strHeightNodeIni;

	/**
	 * @brief	データベース関連管理クラス
	 */
	CDatabaseControl	m_database;
	COutPut	m_output;
	/**
	 * @brief	重なりリンク高さノード設定管理クラス
	 */
	HeightInfoMgr	m_heightNodeMgr;

	std::set<long>	m_cityMeshList;
	std::set<long>	m_execGateIDs;

	std::map<long, CString> m_walkLinkClassMap;
	std::map<long, CString> m_floorMoveClassMap;
	std::map<long, CString> m_onewayClassMap;
	std::map<long, CString> m_roadSurfaceClassMap;
	std::map<long, CString> m_walkRoadClassMap;
	std::map<long, CString> m_walkNodeClassMap;
	std::map<long, CString> m_walkableMap;

	/**
	 * @brief	歩行者路線コードのmap
	 */
	RowsPack m_walkCodeMap;
	/**
	 * @brief	歩行者通行禁止規制テーブルのmultimap
	 */
	MultiRowsPack m_walkNoPassageMap;
	/**
	 * @brief	歩行者一方通行規制テーブルのmultimap
	 */
	MultiRowsPack m_walkOnewayMap;
	/**
	 * @brief	リンク紐付テーブルのmultimap
	 */
	MultiRowsPack m_linkRelationMap;
	/**
	 * @brief	重なりリンク高さノードのmultimap
	 */
	MultiRowsPack m_heightNodeMap;

	/**
	 * @brief	チェック対象
	 */
	properties::check_target m_checkTarget;
	/**
	 * @brief	チェック対象（文字列）
	 */
	CString	m_strCheckTarget;

	double m_coordinateTolerance;//!< 座標許容値
	CComVariant m_modifyFilter; //!< 編集履歴フィルタ
	CityMeshMap m_cityMeshMap;	//!< <都市地図メッシュコード, <都市地図メッシュ形状, 都市地図メッシュ周辺9メッシュ形状>>
	BaseMeshMap m_baseMeshMap;	//!< <ベースメッシュコード, ベースメッシュ形状>
	sindy::schema::walk_type::type_class::ECode m_dataType;             //!< データタイプ
};

