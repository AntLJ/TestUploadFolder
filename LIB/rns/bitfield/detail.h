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
 * @brief 符号付きビットフィールドの値を取り出す。
 *
 * @tparam Arithmetic 算術シフトが必要か
 * @tparam ShiftBits 右シフト数
 * @tparam MaskWidth マスクビット幅
 * @tparam T マスク用の型
 * @param[in,out] val 符号付きの場合、上位ビットを埋め立てなければならない値。
 *
 * @attention
 * この関数は「右シフト演算を算術シフト演算と定義」している処理系でのみ正常に動作する。<br>
 * 「右シフト演算を論理シフト演算と定義」している処理系でも使いたい場合、古いリビジョンの実装を用いること。
 *
 * @note
 * 標準のビットフィールドが shl sar の2命令でこなしている処理を、この関数は shl movsx sar の3命令で終える（VC8）。<br>
 * boost::uint_t::fast はてっきり unsigned int になるものだと思っていたが、今のところは boost::uint_t::least と同じ型になるらしい。
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
 * @brief 符号なしビットフィールドの値を取り出す。
 *
 * @tparam Arithmetic 算術シフトが必要か
 * @tparam ShiftBits 右シフト数
 * @tparam MaskWidth マスクビット幅
 * @tparam T マスク用の型
 * @param[in,out] val 符号付きの場合、上位ビットを埋め立てなければならない値。
 */
template <bool Arithmetic, boost::uint8_t ShiftBits, boost::uint8_t MaskWidth, typename T>
inline void shift(T& val, typename boost::disable_if_c<Arithmetic>::type* = 0)
{
	val = (val >> ShiftBits) & ((1 << MaskWidth) - 1);
}

/**
 * @brief bool 型への型変換を行う。
 */
template <typename R, typename T>
inline R cast(T rhs, typename boost::enable_if<boost::is_same<bool, R> >::type* = 0)
{
	return rhs != 0;
}

/**
 * @brief bool 型以外への型変換を行う。
 */
template <typename R, typename T>
inline R cast(T rhs, typename boost::disable_if<boost::is_same<bool, R> >::type* = 0)
{
	return static_cast<R>(rhs);
}

/**
 * @brief ビットフィールドに値を代入する。
 *
 * @param[in,out] lhs 代入先
 * @param[in] rhs 代入する値
 * @param[in] mask 代入する値のマスク値
 * @param[in] shift 代入するときの左シフト数
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
 * @brief 中間型特性
 */
template <typename T>
struct medium_type_traits
{
	typedef T value_type; ///< 一般的な中間型の組み込み型は、中間型そのもの
};

/**
 * @brief エンディアン指定中間型特性
 */
template <boost::integer::endianness E, typename T, std::size_t n_bits>
struct medium_type_traits<boost::integer::endian<E, T, n_bits> >
{
	typedef T value_type; ///< エンディアン指定中間型の組み込み型
};

/**
 * @brief ビットフィールド基底クラス
 *
 * @tparam BitWidth ビットフィールドビット幅
 * @tparam ShiftBits LSBからビットフィールド終端までのビット数
 * @tparam BitfieldType ビットフィールドの型
 * @tparam MediumType バイト列バッファから組み込み整数型に変換するための中間型（符号付き）
 * @tparam BufferType ビットフィールドを格納しているバイト列の型
 *
 * @sa http://www.boost-consulting.com/vault/index.php?direction=0&amp;order=&amp;directory=bitfield
 *
 * C言語のビットフィールドはバイナリ互換性に乏しい。<br>
 * なので、それよりはずっと互換性のあるクラスを Boost File Value から持ってきて改造してみた。
 *
 * @note
 * 負数は2の補数で表現する。
 *
 * @note
 * 中間型を符号付きに限定しているのは、その方が何かと都合がよいから。<br>
 * このコードで符号無しの中間型を許してしまうと、 get() で不具合が生じる。
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
	static const bool is_signed = std::numeric_limits<BitfieldType>::is_signed; ///< ビットフィールドの型は符号付きか
	typedef typename medium_type_traits<MediumType>::value_type medium_value_type; ///< 中間組み込み型
	static const medium_value_type value_mask = (1 << BitWidth) - 1; ///< マスク値

	BOOST_STATIC_ASSERT(BitWidth <= sizeof(BitfieldType) * 8); // ビット幅は BitfieldType のビット幅以下
	BOOST_STATIC_ASSERT(BitWidth >= (is_signed ? 2 : 1)); // 符号付き型のビット幅は2以上、それ以外は1以上
	BOOST_STATIC_ASSERT(std::numeric_limits<medium_value_type>::is_signed); // 中間型は符号付き

	/**
	 * @brief ビットフィールドを含むバイト列を渡してオブジェクトを構築する。
	 */
	bitfield_base(BufferType val) :
	m_p(val)
	{
	}

	/**
	 * @brief 可変型オブジェクトを元に不変型オブジェクトを構築する。
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
	 * @brief ビットフィールドの値を取り出す。
	 */
	BitfieldType get() const
	{
		// ビットフィールドの値を取り出し、組み込み型に変換
		medium_value_type val = *reinterpret_cast<const MediumType*>(m_p);

		// ビットフィールドの型が符号付きで、ビットフィールドの幅が型と一致しない場合、負数はMSB側を1で埋めなければならない
		bitfield_detail::shift<is_signed, ShiftBits, BitWidth>(val);

		// 型変換して返す
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
	 * @brief ビットフィールドに値を代入する。
	 *
	 * @param[in] rhs 代入する値。
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
	BufferType m_p; ///< 最初のビットを含むバイトを指すポインタ
};

/**
 * @brief ビッグエンディアンなバイト列用のビットフィールドクラス
 *
 * @tparam FirstByte ビットフィールドが始まるバイト番号（0～）
 * @tparam BitWidth ビットフィールドビット幅
 * @tparam ShiftBits LSBからビットフィールド終端までのビット数
 * @tparam BitfieldType ビットフィールドの型
 * @tparam MediumType バイト列バッファから組み込み整数型に変換するための中間型（符号付き）
 * @tparam BufferType ビットフィールドを格納しているバイト列の型
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
 * @brief 組み込み型用のビットフィールドクラス
 *
 * @tparam BitWidth ビットフィールドビット幅
 * @tparam ShiftBits LSBからビットフィールド終端までのビット数
 * @tparam BitfieldType ビットフィールドの型
 * @tparam NativeType 組み込み型（cv修飾無し）
 * @tparam BufferType ビットフィールドを格納しているバイト列の型
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
	 * @brief BufferType が可変バイト列であるとき、可変組み込み型の値を参照受けする。
	 *
	 * @note
	 * const NativeType& は右辺値を渡されかねないので定義しない。
	 */
	explicit native_bitfield(NativeType& val) :
	native_bitfield::bitfield_base_(reinterpret_cast<BufferType>(&val))
	{
	}

	/**
	 * @brief 組み込み型の値をポインタで参照する。
	 *
	 * @note
	 * 組み込み型が const の場合、こちらを使わざるをえない。
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
