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

#ifndef ATL2_TYPE_TRAITS_H_
#define ATL2_TYPE_TRAITS_H_

#include <atlbase.h>
#include <comdef.h>
#include <boost/type_traits.hpp>
#include <atl2/type_traits/is_com_ptr_t.h>

namespace atl2 {

/**
 * @brief インタフェース識別クラス。
 */
template<typename T>
struct is_interface
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile IUnknown&); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief インタフェースポインタ識別クラス。
 */
template<typename T>
struct is_interface_pointer
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile IUnknown*); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief @a T の型を判定するクラス。
 *
 * メンバ @a value に、テンプレート引数 @a T の型種を定義する。
 *
 * @par 型種:
 * - 1 : ポインタ型。
 * - 2 : インタフェース（IUnknown派生）型。
 * - 3 : インタフェース（IUnknown派生）ポインタ型。
 * - 4 : スマートポインタ（_com_ptr_t）型。
 * - 5 : スマートポインタ（CComPTR）型。
 * - 6 : _bstr_t 型。
 * - 7 : CComBSTR 型。
 * - 8 : VARIANT 型。
 * - 9 : _variant_t 型。
 * - 10 : CComVariant 型。
 * - 0 : 以上のいずれでもない型。
 *
 * @note
 * value が 4～10 を示す型を継承した型がどの値を示すかは未定義。
 *
 * @test type_test
 */
template <typename T>
struct type_of
{
	typedef char other;
	typedef struct { char dummy[2]; } ptr;
	typedef struct { char dummy[3]; } unknown;
	typedef struct { char dummy[4]; } interface_ptr;
	typedef struct { char dummy[5]; } com_ptr1;
	typedef struct { char dummy[6]; } com_ptr2;

	template <typename T2> inline static com_ptr2 tester(::ATL::CComPtr<T2>);
	template <typename TIIID> inline static com_ptr1 tester(_com_ptr_t<TIIID>);
	inline static unknown tester(const volatile IUnknown&);
	inline static other tester(...);
	
	inline static interface_ptr tester1(const volatile IUnknown*);
	inline static ptr tester1(const volatile void*);
	inline static other tester1(...);

	inline static T& make_t();

	enum {
		value =
			boost::is_pointer<T>::value ? sizeof(tester1(make_t())) - 1 :
			boost::is_same<T, _bstr_t>::value ? 6 :
			boost::is_same<T, CComBSTR>::value ? 7 :
			boost::is_same<T, VARIANT>::value ? 8 :
			boost::is_same<T, _variant_t>::value ? 9 :
			boost::is_same<T, CComVariant>::value ? 10 :
			sizeof(tester(make_t())) - 1
	};
};

/**
 * @brief インタフェースポインタ型同定クラス。
 */
template <typename T>
struct interface_pointer_of
{
	template <int TType> struct detail_t { template <typename T2> struct unfold { typedef T2 type; }; };
	template <> struct detail_t<2> { template <typename T2> struct unfold { typedef T2* type; }; };
	template <> struct detail_t<4> { template <typename T2> struct unfold { typedef typename T2::Interface* type; }; };
	template <> struct detail_t<5> { template <typename T2> struct unfold { typedef typename T2::_PtrClass* type; }; };

	typedef typename detail_t<type_of<T>::value>::template unfold<T>::type type;
};

/**
 * @brief スマートポインタをはがす。
 *
 * @par はがせるスマートポインタ:
 * - _com_ptr_
 * - _bstr_t
 * - _variant_t
 * - CComPtr
 * - CComBSTR
 * - CComVariant
 *
 * @note
 * スマートポインタクラスを継承したクラスであってもはがすことができるか否かは未定義。
 *
 * @test unwrap_smart_test
 */
template <typename T>
struct unwrap_smart
{
	template <int Type> struct detail_t { template <typename T2> struct unfold { typedef T2 type; }; };
	template <> struct detail_t<4> { template <typename T2> struct unfold { typedef typename T2::Interface* type; }; };
	template <> struct detail_t<5> { template <typename T2> struct unfold { typedef typename T2::_PtrClass* type; }; };
	template <> struct detail_t<6> { template <typename T2> struct unfold { typedef BSTR type; }; };
	template <> struct detail_t<7> { template <typename T2> struct unfold { typedef BSTR type; }; };
	template <> struct detail_t<9> { template <typename T2> struct unfold { typedef VARIANT type; }; };
	template <> struct detail_t<10> { template <typename T2> struct unfold { typedef VARIANT type; }; };

	typedef typename detail_t<type_of<T>::value>::template unfold<T>::type type;
};

} // namespace atl2

#endif // ATL2_TYPE_TRAITS_H_
