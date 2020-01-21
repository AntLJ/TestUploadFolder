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

#ifndef __SINDY_FUNCTIONAL_LESS_H__
#define __SINDY_FUNCTIONAL_LESS_H__

#include <functional>

namespace sindy {

/////////////////////////////////////////////////////////////////////////////
// íËã`
/////////////////////////////////////////////////////////////////////////////

/// @brief SiNDYî≈lessÅióvì¡éÍâªÅj
template <typename T>
struct less : public std::binary_function<T, T, bool>
{
private:
	less() {}
};

/// @brief CComBSTRå^lessì¡éÍâª
template <>
struct less<CComBSTR> : public std::binary_function<CComBSTR, CComBSTR, bool>
{
	bool operator()(const CComBSTR& lhs, const CComBSTR& rhs) const;
};

/// @brief _variant_tå^lessì¡éÍâª
template <>
struct less<_variant_t> : public std::binary_function<_variant_t, _variant_t, bool>
{
	bool operator()(const _variant_t& lhs, const _variant_t& rhs) const;
};

/////////////////////////////////////////////////////////////////////////////
// é¿ëï
/////////////////////////////////////////////////////////////////////////////

/**
 * @brief CComBSTRÇÃè¨Ç»ÇËî‰är
 */
inline bool less<CComBSTR>::operator()(const CComBSTR& lhs, const CComBSTR& rhs) const
{
	return ::wcscmp(lhs, rhs) < 0;
}

/**
 * @brief _variant_tÇÃè¨Ç»ÇËî‰är
 */
inline bool less<_variant_t>::operator()(const _variant_t& lhs, const _variant_t& rhs) const
{
	if(lhs.vt != rhs.vt) 
		return false;

	switch(lhs.vt) {
	case VT_I2:
		return lhs.iVal < rhs.iVal;
	case VT_I4:
		return lhs.lVal < rhs.lVal;
	case VT_R4:
		return lhs.fltVal < rhs.fltVal;
	case VT_R8:
		return lhs.dblVal < rhs.dblVal;
	case VT_BSTR:
		return _bstr_t(lhs.bstrVal) < _bstr_t(rhs.bstrVal);
	case VT_NULL:
		return true;
	default:
		{
			_variant_t va1;
			_variant_t va2;
			va1.ChangeType(VT_I4, &lhs);
			va2.ChangeType(VT_I4, &rhs);
			return (*this)(va1, va2);
		}
	}
}

} // namespace sindy

#endif // __SINDY_FUNCTIONAL_LESS_H__
