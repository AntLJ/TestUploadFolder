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
	GeoClass(){ SetFieldValue( _T("null"), _T("") ); };

	/// �f�X�g���N�^
	~GeoClass(){};

public:

	/**
	 * @brief   �����o�[�Ƀt�B�[���h�����L�[�ɒl���Z�b�g
	 * @param   field [in]  �t�B�[���h��
	 * @param   value [in]  �l
	 */
	void SetFieldValue( const CString& field, const CString& value ){ m_attr[field] = value; };

	/**
	 * @brief   �t�B�[���h������l�擾
	 * @note    �Y���t�B�[���h�����i�[���Ă��Ȃ��ꍇ�́A�󕶎���Ԃ�
	 * @param   field   [in]  �t�B�[���h��
	 * @retval  CString       �����l
	 */
	const CString& GetFieldValue( const CString& field ) const
	{
		auto itr = m_attr.find( field );
		if( itr != m_attr.end() )
			return itr->second;		
		return m_attr.at( _T("null") );
	};

	/**
	 * @brief   �W�I���g�����Z�b�g
	 * @param   ipGeom [in]  �W�I���g��
	 */
	void SetGeometry( const IGeometryPtr& ipGeom ){ m_geometry = ipGeom; };

	/**
	 * @brief   �W�I���g�����擾
	 * @retval  IGeometryPtr �W�I���g��
	 */
	const IGeometryPtr& GetGeometry() const { return m_geometry; };

	/**
	 * @brief   �W�I���g������R-Tree�p�̃{�b�N�X�𓾂�
	 * @note    ��{�I�ɃG���x���[�v�����A�w��͈͕��L����̂���
	 * @param   range      [in]  �L����͈�(�P��:m)
	 * @retval  BGBox �G���x���[�v
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	 * @brief   �w��W�I���g���Ƃ̏d�Ȃ�𔻒肷��
	 * @param   ipGeometry [in]  ����̃W�I���g��
	 * @retval  true  �d�Ȃ��Ă��Ȃ�
	 * @retval  false �d�Ȃ��Ă���
	 */
	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

private:

	std::map<CString, CString> m_attr;     //!< �t�B�[���h���E�l
	IGeometryPtr               m_geometry; //!< �W�I���g��
};


