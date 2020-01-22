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

#ifndef RNS_ANY_ITERATOR_H_
#define RNS_ANY_ITERATOR_H_

#include <boost/scoped_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/static_assert.hpp>
#include <boost/cast.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>

namespace rns {

/**
 * @brief any_iterator �p private ���O���
 */
namespace any_iterator_detail {

/**
 * @brief �����ȑ����d�l�𔲂��o�����^�֐�
 *
 * @par ����:
 * - @e Traversal - �����d�l�B
 * 
 * @par �߂�l:
 * - @e boost::random_access_traversal_tag - �����_���A�N�Z�X�����B
 * - @e boost::bidirectional_traversal_tag - �o���������B
 * - @e boost::single_pass_traversal_tag - ���͑����B
 * - @e boost::incrementable_traversal_tag - �C���N�������g�\�����B
 * - @e void - �s���B
 *
 * @note
 * iterator_categories.hpp �ɂ��郁�^�֐� boost::detail::pure_traversal_tag �̃p�N���B
 */
template <class Traversal>
struct pure_traversal_tag :
	boost::mpl::eval_if<
		boost::is_convertible<Traversal, boost::random_access_traversal_tag>,
		boost::mpl::identity<boost::random_access_traversal_tag>,
		boost::mpl::eval_if<
			boost::is_convertible<Traversal, boost::bidirectional_traversal_tag>,
			boost::mpl::identity<boost::bidirectional_traversal_tag>,
			boost::mpl::eval_if<
				boost::is_convertible<Traversal, boost::single_pass_traversal_tag>,
				boost::mpl::identity<boost::single_pass_traversal_tag>,
				boost::mpl::if_<
					boost::is_convertible<Traversal, boost::incrementable_traversal_tag>,
					boost::incrementable_traversal_tag,
					void
				>
			>
		>
	>
{
};

/**
 * @brief ���z�����q�Ɋi�[���锽���q��ێ�����N���X�̃C���^�t�F�[�X�A�y�т��̎�����񋟂��郁�^�֐�
 *
 * @par ����:
 * - @e Reference - �����q�̎Q�Ƃ͂����ɂ���ĕԂ����^�B
 * - @e Difference - �����q�Ԃ̋�����\������^�B
 *
 * @par �߂�l:
 * - interface_t
 * - implement_t
 */
template <typename Reference, typename Difference>
struct iterator_holder
{
	/**
	 * @brief ���z�����q�Ɋi�[���锽���q��ێ�����N���X�̃C���^�t�F�[�X
	 *
	 * @par �e���v���[�g����:
	 * - ���^�֐� pure_traversal_tag �̖߂�l�B
	 */
	template <typename>
	struct interface_t
	{
		typedef Reference reference;
		typedef Difference difference_type;
		typedef interface_t interface_type;
		
		virtual ~interface_t() {}

		virtual reference dereference() const = 0;
		virtual void increment() = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::single_pass_traversal_tag> : public interface_t<boost::incrementable_traversal_tag>
	{
		virtual bool equal(const interface_type& rhs) const = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::bidirectional_traversal_tag> : public interface_t<boost::single_pass_traversal_tag>
	{
		virtual void decrement() = 0;

		virtual interface_t* clone() const = 0;
	};

	template <>
	struct interface_t<boost::random_access_traversal_tag> : public interface_t<boost::bidirectional_traversal_tag>
	{
		virtual void advance(const difference_type& n) = 0;
		virtual difference_type distance_to(const interface_type& rhs) const = 0;

		virtual interface_t* clone() const = 0;
	};

	/**
	 * @brief ���z�����q�Ɋi�[���锽���q��ێ�����N���X
	 *
	 * @par �e���v���[�g����:
	 * - @e Iterator - �i�[���锽���q�B
	 */
	template <typename Iterator>
	class implement_t :
		public interface_t<
			typename pure_traversal_tag<
				typename boost::iterator_category_to_traversal<
					typename std::iterator_traits<Iterator>::iterator_category
				>::type
			>::type
		>
	{
	public:
		implement_t(const Iterator& it) :
		m_it(it)
		{
		}

	/// @name �C���^�t�F�[�X�̎����i�K�v�Ȃ��̂̂݁j
	//@{
		reference dereference() const
		{
			return *m_it;
		}

		bool equal(const interface_type& rhs) const
		{
			return m_it == dynamic_cast<const implement_t&>(rhs).m_it;
		}

		void increment()
		{
			++m_it;
		}

		void decrement()
		{
			--m_it;
		}

		void advance(const difference_type& n)
		{
			std::advance(m_it, n);
		}

		difference_type distance_to(const interface_type& rhs) const
		{
			return std::distance(m_it, dynamic_cast<const implement_t&>(rhs).m_it);
		}

		virtual implement_t* clone() const
		{
			return new implement_t(m_it);
		}
	//@}

		const Iterator& base() const
		{
			return m_it;
		}

	private:
		Iterator m_it;
	};
};

} // namespace any_iterator_detail

/**
 * @brief ���z���͔����q�N���X
 *
 * �g�p���锽���q�����s���Ɍ��߂�����͔����q�B<br>
 * Boost.Function �̓��͔����q�ŁB�̂悤�Ȃ��́B
 *
 * @par �e���v���[�g����:
 * - @e Value - �v�f�^�B
 * - @e CategoryOrTraversal - �����q�̎�ށB
 * - @e Reference - �Q�Ƃ͂����ɂ���ĕԂ����v�f�̌^�B�f�t�H���g�͗v�f�ւ̎Q�ƌ^�B
 * - @e Difference - �����q�Ԃ̋�����\���̂ɗp����^�B�f�t�H���g�� std::ptrdiff_t �B
 *
 * @attention
 * ���̃N���X��dynamic_cast��p����̂ŁA�g�p�ɂ�RTTI���K�v�B
 */
template <
	typename Value,
	typename CategoryOrTraversal,
	typename Reference = Value&,
	typename Difference = std::ptrdiff_t
>
class any_iterator :
	public boost::iterator_facade<
		any_iterator<Value, CategoryOrTraversal, Reference, Difference>,
		Value,
		CategoryOrTraversal,
		Reference,
		Difference
	>
{
	friend class boost::iterator_core_access;

	typedef typename boost::iterator_category_to_traversal<iterator_category>::type iterator_traversal;
	typedef typename any_iterator_detail::iterator_holder<Reference, Difference> iterator_holder;
	typedef typename iterator_holder::template interface_t<any_iterator_detail::pure_traversal_tag<iterator_traversal>::type> iterator_holder_interface;
public:
	/**
	 * @brief �������ނ̔����q���󂯎��A���z�������݂�B
	 */
	template <typename Iterator>
	any_iterator(const Iterator& it) :
	m_pIt(new iterator_holder::implement_t<Iterator>(it))
	{
	}

	/**
	 * @brief @e Value �� @e CategoryOrTraversal ���قȂ鉼�z�����q�́A�����I�ȕ��������݂�B
	 */
	template <typename V, typename CoT>
	any_iterator(const any_iterator<V, CoT, Reference, Difference>& rhs) :
	m_pIt(rhs.clone_iterator_holder())
	{
		BOOST_STATIC_ASSERT((boost::is_convertible<CoT, CategoryOrTraversal>::value));
	}

	/**
	 * @brief �����ȃR�s�[�R���X�g���N�^�B
	 */
	any_iterator(const any_iterator& rhs) :
	m_pIt(rhs.m_pIt->clone())
	{
	}

	/**
	 * @brief @e Value �� @e CategoryOrTraversal ���قȂ鉼�z�����q�̑�������݂�B
	 */
	template <typename V, typename CoT>
	any_iterator& operator=(const any_iterator<V, CoT, Reference, Difference>& rhs)
	{
		BOOST_STATIC_ASSERT((boost::is_convertible<CoT, CategoryOrTraversal>::value));
		m_pIt.reset(rhs.clone_iterator_holder());
		return *this;
	}

	/**
	 * @brief �����ȑ�����Z�B
	 */
	any_iterator& operator=(const any_iterator& rhs)
	{
		m_pIt.reset(rhs.m_pIt->clone());
		return *this;
	}

	/**
	 * @brief �i�[���Ă��锽���q�����o���B
	 *
	 * @par �e���v���[�g�����F
	 * - @e Iterator - �i�[���Ă��锽���q�̌^�B
	 *
	 * @exception std::bad_cast ���ۂɊi�[����Ă��锽���q�ƈقȂ�^���w�肵���B
	 */
	template <typename Iterator>
	const Iterator& base() const
	{
		return boost::polymorphic_cast<iterator_holder::implement_t<Iterator>*>(m_pIt.get())->base();
	}

	/**
	 * @brief �e���v���[�g�����������I�ɈقȂ� any_iterator �p�̔����q�ێ��I�u�W�F�N�g�����֐��B
	 *
	 * @note
	 * private���������̂����A�ǂ����@���v�������΂Ȃ��B
	 */
	iterator_holder_interface* clone_iterator_holder() const
	{
		return m_pIt->clone();
	}

private:
	reference dereference() const
	{
		return m_pIt->dereference();
	}

	bool equal(const any_iterator& rhs) const
	{
		return m_pIt->equal(*rhs.m_pIt);
	}

	void increment()
	{
		m_pIt->increment();
	}

	void decrement()
	{
		m_pIt->decrement();
	}

	void advance(difference_type n)
	{
		m_pIt->advance(n);
	}

	difference_type distance_to(const any_iterator& rhs) const
	{
		return m_pIt->distance_to(*rhs.m_pIt);
	}

	boost::scoped_ptr<iterator_holder_interface> m_pIt;
};

} // namespace rns

#endif // RNS_ANY_ITERATOR_H_
