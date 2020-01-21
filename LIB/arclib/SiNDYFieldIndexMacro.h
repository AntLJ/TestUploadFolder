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

#ifndef __SINDY_FIELD_INDEX_MACRO_H__
#define __SINDY_FIELD_INDEX_MACRO_H__

#include <sstream>
#include "SiNDYInheritMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELDINDEX_CLASS
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef TOSTRINGSTREAM
#ifdef _UNICODE
#define TOSTRINGSTREAM std::wostringstream
#else
#define TOSTRINGSTREAM std::ostringstream
#endif // _UNICODE
#endif // TOSTRINGSTREAM

/**
 * @brief FieldIndex多重継承マクロ
 *
 * FieldIndexクラスを多重継承し、２個以上の属性フィールドインデックスを取得・格納するクラスの作成を簡略化するマクロ。<br>
 * @a tuple に列挙された @a n 個のFieldIndexを継承したクラス@a c を宣言・定義する。
 *
 * @code
 * template <typename TStream>
 * static void outputSubFields(TStream& oss);
 * @endcode
 * 属性フィールド名称をカンマ区切りで出力ストリーム @a oss に全て出力する。
 * 
 * @code
 * template <typename TQueryFilter>
 * static void putSubFields(TQueryFilter const& ipQueryFilter);
 * @endcode
 * このクラスが継承している全てのフィールド名を @a  ipQueryFilter にput_SubFields()する。<br>
 * メソッド内でstd::ostringstreamを使っているので、sstreamのインクルードが必要。
 * 
 * @code
 * static void addFieldsTo(IQueryFilter* ipQueryFilter);
 * @endcode
 * このクラスが継承している全てのフィールド名を @a ipQueryFilter にAddField()する。
 */
#define SINDY_FIELDINDEX_CLASS(c, n, tuple) SINDY_FIELDINDEX_CLASS_PD(c, n, _LIST_##n tuple)
#define SINDY_FIELDINDEX_CLASS_PD(c, n, list) SINDY_FIELDINDEX_CLASS_D(c, n, (_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDINDEX_FIELDINDEX, _COMMA, list)))
#define SINDY_FIELDINDEX_CLASS_D(c, n, tuple) SINDY_FIELDINDEX_CLASS_I(c, n, _LIST_##n tuple, _SINDY_FIELDINDEX_INHERIT, _SINDY_FIELDINDEX_CONSTRUCT)
#define SINDY_FIELDINDEX_CLASS_I(c, n, list, inheriter, constructor) \
class c : _SINDY_INHERITLIST_UNFOLD_##n(inheriter, _COMMA, list) \
{ \
public: \
	template<typename _1> \
	c(_1 const& _Arg1) : _SINDY_INHERITLIST_UNFOLD_##n(constructor, _COMMA, list) {} \
\
	static void addFieldsTo(IQueryFilter* ipQueryFilter) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDINDEX_ADDFIELD, _SEMICOLON, list); \
	} \
\
	template <typename TQueryFilter> \
	static void putSubFields(TQueryFilter const& ipQueryFilter) \
	{ \
		TOSTRINGSTREAM oss; \
		outputSubFields(oss); \
		ipQueryFilter->put_SubFields(CComBSTR(oss.str().c_str())); \
	} \
\
	template <typename TStream> \
	static void outputSubFields(TStream& oss) \
	{ \
		_SINDY_INHERITLIST_UNFOLD_##n(_SINDY_FIELDINDEX_OUTPUTSUBFIELDS,; oss << static_cast<const TCHAR>(_TCHARCOMMA);, list); \
	} \
\
	template <typename TInterface> \
	sindy::With<TInterface*, c> fusion(TInterface* pInterface) const\
	{ \
		return sindy::With<TInterface*, c>(pInterface, *this); \
	} \
\
	template <typename TIIID> \
	sindy::With<TIIID::Interface*, c> fusion(const _com_ptr_t<TIIID>& rInterface) const\
	{ \
		return sindy::With<TIIID::Interface*, c>(rInterface, *this); \
	} \
}

#define _SINDY_FIELDINDEX_FIELDINDEX(name) sindy::FieldIndex<name>
#define _SINDY_FIELDINDEX_INHERIT(base) public base
#define _SINDY_FIELDINDEX_CONSTRUCT(base) base(_Arg1)
#define _SINDY_FIELDINDEX_ADDFIELD(base) base::addFieldsTo(ipQueryFilter)
#define _SINDY_FIELDINDEX_OUTPUTSUBFIELDS(base) base::outputSubFields(oss)

//////////////////////////////////////////////////////////////////////////////////////////
//
// SINDY_FIELDINDEX_CHAIN_n
//
//////////////////////////////////////////////////////////////////////////////////////////

#define SINDY_FIELDINDEX_CHAIN_1(_1, _2) sindy::FieldIndexChain<_1, _2 >
#define SINDY_FIELDINDEX_CHAIN_2(_1, _2, _3) sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_1(_2, _3) >
#define SINDY_FIELDINDEX_CHAIN_3(_1, _2, _3, _4) sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_2(_2, _3, _4) >
#define SINDY_FIELDINDEX_CHAIN_4(_1, _2, _3, _4, _5) sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_3(_2, _3, _4, _5) >
#define SINDY_FIELDINDEX_CHAIN_5(_1, _2, _3, _4, _5, _6) \
	sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_4(_2, _3, _4, _5, _6) >
#define SINDY_FIELDINDEX_CHAIN_6(_1, _2, _3, _4, _5, _6, _7) \
	sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_5(_2, _3, _4, _5, _6, _7) >
#define SINDY_FIELDINDEX_CHAIN_7(_1, _2, _3, _4, _5, _6, _7, _8) \
	sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_6(_2, _3, _4, _5, _6, _7, _8) >
#define SINDY_FIELDINDEX_CHAIN_8(_1, _2, _3, _4, _5, _6, _7, _8, _9) \
	sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_7(_2, _3, _4, _5, _6, _7, _8, _9) >
#define SINDY_FIELDINDEX_CHAIN_9(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10) \
	sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_8(_2, _3, _4, _5, _6, _7, _8, _9, _10) >
#define SINDY_FIELDINDEX_CHAIN_10( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_9( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11) >
#define SINDY_FIELDINDEX_CHAIN_11( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_10( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12) >
#define SINDY_FIELDINDEX_CHAIN_12( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10,  \
	_11, _12, _13) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_11( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13) >
#define SINDY_FIELDINDEX_CHAIN_13( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_12( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14) >
#define SINDY_FIELDINDEX_CHAIN_14( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_13( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15) >
#define SINDY_FIELDINDEX_CHAIN_15( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_14( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16) >
#define SINDY_FIELDINDEX_CHAIN_16( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_15( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17) >
#define SINDY_FIELDINDEX_CHAIN_17( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_16( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18) >
#define SINDY_FIELDINDEX_CHAIN_18( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_17( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19) >
#define SINDY_FIELDINDEX_CHAIN_19( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_18( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20) >
#define SINDY_FIELDINDEX_CHAIN_20( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_19( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21) >
#define SINDY_FIELDINDEX_CHAIN_21( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_20( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22) >
#define SINDY_FIELDINDEX_CHAIN_22( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_21( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23) >
#define SINDY_FIELDINDEX_CHAIN_23( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_22( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24) >
#define SINDY_FIELDINDEX_CHAIN_24( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_23( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25) >
#define SINDY_FIELDINDEX_CHAIN_25( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_24( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26) >
#define SINDY_FIELDINDEX_CHAIN_26( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_25( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26, _27) >
#define SINDY_FIELDINDEX_CHAIN_27( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_26( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26, _27, _28) >
#define SINDY_FIELDINDEX_CHAIN_28( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28, _29) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_27( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26, _27, _28, _29) >
#define SINDY_FIELDINDEX_CHAIN_29( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28, _29, _30) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_28( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26, _27, _28, _29, _30) >
#define SINDY_FIELDINDEX_CHAIN_30( \
	_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, \
	_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
	_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
	_31) \
		sindy::FieldIndexChain<_1, SINDY_FIELDINDEX_CHAIN_29( \
		_2, _3, _4, _5, _6, _7, _8, _9, _10, \
		_11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
		_21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
		_31) >

#endif // __SINDY_FIELD_INDEX_MACRO_H__
