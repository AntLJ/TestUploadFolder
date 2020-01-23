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

#include "stdafx.h"
#include "Arguments.h"
#include "Logger.h"
#include <TDC/sindylib_core/FieldMap.h>



typedef std::map<CString, IFeaturePtr> FeatureMap;
typedef std::list<IFeaturePtr> FeatureList;
typedef std::list<CString> GidList;
typedef std::set<CString> GidSet;
typedef std::map<CString, GidSet> Node2LinkMap;
typedef std::multimap<long, CString> LRBorderMap;
typedef std::map<CString, LRBorderMap> LaneBorderRelMap;
typedef std::set<long> OidSet;
typedef std::map<CString, std::map<CString, long>> Link2NodeMap;

typedef std::list<long> MeshList;
typedef std::map<long, IGeometryPtr> MeshGeoMap;

/**
 * @brief   メインクラス
 */
class CCorrectZTool
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	explicit CCorrectZTool( const Arguments& args ) : m_args( args ){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CCorrectZTool(){}

	/**
	 * @brief   実行
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool execute();

private:

	/**
	 * @brief   初期化
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool init();

	/**
	 * @brief	車線リンク検索
	 * @note	メッシュ内（跨る場合も含む）の車線リンクを引っ張り出す
	 * @param	mesh			[in] メッシュ
	 * @param	ipcLaneLinkGids	[out] iPCリンク
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool searchLaneLink( const IGeometryPtr& mesh, GidList& ipcLaneLinkGids );

	/**
	 * @brief	関連テーブル検索
	 * @note	車線リンクに紐付く道路境界線リンクを関連テーブルから探し出す
	 * @param	laneLinkGids	[in] 車線リンク
	 * @param	borderLinkGids	[out] 道路境界線リンク
	 * @param	lane2border		[out] 車線リンクと道路境界線リンクの対応表
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool searchRelTable( const GidList& laneLinkGids, GidList& borderLinkGids, LaneBorderRelMap& lane2border );

	/**
	 * @brief	道路境界線リンク検索
	 * @note	実際に道路境界線リンクを引っ張り出す
	 * @param	borderLinkGids	[in] 道路境界線リンク（GIDリスト）
	 * @param	borderLinks		[out] 道路境界線リンク（フィーチャマップ）
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool searchBorderLink( const GidList& borderLinkGids, FeatureMap& borderLinks );

	/**
	 * @brief	車線リンク更新
	 * @note	道路境界線リンクを使って車線リンクのZ値を更新する
	 * @param	laneLinkGids	[in] 車線リンク
	 * @param	atLaneNodeGids	[in] ATノード
	 * @param	lane2border		[in] 車線リンクと道路境界線リンクの対応表
	 * @param	borderLinks		[in] 道路境界線リンク（フィーチャマップ）
	 * @param	meshCode		[in] メッシュコード
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool updateLaneLink( const GidList& laneLinkGids, const GidSet& atLaneNodeGids, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode );

	/**
	 * @brief	NQテーブル検索
	 * @note	車線リンクに紐付く車線ノードをノード列テーブルから探し出す
	 * @param	laneLinkGids	[in] 車線リンク
	 * @param	laneNodeGids	[out] 車線ノード
	 * @param	node2link		[out] 車線ノードと車線リンクの対応表
	 * @param	link2node		[out] 車線リンクと車線ノードの対応表（ノードはSEQUENCE付き）
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool searchNqTable( const GidList& laneLinkGids, GidList& laneNodeGids, Node2LinkMap& node2link, Link2NodeMap& link2node );

	/**
	 * @brief	車線ノード検索
	 * @note	実際に車線ノードを引っ張り出す
	 * @param	laneNodeGids	[out] 車線ノード
	 * @param	atLaneNodeGids	[out] ATノード
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool searchLaneNode( const GidList& laneNodeGids, GidSet& atLaneNodeGids );

	/**
	 * @brief	車線ノード更新
	 * @note	道路境界線リンクを使って車線ノードのZ値を更新する
	 * @param	laneNodeGids	[in] 車線リンク
	 * @param	node2link		[in] 車線ノードと車線リンクの対応表
	 * @param	lane2border		[in] 車線リンクと道路境界線リンクの対応表
	 * @param	borderLinks		[in] 道路境界線リンク（フィーチャマップ）
	 * @param	meshCode		[in] メッシュコード
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool updateLaneNode( const GidList& laneNodeGids, const Node2LinkMap& node2link, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long meshCode );

	/**
	 * @brief	Z値更新処理
	 * @note	リンク/ノードの共通処理を無理やり関数化
	 * @param	ipFeature		[in/out] フィーチャー
	 * @param	ipUpdateCur		[in/out] カーソル
	 * @param	linkGids		[in] 車線リンクのGID
	 * @param	lane2border		[in] 車線リンクと道路境界線リンクの対応表
	 * @param	borderLinks		[in] 道路境界線リンク（フィーチャマップ）
	 * @param	skipPointNums	[in] Z値更新をスキップしたい構成点番号（車線リンク更新時のみ使用）
	 * @param	layer			[in] レイヤ
	 * @param	fieldMap		[in] フィールドマップ
	 * @param	meshCode		[in] メッシュコード
	 */
	void updateRun(
		IFeaturePtr& ipFeature,
		IFeatureCursorPtr& ipUpdateCur,
		const GidSet& linkGids,
		const LaneBorderRelMap& lane2border,
		const FeatureMap& borderLinks,
		const std::set<long>& skipPointNums,
		const CString layer,
		const sindy::CFieldMap& fieldMap,
		long meshCode
	);

	/**
	 * @brief	更新したノードに紐づくリンク同士のZ値が一致しているか確認し、一致していないリンクの構成点はノードのZ値で補正する
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool doConsistentZ();

	/**
	 * @brief	更新したノードを使って、紐づくリンクを取得する
	 * @param	field		[in] フィールド
	 * @param	ids			[in] IN句に使うGIDの帯
	 * @param	node2Links	[out] ノードとリンクの対応表
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool getNode2Links( const CString& field, const CString& ids, std::map<CString, std::map<CString, std::vector<IFeaturePtr>>>& node2Links ) const;

	/**
	 * @brief	紐づくリンク同士のZ値が一致しているか確認し、不一致な場合はリンク同士を接続させる
	 * @note	修正した結果、急勾配になってしまったリンクも、後で線形補間するために確保する
	 * @param	ipFeature		[in] ノード
	 * @param	field2Links		[in] フィールドとリンク
	 * @param	linkInfo		[out] 更新が必要なリンク情報
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool connectLink( const IFeaturePtr& ipFeature, const std::map<CString, std::vector<IFeaturePtr>>& field2Links, std::map<CString, std::pair<IGeometryPtr, CString>>& linkInfo );

	/**
	 * @brief	接続させたリンクを更新する
	 * @param	ids				[in] IN句に使うGIDの帯
	 * @param	linkInfo		[in] 更新が必要なリンク情報
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool reUpdateLinkZ( const CString& ids, std::map<CString, std::pair<IGeometryPtr, CString>> linkInfo );

	/**
	 * @brief	急勾配のリンクに対して線形補間を試みる
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool doLinerInterpolation();

	/**
	 * @brief	線形補間したリンクを更新する
	 * @param	linkGids		[in] 車線リンクのGID
	 * @param	linkGeomMap		[in] 線形補間したリンク情報
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool reUpdateLinkZ( const GidList& linkGids, const std::map<CString, IGeometryPtr>& linkGeomMap );

	/**
	 * @brief	線形補間したリンクの構成点に乗っかるノードを更新する
	 * @param	nodeGids		[in] 車線ノードのGID
	 * @param	node2link		[in] 車線ノードと車線リンクの対応表
	 * @param	link2node		[in] 車線リンクと車線ノードの対応表（ノードはSEQUENCE付き）
	 * @param	linkGeomMap		[in] 線形補間したリンク情報
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool reUpdateNodeZ( const GidList& nodeGids, const Node2LinkMap& node2link, const Link2NodeMap& link2node, const std::map<CString, IGeometryPtr>& linkGeomMap );

	/**
	 * @brief	道路境界線リンク取得
	 * @param	linkGid			[in] 車線リンクのGID
	 * @param	lane2border		[in] 車線リンクと道路境界線リンクの対応表
	 * @param	borderLinks		[in] 道路境界線リンク（フィーチャマップ）
	 * @param	lrCode			[in] 左右コード
	 * @param	borders			[out] 左右コードに応じた道路境界線リンク
	 */
	void getBorderLink( const CString& linkGid, const LaneBorderRelMap& lane2border, const FeatureMap& borderLinks, long lrCode, FeatureList& borders );

	/**
	 * @brief	Z値更新処理
	 * @param	ipFeature		[in/out] フィーチャー
	 * @param	ipGeo			[in] ジオメトリ
	 * @param	fieldMap		[in] フィールドマップ
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool putValue( IFeaturePtr& ipFeature, const IGeometryPtr& ipGeo, const sindy::CFieldMap& fieldMap );

	/**
	 * @brief	IN句で使用する文字列を作成する
	 * @param	target			[in] 要素（文字列）
	 * @param	sep				[in] 要素を囲う文字
	 * @return	文字列
	 */
	std::list<CString> getString( const std::list<CString>& target, const CString& sep = _T("'") );

	/**
	 * @brief	IN句で使用する文字列を作成する
	 * @param	target			[in] 要素（数値）
	 * @return	文字列
	 */
	std::list<CString> getString( const std::list<long>& target );



	/**
	 * @brief	ワークスペース取得
	 * @param	db			[in] 接続先DB
	 * @param	ipWorkspace	[in] ワークスペース
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool openWorkspace( const CString& db, IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	デフォルトバージョンか
	 * @param	ipWorkspace	[in] ワークスペース
	 * @retval	true  デフォルト
	 * @retval	false それ以外
	 */
	bool isDefaultVersion( const IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	メッシュリスト取得
	 * @param	fileName	[in] ファイル
	 * @param	meshList	[out] メッシュリスト
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool getMeshList( const CString& fileName, MeshList& meshList );

	/**
	 * @brief	メッシュポリゴン取得
	 * @param	meshList	[in] メッシュリスト
	 * @param	meshGeoMap	[out] メッシュジオメトリマップ
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap );

	/**
	 * @brief	テーブル取得
	 * @param	ipWorkspace	[in] ワークスペース
	 * @param	tableName	[in] テーブル名
	 * @param	fieldMap	[out] フィールドマップ
	 * @return	テーブル
	 */
	ITablePtr openTable( const IWorkspacePtr& ipWorkspace, const CString& tableName, sindy::CFieldMap& fieldMap );

	/**
	 * @brief	バージョン対応レイヤか
	 * @param	ipFeatureClass	[in] フィーチャクラス
	 * @retval	true  バージョン対応
	 * @retval	false バージョン非対応
	 */
	bool isVersioned( const IFeatureClassPtr& ipFeatureClass );

	/**
	 * @brief	編集開始
	 * @param	ipWorkspace	[in/out] ワークスペース
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool startEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	編集終了
	 * @param	ipWorkspace	[in/out] ワークスペース
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool stopEdit( IWorkspacePtr& ipWorkspace );

	/**
	 * @brief	編集破棄
	 * @param	ipWorkspace	[in/out] ワークスペース
	 */
	static void abortEdit( IWorkspacePtr& ipWorkspace );

private:

	Arguments m_args;									//!< 引数
	IWorkspacePtr m_ipWorkspace;						//!< ワークスペース
	IWorkspacePtr m_ipMeshWorkspace;					//!< メッシュワークスペース

	IFeatureClassPtr m_ipMesh;							//!< メッシュ
	IFeatureClassPtr m_ipLaneLink;						//!< 車線リンク
	IFeatureClassPtr m_ipLaneNode;						//!< 車線ノード
	IFeatureClassPtr m_ipBorderLink;					//!< 道路境界線リンク
	ITablePtr m_ipRelLaneLinkBorderLink;				//!< 車線リンク－道路境界線リンク関連
	ITablePtr m_ipNqLaneNode;							//!< 車線ノード列

	sindy::CFieldMap m_cMeshFM;							//!< フィールドマップ（メッシュ）
	sindy::CFieldMap m_cLaneLinkFM;						//!< フィールドマップ（車線リンク）
	sindy::CFieldMap m_cLaneNodeFM;						//!< フィールドマップ（車線ノード）
	sindy::CFieldMap m_cBorderLinkFM;					//!< フィールドマップ（道路境界線リンク）
	sindy::CFieldMap m_cRelLaneLinkBorderLinkFM;		//!< フィールドマップ（車線リンク－道路境界線リンク関連）
	sindy::CFieldMap m_cNqLaneNodeFM;					//!< フィールドマップ（車線ノード列）

	MeshList m_meshList;								//!< メッシュリスト
	MeshGeoMap m_meshGeoMap;							//!< メッシュジオメトリマップ

	CString m_toolName;									//!< ツール名

	GidSet m_updatedList;								//!< 更新したフィーチャー
	FeatureMap m_updatedNode;							//!< 更新した車線ノードの情報
	GidSet m_steepLink;									//!< 急勾配になった車線リンク
	OidSet m_fixLink;									//!< 線形補間処理で経路探索対象外にするリンク
};

