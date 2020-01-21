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

/**
 * @file atl2/io.h
 * @brief ATLオブジェクトと入出力ストリームの関係定義ファイル。
 */
#ifndef ATL2_IO_H_
#define ATL2_IO_H_

#include <iterator>
#include <iostream>
#include <string>
#include <boost/preprocessor.hpp>
#include <atl2/exception.h>

namespace atl2 {

namespace io {

/**
 * @brief ワイド文字列をマルチバイトストリームに流したり、その逆を実現する名前空間。
 *
 * @note
 * いずれは <b>rns::io::compatible</b> に取って代わられるかもしれない。
 *
 * @test IoCompatibleTest
 */
namespace compatible {

#pragma warning(push)
#if _MSC_VER >= 1400
#pragma warning(disable : 6255)
#endif // _MSC_VER >= 1400
/**
 * @brief ワイド文字列をマルチバイト文字列に変換してマルチバイトストリームに流す。
 *
 * @param os [in] マルチバイトストリーム。
 * @param rhs [in] ワイド文字列。
 * @return @p os
 *
 * @test IoCompatibleTest::testW2A()
 */
inline std::ostream& operator<<(std::ostream& os, LPCWSTR rhs)
{
	USES_CONVERSION;
	return os << W2CA(rhs);
}

/**
 * @brief マルチバイト文字列をワイド文字列に変換してワイドストリームに流す。
 *
 * @param os [in] ワイドストリーム。
 * @param rhs [in] マルチバイト文字列。
 * @return @p os
 *
 * @test IoCompatibleTest::testA2W()
 */
inline std::wostream& operator<<(std::wostream& os, LPCSTR rhs)
{
	USES_CONVERSION;
	return os << A2CW(rhs);
}
#pragma warning(pop)

} // namespace compatible

} // namespace io

} // namespace atl2

/**
 * @brief VARIANT型オブジェクトのストリーム出力定義。
 *
 * <b>VARIANT::vt</b> を確認し、幾つかの型に関しては然るべき値をストリームに渡す。<br>
 * それ以外は <b>VT_BSTR</b> へ <b>CComVariant::ChangeType()</b> し、それを出力する。<br>
 * どの型がどちらに当てはまるかはソースを参照。
 *
 * @param os [in] 出力ストリーム。
 * @param rhs [in] VARIANTオブジェクト。
 * @return @p os
 *
 * @bug MSVC6 has problems finding std::basic_ostream through the using declaration in namespace _STL.
 */
#if _MSC_VER <= 1200 && defined(__SGI_STL_PORT)
using std::basic_ostream;
template <typename Char, typename CharTraits>
inline basic_ostream<Char, CharTraits>& operator<<(basic_ostream<Char, CharTraits>& os, const VARIANT& rhs)
#else
template <typename Char, typename CharTraits>
inline std::basic_ostream<Char, CharTraits>& operator<<(std::basic_ostream<Char, CharTraits>& os, const VARIANT& rhs)
#endif
{
	USES_CONVERSION;

	using namespace atl2::io::compatible;

	switch(rhs.vt) {
	case VT_NULL:
		os << _T("<null>");
		break;
	case VT_EMPTY:
		os << _T("<empty>");
		break;
	case VT_UI1: // unsigned char
		os << rhs.bVal;
		break;
	case VT_I2: // short
		os << rhs.iVal;
		break;
	case VT_I4: // long
		os << rhs.lVal;
		break;
	case VT_R4: // float
		os << rhs.fltVal;
		break;
	case VT_R8: // double
		os << rhs.dblVal;
		break;
	case VT_BOOL: // bool(VARIANT_BOOL)
		os << (rhs.boolVal == VARIANT_TRUE);
		break;
	case VT_BSTR: // BSTR
		// OLE2CW() はWin32系ではノーコスト。
#pragma warning(push)
#if _MSC_VER >= 1400
#pragma warning(disable : 6255)
#endif // _MSC_VER >= 1400
		os << OLE2CW(rhs.bstrVal);
#pragma warning(pop)
		break;
	case VT_I1: // char
		os << rhs.cVal;
		break;
	case VT_UI2: // unsigned short
		os << rhs.uiVal;
		break;
	case VT_UI4: // unsigned long
		os << rhs.ulVal;
		break;
	case VT_INT: // int
		os << rhs.intVal;
		break;
	case VT_UINT: // unsigned int
		os << rhs.uintVal;
		break;
	default:
		{
			CComVariant vt;
			atl2::valid(vt.ChangeType(VT_BSTR, &rhs));
			os << vt;
		}
		break;
	}

	return os;
}

inline std::ostream& operator<<(std::ostream& os, const _bstr_t& rhs)
{
	return os << static_cast<const char*>(rhs);
}

inline std::wostream& operator<<(std::wostream& os, const _bstr_t& rhs)
{
	return os << static_cast<const wchar_t*>(rhs);
}

#pragma warning(push)
#if _MSC_VER >= 1400
#pragma warning(disable : 6255)
#endif // _MSC_VER >= 1400
inline std::ostream& operator<<(std::ostream& os, const CComBSTR& rhs)
{
	USES_CONVERSION;
	return os << OLE2CA(rhs);
}

inline std::wostream& operator<<(std::wostream& os, const CComBSTR& rhs)
{
	USES_CONVERSION;
	return os << OLE2CW(rhs);
}
#pragma warning(pop)

#if _MSC_VER >= 1400
template <typename T>
inline std::basic_istream<T>& operator>>(std::basic_istream<T>& is, _bstr_t& rhs)
{
	std::basic_string<T> str;
	is >> str;
	rhs = str.c_str();
	return is;
}

template <typename T>
inline std::basic_istream<T>& operator>>(std::basic_istream<T>& is, CComBSTR& rhs)
{
	std::basic_string<T> str;
	is >> str;
	rhs = str.c_str();
	return is;
}

/**
 * @brief _com_error型オブジェクトのストリーム出力定義。
 *
 * @param os [in] 出力ストリーム。
 * @param rhs [in] _com_errorオブジェクト。
 * @return @p os
 */
template <typename Char>
inline std::basic_ostream<Char>& operator<<(std::basic_ostream<Char>& os, const _com_error& rhs)
{
	const _bstr_t bstrDescription(rhs.Description());
	if(bstrDescription.length()) {
		return os << bstrDescription;
	}
	else {
		IErrorInfoPtr ipErrorInfo;
		if(::GetErrorInfo(0, &ipErrorInfo) == S_OK) {
			CComBSTR bstr;
			if(ipErrorInfo->GetDescription(&bstr) == S_OK && bstr.Length() > 0) {
				return os << bstr;
			}
		}

		using namespace atl2::io::compatible;

		return os << rhs.ErrorMessage();
	}
}

#define ATL2_ATLCONV_STREAM_OUTPUT(r, data, e)                                               \
	template <typename CharT, int BufferLengthT>                                             \
	inline std::basic_ostream<CharT>&                                                        \
	operator<<(std::basic_ostream<CharT>& os, const BOOST_PP_CAT(e, EX)<BufferLengthT>& rhs) \
	{                                                                                        \
		using namespace atl2::io::compatible;                                                \
		return os << static_cast<const data*>(rhs);                                          \
	};

BOOST_PP_SEQ_FOR_EACH(ATL2_ATLCONV_STREAM_OUTPUT, char, (CA2A)(CA2CA)(CW2A))
BOOST_PP_SEQ_FOR_EACH(ATL2_ATLCONV_STREAM_OUTPUT, wchar_t, (CW2W)(CW2CW)(CA2W))

#undef ATL2_ATLCONV_STREAM_OUTPUT

// 他人の名前空間に侵入するのは好みではないが、
// こうでもしないと上手く定義できない。
namespace std {

template <typename Traits, typename Allocator>
inline ostream& operator<<(ostream& os, const basic_string<wchar_t, Traits, Allocator>& rhs)
{
	return os << CW2A(rhs.c_str());
}

template <typename Traits, typename Allocator>
inline wostream& operator<<(wostream& os, const basic_string<char, Traits, Allocator>& rhs)
{
	using namespace atl2::io::compatible;
	return os << CA2W(rhs.c_str());
}

} // namespace std

#endif // _MSC_VER >= 1400

#endif ATL2_IO_H_
