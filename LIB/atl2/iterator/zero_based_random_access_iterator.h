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
 * @file zero_based_random_access_iterator.h
 */
#ifndef ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_
#define ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_

#include <algorithm>
#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#pragma warning( pop )
#include <boost/iterator/reverse_iterator.hpp>
#include <atl2/exception.h>
#include <atl2/iterator/element_wrapper.h>

namespace atl2 {

/**
 * @brief 0�N�_�̃����_���A�N�Z�X�����q�B
 *
 * ArcObjects�� @b IFields �ȂǁA0���N�_�Ƃ���long�^���L�[�ɂ��������_���A�N�Z�X�����q�B<br>
 *
 * @attention
 * dereference() �̓����o�ϐ��́A distance_to() �͓n���ꂽ�����q�̃����o�ϐ���const_cast����B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase = 0
>
class zero_based_random_access_iterator :
	public boost::iterator_facade<
		zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod>, // CRTP
		TElement,                                                                           // �v�f�^
		boost::random_access_traversal_tag,                                                 // �J�e�S��
		typename detail::element_wrapper_of<TElement>::type                                 // �Q�ƌ^
	>
{
	friend class boost::iterator_core_access;
	typedef reference element_wrapper;
public:
// ��`
	typedef TIContainer container_type;

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
	typedef std::_Undefined_inner_type_tag _Inner_type;
#endif //  (1400 <= _MSC_VER) && (_MSC_VER < 1700)

//@{ @name �\�z/����
	/**
	 * @brief �x���]�������q���\�z����B
	 *
	 * �x���]�������q�́A�ʏ�̔����q�Ƃ̔�r���ɁA�C���f�b�N�X����ӂɒ�܂�B
	 *
	 * @param bBegin [in] �n�[�����q���B
	 */
	zero_based_random_access_iterator(bool bBegin = false) :
	m_ipContainer(0),
	m_nIndex(bBegin ? TBase : -1)
	{
	}

	/**
	 * @brief �����q���\�z����B
	 *
	 * @param ipContainer [in] �R���e�i�B
	 * @param nIndex [in] �v�f�̃C���f�b�N�X�B
	 */
	zero_based_random_access_iterator(TIContainer* ipContainer, long nIndex) :
	m_ipContainer(ipContainer),
	m_nIndex(nIndex)
	{
	}
//@}

//@{ @name �擾
	TIContainer* container() const
	{
		return m_ipContainer; 
	}
//@}

private:
//@{ @name �����q�̒�`
	element_wrapper dereference() const
	{
		_ASSERT(m_ipContainer != 0);
		element_wrapper aElement;
		atl2::valid((m_ipContainer->*TGetMethod)(m_nIndex, &aElement));
		return aElement;
	}

	void increment()
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex++;
	}

	void decrement()
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex--;
	}

	void advance(difference_type n)
	{
		_ASSERT(m_ipContainer != 0);
		m_nIndex += n;
	}

	bool equal(const zero_based_random_access_iterator& rhs) const
	{
		return distance_to(rhs) == 0;
	}

	difference_type distance_to(const zero_based_random_access_iterator& rhs) const
	{
		if(m_ipContainer == 0) {
			// �x���]�������q���m�̔�r�B
			if(rhs.m_ipContainer == 0) {
				// �قȂ�x���]�������q�Ԃ̋��������߂邱�Ƃ͂ł��Ȃ��B
				_ASSERT(rhs.m_nIndex == m_nIndex);
			}
			else 
				return rhs.distance_to(*this);
		}
		else {
			if(rhs.m_ipContainer == 0) {
				// �x���]�������q�̒l��ݒ�B
				zero_based_random_access_iterator& r = const_cast<zero_based_random_access_iterator&>(rhs);

				r.m_ipContainer = m_ipContainer;
				if(r.m_nIndex == -1) {
					atl2::valid((m_ipContainer->*TCountMethod)(&r.m_nIndex));
					r.m_nIndex += TBase;
				}
			}
		}

		return rhs.m_nIndex - m_nIndex;
	}
//@}

// �ϐ�
	_com_ptr_t<_com_IIID<TIContainer, &__uuidof(TIContainer)> > m_ipContainer; ///< �R���e�i�B�x���]�������q�̏ꍇ��null�B
	long m_nIndex; ///< �C���f�b�N�X�B�x���]�������q�̏ꍇ�A-1�Ȃ�ΏI�[���w���B
};

/**
 * @brief �����_���A�N�Z�X�\�ȃR���e�i�̓����B
 *
 * @par �e���v���[�g����:
 * - TIContainer : �C���^�t�F�[�X�B
 * - TElement : �R���e�i�Ɋ܂܂��v�f�B
 * - TGetNextMethod : ���̗v�f�����o���֐��B
 * - TCountMethod : �v�f���𐔂���֐��B
 * - TBase : �N�_�B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase = 0
>
struct zero_based_random_access_iterator_traits
{
// ��`
	typedef zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod, TBase> iterator;
	typedef boost::reverse_iterator<iterator> reverse_iterator;

	typedef iterator const_iterator;
	typedef reverse_iterator const_reverse_iterator;

///@name �����q�̐���
//@{
	/**
	 * @brief �n�[�����q�𓾂�B
	 *
	 * @param ipContainer [in] �R���e�i�Bnull�̏ꍇ�A�I�[�x���]�������q���Ԃ�B
	 */
	static iterator begin(TIContainer* ipContainer)
	{
		if(ipContainer) {
			return iterator(ipContainer, TBase);
		}
		else
			return iterator(false);
	}

	/**
	 * @brief �I�[�����q�𓾂�B
	 *
	 * @param ipContainer [in] �R���e�i�Bnull�̏ꍇ�A�I�[�x���]�������q���Ԃ�B
	 */
	static iterator end(TIContainer* ipContainer)
	{
		if(ipContainer) {
			long nIndex;
			atl2::valid((ipContainer->*TCountMethod)(&nIndex));
			return iterator(ipContainer, nIndex + TBase);
		}
		else
			return iterator(false);
	}
//@}

///@name �t�s�����q�̐���
//@{
	static reverse_iterator rbegin(TIContainer* ipContainer) { return reverse_iterator(end(ipContainer)); }
	static reverse_iterator rend(TIContainer* ipContainer) { return reverse_iterator(begin(ipContainer)); }
//@}

///@name �x���]�������q�̐���
//@{
	static iterator static_begin() { return iterator(true); }
	static iterator static_end() { return iterator(false); }

	static reverse_iterator static_rbegin() { return reverse_iterator(static_end()); }
	static reverse_iterator static_rend() { return reverse_iterator(static_begin()); }
//@}
};

} // namespace atl2


#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)

namespace std {

template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetMethod)(long, TElement*),
	HRESULT (__stdcall TIContainer::*TCountMethod)(long*),
	long TBase
>
class _Checked_iterator_category<atl2::zero_based_random_access_iterator<TIContainer, TElement, TGetMethod, TCountMethod, TBase> >
{
public:
	typedef _Range_checked_iterator_tag _Checked_cat;
};

/**
 * @defgropu atl2_zero_based_random_access_iterator_algorithm zero_base_random_access_iterator���ꉻ�ŃA���S���Y��
 *
 * ���݁iVC8�j�� std::distance() �� std::advance() �� boost::random_access_traversal_tag �𗝉��ł��Ȃ��B<br>
 * ���̂��߁A���œK�ȃA���S���Y�����g���Ă��炤���߂ɂ͂�����œƎ��ɒ�`���Ȃ���΂Ȃ�Ȃ��B
 *
 * @note
 * �V���������q�R���Z�v�g��Technical Report�Ƃ��Ď󂯓�����Ă���B<br>
 * ������͂��̓��ꉻ���v��Ȃ��Ȃ邾�낤�B
 */
//@{
template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base
>
inline std::size_t distance(
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> first,
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> last
)
{
	return last - first;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base
>
inline std::size_t distance(
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> > first,
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> > last
)
{
	return last - first;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base,
	typename Diff
>
inline void advance(
	atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base>& it,
	Diff off
)
{
	it += off;
}

template <
	typename Container,
	typename Element,
	HRESULT (__stdcall Container::*GetMethod)(long, Element*),
	HRESULT (__stdcall Container::*CountMethod)(long*),
	long Base,
	typename Diff
>
inline void advance(
	boost::reverse_iterator<atl2::zero_based_random_access_iterator<Container, Element, GetMethod, CountMethod, Base> >& it,
	Diff off
)
{
	it += off;
}
//@}

} // namespace std

#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

#endif // ATL2_ZERO_BASED_RANDOM_ACCESS_ITERATOR_H_
