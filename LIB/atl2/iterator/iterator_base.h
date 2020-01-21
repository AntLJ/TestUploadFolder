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
 * @file iterator_base.h
 */
#ifndef ATL2_ITERATOR_BASE_H_
#define ATL2_ITERATOR_BASE_H_

#include <atl2/exception.h>
#include <atl2/type_traits.h>

namespace atl2 {

/**
 * @defgroup container_traits_group �R���e�i������`
 *
 * @ref atl2::container_pointer_traits ����ꉻ���A�R���e�i�̓������`�����N���X�B<br>
 * @ref iterator_helper_function�������q�𐶐����邽�߂̎肪����Ƃ���B
 */
/*@{*/

/**
 * @brief �R���e�i�C���^�t�F�[�X�|�C���^�̓����e���v���[�g�N���X�B
 *
 * @ref container_traits_group �́A���̃N���X����ꉻ���čs���B
 * 
 * @par �^:
 * - <b>iterator</b> : �R���e�i�𑀍삷�锽���q�B
 *
 * @par �֐�:
 * - <b>static iterator begin(T)</b> : �n�_�����q��Ԃ��֐��B atl2::begin(TContainer const&) �̎����ɕK�v�B
 * - <b>static iterator end(T)</b> : �I�_�����q��Ԃ��֐��B atl2::end(TContainer const&) �̎����ɕK�v�B
 * - <b>static iterator static_end()</b> : ��ΏI�_�����q��Ԃ��֐��B arctl::end() �̎����ɕK�v�B
 */
template <typename T>
struct container_pointer_traits {};

/*@}*/

/**
 * @brief �R���e�i�C���^�t�F�[�X�̓����N���X�B
 *
 * ���̃N���X�� arctl::container_pointer_traits �ւ̋��n�����s���B
 */
template <typename T> struct container_traits : public container_pointer_traits<typename atl2::interface_pointer_of<T>::type> {};

/// �����q�𓯒肷��N���X�B
template <typename T> struct iterator_of { typedef typename container_traits<T>::iterator type; };

/// �t�s�����q�𓯒肷��N���X�B
template <typename T> struct reverse_iterator_of { typedef typename container_traits<T>::reverse_iterator type; };

} // namespace atl2

#if _MSC_VER >= 1400
/**
 * @brief ADL ���p���O���
 *
 * @note
 * Boost.Range �� adl_begin() �� adl_end() ���猩����Ȃ��悤�ɂ���B
 *
 * @note
 * VC6 �� ADL �͉��Z�q�ȊO�ɂ͓����Ȃ��̂ŁA�������Ƃ�K�v�͂Ȃ��B<br>
 * �Ƃ������A�Ƃ�ƃR���p�C���ł��Ȃ��Ȃ����肷��B
 */
namespace atl2_iterator_unspecified {
#else
namespace atl2 {
#endif // _MSC_VER >= 1400

/**
 * @defgroup iterator_helper_function �����q�����֐�
 *
 * @ref atl2::container_traits "container_traits" ��p���āA�n���ꂽ�R���e�i�𑖍����锽���q�𐶐�����֐��Q�B
 *
 * @note
 * begin(TContainer const&) �� end(TContainer const&) ��������const�Q�ƂŎ󂯎���Ă���̂́A
 * �X�}�[�g�|�C���^�̎Q�ƃJ�E���g�����}�����邱�Ƃ����A
 * �ꎞ�I�u�W�F�N�g�̎����������L�΂��Ӗ��������傫���B<br>
 * @sa <a href="http://docs.sun.com/source/806-6491/lionel.html#669">�ꎞ�I�u�W�F�N�g�̎���</a>
 *
 * @par �p��:
 * @code
 * void foo(IRow* ipRow);
 *
 * void bar(ITable* ipTable)
 * {
 *   std::for_each(arctl::begin(ipTable->_Search(0, VARIANT_FALSE)), arctl::end<ICursor>(), foo);
 * @endcode
 */
/*@{*/

/**
 * @brief �n�[�����q�𐶐�����B
 *
 * @param rContainer [in,ref] �R���e�i�B
 * @return �����q�B
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type begin(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::begin(rContainer);
}

/**
 * @brief �I�[�����q�𐶐�����B
 *
 * @param rContainer [in,ref] �R���e�i�B
 * @return �����q�B
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type end(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::end(rContainer);
}

/**
 * @brief �I�[���w�����������q�𐶐�����B
 *
 * @return �����q�B
 */
template <typename TContainer>
typename atl2::iterator_of<TContainer>::type end()
{
	return atl2::container_traits<TContainer>::static_end();
}

/**
 * @brief �t�s�n�[�����q�𐶐�����B
 *
 * @param rContainer [in,ref] �R���e�i�B
 * @return �t�s�����q�B
 */
template <typename TContainer>
typename atl2::reverse_iterator_of<TContainer>::type rbegin(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::rbegin(rContainer);
}

/**
 * @brief �t�s�I�[�����q�𐶐�����B
 *
 * @param rContainer [in,ref] �R���e�i�B
 * @return �t�s�����q�B
 */
template <typename TContainer>
typename atl2::reverse_iterator_of<TContainer>::type rend(TContainer const& rContainer)
{
	return atl2::container_traits<TContainer>::rend(rContainer);
}

/*@}*/

#if _MSC_VER >= 1400
} // namespace atl2_iterator_unspecified 

namespace atl2 {
	using namespace atl2_iterator_unspecified;
} // namespace atl2
#else
} // namespace atl2
#endif // _MSC_VER >= 1400

#endif // ATL2_ITERATOR_BASE_H_
