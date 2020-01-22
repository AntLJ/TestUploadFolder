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
 * boost::format �⍲�֐���`�t�@�C��
 */

#ifndef RNS_FORMAT_H_
#define RNS_FORMAT_H_

#include <boost/format.hpp>

/**
 * @brief �z��^�����p�I�[�o�[���[�h
 *
 * @note
 * boost::basic_format ��%���Z�͕����z����󂯎���Ă���Ȃ��B<br>
 * SiNDY.Schema���C�u�������g�����Ƃ��ɖʓ|�Ȃ��ƂɂȂ�̂ŁA���̊֐��ŉ���B
 */
template <typename Ch, typename Tr, typename Alloc, typename T>
inline boost::basic_format<Ch, Tr, Alloc>
operator%(boost::basic_format<Ch, Tr, Alloc>& lhs, T* rhs)
{
	return lhs.operator%<T*>(rhs);
}

namespace rns {

#ifdef _TCHAR_DEFINED
typedef boost::basic_format<TCHAR> tformat;
#endif // _TCHAR_DEFINED

} // namespace rns

#endif // RNS_FORMAT_H_
