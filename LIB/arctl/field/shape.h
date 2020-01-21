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

/**
 * @file shape.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_SHAPE_H_
#define ARCTL_FIELD_SHAPE_H_

#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {
namespace field {

/**
 * @ingroup field_index_group �����l�N���X
 * @brief �`�󑮐��C���f�b�N�X�N���X�B
 * @test @ref unit_test FieldShapeTest
 */
class shape_index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name �R���X�g���N�^�ƃf�X�g���N�^
	shape_index_t() : m_n(-1) {}
	shape_index_t(const shape_index_t& rIndex) : m_n(rIndex.m_n) {}
	shape_index_t(IFeature* ipFeature) { operator=(ipFeature); }
	shape_index_t(IFeatureClass* ipFeatureClass) { operator=(ipFeatureClass); }
//@}

//@{ @name ���Z�q
	long index() const { return m_n; }
//@}

//@{ @name ���Z�q
	/// �����t�B�[���h�C���f�b�N�X���擾����B
	operator long() const { return index(); }
	bool operator!() const { return m_n == -1; }

	shape_index_t& operator=(IFeature* ipFeature)
	{
		IObjectClassPtr ipClass;
		atl2::valid(ipFeature->get_Class(&ipClass));
		return operator=(IFeatureClassPtr(ipClass));
	}

	shape_index_t& operator=(IFeatureClass* ipFeatureClass)
	{
		atl2::valid(ipFeatureClass->FindField(bname(ipFeatureClass), &m_n));
		return *this;
	}
//@}
	
//@{ @name �ÓI�֐�
	/// �����t�B�[���h����BSTR�^�Ŏ擾����B
	static CComBSTR bname(IFeatureClass* ipFeatureClass)
	{
		CComBSTR bstrName;
		atl2::valid(ipFeatureClass->get_ShapeFieldName(&bstrName));
		return bstrName;
	}

	/// �����t�B�[���h�����o�͂���B
	template <typename TOutputIterator>
	static void copy_name(IFeatureClass* ipFeatureClass, TOutputIterator result)
	{
		*result = COLE2CT(bname(ipFeatureClass)).m_psz;
		++result;
	}
//@}

private:
// �ϐ�
	long m_n; ///< �����t�B�[���h�C���f�b�N�X�B
};

/** @addtogroup field_value_group */
/*@{*/

/**
 * @brief �`��N���X�e���v���[�g�B
 *
 * �`��̕ێ��A�t�B�[�`���ւ̌`��̏����o�����s���N���X�B
 *
 * @par �����l�̏����o��:
 * IFeature::putref_Shape()<br>
 * IFeatureBuffer::putref_Shape()
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_base_t : public arctl::detail::multi_inherit::base_t
{
public:
// ��`
	typedef shape_index_t index_type;
	typedef _com_ptr_t<_com_IIID<TIShape, &__uuidof(TIShape)> > shape_ref_type;
	typedef shape_ref_type value_type;

/// @name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	shape_base_t() {}
	shape_base_t(TIShape* ipShape) { operator=(ipShape); }
//@}

/// @name �擾
//@{
	/// �ێ����Ă���`��X�}�[�g�|�C���^�ւ̉ώQ�Ƃ�Ԃ��B
	shape_ref_type& shape() { return m_ipShape; }

	/// �ێ����Ă���`��X�}�[�g�|�C���^�ւ̕s�ώQ�Ƃ�Ԃ��B
	const shape_ref_type& shape() const { return m_ipShape; }
//@}

/// @name �����l�N���X�݊�
//@{
	/// �ێ����Ă���`��X�}�[�g�|�C���^�ւ̉ώQ�Ƃ�Ԃ��B
	value_type& value() { return shape(); }

	/// �ێ����Ă���`��X�}�[�g�|�C���^�ւ̕s�ώQ�Ƃ�Ԃ��B
	const value_type& value() const { return shape(); }

	/**
	 * @brief @p ipFeature�Ɍ`��������o���B
	 *
	 * @param ipFeature [in] �`��̒u���������s���t�B�[�`���B
	 * @retval true �`��̏����o���ɐ��������B
	 * @retval false �`��̏����o���Ɏ��s�����B
	 * @exception _com_error_t <b>IFeature::putref_Shape()</b>���G���[�𔭂����B
	 */
	bool write(IFeature* ipFeature, const index_type& = index_type()) const
	{
		return atl2::valid(ipFeature->putref_Shape(m_ipShape)) == S_OK;
	}

	/**
	 * @brief @p ipFeatureBuffer�Ɍ`��������o���B
	 *
	 * @param ipFeatureBuffer [in] �`��̒u���������s���t�B�[�`���B
	 * @retval true �`��̏����o���ɐ��������B
	 * @retval false �`��̏����o���Ɏ��s�����B
	 * @exception _com_error_t <b>IFeature::putref_Shape()</b>���G���[�𔭂����B
	 */
	bool write(IFeatureBuffer* ipFeatureBuffer, const index_type& = index_type()) const
	{
		return atl2::valid(ipFeatureBuffer->putref_Shape(m_ipShape)) == S_OK;
	}

	/**
	 * @brief @a m_ipShape��@p ipFeature�̌`����r����B
	 *
	 * @param ipFeature [in] ��r�ΏہB
	 */
	bool equal_value(IFeature* ipFeature, const index_type& = index_type()) const
	{
		return equal_shape(ipFeature);
	}

	/**
	 * @brief @a m_ipShape��@p ipFeatureBuffer�̌`����r����B
	 *
	 * @param ipFeatureBuffer [in] ��r�ΏہB
	 */
	bool equal_value(IFeatureBuffer* ipFeatureBuffer, const index_type& = index_type()) const
	{
		return equal_shape(ipFeatureBuffer);
	}

	/**
	 * @brief �ێ����Ă���`������ւ���B
	 *
	 * @param ipShape [in] �V�����`��B
	 */
	shape_base_t& operator=(TIShape* ipShape)
	{
		m_ipShape = ipShape;
		return *this;
	}
//@}

private:
// ����
	/**
	 * @brief @a m_ipShape��@p ipFeature�̌`����r����B
	 *
	 * @param ipFeature [in] ��r�ΏہB
	 * @retval true �`��͈�v����B
	 * @retval false �`��͈�v���Ȃ��B
	 * @exception _com_error_t �`���r���ɗ\�����Ȃ��G���[�����������B
	 */
	template <typename TIFeature>
	bool equal_shape(TIFeature* ipFeature) const
	{
		IGeometryPtr ipGeometry;
		atl2::valid(ipFeature->get_Shape(&ipGeometry));

		VARIANT_BOOL vbEquals = VARIANT_FALSE;
		atl2::valid(IClonePtr(ipGeometry)->IsEqual(IClonePtr(m_ipShape), &vbEquals));
		return vbEquals != VARIANT_FALSE;
	}

// �ϐ�
	value_type m_ipShape;
};

/**
 * @brief �t�B�[�`���`��N���X�e���v���[�g�B
 *
 * �t�B�[�`���̌`��̎擾�E�ێ��E�����o�����s���N���X�B<br>
 * @ref shape_base_t�ɁA�t�B�[�`������̌`��擾�@�\������t�������́B
 *
 * @par �����l�擾�\�I�u�W�F�N�g:
 * IFeature
 * @par �����l�̎擾:
 * (IFeature::*TGetShapeMethod)()
 *
 * @par �e���v���[�g����:
 * - TGetShapeMethod : �t�B�[�`������`����擾���郁�\�b�h�B
 * - TIShape : IGeometry�p���N���X�B�f�t�H���g��<b>IGeometry</b>�B
 *
 * @sa shape_t, shape_copy_t
 *
 * @test @ref unit_test FieldShapeTest
 */
template <HRESULT (__stdcall IFeature::*TGetShapeMethod)(IGeometry**), typename TIShape = IGeometry>
class feature_shape_t : public shape_base_t<TIShape>
{
	typedef shape_base_t<TIShape> super;
public:
/// @name �R���X�g���N�^�ƃf�X�g���N�^
//@{ 
	feature_shape_t() {}
	feature_shape_t(IFeature* ipFeature) { operator=(ipFeature); }
	feature_shape_t(IFeature* ipFeature, const index_type&) { operator=(ipFeature); }
	feature_shape_t(TIShape* ipShape) : super(ipShape) {}
//@}

/// @name ���Z�q
//@{
	feature_shape_t& operator=(IFeature* ipFeature)
	{
		IGeometryPtr ipGeometry;
		atl2::valid((ipFeature->*TGetShapeMethod)(&ipGeometry));
		value() = ipGeometry;
		return *this;
	}
//@}
};

/**
 * @brief �t�B�[�`������̌`��擾��<b>IFeature::get_Shape()</b>���g��@ref feature_shape_t�B
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_t : public feature_shape_t<&IFeature::get_Shape, TIShape>
{
	typedef feature_shape_t<&IFeature::get_Shape, TIShape> super;
public:
/// @name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	shape_t() {}
	shape_t(IFeature* ipFeature) : super(ipFeature) {}
	shape_t(IFeature* ipFeature, const super::index_type&) : super(ipFeature) {}
	shape_t(IFeatureBuffer* ipFeatureBuffer) { operator=(ipFeatureBuffer); }
	shape_t(IFeatureBuffer* ipFeatureBuffer, const super::index_type&) { operator=(ipFeatureBuffer); }
	shape_t(TIShape* ipShape) : super(ipShape) {}
//@}

/// @name ���Z�q
//@{
	shape_t& operator=(IFeature* ipFeature)
	{
		super::operator=(ipFeature);
		return *this;
	}

	shape_t& operator=(IFeatureBuffer* ipFeatureBuffer)
	{
		IGeometryPtr ipGeometry;
		atl2::valid(ipFeatureBuffer->get_Shape(&ipGeometry));
		value() = ipGeometry;
		return *this;
	}
//@}
};

/**
 * @brief �t�B�[�`������̌`��擾��<b>IFeature::get_ShapeCopy()</b>���g��@ref feature_shape_t�B
 *
 * @test @ref unit_test FieldShapeTest
 */
template <typename TIShape = IGeometry>
class shape_copy_t : public feature_shape_t<&IFeature::get_ShapeCopy, TIShape>
{
	typedef feature_shape_t<&IFeature::get_ShapeCopy, TIShape> super;
public:
/// @name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	shape_copy_t() {}
	shape_copy_t(IFeature* ipFeature) : super(ipFeature) {}
	shape_copy_t(IFeature* ipFeature, const super::index_type&) : super(ipFeature) {}
//@}
};

/*@}*/

/**
 * @ingroup field_value_mutabl_get_group
 */ 
//@{
/// �`������o���B
template <typename T>
inline T* get(const shape_base_t<T>& val) { return val.value(); }
//@}

} // namespace field
} // namespace arctl

#endif // ARCTL_FIELD_SHAPE_H_
