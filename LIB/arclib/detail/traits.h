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

#ifndef __SINDY_DETAIL_TRAITS_H__
#define __SINDY_DETAIL_TRAITS_H__

#include "ref.h"

namespace sindy {

/**
 * @defgroup traits
 */
/*@{*/

/**
 * @brief ������`�N���X�e���v���[�g
 *
 * SiNDY�n�̃N���X�E�C���^�t�F�[�X�̓������`����N���X�̐��^�B<br>
 * �ΏۂƂȂ�N���X�E�C���^�t�F�[�X�̃|�C���^�œ��ꉻ����B<br>
 * �|�C���^�œ��ꉻ����̂́AVC�ł̓C���^�t�F�[�X�|�C���^�^�̃e���v���[�g�����i _IRow* �Ȃǁj����|�C���^���O���̂Ɏ�Ԃ��|���邩��B
 */
template <typename T>
struct pointer_traits {};

namespace detail {

template <int T> struct traits_selector { template <typename T> struct traits_of { typedef pointer_traits<T*> type; };  };
template <> struct traits_selector<1> { template <typename T> struct traits_of { typedef pointer_traits<T> type; }; };
template <> struct traits_selector<2> { template <typename T> struct traits_of { typedef pointer_traits<T> type; }; };
template <> struct traits_selector<3> { template <typename T> struct traits_of { typedef pointer_traits<typename T::Interface*> type; }; };
template <> struct traits_selector<4> { template <typename T> struct traits_of { typedef pointer_traits<typename T::type*> type; }; };

} // namespace detail

/**
 * @brief �������o���e���v���[�g
 *
 * �l�X�Ȍ^�킩��A�R��ׂ� pointer_traits �𓱂��o�����߂̃e���v���[�g�N���X�B<br>
 * @a T �̌^��ɂ���āA�ȉ��̂悤�ɉ�����B
 * - ��|�C���^ : pointer_traits<T*>
 * - �|�C���^ : pointer_traits<T>
 * - _com_ptr_t : pointer_traits<T::Interface*>
 * - reference_wrapper : pointer_traits<T::type*>
 */
template <typename T> struct traits : public detail::traits_selector<type_of<T>::value>::traits_of<T>::type {};

/*@}*/

/**
 * @defgroup container_traits
 */
/*@{*/

namespace detail {

template <int T> struct container_traits_selector { template <typename T> struct traits_of { struct type {}; }; };
template <> struct container_traits_selector<0>
{
	template <typename T>
	struct traits_of
	{
		struct type
		{
			typedef typename T::ElementTraits ElementTraits;
			typedef typename ElementTraits::Element Element;
			typedef typename ElementTraits::ElementHolder ElementHolder;
			typedef typename T::iterator iterator;

			/// @note �e���v���[�g�����Ȃ���C1001�������B
			template <typename T2> static typename T::iterator make_first(T2& rT2) { return T::make_first(rT2); }
		};
	};
};
template <> struct container_traits_selector<2> { template <typename T> struct traits_of { typedef traits<T> type; }; };
template <> struct container_traits_selector<3> { template <typename T> struct traits_of { typedef traits<T> type; }; };
template <> struct container_traits_selector<4> { template <typename T> struct traits_of { typedef typename container_traits_selector<0>::traits_of<typename T::type>::type type; }; };

} // namespace detail

/**
 * @brief �R���e�i�������o���e���v���[�g
 *
 * @a T ���L���ȃR���e�i�ł���Ȃ�΁A�Œ���ȉ��̌^�Ɗ֐�����`�����B
 * - �^
 *   - ElementTraits : �v�f�����B
 *   - ElementHolder : �v�f�ێ��N���X�B�R���X�g���N�^�v�f���i�[���A�f�X�g���N�^�ŉ������B
 *   - iterator : �����q�N���X�B
 * - �֐�
 *   - static iterator make_first(T&) : @a T �̎n�_�����q���擾����֐��B
 */
template <typename T>
struct container_traits : public detail::container_traits_selector<type_of<T>::value>::traits_of<T>::type {};

/*@}*/

} // namespace sindy

#endif // __SINDY_DETAIL_TRAITS_H__
