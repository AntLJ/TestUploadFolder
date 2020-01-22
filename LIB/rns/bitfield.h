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

#ifndef RNS_BITFIELD_H_
#define RNS_BITFIELD_H_

/**
 * @file
 * @brief �r�b�g�t�B�[���h���C�u����
 *
 * @attention
 * �g�p����ɂ� http://www.boost-consulting.com/vault/index.php?direction=0&amp;order=&amp;directory=Integer ���K�v�B
 */
#include <iostream>
#include <limits>
#include "./patch/endian.h" // https://svn.boost.org/trac/boost/ticket/7516
#include <boost/static_assert.hpp>
#include <boost/cstdint.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/if.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer.hpp>
#include <boost/integer/endian.hpp>
#include "./bitfield/detail.h"

// �I�[�o�[�t���[�𔺂������t���������Ȃ��^�ϊ��iJIS X 3014:2003 4.7/3�j���z��ǂ���ł��邱�Ƃ��m�F
BOOST_STATIC_ASSERT(static_cast<int>(0xffffffff) == static_cast<int>(-1));

// �E�V�t�g���Z���Z�p�V�t�g�iJIS X 3014:2003 5.8/3�j�ł��邱�Ƃ��m�F
BOOST_STATIC_ASSERT((static_cast<int>(-1) >> 1) == static_cast<int>(-1));

namespace rns {

/** 
 * @brief �r�b�O�G���f�B�A���ȃo�C�g��p�̃r�b�g�t�B�[���h�N���X�����x���N���X
 *
 * @tparam FirstBit �擪�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @tparam LastBit �I�[�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @tparam BitfieldType �r�b�g�t�B�[���h�̌^
 * @tparam ByteType �r�b�g�t�B�[���h���i�[���Ă���o�C�g��̉σo�C�g�^�i�f�t�H���g : unsigned char�j
 */
template <
	boost::uint8_t FirstBit, boost::uint8_t LastBit,
	typename BitfieldType, typename ByteType = unsigned char
>
struct bitfield_t
{
	BOOST_STATIC_ASSERT(FirstBit <= LastBit);

	static const boost::uint8_t bit_width = LastBit - FirstBit + 1;
	static const boost::uint8_t shift_bits = 7 - LastBit % 8;

	static const boost::uint8_t first_byte = FirstBit / 8;
	static const boost::uint8_t last_byte = LastBit / 8;
	static const boost::uint8_t byte_width = last_byte - first_byte + 1;
	
	typedef
		typename boost::int_t<
			((byte_width > sizeof(BitfieldType))
			? byte_width
			: sizeof(BitfieldType)) * 8
		>::fast medium_value_type;

	typedef boost::integer::endian<boost::integer::big, medium_value_type, byte_width * 8> medium_type;

	typedef
		bitfield_detail::bitfield<
			first_byte, bit_width, shift_bits,
			BitfieldType, medium_type, ByteType*
		> variable; ///< �ό^�r�b�g�t�B�[���h

	typedef
		bitfield_detail::bitfield<
			first_byte, bit_width, shift_bits,
			BitfieldType, medium_type, const ByteType*
		> constant; ///< �s�ό^�r�b�g�t�B�[���h
};

/** 
 * @brief �g�ݍ��݌^�p�̃r�b�g�t�B�[���h�N���X�����x���N���X
 *
 * @tparam FirstBit �擪�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @tparam LastBit �I�[�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @tparam BitfieldType �r�b�g�t�B�[���h�Ƃ̒l�̂����ɗp����^
 * @tparam NativeType �r�b�g�t�B�[���h���܂ޑg�ݍ��݌^
 */
template <
	boost::uint8_t FirstBit, boost::uint8_t LastBit,
	typename BitfieldType, typename NativeType
>
struct native_bitfield_t
{
	BOOST_STATIC_ASSERT(FirstBit <= LastBit);

	static const boost::uint8_t bit_width = LastBit - FirstBit + 1;
	static const boost::uint8_t shift_bits = sizeof(NativeType) * 8 - 1 - LastBit % (sizeof(NativeType) * 8);

	typedef
		bitfield_detail::native_bitfield<
			bit_width, shift_bits,
			BitfieldType, NativeType, unsigned char*
		> variable; ///< �ό^�r�b�g�t�B�[���h

	typedef
		bitfield_detail::native_bitfield<
			bit_width, shift_bits,
			BitfieldType, NativeType, const unsigned char*
		> constant; ///< �s�ό^�r�b�g�t�B�[���h
};

} // namespace rns

/**
 * @brief �o�C�g����i�[����ϐ� bytes ���p�ӂ���Ă���C�݊��\���̂ɁA�r�b�g�t�B�[���h�ւ̃A�N�Z�X���s���^�Ɗ֐���ǉ�����}�N��
 *
 * @param first �擪�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @param last �I�[�r�b�g�ԍ��iMSB���[���ԂƂ���j
 * @param type �r�b�g�t�B�[���h�̌^
 * @param name �֐��^�^���B�^���͂���ɐڔ��� _t ���t��
 *
 * @par �p��:
 * @code
 * struct BitFields {
 *     boost::uint8_t bytes[1];
 * // �ȉ�2�͊��S�ɓ���
 * #ifdef RNS_MEMBER_BITFIELD
 *     RNS_MEMBER_BITFIELD(0, 0, bool, field1);
 *     RNS_MEMBER_BITFIELD(2, 7, unsignd int, field2);
 * #else
 *     typedef rns::bitfield_t<0, 0, bool> field1_t;
 *     typedef rns::bitfield_t<2, 7, unsigned int> field2_t;
 *
 *     field1_t::constant field1() const { return bytes; };
 *     field1_t::variable field1() { return bytes; };
 *     field2_t::constant field2() const { return bytes; };
 *     field2_t::variable field2() { return bytes; };
 * #endif
 * };
 * @endcode
 * @note
 * �R���X�g���N�^��f�X�g���N�^�A���z�֐���ǉ����Ȃ���΁AC�݊��\���̂Ƀ����o�֐���ǉ����Ă��ˑRC�݊��\���̂Ȃ͂��B
 */
#define RNS_MEMBER_BITFIELD(first, last, type, name) \
	typedef rns::bitfield_t<first, last, type> name ## _t; \
	name ## _t::constant name() const { return bytes; } \
	name ## _t::variable name() { return bytes; }

/**
 * @brief �o�C�g����i�[����ϐ��ƁA������r�b�g�t�B�[���h�Ƃ��ăA�N�Z�X����^�Ɗ֐�������C�݊��\���̂��`����}�N��
 *
 * @param name �\���̖�
 * @param preseq
 * �r�b�g�t�B�[���h��MSB���珇�ɒ�`����<a href="http://www.boost.org/doc/libs/1_38_0/libs/preprocessor/doc/data/sequences.html">�V�[�P���X</a>�B<br>
 * �e�v�f�̍\���͈ȉ���2��ށB
 * - <b>�r�b�g��, _(�^, �r�b�g�t�B�[���h��)</b>
 *   - RNS_MEMBER_BITFIELD(��s�r�b�g��, ��s�r�b�g��+�r�b�g��-1, �^, �r�b�g�t�B�[���h��) �Ńr�b�g�t�B�[���h��ǉ�����
 *   - �r�b�g�����A��s�r�b�g���𑝂₷
 * - <b>�r�b�g��, _reserved</b>
 *   - �r�b�g�t�B�[���h�͒ǉ����Ȃ�
 *   - �r�b�g�����A��s�r�b�g���𑝂₷
 *
 * @par �p��:
 * @code
 * // �ȉ�2�͂قړ���
 * #ifdef RNS_BITFIELDS_STRUCT
 * RNS_BITFIELDS_STRUCT(
 *     BitFields,
 *     (1, _(bool, field1))
 *     (1, _reserved)
 *     (6, _(unsigned int, field2))
 * );
 * #else
 * struct BitFields {
 *     RNS_MEMBER_BITFIELD(0, 0, bool, field1);
 *     RNS_MEMBER_BITFIELD(2, 7, unsigned int, field2);
 *     boost::uint8_t bytes[1];
 * };
 * #endif
 * @endcode
 *
 * �\���̂̃T�C�Y�̓r�b�g���̍��v�ɓ������B
 *
 * @attention
 * �r�b�g���̍��v��8�̔{���ɂȂ�Ȃ���΂Ȃ�Ȃ��B<br>
 * �����Ȃ��Ă��Ȃ��ꍇ�̓R���p�C���G���[�ƂȂ�B
 *
 * @note
 * @ref RNS_MEMBER_BITFIELD �̕������₷����������₷���̂ł͂Ȃ��낤���c�c
 */
#define RNS_BITFIELDS_STRUCT(name, preseq) RNS_BITFIELDS_STRUCT_D(name, RNS_MAKE_SEQ(2, preseq))

#endif // RNS_BITFIELD_H_
