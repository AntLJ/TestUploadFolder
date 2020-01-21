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

#ifndef ARCTL_ZERO_BASED_RANDOM_ACCESS_ITERATORS_H_
#define ARCTL_ZERO_BASED_RANDOM_ACCESS_ITERATORS_H_

#include <atl2/iterator/iterator_base.h>
#include <atl2/iterator/zero_based_random_access_iterator.h>

#define ARCTL_ITERATOR_TRAITS(i, e, g, c)                                   \
	template <> struct container_pointer_traits<i*> :                       \
	public zero_based_random_access_iterator_traits<i, e, &i::g, &i::c> {};

namespace atl2 {

/**@ingroup container_traits_group*/
/*@{*/

ARCTL_ITERATOR_TRAITS(IArray, LPUNKNOWN, get_Element, get_Count);
ARCTL_ITERATOR_TRAITS(ILongArray, long, get_Element, get_Count);
ARCTL_ITERATOR_TRAITS(IFields, IField*, get_Field, get_FieldCount);
#if 0 // VC6.0でしか通らない、規格非準拠のへたれコード。
ARCTL_ITERATOR_TRAITS(IFieldsEdit, IField*, get_Field, get_FieldCount);
#endif
ARCTL_ITERATOR_TRAITS(IPointCollection, IPoint*, get_Point, get_PointCount);
ARCTL_ITERATOR_TRAITS(ISegmentCollection, _ISegment*, get_Segment, get_SegmentCount);
ARCTL_ITERATOR_TRAITS(IGeometryCollection, IGeometry*, get_Geometry, get_GeometryCount);

ARCTL_ITERATOR_TRAITS(IIndexes, IIndex*, get_Index, get_IndexCount);

#if !defined( ARCTL_NO_USE_CARTO )
ARCTL_ITERATOR_TRAITS(IFeatureCache, IFeature*, get_Feature, get_Count);
ARCTL_ITERATOR_TRAITS(IMaps, IMap*, get_Item, get_Count);
ARCTL_ITERATOR_TRAITS(ICompositeLayer, ILayer*, get_Layer, get_Count);
ARCTL_ITERATOR_TRAITS(IStandaloneTableCollection, IStandaloneTable*, get_StandaloneTable, get_StandaloneTableCount);
#endif // !defined( ARCTL_NO_USE_CARTO )

/*@}*/

} // namespace arctl

#undef ARCTL_ITERATOR_TRAITS

#endif // ARCTL_ZERO_BASED_RANDOM_ACCESS_ITERATORS_H_
