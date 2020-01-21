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
 * @file
 * @brief �����q����`����Ă���_com_ptr_t��Boost.Range�̃��f���Ƃ��Ē�`���郉�C�u�����B
 *
 * @note
 * Boost.RangeEx�̓����ɔ����쐬�B<br>
 * �����<atl2/algorithm.h>�͕s�v�ɂȂ�B
 *
 * @note
 * CComPtr�����f�������悤���Ǝv�������A�܂��g��Ȃ�����~�߁B<br>
 * ���̃C���^�t�F�[�X�|�C���^�����f�����������Ƃ��낾���A���̈ʒu�œ��ꉻ����̂͌���d�l�㖳�����ۂ��̂Ŏ~�߁B
 *
 * @note
 * ���ł�<boost-vault/boost/range/atl.hpp>���C���N���[�h���Ă��܂������B
 */
#ifndef ATL2_RANGE_H_
#define ATL2_RANGE_H_

#include <boost/range/functions.hpp>
#include <atl2/iterator/iterator_base.h>

#define ATL2_TEMPLATE_RANGE_TRAITS(c)  \
	template <typename T> struct range_iterator<c<T> > { typedef typename atl2::iterator_of<c<T> >::type type; }; \
	template <typename T> struct range_const_iterator<c<T> > { typedef typename atl2::iterator_of<c<T> >::type type; }; \
	template <typename T> struct range_reverse_iterator<c<T> > { typedef typename atl2::reverse_iterator_of<c<T> >::type type; }; \
	template <typename T> struct range_const_reverse_iterator<c<T> > { typedef typename atl2::reverse_iterator_of<c<T> >::type type; }; \
	template <typename T> struct range_size<c<T> > { typedef typename std::size_t type; };
	
#define ATL2_TEMPLATE_RANGE_FUNCTION(c, i, f) \
	template <typename T> inline typename atl2::container_traits<c<T> >::i f(c<T>& r) { return atl2::f(r); } \
	template <typename T> inline typename atl2::container_traits<c<T> >::i f(const c<T>& r) { return atl2::f(r); }

namespace boost {

ATL2_TEMPLATE_RANGE_TRAITS(_com_ptr_t)
ATL2_TEMPLATE_RANGE_TRAITS(const _com_ptr_t)
ATL2_TEMPLATE_RANGE_FUNCTION(_com_ptr_t, iterator, begin)
ATL2_TEMPLATE_RANGE_FUNCTION(_com_ptr_t, iterator, end)
ATL2_TEMPLATE_RANGE_FUNCTION(_com_ptr_t, reverse_iterator, rbegin)
ATL2_TEMPLATE_RANGE_FUNCTION(_com_ptr_t, reverse_iterator, rend)

template <typename T>
inline std::size_t size(_com_ptr_t<T>& r)
{
	return std::distance(atl2::begin(r), atl2::end(r));
}

template <typename T>
inline std::size_t size(const _com_ptr_t<T>& r)
{
	return std::distance(atl2::begin(r), atl2::end(r));
}

} // namespace boost

#undef ATL2_TEMPLATE_RANGE_FUNCTION
#undef ATL2_TEMPLATE_RANGE_TRAITS

#endif // ATL2_RANGE_H_
