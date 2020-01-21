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

#ifndef ARCTL_RECURSIVE_LAYER_ITERATOR_H_
#define ARCTL_RECURSIVE_LAYER_ITERATOR_H_

#include <memory>
#include <boost/scoped_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <arctl/iterator/zero_based_random_access_iterators.h>

namespace arctl {

/**
 * @brief ���C���ċA���񔽕��q
 *
 * �������C���̒����ċA�I�ɏ���B
 *
 * @par �e���v���[�g����:
 * - @a BaseIterator - ILayerPtr ��Ԃ������q�B
 */
template <typename BaseIterator>
class recursive_layer_iterator :
	public boost::iterator_facade<
		recursive_layer_iterator<BaseIterator>,
		ILayerPtr,
		boost::single_pass_traversal_tag,
		ILayerPtr
	>
{
	friend class boost::iterator_core_access;
public:
// �R���X�g���N�^
	/**
	 * @brief ���C���ċA���񔽕��q�n�_���\�z����B
	 *
	 * @param it [in] ���C�����񔽕��q�n�_�B
	 * @param last [in,optional] ���C�����񔽕��q�I�_�B
	 *
	 * @warning
	 * @a last ���ȗ��ł���̂́A BaseIterator() ���I�_���w���ꍇ�̂݁B
	 */
	recursive_layer_iterator(BaseIterator it, BaseIterator last = BaseIterator()) :
	m_it(it),
	m_itLast(last)
	{
		expand_composite_layer();
	}

	recursive_layer_iterator(const recursive_layer_iterator& rhs) :
	m_it(rhs.m_it),
	m_itLast(rhs.m_itLast),
	m_pChildRange(rhs.m_pChildRange ? new child_range(*rhs.m_pChildRange) : 0)
	{
	}

// �擾
	const BaseIterator& base() const
	{
		return m_it;
	}

	const BaseIterator& end() const
	{
		return m_itLast;
	}

// ���Z
	recursive_layer_iterator& operator=(const recursive_layer_iterator& rhs)
	{
		iterator_adaptor_::operator=(rhs);
		m_itLast = rhs.m_itLast;
		m_pChildRange.reset(rhs.m_pChildRange ? new child_range(*rhs.m_pChildRange) : 0);

		return *this;
	}

private:
// �^��`
	typedef recursive_layer_iterator<atl2::iterator_of<ICompositeLayer>::type> child_iterator; ///< �������C���̂P���̊K�w�����锽���q
	typedef std::pair<child_iterator, child_iterator> child_range; ///< �������C���̂P���̊K�w�����锽���q�̑΁B

/// @name iterator_facade�p�֐�
//@{
	/**
	 * @brief �w���Ă��郌�C����Ԃ��B
	 *
	 * @pre *this != end()
	 *
	 * @return �����q���w���Ă��郌�C���B
	 */
	ILayerPtr dereference() const
	{
		return m_pChildRange ? ILayerPtr(*m_pChildRange->first) : *m_it;
	}

	/**
	 * @brief �����v�f���w���Ă��邩�B
	 *
	 * @param rhs [in] ��r�Ώ۔����q�B
	 * @retval true �����v�f���w���Ă���B
	 * @retval false �قȂ�v�f���w���Ă���B
	 */
	bool equal(const recursive_layer_iterator& rhs) const
	{
		if(m_it != rhs.m_it)
			return false;

		return
			m_pChildRange
			? (rhs.m_pChildRange ? m_pChildRange->first == rhs.m_pChildRange->first : false)
			: (! rhs.m_pChildRange);
	}

	/**
	 * @brief ���̗v�f���w���B
	 *
	 * @pre *this != end()
	 *
	 * �������C���̒��������Ă���Ԃ́A�������C���̒��̎��̗v�f���w���B<br>
	 * �������C�����߂���I������玟�̗v�f�ֈړ����A expand_composite_layer() ���s���B
	 */
	void increment()
	{
		if(m_pChildRange) {
			if(++(m_pChildRange->first) != m_pChildRange->second)
				return;

			m_pChildRange.reset();
		}

		++m_it;

		expand_composite_layer();
	}
//@}

	/**
	 * @brief �������C���̒�������n�߂�B
	 *
	 * ���ݎw���Ă���v�f���������C���ł͂Ȃ��ꍇ�͉������Ȃ��B
	 *
	 * @pre ! m_pChildRange
	 */
	void expand_composite_layer()
	{
		// �������C����W�J���ɁA����
		_ASSERT(! m_pChildRange);

		for(; m_it != m_itLast; ++m_it) {
			// �������C�����B
			ICompositeLayerPtr ipCompositeLayer(*m_it);
			if(ipCompositeLayer == 0)
				break;

			// �������C����W�J�B
			std::auto_ptr<child_range>
				pChildRange(new child_range(atl2::begin(ipCompositeLayer), atl2::end(ipCompositeLayer)));

			// �������C���̒��g���󂶂�Ȃ����Ƃ��m�F�B
			if(pChildRange->first != pChildRange->second) {
				m_pChildRange.reset(pChildRange.release());
				break;
			}
		}
	}

// �ϐ�
	BaseIterator m_it, m_itLast; ///< �I�_�����q�B
	boost::scoped_ptr<child_range> m_pChildRange; ///< �������C���̎n�I�_�����q�B
};

/**
 * @brief ���C���ċA���񔽕��q���쐬����B
 *
 * @param first [in] ���C�����񔽕��q�n�_�B
 * @param last [in,optional] ���C�����񔽕��q�I�_�B
 * @return ���C���ċA���񔽕��q�B
 */
template <typename BaseIterator>
inline recursive_layer_iterator<BaseIterator>
make_recursive_layer_iterator(BaseIterator first, BaseIterator last = BaseIterator())
{
	return recursive_layer_iterator<BaseIterator>(first, last);
}

} // namespace arctl

#endif // ARCTL_RECURSIVE_LAYER_ITERATOR_H_
