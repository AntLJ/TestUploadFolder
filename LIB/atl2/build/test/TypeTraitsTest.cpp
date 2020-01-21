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

#include "stdafx.h"
#include <sstream>
#include <boost/static_assert.hpp>
#include <atl2/type_traits.h>

using namespace boost;
using namespace atl2;

namespace type_traits_test {

namespace type_of_test {

BOOST_STATIC_ASSERT(type_of<int>::value == 0);
BOOST_STATIC_ASSERT(type_of<short>::value == 0);
BOOST_STATIC_ASSERT(type_of<double>::value == 0);
BOOST_STATIC_ASSERT(type_of<int*>::value == 1);
BOOST_STATIC_ASSERT(type_of<IUnknown>::value == 2);
BOOST_STATIC_ASSERT(type_of<IUnknown*>::value == 3);
BOOST_STATIC_ASSERT(type_of<IUnknownPtr>::value == 4);
BOOST_STATIC_ASSERT(type_of<CComPtr<IUnknown> >::value == 5);
BOOST_STATIC_ASSERT(type_of<_bstr_t>::value == 6);
BOOST_STATIC_ASSERT(type_of<CComBSTR>::value == 7);
BOOST_STATIC_ASSERT(type_of<VARIANT>::value == 8);
BOOST_STATIC_ASSERT(type_of<_variant_t>::value == 9);
BOOST_STATIC_ASSERT(type_of<CComVariant>::value == 10);

class IUnknownPtr2 : public IUnknownPtr {};
class IUnknownPtr3 : public CComPtr<IUnknown> {};
class Variant2 : public _variant_t {};

BOOST_STATIC_ASSERT(type_of<IUnknownPtr2>::value == 4);
BOOST_STATIC_ASSERT(type_of<IUnknownPtr3>::value == 5);
BOOST_STATIC_ASSERT(type_of<Variant2>::value == 0);

} // namespace type_of_test

namespace unwrap_smart_test {

BOOST_STATIC_ASSERT((is_same<unwrap_smart<long>::type, long>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<double>::type, double>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<IUnknownPtr>::type, IUnknown*>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<CComPtr<IUnknown> >::type, IUnknown*>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<BSTR>::type, BSTR>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<_bstr_t>::type, BSTR>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<CComBSTR>::type, BSTR>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<VARIANT>::type, VARIANT>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<_variant_t>::type, VARIANT>::value));
BOOST_STATIC_ASSERT((is_same<unwrap_smart<CComVariant>::type, VARIANT>::value));

} // namespace unwrap_smart_test

} // namespace type_traits_test
