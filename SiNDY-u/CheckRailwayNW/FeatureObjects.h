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

typedef double coordinate_type;
static const size_t dimension = 2;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<coordinate_type, dimension, bg::cs::cartesian> BGPoint;
typedef bg::model::box<BGPoint> BGBox;
typedef std::pair<BGBox, unsigned> value;
typedef bgi::rtree< value, bgi::quadratic<16> > BGRTree;

/**
 * @brief ジオメトリと種別コードを格納するクラス
 */
class GeoClass
{
public:
	/// コンストラクタ
	GeoClass(){
//		classCode = -1;
	};
	/// デストラクタ
	~GeoClass(){};

public:
//	long classCode; // 属性(1つだけは共通で持たせておく)
	std::map<CString, CString> attr; ///< フィールド名・値
	CAdapt<IGeometryPtr> geometry; ///< ジオメトリ

public:
	/**
	 * @brief ジオメトリからR-Tree用のボックスを得る
	 * @note 基本的にエンベロープだが、指定範囲分広げるのも可
	 * @param range [in] 広げる範囲(単位:m)
	 * @retval true  正常終了
	 * @retval false 異常終了
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	 * @brief 指定ジオメトリとの重なりを判定する
	 * @note  Touch,Disjoint,Cross以外は重なっていると判定する
	 * @param ipGeometry [in] 相手のジオメトリ
	 * @retval true  正常終了
	 * @retval false 異常終了
	 */
	bool IsOverRap( IGeometryPtr ipGeometry ) const;

	bool IsEqual( IGeometryPtr ipGeometry ) const;

	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

	bool IsCross( IGeometryPtr ipGeometry ) const;
};

class recClass
{
public:
	/// コンストラクタ
	recClass(){};
	/// デストラクタ
	~recClass(){};

public:

	std::map<CString, CString> attr; ///< フィールド名・値

};

