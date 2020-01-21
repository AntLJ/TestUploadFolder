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

#ifndef SINDY_LQ_LIST_H_
#define SINDY_LQ_LIST_H_

#include <vector>
#include <algorithm>
#include <functional>
#include <rns/sstream.h>
#include <arctl/field.h>
#include <arctl/iterator.h>
#include <../../LIB/sindy/schema.h>
#include <../../LIB/arclib/sindy/link_queue/lq.h>

namespace sindy {

/**
 * @brief �����N��N���X
 *
 * �����N��� @a Lq ��0�N�_�z��ŕ\�������́B<br>
 * Sequence�����l��1�N�_�Ȃ̂ŁA�z��ԍ�=�V�[�P���X-1�ƂȂ�B
 *
 * @par �e���v���[�g����:
 * Lq : �����N����\�����郊���N�P��\���^�B
 */
template <typename Lq>
class lq_list_base : public std::vector<Lq>
{
public:
// ��`
	/// Lq�����l�ꗗ�B
	ARCTL_FIELD_VALUE_CLASS(lq_field_value_type, ((Lq)) (2(link_queue::kSequence, long)));
	/// �}�������q�B
	class insert_iterator
	{
	public:
		insert_iterator(lq_list_base& rList, const lq_field_value_type::index_type* pIndex = 0) :
		m_rLqList(rList),
		m_pIndex(pIndex)
		{
		}

		insert_iterator(lq_list_base& rList, const lq_field_value_type::index_type& rIndex) :
		m_rLqList(rList),
		m_pIndex(&rIndex)
		{
		}

		insert_iterator& operator=(IRowBuffer* ipRow)
		{
			if(m_pIndex) {
				m_rLqList.insert_lq(lq_field_value_type(ipRow, *m_pIndex));
			}
			else {
				m_rLqList.insert_lq(lq_field_value_type(ipRow));
			}

			return *this;
		}

		insert_iterator& operator*() { return *this; }
		insert_iterator& operator++() { return *this; }

	private:
		lq_list_base& m_rLqList;
		const lq_field_value_type::index_type* m_pIndex;
	};

// �ݒ�
	/**
	 * @brief LQ�s��ǉ�����
	 *
	 * @param row [in] LQ�s�B
	 * @retval true �ǉ������B
	 * @retval false �ǉ����s�B
	 */
	template<typename Row>
	bool insert_lq(Row const& row)
	{
		return insert_lq(arctl::field::value_t<link_queue::kSequence, long>(row).value(), row);
	}

	/**
	 * @brief LQ�s��ǉ�����
	 *
	 * @param sequence [in] �V�[�P���X�ԍ��B
	 * @param row [in] LQ�s�B
	 * @retval true �ǉ������B
	 * @retval false �ǉ����s�B
	 */
	template<typename Row>
	bool insert_lq(long sequence, Row const& row)
	{
		// �V�[�P���X�ԍ���1�ȏ�ł���B
		if(sequence <= 0)
			return false;

		// �K�v�ɉ����Ĕz����g������B
		if(size() < static_cast<unsigned>(sequence)) {
			resize(sequence);
		}

		// LQ���o�^����B
		(*this)[sequence - 1] = row;

		return true;
	}

	/**
	 * @brief LQ�e�[�u���ƃ����N��ID�w��Ŕz����\�z�i�C���f�b�N�X�K�{�Łj�B
	 *
	 * @param ipLqTable [in] LQ�e�[�u���B
	 * @param inf_id [in] �����N��ID�B
	 * @param index [in] �����C���f�b�N�X�B
	 */
	void build(ITable* ipLqTable, long inf_id, const lq_field_value_type::index_type& index)
	{
		build(ipLqTable, inf_id, &index);
	}

	/**
	 * @brief LQ�e�[�u���ƃ����N��ID�w��Ŕz����\�z�i�C���f�b�N�X�C�ӔŁj�B
	 *
	 * @param ipLqTable [in] LQ�e�[�u���B
	 * @param inf_id [in] �����N��ID�B
	 * @param index [in,optioanl] �����C���f�b�N�X�B
	 */
	void build(ITable* ipLqTable, long inf_id, const lq_field_value_type::index_type* index = 0)
	{
		using namespace arctl;

		// �t�B���^�p�ӁB
		IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		ipQueryFilter->put_SubFields(CComBSTR(link_queue::kLinkId));
		Lq::index_type::copy_name(adder(ipQueryFilter, &IQueryFilter::_AddField));

		// Where��p�ӁB
		rns::tostringstream oss;
		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);
		oss << link_queue::kInfId << _T("=") << inf_id;
		ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str()));

		// ���o���ꂽLQ�Q�Ŕz����\�z�B
		std::copy(
			atl2::begin(ipLqTable->_Search(ipQueryFilter, VARIANT_FALSE)),
			atl2::end<ICursor>(),
			insert_iterator(*this, index)
		);
	}

// �擾
	insert_iterator make_insert_iterator() { return insert_iterator(*this); }
	insert_iterator make_insert_iterator(const lq_field_value_type::index_type& index) { return insert_iterator(*this, index); }

// ����
	/**
	 * @brief �w�肵�������NID�̕��тɁA���̃����N��͊܂܂�邩
	 *
	 * @par �e���v���[�g����:
	 * - InputIterator �����q�̌^�B
	 * �����q�̗v�f�� arctl::field::value_t<link_queue::kLinkId, long> �ֈÖق̌^�ϊ����s���Ȃ���΂Ȃ�Ȃ��B
	 *
	 * @param first [in] �n�_�����q�B
	 * @param last [in] �I�_�����q�B
	 * @retval true �܂܂��B
	 * @retval false �܂܂�Ȃ��B
	 */
	template <typename InputIterator>
	bool within(InputIterator first, InputIterator last) const
	{
		// �v�f���̍��𓾂�B
		int gap = std::distance(first, last) - size();

		// �܂܂�邩�ۂ����肷��B
		for(int i = 0; i <= gap; i++, ++first) {
			if(std::equal(begin(), end(), first, std::equal_to<arctl::field::value_t<link_queue::kLinkId, long> >()))
				return true;
		}

		return false;
	}
};

/**
 * @brief LQ��^�����N��N���X��`�B
 *
 * �����N����u�����NID�v�Ɓu�����v�̔z��ŕ\�������́B
 */
typedef lq_list_base<lq_t> lq_list;

/**
 * @brief �����NID��^�����N��N���X��`�B
 *
 * �����N����u�����NID�v�̔z��ŕ\�������́B
 */
typedef lq_list_base<arctl::field::value_t<link_queue::kLinkId, long> > link_id_list;

/**
 * @brief sindy::lq3 �ɂ��A�����N�̕��уN���X��`�B 
 *
 * �����N����u�����NID�v�Ɓu�����v�Ɓu���b�V���R�[�h�v�̔z��ŕ\�������́B
 */
typedef lq_list_base<lq3_t> lq3_list;

} // namespace sindy

#endif // SINDY_LQ_LIST_H_
