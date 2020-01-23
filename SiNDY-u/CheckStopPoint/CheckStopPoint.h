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
#include "LogManager.h"
#include <TDC/sindylib_core/FieldMap.h>

class CheckStopPoint
{
public:
	/**
	 * @brief   道路リンク保持用構造体
	 */
	struct LINK {
		long roadClass;
		long linkClass;
		long upDownClass;
		long roadWidth;
		long fromID;
		long toID;
		bool foreOneway;
		bool revOneway;
		bool noPassage;
		bool outOfMesh;     //!< メッシュと接しているが、処理対象メッシュ外
		IGeometryPtr ipGeo;

		LINK(){
			roadClass    = -1;
			linkClass    = -1;
			upDownClass  = -1;
			roadWidth    = -1;
			fromID       = -1;
			toID         = -1;
			foreOneway   = false;
			revOneway    = false;
			noPassage    = false;
			outOfMesh    = false;
		};
	};

	/**
	 * @brief   道路ノード保持用構造体
	 */
	struct NODE {
		long nodeClass;
		long signal;
		std::list<long> linkIDs; //!< 接続リンク数
		IGeometryPtr ipGeo;

		NODE(){
			nodeClass = -1;
			signal    = -1;
		};
	};

	/**
	 * @brief   一時停止保持用構造体
	 */
	struct STOP {
		long linkID;
		long nodeID;
		IPointPtr ipPoint;

		STOP(){
			linkID = -1;
			nodeID = -1;
		};

		bool operator == ( const STOP& obj ) const {
			return ( linkID == obj.linkID && nodeID == obj.nodeID );
		};

		bool operator < ( const STOP& obj ) const {
			if( linkID != obj.linkID )
				return ( linkID < obj.linkID );
			return ( nodeID < obj.nodeID );
		};
	};

	typedef std::map<long, LINK> LINKMAP; //!< key:リンクID、 value:リンク属性情報
	typedef std::map<long, NODE> NODEMAP; //!< key:ノードID、 value:ノード属性情報
	typedef std::map<long, STOP> STOPMAP; //!< key:一時停止ポイントのオブジェクトID、value:一時停止属性情報

public:
	/// コンストラクタ
	CheckStopPoint( const Arguments& args ){
		m_strDBRoad      = (args.m_db_road).c_str();
		m_strDBRoad_past = (args.m_db_road_past).c_str();
		m_strDBStop      = (args.m_db_stop).c_str();
		m_strDBMesh      = (args.m_db_mesh).c_str();
		m_strOutput      = (args.m_output).c_str();
		m_strMeshlist    = (args.m_meshlist).c_str();
		m_extract        = args.m_extract;
		m_check          = args.m_check;
	};

	/// デストラクタ
	~CheckStopPoint(){};

	/**
	 * @brief   処理メイン関数
	 * @retval  true:  処理成功
	 * @retval  false: 処理失敗
	 */
	bool execute( );

	/**
	 * @brief   各種初期化
	 * @retval  true:  初期化成功
	 * @retval  false: 初期化失敗
	 */
	bool init();

	/**
	 * @brief   フィーチャクラスオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipFeatureClass [out] 取得するフィーチャクラス
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @return  true:成功  false:失敗
	 */
	bool OpenFeatureClass( const CString& name, const IWorkspacePtr& ipWorkspace, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   テーブルオープン＆フィールドマップ作成
	 * @param   name           [in]  フィーチャクラス名
	 * @param   ipWorkspace    [in]  オープンするためのワークスペース
	 * @param   ipTable        [out] 取得するテーブル
	 * @param   fieldMap       [out] 取得するフィールドマップ
	 * @return  true:成功  false:失敗
	 */
	bool OpenTable( const CString& name, const IWorkspacePtr& ipWorkspace, ITablePtr& ipTable, sindy::CFieldMap& fieldMap );

	/**
	 * @brief   メッシュポリゴン取得
	 * @param   mesh      [in]  2次メッシュコード
	 * @param   ipMeshGeo [out] メッシュポリゴンのジオメトリ
	 * @retval  true:  成功
	 * @retval  false: 失敗
	 */
	bool GetMeshPolygon( long mesh, IGeometryPtr& ipMeshGeo );

	/**
	 * @brief   メッシュ内の既存一時停止を取得
	 * @param   ipMeshGeo [in]  メッシュポリゴンのジオメトリ
	 * @param   stops     [out] 流入リンクIDと流出ノードIDのset 
	 * @retval  true:  取得成功
	 * @retval  false: 取得失敗
	 */
	bool GetStopInMesh( const IGeometryPtr& ipMeshGeo, STOPMAP& stops );

	/**
	 * @brief   メッシュ内の道路リンクと道路ノードを取得
	 * @note    ついでに、ノードに対する接続リンクも保持する
	 * @param   ipMeshGeo [in]  メッシュポリゴンのジオメトリ
	 * @param   linkMap   [out] 取得した道路リンクMap(キー：リンクのObjectID)
	 * @param   nodeMap   [out] ノードに対する接続リンクIDsのMap(キー：ノードのObjectID)
	 * @param   ipLinkFC  [in]  対象とする道路リンクフィーチャクラス
	 * @param   linkFM    [in]  linkFCのフィールドマップ
	 * @param   ipNodeFC  [in]  対象とする道路ノードフィーチャクラス
	 * @param   nodeFM    [in]  nodeFCのフィールドマップ
	 * @retval  true:  成功
	 * @retval  false: 失敗
	 */
	bool GetLinkNodeInMesh( const IGeometryPtr& ipMeshGeo, LINKMAP& linkMap, NODEMAP& nodeMap,
		IFeatureClassPtr& ipLinkFC, sindy::CFieldMap& linkFM, IFeatureClassPtr& ipNodeFC, sindy::CFieldMap& nodeFM );

	/**
	 * @brief   抽出処理メイン関数
	 * @param   linkMap  [in]  道路リンクMap
	 * @param   nodeMap  [in]  道路ノードMap
	 * @param   stops    [in]  既存一時停止のSet
	 * @param   mesh     [in]  メッシュコード(ログ用)
	 */
	void Extract( const LINKMAP& linkMap, const NODEMAP& nodeMap, const STOPMAP& stops, long mesh );

	/**
	 * @brief   チェック処理メイン関数
	 * @param   linkMap       [in]  道路リンクMap
	 * @param   nodeMap       [in]  道路ノードMap
	 * @param   linkMap_past  [in]  経年変化前の道路リンクMap
	 * @param   nodeMap_past  [in]  経年変化前の道路ノードMap
	 * @param   stops         [in]  既存一時停止のSet
	 * @param   mesh          [in]  メッシュコード(ログ用)
	 */
	void ErrorCheck( const LINKMAP& linkMap, const NODEMAP& nodeMap, const LINKMAP& linkMap_past, const NODEMAP& nodeMap_past, const STOPMAP& stops, long mesh );

	/**
	 * @brief   存在しないものと見なす道路種別か？
	 * @note    roadclass_c >= 201 の道路リンクは、存在しないものとしてみなす。
	 * @param   roadClass [in]  道路種別
	 * @retval  true:  存在しないものと見なす
	 * @retval  false: 普通に処理対象
	 */
	bool IsNoTarget( long roadClass );

	/**
	 * @brief   リンクIDとノードIDの組み合わせが既存一時停止に存在するか？
	 * @param   linkID  [in]  リンクのObjectID
	 * @param   nodeID  [in]  ノードのObjectID
	 * @param   stops   [in]  既存一時停止のSet
	 * @retval  true:  存在する
	 * @retval  false: 存在しない
	 */
	bool FindStopPoint( long linkID, long nodeID, const STOPMAP& stops );


	/**
	 * @brief   リンクとノードの関係から、抽出条件を満たすか確認する
	 * @param   link     [in]  リンクの情報を保持した構造体
	 * @param   nodeID   [in]  ノードのObjectID
	 * @param   links    [in]  リンクMap
	 * @param   nodeMap  [in]  ノードMap
	 * @retval  true:  抽出条件を満たす(抽出対象)
	 * @retval  false: 抽出条件を満たさない
	 */
	bool IsExtractTarget( const LINK& link, long nodeID, const LINKMAP& links, const NODEMAP& nodeMap );

	/**
	 * @brief   リンクIDからリンクを取得し、主要地方道以上か確認する
	 * @note    主要地方道以上：updownclass_c not in (0,6) or (updownclass_c in (0,6) and roadclass_c in (1,2,3,103,104,105))
	 * @param   links   [in]  道路リンク情報群
	 * @param   linkID  [in]  リンクのObjectID
	 * @retval  true:  主要地方道以上リンク
	 * @retval  false: 上記以外
	 */
	bool IsPrefOrOver( const LINKMAP& links, long linkID );

	/**
	 * @brief   リンクIDからリンクを取得し、国道以上か確認する
	 * @note    国道以上：updownclass_c not in (0,6) or (updownclass_c in (0,6) and roadclass_c in (1,103))
	 * @param   links   [in]  道路リンク情報群
	 * @param   linkID  [in]  リンクのObjectID
	 * @retval  true:  国道以上リンク
	 * @retval  false: 上記以外
	 */
	bool IsCountryOrOver( const LINKMAP& links, long linkID );

	/**
	 * @brief   自転車道ではなく(リンク種別(not 8))＆HWYではない(ハイウェイ路線(0,6))か確認する
	 * @param   link  [in]  リンクの情報を保持した構造体
	 * @retval  true:  not 自転車道 && not HWY
	 * @retval  false: 上記以外
	 */
	bool IsNotBicycleNotHWY( const LINK& link );

	/**
	 * @brief   通行禁止がついていないか、一方通行の向きが問題ないか確認する
	 * @param   link   [in]  リンクの情報を保持した構造体
	 * @param   nodeID [in]  ノードID
	 * @retval  true:  問題なし（一時停止付与対象）
	 * @retval  false: 問題あり
	 */
	bool IsRegOK( const LINK& link, long nodeID );

	/**
	 * @brief   リンクが環状交差点であるかを判定する
	 * @param   link   [in]  リンクの情報を保持した構造体
	 * @retval  true:  環状交差点である
	 * @retval  false: 環状交差点でない
	 */
	bool IsRoundAbout( const LINK& link );

	/**
	 * @brief   リンクが本線（上下分離）リンクであるかを判定する
	 * @param   link   [in]  リンクの情報を保持した構造体
	 * @retval  true:  本線（上下分離）リンクである
	 * @retval  false: 本線（上下分離）リンクでない
	 */
	bool IsMainUpDown( const LINK& link );

	/**
	 * @brief   リンクがHWY本線であるかを判定する
	 * @param   link   [in]  リンクの情報を保持した構造体
	 * @retval  true:  HWY本線である
	 * @retval  false: HWY本線でない
	 */
	bool IsHighwayMain( const LINK& link );

	/**
	 * @brief 2次メッシュ境界ノードかどうかを判定する
	 * @param node  [in]   道路ノードの情報を格納した構造体
	 * @retval true  メッシュ境界ノードである
	 * @retval false メッシュ境界ノードではない
	 */
	bool IsMeshEdge( const NODE& node );

	/**
	* @brief 交差点ノードかどうかを判定するを判定する
	* @param node  [in]   道路ノードの情報を格納した構造体
	* @retval true  交差点ノードである
	* @retval false 交差点ノードではない
	*/
	bool IsCross(const NODE& node);

	/**
	 * @brief 道路ノードが信号機付き交差点であるかどうかを判定する
	 * @param node  [in]   道路ノードの情報を格納した構造体
	 * @retval true  信号機付き交差点である
	 * @retval false 信号機付き交差点ではない
	 */
	bool IsSignalCross( const NODE& node );

	/**
	 * @brief   2つのリンクがなす角度を求める
	 * @param   link1  [in]  リンク1
	 * @param   link2  [in]  リンク2
	 * @retval  double: 角度(0～180°)
	 */
	double GetAngle( const LINK& link1, const LINK& link2 );

	/**
	 * @brief  一時停止ポイントを生成する関数
	 * @note   一時停止付与ツールと同様のアルゴリズムで生成される
	 * @param  linkID  [in] 流入リンクID
	 * @param  nodeID  [in] 流出ノードID
	 * @param  link    [in] 流入リンク情報
	 * @param  node    [in] 流出ノード情報
	 * @param  ipSpRef [in] 一時停止ポイントに当てる空間参照
	 * @return 一時停止ポイント付与ツールと同じアルゴリズムで生成されたポイント
	 */
	IGeometryPtr CreateStopPointGeometry(long linkID, long nodeID, const LINK& link,  const NODE& node, ISpatialReferencePtr ipSpRef);

private:

	CString          m_strDBRoad;          //!< 道路DB接続文字列
	CString          m_strDBRoad_past;     //!< 経年変化前の道路DB接続文字列
	CString          m_strDBStop;          //!< 一時停止DB接続文字列
	CString          m_strDBMesh;          //!< メッシュDB接続文字列
	CString          m_strOutput;          //!< ログパス
	CString          m_strMeshlist;        //!< メッシュリストパス
	bool             m_extract;            //!< 抽出モードか
	bool             m_check;              //!< チェックモードか

	IWorkspacePtr    m_ipRoadWorkspace;      //!< 道路DBワークスペース
	IWorkspacePtr    m_ipRoadPastWorkspace;  //!< 道路DBワークスペース
	IWorkspacePtr    m_ipStopWorkspace;      //!< 一時停止DBワークスペース
	IWorkspacePtr    m_ipMeshWorkspace;      //!< メッシュDBワークスペース

	IFeatureClassPtr m_ipRoadLinkFC;         //!< 道路リンクフィーチャクラス
	IFeatureClassPtr m_ipRoadLinkFC_past;    //!< 経年変化前の道路リンクフィーチャクラス
	IFeatureClassPtr m_ipRoadNodeFC;         //!< 道路ノードフィーチャクラス
	IFeatureClassPtr m_ipRoadNodeFC_past;    //!< 経年変化前の道路ノードフィーチャクラス
	IFeatureClassPtr m_ipStopPointFC;        //!< 一時停止フィーチャクラス
	IFeatureClassPtr m_ipBaseMeshFC;         //!< ベースメッシュフィーチャクラス

	sindy::CFieldMap m_RoadLinkFM;           //!< 道路リンクフィールドマップ
	sindy::CFieldMap m_RoadNodeFM;           //!< 道路ノードフィールドマップ
	sindy::CFieldMap m_RoadLinkFM_past;      //!< 経年変化前の道路リンクフィールドマップ
	sindy::CFieldMap m_RoadNodeFM_past;      //!< 経年変化前の道路ノードフィールドマップ
	sindy::CFieldMap m_StopPointFM;          //!< 一時停止フィールドマップ
	sindy::CFieldMap m_BaseMeshFM;           //!< ベースメッシュフィールドマップ

	CLogManager      m_logManager;           //!< ログ管理クラス

	std::set<long>   m_meshList;             //!< メッシュリスト
};
