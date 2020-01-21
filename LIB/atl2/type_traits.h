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
 * @brief �C���^�t�F�[�X���ʃN���X�B
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
 * @brief �C���^�t�F�[�X�|�C���^���ʃN���X�B
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
 * @brief @a T �̌^�𔻒肷��N���X�B
 *
 * �����o @a value �ɁA�e���v���[�g���� @a T �̌^����`����B
 *
 * @par �^��:
 * - 1 : �|�C���^�^�B
 * - 2 : �C���^�t�F�[�X�iIUnknown�h���j�^�B
 * - 3 : �C���^�t�F�[�X�iIUnknown�h���j�|�C���^�^�B
 * - 4 : �X�}�[�g�|�C���^�i_com_ptr_t�j�^�B
 * - 5 : �X�}�[�g�|�C���^�iCComPTR�j�^�B
 * - 6 : _bstr_t �^�B
 * - 7 : CComBSTR �^�B
 * - 8 : VARIANT �^�B
 * - 9 : _variant_t �^�B
 * - 10 : CComVariant �^�B
 * - 0 : �ȏ�̂�����ł��Ȃ��^�B
 *
 * @note
 * value �� 4�`10 �������^���p�������^���ǂ̒l���������͖���`�B
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
 * @brief �C���^�t�F�[�X�|�C���^�^����N���X�B
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
 * @brief �X�}�[�g�|�C���^���͂����B
 *
 * @par �͂�����X�}�[�g�|�C���^:
 * - _com_ptr_
 * - _bstr_t
 * - _variant_t
 * - CComPtr
 * - CComBSTR
 * - CComVariant
 *
 * @note
 * �X�}�[�g�|�C���^�N���X���p�������N���X�ł����Ă��͂������Ƃ��ł��邩�ۂ��͖���`�B
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
