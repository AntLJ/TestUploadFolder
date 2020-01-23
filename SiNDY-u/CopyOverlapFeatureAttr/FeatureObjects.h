#pragma once

// RTreeのメモリリーク回避のため、boost 1.56を使用
// 切り替えてビルドすること（CheckWalkよりコピペ）
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
 * @brief ジオメトリとフィールド値を格納するクラス
 */
class GeoClass
{
public:
	/// コンストラクタ
	GeoClass():oid(-1){}
	/// デストラクタ
	virtual ~GeoClass(){}

public:
	
	long oid;                          //!< オブジェクトID
	CAdapt<IGeometryPtr> geometry;     //!< 形状
	std::pair<CString, CString> attr;  //!< フィールド名・値

public:
	/**
	 * @brief ジオメトリからR-Tree用のボックスを得る
	 * @note 基本的にエンベロープだが、指定範囲分広げるのも可
	 * @param range [in] 広げる範囲(単位:m)
	 * @retrun boost::R-TreeのEnvelope
	 */
	BGBox GetBox( double range=0 ) const;
};
