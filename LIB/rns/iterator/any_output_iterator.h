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

#ifndef RNS_ANY_OUTPUT_ITERATOR_H_
#define RNS_ANY_OUTPUT_ITERATOR_H_

#include <memory>
#include <boost/shared_ptr.hpp>

namespace rns {

/**
 * @brief ���z�o�͔����q�N���X
 *
 * �g�p���锽���q�����s���Ɍ��߂���o�͔����q�B<br>
 * Boost.Function �̏o�͔����q�ŁB�̂悤�Ȃ��́B
 *
 * @par �e���v���[�g����:
 * - Value - �����q����ďo�͂���v�f�̌^�B
 *
 * @note
 * �o�͔����q�͈ȉ���2��ނɕ�������B
 * <ul>
 * <li>�Q�Ƃ͂����ɂ���ėv�f�̑�����ł��A�C���N�������g���Z���s�����ƂŎ��̗v�f���w�����Ƃ��ł������
 * <li>�v�f��������Ǝ����I�Ɏ��̑������w���A�C���N�������g���Z�q�͉��̈Ӗ��������Ȃ�����
 * </ul>
 * any_output_iterator �́A<b>�i�[���Ă��锽���q�̎�ނɊւ�炸��҂Ƃ��ē��삷��</b>�B<br>
 * ��҂ł���΁A���z�Ăяo�����u������Z�v�����ōςނ��炾�B�i�O�҂́u�C���N�������g���Z�v�̉��z�����v������j
 * 
 * @test AnyOutputIteratorTest
 */
template <typename Value>
class any_output_iterator : public std::iterator<std::output_iterator_tag, void, void, void, void>
{
public:
	/**
	 * @brief �v�f�̑�����󂯕t����N���X
	 */
	class output_proxy
	{
	public:
		virtual ~output_proxy()
		{
		}

		output_proxy& operator=(const Value& rhs)
		{
			output(rhs);
			return *this;
		}

	protected:
		/**
		 * @brief �v�f�̑�����s���A�����q�Ɏ��̗v�f���w������B
		 *
		 * @param rhs [in] �������v�f�B
		 */
		virtual void output(const Value& rhs) = 0;
	};

	/**
	 * @brief �����q���i�[����B
	 *
	 * @test VirtualOutputIteratorTest
	 */
	template <typename Iterator>
	any_output_iterator(const Iterator& it) :
	m_pIt(new output_iterator_holder<Iterator>(it))
	{
	}

	/**
	 * @brief �v�f�̑���������肷�� output_proxy ��\�ɏo���B
	 *
	 * @test VirtualOutputIteratorTest
	 */
	output_proxy& operator*()
	{
		return *m_pIt;
	}

  	/**
	 * @brief �C���N�������g���Z�ł͉����s��Ȃ��B
	 *
	 * @test VirtualOutputIteratorTest
	 */
	any_output_iterator& operator++()
	{
		return *this;
	}

	/**
	 * @brief ��u�C���N�������g�ł������s��Ȃ��B
	 */
    any_output_iterator& operator++(int)
	{
		return *this;
	}

private:
	/**
	 * @brief �R���X�g���N�^�Ŏ󂯎���������q���i�[����N���X
	 */
	template<typename Iterator>
	class output_iterator_holder : public output_proxy
	{
	public:
		output_iterator_holder(const Iterator& it) :
		m_it(it)
		{
		}

	protected:
		virtual void output(const Value& rhs)
		{
			*m_it = rhs;
			++m_it;
		}

	private:
		Iterator m_it;
	};

	boost::shared_ptr<output_proxy> m_pIt;
};

} // namespace rns

#endif // RNS_ANY_OUTPUT_ITERATOR_H_
