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

#ifndef ARCTL_DETAIL_MULTI_INHERIT_H_
#define ARCTL_DETAIL_MULTI_INHERIT_H_

namespace arctl {

namespace detail {

namespace multi_inherit {

struct base_t { enum { is_arctl_multi_inherit = false }; };

template <bool IsMultiInherit>
struct function_selector
{
	template <typename TMultiObject, typename Function>
	static void for_each_ref(const TMultiObject& rObject, Function& rFunction) { rObject.for_each_ref(rFunction); }
};

template <>
struct function_selector<false>
{
	template <typename TSingleObject, typename Function>
	static void for_each_ref(const TSingleObject& rObject, Function& rFunction) { rFunction(rObject); }
};

template <typename TObject, typename Function>
inline void for_each_ref(const TObject& rObject, Function& rFunction)
{
	function_selector<TObject::is_arctl_multi_inherit>::for_each_ref(rObject, rFunction);
}

} // namespace multi_inherit

} // namespace detail

} // namespace arctl

#endif // ARCTL_DETAIL_MULTI_INHERIT_H_
