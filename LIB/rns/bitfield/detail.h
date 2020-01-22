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

#ifndef RNS_BITFIELD_DETAIL_H_
#define RNS_BITFIELD_DETAIL_H_

#include <boost/preprocessor.hpp>

namespace rns {
namespace bitfield_detail {

/**
 * @brief �����t���r�b�g�t�B�[���h�̒l�����o���B
 *
 * @tparam Arithmetic �Z�p�V�t�g���K�v��
 * @tparam ShiftBits �E�V�t�g��
 * @tparam MaskWidth �}�X�N�r�b�g��
 * @tparam T �}�X�N�p�̌^
 * @param[in,out] val �����t���̏ꍇ�A��ʃr�b�g�𖄂ߗ��ĂȂ���΂Ȃ�Ȃ��l�B
 *
 * @attention
 * ���̊֐��́u�E�V�t�g���Z���Z�p�V�t�g���Z�ƒ�`�v���Ă��鏈���n�ł̂ݐ���ɓ��삷��B<br>
 * �u�E�V�t�g���Z��_���V�t�g���Z�ƒ�`�v���Ă��鏈���n�ł��g�������ꍇ�A�Â����r�W�����̎�����p���邱�ƁB
 *
 * @note
 * �W���̃r�b�g�t�B�[���h�� shl sar ��2���߂ł��Ȃ��Ă��鏈�����A���̊֐��� shl movsx sar ��3���߂ŏI����iVC8�j�B<br>
 * boost::uint_t::fast �͂Ă����� unsigned int �ɂȂ���̂��Ǝv���Ă������A���̂Ƃ���� boost::uint_t::least �Ɠ����^�ɂȂ�炵���B
 */
template <bool Arithmetic, boost::uint8_t ShiftBits, boost::uint8_t MaskWidth, typename T>
inline void shift(T& val, typename boost::enable_if_c<Arithmetic>::type* = 0)
{
	typedef typename boost::uint_t<sizeof(T) * 8>::fast unsigned_type;

	const boost::uint8_t type_bits = sizeof(unsigned_type) * 8;
	const boost::uint8_t left_bits = type_bits - MaskWidth;
	
	unsigned_type uval = val; 
	val = uval << (left_bits - ShiftBits);
	val >>= left_bits;
}

/**
 * @brief �����Ȃ��r�b�g�t�B�[���h�̒l�����o���B
 *
 * @tparam Arithmetic �Z�p�V�t�g���K�v��
 * @tparam ShiftBits �E�V�t�g��
 * @tparam MaskWidth �}�X�N�r�b�g��
 * @tparam T �}�X�N�p�̌^
 * @param[in,out] val �����t���̏ꍇ�A��ʃr�b�g�𖄂ߗ��ĂȂ���΂Ȃ�Ȃ��l�B
 */
template <bool Arithmetic, boost::uint8_t ShiftBits, boost::uint8_t MaskWidth, typename T>
inline void shift(T& val, typename boost::disable_if_c<Arithmetic>::type* = 0)
{
	val = (val >> ShiftBits) & ((1 << MaskWidth) - 1);
}

/**
 * @brief bool �^�ւ̌^�ϊ����s���B
 */
template <typename R, typename T>
inline R cast(T rhs, typename boost::enable_if<boost::is_same<bool, R> >::type* = 0)
{
	return rhs != 0;
}

/**
 * @brief bool �^�ȊO�ւ̌^�ϊ����s���B
 */
template <typename R, typename T>
inline R cast(T rhs, typename boost::disable_if<boost::is_same<bool, R> >::type* = 0)
{
	return static_cast<R>(rhs);
}

/**
 * @brief �r�b�g�t�B�[���h�ɒl��������B
 *
 * @param[in,out] lhs �����
 * @param[in] rhs �������l
 * @param[in] mask �������l�̃}�X�N�l
 * @param[in] shift �������Ƃ��̍��V�t�g��
 */
template <typename T1, typename T2>
inline void assign(T1& lhs, T2 rhs, T2 mask, int shift)
{
	lhs =
		static_cast<T2>(
			(static_cast<T2>(lhs) & ~(mask << shift)) |
			((rhs & mask) << shift)
		);
}

/**
 * @brief ���Ԍ^����
 */
template <typename T>
struct medium_type_traits
{
	typedef T value_type; ///< ��ʓI�Ȓ��Ԍ^�̑g�ݍ��݌^�́A���Ԍ^���̂���
};

/**
 * @brief �G���f�B�A���w�蒆�Ԍ^����
 */
template <boost::integer::endianness E, typename T, std::size_t n_bits>
struct medium_type_traits<boost::integer::endian<E, T, n_bits> >
{
	typedef T value_type; ///< �G���f�B�A���w�蒆�Ԍ^�̑g�ݍ��݌^
};

/**
 * @brief �r�b�g�t�B�[���h���N���X
 *
 * @tparam BitWidth �r�b�g�t�B�[���h�r�b�g��
 * @tparam ShiftBits LSB����r�b�g�t�B�[���h�I�[�܂ł̃r�b�g��
 * @tparam BitfieldType �r�b�g�t�B�[���h�̌^
 * @tparam MediumType �o�C�g��o�b�t�@����g�ݍ��ݐ����^�ɕϊ����邽�߂̒��Ԍ^�i�����t���j
 * @tparam BufferType �r�b�g�t�B�[���h���i�[���Ă���o�C�g��̌^
 *
 * @sa http://www.boost-consulting.com/vault/index.php?direction=0&amp;order=&amp;directory=bitfield
 *
 * C����̃r�b�g�t�B�[���h�̓o�C�i���݊����ɖR�����B<br>
 * �Ȃ̂ŁA������͂����ƌ݊����̂���N���X�� Boost File Value ���玝���Ă��ĉ������Ă݂��B
 *
 * @note
 * ������2�̕␔�ŕ\������B
 *
 * @note
 * ���Ԍ^�𕄍��t���Ɍ��肵�Ă���̂́A���̕��������Ɠs�����悢����B<br>
 * ���̃R�[�h�ŕ��������̒��Ԍ^�������Ă��܂��ƁA get() �ŕs���������B
 */
template <
	boost::uint8_t BitWidth, boost::uint8_t ShiftBits,
	typename BitfieldType, typename MediumType, typename BufferType
>
class bitfield_base
{
	template <
		boost::uint8_t, boost::uint8_t,
		typename, typename, typename
	> friend class bitfield_base;

protected:
	typedef bitfield_base<BitWidth, ShiftBits, BitfieldType, MediumType, BufferType> bitfield_base_;

public:
	static const bool is_signed = std::numeric_limits<BitfieldType>::is_signed; ///< �r�b�g�t�B�[���h�̌^�͕����t����
	typedef typename medium_type_traits<MediumType>::value_type medium_value_type; ///< ���ԑg�ݍ��݌^
	static const medium_value_type value_mask = (1 << BitWidth) - 1; ///< �}�X�N�l

	BOOST_STATIC_ASSERT(BitWidth <= sizeof(BitfieldType) * 8); // �r�b�g���� BitfieldType �̃r�b�g���ȉ�
	BOOST_STATIC_ASSERT(BitWidth >= (is_signed ? 2 : 1)); // �����t���^�̃r�b�g����2�ȏ�A����ȊO��1�ȏ�
	BOOST_STATIC_ASSERT(std::numeric_limits<medium_value_type>::is_signed); // ���Ԍ^�͕����t��

	/**
	 * @brief �r�b�g�t�B�[���h���܂ރo�C�g���n���ăI�u�W�F�N�g���\�z����B
	 */
	bitfield_base(BufferType val) :
	m_p(val)
	{
	}

	/**
	 * @brief �ό^�I�u�W�F�N�g�����ɕs�ό^�I�u�W�F�N�g���\�z����B
	 */
	template <typename BufferType2>
	bitfield_base(
		const bitfield_base<
			BitWidth, ShiftBits,
			BitfieldType, MediumType, BufferType2
		>& rhs,
		typename boost::enable_if<
			boost::is_convertible<BufferType2, BufferType>
		>::type* = 0
	) :
	m_p(rhs.m_p)
	{
	}

	/**
	 * @brief �r�b�g�t�B�[���h�̒l�����o���B
	 */
	BitfieldType get() const
	{
		// �r�b�g�t�B�[���h�̒l�����o���A�g�ݍ��݌^�ɕϊ�
		medium_value_type val = *reinterpret_cast<const MediumType*>(m_p);

		// �r�b�g�t�B�[���h�̌^�������t���ŁA�r�b�g�t�B�[���h�̕����^�ƈ�v���Ȃ��ꍇ�A������MSB����1�Ŗ��߂Ȃ���΂Ȃ�Ȃ�
		bitfield_detail::shift<is_signed, ShiftBits, BitWidth>(val);

		// �^�ϊ����ĕԂ�
		return bitfield_detail::cast<BitfieldType>(val);
	}

	/**
	 * @copydoc get()
	 */
	operator BitfieldType () const
	{
		return get();
	}

	/**
	 * @brief �r�b�g�t�B�[���h�ɒl��������B
	 *
	 * @param[in] rhs �������l�B
	 */
	bitfield_base& operator=(BitfieldType rhs)
	{
		bitfield_detail::assign(
			*reinterpret_cast<MediumType*>(m_p),
			static_cast<medium_value_type>(rhs),
			value_mask,
			ShiftBits
		);

		return *this;
	}

private:
	BufferType m_p; ///< �ŏ��̃r�b�g���܂ރo�C�g���w���|�C���^
};

/**
 * @brief �r�b�O�G���f�B�A���ȃo�C�g��p�̃r�b�g�t�B�[���h�N���X
 *
 * @tparam FirstByte �r�b�g�t�B�[���h���n�܂�o�C�g�ԍ��i0�`�j
 * @tparam BitWidth �r�b�g�t�B�[���h�r�b�g��
 * @tparam ShiftBits LSB����r�b�g�t�B�[���h�I�[�܂ł̃r�b�g��
 * @tparam BitfieldType �r�b�g�t�B�[���h�̌^
 * @tparam MediumType �o�C�g��o�b�t�@����g�ݍ��ݐ����^�ɕϊ����邽�߂̒��Ԍ^�i�����t���j
 * @tparam BufferType �r�b�g�t�B�[���h���i�[���Ă���o�C�g��̌^
 */
template <
	boost::uint8_t FirstByte,
	boost::uint8_t BitWidth, boost::uint8_t ShiftBits,
	typename BitfieldType, typename MediumType, typename BufferType
>
class bitfield : public bitfield_base<BitWidth, ShiftBits, BitfieldType, MediumType, BufferType>
{
public:
	bitfield(BufferType val) :
	bitfield::bitfield_base_(val + FirstByte)
	{
	}

	template <typename BufferType2>
	bitfield(
		const bitfield<
			FirstByte, BitWidth, ShiftBits,
			BitfieldType, MediumType, BufferType2
		>& rhs,
		typename boost::enable_if<
			boost::is_convertible<BufferType2, BufferType>
		>::type* = 0
	) :
	bitfield::bitfield_base_(rhs)
	{
	}

	bitfield& operator=(BitfieldType rhs)
	{
		bitfield::bitfield_base_::operator=(rhs);
		return *this;
	}
};

/**
 * @brief �g�ݍ��݌^�p�̃r�b�g�t�B�[���h�N���X
 *
 * @tparam BitWidth �r�b�g�t�B�[���h�r�b�g��
 * @tparam ShiftBits LSB����r�b�g�t�B�[���h�I�[�܂ł̃r�b�g��
 * @tparam BitfieldType �r�b�g�t�B�[���h�̌^
 * @tparam NativeType �g�ݍ��݌^�icv�C�������j
 * @tparam BufferType �r�b�g�t�B�[���h���i�[���Ă���o�C�g��̌^
 */
template <
	boost::uint8_t BitWidth, boost::uint8_t ShiftBits,
	typename BitfieldType, typename NativeType, typename BufferType
>
class native_bitfield :
	public bitfield_base<
		BitWidth, ShiftBits,
		BitfieldType, typename boost::int_t<sizeof(NativeType) * 8>::least, BufferType
	>
{
public:
	/**
	 * @brief BufferType ���σo�C�g��ł���Ƃ��A�ϑg�ݍ��݌^�̒l���Q�Ǝ󂯂���B
	 *
	 * @note
	 * const NativeType& �͉E�Ӓl��n���ꂩ�˂Ȃ��̂Œ�`���Ȃ��B
	 */
	explicit native_bitfield(NativeType& val) :
	native_bitfield::bitfield_base_(reinterpret_cast<BufferType>(&val))
	{
	}

	/**
	 * @brief �g�ݍ��݌^�̒l���|�C���^�ŎQ�Ƃ���B
	 *
	 * @note
	 * �g�ݍ��݌^�� const �̏ꍇ�A��������g�킴������Ȃ��B
	 */
	explicit native_bitfield(
		typename boost::mpl::if_<
			boost::is_const<typename boost::remove_pointer<BufferType>::type>,
			const NativeType,
			NativeType
		>::type* val
	) :
	native_bitfield::bitfield_base_(reinterpret_cast<BufferType>(val))
	{
	}

	template <typename BufferType2>
	native_bitfield(
		const native_bitfield<
			BitWidth, ShiftBits,
			BitfieldType, NativeType, BufferType2
		>& rhs,
		typename boost::enable_if<
			boost::is_convertible<BufferType2, BufferType>
		>::type* = 0
	) :
	native_bitfield::bitfield_base_(rhs)
	{
	}

	native_bitfield& operator=(BitfieldType rhs)
	{
		native_bitfield::bitfield_base_::operator=(rhs);
		return *this;
	}
};

} // namespace bitfield_detail
} // namespace rns

#define RNS_MAKE_SEQ(size, rel) BOOST_PP_CAT(RNS_MAKE_SEQ_A_ ## size rel, 0X0)()
#define RNS_MAKE_SEQ_A_2(x, y) ((x, y)) RNS_MAKE_SEQ_B_2
#define RNS_MAKE_SEQ_B_2(x, y) ((x, y)) RNS_MAKE_SEQ_A_2
#define RNS_MAKE_SEQ_A_20X0()
#define RNS_MAKE_SEQ_B_20X0()

#define RNS_RESERVED_BITFIELD(null)

#define RNS_BFS_MEMBER_DEFINE(seq) \
	BOOST_PP_IF( \
		BOOST_PP_EQUAL(BOOST_PP_SEQ_SIZE(seq), 4), \
		RNS_MEMBER_BITFIELD, \
		BOOST_PP_ASSERT \
	) BOOST_PP_EXPAND(BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_REVERSE(seq)))

#define RNS_BFS_SEQ_(type, name) (name)(type)RNS_BFS_SEQ_end
#define RNS_BFS_SEQ_end(first, count) (BOOST_PP_ADD(first, BOOST_PP_DEC(count)))(first)
#define RNS_BFS_SEQ_reserved(first, count) (1)

#define RNS_BFS_SUM_BITS(i, seq) BOOST_PP_SEQ_FOLD_LEFT(RNS_BFS_ADD_BITS, 0, BOOST_PP_SEQ_FIRST_N(i, seq))
#define RNS_BFS_SUM_BITS_ZERO(i, seq) 0
#define RNS_BFS_ADD_BITS(s, state, x) BOOST_PP_ADD(state, BOOST_PP_TUPLE_ELEM(2, 0, x))

#define RNS_BFS_MEMBERS_DEFINE(r, data, i, e) \
	BOOST_PP_EXPAND( \
		RNS_BFS_MEMBER_DEFINE(\
			BOOST_PP_CAT(RNS_BFS_SEQ, BOOST_PP_TUPLE_ELEM(2, 1, e)) \
			(BOOST_PP_IF(i, RNS_BFS_SUM_BITS, RNS_BFS_SUM_BITS_ZERO)(i, data), BOOST_PP_TUPLE_ELEM(2, 0, e)) \
		)\
	)

#define RNS_BFS_BYTES_DEFINE(bits) \
	boost::uint8_t bytes[bits / 8]; \
	BOOST_STATIC_ASSERT(bits % 8 == 0);

#define RNS_BITFIELDS_STRUCT_D(c, seq) \
	struct c { \
		BOOST_PP_SEQ_FOR_EACH_I(RNS_BFS_MEMBERS_DEFINE, seq, seq) \
		RNS_BFS_BYTES_DEFINE(BOOST_PP_SEQ_FOLD_LEFT(RNS_BFS_ADD_BITS, 0, seq)) \
	};

#endif // RNS_BITFIELD_DETAIL_H_
