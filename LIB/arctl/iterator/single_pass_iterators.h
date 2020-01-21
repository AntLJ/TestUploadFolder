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

#ifndef ARCTL_SIGLE_PASS_ITERATORS_H_
#define ARCTL_SIGLE_PASS_ITERATORS_H_

#include <atl2/iterator/iterator_base.h>
#include <atl2/iterator/single_pass_iterator.h>

#define ARCTL_SPC_ITERATOR_TRAITS(i, e, n)               \
	template <> struct container_pointer_traits<i*> :    \
	public single_pass_container_traits<i, e, &i::n> {};

#define ARCTL_EC_ITERATOR_TRAITS(i, e)                \
	template <> struct container_pointer_traits<i*> : \
	public enumeration_container_traits<i, e> {};

namespace atl2 {

/**@ingroup container_traits_group*/
/*@{*/

ARCTL_SPC_ITERATOR_TRAITS(_ICursor, _IRow*, NextRow);
ARCTL_SPC_ITERATOR_TRAITS(IFeatureCursor, IFeature*, NextFeature);
ARCTL_EC_ITERATOR_TRAITS(IEnumDatasetName, IDatasetName*);

ARCTL_EC_ITERATOR_TRAITS(_ISet, IUnknown*);

#if !defined( ARCTL_NO_USE_CARTO )
ARCTL_EC_ITERATOR_TRAITS(IEnumLayer, ILayer*);
#endif // !defined( ARCTL_NO_USE_CARTO )

/*@}*/

} // namespace atl2

#undef ARCTL_SPC_ITERATOR_TRAITS
#undef ARCTL_EC_ITERATOR_TRAITS

#endif // ARCTL_SIGLE_PASS_ITERATORS_H_
