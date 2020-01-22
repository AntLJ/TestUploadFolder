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
	GeoClass(){ SetFieldValue( _T("null"), _T("") ); };

	/// デストラクタ
	~GeoClass(){};

public:

	/**
	 * @brief   メンバーにフィールド名をキーに値をセット
	 * @param   field [in]  フィールド名
	 * @param   value [in]  値
	 */
	void SetFieldValue( const CString& field, const CString& value ){ m_attr[field] = value; };

	/**
	 * @brief   フィールド名から値取得
	 * @note    該当フィールド名を格納していない場合は、空文字を返す
	 * @param   field   [in]  フィールド名
	 * @retval  CString       属性値
	 */
	const CString& GetFieldValue( const CString& field ) const
	{
		auto itr = m_attr.find( field );
		if( itr != m_attr.end() )
			return itr->second;		
		return m_attr.at( _T("null") );
	};

	/**
	 * @brief   ジオメトリをセット
	 * @param   ipGeom [in]  ジオメトリ
	 */
	void SetGeometry( const IGeometryPtr& ipGeom ){ m_geometry = ipGeom; };

	/**
	 * @brief   ジオメトリを取得
	 * @retval  IGeometryPtr ジオメトリ
	 */
	const IGeometryPtr& GetGeometry() const { return m_geometry; };

	/**
	 * @brief   ジオメトリからR-Tree用のボックスを得る
	 * @note    基本的にエンベロープだが、指定範囲分広げるのも可
	 * @param   range      [in]  広げる範囲(単位:m)
	 * @retval  BGBox エンベロープ
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	 * @brief   指定ジオメトリとの重なりを判定する
	 * @param   ipGeometry [in]  相手のジオメトリ
	 * @retval  true  重なっていない
	 * @retval  false 重なっている
	 */
	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

private:

	std::map<CString, CString> m_attr;     //!< フィールド名・値
	IGeometryPtr               m_geometry; //!< ジオメトリ
};


