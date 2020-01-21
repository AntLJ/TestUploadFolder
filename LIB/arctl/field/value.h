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
 * @file value.h
 * @brief �ÓI�����t�B�[���h�l�N���X��`�w�b�_�B
 */
#ifndef ARCTL_FIELD_VALUE_H_
#define ARCTL_FIELD_VALUE_H_

#include <functional>
#include <boost/preprocessor.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/operators.hpp>
#include <boost/call_traits.hpp>
#include <arctl/field/index.h>
#include <arctl/detail/multi_inherit.h>

namespace arctl {

namespace field {

/**
 * @brief IRowBuffer ���瑮���l���֐��N���X
 *
 * @note
 * �֐��̎����́A�ȉ��̃R�[�h�Ƃقړ����ł���B
 * @code
 * std::mem_fun(&IRowBuffer::GetValue)
 * @endcode
 */
struct get_value : public std::binary_function<IRowBuffer*, long, _variant_t>
{
	_variant_t operator()(IRowBuffer* ipRowBuffer, long nIndex) const
	{
		VARIANT va;
		::VariantInit(&va);
		atl2::valid(ipRowBuffer->get_Value(nIndex, &va));
		return _variant_t(va, false);
	}
};

/**
 * @brief �C���f�b�N�X���L���l�ł���ꍇ�Ɍ��� IRowBuffer ���瑮���l�����o���֐��N���X
 *
 * �C���f�b�N�X�������l�i-1�j�ł���ꍇ�A�߂�l�� VT_EMPTY �ƂȂ�B
 */
struct get_optional_value : public std::binary_function<IRowBuffer*, long, _variant_t>
{
	_variant_t operator()(IRowBuffer* ipRowBuffer, long nIndex) const
	{
		return nIndex >= 0 ? get_value()(ipRowBuffer, nIndex) : _variant_t();
	}
};

/**
 * @brief �����l�N���X�̊�ꕔ�B
 *
 * @par �e���v���[�g����:
 * - TFieldName : �����t�B�[���h���B
 */
template <LPCTSTR FieldName>
struct value_base_t : public arctl::detail::multi_inherit::base_t
{
// ��`
	typedef field::index_t<FieldName> index_type;

//@{ @name �ÓI�֐�
	/// �����t�B�[���h�����擾����B
	static LPCTSTR name() { return FieldName; }
//@}
};

template <LPCTSTR FieldName, typename ValueType>
class value_holder_t : public value_base_t<FieldName>, boost::totally_ordered<value_holder_t<FieldName, ValueType> >
{
public:
// ��`
	typedef ValueType value_type;

/// @name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	/**
	 * @brief �����l�̓f�t�H���g�R���X�g���N�^�ŏ����������B
	 *
	 * @note
	 * VC7.0�ȑO�̃R���p�C���ł́APOD�^�̃����o�ϐ����f�t�H���g�������Ń[�����������Ă���Ȃ��̂ŁA�������׍H���Ă���B
	 */
	value_holder_t() :
#if _MSC_VER <= 1300
	m_Value(ValueType())
#else
	m_Value()
#endif
	{
	}

	value_holder_t(typename boost::call_traits<ValueType>::param_type cVal) :
	m_Value(cVal)
	{
	}
//@}

/// @name �擾
//@{
	/// �����l�̉ώQ�Ƃ�Ԃ��B
	ValueType& value() { return m_Value; }

	/// �����l�̕s�ώQ�Ƃ�Ԃ��B
	const ValueType& value() const { return m_Value; }
//@}

/// @name ���Z�q
//@{
	value_holder_t& operator=(typename boost::call_traits<ValueType>::param_type cValue)
	{
		m_Value = cValue;
		return *this;
	}

	template <typename ValueType2>
	bool operator==(const value_holder_t<FieldName, ValueType2>& rhs) const
	{
		return std::equal_to<ValueType>()(value(), rhs.value());
	}

	bool operator==(typename boost::call_traits<ValueType>::param_type rhs) const
	{
		return std::equal_to<ValueType>()(value(), rhs);
	}

	template <typename ValueType2>
	bool operator<(const value_holder_t<FieldName, ValueType2>& rhs) const
	{
		return std::less<ValueType>()(value(), rhs.value());
	}

	bool operator<(typename boost::call_traits<ValueType>::param_type rhs) const
	{
		return std::less<ValueType>()(value(), rhs);
	}
//@}

private:
	value_type m_Value; ///< �����l�B
};

/**
 * @defgroup field_value_group �����l�N���X
 *
 * �����l�̎擾�E�ێ��E�i�\�ł���΁j�����o�����s���A@ref arctl::detail::multi_inherit::base_t�p���N���X�B<br>
 * �K�v�Ȓ�`�E�֐�������邱�ƂŁA@ref ARCTL_FIELD_VALUE_CLASS�ɂ�鑽�d�p�����\�ƂȂ�B
 */
//@{

/**
 * @brief value_t �� @ref value_t::write() "write()" �������ŁB
 */
template <LPCTSTR FieldName, typename ValueType, typename GetFunction = get_value>
class static_value_t : public value_holder_t<FieldName, ValueType>
{
	typedef value_holder_t<FieldName, ValueType> super;
public:
//@{ @name �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief �����l�̓f�t�H���g�R���X�g���N�^�ŏ����������B
	 */
	static_value_t()
	{
	}

	/**
	 * @brief �l�X�Ȍ^�̃I�u�W�F�N�g�ɂ�鏉���������݂�B
	 *
	 * @p rRow�͈ȉ��̂����ꂩ�ł��邱�Ƃ����҂���:
	 * - ���N���X�Ɠ��� value_base_t ���p�������I�u�W�F�N�g�B
	 * - <b>IRowBuffer*</b> �ֈÖق̌^�ϊ����\�ȃI�u�W�F�N�g�B
	 *   - @ref value_base_t::index_type "index_type" �ւ̈Öق̌^�ϊ����p�ӂ���Ă���ꍇ�A�����͍œK�������B
	 *
	 * @param rRow [in] �I�u�W�F�N�g�B
	 */
	template <typename Row>
	static_value_t(Row const& rRow) :
	super(constructor<boost::is_convertible<Row, const value_base_t<FieldName>&>::value>::construct(rRow, rRow))
	{
	}

	/**
	 * @brief <b>IRowBuffer</b>�p���I�u�W�F�N�g�Ƒ����t�B�[���h�C���f�b�N�X���狁�߂��鑮���l�ŏ���������B
	 *
	 * @param ipRowBuffer [in] �I�u�W�F�N�g�B
	 * @param rIndex [in] @ref value_base_t::index_type "index_type" �ւ̈Öق̌^�ϊ����\�ȑ����t�B�[���h�C���f�b�N�X�B
	 */
	template <typename Index>
	static_value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :
	super(constructor<false>::construct(ipRowBuffer, rIndex))
	{
	}
//@}

//@{ @name ����
	/**
	 * @brief @a m_Value �� @p ipRowBuffer �̑����l���r����B
	 *
	 * �����l�̓��ꐫ�m�F�ɂ� <b>std:equal_to<_variant_t>()</b> ��p����B
	 *
	 * @param ipRowBuffer [in] �����l��r�ΏۃI�u�W�F�N�g�B
	 * @param rIndex [in] �����t�B�[���h�C���f�b�N�X�B
	 * @retval true �����l�͓������B
	 * @retval false �����l�͓������Ȃ��B
	 */
	bool equal_value(IRowBuffer* ipRowBuffer, const index_type& rIndex) const
	{
		return std::equal_to<_variant_t>()(GetFunction()(ipRowBuffer, rIndex), value());
	}

	bool equal_value(IRowBuffer* ipRowBuffer) const
	{
		return equal_value(ipRowBuffer, ipRowBuffer);
	}
//@}

//@{ @name ���Z�q
	/**
	 * @brief �l�X�Ȍ^�̃I�u�W�F�N�g�ɂ�鑮���l�̓���ւ������݂�B
	 *
	 * ������Z�q�ł��邱�Ƃ������� value_t(TRow const&) �ɓ������B
	 *
	 * @param rRow [in] �I�u�W�F�N�g�B
	 */
	template <typename Row>
	static_value_t& operator=(Row const& rRow)
	{
		super::operator=(
			constructor<
				boost::is_convertible<Row, const value_base_t<FieldName>&>::value
			>::construct(rRow, rRow)
		);
		return *this;
	}

	/**
	 * @brief �����l�𒼐ړ���ւ���B
	 *
	 * @param cValue [in] �V���������l�B
	 */
	static_value_t& operator=(typename boost::call_traits<value_type>::param_type cValue)
	{
		super::operator=(cValue);
		return *this;
	}
//@}

private:
// ��`
	/// �\�z�⏕�N���X�B
	template <bool TCopy>
	class constructor
	{
	public:
		template <typename TRow, typename TIndex>
		static value_type construct(TRow const& rRow, TIndex const& rIndex)
		{
			typedef atl2::interface_pointer_of<TIndex>::type rhs_type;

			return
				detail_t<
					boost::is_convertible<TRow, IRowBuffer*>::value, 
					boost::is_convertible<rhs_type, index_type>::value
				>::construct(rRow, static_cast<rhs_type const&>(rIndex));
		}

	private:
		/// �l�����̂܂ܑ���B
		template <bool TIsRow, bool THaveIndex>
		struct detail_t
		{
			template <typename TValue, typename T>
			static value_type construct(TValue const& rValue, T const&) { return rValue; }
		};

		/// IRowBuffer* �� index_type ����̑������o���B
		template<>
		struct detail_t<true, true>
		{
			static value_type construct(IRowBuffer* ipRowBuffer, index_type const& rIndex)
			{
				return GetFunction()(ipRowBuffer, rIndex);
			}
		};
	};

	/// �\�z�⏕�N���X���R�s�[�R���X�g���N�^�p�ɓ��ꉻ�B
	template <>
	class constructor<true>
	{
	public:
		template <typename TRow>
		static value_type construct(TRow const& rRow, TRow const&)
		{
			return get<FieldName>(rRow);
		}
	};
};

/**
 * @brief �ÓI�ȑ����t�B�[���h���ɂ���ē��肳���@ref field_value_group�B
 *
 * ���j�[�N�Ȗ��O�ɂ���đ����t�B�[���h����肵�A�l�̎擾�E�ێ��E�����o�����s���B<br>
 * �����t�B�[���h���ɂ���ăN���X�����̉�����邽�߁A�����t�B�[���h���𓮓I�ɕς��邱�Ƃ͂ł��Ȃ��B
 *
 * @par �����l�擾�\�I�u�W�F�N�g:
 * IRowBuffer
 * @par �����l�̎擾:
 * IRowBuffer::get_Value()
 * @par �����l�̏����o��:
 * IRowBuffer::put_Value()
 *
 * @par �e���v���[�g����:
 * - FieldName : �����t�B�[���h���B
 * - ValueType : �����l�̌^�B�ȉ��Ɏ��������𖞂������ƁB�f�t�H���g�� @b _variant_t �B
 *   - �f�t�H���g�������E�����E����\
 *   - _variant_t ����Öق̌^�ϊ����\
 *   - _variant_t �ֈÖق̌^�ϊ����\
 *
 * @test @ref unit_test FieldValueTest
 */
template <LPCTSTR FieldName, typename ValueType = _variant_t>
class value_t : public static_value_t<FieldName, ValueType>
{
	typedef static_value_t<FieldName, ValueType> super;
public:
///@name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	value_t() {}

	template <typename Row>
	value_t(Row const& rRow) : super(rRow) {}

	template <typename Index>
	value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :super(ipRowBuffer, rIndex) {}
//@}

///@name ����
//@{
	/**
	 * @brief @p ipRowBuffer�ɑ����l�������o���B
	 *
	 * @param ipRowBuffer [in] �����l�������o���I�u�W�F�N�g�B
	 * @param rIndex [in] �����t�B�[���h�C���f�b�N�X�B
	 * @retval true �����̏����o���ɐ��������B
	 * @retval false �����̏����o���Ɏ��s�����B
	 */
	bool write(IRowBuffer* ipRowBuffer, const super::index_type& rIndex) const
	{
		return atl2::valid(ipRowBuffer->put_Value(rIndex, static_cast<_variant_t const&>(value()))) == S_OK;
	}

	bool write(IRowBuffer* ipRowBuffer) const
	{
		return write(ipRowBuffer, ipRowBuffer);
	}
//@}
};

template <LPCTSTR FieldName>
class optional_value_t : public static_value_t<FieldName, _variant_t, get_optional_value>
{
	typedef static_value_t<FieldName, _variant_t, get_optional_value> super;
public:
///@name �R���X�g���N�^�ƃf�X�g���N�^
//@{
	optional_value_t() {}

	template <typename Row>
	optional_value_t(Row const& rRow) : super(rRow) {}

	template <typename Index>
	optional_value_t(IRowBuffer* ipRowBuffer, Index const& rIndex) :super(ipRowBuffer, rIndex) {}
//@}

///@name ����
//@{
	/**
	 * @brief �l����l�ł͂Ȃ��A�C���f�b�N�X������Ȓl�������Ȃ�΁A @p ipRowBuffer�ɑ����l�������o���B
	 *
	 * @param ipRowBuffer [in] �����l�������o���I�u�W�F�N�g�B
	 * @param rIndex [in] �����t�B�[���h�C���f�b�N�X�B
	 * @retval true �����̏����o���ɐ��������B
	 * @retval false �����̏����o���͍s���Ȃ������B
	 */
	bool write(IRowBuffer* ipRowBuffer, const super::index_type& rIndex) const
	{
		if(V_VT(&value()) == VT_EMPTY || rIndex < 0)
			return false;

		return atl2::valid(ipRowBuffer->put_Value(rIndex, value())) == S_OK;
	}

	bool write(IRowBuffer* ipRowBuffer) const
	{
		return write(ipRowBuffer, ipRowBuffer);
	}
//@}
};

//@} // field_value_group

/**
 * @ingroup field_value_group
 * @defgroup field_value_get_group �����I�u�W�F�N�g�^�l�擾�֐�
 *
 * @ref ARCTL_FIELD_VALUE_CLASS ��`�N���X�̃I�u�W�F�N�g�A�܂��̓I�u�W�F�N�g�Ɋi�[����Ă���l�����o���֐��Q�B
 */
//@{

#define ARCTL_GLOBAL_VALUE_METHOD_DEFINE(type) \
	template <LPCTSTR FieldName> inline const type get(const static_value_t<FieldName, type>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline type& get(static_value_t<FieldName, type>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline const type get(const static_value_t<FieldName, type, get_optional_value>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline type& get(static_value_t<FieldName, type, get_optional_value>& rVal) { return rVal.value(); } \
	template <LPCTSTR FieldName> inline const value_t<FieldName, type>& value_cast(const value_t<FieldName, type>& rVal) { return rVal; } \
	template <LPCTSTR FieldName> inline value_t<FieldName, type>& value_cast(value_t<FieldName, type>& rVal) { return rVal; }

ARCTL_GLOBAL_VALUE_METHOD_DEFINE(long)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(unsigned long)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(double)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(_variant_t)
ARCTL_GLOBAL_VALUE_METHOD_DEFINE(_bstr_t)

#undef ARCTL_GLOBAL_VALUE_METHOD_DEFINE
//@}

template <typename TValue>
struct value_traits
{
	typedef typename TValue::index_type index_type;
};

/**
 * @brief IRowBuffer�h���N���X �� TValue �ɕϊ�����֐��N���X�B
 *
 * �����ϊ����邾���ł���� @b std::identity �ŗǂ��B<br>
 * ���炩���ߋ��߂Ă������C���f�b�N�X�I�u�W�F�N�g���g���邱�Ƃɂ��̃N���X�̈Ӗ�������B
 *
 * @sa @ref make_interpret_iterator_group
 */
template <typename TValue>
struct interpreter : public std::unary_function<IRowBuffer*, TValue>
{
public:
// ��`
	typedef TValue field_value_type; ///< �����l�^�B
	typedef typename value_traits<field_value_type>::index_type index_type; ///< �����C���f�b�N�X�^�B

// �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief ���炩���ߋ��߂Ă������C���f�b�N�X��n���B
	 *
	 * @p pIndex ��NULL�Ȃ̂́A @b boost::transform_iterator �̏I�[�����q�Ƃ��Ďg����悤�ɂ��邽�߁B
	 *
	 * @param pIndex [in,nullable] ���炩���ߋ��߂Ă������C���f�b�N�X�I�u�W�F�N�g�ւ̃|�C���^�B
	 */
	interpreter(const index_type* pIndex) :
	m_pIndex(pIndex)
	{
	}

///@name ���Z
//@{
	field_value_type operator()(IRowBuffer* ipRowBuffer) const
	{
		return m_pIndex ? field_value_type(ipRowBuffer, *m_pIndex) : field_value_type(ipRowBuffer); 
	}

	field_value_type operator()(_IRow* ipRow) const
	{
		return m_pIndex ? field_value_type(ipRow, *m_pIndex) : field_value_type(ipRow); 
	}

	field_value_type operator()(IFeature* ipFeature) const
	{
		return m_pIndex ? field_value_type(ipFeature, *m_pIndex) : field_value_type(ipFeature); 
	}
//@}

private:
// �����o�ϐ�
	const index_type* m_pIndex; ///< �C���f�b�N�X�I�u�W�F�N�g�B
};

/**
 * @defgroup make_interpret_iterator_group interpreter �����x���֐�
 */
//@{
template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<interpreter<TValue>, TInputIterator>
	make_interpret_iterator(TInputIterator const& it, typename value_traits<TValue>::index_type const& index)
{
	return make_interpret_iterator<TValue>(it, &index);
}

template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<interpreter<TValue>, TInputIterator>
	make_interpret_iterator(TInputIterator const& it, typename value_traits<TValue>::index_type const* index = 0)
{
	return boost::transform_iterator<interpreter<TValue>, TInputIterator>(it, interpreter<TValue>(index));
}
//@}

template <typename TValue>
struct indirect_t : public std::unary_function<const TValue, typename TValue::value_type>
{
public:
	typedef TValue field_value_type;
	typename field_value_type::value_type operator()(const field_value_type& rFieldValue) const { return rFieldValue.value(); }
};

template <typename TValue, typename TInputIterator>
inline boost::transform_iterator<indirect_t<TValue>, TInputIterator>
	make_indirect_iterator(TInputIterator const& it)
{
	return boost::transform_iterator<detail::indirect_t<TValue>, TInputIterator>(it, indirect_t<TValue>());
}

} // namespace field
} // namespace arctl

/**
 * @brief @ref field_value_group�𑽏d�p�����A�܂Ƃ߂ĊǗ�����N���X���`����}�N���B
 *
 * @param c
 * �N���X���B
 * @param seq
 * �p������@ref field_value_group��<a href="http://boost.cppll.jp/HEAD/libs/preprocessor/doc/data/sequences.html">�V�[�P���X</a>�B<br>
 * @ref field_value_group�̎w����@�͈ȉ���3��ށB
 * -# �����l�N���X�𒼐ڎw�肷��
 *   - <b>(�����l�N���X)</b>
 * -# �������̍L��ϐ����w�肷��i<b>(@link arctl::field::value_t value_t@<�������̍L��ϐ�@>@endlink)</b>�Ɠ��`�j
 *   - <b>1(�������̍L��ϐ�) ... =</b>
 * -# �������̍L��ϐ��ƌ^���w�肷��i<b>(@link arctl::field::value_t value_t@<�������̍L��ϐ�, �^@>@endlink)</b>�Ɠ��`�j
 *   - <b>2(�������̍L��ϐ�, �^)</b>
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
 * ARCTL_FIELD_VALUE_CLASS(field_value_type, ((oid_t)) (1(kValue1)) (2(kValue2, long)));
 * @endcode
 * - <b>�N���X��:</b> field_value_type
 * - <b>�����l�ꗗ:</b>
 *   - <b>�I�u�W�F�N�gID</b>
 *     - <b>�^:</b> long �i@ref arctl::field::oid_t "oid_t" �Q�Ɓj
 *   - <b>"Value1"</b>
 *     - <b>�^:</b> _variant_t
 *   - <b>"Value2"</b>
 *     - <b>�^:</b> long
 *
 * <h2>��`�����֐�</h2>
 * �ȉ��́A�N���X @a c �ɒ�`����郁���o�֐��̈ꗗ�ł���B<br>
 * �f�t�H���g�R���X�g���N�^�ȊO�͑S�ăe���v���[�g�֐��ł��邽�߁A�K�v��������Ύ��̉�����Ȃ��B
 *
 * @code
 * c();
 * @endcode
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"�̃f�t�H���g�R���X�g���N�^���Ăяo���B
 *
 * @code
 * template <typename T>
 * explicit c(T const& rT)
 * @endcode
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"��
 * @a rT �ō\�z����B
 *
 * @code
 * template <typename TIndex>
 * c(IRowBuffer* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(_IRow* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(IFeatureBuffer* ipRow, TIndex const& rIndex)
 *
 * template <typename TIndex>
 * c(IFeature* ipRow, TIndex const& rIndex)
 * @endcode
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"��
 * @a rRow �A @a rIndex �ō\�z����B
 *
 * @code
 * c(...)
 * @endcode
 * ���d�p�����ɂ̂ݗL���ƂȂ�����B<br>
 * �����̐��͌p�������N���X�̐��Ɠ����B<br>
 * �n���ꂽ������@ref field_value_group "�����l�I�u�W�F�N�g"�����ꂼ�ꏉ��������B
 *
 * @code
 * template <typename T>
 * c& operator=(T const& rT)
 * @endcode
 * �l������Z�B<br>
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"��
 * @b operator=(rT) ���s���B
 *
 * @code
 * template <typename T>
 * c& operator==(T const& rT)
 * @endcode
 * �������Z�B<br>
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"�ɑ΂���
 * <b>std::equal_to()(*this, rT)</b> ���s���B<br>
 * �K�v�ȂƂ��ɂ̂ݎ��������悤�e���v���[�g�֐������Ă��邪�A@p T �� @b c �ɈÖٌ^�ϊ��ł��Ȃ���΂Ȃ�Ȃ��B
 *
 * @code
 * template <typename T>
 * c& operator<(T const& rT)
 * @endcode
 * ���Ȃ��r���Z�B<br>
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"���������Ŕ�r����B<br>
 * �K�v�ȂƂ��ɂ̂ݎ��������悤�e���v���[�g�֐������Ă��邪�A@p T �� @b c �ɈÖٌ^�ϊ��ł��Ȃ���΂Ȃ�Ȃ��B
 *
 * @code
 * template <typename TRow>
 * bool write(TRow const& rRow) const
 * @endcode
 * �l�̏����o���B<br>
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"��
 * @ref arctl::field::value_t::write() "write(rRow, rRow)" ���s���B<br>
 * �߂�l�͘_���a�ŕԂ����B
 *
 * @code
 * template <typename TRow, typename TIndex>
 * bool write(TRow const& rRow, TIndex const& rIndex) const
 * @endcode
 * �l�̏����o���B<br>
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"��
 * @ref arctl::field::value_t::write() "write(rRow, rIndex)" ���s���B<br>
 * �߂�l�͘_���a�ŕԂ����B
 *
 * @code
 * template <typename Function>
 * Function for_each(Function f) const
 * @endcode
 * �S�Ă�@ref field_value_group "�����l�I�u�W�F�N�g"�Ɋ֐� @a f() ��K�p����B
 *
 * @test @ref unit_test FieldValueTest
 */
#define ARCTL_FIELD_VALUE_CLASS(c, seq)											\
	class c : BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_INHERIT, _, seq), boost::totally_ordered<c> \
	{																			\
		friend arctl::detail::multi_inherit::function_selector<true>;			\
	public:																		\
		ARCTL_FIELD_INDEX_CLASS_D(index_type, BOOST_PP_SEQ_TRANSFORM(ARCTL_FIELD_VALUE_CLASS_INDEX, _, seq));	\
		enum { is_arctl_multi_inherit = true }; \
	\
		c() {}	\
	\
		template <typename T> explicit c(T const& rT) :									\
		BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN1, rT, seq) {}	\
	\
		BOOST_PP_REPEAT_FROM_TO(0, BOOST_PP_ARRAY_SIZE(ARCTL_ROW_FAMILY), ARCTL_FIELD_VALUE_CLASS_CONSTRUCTS2, (c, seq))	\
	\
		BOOST_PP_EXPR_IF(BOOST_PP_GREATER(BOOST_PP_SEQ_SIZE(seq), 1), ARCTL_FIELD_VALUE_CLASS_CONSTRUCT(c, seq))	\
	\
		template <typename T>											\
		c& operator=(T const& rT)										\
		{																\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_SUB, rT, seq)	\
			return *this;												\
		}																\
	\
		template <typename T>																				\
		bool operator==(T const& rT) const																	\
		{																									\
			return BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_EQUAL, static_cast<const c&>(rT), seq);	\
		}																									\
	\
		template <typename T>																	\
		bool operator<(T const& rT) const														\
		{																						\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_LESS, static_cast<const c&>(rT), seq)	\
			return false;																		\
		}																						\
	\
		template <typename TRow>			\
		bool write(TRow const& rRow) const	\
		{									\
			return write(rRow, rRow);		\
		}									\
	\
		template <typename TRow, typename TIndex>							\
		bool write(TRow const& rRow, TIndex const& rIndex) const			\
		{																	\
			bool bResult = true;											\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_WRITE, _, seq);	\
			return bResult;													\
		}																	\
	\
		template <typename TFunction>										\
		TFunction for_each(TFunction f) const								\
		{																	\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_FOR_EACH, _, seq)	\
			return f;														\
		}																	\
	\
		template <typename TFunction>														\
		void distribute_value(TFunction f) const											\
		{																					\
			f(BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_DISTRIBUTE_VALUE, _, seq));	\
		}																					\
	\
	private:																\
		template <typename TFunction>										\
		void for_each_ref(TFunction& f) const								\
		{																	\
			BOOST_PP_SEQ_FOR_EACH(ARCTL_FIELD_VALUE_CLASS_FOR_EACH, _, seq)	\
		}																	\
	}

#ifndef DOXYGEN

#define ARCTL_ROW_FAMILY (4, (_IRow, IRowBuffer, IFeatureBuffer, IFeature))

/// �����l�N���X���W�J�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_(elem) elem
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_1(name) arctl::field::value_t<name>
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_2(name, type) arctl::field::value_t<name, type>
#define ARCTL_FIELD_VALUE_CLASS_EXPAND_O1(name) arctl::field::optional_value_t<name>

/// �����^�W�J�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_(elem) elem::value_type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_1(name) arctl::field::value_t<name>::value_type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_2(name, type) type
#define ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND_O1(name, type) arctl::field::optional_value_t<name>::value_type

/// �����t�B�[���h�C���f�b�N�X�N���X���W�J�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem) BOOST_PP_CAT(ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_, elem)
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_(elem) arctl::field::value_traits< elem >::index_type
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_1(name) arctl::field::index_t<name>
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_2(name, type) arctl::field::index_t<name>
#define ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND_O1(name) arctl::field::index_t<name>

/// �p�������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_INHERIT(r, data, i, elem) BOOST_PP_COMMA_IF(i) public ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)

/// �l���ڑ�����R���X�g���N�^��`�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT(c, seq)								\
	c(BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_VALUE_ARGS, x, seq)) :	\
	BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN, x, seq) {}

// �񍀃R���X�g���N�^�����}�N���B
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCTS2(z, i, data)															\
	template <typename TIndex>																					\
	BOOST_PP_TUPLE_ELEM(2, 0, data)(BOOST_PP_ARRAY_ELEM(i, ARCTL_ROW_FAMILY)* ipRow, TIndex const& rIndex) :	\
	BOOST_PP_SEQ_FOR_EACH_I(ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN2, ipRow, BOOST_PP_TUPLE_ELEM(2, 1, data)) {}

/// �l���ڑ�����R���X�g���N�^�������Q�W�J�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_VALUE_ARGS(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) const ARCTL_FIELD_VALUE_CLASS_VALUE_TYPE_EXPAND(elem)& data ## i

/// �����t�B�[���h�C���f�b�N�X��`�}�N���B
#define ARCTL_FIELD_VALUE_CLASS_INDEX(d, data, elem) ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem)

#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data ## i)

/// �R���X�g���N�^�`�F�C�������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN1(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data)

/// �񍀃R���X�g���N�^�`�F�C�������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_CONSTRUCT_CHAIN2(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)(data, rIndex)

/// �������Z�����}�N���B
#define ARCTL_FIELD_VALUE_CLASS_EQUAL(r, data, i, elem) \
	BOOST_PP_EXPR_IF(i, &&) std::equal_to<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) >()(*this, data)

/// ���Ȃ艉�Z�����}�N���B
#define ARCTL_FIELD_VALUE_CLASS_LESS(r, data, elem)			\
	{															\
		std::less<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) > less_;	\
		if(less_(*this, data)) return true;						\
		else if(less_(data, *this)) return false;				\
	}

/// ����֐������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_SUB(r, data, elem) static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem)&>(*this) = data;

/// value_t::write() �A�����s�֐������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_WRITE(r, data, elem) \
	bResult &= static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this)->write(rRow, ARCTL_FIELD_VALUE_CLASS_INDEX_EXPAND(elem)(rIndex));

/// for_each�����}�N���B
#define ARCTL_FIELD_VALUE_CLASS_FOR_EACH(r, data, elem) \
	arctl::detail::multi_inherit::for_each_ref(*static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this), f);

/// �l���z�֐������}�N���B
#define ARCTL_FIELD_VALUE_CLASS_DISTRIBUTE_VALUE(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) static_cast<ARCTL_FIELD_VALUE_CLASS_EXPAND(elem) const*>(this)->value()

#endif // DOXYGEN

#endif // ARCTL_FIELD_VALUE_H_
