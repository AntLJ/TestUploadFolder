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

#include "Arguments.h"
#include "OutputLog.h"
#include "FeatureObjects.h"
#include <TDC/sindylib_core/FieldMap.h>

/**
 * @brief   メインクラス
 */
class CCheckRailwayNW
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CCheckRailwayNW( const Arguments& args ){
		m_strRailNwDB        = (args.m_strRailNwDB).c_str();
		m_strRailPoiDB       = (args.m_strRailPoiDB).c_str();
		m_strMeshDB          = (args.m_strMeshDB).c_str();
		m_strStationDB       = (args.m_strStationDB).c_str();
		//m_strMeshLog         = (args.m_strMeshLog).c_str();
		//m_strQueryLog        = (args.m_strQueryLog).c_str();
		m_strLog             = (args.m_strLog).c_str();
		m_strMeshList        = (args.m_strMeshList).c_str();
		m_dist_threshold     = _ttof( ( args.m_dist_threshold ).c_str() );
		m_neighbor_threshold = _ttof( ( args.m_neighbor_threshold ).c_str() );
		//m_meshMode           = args.m_meshMode;
		//m_queryMode          = args.m_queryMode;

		m_strDistThreshold.Format( _T("%s m"), (args.m_dist_threshold).c_str() );
		m_strNeighborThreshold.Format( _T("%s m"), (args.m_neighbor_threshold).c_str() );
	};

	/**
	 * @brief   デストラクタ
	 */
	CCheckRailwayNW(){};

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	typedef std::map<long, std::set<long>> IDMAP;

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	//bool init_mesh();
	//bool init_query();

	/**
	 * @brief   ワークスペースオープン
	 * @param   name           [in]  接続名
	 * @param   ipWorkspace    [in]  ワークスペース
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool OpenWorkspace( const CString& name, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief   フィーチャクラスオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipFeatureClass [out] 取得するフィーチャクラス
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   テーブルオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipTable        [out] 取得するテーブル
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	//与えたメッシュ隣接9メッシュリストを作成する
	void Get9Mesh( long mesh, std::set<long>& meshlist );

	// メッシュリストから、メッシュポリゴンを取得
	bool GetMeshPolygons( long mesh, std::set<long>& meshlist, IGeometryPtr& ipMeshGeo, IGeometryPtr& ipUnionMeshGeo );

	std::vector<CString> CreateWhereByList( const std::set<long>& list, const CString& field );

	/**
	 * @brief   与えられたメッシュ形状内の歩行者リンクと道路ノードを取得
	 * @param   ipMeshGeo  [in]  メッシュポリゴンのジオメトリ
	 * @param   linkMap    [out] 取得した道路リンクMap(キー：ObjectID)
	 * @param   nodeMap    [out] 取得した道路ノードMap(キー：ObjectID)
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool GetRailwayInMesh( 
		const IGeometryPtr& ipMeshGeo, 
		std::map<long, GeoClass>& railLinkMap,
		std::map<long, GeoClass>& railNodeMap,
		IDMAP& nodeNodeIDs,
		IDMAP& nodeLinkIDs
		);
	bool GetObjInMesh( 
		const IGeometryPtr& ipMeshGeo, 
		const IFeatureClassPtr& ipFeatureClass,
		const sindy::CFieldMap& fieldMap,
		std::map<long, GeoClass>& objMap
		);

	bool GetRelRecAll(
		const ITablePtr& ipTable,
		const sindy::CFieldMap& fieldMap,
		const CString& tableName,
		std::map<long, std::vector<recClass>>& recMap
		);
	bool GetOtherRecAll(
		const ITablePtr& ipTable,
		const sindy::CFieldMap& fieldMap,
		const CString& tableName,
		std::map<long, recClass>& recMap
		);

	/**
	 * @brief R-Treeを生成する
	 * @param geoClassMap [in]  対象フィーチャクラス
	 * @param rTree       [out] 結果のR-Tree(冒頭の[r]は参照の意味ではないです)
	 */
	void createRTree( 
		const std::map<long, GeoClass>& geoMap,
		BGRTree& rTree
		);

	/**
	 * @brief   R-Treeから、与えた箱の範囲に引っかかるID群を取得
	 * @param   box   [in]    取得したい範囲(エンベロープ)
	 * @param   rTree [in]    取得してくる対象のR-Tree
	 * @retval  set<long>     対象範囲のID群
	 */
	std::set<long> getIntersectsIDs( const BGBox& box, const BGRTree& rTree );

	/**
	 *
	 */
	void MeshCheck( 
		long mesh,
		const IGeometryPtr& ipMeshGeo,
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const std::map<long, GeoClass>& staPointMap,
		const std::map<long, GeoClass>& baseStaMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const BGRTree& staPointRTree,
		const BGRTree& baseStaRTree,
		const IDMAP& nodeNodeIDs,
		const IDMAP& nodeLinkIDs
		);

	void CheckRailLink(
		const IGeometryPtr& ipMeshGeo,
		const IGeometryPtr& ipBoundaryMesh, 
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const IDMAP& nodeLinkIDs
		);

	void CheckRailNode(
		const IGeometryPtr& ipMeshGeo, 
		const IGeometryPtr& ipBoundaryMesh, 
		const std::map<long, GeoClass>& railLinkMap,
		const std::map<long, GeoClass>& railNodeMap,
		const std::map<long, GeoClass>& staPointMap,
		const std::map<long, GeoClass>& baseStaMap,
		const BGRTree& railLinkRTree,
		const BGRTree& railNodeRTree,
		const BGRTree& staPointRTree,
		const BGRTree& baseStaRTree,
		const IDMAP& nodeLinkIDs
		);

	bool FindJoinLineInfo( long railNodeID, std::set<long>& stationIDs, std::set<long>& lineIDs );
	bool FindRailwayLine( long railLinkID, std::set<long>& railwayLineIDs );
	// IDセットと自身のID、Objectマップを渡し、自身以外のオブジェクトを取得
	std::list<GeoClass> GetObjects( 
		long oID,
		const std::set<long>& oIDs,
		const std::map<long, GeoClass>& geoMap
		);

	void QueryCheck();

	void CheckStaNumInLine();

	void CheckNonRel(const ITablePtr& ipTable, const CString& fcTableName, const CString& relTableName, const std::map<long, std::vector<recClass>>& relAll );

	bool GetOIDAll(
		const ITablePtr& ipTable,
		const CString& tableName,
		std::set<long>& oIDs
		);

	void SameLineSeqLinkChk();

	/**
	 * @brief   指定したフィーチャクラスの関連テーブルと紐付く鉄道POIのテーブルのレコードが存在するかチェックする
	 * @param   featureClassName [in]  フィーチャクラス名
	 * @param   relRecMap        [in]  関連テーブルのレコード群
	 * @param   relPoiMap        [in]  鉄道POIのテーブルのレコード群
	 */
	void checkRelRecordExstence(const CString& featureClassName, const std::map<long, std::vector<recClass>>& relRecMap, const std::map<long, recClass>& poiRecMap);

	// 自身につながるノード列を作成する
	std::set<long> GetRelNodeIDs( long nodeID, const IDMAP& nodeNodeIDs );

	CString GetLineName( long lineID )
	{
		using namespace sindy::schema::railway_line;
		auto itr = m_railwayLine.find( lineID );
		if( itr == m_railwayLine.end() )
			return _T("");
		return itr->second.attr.at( kName );
	};

private:

	CString           m_strRailNwDB;        //!< 鉄道NW接続先
	CString           m_strRailPoiDB;       //!< 鉄道POI接続先
	CString           m_strMeshDB;          //!< メッシュDB接続先
	CString           m_strStationDB;       //!< 中縮駅舎接続先
	CString           m_strLog;             //!< ログファイル
	//CString           m_strMeshLog;         //!< メッシュモードログファイル
	//CString           m_strQueryLog;        //!< クエリモードログファイル
	CString           m_strMeshList;        //!< メッシュリスト
	CString           m_strDistThreshold;   //!< 微小距離閾値(m)
	CString           m_strNeighborThreshold; //!< 近傍距離閾値(m)

	double            m_dist_threshold;     //!< 微小距離閾値(m)
	double            m_neighbor_threshold; //!< 近傍距離閾値(m)

	//bool              m_meshMode;           //!< メッシュモード
	//bool              m_queryMode;          //!< クエリモード

	IWorkspacePtr     m_ipRailNwWorkspace;  //!< 鉄道NWワークスペース
	IWorkspacePtr     m_ipMeshWorkspace;    //!< メッシュワークスペース
	IWorkspacePtr     m_ipRailPoiWorkspace; //!< 鉄道POIワークスペース
	IWorkspacePtr     m_ipStationWorkspace; //!< 収縮駅舎ワークスペース

	IFeatureClassPtr  m_ipRailLinkFC;       //!< 鉄道リンクフィーチャクラス
	IFeatureClassPtr  m_ipRailNodeFC;       //!< 鉄道ノードフィーチャクラス
	ITablePtr         m_ipRelRailLinkTable; //!< 鉄道リンク紐付きテーブル
	ITablePtr         m_ipRelRailNodeTable; //!< 鉄道ノード紐付きテーブル

	IFeatureClassPtr  m_ipStationPointFC;   //!< 駅ポイントフィーチャクラス
	ITablePtr         m_ipRailwayLineTable; //!< 鉄道路線テーブル
	ITablePtr         m_ipJoinLineTable;    //!< 乗り入れ路線テーブル

	IFeatureClassPtr  m_ipBaseStationFC;    //!< 中縮駅舎フィーチャクラス

	IFeatureClassPtr  m_ipBaseMeshFC;       //!< ベースメッシュフィーチャクラス

	sindy::CFieldMap  m_railLinkFM;         //!< 鉄道リンクフィールドマップ
	sindy::CFieldMap  m_railNodeFM;         //!< 鉄道ノードフィールドマップ
	sindy::CFieldMap  m_relRailLinkFM;      //!< 鉄道リンク紐付きテーブルフィールドマップ
	sindy::CFieldMap  m_relRailNodeFM;      //!< 鉄道ノード紐付きテーブルフィールドマップ
	sindy::CFieldMap  m_baseMeshFM;         //!< ベースメッシュフィールドマップ
	sindy::CFieldMap  m_stationPointFM;     //!< 駅ポイントフィールドマップ
	sindy::CFieldMap  m_railwayLineFM;      //!< 鉄道路線テーブルフィールドインデックス
	sindy::CFieldMap  m_joinLineFM;         //!< 乗り入れ路線テーブルフィールドインデックス
	sindy::CFieldMap  m_baseStationFM;      //!< 中縮駅舎フィールドマップ

	COutPut           m_output;
	std::set<long>    m_meshList;

	std::map<long, CString> m_railLinkClassMap;
	std::map<long, CString> m_dispScaleClassMap;
	std::map<long, CString> m_railNodeClassMap;

	std::map<long, std::vector<recClass>> m_relRailwayLink;  //!< 鉄道リンク紐付きテーブル格納用（キー：RW_LINK_ID）
	std::map<long, std::vector<recClass>> m_relRailwayNode;  //!< 鉄道ノード紐付きテーブル格納用（キー：RW_NODE_ID）
	std::map<long, recClass> m_joinLineInfo;    //!< 乗り入れ路線テーブル格納用（キー：ObjectID）
	std::map<long, recClass> m_railwayLine;     //!< 鉄道路線テーブル格納用（キー：ObjectID）

//	std::map<CString, std::vector<CString>> m_execFields;
};

