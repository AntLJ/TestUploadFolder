#pragma once

#include "stdafx.h"
#include <TDC/sindylib_core/FieldMap.h>
#include "Arguments.h"
#include "Logger.h"

// RTreeのメモリリーク回避のため、boost 1.56を使用
// 切り替えてビルドすること
#include <boost/version.hpp>

#if BOOST_VERSION != 105600
#error "RTreeのメモリリーク対策のため、boost 1.56でビルドして下さい。"
#endif

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <boost/next_prior.hpp>

#pragma warning(disable:4503)

typedef double coordinateType;
static const size_t dimension = 2;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<coordinateType, dimension, bg::cs::cartesian> BGPoint;
typedef bg::model::box<BGPoint> BGBox;
typedef std::pair<BGBox, unsigned> value;
typedef bgi::rtree<value, bgi::quadratic<16>> BGRTree;


typedef std::map<long, IFeaturePtr> FeatureMap;
typedef std::set<unsigned int> MeshList;
typedef std::map<unsigned int, IGeometryPtr> MeshGeoMap;
typedef std::pair<unsigned int, IGeometryPtr> MeshGeo;


/**
 * @brief   メインクラス
 */
class CBuildOid2Step
{

public:

	/**
	 * @brief   コンストラクタ
	 * @param   args [in]  引数
	 */
	CBuildOid2Step( const Arguments& args ) : m_args( args ), m_bufDistance(0.0){}

	/**
	 * @brief   デストラクタ
	 */
	virtual ~CBuildOid2Step(){};

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
	 * @brief   建物ポリゴン検索
	 * @param	ipBufferedMesh	[in] バッファ付与済みメッシュ
	 * @param	rTree			[out] 空間キャッシュ
	 * @param	featureMap		[out] フィーチャマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool searchBuild( const IGeometryPtr& ipBufferedMesh, BGRTree& rTree, FeatureMap& featureMap );

	/**
	 * @brief   階数ポリゴン更新
	 * @param	mesh			[in] メッシュ
	 * @param	ipBufferedMesh	[in] バッファ付与済みメッシュ
	 * @param	rTree			[in] 空間キャッシュ
	 * @param	featureMap		[in] フィーチャマップ
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool updateStep( const MeshGeo& mesh, const IGeometryPtr& ipBufferedMesh, const BGRTree& rTree, const FeatureMap& featureMap );

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
	 * @retval  true  成功
	 * @retval  false 失敗
	 */
	bool createMeshList( const CString& fileName, MeshList& meshList );

	/**
	 * @brief	フィーチャクラス取得
	 * @param	ipWorkspace		[in] ワークスペース
	 * @param	layerName		[in] レイヤ名
	 * @param	ipFeatureClass	[out] フィーチャクラス
	 * @param	fieldMap		[out] フィールドマップ
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool openFeatureClass( const IWorkspacePtr& ipWorkspace, const CString& layerName, IFeatureClassPtr& ipFeatureClass, sindy::CFieldMap& fieldMap );

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

	/**
	 * @brief	メッシュポリゴン取得
	 * @param	meshList	[in] メッシュリスト
	 * @param	meshGeoMap	[out] メッシュジオメトリマップ
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool getMeshPolygon( const MeshList& meshList, MeshGeoMap& meshGeoMap );

	/**
	 * @brief	バッファ付与
	 * @param	ipGeometry	[in] ジオメトリ
	 * @param	buf			[in] バッファ距離（メートル）
	 * @return	バッファ後の形状
	 */
	IGeometryPtr createBuffer( const IGeometryPtr& ipGeometry, double buf );

	/**
	 * @brief	ボックス作成
	 * @param	ipGeometry	[in] ジオメトリ
	 * @return	ボックス
	 */
	BGBox getBox( const IGeometryPtr& ipGeometry );

	/**
	 * @brief	階数と建物の空間関係分析（RTree）
	 * @param	ipGeometry	[in] ジオメトリ
	 * @param	rTree		[in] 空間キャッシュ
	 * @param	featureMap	[in] フィーチャマップ
	 * @param	id			[out] 建物ID
	 * @retval	true  成功
	 * @retval	false 失敗
	 */
	bool analyzeRefByRTree( const IGeometryPtr& ipGeometry,  const BGRTree& rTree, const FeatureMap& featureMap, long& id );

private:

	Arguments m_args;									//!< 引数
	IWorkspacePtr m_ipWorkspace;						//!< ワークスペース
	IWorkspacePtr m_ipMeshWorkspace;					//!< メッシュワークスペース

	IFeatureClassPtr m_ipBuilding;						//!< 建物ポリゴン
	IFeatureClassPtr m_ipBuildingStep;					//!< 階数ポリゴン
	IFeatureClassPtr m_ipMeshFc;						//!< 3次メッシュ
	sindy::CFieldMap m_BuildingFieldMap;				//!< フィールドマップ（建物ポリゴン）
	sindy::CFieldMap m_BuildingStepFieldMap;			//!< フィールドマップ（階数ポリゴン）
	sindy::CFieldMap m_meshFieldMap;					//!< フィールドマップ（3次メッシュ）
	ISpatialReferencePtr m_ipSpatialRef;				//!< 空間参照
	MeshList m_meshList;								//!< メッシュリスト
	MeshGeoMap m_meshGeoMap;							//!< メッシュジオメトリマップ
	double m_bufDistance;								//!< メッシュバッファ距離（メートル）
};

