#pragma once

// RTree�̃��������[�N����̂��߁Aboost 1.56���g�p
// �؂�ւ��ăr���h���邱�ƁiCheckWalk���R�s�y�j
#include <boost/version.hpp>

#if BOOST_VERSION != 105600
#error "RTree�̃��������[�N�΍�̂��߁Aboost 1.56�Ńr���h���ĉ������B"
#endif

#pragma warning(disable:4503) // boost�̎d�l��K�v

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
 * @brief �W�I���g���ƃt�B�[���h�l���i�[����N���X
 */
class GeoClass
{
public:
	/// �R���X�g���N�^
	GeoClass():oid(-1){}
	/// �f�X�g���N�^
	virtual ~GeoClass(){}

public:
	
	long oid;                          //!< �I�u�W�F�N�gID
	CAdapt<IGeometryPtr> geometry;     //!< �`��
	std::pair<CString, CString> attr;  //!< �t�B�[���h���E�l

public:
	/**
	 * @brief �W�I���g������R-Tree�p�̃{�b�N�X�𓾂�
	 * @note ��{�I�ɃG���x���[�v�����A�w��͈͕��L����̂���
	 * @param range [in] �L����͈�(�P��:m)
	 * @retrun boost::R-Tree��Envelope
	 */
	BGBox GetBox( double range=0 ) const;
};
