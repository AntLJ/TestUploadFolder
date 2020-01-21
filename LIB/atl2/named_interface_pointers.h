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

#ifndef ATL2_NAMED_INTERFACE_POINTERS_H_
#define ATL2_NAMED_INTERFACE_POINTERS_H_

#include <boost/mpl/list.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/static_assert.hpp>
#include <rns/mpl/name.h>
#include <rns/mpl/linearly_inheritance.h>
#include <atl2/type_traits/is_com_ptr_t.h>

namespace atl2 {

/**
 * @brief ���O�t���C���^�t�F�[�X�|�C���^�N���X
 *
 * �ÓI���̕�����ƃC���^�t�F�[�X�|�C���^�^��g�ݍ��킹�Ĉ�̌^�Ƃ���B
 *
 * @par �e���v���[�g����:
 * - @a Name - ���̕�����
 * - @a InterfacePtr - _com_ptr_t �^
 */
template <LPCTSTR Name, typename InterfacePtr>
class named_interface_pointer
{
public:
	BOOST_STATIC_ASSERT(atl2::is_com_ptr_t<InterfacePtr>::value);

	typedef typename InterfacePtr::Interface interface_type;

	LPCTSTR name() const { return Name; }

/// @name �X�}�[�g�|�C���^�Ƃ��Ă̋@�\
//@{
	InterfacePtr& get() { return val; }
	const InterfacePtr& get() const { return val; }

	interface_type& operator*() const { return *val; }
	interface_type* operator->() const { return val; }
//@}

/// @name �e��A���S���Y���p���Ȏ��ʊ֐�
//@{
	template <LPCTSTR T>
	InterfacePtr& get(
		typename boost::enable_if<
			boost::is_same<
				rns::mpl::name<T>,
				rns::mpl::name<Name>
			>
		>::type* = 0
	)
	{
		return val;
	}

	template <LPCTSTR T>
	const InterfacePtr& get(
		typename boost::enable_if<
			boost::is_same<
				rns::mpl::name<T>,
				rns::mpl::name<Name>
			>
		>::type* = 0
	) const
	{
		return val;
	}
//@}

private:
	InterfacePtr val;
};

namespace named_interface_pointers_detail { 

/**
 * @brief 2�� named_interface_pointer �p���N���X���p������N���X��Ԃ����^�֐�
 *
 * @par ����:
 * - @a Base - �ǂ��炩�Ƃ����ƃx�[�X�ƂȂ� named_interface_pointer �p���N���X
 * - @a T - �ǂ��炩�Ƃ����ƒǉ����� named_interface_pointer �p���N���X
 */
template <typename Base, typename T>
struct inherit
{
	struct type : Base, T
	{
		using Base::get;
		using T::get;
	};
};

/**
 * @brief ��1�����̒l�� boost::mpl::empty_base �ł���ꍇ�̓��ꉻ�� 
 *
 * @par ����:
 * - @a T - ���^�֐��߂�l
 */
template <typename T>
struct inherit<boost::mpl::empty_base, T>
{
	typedef T type;
};

} // namespace named_interface_pointers_detail

/**
 * @brief named_interface_pointer ���d�p���N���X
 * 
 * @par ����:
 * - @a List - named_interface_pointer �V�[�P���X
 */
template <typename Seq>
class named_interface_pointers :
	public rns::mpl::linearly_inheritance<
		Seq,
		named_interface_pointers_detail::inherit<
			boost::mpl::_,
			boost::mpl::_
		>
	>
{
};

} // namespace atl2

#endif // ATL2_NAMED_INTERFACE_POINTERS_H_
