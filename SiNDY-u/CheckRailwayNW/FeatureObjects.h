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

// RTree�̃��������[�N����̂��߁Aboost 1.56���g�p
// �؂�ւ��ăr���h���邱��
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
 * @brief �W�I���g���Ǝ�ʃR�[�h���i�[����N���X
 */
class GeoClass
{
public:
	/// �R���X�g���N�^
	GeoClass(){
//		classCode = -1;
	};
	/// �f�X�g���N�^
	~GeoClass(){};

public:
//	long classCode; // ����(1�����͋��ʂŎ������Ă���)
	std::map<CString, CString> attr; ///< �t�B�[���h���E�l
	CAdapt<IGeometryPtr> geometry; ///< �W�I���g��

public:
	/**
	 * @brief �W�I���g������R-Tree�p�̃{�b�N�X�𓾂�
	 * @note ��{�I�ɃG���x���[�v�����A�w��͈͕��L����̂���
	 * @param range [in] �L����͈�(�P��:m)
	 * @retval true  ����I��
	 * @retval false �ُ�I��
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	 * @brief �w��W�I���g���Ƃ̏d�Ȃ�𔻒肷��
	 * @note  Touch,Disjoint,Cross�ȊO�͏d�Ȃ��Ă���Ɣ��肷��
	 * @param ipGeometry [in] ����̃W�I���g��
	 * @retval true  ����I��
	 * @retval false �ُ�I��
	 */
	bool IsOverRap( IGeometryPtr ipGeometry ) const;

	bool IsEqual( IGeometryPtr ipGeometry ) const;

	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

	bool IsCross( IGeometryPtr ipGeometry ) const;
};

class recClass
{
public:
	/// �R���X�g���N�^
	recClass(){};
	/// �f�X�g���N�^
	~recClass(){};

public:

	std::map<CString, CString> attr; ///< �t�B�[���h���E�l

};

