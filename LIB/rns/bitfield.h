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
 * @brief ビットフィールドライブラリ
 *
 * @attention
 * 使用するには http://www.boost-consulting.com/vault/index.php?direction=0&amp;order=&amp;directory=Integer が必要。
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

// オーバーフローを伴う符号付き→符合なし型変換（JIS X 3014:2003 4.7/3）が想定どおりであることを確認
BOOST_STATIC_ASSERT(static_cast<int>(0xffffffff) == static_cast<int>(-1));

// 右シフト演算が算術シフト（JIS X 3014:2003 5.8/3）であることを確認
BOOST_STATIC_ASSERT((static_cast<int>(-1) >> 1) == static_cast<int>(-1));

namespace rns {

/** 
 * @brief ビッグエンディアンなバイト列用のビットフィールドクラス生成支援クラス
 *
 * @tparam FirstBit 先頭ビット番号（MSBをゼロ番とする）
 * @tparam LastBit 終端ビット番号（MSBをゼロ番とする）
 * @tparam BitfieldType ビットフィールドの型
 * @tparam ByteType ビットフィールドを格納しているバイト列の可変バイト型（デフォルト : unsigned char）
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
		> variable; ///< 可変型ビットフィールド

	typedef
		bitfield_detail::bitfield<
			first_byte, bit_width, shift_bits,
			BitfieldType, medium_type, const ByteType*
		> constant; ///< 不変型ビットフィールド
};

/** 
 * @brief 組み込み型用のビットフィールドクラス生成支援クラス
 *
 * @tparam FirstBit 先頭ビット番号（MSBをゼロ番とする）
 * @tparam LastBit 終端ビット番号（MSBをゼロ番とする）
 * @tparam BitfieldType ビットフィールドとの値のやり取りに用いる型
 * @tparam NativeType ビットフィールドを含む組み込み型
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
		> variable; ///< 可変型ビットフィールド

	typedef
		bitfield_detail::native_bitfield<
			bit_width, shift_bits,
			BitfieldType, NativeType, const unsigned char*
		> constant; ///< 不変型ビットフィールド
};

} // namespace rns

/**
 * @brief バイト列を格納する変数 bytes が用意されているC互換構造体に、ビットフィールドへのアクセスを行う型と関数を追加するマクロ
 *
 * @param first 先頭ビット番号（MSBをゼロ番とする）
 * @param last 終端ビット番号（MSBをゼロ番とする）
 * @param type ビットフィールドの型
 * @param name 関数／型名。型名はこれに接尾辞 _t が付く
 *
 * @par 用例:
 * @code
 * struct BitFields {
 *     boost::uint8_t bytes[1];
 * // 以下2つは完全に等価
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
 * コンストラクタやデストラクタ、仮想関数を追加しなければ、C互換構造体にメンバ関数を追加しても依然C互換構造体なはず。
 */
#define RNS_MEMBER_BITFIELD(first, last, type, name) \
	typedef rns::bitfield_t<first, last, type> name ## _t; \
	name ## _t::constant name() const { return bytes; } \
	name ## _t::variable name() { return bytes; }

/**
 * @brief バイト列を格納する変数と、それをビットフィールドとしてアクセスする型と関数を持つC互換構造体を定義するマクロ
 *
 * @param name 構造体名
 * @param preseq
 * ビットフィールドをMSBから順に定義する<a href="http://www.boost.org/doc/libs/1_38_0/libs/preprocessor/doc/data/sequences.html">シーケンス</a>。<br>
 * 各要素の構文は以下の2種類。
 * - <b>ビット数, _(型, ビットフィールド名)</b>
 *   - RNS_MEMBER_BITFIELD(先行ビット数, 先行ビット数+ビット数-1, 型, ビットフィールド名) でビットフィールドを追加する
 *   - ビット数分、先行ビット数を増やす
 * - <b>ビット数, _reserved</b>
 *   - ビットフィールドは追加しない
 *   - ビット数分、先行ビット数を増やす
 *
 * @par 用例:
 * @code
 * // 以下2つはほぼ等価
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
 * 構造体のサイズはビット数の合計に等しい。
 *
 * @attention
 * ビット数の合計は8の倍数にならなければならない。<br>
 * そうなっていない場合はコンパイルエラーとなる。
 *
 * @note
 * @ref RNS_MEMBER_BITFIELD の方が見やすいし分かりやすいのではなかろうか……
 */
#define RNS_BITFIELDS_STRUCT(name, preseq) RNS_BITFIELDS_STRUCT_D(name, RNS_MAKE_SEQ(2, preseq))

#endif // RNS_BITFIELD_H_
