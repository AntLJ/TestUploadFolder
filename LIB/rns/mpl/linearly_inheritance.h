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

#ifndef RNS_MPL_LINEARLY_INHERITANCE_H_
#define RNS_MPL_LINEARLY_INHERITANCE_H_

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_pointer.hpp>

namespace rns {

namespace mpl {

/**
 * @brief �g���^����p���N���X
 *
 * ���^�֐� inherit_linearly �̉��ɁA�A���S���Y���p�̑�����ǉ������N���X�B
 */
template <typename Seq, typename Node, typename Root = boost::mpl::empty_base>
struct linearly_inheritance : public boost::mpl::inherit_linearly<Seq, Node, Root>::type
{
	typedef Seq inheritance_type;
};

namespace detail {

template <typename List, typename Function>
class apply_each;

/**
 * @brief rns::mpl::linearly_inheritance �Ōp�������X�̌^�̎��̂ɑ΂��ČʂɊ֐� f() ��K�p����B
 *
 * @param[in] l rns::mpl::linearly_inheritance �I�u�W�F�N�g�B
 * @param[in] f �P���֐��B rns::mpl::for_each() �̎��̂��Q�ƁB
 */
template <typename List, typename Function>
inline void for_each_impl(List& l, Function& f)
{
	using namespace boost::mpl;

	// �d���R���X�g���N�^�^�f�X�g���N�^�����^���p�����Ă���\��������ȏ�A
	// add_pointer �Ń|�C���^�^�ɕϊ�������������B
	for_each<typename List::inheritance_type, boost::add_pointer<_>	>(
		apply_each<List, Function>(l, f)
	);
}

/**
 * @brief boost::mpl::for_each() �p�֐��K�p�֐��N���X
 */
template <typename List, typename Function>
class apply_each
{
public:
	/**
	 * @param[in] list �֐��K�p�ΏہB
	 * @param[in] func �P���֐��B
	 */
	apply_each(List& list, Function& func) :
	m_list(list),
	m_func(func)
	{
	}

	/**
	 * @brief �^ @e Item �� rns::mpl::linearly_inheritance �ł������ꍇ�A�A�b�v�L���X�g�����̂� rns::mpl::for_each() ���J�n�B
	 */
	template <typename Item>
	void operator()(Item*, typename Item::inheritance_type* = 0) const
	{
		for_each_impl(get<Item>(), m_func);
	}

	/**
	 * @brief �^ @e Item �ɃA�b�v�L���X�g���Ċ֐���K�p����B
	 */
	template <typename Item>
	void operator()(Item*, ...) const
	{
		m_func(get<Item>());
	}

private:
	/**
	 * @brief @e List �� const �Ȃ� const @e Item �ɁA const �łȂ���� @e Item �ɃA�b�v�L���X�g����B
	 */
	template <typename Item>
	typename boost::mpl::if_<boost::is_const<List>, boost::add_const<Item>, Item>::type& get() const
	{
		return m_list;
	};
		
	List& m_list;
	Function& m_func;
};

} // namespace detail

/**
 * @brief rns::mpl::linearly_inheritance �Ōp�������X�̌^�̎��̂ɑ΂��ČʂɊ֐� f() ��K�p����B
 *
 * @param[in] l rns::mpl::linearly_inheritance �I�u�W�F�N�g�B
 * @param[in] f �P���֐��B
 * @return f
 *
 * ��1������ inheritance_type ������`����Ă���� rns::mpl::linearly_inheritance �ł���K�v�͂Ȃ��B<br>
 * �K�v������΁u�p�����Ă���C�ӂ̌^���ꂼ��Ɋ֐� f() ��K�p����v�����ɂ��邱�Ƃ��\�B
 *
 * rns::mpl::linearly_inheritance �� rns::mpl::linearly_inheritance ���p�����Ă���ꍇ�A��҂��p������X�̌^�̎��̂ɑ΂��Ċ֐� f() ���K�p�����B<br>
 * rns::mpl::linearly_inheritance ���̂Ɋ֐� f() ��K�p���邱�Ƃ͂ł��Ȃ��B
 *
 * @note
 * rns �� rns::mpl �ǂ���̖��O��Ԃɒu���̂��Ó����낤�B<br>
 * ���̊֐��� std::for_each �� boost::mpl::for_each �̂ǂ���ɋ߂��H
 *
 * @note
 * ��ԋ߂��̂́ABoost 1.35�Œǉ������\��� boost::fusion::for_each �B<br>
 * �����ċA�̗L���������΁A��낤�Ƃ��Ă��邱�Ƃ͑S���ꏏ�B
 */
template <typename List, typename Function>
inline Function for_each(List& l, Function f, typename List::inheritance_type* = 0)
{
	mpl::detail::for_each_impl(l, f);
	return f;
}

} // namespace mpl

} // namespace rns

#endif // RNS_MPL_LINEARLY_INHERITANCE_H_
