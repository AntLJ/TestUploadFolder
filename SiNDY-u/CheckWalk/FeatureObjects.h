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

// RTreeのメモリリーク回避のため、boost 1.56を使用
// 切り替えてビルドすること
#include <boost/version.hpp>

#if BOOST_VERSION != 105600
#error "RTreeのメモリリーク対策のため、boost 1.56でビルドして下さい。"
#endif

#pragma warning(disable:4503) // boostの仕様上必要

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>
#include <TDC/sindylib_core/FieldMap.h>

typedef double coordinate_type;
static const size_t dimension = 2;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<coordinate_type, dimension, bg::cs::cartesian> BGPoint;
typedef bg::model::box<BGPoint> BGBox;
typedef std::pair<BGBox, unsigned> value;
typedef bgi::rtree< value, bgi::quadratic<16> > BGRTree;

typedef std::map<long, std::set<long>> NODEIDMAP; //!< ノードIDをキーとしたmap

static const int FLAG_ON = 1; //!< フラグがONの時の値

typedef std::multimap<long, _IRowPtr>::iterator MULTIMAP_ITE; //!< 属性のmultimapのiterator

/**
 * @brief ジオメトリと種別コードを格納するクラス
 */
class GeoClass
{
public:
	/// コンストラクタ
	GeoClass() : m_ObjectID( -1 ) {}
	GeoClass( const IFeaturePtr ipFeature );
	/// デストラクタ
	virtual ~GeoClass(){}

private:
	IFeaturePtr  m_ipFeature;	//!< フィーチャ
public:
	IGeometryPtr m_ipGeometry;  //!< 形状
	long         m_ObjectID;	//!< オブジェクトID

	/**
	 * @brief ジオメトリからR-Tree用のボックスを得る
	 * @note 基本的にエンベロープだが、指定範囲分広げるのも可
	 * @param range [in] 広げる範囲(単位:m)
	 * @retrun boost::R-TreeのEnvelope
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	* @brief 指定インデックスの数値型の値を得る
	* @note m_ipFeatureがNULL又は属性がない場合は-1を返す
	* @param index [in] フィールドのインデックス
	* @retrun long型の属性値
	*/
	long GetLongValue(int index) const;

	/**
	* @brief 指定インデックスのCComVariant型の値を得る
	* @note m_ipFeatureがNULLの場合はデフォルト値を返す
	* @param index [in] フィールドのインデックス
	* @retrun CComVariant型の属性値
	*/
	CComVariant GetValue(int index) const;

	/**
	* @brief 指定インデックスのCComVariant型の値を得る
	* @note m_ipFeatureがNULLの場合はデフォルト値を返す
	* @param index [in] フィールドのインデックス
	* @retrun 文字列型の属性値
	*/
	CString GetStringValue(int index) const;

	/**
	* @brief 指定インデックスの属性値を比較する
	* @param index [in] フィールドのインデックス
	* @retval true  一致
	* @retval false 不一致
	*/
	bool CompareAt(const int index, const GeoClass& cmp) const;

	/**
	 * @brief 自身の形状が指定ジオメトリと重なっているかどうかを判定する
	 * @note AheIsOvelap2と同様の挙動
	 * @param ipGeometry [in] 相手のジオメトリ
	 * @retval true  重なっている
	 * @retval false 重なっていない
	 */
	bool IsOverlap( IGeometryPtr ipGeometry ) const;

	bool IsEqual( IGeometryPtr ipGeometry ) const;

	bool IsWithin( IGeometryPtr IpGeometry ) const;

	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

	bool IsCross( IGeometryPtr ipGeometry ) const;

	bool IsOk() const { return m_ObjectID >= 0; }
};

/**
* @brief フィーチャ群とフィールドマップを合わせて管理するためのクラス
*/
// TODO: RTreeの使われ方によっては、これに含めることを検討する
class FeaturesPack
{
public:
	/// コンストラクタ（引数なしでは使えないようにする）
	FeaturesPack( ITablePtr ipTable, const sindy::CFieldMap& fieldMap );
	/// コピーコンストラクタっぽいが、主にm_FeatureMapはコピーしたくない場合用
	FeaturesPack( const FeaturesPack& src, const bool isCopyFeatures );
	/// デストラクタ
	virtual ~FeaturesPack() {}
	sindy::CFieldMap m_FieldMap;			//!< フィールドマップ（初期化時に設定する必要あり）
	std::map<long, GeoClass> m_FeatureMap;	//!< ObjectIDとGeoClassのマップ
	ITablePtr m_ipTable;					//!< テーブル（初期化時に設定する必要あり）
};


/**
* @brief テーブルのデータとフィールドマップを合わせて管理するためのクラス
*/
class RowsPack {
public:
	/// コンストラクタ
	RowsPack( ) { }
	/// デストラクタ
	virtual ~RowsPack() {}
	sindy::CFieldMap m_FieldMap;		//!< フィールドマップ
	std::map<long, _IRowPtr> m_Rows;	//!< キー値とロウのマップ
};

/**
* @brief テーブルのデータとフィールドマップを合わせて管理するためのクラス
　　　　　マルチマップタイプ
*/
class MultiRowsPack {
public:
	/// コンストラクタ
	MultiRowsPack() {}
	/// デストラクタ
	virtual ~MultiRowsPack() {}
	sindy::CFieldMap m_FieldMap;			//!< フィールドマップ
	std::multimap<long, _IRowPtr> m_Rows;	//!< キー値とロウのマルチマップ
};
