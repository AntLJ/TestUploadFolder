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
#include <iterator>
#include <algorithm>
#include <functional>
#include <arctl/field.h>
#include <arctl/iterator.h>
#include <CodeanAlysis/SourceAnnotations.h>

namespace sindy {

/**
 * @brief �����N��N���X�e���v���[�g
 *
 * �����N����\������ @b Lq �I�u�W�F�N�g��v�f�Ƃ���vector�z��B<br>
 * LQ�e�[�u���̃V�[�P���X�ԍ����� @b SEQUENCE ��1�N�_�Ȃ̂ŁA <b>�z��̃C���f�b�N�X=SEQUENCE-1</b> �ƂȂ�B
 *
 * @par �e���v���[�g����:
 * Lq : �����N����\�����郊���N1��\���^�B
 */
template <typename Lq>
class lq_list : public std::vector<Lq>
{
public:
	/**
	 * @class lq_field_value_type
	 * @brief LQ�����l�ꗗ
	 */
	ARCTL_FIELD_VALUE_CLASS(
		lq_field_value_type,
		((Lq))
		(2(schema::link_queue::kSequence, long))
	);

	/**
	 * @brief LQ�}�������q
	 *
	 * �������܂ꂽ lq_field_value_type �i���邢�͂���ɈÖق̌^�ϊ����\�ȁj�I�u�W�F�N�g�� lq_list::insert_lq() �ɓn�������q�B
	 */
	class lq_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
	{
		typedef lq_insert_iterator self;
	public:
		lq_insert_iterator(lq_list& rList) :
		m_rLqList(rList)
		{
		}

		template<typename Row>
		self& operator=(Row const& rRow)
		{
			m_rLqList.insert_lq(rRow);
			return *this;
		}

		self& operator*() { return *this; }
		self& operator++() { return *this; }

	private:
		lq_list& m_rLqList;
	};

	/**
	 * @brief LQ��}������B
	 *
	 * @param row [in] lq_field_value_type ���A����ɈÖق̌^�ϊ����\�ȃI�u�W�F�N�g�B
	 */
	template<typename Row>
	void insert_lq(Row const& row)
	{
		using namespace arctl::field;
		using namespace schema::link_queue;

		insert_lq(value_t<kSequence, long>(row).value(), row);
	}

	/**
	 * @brief LQ��}������B
	 *
	 * @param sequence [in] �V�[�P���X�ԍ��B
	 * @param row [in] @b Lq ���A����ɈÖق̌^�ϊ����\�ȃI�u�W�F�N�g�B
	 */
	template<typename Row>
	void insert_lq(long sequence, Row const& row)
	{
		// �V�[�P���X�ԍ���1�ȏ�ł���B
		if(sequence <= 0)
			throw std::underflow_error("sequence <= 0.");

		// �K�v�ɉ����Ĕz����g������B
		if(size() < static_cast<unsigned>(sequence)) {
			resize(sequence);
		}

		// LQ���}������B
		(*this)[sequence - 1] = row;
	}

	/**
	 * @brief LQ�e�[�u���ƃ����N��ID�w��Ŕz��̗v�f�����蓖�Ă�B
	 *
	 * @param ipLqTable [in] LQ�e�[�u���B
	 * @param inf_id [in] �����N��ID�B
	 */
	void assign_lq([SA_Pre(Null=SA_No)] ITable* ipLqTable, long inf_id)
	{
		assign_lq(ipLqTable, inf_id, lq_field_value_type::index_type(ipLqTable));
	}

	/**
	 * @brief LQ�e�[�u���ƃ����N��ID�w��Ŕz��̗v�f�����蓖�Ă�B
	 *
	 * @param ipLqTable [in] LQ�e�[�u���B
	 * @param inf_id [in] �����N��ID�B
	 * @param index [in] �����t�B�[���h�C���f�b�N�X�B
	 */
	void assign_lq([SA_Pre(Null=SA_No)] ITable* ipLqTable, long inf_id, const typename lq_field_value_type::index_type& index)
	{
		using namespace atl2;
		using namespace arctl;
		using namespace schema::link_queue;

		clear();

		const IQueryFilterPtr ipQueryFilter(__uuidof(QueryFilter));
		valid(ipQueryFilter->put_SubFields(CComBSTR(_T(""))));
		lq_field_value_type::index_type::copy_name(ipLqTable, adder(ipQueryFilter, &IQueryFilter::_AddField));

		std::basic_ostringstream<TCHAR> oss;

		std::locale loc(std::locale("japanese"), "C", std::locale::numeric);
		oss.imbue(loc);

		oss << kInfID << _T("=") << inf_id;
		valid(ipQueryFilter->put_WhereClause(CComBSTR(oss.str().c_str())));

		_ICursorPtr ipCursor;
		valid(ipLqTable->Search(ipQueryFilter, VARIANT_FALSE, &ipCursor));

		std::transform(
			atl2::begin(ipCursor),
			atl2::end(ipCursor),
			lq_inserter(),
			field::interpreter<lq_field_value_type>(&index)
		);
	}

	/**
	 * @brief LQ�}�������q�𐶐�����B
	 */
	lq_insert_iterator lq_inserter()
	{
		return lq_insert_iterator(*this);
	}
};

/**
 * 
 */
template <typename LqListSpecifier>
class indirect_lq_insert_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
	typedef indirect_lq_insert_iterator self;
public:
	indirect_lq_insert_iterator(LqListSpecifier cSpecifier) :
	m_Specifier(cSpecifier)
	{
	}

	template <typename Lq>
	self& operator=(const Lq& rLq)
	{
		insert_lq(m_Specifier(rLq), rLq);
		return *this;
	}
	
	self& operator*() { return *this; }
	self& operator++() { return *this; }

private:
	template <typename LqList, typename Lq>
	void insert_lq(LqList& rLqList, const Lq& rLq)
	{
		rLqList.insert_lq(rLq);
	}

	template <typename LqList, typename Lq>
	void insert_lq(LqList* pLqList, const Lq& rLq)
	{
		if(pLqList) {
			insert_lq(*pLqList, rLq);
		}
	}

	LqListSpecifier m_Specifier;
};

template <typename LqListSpecifier>
indirect_lq_insert_iterator<LqListSpecifier> make_indirect_lq_insert_iterator(LqListSpecifier& rLqListSpecifier)
{
	return indirect_lq_insert_iterator<LqListSpecifier>(rLqListSpecifier);
}

} // namespace sindy

#endif // SINDY_LQ_LIST_H_
