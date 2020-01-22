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

#ifndef RNS_PATCH_ENDIAN_H_
#define RNS_PATCH_ENDIAN_H_

// https://svn.boost.org/trac/boost/ticket/7516 �΍�
// 2013/6/3���_�ŐV�i1.53.0�j��boost/endian.hpp�͈ȉ��̗��R�ɂ��o�C�g�I�[�_�[��F���ł��Ȃ���������B
// �Eg++��endian.h���捞��ŃG���f�B�A���������ʂ��邱�Ƃ��ł��Ȃ�
// �E__ARMEL__�����g���G���f�B�A�����ƔF�����Ȃ�
// �����ŁAg++���ŃG���f�B�A���w�肪�Ȃ��ꍇ�Aendian.h���捞���boost/endian.hpp���F���ł���G���f�B�A���w����s���B
#if defined(__GNUC__) && ! defined(__LITTLE_ENDIAN__) && ! defined(__BIG_ENDIAN__)
# include <endian.h>
# if defined(__BYTE_ORDER__)
#  if (__BYTE_ORDER__ == __LITTLE_ENDIAN)
#   define __LITTLE_ENDIAN__
#  elif (__BYTE_ORDER__ == __BIG_ENDIAN)
#   define __BIG_ENDIAN__
#  endif // __BYTE_ORDER__ == __LITTLE/BIG_ENDIAN
# endif // defined(__BYTE_ORDER__)
// ����
// boost/endian.hpp�́u_LITTLE_ENDIAN����`����Ă���_BIG_ENDIAN����`����Ă��Ȃ��v��Ԃ����g���G���f�B�A�����ƔF����������ƂȂ��Ă��邪�A
// Android NDK��endian.h�́u_BIG_ENDIAN��_LITTLE_ENDIAN�̗������`����i__BIG_ENDIAN��__LITTLE_ENDIAN�Ɠ����l�ɂ���j�v���߁A������Ŕ��f�����邱�Ƃ͂ł��Ȃ��B
// ���̂��߁u__LITTLE_ENDIAN__����`����Ă���__BIG_ENDIAN__����`����Ă��Ȃ��v��ԂŔ��肳����悤�ɂ��Ă���B
#endif // defined(__GNUC__) && ! defined(__LITTLE_ENDIAN__) && ! defined(__BIG_ENDIAN__)

#endif // RNS_PATCH_ENDIAN_H_
