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
 * @file single_pass_iterator.h
 */
#ifndef ATL2_SIGLE_PASS_ITERATOR_H_
#define ATL2_SIGLE_PASS_ITERATOR_H_

#pragma warning( push )
#pragma warning( disable : 4284 )
#include <boost/iterator/iterator_facade.hpp>
#pragma warning( pop )
#include <atl2/exception.h>
#include <atl2/iterator/element_wrapper.h>

namespace atl2 {

/**
 * @brief �P�����������q�B
 *
 * <b>IEnum*</b>�n�̂悤�ɁA�n�܂肩��I���܂ŒP���ɑ������邱�Ƃ����ł��Ȃ��R���e�i�̔����q�B<br>
 * �����q���m�̔�r�́A���҂��I�[�����q�ł��邩�ۂ��̔��肵���ł��Ȃ��B
 *
 * @note
 * ���̔����q�� single pass iterator �łȂ̂ŁA�������Ă��u�ʒu�v�͕ۑ�����Ȃ����A�����q���w���u�v�f�v�͕ۑ������B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
class single_pass_iterator :
	public boost::iterator_facade<
		single_pass_iterator<TIContainer, TElement, TGetNextMethod>, // CRTP
		TElement,                                                    // �v�f�^
		boost::single_pass_traversal_tag,                            // �J�e�S��
		typename detail::element_wrapper_of<TElement>::type          // �Q�ƌ^
	>
{
	friend class boost::iterator_core_access;
	typedef reference element_wrapper;
public:
// ��`
	typedef TIContainer container_type;

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)
	typedef std::_Undefined_inner_type_tag _Inner_type;
#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

//@{ @name �R���X�g���N�^�ƃf�X�g���N�^
	/**
	 * @brief �I�[�����q���\�z����B
	 */
	single_pass_iterator() :
	m_ipContainer(0)
	{
	}

	/**
	 * @brief �n�[�����q���\�z����B
	 *
	 * @param ipContainer [in] �R���e�i�B
	 */
	explicit single_pass_iterator(TIContainer* ipContainer) :
	m_ipContainer(ipContainer)
	{
		if(m_ipContainer != 0) {
			increment();
		}
	}

	~single_pass_iterator()
	{
	}
//@}

//@{ @name �擾
	TIContainer* container() const { return m_ipContainer; }
//@}

private:
//@{ @name �����q�̒�`
	element_wrapper dereference() const
	{
		return m_Element;
	}

	void increment()
	{
		_ASSERT(m_ipContainer != 0);
		atl2::valid((m_ipContainer->*TGetNextMethod)(&m_Element));
	}

	bool equal(single_pass_iterator const& rIterator) const
	{
		return m_Element == 0 && rIterator.m_Element == 0;
	}
//@}

// �ϐ�
	_com_ptr_t<_com_IIID<TIContainer, &__uuidof(TIContainer)> > m_ipContainer; ///< �R���e�i�B
	element_wrapper m_Element; ///< ���ݎw�������Ă���v�f�B
};

/**
 * @brief �P�������݂̂��\�ȃR���e�i�̓����B
 *
 * @par �e���v���[�g����:
 * - TIContainer : �C���^�t�F�[�X�B
 * - TElement : �R���e�i�Ɋ܂܂��v�f�B
 * - TGetNextMethod : ���̗v�f�����o���֐��B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
struct single_pass_container_traits
{
	typedef single_pass_iterator<TIContainer, TElement, TGetNextMethod> iterator;

	static iterator begin(TIContainer* ipContainer) { return iterator(ipContainer); }
	static iterator end(TIContainer*) { return iterator(); }

	static iterator static_end() { return iterator(); }
};

/**
 * @brief �ŏ��̗v�f���擾���邽�߂� Reset() ���� single_pass_container_traits �B
 *
 * @par �e���v���[�g����:
 * - TIContainer : �C���^�t�F�[�X�B
 * - TElement : �R���e�i�Ɋ܂܂��v�f�B
 * - TGetNextMethod : ���̗v�f�����o���֐��B
 */
template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*) = &TIContainer::Next
>
struct enumeration_container_traits : public single_pass_container_traits<TIContainer, TElement, TGetNextMethod>
{
	static iterator begin(TIContainer* ipContainer)
	{
		if(ipContainer) {
			ipContainer->Reset();
		}
		return iterator(ipContainer);
	}
};

} // namespace atl2

#if (1400 <= _MSC_VER) && (_MSC_VER < 1700)

namespace std {

template <
	typename TIContainer,
	typename TElement,
	HRESULT (__stdcall TIContainer::*TGetNextMethod)(TElement*)
>
class _Checked_iterator_category<atl2::single_pass_iterator<TIContainer, TElement, TGetNextMethod> >
{
public:
	typedef _Range_checked_iterator_tag _Checked_cat;
};

} // namespace std

#endif // (1400 <= _MSC_VER) && (_MSC_VER < 1700)

#endif // ATL2_SIGLE_PASS_ITERATOR_H_
