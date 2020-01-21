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

#ifndef ARCTL_SELECTED_LAYER_ITERATOR_H_
#define ARCTL_SELECTED_LAYER_ITERATOR_H_

#include <boost/iterator/iterator_facade.hpp>
#include <atl2/exception.h>

namespace arctl {

/**
 * @brief �R���e���c�r���[�őI������Ă��郌�C�������񂷂锽���q
 */
class selected_layer_iterator :
	public boost::iterator_facade<
		selected_layer_iterator,
		const ILayerPtr,
		boost::single_pass_traversal_tag
	>
{
	friend class boost::iterator_core_access;
public:
// �R���X�g���N�^
	/**
	 * @brief �I�_�����q���\�z����B
	 */
	selected_layer_iterator()
	{
	}

	/**
	 * @brief �n�_�����q���\�z����B
	 *
	 * ���C����I�����Ă��Ȃ��ꍇ�͏I�_�����q�ƂȂ�B
	 *
	 * @param ipCV [in] �R���e���c�r���[�B
	 */
	selected_layer_iterator(IContentsView* ipCV)
	{
		_ASSERT(ipCV);

//		_variant_t vSelectedItem(ipCV->SelectedItem);
		_variant_t vSelectedItem;
		ipCV->get_SelectedItem(&vSelectedItem);

		// �����I������Ă��Ȃ��ꍇ�� VT_NULL
		switch(V_VT(&vSelectedItem)) {
		case VT_UNKNOWN:
		case VT_DISPATCH:
			break;
		default:
			return;
		}

		m_ipSet = vSelectedItem;
		if(m_ipSet != 0) {
			// �����̃��C�����I������Ă���B
			atl2::valid(m_ipSet->Reset());
			increment();
		}
		else {
			// �P�̃��C�����I������Ă���B
			m_ipLayer = vSelectedItem;
		}
	}
	
private:
/// @name iterator_facade�p�֐�
//@{
	/**
	 * @brief �w���Ă��郌�C����Ԃ��B
	 *
	 * @pre *this != selected_layer_iterator()
	 *
	 * @return �����q���w���Ă��郌�C���B
	 */
	const ILayerPtr& dereference() const
	{
		return m_ipLayer;
	}

	/**
	 * @brief �I�_�ɒB�������𔻒肷��B
	 *
	 * @param rhs [in] ��r�Ώ۔����q�B
	 * @retval true �ǂ���̔����q���I�_���w���Ă���B
	 * @retval false �ǂ��炩�̔����q���I�[���w���Ă��Ȃ��B
	 */
	bool equal(const selected_layer_iterator& rhs) const
	{
		return m_ipLayer == 0 && rhs.m_ipLayer == 0;
	}

	/**
	 * @brief �����̃��C�����I������Ă���ꍇ�Ɍ���A���̃��C�����w���B
	 *
	 * @pre *this != selected_layer_iterator()
	 */
	void increment()
	{
		m_ipLayer = 0;

		if(m_ipSet != 0) {
			for(IUnknownPtr ipUnk; atl2::valid(m_ipSet->Next(&ipUnk)) == S_OK; ) {
				m_ipLayer = ipUnk;
				if(m_ipLayer != 0)
					break;
			}
		}
	}
//@}

// �ϐ�
	_ISetPtr m_ipSet; ///< �I������Ă��郌�C���̈ꗗ�B
	ILayerPtr m_ipLayer; ///< �����q�����w���Ă��郌�C���B
};

} // namespace arctl

#endif // ARCTL_SELECTED_LAYER_ITERATOR_H_
