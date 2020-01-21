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

#ifndef __SINDY_DETAIL_REF_H__
#define __SINDY_DETAIL_REF_H__

namespace sindy {

/**
 * @brief リファレンスラッパークラス。
 */
template<class T>
class reference_wrapper
{ 
public:
// 定義
    typedef T type;

// 構築
    explicit reference_wrapper(T& t): m_T(&t) {}

// 取得
    T& get() const { return *m_T; }
    T* get_pointer() const { return m_T; }

// 演算子のオーバーライド
    operator T& () const { return *m_T; }

private:
// 変数
    T* m_T;
};

/**
 * @brief リファレンスラッパーのヘルパ関数
 */
template<class T> inline reference_wrapper<T> ref(T& t)
{ 
    return reference_wrapper<T>(t);
}

/**
 * @brief ポインタ識別クラス。
 *
 * @note 原典は http://www.tietew.jp/cppll/archive/6360 。
 */
template<typename T> struct is_pointer
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	struct helper { helper(const volatile void*); };
	inline static yestype tester(helper);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};
template <> struct is_pointer<void> { enum { value = 0 }; };

/**
 * @brief インタフェース識別クラス。
 */
template<typename T> struct is_interface
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
template<typename T> struct is_interface_pointer
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
 * @brief スマートポインタ識別クラス。
 */
template<typename T> struct is_com_ptr_t
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	template <typename TIIID> inline static yestype tester(const volatile _com_ptr_t<TIIID>&);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};
	
/**
 * @brief リファレンスラッパ識別クラス。
 */
template<typename T> struct is_reference_wrapper
{
	typedef char yestype;
	typedef struct { char dummy[2]; } notype;
	
	template <typename T> inline static yestype tester(const volatile reference_wrapper<T>&);
	inline static notype tester(...);
	
	inline static T& make_t();
	enum { value = sizeof(tester(make_t()))==sizeof(yestype) };
};

/**
 * @brief @a T が「ポインタ」「インタフェースポインタ」「スマートポインタ」「リファレンスラッパ」のいずれに属するかの判定を行うクラス
 *
 * - value
 *   - 0 : 以下のいずれでもない型。
 *   - 1 : ポインタ型。
 *   - 2 : インタフェース（IUnknon派生）ポインタ型。
 *   - 3 : スマートポインタ（_com_ptr_t）型。
 *   - 4 : リファレンスラッパ（sindy::ref）型。
 */
template <typename T>
struct type_of
{
	typedef char not_ptr;
	typedef struct { char dummy[2]; } ptr;
	typedef struct { char dummy[3]; } interface_ptr;
	typedef struct { char dummy[4]; } com_ptr;
	typedef struct { char dummy[5]; } sindy_ref;
	
	template <typename T> inline static sindy_ref tester(reference_wrapper<T>);
	template <typename TIIID> inline static com_ptr tester(_com_ptr_t<TIIID>);
	inline static interface_ptr tester(const volatile IUnknown*);
	inline static ptr tester(const volatile void*);
	inline static not_ptr tester(...);
	
	inline static T& make_t();

	enum {
		value = 
			sizeof(tester(make_t())) == sizeof(sindy_ref) ? 4 :
			sizeof(tester(make_t())) == sizeof(com_ptr) ? 3 :
			sizeof(tester(make_t())) == sizeof(interface_ptr) ? 2 :
			sizeof(tester(make_t())) == sizeof(ptr) ? 1 : 0
	};
};

namespace detail
{

template<bool wrapped> struct complement_interface_pointer { template <typename T> struct apply { typedef T type; }; };
template<> struct complement_interface_pointer<true> { template <typename T> struct apply { typedef T* type; }; };

template<bool wrapped> struct reference_unwrapper { template <typename T> struct apply { typedef T type; }; };
template<> struct reference_unwrapper<true> { template <typename T> struct apply { typedef typename T::type type; }; };

} // namespace detail

/**
 * @brief IUnknown→IUnknown*、IUnknown*→IUnknown*、other→other
 */
template <typename T>
struct interface_to_pointer : public detail::complement_interface_pointer<is_interface<T>::value>::apply<T> {};

/**
 * @brief ref<T>→T、other→other
 */
template <typename T>
struct unwrap_reference : public detail::reference_unwrapper<is_reference_wrapper<T>::value>::apply<T> {};

} // namespace sindy

#endif // __SINDY_DETAIL_REF_H__
