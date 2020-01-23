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
#include <TDC/sindylib_core/FieldMap.h>

typedef double coordinate_type;
static const size_t dimension = 2;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;
typedef bg::model::point<coordinate_type, dimension, bg::cs::cartesian> BGPoint;
typedef bg::model::box<BGPoint> BGBox;
typedef std::pair<BGBox, unsigned> value;
typedef bgi::rtree< value, bgi::quadratic<16> > BGRTree;

typedef std::map<long, std::set<long>> NODEIDMAP; //!< �m�[�hID���L�[�Ƃ���map

static const int FLAG_ON = 1; //!< �t���O��ON�̎��̒l

typedef std::multimap<long, _IRowPtr>::iterator MULTIMAP_ITE; //!< ������multimap��iterator

/**
 * @brief �W�I���g���Ǝ�ʃR�[�h���i�[����N���X
 */
class GeoClass
{
public:
	/// �R���X�g���N�^
	GeoClass() : m_ObjectID( -1 ) {}
	GeoClass( const IFeaturePtr ipFeature );
	/// �f�X�g���N�^
	virtual ~GeoClass(){}

private:
	IFeaturePtr  m_ipFeature;	//!< �t�B�[�`��
public:
	IGeometryPtr m_ipGeometry;  //!< �`��
	long         m_ObjectID;	//!< �I�u�W�F�N�gID

	/**
	 * @brief �W�I���g������R-Tree�p�̃{�b�N�X�𓾂�
	 * @note ��{�I�ɃG���x���[�v�����A�w��͈͕��L����̂���
	 * @param range [in] �L����͈�(�P��:m)
	 * @retrun boost::R-Tree��Envelope
	 */
	BGBox GetBox( double range=0 ) const;

	/**
	* @brief �w��C���f�b�N�X�̐��l�^�̒l�𓾂�
	* @note m_ipFeature��NULL���͑������Ȃ��ꍇ��-1��Ԃ�
	* @param index [in] �t�B�[���h�̃C���f�b�N�X
	* @retrun long�^�̑����l
	*/
	long GetLongValue(int index) const;

	/**
	* @brief �w��C���f�b�N�X��CComVariant�^�̒l�𓾂�
	* @note m_ipFeature��NULL�̏ꍇ�̓f�t�H���g�l��Ԃ�
	* @param index [in] �t�B�[���h�̃C���f�b�N�X
	* @retrun CComVariant�^�̑����l
	*/
	CComVariant GetValue(int index) const;

	/**
	* @brief �w��C���f�b�N�X��CComVariant�^�̒l�𓾂�
	* @note m_ipFeature��NULL�̏ꍇ�̓f�t�H���g�l��Ԃ�
	* @param index [in] �t�B�[���h�̃C���f�b�N�X
	* @retrun ������^�̑����l
	*/
	CString GetStringValue(int index) const;

	/**
	* @brief �w��C���f�b�N�X�̑����l���r����
	* @param index [in] �t�B�[���h�̃C���f�b�N�X
	* @retval true  ��v
	* @retval false �s��v
	*/
	bool CompareAt(const int index, const GeoClass& cmp) const;

	/**
	 * @brief ���g�̌`�󂪎w��W�I���g���Əd�Ȃ��Ă��邩�ǂ����𔻒肷��
	 * @note AheIsOvelap2�Ɠ��l�̋���
	 * @param ipGeometry [in] ����̃W�I���g��
	 * @retval true  �d�Ȃ��Ă���
	 * @retval false �d�Ȃ��Ă��Ȃ�
	 */
	bool IsOverlap( IGeometryPtr ipGeometry ) const;

	bool IsEqual( IGeometryPtr ipGeometry ) const;

	bool IsWithin( IGeometryPtr IpGeometry ) const;

	bool IsDisjoint( IGeometryPtr ipGeometry ) const;

	bool IsCross( IGeometryPtr ipGeometry ) const;

	bool IsOk() const { return m_ObjectID >= 0; }
};

/**
* @brief �t�B�[�`���Q�ƃt�B�[���h�}�b�v�����킹�ĊǗ����邽�߂̃N���X
*/
// TODO: RTree�̎g�����ɂ���ẮA����Ɋ܂߂邱�Ƃ���������
class FeaturesPack
{
public:
	/// �R���X�g���N�^�i�����Ȃ��ł͎g���Ȃ��悤�ɂ���j
	FeaturesPack( ITablePtr ipTable, const sindy::CFieldMap& fieldMap );
	/// �R�s�[�R���X�g���N�^���ۂ����A���m_FeatureMap�̓R�s�[�������Ȃ��ꍇ�p
	FeaturesPack( const FeaturesPack& src, const bool isCopyFeatures );
	/// �f�X�g���N�^
	virtual ~FeaturesPack() {}
	sindy::CFieldMap m_FieldMap;			//!< �t�B�[���h�}�b�v�i���������ɐݒ肷��K�v����j
	std::map<long, GeoClass> m_FeatureMap;	//!< ObjectID��GeoClass�̃}�b�v
	ITablePtr m_ipTable;					//!< �e�[�u���i���������ɐݒ肷��K�v����j
};


/**
* @brief �e�[�u���̃f�[�^�ƃt�B�[���h�}�b�v�����킹�ĊǗ����邽�߂̃N���X
*/
class RowsPack {
public:
	/// �R���X�g���N�^
	RowsPack( ) { }
	/// �f�X�g���N�^
	virtual ~RowsPack() {}
	sindy::CFieldMap m_FieldMap;		//!< �t�B�[���h�}�b�v
	std::map<long, _IRowPtr> m_Rows;	//!< �L�[�l�ƃ��E�̃}�b�v
};

/**
* @brief �e�[�u���̃f�[�^�ƃt�B�[���h�}�b�v�����킹�ĊǗ����邽�߂̃N���X
�@�@�@�@�@�}���`�}�b�v�^�C�v
*/
class MultiRowsPack {
public:
	/// �R���X�g���N�^
	MultiRowsPack() {}
	/// �f�X�g���N�^
	virtual ~MultiRowsPack() {}
	sindy::CFieldMap m_FieldMap;			//!< �t�B�[���h�}�b�v
	std::multimap<long, _IRowPtr> m_Rows;	//!< �L�[�l�ƃ��E�̃}���`�}�b�v
};
