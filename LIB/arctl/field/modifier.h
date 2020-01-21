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
 * @file modifier.h
 * @brief �����l�s��v�������X�V�N���X��`�w�b�_�B
 */
#ifndef ARCTL_FIELD_VALUE_MODIFIER_H_
#define ARCTL_FIELD_VALUE_MODIFIER_H_

#include <arctl/field/value.h>

namespace arctl {

namespace field {

/**
 * @brief �����l�s��v�������X�V�N���X�B
 *
 * <b>IRowBuffer</b>�p���I�u�W�F�N�g�������Ƃ���P�����Z�N���X�B<br>
 * �l���قȂ�ꍇ�̂ݑ����l���X�V���A @b true ��Ԃ��B<br>
 * �����l�̓��ꐫ��r�ɂ�@ref arctl::field::value_t::equal_value() "equal_value()"���A
 * �l�̍X�V�ɂ�@ref arctl::field::value_t::write() "write()"���g�p����B
 */
template <typename Value>
struct modifier : public std::unary_function<IRowBuffer*, bool>
{
public:
// ��`
	typedef Value field_value_type;
	typedef typename value_traits<field_value_type>::index_type index_type;

//@{ @name �R���X�g���N�^�ƃf�X�g���N�^
	modifier(const field_value_type& rValue, const index_type& rIndex) :
	m_rValue(rValue),
	m_rIndex(rIndex)
	{
	}
//@}

//@{ @name ���Z
	bool operator()(IRowBuffer* ipRowBuffer) const
	{
		func<IRowBuffer> aFunc(ipRowBuffer, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}

	bool operator()(IFeatureBuffer* ipFeatureBuffer) const
	{
		func<IFeatureBuffer> aFunc(ipFeatureBuffer, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}

	bool operator()(IFeature* ipFeature) const
	{
		func<IFeature> aFunc(ipFeature, m_rIndex);
		arctl::detail::multi_inherit::for_each_ref(m_rValue, aFunc);
		return aFunc.result();
	}
//@}

private:
// ��`
	template <typename Interface>
	class func
	{
	public:
	//@{ @name �R���X�g���N�^�ƃf�X�g���N�^
		func(Interface* pInterface, const index_type& rIndex) :
		m_pInterface(pInterface),
		m_rIndex(rIndex),
		m_bResult(false)
		{
		}
	//@}

	//@{ @name �擾
		bool result() const { return m_bResult; }
	//@}

	//@{ @name ���Z
		template <typename ValueHolder>
		void operator()(const ValueHolder& rValueHolder)
		{
			if(! rValueHolder.equal_value(m_pInterface, m_rIndex)) {
				m_bResult |= rValueHolder.write(m_pInterface, m_rIndex);
			}
		}
	//@}

	private:
	// �ϐ�
		Interface* m_pInterface;
		const index_type& m_rIndex;
		bool m_bResult;
	};

// �ϐ�
	const field_value_type& m_rValue;
	const index_type& m_rIndex;
};

/**
 * @brief modifier�����x���֐��B
 *
 * @param rValue [in] @ref field_value_group��@ref ARCTL_FIELD_VALUE_CLASS�B
 * @param rIndex [in] @p rValue�̃C���f�b�N�X�B
 */
template <typename TValue>
inline modifier<TValue> make_modifier(const TValue& rValue, const typename TValue::index_type& rIndex)
{
	return modifier<TValue>(rValue, rIndex);
}

} // namespace field

} // namespace arctl

#endif // ARCTL_FIELD_VALUE_MODIFIER_H_
