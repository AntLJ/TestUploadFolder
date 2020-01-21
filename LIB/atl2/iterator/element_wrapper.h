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

#ifndef ATL2_ELEMENT_INITIALIZER_H_
#define ATL2_ELEMENT_INITIALIZER_H_

#include <boost/type_traits.hpp>
#include <boost/get_pointer.hpp>

namespace atl2 {

namespace detail {

#if _MSC_VER <= 1200

/**
 * @brief IUnknown派生オブジェクトポインタのスマートポインタクラス。
 *
 * @note コンパイラが CComPtr<boost::remove_pointer<T>::type> を認めるならば、このクラスは不要である（つまりVC8では不要）。
 */
template <typename T>
struct element_wrapper_for_interface_pointer
{
public:
	typedef T InterfacePtr;

///@name コンストラクタとデストラクタ
//@{
	element_wrapper_for_interface_pointer() :
	p(0)
	{
	}

	element_wrapper_for_interface_pointer(const element_wrapper_for_interface_pointer& rhs) :
	p(rhs.p)
	{
		AddRef();
	}

	~element_wrapper_for_interface_pointer()
	{
		Release();
	}
//@}

///@name 演算
//@{
	T operator->() const
	{
		return p; 
	}

	operator T() const
	{
		return p; 
	}

	bool operator!() const
	{
		return ! p;
	}

	bool operator==(const T rhs) const
	{
		return p == rhs;
	}

	T* operator&()
	{
		Release();
		return &p;
	}

	element_wrapper_for_interface_pointer& operator=(T const rhs)
	{
		Release();
		p = rhs;
		AddRef();
		return *this;
	}

	element_wrapper_for_interface_pointer& operator=(element_wrapper_for_interface_pointer const& rhs)
	{
		Release();
		p = rhs.p;
		AddRef();
		return *this;
	}
//@}

private:
	void AddRef()
	{
		if(p) {
			p->AddRef();
		}
	}
	void Release()
	{
		if(p) {
			p->Release();
			p = 0;
		}
	}

// 変数
	T p;
};

#else

template <typename T>
struct element_wrapper_for_interface_of
{
	typedef _com_ptr_t<_com_IIID<T, &__uuidof(T)> > type;
};

template <typename T, int Category>
struct element_wrapper_for_category_of
{
	typedef T type;
};

template <typename T>
struct element_wrapper_for_category_of<T, 3>
{
	typedef typename element_wrapper_for_interface_of<typename boost::remove_pointer<T>::type>::type type;
};

#endif // _MSC_VER <= 1200

template <typename T>
struct element_wrapper_of
{
#if _MSC_VER <= 1200
	template <int TType> struct detail_t { typedef T type; };
	template <> struct detail_t<3> { typedef element_wrapper_for_interface_pointer<T> type; };

	typedef typename detail_t<atl2::type_of<T>::value>::type type;
#else
	typedef typename element_wrapper_for_category_of<T, atl2::type_of<T>::value>::type type;
#endif // _MAX_VER <= 1200
};

template <>
struct element_wrapper_of<BSTR>
{
	class type : public CComBSTR
	{
		BSTR* operator&()
		{
			Empty();
			return CComBSTR::operator&();
		}
	};
};

template <>
struct element_wrapper_of<VARIANT>
{
	class type : public _variant_t
	{
	public:
		VARIANT* operator&()
		{
			Clear();
			return this;
		}
	};
};

} // namespace detail

} // namespace atl2

#if _MSC_VER <= 1200

namespace boost { 

template <typename T> 
inline typename atl2::detail::element_wrapper_for_interface_pointer<T>::InterfacePtr
get_pointer(atl2::detail::element_wrapper_for_interface_pointer<T> const& p) 
{ 
	return p; 
} 

} // namespace boost

#endif // _MSC_VER <= 1200

#endif // ATL2_ELEMENT_INITIALIZER_H_
