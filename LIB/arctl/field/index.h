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
 * @file index.h
 * @brief for field.h
 */
#ifndef ARCTL_FIELD_INDEX_H_
#define ARCTL_FIELD_INDEX_H_

#include <boost/preprocessor.hpp>
#include <atl2/type_traits.h>
#include <atl2/exception.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {

namespace field {

/**
 * @defgroup field_index_group �����t�B�[���h�C���f�b�N�X�N���X
 *
 * �����t�B�[���h�C���f�b�N�X�̎擾�E�ێ����s���N���X�Q�B
 * �K�v�Ȓ�`�E�֐�������邱�ƂŁA @ref ARCTL_FIELD_INDEX_CLASS_D �ɂ�鑽�d�p�����\�ƂȂ�B
 */
//@{
/**
 * @brief �����t�B�[���h���ɂ�鑮���t�B�[���h�C���f�b�N�X�N���X�B
 *
 * IFields �Ȃǂ��瑮���t�B�[���h�C���f�b�N�X�����߁A�ێ�����N���X�B
 * 
 * @par �e���v���[�g����:
 * - @a TFieldName : �������̍L��ϐ��B
 *
 * @test @ref unit_test FieldIndexTest
 */
template <LPCTSTR TFieldName>
class index_t : public arctl::detail::multi_inherit::base_t
{
public:
//@{ @name �\�z/����
	/// �f�t�H���g�R���X�g���N�^�Ő������ꂽ�I�u�W�F�N�g�́A�������C���f�b�N�X����������܂Ŗ����ł���B
	index_t() : m_nIndex(-1) {}

	/// IFields::FindField() �ɂ��\�z�B
	index_t(IFields* ipFields) { operator=(ipFields); }

	/// IRowBuffer::get_Fields() �ɂ���ē���ꂽ IFields �ɂ��\�z�B
	index_t(IRowBuffer* ipRowBuffer) { operator=(ipRowBuffer); }

	/// IClass::get_Fields() �ɂ���ē���ꂽ IFields �ɂ��\�z�B
	index_t(IClass* ipClass) { operator=(ipClass); }
//@}

//@{ @name ���Z�q
	long index() const { return m_nIndex; }
//@}

//@{ @name ���Z�q
	/**
	 * @brief �����t�B�[���h�C���f�b�N�X���擾����B
	 *
	 * @remarks
	 * ������extractor�̑��݂��ς킵���v���Ă��鎞�����邩������Ȃ��B<br>
	 * ���̎��̂��߂ɁA�Ȃ�ׂ��Ȃ� index() ���g���悤�ɂ��Ăق����B
	 */
	operator long() const { return index(); }

	/**
	 * @brief �I�u�W�F�N�g�̖����f�f���s���B
	 *
	 * �I�u�W�F�N�g�͈ȉ��̂Ƃ��ɖ����ƂȂ�:
	 * - �f�t�H���g�R���X�g���N�^�ō\�z���ꂽ�B
	 * - �Y�����鑮���t�B�[���h�����݂��Ȃ� IFields ���n���ꂽ�B
	 *
	 * �����ɂȂ����I�u�W�F�N�g�́A�ȉ��̂Ƃ��ɍĂїL���ƂȂ�:
	 * - �Y�����鑮���t�B�[���h�����݂��� IFields ���n���ꂽ�B
	 */
	bool operator!() const { return m_nIndex == -1; }

	/**
	 * @brief IFields::FindField() �ɂ��C���f�b�N�X�X�V�B
	 */
	index_t& operator=(IFields* ipFields)
	{
		atl2::valid(ipFields->FindField(CComBSTR(name()), &m_nIndex));
		return *this;
	}

	/**
	 * @brief IRowBuffer::get_Fields() �ɂ���ē���ꂽ IFields �ɂ��C���f�b�N�X�X�V�B
	 */
	index_t& operator=(IRowBuffer* ipRowBuffer)
	{
		IFieldsPtr ipFields;
		atl2::valid(ipRowBuffer->get_Fields(&ipFields));
		return operator=(ipFields);
	}

	/**
	 * @brief IClass::get_Fields() �ɂ���ē���ꂽ IFields �ɂ��C���f�b�N�X�X�V�B
	 */
	index_t& operator=(IClass* ipClass)
	{
		IFieldsPtr ipFields;
		atl2::valid(ipClass->get_Fields(&ipFields));
		return operator=(ipFields);
	}
//@}
	
//@{ @name �ÓI�֐�
	/**
	 * @brief �����t�B�[���h�����擾����B
	 */
	static LPCTSTR name() { return TFieldName; }

	/**
	 * @brief �����t�B�[���h����BSTR�^�Ŏ擾����ioid_t�Ashape_t�݊��p�j�B
	 */
	static CComBSTR bname(IClass*) { return CComBSTR(name()); }

	/**
	 * @brief �����t�B�[���h�����o�͂���B
	 */
	template <typename TOutputIterator>
	static void copy_name(TOutputIterator result)
	{
		*result = name();
		++result;
	}

	/**
	 * @brief �����t�B�[���h�����o�͂���B
	 *
	 * �����t�B�[���h���̂̎擾�� ITable �� IFeatureClass ���K�v��
	 * arctl::field::oid_t �� arctl::field::shape_t �Ƃ̌݊���ۂ��߂ɒ�`���ꂽ�֐��B<br>
	 * ���[�U�������I�ɌĂяo�������͂Ȃ��B
	 */
	template <typename TOutputIterator>
	static void copy_name(IClass*, TOutputIterator result)
	{
		copy_name(result);
	}
//@}

private:
// �ϐ�
	long m_nIndex; ///< �����t�B�[���h�C���f�b�N�X�B
};
//@}

} // namespace field

} // namespace arctl

/**
 * @brief @ref arctl::field::index_t "index_t" �𑽏d�p�����A�܂Ƃ߂ĊǗ�����N���X���`����}�N���B
 *
 * @param c
 * �N���X���B
 * @param seq
 * �������̍L��ϐ���<a href="http://boost.cppll.jp/HEAD/libs/preprocessor/doc/data/sequences.html">�V�[�P���X</a>�B
 * �e�X�� @ref arctl::field::index_t "index_t" �̃e���v���[�g�����ƂȂ�B
 *
 * @par �p��:
 * @code
 * using namespace arctl::field;
 *
 * // �������̍L��ϐ�
 * const TCHAR kValue1[] = _T("Value1");
 * const TCHAR kValue2[] = _T("Value2");
 *
 * // �����l�Ǘ��N���X��`
 * ARCTL_FIELD_INDEX_CLASS(index_type, (kValue1) (kValue2));
 * @endcode
 * - <b>�N���X��:</b> index_type
 * - <b>�����t�B�[���h���ꗗ:</b>
 *   - "Value1"
 *   - "Value2"
 * 
 * <h2>��`�����֐�</h2>
 * �ȉ��́A�N���X @a c �ɒ�`����郁���o�֐��̈ꗗ�ł���B<br>
 * �����f�f�ȊO�͑S�ăe���v���[�g�֐��ł��邽�߁A�K�v��������Ύ��̉�����Ȃ��B
 *
 * @code
 * template <typename T>
 * explicit c(T const& rT);
 * @endcode
 * @b operator=() ���Ăяo���B
 *
 * @code
 * template <typename T>
 * c& operator=(T const& rT);
 * @endcode
 * ������Z�B<br>
 * �S�Ă�@ref field_index_group "�����t�B�[���h�C���f�b�N�X�I�u�W�F�N�g"�ɑ΂���
 * @b operator=(rT) ���s���B
 *
 * @code
 * bool operator!() const;
 * @endcode
 * �I�u�W�F�g�̖����f�f�B<br>
 * �S�Ă�@ref field_index_group "�����t�B�[���h�C���f�b�N�X�I�u�W�F�N�g"�ɑ΂���
 * @b operator!() ���s���A���̖߂�l�̘_���a��Ԃ��B
 *
 * @code
 * template <typename TOutputIterator>
 * static void copy_name(TOuptutIterator result);
 * @endcode
 * �����t�B�[���h�������o���A���S���Y���B<br>
 * �S�Ă�@ref field_index_group "�����t�B�[���h�C���f�b�N�X�I�u�W�F�N�g"�ɑ΂���
 * @ref arctl::field::index_t::copy_name(TOutputIterator) "copy_name(result)" ���s���B
 *
 * @code
 * template <typename TClass, typename TOutputIterator>
 * static void copy_name(TClass const& rClass, TOutputIterator result);
 * @endcode
 * �����t�B�[���h�������o���A���S���Y���B<br>
 * �S�Ă�@ref field_index_group "�����t�B�[���h�C���f�b�N�X�I�u�W�F�N�g"�ɑ΂���
 * @ref arctl::field::index_t::copy_name(IClass*,TOutputIterator) "copy_name(rClass,result)" ���s���B<br>
 *
 * @test @ref unit_test FieldIndexTest
 */
#define ARCTL_FIELD_INDEX_CLASS(c, seq) ARCTL_FIELD_INDEX_CLASS_D(c, BOOST_PP_SEQ_TRANSFORM(ARCTL_FIELD_INDEX_CLASS_EXPAND, _, seq))

/**
 * @brief @ref field_index_group�𑽏d�p�����A�܂Ƃ߂ĊǗ�����N���X���`����}�N���B
 */
#define ARCTL_FIELD_INDEX_CLASS_D(c, seq)										\
	class c : BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_INDEX_CLASS_INHERIT, _, seq)	\
	{																			\
	public:																		\
		enum { is_arctl_multi_inherit = true };									\
																				\
		c() {}																	\
		template <typename T> explicit c(T const& rT) { operator=(rT); }		\
	\
		template <typename T>											\
		c& operator=(T const& r)										\
		{																\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_SUB, r, seq)	\
			return *this;												\
		}																\
	\
		bool operator!() const														\
		{																			\
			return BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_INDEX_CLASS_NOT, r, seq);	\
		}																			\
	\
		template <typename TOutputIterator>										\
		static void copy_name(TOutputIterator result)							\
		{																		\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_COPY, result, seq)	\
		}																		\
	\
		template <typename TClass, typename TOutputIterator>					\
		static void copy_name(TClass const& rClass, TOutputIterator result)		\
		{																		\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_INDEX_CLASS_COPY2, result, seq)	\
		}																		\
	}

#ifndef DOXYGEN

/// �����t�B�[���h���𑮐��t�B�[���h�N���X�ɕϊ�����}�N���B
#define ARCTL_FIELD_INDEX_CLASS_EXPAND(d, data, elem) arctl::field::index_t<elem>

/// �p�������}�N���B
#define ARCTL_FIELD_INDEX_CLASS_INHERIT(r, data, i, elem) BOOST_PP_COMMA_IF(i) public elem

/// ����֐������}�N���B
#define ARCTL_FIELD_INDEX_CLASS_SUB(r, data, elem) static_cast<elem&>(*this) = data;

/// �ے艉�Z�A���S���Y�������}�N���B
#define ARCTL_FIELD_INDEX_CLASS_NOT(r, data, i, elem) BOOST_PP_EXPR_IF(i, ||) ! *static_cast<const elem*>(this)

/// copy_name�A���S���Y�������}�N���B
#define ARCTL_FIELD_INDEX_CLASS_COPY(r, data, elem) elem::copy_name(data);

/// ��copy_name�A���S���Y�������}�N���B
#define ARCTL_FIELD_INDEX_CLASS_COPY2(r, data, elem) elem::copy_name(rClass, data);

#endif // DOXYGEN

#endif // ARCTL_FIELD_INDEX_H_
