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

#ifndef __SINDY_INHERITLIST_MACRO_H__
#define __SINDY_INHERITLIST_MACRO_H__

#define SINDY_MULTIPLE_INHERITANCE(c, n, tuple) SINDY_MULTIPLE_INHERITANCE_I(c, n, _LIST_##n tuple, _SINDY_INHERIT, _SINDY_CONSTRUCT_1)
#define SINDY_MULTIPLE_INHERITANCE_I(c, n, list, inheriter, constructor) \
class c : _SINDY_INHERITLIST_UNFOLD_##n(inheriter, _COMMA, list) \
{ \
public: \
	template<typename _1> \
	c(_1 const& _Arg1) : _SINDY_INHERITLIST_UNFOLD_##n(constructor, _COMMA, list) \
	{ \
	} \
};

#define _SINDY_INHERIT(c) public c
#define _SINDY_CONSTRUCT_1(c) c(_Arg1)

//////////////////////////////////////////////////////////////////////////////////////////
//
// •”•i
//
//////////////////////////////////////////////////////////////////////////////////////////

#define _COMMA ,
#define _SEMICOLON ;
#define _TCHARCOMMA _T(',')

#define _LIST_FIRST(first, rest) first
#define _LIST_REST(first, rest) rest

#define _LIST_1(_1) (_1)
#define _LIST_2(_1,_2) (_1,(_2))
#define _LIST_3(_1,_2,_3) (_1,_LIST_2(_2,_3))
#define _LIST_4(_1,_2,_3,_4) (_1,_LIST_3(_2,_3,_4))
#define _LIST_5(_1,_2,_3,_4,_5) (_1,_LIST_4(_2,_3,_4,_5))
#define _LIST_6(_1,_2,_3,_4,_5,_6) (_1,_LIST_5(_2,_3,_4,_5,_6))
#define _LIST_7(_1,_2,_3,_4,_5,_6,_7) (_1,_LIST_6(_2,_3,_4,_5,_6,_7))
#define _LIST_8(_1,_2,_3,_4,_5,_6,_7,_8) (_1,_LIST_7(_2,_3,_4,_5,_6,_7,_8))
#define _LIST_9(_1,_2,_3,_4,_5,_6,_7,_8,_9) (_1,_LIST_8(_2,_3,_4,_5,_6,_7,_8,_9))
#define _LIST_10(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) (_1,_LIST_9(_2,_3,_4,_5,_6,_7,_8,_9,_10))
#define _LIST_11(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) (_1,_LIST_10(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11))
#define _LIST_12(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) (_1,_LIST_11(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12))
#define _LIST_13(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) (_1,_LIST_12(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13))
#define _LIST_14(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) (_1,_LIST_13(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14))
#define _LIST_15(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) (_1,_LIST_14(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15))
#define _LIST_16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) (_1,_LIST_15(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16))
#define _LIST_17(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17) (_1,_LIST_16(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17))
#define _LIST_18(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18) (_1,_LIST_17(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18))
#define _LIST_19(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19) (_1,_LIST_18(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19))
#define _LIST_20(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20) \
 (_1,_LIST_19(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20))
#define _LIST_21(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21) \
 (_1,_LIST_20(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21))
#define _LIST_22(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22) \
 (_1,_LIST_21(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22))
#define _LIST_23(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23) \
 (_1,_LIST_22(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23))
#define _LIST_24(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24) \
 (_1,_LIST_23(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24))
#define _LIST_25(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25) \
 (_1,_LIST_24(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25))
#define _LIST_26(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26) \
 (_1,_LIST_25(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26))
#define _LIST_27(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27) \
 (_1,_LIST_26(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27))
#define _LIST_28(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28) \
 (_1,_LIST_27(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28))
#define _LIST_29(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29) \
 (_1,_LIST_28(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29))
#define _LIST_30(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30) \
 (_1,_LIST_29(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30))
#define _LIST_31(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31) \
 (_1,_LIST_30(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31))
#define _LIST_32(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32) \
 (_1,_LIST_31(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32))
#define _LIST_33(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33) \
 (_1,_LIST_32(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33))
#define _LIST_34(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34) \
 (_1,_LIST_33(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34))
#define _LIST_35(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35) \
 (_1,_LIST_34(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35))
#define _LIST_36(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36) \
 (_1,_LIST_35(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36))
#define _LIST_37(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37) \
 (_1,_LIST_36(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37))
#define _LIST_38(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38) \
 (_1,_LIST_37(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38))
#define _LIST_39(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39) \
 (_1,_LIST_38(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39))
#define _LIST_40(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40) \
 (_1,_LIST_39(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40))
#define _LIST_41(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41) \
 (_1,_LIST_40(_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41))

#define _SINDY_INHERITLIST_UNFOLD_1(macro, sep, arg) macro arg
#define _SINDY_INHERITLIST_UNFOLD_2(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_1(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_3(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_2(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_4(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_3(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_5(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_4(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_6(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_5(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_7(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_6(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_8(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_7(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_9(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_8(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_10(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_9(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_11(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_10(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_12(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_11(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_13(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_12(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_14(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_13(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_15(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_14(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_16(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_15(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_17(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_16(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_18(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_17(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_19(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_18(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_20(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_19(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_21(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_20(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_22(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_21(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_23(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_22(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_24(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_23(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_25(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_24(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_26(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_25(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_27(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_26(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_28(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_27(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_29(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_28(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_30(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_29(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_31(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_30(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_32(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_31(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_33(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_32(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_34(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_33(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_35(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_34(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_36(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_35(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_37(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_36(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_38(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_37(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_39(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_38(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_40(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_39(macro, sep, _LIST_REST list)
#define _SINDY_INHERITLIST_UNFOLD_41(macro, sep, list) macro(_LIST_FIRST list) sep _SINDY_INHERITLIST_UNFOLD_40(macro, sep, _LIST_REST list)

#endif // __SINDY_INHERITLIST_MACRO_H__
