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

#ifndef ARCTL_ADD_ITERATOR_H_
#define ARCTL_ADD_ITERATOR_H_

#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <atl2/type_traits.h>

#ifndef DOXYGEN 

#define ARCTL_ADDER_NAME(X) X
#define ARCTL_ADDER_CC

#include <arctl/iterator/iterator_cc.h>

#undef ARCTL_ADDER_NAME
#undef ARCTL_ADDER_CC

#define ARCTL_ADDER_NAME(X) X##_stdcall
#define ARCTL_ADDER_CC __stdcall

#include <arctl/iterator/iterator_cc.h>

#undef ARCTL_ADDER_NAME
#undef ARCTL_ADDER_CC

#else

#include <arctl/iterator/iterator_cc.h>

#endif // DOXYGEN

namespace arctl {

template <typename InterfacePtr>
typename boost::enable_if<
	boost::is_same<typename atl2::interface_pointer_of<InterfacePtr>::type, IQueryFilter*>,
	add_iterator1<IQueryFilter, _bstr_t>
>::type
adder(const InterfacePtr& ipInterface)
{
	return add_iterator1<IQueryFilter, _bstr_t>(ipInterface, &IQueryFilter::_AddField);
}

} // namespace arctl

#endif // ARCTL_ADD_ITERATOR_H_
